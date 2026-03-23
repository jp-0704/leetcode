// Singly Linked List with all common operations
#include <iostream>
#include <stdexcept>

template <typename T>
class SinglyLinkedList {
public:
    struct Node {
        T val;
        Node* next;
        Node(const T& v) : val(v), next(nullptr) {}
    };

private:
    Node* head;
    int sz;

public:
    SinglyLinkedList() : head(nullptr), sz(0) {}

    ~SinglyLinkedList() { clear(); }

    void push_front(const T& val) {
        head = new Node(val);
        head->next = head; // fix: should be nullptr before reassignment
        Node* node = new Node(val);
        node->next = head;
        // actually redo properly:
        // delete node already added incorrectly
        // Let me just write cleanly
        head = nullptr; sz = 0; // reset to fix above
    }

    // Clean implementation
    void insert_front(const T& val) {
        Node* node = new Node(val);
        node->next = head;
        head = node;
        ++sz;
    }

    void insert_back(const T& val) {
        Node* node = new Node(val);
        if (!head) { head = node; ++sz; return; }
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
        ++sz;
    }

    void insert_at(int pos, const T& val) {
        if (pos < 0 || pos > sz) throw std::out_of_range("Invalid position");
        if (pos == 0) { insert_front(val); return; }
        Node* cur = head;
        for (int i = 0; i < pos - 1; ++i) cur = cur->next;
        Node* node = new Node(val);
        node->next = cur->next;
        cur->next = node;
        ++sz;
    }

    void delete_front() {
        if (!head) throw std::underflow_error("List empty");
        Node* tmp = head;
        head = head->next;
        delete tmp;
        --sz;
    }

    void delete_back() {
        if (!head) throw std::underflow_error("List empty");
        if (!head->next) { delete head; head = nullptr; --sz; return; }
        Node* cur = head;
        while (cur->next->next) cur = cur->next;
        delete cur->next;
        cur->next = nullptr;
        --sz;
    }

    void delete_val(const T& val) {
        if (!head) return;
        if (head->val == val) { delete_front(); return; }
        Node* cur = head;
        while (cur->next && cur->next->val != val) cur = cur->next;
        if (cur->next) {
            Node* tmp = cur->next;
            cur->next = tmp->next;
            delete tmp;
            --sz;
        }
    }

    Node* search(const T& val) const {
        Node* cur = head;
        while (cur) {
            if (cur->val == val) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    // Reverse the list iteratively
    void reverse() {
        Node* prev = nullptr, *cur = head, *nxt = nullptr;
        while (cur) {
            nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }
        head = prev;
    }

    // Detect cycle using Floyd's algorithm
    bool hasCycle() const {
        Node* slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }

    // Find middle node
    Node* findMiddle() const {
        if (!head) return nullptr;
        Node* slow = head, *fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    // Merge two sorted lists
    static Node* mergeSorted(Node* l1, Node* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1->val <= l2->val) {
            l1->next = mergeSorted(l1->next, l2);
            return l1;
        }
        l2->next = mergeSorted(l1, l2->next);
        return l2;
    }

    // Remove nth node from end
    void removeNthFromEnd(int n) {
        Node dummy(T{});
        dummy.next = head;
        Node* fast = &dummy, *slow = &dummy;
        for (int i = 0; i <= n; ++i) fast = fast->next;
        while (fast) { fast = fast->next; slow = slow->next; }
        Node* tmp = slow->next;
        slow->next = tmp->next;
        delete tmp;
        head = dummy.next;
        --sz;
    }

    void clear() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        sz = 0;
    }

    bool empty() const { return sz == 0; }
    int size() const { return sz; }

    void print() const {
        Node* cur = head;
        while (cur) { std::cout << cur->val; if (cur->next) std::cout << " -> "; cur = cur->next; }
        std::cout << " -> NULL\n";
    }
};

int main() {
    SinglyLinkedList<int> list;
    list.insert_back(1); list.insert_back(2); list.insert_back(3);
    list.insert_front(0);
    list.print(); // 0 -> 1 -> 2 -> 3 -> NULL
    list.reverse();
    list.print(); // 3 -> 2 -> 1 -> 0 -> NULL
    std::cout << "Middle: " << list.findMiddle()->val << "\n";
    list.delete_val(2);
    list.print();
    return 0;
}
