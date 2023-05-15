# Forkfuzz

## 0. Installation
```
sudo apt-get install binutils-dev libunwind-dev libblocksruntime-dev clang
make
```

## 1. Dining Philosophers Problem
This case study shows a plausible pattern where forked processes stay in a hung state indefinitely without the possibility of exiting.
To avoid a process staying in a hung state and occupying resources, most fuzzers incorporate a timeout feature, which terminates a process if its execution time exceeds a specified limit.
Forkfuzz succeeds in detecting the timeout in the deadlocked runs.

```
export FORKFUZZ_DIR=[path to forkfuzz]
cd $FORKFUZZ_DIR/forkfuzz_testcases/dining_philosophers
CC=$FORKFUZZ_DIR/hfuzz_cc/hfuzz-clang make
$FORKFUZZ_DIR/honggfuzz -i ./input/ -s -n 1 -N 200 -- ./dining_philosophers.o
```

Note that deadlocks keep shared memory open, if necessary use this script to clean up the shared memory.
```
for x in $(ipcs -m | awk '{print $2}'); do ipcrm -m $x; done
```

## 2. Producer Consumer Problem
```
export FORKFUZZ_DIR=./
cd $FORKFUZZ_DIR/forkfuzz_testcases/producer_consumer
CC=$FORKFUZZ_DIR/hfuzz_cc/hfuzz-clang make
$FORKFUZZ_DIR/honggfuzz -f ./input/ -n 1 -N 2 -Q -- ./producer_consumer.o ___FILE___
```

## 3. Web Server
```
export FORKFUZZ_DIR=./
cd $FORKFUZZ_DIR/forkfuzz_testcases/web_server
CC=$FORKFUZZ_DIR/hfuzz_cc/hfuzz-clang make
_HF_TCP_PORT=8080 $FORKFUZZ_DIR/honggfuzz -i ./corpus --dict $FORKFUZZ_DIR/examples/apache-httpd/httpd.wordlist --workspace ./crashes --output ./new_corpus -N 100 -- ./server
```
