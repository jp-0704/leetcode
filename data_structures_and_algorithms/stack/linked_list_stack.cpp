// Stack implemented using a singly linked list (dynamic size)
#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedStack {
private:
    struct Node {
        T val;
        Node* next;
        Node(const T& v, Node* n = nullptr) : val(v), next(n) {}
    };
    Node* head;
    int sz;

public:
    LinkedStack() : head(nullptr), sz(0) {}

    ~LinkedStack() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void push(const T& val) {
        head = new Node(val, head);
        ++sz;
    }

    void pop() {
        if (empty()) throw std::underflow_error("Stack underflow");
        Node* tmp = head;
        head = head->next;
        delete tmp;
        --sz;
    }

    T& top() {
        if (empty()) throw std::underflow_error("Stack is empty");
        return head->val;
    }

    bool empty() const { return head == nullptr; }
    int size() const { return sz; }

    void print() const {
        Node* cur = head;
        while (cur) {
            std::cout << cur->val << " ";
            cur = cur->next;
        }
        std::cout << "\n";
    }
};

int main() {
    LinkedStack<int> s;
    s.push(1); s.push(2); s.push(3);
    std::cout << "Top: " << s.top() << "\n"; // 3
    s.pop();
    s.print(); // 2 1
    return 0;
}
