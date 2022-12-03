#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Use.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Support/Format.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <cstdint>
#include <map>
#include <vector>
#include <unordered_map>

using namespace llvm;
namespace{

    struct hash_pair {
        size_t operator()(const std::pair<Value*, Type::TypeID>& p) const
        {
            auto hash1 = std::hash<Value*>{}(p.first);
            auto hash2 = std::hash<Type::TypeID>{}(p.second);
    
            if (hash1 != hash2) {
                return hash1 ^ hash2;             
            }
            
            return hash1;
        }
    };

    enum NodeType {
        INSTRUCTION,
        CONSTANT,
        OTHER
    };

    enum NodeFlag {
        NONE,
        MONOTONIC_CONSTANTS
    };

    enum MonotonicOp {
        ADD,
        MUL
    };

    struct MonotonicInfo {
        int64_t start;
        int64_t end;
        int64_t increment;
        MonotonicOp monotonic_op;
    };

    struct Node {
        std::vector<Value*> values;
        std::vector<Node> edges;
        bool is_match;
        NodeType type;
        NodeFlag flag;

        // extra info (may not always exist, check NodeFlag before accessing)
        MonotonicInfo monotonicInfo;
    };

	struct HW1: public FunctionPass {
        static char ID;
		HW1() : FunctionPass(ID) {
        }

        void getAnalysisUsage(AnalysisUsage &AU) const{
            AU.addRequired<BlockFrequencyInfoWrapperPass>(); // Analysis pass to load block execution count
            AU.addRequired<BranchProbabilityInfoWrapperPass>();  // Analysis pass to load branch probability
        }

        bool check_monotonic(std::vector<Value*> &group) {
            std::vector<int64_t> int_vals;
            for (Value *C: group) {
                ConstantInt *ci = (ConstantInt*) C;
                int_vals.push_back(ci->getLimitedValue());
            }
            int64_t diff = int_vals[1] - int_vals[0];
            for (int k = 1; k < group.size() - 1; ++k) {
                if (int_vals[k + 1] - int_vals[k] != diff) return false; 
            }
            return true;
        }

        bool check_monotonic_mul(std::vector<Value*> &group) {
            std::vector<int64_t> int_vals;
            for (Value *C: group) {
                ConstantInt *ci = (ConstantInt*) C;
                int_vals.push_back(ci->getLimitedValue());
            }

            if (int_vals[0] == 0) return false;  // exit, cannot start from 0
            if (int_vals[1] % int_vals[0] != 0) return false;
            int64_t diff = int_vals[1] / int_vals[0];
            if (diff == 1) return false;  // trivial case, not considered multiplication

            for (int k = 1; k < group.size() - 1; ++k) {
                if (int_vals[k] == 0) return false;  // exit, cannot start from 0
                if (int_vals[k + 1] % int_vals[k] != 0) return false;
                if (int_vals[k + 1] / int_vals[k] != diff) return false; 
            }
            return true;
        }

        bool check_equivalence(std::vector<Value*> &group) {
            if (group.empty()) return false;
            if (group.size() < 2) return true;

            bool is_instruction = true;
            bool is_constant = true;

            for (auto& val: group) {
                if (is_instruction && isa<Instruction>(val)) {
                    is_instruction = true;
                    is_constant = false;
                } else if (is_constant && isa<Constant>(val)) {
                    is_instruction = false;
                    is_constant = true;
                } else {
                    is_instruction = false;
                    is_constant = false;
                }
            }

            if (is_instruction) {
                Instruction* I0 = (Instruction*) group[0];

                auto opcode = I0->getOpcode();
                auto operator_type = I0->getType()->getTypeID();
                std::vector<Type::TypeID> operand_types(I0->getNumOperands());
                for (int i = 0; i < I0->getNumOperands(); ++i) operand_types[i] = I0->getOperand(i)->getType()->getTypeID();

                for (int i = 1; i < group.size(); ++i) {
                    Instruction *I = (Instruction*) group[i];
                    if (I->getOpcode() != opcode) return false;
                    if (I->getType()->getTypeID() != operator_type) return false;
                    for (int j = 0; j < I0->getNumOperands(); ++j) {
                        if (I->getOperand(j)->getType()->getTypeID() != operand_types[j]) return false;
                    }
                }
                return true;
            } else if (is_constant) {
                if (isa<ConstantInt>(group[0])) {
                    if (check_monotonic(group)) return true;
                    if (check_monotonic_mul(group)) return true;
                }

                Value* val = group[0];
                for (Value* C: group) {
                    if (C != val) return false;
                }

                return true;
                // if (isa<ConstantInt>(group[0])) {
                //     APInt int_val = (static_cast<ConstantInt *>(group[0]))->getValue();
                //     for (auto C: group) {
                //         ConstantInt* ci = static_cast<ConstantInt *>(C);
                //         if (ci->getValue() != int_val) return false;
                //     }
                //     return true;
                // } else if (isa<Function>(group[0])) {
                // }
                // return false;  // TODO
            } else {
                // errs() << *group[0] << "\n";
                return false;
            }
        }

        Node create_alignment_graph(std::vector<Value*> &group) {
            Node n = Node();
            n.values = group;
            n.is_match = true;
            n.flag = NodeFlag::NONE;

            if (isa<Instruction>(group[0])) {
                n.type = NodeType::INSTRUCTION;

                // errs () << *((Instruction*) group[0]) << "\n";
                for (int i = 0; i < ((Instruction*) group[0])->getNumOperands(); ++i) {
                    std::vector<Value*> operandGroup;
                    for (auto& I: group) {
                        // errs() << ((Instruction*) I)->getNumOperands() << "\n";
                        auto use = ((Instruction*) I)->getOperandUse(i).get();
                        operandGroup.push_back(use);
                    }
                    if (check_equivalence(operandGroup)) {
                        Node e = create_alignment_graph(operandGroup);
                        n.edges.push_back(e);
                    } else {
                        Node e = Node();
                        e.is_match = false;
                        n.edges.push_back(e);
                    }
                }
            } else if (isa<Constant>(group[0])) {
                n.type = NodeType::CONSTANT;
            } else {
                n.type = NodeType::OTHER;
            }

            return n;
        }

        Node insert_monotonic_info(Node n) {
            if (n.type == NodeType::CONSTANT && isa<ConstantInt>(n.values[0])) {
                if (n.values.size() >= 2) {
                    if (check_monotonic(n.values)) {
                        n.flag = NodeFlag::MONOTONIC_CONSTANTS;
                        std::vector<int64_t> int_vals;
                        for (Value *C: n.values) {
                            ConstantInt *ci = (ConstantInt*) C;
                            int_vals.push_back(ci->getLimitedValue());
                        }
                        int64_t diff = int_vals[1] - int_vals[0];
                        n.monotonicInfo = {int_vals[0], int_vals[int_vals.size() - 1], diff, MonotonicOp::ADD};
                    } else if (check_monotonic_mul(n.values)) {
                        n.flag = NodeFlag::MONOTONIC_CONSTANTS;
                        std::vector<int64_t> int_vals;
                        for (Value *C: n.values) {
                            ConstantInt *ci = (ConstantInt*) C;
                            int_vals.push_back(ci->getLimitedValue());
                        }
                        int64_t diff = int_vals[1] / int_vals[0];
                        n.monotonicInfo = {int_vals[0], int_vals[int_vals.size() - 1], diff, MonotonicOp::MUL};
                    }
                }
            }

            for (int i = 0; i < n.edges.size(); ++i) {
                n.edges[i] = insert_monotonic_info(n.edges[i]);
            }

            return n;
        }

        void print_graph(Node n, int level) {
            if (!n.is_match) {
                 errs() << "level: " << level << ", mismatch\n";
            } else {
                errs() << "START GROUP\n";
                if (n.flag == NodeFlag::MONOTONIC_CONSTANTS) {
                    if (n.monotonicInfo.monotonic_op == MonotonicOp::ADD) errs () << "[ADD SEQUENCE] ";
                    if (n.monotonicInfo.monotonic_op == MonotonicOp::MUL) errs () << "[MUL SEQUENCE] ";
                    errs() << "start: " << n.monotonicInfo.start << " end: " << n.monotonicInfo.end << " increment: " << n.monotonicInfo.increment << "\n";
                }

                for (auto &val: n.values) {
                    errs() << "level: " << level << ", match" << ", val:" << *val << "\n";
                }
                errs() << "END GROUP\n\n";
                for (auto edge: n.edges) {
                    print_graph(edge, level + 1);
                }
            }
        }

        bool canRoll(Node &node) {
            if (node.flag == NodeFlag::MONOTONIC_CONSTANTS && node.monotonicInfo.start != node.monotonicInfo.end) {
                return true;
            }
            for (auto edge: node.edges) {
                if (canRoll(edge)) {
                    return true;
                }
            }
            return false;
        }

        void dfsGraph(Node &curNode, int level, std::vector<int>& maxDepth,  std::vector<Value*> &values, std::vector<int64_t>& mono, std::vector<MonotonicOp>& monoOp) {
            if (level > maxDepth[0] && maxDepth[0] >= 0) {
                maxDepth[0] = level;
            } 

            if (curNode.flag == NodeFlag::MONOTONIC_CONSTANTS && curNode.monotonicInfo.start != curNode.monotonicInfo.end) {
                mono.push_back(curNode.monotonicInfo.start);
                mono.push_back(curNode.monotonicInfo.end);
                mono.push_back(curNode.monotonicInfo.increment);
                monoOp.push_back(curNode.monotonicInfo.monotonic_op);
            }

            

            for (auto node : curNode.edges) {
                dfsGraph(node, level + 1, maxDepth, values, mono, monoOp);
            }

            if (maxDepth[0] >= 0 && level == maxDepth[0] -1) {
                values.push_back(curNode.values[0]);
                maxDepth[0] = -1;
            }

            if (level == 2) {
                values.push_back(curNode.values[0]);
            }
            
        }

        void eraseDfs(int level, Node& node) {

            for (auto val : node.values) {
                dyn_cast<Instruction>(val)->eraseFromParent();
            }

            if (level != 2) {
                eraseDfs(level + 1, node.edges[0]);
            }
        }
        
        void erasePrevInstructions(Node &graph) {
            eraseDfs(0, graph);
        }

        void generateLoop(Function &F, Node &graph) {

            LLVMContext* context = &F.getContext();
            IRBuilder<> builder(*context);
            Instruction* firstInstr = dyn_cast<Instruction>(graph.values[0]);
            BasicBlock* first = firstInstr->getParent();
            std::vector<int64_t> mono;
            std::vector<Value*> values; // [basePtr, ]
            std::vector<int> maxDepth = {0};
            std::vector<MonotonicOp> monoOp;

            dfsGraph(graph, 0, maxDepth, values, mono, monoOp);

            BasicBlock* preHeader = SplitBlock(firstInstr->getParent(), firstInstr);
            BasicBlock* loopBody = SplitBlock(firstInstr->getParent(), firstInstr);
            BasicBlock* endLoop = SplitBlock(firstInstr->getParent(), firstInstr);
            BasicBlock* last = SplitBlock(firstInstr->getParent(), firstInstr);

            builder.SetInsertPoint(preHeader->getTerminator());
            Value* stepVal = ConstantInt::get(*context, APInt(64, mono[0]));
            AllocaInst* stepAlloca = builder.CreateAlloca(stepVal->getType(), stepVal);
            LoadInst* stepVar = builder.CreateLoad(Type::getInt64Ty(*context), stepAlloca, "stepVar");

            builder.SetInsertPoint(loopBody->getTerminator());
            PHINode* phi = builder.CreatePHI(stepVar->getType(), 2, "i");
            phi->addIncoming(stepVar, preHeader);
            GetElementPtrInst* elemPtr = dyn_cast<GetElementPtrInst>(dyn_cast<Instruction>(values[1])->clone());
            elemPtr->setOperand(2, phi);
            elemPtr->insertBefore(loopBody->getTerminator());
            LoadInst* elemVal = builder.CreateLoad(Type::getInt32Ty(*context), elemPtr, "elemVal");
            CallInst* call = dyn_cast<CallInst>(firstInstr->clone());
            call->setOperand(0, elemVal);
            call->insertBefore(loopBody->getTerminator());

            builder.SetInsertPoint(endLoop->getTerminator());
            Value* incr;
            if (monoOp[0] == MonotonicOp::ADD) {
                incr = builder.CreateAdd(phi, ConstantInt::get(*context, APInt(64, mono[2])));
            } else {
                incr = builder.CreateMul(phi, ConstantInt::get(*context, APInt(64, mono[2])));
            }
            Value* cond;
            if (mono[2] > 0) {
                cond = builder.CreateICmpSLE(incr, ConstantInt::get(*context, APInt(64, mono[1])));
            } else {
                cond = builder.CreateICmpSGE(incr, ConstantInt::get(*context, APInt(64, mono[1])));
            }
            builder.CreateCondBr(cond, loopBody, last);
            endLoop->getTerminator()->eraseFromParent();
            phi->addIncoming(incr, endLoop);
            
            erasePrevInstructions(graph);
        }

		virtual bool runOnFunction(Function &F) override{
            /* *******Implementation of Your code ******* */
            std::vector<Node> graphs;

            for (auto bb = F.getBasicBlockList().begin(); bb != F.getBasicBlockList().end(); ++bb) {
                std::unordered_map<std::pair<Value*, Type::TypeID>, std::vector<Value*>, hash_pair> storeMap;
                std::unordered_map<Value*, std::vector<Value*>> functionMap;
                for (auto L = bb->begin(); L != bb->end(); ++L) {
                    const int opCode = L->getOpcode();
                    if (opCode == Instruction::Store) {
                        storeMap[{ L->getOperand(0), L->getType()->getTypeID()}].push_back(&(*L));
                    } else if (opCode == Instruction::Call) {
                        functionMap[L->getOperand(1)].push_back(&(*L));
                    }
                }
                // errs()<<"store size: " << storeMap.size() << "\n";
                // errs()<<"func size: " << storeMap.size() << "\n";

                

                for (auto item: storeMap) {
                    graphs.push_back(create_alignment_graph(item.second));
                }

                for (auto item: functionMap) {
                    graphs.push_back(create_alignment_graph(item.second));
                }

                for (int j = 0; j < graphs.size(); ++j) {
                    graphs[j] = insert_monotonic_info(graphs[j]);
                }

                // for (auto g: graphs) {
                //     print_graph(g, 0);
                // }
                
            }

            errs() << "Original Code" << '\n';
            for (auto bb = F.getBasicBlockList().begin(); bb != F.getBasicBlockList().end(); ++bb) {
                for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
                    errs() << *i << '\n';
                }
                errs() << ' ' << '\n';
            }

            errs() << "\n\n\n";

            for (Node graph: graphs) {
                //print_graph(graph, 0);

                if (canRoll(graph)) {
                    generateLoop(F, graph);
                }

            }

            errs() << "Altered Code" << '\n';
            for (auto bb = F.getBasicBlockList().begin(); bb != F.getBasicBlockList().end(); ++bb) {
                for (BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i) {
                    errs() << *i << '\n';
                }
                errs() << ' ' << '\n';
            }

			return false; // template code is just return false
		}
	};
}
char HW1::ID = 0;
static RegisterPass<HW1> X("hw1", "HW1 pass",
    false /* Only looks at CFG */,
    false /* Analysis Pass */);
