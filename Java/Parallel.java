package Java;

import java.util.concurrent.atomic.AtomicInteger;

public class Parallel {
    static AtomicInteger threadCount = new AtomicInteger(1);

    public static void printThreadCount() {
        System.out.println(threadCount.get());
    }

    public static void parallelSort(int x[], int start, int end, int nrThreads) {
        int maxThreadDepth = (int) (Math.log(nrThreads) / Math.log(2));
        parallelDepth(x, start, end, 0, maxThreadDepth); 
    }

    public static void parallelDepth(int x[], int start, int end, int depth, int maxThreadDepth) {
        if(depth < maxThreadDepth) {
            try {
                int mid = start + (end-start) / 2;
                Thread t = new MergeThread(x, start, mid, depth+1, maxThreadDepth);
                t.start(); 
                // threadCount.incrementAndGet(); // debug
                parallelDepth(x, mid+1, end, depth+1, maxThreadDepth);
                t.join();
                MergeSort.merge(x, start, mid, end);
            } catch(Exception e) {
                e.printStackTrace();
            }
        } else {
            MergeSort.mergeSort(x, start, end);
        }
    }

    static class MergeThread extends Thread {
        int[] x; 
        int start, end, depth, maxDepth;

        public MergeThread(int[] x, int start, int end, int depth, int maxDepth) {
            this.x = x;
            this.start = start;
            this.end = end; 
            this.depth = depth; 
            this.maxDepth = maxDepth; 
        }

        public void run() {
            parallelDepth(x, start, end, depth, maxDepth);
        }
    }
}
