// Bubble Sort — O(n^2) worst/average, O(n) best (optimized), O(1) space, stable
#include <iostream>
#include <vector>

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break; // already sorted
    }
}

// Cocktail Shaker Sort (bidirectional bubble sort)
void cocktailSort(std::vector<int>& arr) {
    int left = 0, right = arr.size() - 1;
    while (left < right) {
        for (int i = left; i < right; ++i)
            if (arr[i] > arr[i+1]) std::swap(arr[i], arr[i+1]);
        --right;
        for (int i = right; i > left; --i)
            if (arr[i] < arr[i-1]) std::swap(arr[i], arr[i-1]);
        ++left;
    }
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {64, 34, 25, 12, 22, 11, 90};
    bubbleSort(a);
    std::cout << "Bubble: "; print(a);

    std::vector<int> b = {5, 1, 4, 2, 8, 3};
    cocktailSort(b);
    std::cout << "Cocktail: "; print(b);
    return 0;
}
