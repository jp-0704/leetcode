// Fenwick Tree (Binary Indexed Tree) — prefix sums in O(log n)
#include <iostream>
#include <vector>

class FenwickTree {
private:
    int n;
    std::vector<long long> tree;

public:
    FenwickTree(int n) : n(n), tree(n + 1, 0) {}

    // Build from array in O(n)
    FenwickTree(const std::vector<int>& arr) : n(arr.size()), tree(arr.size() + 1, 0) {
        for (int i = 0; i < n; ++i) update(i + 1, arr[i]);
    }

    // Add delta to index i (1-based)
    void update(int i, long long delta) {
        for (; i <= n; i += i & (-i))
            tree[i] += delta;
    }

    // Prefix sum [1..i]
    long long query(int i) const {
        long long sum = 0;
        for (; i > 0; i -= i & (-i))
            sum += tree[i];
        return sum;
    }

    // Range sum [l..r] (1-based)
    long long query(int l, int r) const {
        return query(r) - query(l - 1);
    }

    // Point query (useful if BIT stores differences)
    long long pointQuery(int i) const {
        return query(i) - query(i - 1);
    }

    // Find k-th element (1-indexed) in O(log n)
    int kthElement(long long k) const {
        int pos = 0;
        for (int pw = 1 << (int)log2(n); pw > 0; pw >>= 1) {
            if (pos + pw <= n && tree[pos + pw] < k) {
                pos += pw;
                k -= tree[pos];
            }
        }
        return pos + 1;
    }
};

// 2D Fenwick Tree
class FenwickTree2D {
    int rows, cols;
    std::vector<std::vector<long long>> tree;

public:
    FenwickTree2D(int r, int c) : rows(r), cols(c), tree(r+1, std::vector<long long>(c+1, 0)) {}

    void update(int r, int c, long long delta) {
        for (int i = r; i <= rows; i += i & (-i))
            for (int j = c; j <= cols; j += j & (-j))
                tree[i][j] += delta;
    }

    long long query(int r, int c) const {
        long long sum = 0;
        for (int i = r; i > 0; i -= i & (-i))
            for (int j = c; j > 0; j -= j & (-j))
                sum += tree[i][j];
        return sum;
    }

    long long query(int r1, int c1, int r2, int c2) const {
        return query(r2, c2) - query(r1-1, c2) - query(r2, c1-1) + query(r1-1, c1-1);
    }
};

int main() {
    std::vector<int> arr = {3, 2, -1, 6, 5, 4, -3, 3, 7, 2, 3};
    FenwickTree bit(arr);

    std::cout << "Sum [1..5]: " << bit.query(1, 5) << "\n"; // 15
    std::cout << "Sum [3..8]: " << bit.query(3, 8) << "\n"; // 16
    bit.update(6, 3); // add 3 to index 6 (arr[5] becomes 7)
    std::cout << "After update, sum [1..11]: " << bit.query(1, 11) << "\n";

    // 2D BIT
    FenwickTree2D bit2d(4, 4);
    bit2d.update(1, 1, 3);
    bit2d.update(2, 3, 4);
    bit2d.update(3, 2, 2);
    std::cout << "2D sum [1,1..3,3]: " << bit2d.query(1,1,3,3) << "\n"; // 9
    return 0;
}
