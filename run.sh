#!/bin/bash
set -e

# Settings
projects=(C Java Rust Go)
thread_levels=(1 2 4 8 16 32)
workload_sizes=(64 1024 16384 262144 4194304)
workload_inversion_ratios=(0 25 50 75 100)
runs=8
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

# WSL cannot use perf command so, we allow for 
# setting custom perf path with argument
perf_path="perf"
if [[ "$#" -eq 1 ]]; then
    perf_path="$1"
    echo "Using custom perf_path: ${perf_path}"
fi

# Checking perf exists
"$perf_path" --version >/dev/null

# Setup 'perf stats' event input string
perf_stats_string="${perf_stats[0]}"
for perf_stat in "${perf_stats[@]:1}"; do
    perf_stats_string="${perf_stats_string},${perf_stat}"
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

# Append headers to result file
result_header="project, size, inversion-ratio, thr  eads, run, time"
for perf_stat in "${perf_stats[@]}"; do
    result_header="${result_header}, ${perf_stat}"
done
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

                    if ((size < 20000)); then
                        # perf doesn't get time to measure all events on small data sets,
                        # so with smaller data sets we run 'perf stat' once for each event,
                        # and average 'time' across all runs
                        perf_results=""
                        sum=0
                        num_perf_stats=${#perf_stats[@]}
                        for ((i = 0 ; i < $num_perf_stats; i++ )); do
                            perf_stat="${perf_stats[i]}"
                            time_ms=$("${perf_path}" stat -e "${perf_stat}" -o "${temp_perf_output}" ./run.sh "${workload}" ${threads})
                            sum=$(echo "scale=6; $sum + $time_ms" | bc)

                            stat_line=$(grep "  ${perf_stat}" "${temp_perf_output}")
                            stat_value=$(echo "$stat_line" | sed -E 's/[ \t]+(<not supported>|<not counted>|[0-9,.]+)[ \t]+[a-zA-Z0-9\-]+.*/\1/g')
                            stat_value=$(echo "$stat_value" | sed -E 's/,//g') # Remove thousand separator

                            if (($i==0)); then
                                perf_results="${stat_value}"
                            else
                                perf_results="${perf_results}, ${stat_value}"
                            fi
                        done
                            
                        average_time=$(echo "scale=2; $sum/$num_perf_stats" | bc )
                        echo "${average_time} ms"

                        result="${project}, ${size}, ${inversion_ratio}, ${threads}, ${run}, ${average_time}, ${perf_results}"
                    else
                        # Just run regularly (all stats at the same time)
                        time_ms=$("${perf_path}" stat -e ${perf_stats_string} -o "${temp_perf_output}" ./run.sh "${workload}" ${threads})
                        echo "${time_ms} ms"

                        result="${project}, ${size}, ${inversion_ratio}, ${threads}, ${run}, ${time_ms}"
                        for perf_stat in "${perf_stats[@]}"; do
                            stat_line=$(grep "  ${perf_stat}" "${temp_perf_output}")
                            stat_value=$(echo "$stat_line" | sed -E 's/[ \t]+(<not supported>|<not counted>|[0-9,.]+)[ \t]+[a-zA-Z0-9\-]+.*/\1/g')
                            stat_value=$(echo "$stat_value" | sed -E 's/,//g') # Remove thousand separator
                            result="${result}, ${stat_value}"
                        done
                    fi

                    echo "  ${result}"
                    echo "${result}" >> "${result_file}"
                done
            done
        done
    done
    cd ..
done