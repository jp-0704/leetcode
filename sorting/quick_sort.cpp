// Quick Sort — O(n log n) average, O(n^2) worst, O(log n) space, NOT stable
#include <iostream>
#include <vector>
#include <algorithm>

// Lomuto partition scheme
int partitionLomuto(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) std::swap(arr[++i], arr[j]);
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Hoare partition scheme (fewer swaps)
int partitionHoare(std::vector<int>& arr, int low, int high) {
    int pivot = arr[low + (high - low) / 2];
    int i = low - 1, j = high + 1;
    while (true) {
        do { ++i; } while (arr[i] < pivot);
        do { --j; } while (arr[j] > pivot);
        if (i >= j) return j;
        std::swap(arr[i], arr[j]);
    }
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionLomuto(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 3-way partition (Dutch National Flag) — optimal for many duplicates
void quickSort3Way(std::vector<int>& arr, int low, int high) {
    if (low >= high) return;
    int lt = low, gt = high, i = low;
    int pivot = arr[low + rand() % (high - low + 1)];
    while (i <= gt) {
        if      (arr[i] < pivot) std::swap(arr[lt++], arr[i++]);
        else if (arr[i] > pivot) std::swap(arr[i], arr[gt--]);
        else                     ++i;
    }
    quickSort3Way(arr, low, lt - 1);
    quickSort3Way(arr, gt + 1, high);
}

// Randomized quick sort (avoids worst case for sorted input)
int randomPartition(std::vector<int>& arr, int low, int high) {
    int r = low + rand() % (high - low + 1);
    std::swap(arr[r], arr[high]);
    return partitionLomuto(arr, low, high);
}

void quickSortRandom(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = randomPartition(arr, low, high);
        quickSortRandom(arr, low, pi - 1);
        quickSortRandom(arr, pi + 1, high);
    }
}

// Quickselect — find k-th smallest in O(n) average
int quickSelect(std::vector<int>& arr, int low, int high, int k) {
    if (low == high) return arr[low];
    int pi = randomPartition(arr, low, high);
    if (pi == k) return arr[pi];
    if (k < pi)  return quickSelect(arr, low, pi - 1, k);
    return           quickSelect(arr, pi + 1, high, k);
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {10, 7, 8, 9, 1, 5};
    quickSort(a, 0, a.size() - 1);
    std::cout << "Quick Sort: "; print(a);

    std::vector<int> b = {3, 3, 3, 1, 2, 2, 4};
    quickSort3Way(b, 0, b.size() - 1);
    std::cout << "3-Way Quick Sort: "; print(b);

    std::vector<int> c = {7, 10, 4, 3, 20, 15};
    std::cout << "3rd smallest: " << quickSelect(c, 0, c.size()-1, 2) << "\n"; // 7
    return 0;
}
