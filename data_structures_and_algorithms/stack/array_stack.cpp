// Stack implemented using a fixed-size array
#include <iostream>
#include <stdexcept>

template <typename T, int CAPACITY = 1000>
class ArrayStack {
private:
    T data[CAPACITY];
    int top_idx;

public:
    ArrayStack() : top_idx(-1) {}

    void push(const T& val) {
        if (top_idx >= CAPACITY - 1)
            throw std::overflow_error("Stack overflow");
        data[++top_idx] = val;
    }

    void pop() {
        if (empty()) throw std::underflow_error("Stack underflow");
        --top_idx;
    }

    T& top() {
        if (empty()) throw std::underflow_error("Stack is empty");
        return data[top_idx];
    }

    bool empty() const { return top_idx == -1; }
    int size() const { return top_idx + 1; }

    void print() const {
        for (int i = top_idx; i >= 0; --i)
            std::cout << data[i] << " ";
        std::cout << "\n";
    }
};

int main() {
    ArrayStack<int> s;
    s.push(10); s.push(20); s.push(30);
    std::cout << "Top: " << s.top() << "\n"; // 30
    s.pop();
    std::cout << "After pop, top: " << s.top() << "\n"; // 20
    std::cout << "Size: " << s.size() << "\n";
    s.print();
    return 0;
}
