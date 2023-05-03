package Java;

class MergeSort {

  public static void mergeSort(int x[], int start, int end) {
    if(start < end) {
        // Divide
        int mid = start + (end-start) / 2;
        mergeSort(x, start, mid);
        mergeSort(x, mid + 1, end);

        // Merge
        merge(x, start, mid, end);
    }
  }

  public static void merge(int x[], int start, int mid, int end) {
    int i,j,k;
    int n1 = (mid - start) + 1;
    int n2 = end - mid;
    int L[] = new int[n1];
    int R[] = new int[n2];
    
    // Copy arrays
    System.arraycopy(x, start, L, 0, n1); // todo check correctness
    System.arraycopy(x, mid+1, R, 0, n2);

    i = 0; 
    j = 0;
    k = start; 

    while(i < n1 && j < n2) {
        if(L[i] < R[j]) 
            x[k++] = L[i++];
        else 
            x[k++] = R[j++];
    }

    while(i < n1) {
        x[k++] = L[i++];
    }
    while(j < n2) {
        x[k++] = R[j++];
    }
  }
}
