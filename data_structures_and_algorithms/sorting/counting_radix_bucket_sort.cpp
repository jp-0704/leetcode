// Counting Sort, Radix Sort, Bucket Sort — linear time sorts
#include <iostream>
#include <vector>
#include <algorithm>

// ── Counting Sort ─────────────────────────────────────────────────────────────
// O(n + k), stable, only for non-negative integers in range [0, k]
void countingSort(std::vector<int>& arr, int max_val) {
    std::vector<int> count(max_val + 1, 0);
    for (int x : arr) ++count[x];
    int idx = 0;
    for (int v = 0; v <= max_val; ++v)
        while (count[v]--) arr[idx++] = v;
}

// Stable counting sort (preserves relative order) — used inside radix sort
void countingSortStable(std::vector<int>& arr, int exp) {
    int n = arr.size();
    std::vector<int> output(n), count(10, 0);
    for (int x : arr) ++count[(x / exp) % 10];
    for (int i = 1; i < 10; ++i) count[i] += count[i-1];
    for (int i = n - 1; i >= 0; --i) {
        int digit = (arr[i] / exp) % 10;
        output[--count[digit]] = arr[i];
    }
    arr = output;
}

// ── Radix Sort (LSD) ──────────────────────────────────────────────────────────
// O(d * (n + k)), stable, d = number of digits
void radixSort(std::vector<int>& arr) {
    int max_val = *std::max_element(arr.begin(), arr.end());
    for (int exp = 1; max_val / exp > 0; exp *= 10)
        countingSortStable(arr, exp);
}

// ── Bucket Sort ───────────────────────────────────────────────────────────────
// O(n + k) average, best for uniformly distributed floats in [0, 1)
void bucketSort(std::vector<float>& arr) {
    int n = arr.size();
    std::vector<std::vector<float>> buckets(n);
    for (float x : arr)
        buckets[int(n * x)].push_back(x);
    for (auto& b : buckets)
        std::sort(b.begin(), b.end());
    int idx = 0;
    for (auto& b : buckets)
        for (float x : b) arr[idx++] = x;
}

// Bucket sort for integers in a known range
void bucketSortInt(std::vector<int>& arr, int min_val, int max_val) {
    int n = arr.size();
    int range = max_val - min_val + 1;
    int bucket_size = std::max(1, range / n);
    int num_buckets = (range + bucket_size - 1) / bucket_size;
    std::vector<std::vector<int>> buckets(num_buckets);
    for (int x : arr)
        buckets[(x - min_val) / bucket_size].push_back(x);
    int idx = 0;
    for (auto& b : buckets) {
        std::sort(b.begin(), b.end());
        for (int x : b) arr[idx++] = x;
    }
}

void print(const std::vector<int>& arr) {
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::vector<int> a = {4, 2, 2, 8, 3, 3, 1};
    countingSort(a, 8);
    std::cout << "Counting Sort: "; print(a);

    std::vector<int> b = {170, 45, 75, 90, 802, 24, 2, 66};
    radixSort(b);
    std::cout << "Radix Sort: "; print(b);

    std::vector<float> c = {0.897f, 0.565f, 0.656f, 0.123f, 0.665f, 0.343f};
    bucketSort(c);
    std::cout << "Bucket Sort: ";
    for (float x : c) std::cout << x << " ";
    std::cout << "\n";

    std::vector<int> d = {29, 25, 3, 49, 9, 37, 21, 43};
    bucketSortInt(d, 3, 49);
    std::cout << "Bucket Sort (int): "; print(d);
    return 0;
}
