// Priority Queue (Binary Heap based) — both min-heap and max-heap
#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> heap;
    Compare cmp;

    void sift_up(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (cmp(heap[parent], heap[i])) {
                std::swap(heap[parent], heap[i]);
                i = parent;
            } else break;
        }
    }

    void sift_down(int i) {
        int n = heap.size();
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2, best = i;
            if (left  < n && cmp(heap[best], heap[left]))  best = left;
            if (right < n && cmp(heap[best], heap[right])) best = right;
            if (best == i) break;
            std::swap(heap[i], heap[best]);
            i = best;
        }
    }

public:
    void push(const T& val) {
        heap.push_back(val);
        sift_up(heap.size() - 1);
    }

    void pop() {
        if (empty()) throw std::underflow_error("PQ empty");
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) sift_down(0);
    }

    const T& top() const {
        if (empty()) throw std::underflow_error("PQ empty");
        return heap[0];
    }

    bool empty() const { return heap.empty(); }
    int size() const { return heap.size(); }
};

// Heap Sort using max-heap
void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    // Build max-heap
    for (int i = n / 2 - 1; i >= 0; --i) {
        int j = i;
        while (true) {
            int l = 2*j+1, r = 2*j+2, best = j;
            if (l < n && arr[l] > arr[best]) best = l;
            if (r < n && arr[r] > arr[best]) best = r;
            if (best == j) break;
            std::swap(arr[j], arr[best]); j = best;
        }
    }
    // Extract elements
    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        int j = 0, sz = i;
        while (true) {
            int l = 2*j+1, r = 2*j+2, best = j;
            if (l < sz && arr[l] > arr[best]) best = l;
            if (r < sz && arr[r] > arr[best]) best = r;
            if (best == j) break;
            std::swap(arr[j], arr[best]); j = best;
        }
    }
}

int main() {
    // Max-heap (default)
    PriorityQueue<int> maxpq;
    maxpq.push(3); maxpq.push(1); maxpq.push(4); maxpq.push(1); maxpq.push(5);
    std::cout << "Max-heap top: " << maxpq.top() << "\n"; // 5

    // Min-heap using greater comparator
    PriorityQueue<int, std::greater<int>> minpq;
    minpq.push(3); minpq.push(1); minpq.push(4);
    std::cout << "Min-heap top: " << minpq.top() << "\n"; // 1

    std::vector<int> arr = {5, 2, 8, 1, 9, 3};
    heapSort(arr);
    std::cout << "Heap sorted: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
    return 0;
}
