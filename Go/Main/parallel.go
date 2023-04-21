package main

import (
	"fmt"
	"math"
	"sync/atomic"
)

var thread_count int32 = 1

func print_nr_threads() {
	fmt.Printf("Threads: %d\n", thread_count)
}

func paralell_depth(nums []int, start int, end int, depth int, maxThreadDepth int) {
	if depth < maxThreadDepth {

		mid := start + (end-start)/2

		// run thread 1
		done := make(chan struct{})
		go func() {
			paralell_depth(nums, start, mid, depth+1, maxThreadDepth)
			close(done)
		}()

		atomic.AddInt32(&thread_count, 1)

		// run thread 2 (this thread)
		paralell_depth(nums, mid+1, end, depth+1, maxThreadDepth)
		<-done

		merge(nums, start, mid, end)

	} else {
		merge_sort(nums, start, end)
	}
}

func paralell_sort(nums []int, start int, end int, nrThreads int) {
	var maxThreadDepth int = int(math.Log2(float64(nrThreads)))
	paralell_depth(nums, start, end, 0, maxThreadDepth)
}
