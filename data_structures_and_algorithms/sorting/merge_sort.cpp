// Merge Sort — O(n log n) all cases, O(n) space, stable
#include <iostream>
#include <vector>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> tmp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else tmp[k++] = arr[j++];
    }
    while (i <= mid)  tmp[k++] = arr[i++];
    while (j <= right) tmp[k++] = arr[j++];
    for (int x = 0; x < k; ++x) arr[left + x] = tmp[x];
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Bottom-up iterative merge sort
void mergeSortIterative(std::vector<int>& arr) {
    int n = arr.size();
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid   = std::min(left + size - 1, n - 1);
            int right = std::min(left + 2 * size - 1, n - 1);
            if (mid < right) merge(arr, left, mid, right);
        }
    }
}

// Count inversions using merge sort
long long countInversions(std::vector<int>& arr, int left, int right) {
    if (left >= right) return 0;
    int mid = left + (right - left) / 2;
    long long count = countInversions(arr, left, mid) +
                      countInversions(arr, mid + 1, right);
    // Count split inversions during merge
    std::vector<int> tmp;
    int i = left, j = mid + 1;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) tmp.push_back(arr[i++]);
        else { count += (mid - i + 1); tmp.push_back(arr[j++]); }
    }
    while (i <= mid)  tmp.push_back(arr[i++]);
    while (j <= right) tmp.push_back(arr[j++]);
    for (int k = 0; k < (int)tmp.size(); ++k) arr[left + k] = tmp[k];
    return count;
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {38, 27, 43, 3, 9, 82, 10};
    mergeSort(a, 0, a.size() - 1);
    std::cout << "Merge Sort: "; print(a);

    std::vector<int> b = {5, 2, 8, 1, 9};
    mergeSortIterative(b);
    std::cout << "Iterative Merge Sort: "; print(b);

    std::vector<int> c = {8, 4, 2, 1};
    std::cout << "Inversions: " << countInversions(c, 0, c.size()-1) << "\n"; // 6
    return 0;
}
