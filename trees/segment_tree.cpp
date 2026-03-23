// Segment Tree — range queries and point updates in O(log n)
// Supports: range sum, range min, range max
#include <iostream>
#include <vector>
#include <climits>
#include <functional>

// Generic Segment Tree
template <typename T>
class SegmentTree {
private:
    int n;
    std::vector<T> tree;
    std::function<T(T, T)> combine;
    T identity;

    void build(const std::vector<T>& arr, int node, int lo, int hi) {
        if (lo == hi) { tree[node] = arr[lo]; return; }
        int mid = (lo + hi) / 2;
        build(arr, 2*node, lo, mid);
        build(arr, 2*node+1, mid+1, hi);
        tree[node] = combine(tree[2*node], tree[2*node+1]);
    }

    void update(int node, int lo, int hi, int idx, T val) {
        if (lo == hi) { tree[node] = val; return; }
        int mid = (lo + hi) / 2;
        if (idx <= mid) update(2*node, lo, mid, idx, val);
        else            update(2*node+1, mid+1, hi, idx, val);
        tree[node] = combine(tree[2*node], tree[2*node+1]);
    }

    T query(int node, int lo, int hi, int l, int r) const {
        if (r < lo || hi < l) return identity;
        if (l <= lo && hi <= r) return tree[node];
        int mid = (lo + hi) / 2;
        return combine(query(2*node, lo, mid, l, r),
                       query(2*node+1, mid+1, hi, l, r));
    }

public:
    SegmentTree(const std::vector<T>& arr, std::function<T(T,T)> fn, T id)
        : n(arr.size()), tree(4 * arr.size(), id), combine(fn), identity(id) {
        build(arr, 1, 0, n-1);
    }

    void update(int idx, T val) { update(1, 0, n-1, idx, val); }

    T query(int l, int r) const { return query(1, 0, n-1, l, r); }
};

// ── Lazy Propagation Segment Tree (range update, range query) ─────────────────
class LazySegTree {
    int n;
    std::vector<long long> tree, lazy;

    void build(const std::vector<int>& arr, int node, int lo, int hi) {
        lazy[node] = 0;
        if (lo == hi) { tree[node] = arr[lo]; return; }
        int mid = (lo + hi) / 2;
        build(arr, 2*node, lo, mid);
        build(arr, 2*node+1, mid+1, hi);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    void pushDown(int node, int lo, int hi) {
        if (lazy[node]) {
            int mid = (lo + hi) / 2;
            tree[2*node]   += lazy[node] * (mid - lo + 1);
            tree[2*node+1] += lazy[node] * (hi - mid);
            lazy[2*node]   += lazy[node];
            lazy[2*node+1] += lazy[node];
            lazy[node] = 0;
        }
    }

    void update(int node, int lo, int hi, int l, int r, long long val) {
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) {
            tree[node] += val * (hi - lo + 1);
            lazy[node] += val;
            return;
        }
        pushDown(node, lo, hi);
        int mid = (lo + hi) / 2;
        update(2*node, lo, mid, l, r, val);
        update(2*node+1, mid+1, hi, l, r, val);
        tree[node] = tree[2*node] + tree[2*node+1];
    }

    long long query(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) return 0;
        if (l <= lo && hi <= r) return tree[node];
        pushDown(node, lo, hi);
        int mid = (lo + hi) / 2;
        return query(2*node, lo, mid, l, r) +
               query(2*node+1, mid+1, hi, l, r);
    }

public:
    LazySegTree(const std::vector<int>& arr)
        : n(arr.size()), tree(4*arr.size()), lazy(4*arr.size(), 0) {
        build(arr, 1, 0, n-1);
    }

    void rangeUpdate(int l, int r, long long val) { update(1, 0, n-1, l, r, val); }
    long long rangeQuery(int l, int r) { return query(1, 0, n-1, l, r); }
};

int main() {
    std::vector<int> arr = {1, 3, 5, 7, 9, 11};

    // Sum segment tree
    SegmentTree<int> sumST(arr, [](int a, int b){ return a+b; }, 0);
    std::cout << "Sum [1,3]: " << sumST.query(1, 3) << "\n"; // 15
    sumST.update(1, 10);
    std::cout << "Sum [1,3] after update: " << sumST.query(1, 3) << "\n"; // 22

    // Min segment tree
    SegmentTree<int> minST(arr, [](int a, int b){ return std::min(a,b); }, INT_MAX);
    std::cout << "Min [2,5]: " << minST.query(2, 5) << "\n"; // 5

    // Max segment tree
    SegmentTree<int> maxST(arr, [](int a, int b){ return std::max(a,b); }, INT_MIN);
    std::cout << "Max [0,4]: " << maxST.query(0, 4) << "\n"; // 9

    // Lazy propagation
    LazySegTree lazy(arr);
    std::cout << "Range sum [0,5]: " << lazy.rangeQuery(0, 5) << "\n"; // 36
    lazy.rangeUpdate(1, 3, 5); // add 5 to [1..3]
    std::cout << "After range add 5 to [1,3], sum [0,5]: "
              << lazy.rangeQuery(0, 5) << "\n"; // 56
    return 0;
}
