// Selection Sort — O(n^2) all cases, O(1) space, NOT stable
#include <iostream>
#include <vector>

void selectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j)
            if (arr[j] < arr[min_idx]) min_idx = j;
        std::swap(arr[i], arr[min_idx]);
    }
}

// Stable selection sort using insertion to maintain order
void stableSelectionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j)
            if (arr[j] < arr[min_idx]) min_idx = j;
        // Shift instead of swap to maintain stability
        int key = arr[min_idx];
        while (min_idx > i) {
            arr[min_idx] = arr[min_idx - 1];
            --min_idx;
        }
        arr[i] = key;
    }
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {64, 25, 12, 22, 11};
    selectionSort(a);
    std::cout << "Selection: "; print(a);

    std::vector<int> b = {5, 3, 1, 4, 2};
    stableSelectionSort(b);
    std::cout << "Stable Selection: "; print(b);
    return 0;
}
