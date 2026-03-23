// Double-Ended Queue (Deque) using a doubly linked list
#include <iostream>
#include <stdexcept>

template <typename T>
class Deque {
private:
    struct Node {
        T val;
        Node* prev;
        Node* next;
        Node(const T& v) : val(v), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int sz;

public:
    Deque() : head(nullptr), tail(nullptr), sz(0) {}

    ~Deque() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

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
        if (empty()) throw std::underflow_error("Deque empty");
        Node* tmp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete tmp;
        --sz;
    }

    void pop_back() {
        if (empty()) throw std::underflow_error("Deque empty");
        Node* tmp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete tmp;
        --sz;
    }

    T& front() { return head->val; }
    T& back()  { return tail->val; }
    bool empty() const { return sz == 0; }
    int size() const { return sz; }

    void print() const {
        Node* cur = head;
        while (cur) { std::cout << cur->val << " "; cur = cur->next; }
        std::cout << "\n";
    }
};

// Sliding Window Maximum using Deque (classic interview problem)
#include <vector>
std::vector<int> slidingWindowMax(const std::vector<int>& nums, int k) {
    std::deque<int> dq; // stores indices
    std::vector<int> result;
    for (int i = 0; i < (int)nums.size(); ++i) {
        while (!dq.empty() && dq.front() < i - k + 1) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] < nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) result.push_back(nums[dq.front()]);
    }
    return result;
}

int main() {
    Deque<int> d;
    d.push_back(1); d.push_back(2); d.push_back(3);
    d.push_front(0);
    d.print(); // 0 1 2 3
    d.pop_front(); d.pop_back();
    d.print(); // 1 2

    std::vector<int> nums = {1,3,-1,-3,5,3,6,7};
    auto res = slidingWindowMax(nums, 3);
    std::cout << "Sliding Window Max: ";
    for (int x : res) std::cout << x << " "; // 3 3 5 5 6 7
    std::cout << "\n";
    return 0;
}
