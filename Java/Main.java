package Java;
import java.util.Random; 

class Main {
    public static void main(String[] args) {
        int x[] = randomArray(1000000);
        assert(args.length == 1);
        int nrThreads = Integer.parseInt(args[0]); 
        assert(!isSorted(x));

        // Measure
        long startTime = System.nanoTime();
        Parallel.parallelSort(x, 0, x.length - 1, nrThreads);
        long endTime = System.nanoTime();

        assert(isSorted(x));
        long elapsedNanos = endTime - startTime;
        double elapsedMillis = (double) elapsedNanos / 1_000_000.0;
        System.out.printf("%.3f ms%n", elapsedMillis);
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