mod mergesort;
use std::fs;
use std::time::Instant;

fn main() {

    let mut numbers = read_file("test-data.txt");    

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
