use std::thread;

pub fn sort(numbers: &mut [i32], parallelize_depth: i32) {
    let mut work_area: Vec<i32> = vec![0i32; numbers.len()];
    sort_recursion(numbers, &mut work_area, parallelize_depth);
}

fn sort_recursion(numbers: &mut[i32], work_area: &mut[i32], parallelize_depth: i32){
    assert!(numbers.len() > 0);

    if numbers.len() == 1 {
        return;
    }

    let mid = numbers.len() / 2;    

    let length = numbers.len();
    let (sequence1, sequence2) = numbers.split_at_mut(mid);
    let (work_area1, work_area2) = work_area.split_at_mut(mid);
    
    if parallelize_depth > 0 {
        thread::scope(|scope| {
            scope.spawn(||{
                sort_recursion(sequence2, work_area2, parallelize_depth - 1);    
            });
            sort_recursion(sequence1, work_area1, parallelize_depth - 1);
        });
    }
    else {
        sort_recursion(sequence1, work_area1, parallelize_depth - 1);
        sort_recursion(sequence2, work_area2, parallelize_depth - 1);
    }

    let mut next1 = 0;
    let mut next2 = 0;

    for i in 0..length {
        if next1 >= sequence1.len() {
            work_area[i] = sequence2[next2];
            next2 += 1;
            continue
        }
        if next2 >= sequence2.len() {
            work_area[i] = sequence1[next1];
            next1 += 1;
            continue;    
        }
        if sequence1[next1] <= sequence2[next2] {
            work_area[i] = sequence1[next1];
            next1 += 1;
            continue;    
        }
        if sequence2[next2] <= sequence1[next1] {
            work_area[i] = sequence2[next2];
            next2 += 1;
            continue; 
        }  
    }        

    for i in 0..numbers.len() {
        numbers[i] = work_area[i];
    }
}


#[cfg(test)]
mod tests {

    use super::*;

    #[test]
    fn even_number_of_elemeents() {
        let mut numbers = [9, 4, 86, 12, 58, 3];        
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers)
    }
    
    #[test]
    fn uneven_number_of_elements() {
        let mut numbers = [9, 4, 86, 12, 58];        
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers)
    }

    #[test]
    fn single_element() {
        let mut numbers = [1];
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers) 
    }

    #[test]
    fn already_sorted_elements() {
        let mut numbers = [3, 4, 9, 12, 58, 86];        
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers)
    }

    #[test]
    fn reverse_sorted_elements() {
        let mut numbers = [96, 58, 12, 9, 4, 3]; 
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers)              
    }

    #[test]
    fn some_elements_are_equal() {
        let mut numbers = [9, 4, 86, 12, 4, 58, 3, 58];        
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers)          
    }    

    #[test]
    fn all_elements_are_equal() {
        let mut numbers = [1, 1, 1, 1, 1, 1];              
        sort(&mut numbers, 0);
        assert_numbers_are_sorted(&numbers)          
    }    

    #[test]
    fn more_splits_than_threads() {
        let mut numbers = [9, 4, 86, 12, 58, 3]; 
        sort(&mut numbers, 2);       
        assert_numbers_are_sorted(&numbers)          
    }

    
    #[test]
    fn more_threads_than_splits() {
        let mut numbers = [9, 4, 86, 12, 58, 3]; 
        sort(&mut numbers, 10);       
        assert_numbers_are_sorted(&numbers)          
    }

    fn assert_numbers_are_sorted(numbers: &[i32]) {
        for i in 0..numbers.len()-1 {
            assert!(numbers[i] <= numbers[i+1]);
        }
    }
}   