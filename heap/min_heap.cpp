// Min-Heap with all operations
#include <iostream>
#include <vector>
#include <stdexcept>

class MinHeap {
private:
    std::vector<int> data;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i)   { return 2 * i + 1; }
    int right(int i)  { return 2 * i + 2; }

    void sift_up(int i) {
        while (i > 0 && data[parent(i)] > data[i]) {
            std::swap(data[parent(i)], data[i]);
            i = parent(i);
        }
    }

    void sift_down(int i) {
        int n = data.size(), smallest = i;
        int l = left(i), r = right(i);
        if (l < n && data[l] < data[smallest]) smallest = l;
        if (r < n && data[r] < data[smallest]) smallest = r;
        if (smallest != i) {
            std::swap(data[i], data[smallest]);
            sift_down(smallest);
        }
    }

public:
    MinHeap() = default;

    // Build heap from vector in O(n)
    explicit MinHeap(std::vector<int> arr) : data(std::move(arr)) {
        for (int i = data.size() / 2 - 1; i >= 0; --i)
            sift_down(i);
    }

    void push(int val) {
        data.push_back(val);
        sift_up(data.size() - 1);
    }

    void pop() {
        if (empty()) throw std::underflow_error("Heap empty");
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) sift_down(0);
    }

    int top() const {
        if (empty()) throw std::underflow_error("Heap empty");
        return data[0];
    }

    void decrease_key(int i, int new_val) {
        if (new_val > data[i]) throw std::invalid_argument("New value larger");
        data[i] = new_val;
        sift_up(i);
    }

    void delete_at(int i) {
        decrease_key(i, INT_MIN);
        pop();
    }

    bool empty() const { return data.empty(); }
    int size() const { return data.size(); }

    void print() const {
        for (int x : data) std::cout << x << " ";
        std::cout << "\n";
    }

    // Merge two heaps — O(n)
    void merge(const MinHeap& other) {
        for (int x : other.data) push(x);
    }
};

// Find k smallest elements using min-heap
std::vector<int> kSmallest(const std::vector<int>& arr, int k) {
    MinHeap h(arr);
    std::vector<int> result;
    for (int i = 0; i < k && !h.empty(); ++i) {
        result.push_back(h.top());
        h.pop();
    }
    return result;
}

int main() {
    MinHeap h;
    h.push(5); h.push(3); h.push(8); h.push(1); h.push(10);
    std::cout << "Min: " << h.top() << "\n"; // 1
    h.pop();
    std::cout << "After pop, min: " << h.top() << "\n"; // 3
    h.print();

    MinHeap h2({9, 4, 7, 2, 6, 1});
    std::cout << "Built from array, min: " << h2.top() << "\n"; // 1

    auto smallest = kSmallest({7, 10, 4, 3, 20, 15}, 3);
    std::cout << "3 smallest: ";
    for (int x : smallest) std::cout << x << " "; // 3 4 7
    std::cout << "\n";
    return 0;
}
