// Queue implemented using a linked list
#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedQueue {
private:
    struct Node {
        T val;
        Node* next;
        Node(const T& v) : val(v), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int sz;

public:
    LinkedQueue() : head(nullptr), tail(nullptr), sz(0) {}

    ~LinkedQueue() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void enqueue(const T& val) {
        Node* node = new Node(val);
        if (tail) tail->next = node;
        tail = node;
        if (!head) head = node;
        ++sz;
    }

    void dequeue() {
        if (empty()) throw std::underflow_error("Queue underflow");
        Node* tmp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
        --sz;
    }

    T& front() {
        if (empty()) throw std::underflow_error("Queue empty");
        return head->val;
    }

    T& back() {
        if (empty()) throw std::underflow_error("Queue empty");
        return tail->val;
    }

    bool empty() const { return sz == 0; }
    int size() const { return sz; }

    void print() const {
        Node* cur = head;
        while (cur) { std::cout << cur->val << " "; cur = cur->next; }
        std::cout << "\n";
    }
};

int main() {
    LinkedQueue<int> q;
    q.enqueue(1); q.enqueue(2); q.enqueue(3);
    q.print();       // 1 2 3
    q.dequeue();
    q.print();       // 2 3
    std::cout << "Front: " << q.front() << "\n"; // 2
    return 0;
}
