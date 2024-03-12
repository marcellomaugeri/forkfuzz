# Forkfuzz
Official repository for the paper <i>Forkfuzz: Leveraging the Fork-Awareness in Coverage-Guided Fuzzing</i>.
Please cite the paper using the provided bib entry if you reference or utilise this work.
```
@InProceedings{10.1007/978-3-031-54129-2_17,
    author="Maugeri, Marcello
    and Daniele, Cristian
    and Bella, Giampaolo",
    editor="Katsikas, Sokratis
    and Abie, Habtamu
    and Ranise, Silvio
    and Verderame, Luca
    and Cambiaso, Enrico
    and Ugarelli, Rita
    and Pra{\c{c}}a, Isabel
    and Li, Wenjuan
    and Meng, Weizhi
    and Furnell, Steven
    and Katt, Basel
    and Pirbhulal, Sandeep
    and Shukla, Ankur
    and Ianni, Michele
    and Dalla Preda, Mila
    and Choo, Kim-Kwang Raymond
    and Pupo Correia, Miguel
    and Abhishta, Abhishta
    and Sileno, Giovanni
    and Alishahi, Mina
    and Kalutarage, Harsha
    and Yanai, Naoto",
    title="Forkfuzz: Leveraging the Fork-Awareness in Coverage-Guided Fuzzing",
    booktitle="Computer Security. ESORICS 2023 International Workshops",
    year="2024",
    publisher="Springer Nature Switzerland",
    address="Cham",
    pages="291--308",
    abstract="Fuzzing is a widely adopted technique for automated vulnerability testing due to its effectiveness and applicability throughout the Software Development Life Cycle. Nevertheless, applying fuzzing ``out of the box'' to any system can prove to be a challenging endeavour. Consequently, the demand for target-specific solutions necessitates a substantial amount of manual intervention, which diverges from the automated nature typically associated with fuzzing. For example, prior research identified the lack of a solution for testing multi-process systems effectively. The problem is that coverage-guided fuzzers do not consider the possibility of having a system with more than one process. In this paper, we present Forkfuzz, a ``fork-aware'' fuzzer able to deal with multi-process systems. To the best of our knowledge, Forkfuzz is the first fork-aware fuzzer. It is built on top of Honggfuzz, one of the most popular and effective coverage-guided fuzzers, as reported by the Fuzzbench benchmark. To show its effectiveness, we tested our fuzzer over two classical programming problems: the Dining Philosophers Problem and a version of the Producer-Consumer Problem where the consumer (the child) process crashes for specific inputs. Furthermore, we evaluated Forkfuzz against a real and more complex scenario involving an HTTP server that handles multiple connections through multiple processes. The results of our evaluation demonstrate the effectiveness of Forkfuzz in identifying crashes and timeouts. Finally, we discuss possible improvements and challenges for the development and application of fork-aware fuzzing techniques.",
    isbn="978-3-031-54129-2"
}
```

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
