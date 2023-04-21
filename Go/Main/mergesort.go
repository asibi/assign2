package main

import (
	"math/rand"
	"time"
)

func merge(nums []int, start int, mid int, end int) {
	n1 := mid - start + 1
	n2 := end - mid
	l := make([]int, n1)
	r := make([]int, n2)

	// Copy arrays
	copy(l, nums[start:mid+1])
	copy(r, nums[mid+1:end+1])

	i := 0
	j := 0
	k := start

	for i < n1 && j < n2 {
		if l[i] <= r[j] {
			nums[k] = l[i]
			i++
		} else {
			nums[k] = r[j]
			j++
		}
		k++
	}

	for i < n1 {
		nums[k] = l[i]
		i++
		k++
	}

	for j < n2 {
		nums[k] = r[j]
		j++
		k++
	}
}

func merge_sort(nums []int, start int, end int) {
	if start < end {
		mid := start + (end-start)/2
		merge_sort(nums, start, mid)
		merge_sort(nums, mid+1, end)
		merge(nums, start, mid, end)
	}
}

func RandomInts(min, max, size int) []int {
	// Initialize a new random seed
	rand.Seed(time.Now().UnixNano())

	// Create a slice of the given size
	nums := make([]int, size)

	// Fill the slice with random integers in the given range
	for i := 0; i < size; i++ {
		nums[i] = rand.Intn(max-min) + min
	}

	return nums
}
