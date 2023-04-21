package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

func isSorted(nums []int) bool {
	for i := 0; i < len(nums)-1; i++ {
		if nums[i] > nums[i+1] {
			return false
		}
	}
	return true
}

func measure(sortFunc func([]int, int, int, int), nums []int, start int, end int, nrThreads int) int64 {
	before := time.Now()

	// Run
	sortFunc(nums, start, end, nrThreads)

	after := time.Now()
	time_spent := after.Sub(before).Milliseconds()
	fmt.Printf("%.3d ms\n", time_spent)

	return time_spent
}

func main() {
	if len(os.Args) != 2 {
		fmt.Print("must provide number of threads\n")
		return
	}

	nrThreads, err := strconv.Atoi(os.Args[1])
	if err != nil {
		panic(err)
	}
	n := 10000000

	nums := RandomInts(0, 1000000, n)

	measure(paralell_sort, nums, 0, n-1, nrThreads)
	print_nr_threads()

	if !isSorted(nums) {
		fmt.Print("not sorted\n")
		return
	}
}
