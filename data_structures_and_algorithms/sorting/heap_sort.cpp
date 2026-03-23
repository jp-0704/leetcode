// Heap Sort — O(n log n) all cases, O(1) space, NOT stable
#include <iostream>
#include <vector>

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1, right = 2 * i + 2;
    if (left  < n && arr[left]  > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    // Build max-heap (Floyd's algorithm) — O(n)
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    // Extract from heap one by one
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// K-th largest element using heap
int kthLargest(std::vector<int> arr, int k) {
    int n = arr.size();
    // Build max-heap
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);
    // Extract k-1 times
    for (int i = 0; i < k - 1; ++i) {
        std::swap(arr[0], arr[n - 1 - i]);
        heapify(arr, n - 1 - i, 0);
    }
    return arr[0];
}

// Sort nearly sorted (k-sorted) array — O(n log k)
#include <queue>
void sortNearlySorted(std::vector<int>& arr, int k) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    int idx = 0;
    for (int i = 0; i <= k && i < (int)arr.size(); ++i)
        minHeap.push(arr[i]);
    for (int i = k + 1; i < (int)arr.size(); ++i) {
        arr[idx++] = minHeap.top(); minHeap.pop();
        minHeap.push(arr[i]);
    }
    while (!minHeap.empty()) {
        arr[idx++] = minHeap.top(); minHeap.pop();
    }
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {12, 11, 13, 5, 6, 7};
    heapSort(a);
    std::cout << "Heap Sort: "; print(a);

    std::vector<int> b = {3, 2, 1, 5, 6, 4};
    std::cout << "2nd largest: " << kthLargest(b, 2) << "\n"; // 5

    std::vector<int> c = {6, 5, 3, 2, 8, 10, 9};
    sortNearlySorted(c, 3);
    std::cout << "K-sorted: "; print(c);
    return 0;
}
