#!/bin/bash
### run.sh
### benchmark runner script
### Locate this script at each benchmark directory. e.g, 583simple/run.sh
### usage: ./run.sh ${benchmark_name} ${input} 
### e.g., ./run.sh compress compress.in or ./run.sh simple or ./run.sh wc cccp.c
### Note: Do NOT inlude inputs/ in ${input}, `./run.sh compress inputs/compress.in` will provide different results

PATH_MYPASS=~/LoopRollingCode/build/HW1_template/hw1pass/LLVMHW1.so ### Action Required: Specify the path to your pass ###
NAME_MYPASS=-hw1 ### Action Required: Specify the name for your pass ###
BENCH=src/${1}.c
INPUT=${2}

setup(){
if [[ ! -z "${INPUT}" ]]; then
echo "INPUT:${INPUT}"
ln -sf input1/${INPUT} .
fi
}

cleanup(){
rm *.profdata
rm *.bc
rm *.prof
rm *.profraw
if [[ ! -z "${INPUT}" ]]; then
rm *.in
rm *.in.Z
fi
}

# clean up previous runs
cleanup
# Prepare input to run
setup
# Convert source code to bitcode (IR)
# This approach has an issue with -O2, so we are going to stick with default optimization level (-O0)
clang -emit-llvm -c ${BENCH} -o ${1}.bc 
# Instrument profiler
opt -enable-new-pm=0 -pgo-instr-gen -instrprof ${1}.bc -o ${1}.prof.bc
# Generate binary executable with profiler embedded
clang -fprofile-instr-generate ${1}.prof.bc -o ${1}.prof
# Collect profiling data
./${1}.prof ${INPUT}
# Translate raw profiling data into LLVM data format
llvm-profdata merge -output=pgo.profdata default.profraw

# Prepare input to run
setup
# Apply your pass to bitcode (IR)
# opt -enable-new-pm=0 -pgo-instr-use -pgo-test-profile-file=pgo.profdata -load ${PATH_MYPASS} ${NAME_MYPASS} < ${1}.bc > /dev/null
opt -enable-new-pm=0 -pgo-instr-use -pgo-test-profile-file=pgo.profdata -load ${PATH_MYPASS} ${NAME_MYPASS} < ${1}.bc > /dev/null
