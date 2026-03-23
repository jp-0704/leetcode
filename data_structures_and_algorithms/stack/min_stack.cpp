// MinStack: supports push, pop, top, and getMin in O(1)
#include <iostream>
#include <stack>
#include <stdexcept>

class MinStack {
private:
    std::stack<int> main_stack;
    std::stack<int> min_stack;

public:
    void push(int val) {
        main_stack.push(val);
        if (min_stack.empty() || val <= min_stack.top())
            min_stack.push(val);
    }

    void pop() {
        if (main_stack.empty()) throw std::underflow_error("Empty");
        if (main_stack.top() == min_stack.top())
            min_stack.pop();
        main_stack.pop();
    }

    int top() const {
        return main_stack.top();
    }

    int getMin() const {
        return min_stack.top();
    }
};

int main() {
    MinStack s;
    s.push(5); s.push(3); s.push(7); s.push(2); s.push(4);
    std::cout << "Top: " << s.top() << "\n";    // 4
    std::cout << "Min: " << s.getMin() << "\n"; // 2
    s.pop(); s.pop();
    std::cout << "Min after 2 pops: " << s.getMin() << "\n"; // 3
    return 0;
}
