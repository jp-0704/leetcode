// Max-Heap with all operations + median-of-stream using two heaps
#include <iostream>
#include <vector>
#include <stdexcept>
#include <queue>

class MaxHeap {
private:
    std::vector<int> data;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i)   { return 2 * i + 1; }
    int right(int i)  { return 2 * i + 2; }

    void sift_up(int i) {
        while (i > 0 && data[parent(i)] < data[i]) {
            std::swap(data[parent(i)], data[i]);
            i = parent(i);
        }
    }

    void sift_down(int i) {
        int n = data.size(), largest = i;
        if (left(i)  < n && data[left(i)]  > data[largest]) largest = left(i);
        if (right(i) < n && data[right(i)] > data[largest]) largest = right(i);
        if (largest != i) {
            std::swap(data[i], data[largest]);
            sift_down(largest);
        }
    }

public:
    MaxHeap() = default;

    explicit MaxHeap(std::vector<int> arr) : data(std::move(arr)) {
        for (int i = data.size() / 2 - 1; i >= 0; --i)
            sift_down(i);
    }

    void push(int val) { data.push_back(val); sift_up(data.size() - 1); }

    void pop() {
        if (empty()) throw std::underflow_error("Heap empty");
        data[0] = data.back(); data.pop_back();
        if (!data.empty()) sift_down(0);
    }

    int top() const { return data.at(0); }
    bool empty() const { return data.empty(); }
    int size() const { return data.size(); }

    void print() const {
        for (int x : data) std::cout << x << " ";
        std::cout << "\n";
    }
};

// ── Median of a Data Stream (two-heap approach) ────────────────────────────────
class MedianFinder {
    std::priority_queue<int> maxHeap;                           // lower half
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // upper half

public:
    void addNum(int num) {
        maxHeap.push(num);
        minHeap.push(maxHeap.top()); maxHeap.pop();
        if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top()); minHeap.pop();
        }
    }

    double findMedian() const {
        if (maxHeap.size() > minHeap.size()) return maxHeap.top();
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};

int main() {
    MaxHeap h;
    h.push(4); h.push(10); h.push(3); h.push(5); h.push(1);
    std::cout << "Max: " << h.top() << "\n"; // 10
    h.pop();
    std::cout << "After pop, max: " << h.top() << "\n"; // 5
    h.print();

    MaxHeap h2({3, 1, 4, 1, 5, 9, 2, 6});
    std::cout << "Built from array, max: " << h2.top() << "\n"; // 9

    MedianFinder mf;
    for (int x : {5, 15, 1, 3}) {
        mf.addNum(x);
        std::cout << "Median after " << x << ": " << mf.findMedian() << "\n";
    }
    return 0;
}
