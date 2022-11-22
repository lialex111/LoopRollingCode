//===-- Frequent Path Loop Invariant Code Motion Pass ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// EECS583 F22 - This pass can be used as a template for your Frequent Path LICM
//               homework assignment. The pass gets registered as "fplicm".
//
// This pass performs loop invariant code motion, attempting to remove as much
// code from the body of a loop as possible.  It does this by either hoisting
// code into the preheader block, or by sinking code to the exit blocks if it is
// safe. 
//
////===----------------------------------------------------------------------===//
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>

/* *******Implementation Starts Here******* */
// include necessary header files
/* *******Implementation Ends Here******* */

using namespace llvm;

#define DEBUG_TYPE "fplicm"


namespace Correctness{
struct FPLICMPass : public LoopPass {
  static char ID;
  FPLICMPass() : LoopPass(ID) {}

  bool runOnLoop(Loop *L, LPPassManager &LPM) override {
    bool Changed = false;

    /* *******Implementation Starts Here******* */
    
    BranchProbabilityInfo &bpi = getAnalysis<BranchProbabilityInfoWrapperPass>().getBPI();
    std::vector<BasicBlock*> freqBasicBlocks;
    std::unordered_set<BasicBlock*> visitedFreqBasicBlocks;
    std::unordered_map<Value*, std::vector<LoadInst*>> hoistInstructions;
    std::unordered_map<Value*, std::unordered_set<StoreInst*>> storeDependencies;

    BasicBlock* preheader = L->getLoopPreheader();
    BasicBlock* bb = L->getHeader();
    while (freqBasicBlocks.empty() || visitedFreqBasicBlocks.find(bb) == visitedFreqBasicBlocks.end()) {
      
      freqBasicBlocks.push_back(bb);
      visitedFreqBasicBlocks.insert(preheader);
      visitedFreqBasicBlocks.insert(bb);

      for (BasicBlock *succ : successors(&*bb)) {
        if (bpi.getEdgeProbability(&*bb, succ) >= BranchProbability(8, 10)) {
          bb = succ;
        }
      }
    }

    for (BasicBlock *freqBb : freqBasicBlocks) {
      for (BasicBlock::iterator i = freqBb->begin(), e = freqBb->end(); i != e; ++i) {
        if (isa<LoadInst>(i)) {
          bool infreqDependency = false;
          bool freqDependency = false;
          std::vector<Instruction*> tempStoreDependencies;
          for(auto user : i->getOperand(0)->users()) {
            if (auto instr = dyn_cast<Instruction>(user)) {
              if (isa<StoreInst>(instr)) {
                if (visitedFreqBasicBlocks.find(instr->getParent()) == visitedFreqBasicBlocks.end()) { 
                  // this store instr is on an infreq. path using a load operand
                  infreqDependency = true;
                  tempStoreDependencies.push_back(instr);
                } else if (instr->getParent() != preheader) {
                  freqDependency = true;
                }
              } 
            }
          }
          if (!freqDependency && infreqDependency) {
            Changed = true;
            hoistInstructions[i->getOperand(0)].push_back((LoadInst*)&*i);
            for (auto instr : tempStoreDependencies) {
              storeDependencies[i->getOperand(0)].insert((StoreInst*)instr);
            }
          }
        }
      }
    }

    for (auto op : hoistInstructions) {
      LoadInst* loadInst = op.second[0];
      AllocaInst* newVar = new AllocaInst(loadInst->getType(), 0, nullptr, loadInst->getAlign(), "", preheader->getTerminator());
      Instruction* loadInstClone = loadInst->clone();
      StoreInst* newStore = new StoreInst(loadInstClone, newVar, preheader->getTerminator());
      loadInstClone->insertBefore(newStore);

      for (auto oldLoadInst : op.second) {
        oldLoadInst->setOperand(0, newVar);
      }
      for (StoreInst* storeInst : storeDependencies[op.first]) {
        storeInst->setOperand(1, newVar);
      }
    }
    
    /* *******Implementation Ends Here******* */
    return Changed;
  }


  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<BranchProbabilityInfoWrapperPass>();
    AU.addRequired<BlockFrequencyInfoWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
  }

private:
  /// Little predicate that returns true if the specified basic block is in
  /// a subloop of the current one, not the current one itself.
  bool inSubLoop(BasicBlock *BB, Loop *CurLoop, LoopInfo *LI) {
    assert(CurLoop->contains(BB) && "Only valid if BB is IN the loop");
    return LI->getLoopFor(BB) != CurLoop;
  }

};
} // end of namespace Correctness

char Correctness::FPLICMPass::ID = 0;
static RegisterPass<Correctness::FPLICMPass> X("fplicm-correctness", "Frequent Loop Invariant Code Motion for correctness test", false, false);


namespace Performance{
struct FPLICMPass : public LoopPass {
  static char ID;
  FPLICMPass() : LoopPass(ID) {}

  bool runOnLoop(Loop *L, LPPassManager &LPM) override {
    bool Changed = false;

    /* *******Implementation Starts Here******* */
    

    /* *******Implementation Ends Here******* */
    
    return Changed;
  }


  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<BranchProbabilityInfoWrapperPass>();
    AU.addRequired<BlockFrequencyInfoWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
  }

private:
  /// Little predicate that returns true if the specified basic block is in
  /// a subloop of the current one, not the current one itself.
  bool inSubLoop(BasicBlock *BB, Loop *CurLoop, LoopInfo *LI) {
    assert(CurLoop->contains(BB) && "Only valid if BB is IN the loop");
    return LI->getLoopFor(BB) != CurLoop;
  }

}; 
} // end of namespace Performance

char Performance::FPLICMPass::ID = 0;
static RegisterPass<Performance::FPLICMPass> Y("fplicm-performance", "Frequent Loop Invariant Code Motion for performance test", false, false);
