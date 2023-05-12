#!/bin/bash

set -e

gcc Main.c -pthread -lm -o program

# Iterate over threads (t)
for t in 1 2 4 8 16 32
do
    ./program $t
done