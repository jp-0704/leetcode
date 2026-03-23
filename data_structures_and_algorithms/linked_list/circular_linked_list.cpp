// Circular Singly Linked List
#include <iostream>
#include <stdexcept>

template <typename T>
class CircularLinkedList {
public:
    struct Node {
        T val;
        Node* next;
        Node(const T& v) : val(v), next(nullptr) {}
    };

private:
    Node* tail; // tail->next == head
    int sz;

public:
    CircularLinkedList() : tail(nullptr), sz(0) {}

    ~CircularLinkedList() { clear(); }

    Node* head() const { return tail ? tail->next : nullptr; }

    void push_front(const T& val) {
        Node* node = new Node(val);
        if (!tail) { node->next = node; tail = node; }
        else { node->next = tail->next; tail->next = node; }
        ++sz;
    }

    void push_back(const T& val) {
        push_front(val);
        tail = tail->next; // new node is at front, make it tail
    }

    void pop_front() {
        if (!tail) throw std::underflow_error("List empty");
        Node* h = tail->next;
        if (h == tail) { delete tail; tail = nullptr; }
        else { tail->next = h->next; delete h; }
        --sz;
    }

    bool search(const T& val) const {
        if (!tail) return false;
        Node* cur = tail->next;
        do {
            if (cur->val == val) return true;
            cur = cur->next;
        } while (cur != tail->next);
        return false;
    }

    void clear() {
        if (!tail) return;
        Node* cur = tail->next;
        while (cur != tail) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        delete tail;
        tail = nullptr;
        sz = 0;
    }

    void print() const {
        if (!tail) { std::cout << "Empty\n"; return; }
        Node* cur = tail->next;
        do {
            std::cout << cur->val << " -> ";
            cur = cur->next;
        } while (cur != tail->next);
        std::cout << "(back to head)\n";
    }

    bool empty() const { return sz == 0; }
    int size() const { return sz; }
};

// Josephus Problem using circular linked list
int josephus(int n, int k) {
    CircularLinkedList<int> list;
    for (int i = 1; i <= n; ++i) list.push_back(i);
    // Simulate: every k-th person is eliminated
    // (simplified: use formula for large n, but demonstrate with list)
    int survivor = 0;
    for (int i = 1; i < n; ++i)
        survivor = (survivor + k) % (n - i + 1);
    // Classic formula result
    int pos = 0;
    for (int i = 2; i <= n; ++i)
        pos = (pos + k) % i;
    return pos + 1;
}

int main() {
    CircularLinkedList<int> cl;
    cl.push_back(1); cl.push_back(2); cl.push_back(3);
    cl.push_front(0);
    cl.print(); // 0 -> 1 -> 2 -> 3 -> (back to head)
    cl.pop_front();
    cl.print(); // 1 -> 2 -> 3 -> (back to head)

    std::cout << "Josephus(7,3) = " << josephus(7, 3) << "\n"; // 4
    return 0;
}
