package main

import (
	"bufio"
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

func measure(sortFunc func([]int, int, int, int), nums []int, start int, end int, nrThreads int) float64 {
	before := time.Now()

	// Run
	sortFunc(nums, start, end, nrThreads)

	after := time.Now()
	elapsed := after.Sub(before)
	time_spent := float64(elapsed.Nanoseconds()) / 1000000.0
	fmt.Printf("%.2f\n", time_spent)

	return time_spent
}

func main() {
	if len(os.Args) != 3 {
		fmt.Print("First argument should be path to file\n")
		fmt.Print("Second argument should be number of threads\n")
		return
	}

	nrThreads, err := strconv.Atoi(os.Args[2])
	if err != nil {
		panic(err)
	}

	// nums := RandomInts(0, 1000000, n)

	path := os.Args[1]

	file, err := os.Open(path)
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	var numbers []int

	for scanner.Scan() {
		line := scanner.Text()
		num, err := strconv.Atoi(line)
		if err != nil {
			fmt.Println("Error parsing number:", err)
			return
		}
		numbers = append(numbers, num)
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error scanning file:", err)
		return
	}

	measure(paralell_sort, numbers, 0, len(numbers)-1, nrThreads)
	// print_nr_threads()

	if !isSorted(numbers) {
		fmt.Print("not sorted\n")
		return
	}
}
