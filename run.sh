#!/bin/bash
set -e

projects=(Java)
thread_levels=(1 2 4 8 16 32)
workload_sizes=(4194304) #(64 1024 16384 262144 4194304)
workload_inversion_ratios=(0 25 50 75 100)
runs=1 #6
perf_stats=(
    instructions 
    cycles 
    context-switches
    cache-misses 
    L1-dcache-load-misses
    stalled-cycles-frontend 
    stalled-cycles-backend
)

workloads_dir="$(realpath Workloads)"
result_file="$(realpath result.csv)"
temp_perf_output="$(realpath temp-perf-output.txt)"

perf_path="perf"
if [[ "$#" -eq 1 ]]; then
    perf_path="$1"
    echo "Using custom perf_path: ${perf_path}"
fi

# Checking perf exists
"$perf_path" --version >/dev/null

# Setup Perf stats string
perf_stats_string="${perf_stats[0]}"
for perf_stat in "${perf_stats[@]:1}"; do
    perf_stats_string="${perf_stats_string},${perf_stat}"
done

#Append headers
result_header="project, size, inversion-ratio, thr  eads, run, time"
for perf_stat in "${perf_stats[@]}"; do
    result_header="${result_header}, ${perf_stat}"
done

echo
# Building projects
for project in "${projects[@]}"; do
    echo "Building ${project}"
    cd "${project}"
    ./build.sh
    cd ..
done

echo
echo "Running tests"

echo "${result_header}" > "${result_file}"

# Running projects
for project in "${projects[@]}"; do
    cd "${project}"
    for size in "${workload_sizes[@]}"; do
        for inversion_ratio in "${workload_inversion_ratios[@]}"; do
            workload="${workloads_dir}/${size}-${inversion_ratio}.txt"
            for threads in "${thread_levels[@]}"; do
                for run in $(seq 1 $runs); do
                    printf "${project}, n = ${size}, r = ${inversion_ratio}, t = ${threads}, i = ${run} ..  "  
                    time_ms=$("${perf_path}" stat -e ${perf_stats_string} -o "${temp_perf_output}" ./run.sh "${workload}" ${threads})
                    echo "${time_ms} ms"

                    result="${project}, ${size}, ${inversion_ratio}, ${threads}, ${run}, ${time_ms}"
                    for perf_stat in "${perf_stats[@]}"; do
                        stat_line=$(grep "  ${perf_stat}" "${temp_perf_output}")
                        stat_value=$(echo "$stat_line" | sed -E 's/[ \t]+(<not supported>|<not counted>|[0-9,.]+)[ \t]+[a-zA-Z0-9\-]+.*/\1/g')
                        stat_value=$(echo "$stat_value" | sed -E 's/,//g') # Remove thousand separator
                        result="${result}, ${stat_value}"
                    done

                    echo "  ${result}"
                    echo "${result}" >> "${result_file}"
                done
            done
        done
    done
    cd ..
done