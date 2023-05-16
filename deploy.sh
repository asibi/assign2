#!/bin/bash

set -e

if [[ $# -ne 1 ]]; then
    echo "Missing argument: path to SSH key"
    exit 1  
fi

if [[ ! -f "$1" ]]; then
    echo "Cannot find SSH key '${1}'"
    exit 1
fi


files=(
    C/ Rust/ Java/ Go/ Workloads/ run.sh
)

target_folder="csp-assignment-2"

ssh -i "$1" -t group05@dasya1.itu.dk "rm -rf ${target_folder}; mkdir ${target_folder}"

scp -i "$1" -r "${files[@]}" group05@dasya1.itu.dk:~/"${target_folder}"/

ssh -i "$1" -t group05@dasya1.itu.dk "cd ${target_folder}; chmod +x C/build.sh; chmod +x Rust/build.sh; chmod +x Go/build.sh; chmod +x Java/build.sh; chmod +x C/run.sh; chmod +x Rust/run.sh; chmod +x Go/run.sh; chmod +x Java/run.sh; ./run.sh"

scp -i "$1" group05@dasya1.itu.dk:~/"${target_folder}"/result.csv result.csv