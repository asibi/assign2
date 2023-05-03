mod mergesort;
use std::{fs, env};
use std::time::Instant;

fn main() {

    let args: Vec<String> = env::args().collect();
    
    let file_path = &args[1];
    let num_threads = args[2].parse::<i32>().expect("Invalid threads argument");

    const VALID_NUM_THREADS: [i32; 6] = [1, 2, 4, 8, 16, 32];    
    assert!(VALID_NUM_THREADS.contains(&num_threads), "Invalid number of threads");

    let mut numbers = read_file(file_path);    

    const THREADS: i32 = 1;
    let now = Instant::now();
    mergesort::sort(&mut numbers, (THREADS as f64).log2().ceil() as i32);
    println!("{}", now.elapsed().as_millis());
}


fn read_file(file_path: &str) -> Vec<i32> {
    let file_content = fs::read_to_string(file_path).expect("Error test data file");

    let mut numbers: Vec<i32> = Vec::new();

    for line in file_content.lines() {
        numbers.push(line.parse::<i32>().expect("Invalid test data value"));
    }

    numbers
}
