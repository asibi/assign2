#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <random>

#include "inversion.hpp"

namespace fs = std::filesystem;

std::random_device rd;
std::mt19937 mt(rd());


float estimate_inversion_ratio(const std::vector<int32_t>& numbers) {
    const size_t num_iterations = numbers.size() * 10;
    std::uniform_int_distribution<size_t> distribution(0, numbers.size());

    size_t num_tests = 0;
    size_t num_inversions = 0;
    for( size_t i = 1; i <= num_iterations; i++ ) {
        size_t index1 = distribution(mt);
        size_t index2 = distribution(mt);
        if( index1 == index2 ) continue;

        if( index1 > index2 && numbers[index1] < numbers[index2] ) {
            num_inversions++;
        }
        if( index1 < index2 && numbers[index1] > numbers[index2] ) {
            num_inversions++;
        }

        num_tests++;
    }

    return num_inversions == num_tests ? 1.0 : (double)num_inversions / num_iterations;
}


std::vector<int> generate_workload(int size, float desired_inversion_ratio) {
    assert(desired_inversion_ratio >= 0 && desired_inversion_ratio <= 1 && "Inversion ratio must be between 0 and 1");

    // Exact numbers doesn't really matter, so we just create list of sequential numbers
    std::vector<int32_t> numbers(size);

    if( desired_inversion_ratio == 1 ) {
        for( int i = 0; i < size; i++ ) {
            numbers[i] = size - i;
        }    
        return numbers;
    }

    for( int i = 0; i < size; i++ ) {
        numbers[i] = i;
    }    

    if( desired_inversion_ratio == 0 ) {
        return numbers;
    }

    static std::vector<size_t> swap_indices;
    swap_indices.clear();
    swap_indices.reserve(size);
    for( int i = 0; i < size; i++ ) {
        swap_indices.push_back(i);
    }

    std::shuffle(swap_indices.begin(), swap_indices.end(), mt);

    const double PERCENTAGE_SWAP_PER_ITERATION = 0.01;

    int swaps_per_iteration = (int)(size * PERCENTAGE_SWAP_PER_ITERATION);
    if( swaps_per_iteration == 0 ) {
        swaps_per_iteration = 1;
    }

    std::cout << "  Swap rate: " << swaps_per_iteration << std::endl;

    size_t iteration = 0;
    while( true ) {
        if( swap_indices.size() == 0 ) break;

        iteration++;

        for( int j = 0; j < swaps_per_iteration; j++ ) {
            if( swap_indices.size() == 0 ) break;

            size_t index_to_swap = swap_indices.back();
            swap_indices.pop_back();
            numbers[index_to_swap] = size - (index_to_swap + 1);
        }

        float inversion_ratio = estimate_inversion_ratio(numbers);
        std::cout << "  Iteration " << iteration << ": " << inversion_ratio << std::endl;
        if( inversion_ratio >= desired_inversion_ratio ) break;
    }

    std::cout << "  Desired inversion ratio reached!" << std::endl;


    return numbers;
}


void store_workload(const fs::path& file_path, const std::vector<int32_t> workload) {
    std::ofstream output_file;
    output_file.open(file_path, std::ios::trunc);
    
    for( int number : workload ) {
        output_file << number << std::endl;
    }

    output_file.close();
}


int main(int argc, char** argv) {

    // Ensure output dir exists
    const fs::path output_dir = fs::current_path() / "workloads";
    if( !fs::exists(output_dir) ) {
        fs::create_directory(output_dir);
    }
    
    fs::file_status output_dir_status = fs::status(output_dir);
    assert(output_dir_status.type() == fs::file_type::directory && "Output path is not a directory");

    const size_t size_levels[5] = {
        64, // 2^6
        1'024, // 2^10
        16'384, // 2^14
        262'144, // 2^18
        4'194'304 // 2^22
    };

    const double inversion_levels[5] = { 0, 0.25, 0.5, 0.75, 1 };
    
    for( size_t size : size_levels ) {
        for( double inversion : inversion_levels ) {
            std::cout << "Workload: " << size << ", " << inversion << ".." << std::endl;
            std::vector<int32_t> workload = generate_workload(size, inversion);
            fs::path output_file_path = 
                output_dir / (std::to_string(size) + "-" + std::to_string((int)(inversion * 100)) + ".txt");
            store_workload(output_file_path, workload);
        }    
    }   
}