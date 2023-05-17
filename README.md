# Mini Project 2 source code
### Comparing how different languages scale with threads

## Overview

 - `WorkloadGenerator` is the program that generate the workloads from a list of sizes and inversion ratios.
 - `Workloads/` contain the workloads that are used for benchmarking (generated by the `WorkloadGenerator`)
 - The mergesort implmentation for each language is in the folder with the given language (i.e. `C`, `Go`, `Java` and `Rust`). Each folder contains a `build.sh` and `run.sh` script that build and runs the code.
 - The scripts in the root folder are used for deploying the code to the server and using the language specific implementations to generate benchmarks.

## Running
**Local Linux machine:**
```bash
./run.sh
```

**Deploy and run on server:**
Local Linux machine:
```bash
./deploy.sh
```

**Generate workloads**
Compile `WorkloadGenerator/main.cpp` and run the output file.
The workload variations (size and inverstion ratio) can be changedin in `main.cpp`.
