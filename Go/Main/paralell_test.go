package main

import "testing"

func TestParallelSort(t *testing.T) {
	// Generate a random slice of integers
	nums := RandomInts(0, 1000000, 1000000)
	// Sort the slice
	paralell_sort(nums, 0, len(nums)-1, 4)
	// Check if the slice is sorted
	if !isSorted(nums) {
		t.Errorf("slice is not sorted")
	}
}
