// Insertion Sort — O(n^2) worst, O(n) best, O(1) space, stable
// Best for small arrays or nearly sorted data
#include <iostream>
#include <vector>

void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Binary Insertion Sort: O(n log n) comparisons, O(n^2) swaps
void binaryInsertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int lo = 0, hi = i;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (arr[mid] <= key) lo = mid + 1;
            else hi = mid;
        }
        for (int j = i; j > lo; --j) arr[j] = arr[j-1];
        arr[lo] = key;
    }
}

// Shell Sort — generalization of insertion sort with gap sequences
void shellSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            int key = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > key) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = key;
        }
    }
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {12, 11, 13, 5, 6};
    insertionSort(a);
    std::cout << "Insertion: "; print(a);

    std::vector<int> b = {9, 3, 7, 1, 8, 2};
    binaryInsertionSort(b);
    std::cout << "Binary Insertion: "; print(b);

    std::vector<int> c = {5, 3, 8, 4, 2, 9, 1, 7};
    shellSort(c);
    std::cout << "Shell: "; print(c);
    return 0;
}
