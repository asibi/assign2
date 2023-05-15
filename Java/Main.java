package Java;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner; 

class Main {

    public static int[] read_data(String path) {
        List<Integer> list = new ArrayList<Integer>(); 

        try {
            Scanner scanner = new Scanner(new File(path)); 
            
            while(scanner.hasNextInt()) {
                list.add(scanner.nextInt());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list.stream().mapToInt(i -> i).toArray(); 
    }

    // arguments: <path to workload file> <num. threads> 
    public static void main(String[] args) {
        // Handle arguments
        assert(args.length == 2);
        String path = args[0];
        int nrThreads = Integer.parseInt(args[1]); 

        int x[] = read_data(path);
        assert(!isSorted(x));

        // Measure
        long startTime = System.nanoTime();
        Parallel.parallelSort(x, 0, x.length - 1, nrThreads);
        long endTime = System.nanoTime();

        assert(isSorted(x));
        long elapsedNanos = endTime - startTime;
        double elapsedMillis = (double) elapsedNanos / 1_000_000.0;
        System.out.printf("%.2f%n", elapsedMillis);
        // Parallel.printThreadCount();
    }

    static boolean isSorted(int[] array) {
        for (int i = 0; i < array.length - 1; i++) {
            if (array[i] > array[i + 1])
                return false;
        }
        return true;
    }

    static int[] randomArray(int length) {
        Random rd = new Random();
        int[] arr = new int[length];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = rd.nextInt();
        }
        return arr; 
    }
}