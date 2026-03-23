// Doubly Linked List with all common operations
#include <iostream>
#include <stdexcept>

template <typename T>
class DoublyLinkedList {
public:
    struct Node {
        T val;
        Node* prev;
        Node* next;
        Node(const T& v) : val(v), prev(nullptr), next(nullptr) {}
    };

private:
    Node* head;
    Node* tail;
    int sz;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), sz(0) {}

    ~DoublyLinkedList() { clear(); }

    void push_front(const T& val) {
        Node* node = new Node(val);
        if (head) { node->next = head; head->prev = node; }
        head = node;
        if (!tail) tail = node;
        ++sz;
    }

    void push_back(const T& val) {
        Node* node = new Node(val);
        if (tail) { tail->next = node; node->prev = tail; }
        tail = node;
        if (!head) head = node;
        ++sz;
    }

    void pop_front() {
        if (!head) throw std::underflow_error("List empty");
        Node* tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete tmp;
        --sz;
    }

    void pop_back() {
        if (!tail) throw std::underflow_error("List empty");
        Node* tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete tmp;
        --sz;
    }

    void insert_after(Node* node, const T& val) {
        if (!node) return;
        Node* newNode = new Node(val);
        newNode->next = node->next;
        newNode->prev = node;
        if (node->next) node->next->prev = newNode;
        node->next = newNode;
        if (node == tail) tail = newNode;
        ++sz;
    }

    void delete_node(Node* node) {
        if (!node) return;
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
        delete node;
        --sz;
    }

    Node* search(const T& val) const {
        Node* cur = head;
        while (cur) {
            if (cur->val == val) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void reverse() {
        Node* cur = head;
        std::swap(head, tail);
        while (cur) {
            std::swap(cur->prev, cur->next);
            cur = cur->prev; // was next before swap
        }
    }

    void clear() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        sz = 0;
    }

    void print() const {
        Node* cur = head;
        std::cout << "NULL <-> ";
        while (cur) {
            std::cout << cur->val;
            if (cur->next) std::cout << " <-> ";
            cur = cur->next;
        }
        std::cout << " <-> NULL\n";
    }

    void print_reverse() const {
        Node* cur = tail;
        while (cur) { std::cout << cur->val << " "; cur = cur->prev; }
        std::cout << "\n";
    }

    bool empty() const { return sz == 0; }
    int size() const { return sz; }
    Node* front() const { return head; }
    Node* back() const { return tail; }
};

int main() {
    DoublyLinkedList<int> list;
    list.push_back(1); list.push_back(2); list.push_back(3);
    list.push_front(0);
    list.print(); // NULL <-> 0 <-> 1 <-> 2 <-> 3 <-> NULL

    list.print_reverse(); // 3 2 1 0
    list.pop_front(); list.pop_back();
    list.print(); // NULL <-> 1 <-> 2 <-> NULL

    list.reverse();
    list.print(); // NULL <-> 2 <-> 1 <-> NULL
    return 0;
}
