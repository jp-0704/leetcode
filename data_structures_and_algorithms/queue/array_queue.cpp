// Queue implemented using a circular array
#include <iostream>
#include <stdexcept>

template <typename T, int CAPACITY = 1000>
class CircularQueue {
private:
    T data[CAPACITY + 1]; // +1 to distinguish full from empty
    int front_idx, rear_idx;

public:
    CircularQueue() : front_idx(0), rear_idx(0) {}

    void enqueue(const T& val) {
        if (full()) throw std::overflow_error("Queue overflow");
        data[rear_idx] = val;
        rear_idx = (rear_idx + 1) % (CAPACITY + 1);
    }

    void dequeue() {
        if (empty()) throw std::underflow_error("Queue underflow");
        front_idx = (front_idx + 1) % (CAPACITY + 1);
    }

    T& front() {
        if (empty()) throw std::underflow_error("Queue empty");
        return data[front_idx];
    }

    T& back() {
        if (empty()) throw std::underflow_error("Queue empty");
        return data[(rear_idx - 1 + CAPACITY + 1) % (CAPACITY + 1)];
    }

    bool empty() const { return front_idx == rear_idx; }
    bool full() const { return (rear_idx + 1) % (CAPACITY + 1) == front_idx; }

    int size() const {
        return (rear_idx - front_idx + CAPACITY + 1) % (CAPACITY + 1);
    }

    void print() const {
        int i = front_idx;
        while (i != rear_idx) {
            std::cout << data[i] << " ";
            i = (i + 1) % (CAPACITY + 1);
        }
        std::cout << "\n";
    }
};

int main() {
    CircularQueue<int> q;
    q.enqueue(10); q.enqueue(20); q.enqueue(30);
    std::cout << "Front: " << q.front() << "\n"; // 10
    std::cout << "Back:  " << q.back()  << "\n"; // 30
    q.dequeue();
    std::cout << "After dequeue, front: " << q.front() << "\n"; // 20
    q.print();
    return 0;
}
