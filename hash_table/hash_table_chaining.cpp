// Hash Table with Separate Chaining (linked list per bucket)
#include <iostream>
#include <list>
#include <vector>
#include <functional>
#include <stdexcept>

template <typename K, typename V>
class HashMap {
private:
    static const int DEFAULT_BUCKETS = 16;
    std::vector<std::list<std::pair<K,V>>> buckets;
    int sz;
    float max_load;

    int bucket_index(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }

    void rehash() {
        int new_size = buckets.size() * 2;
        std::vector<std::list<std::pair<K,V>>> new_buckets(new_size);
        for (auto& bucket : buckets) {
            for (auto& [k, v] : bucket) {
                int idx = std::hash<K>{}(k) % new_size;
                new_buckets[idx].emplace_back(k, v);
            }
        }
        buckets = std::move(new_buckets);
    }

public:
    HashMap(int init_buckets = DEFAULT_BUCKETS, float max_load = 0.75f)
        : buckets(init_buckets), sz(0), max_load(max_load) {}

    void insert(const K& key, const V& val) {
        if ((float)(sz + 1) / buckets.size() > max_load) rehash();
        int idx = bucket_index(key);
        for (auto& [k, v] : buckets[idx]) {
            if (k == key) { v = val; return; } // update
        }
        buckets[idx].emplace_back(key, val);
        ++sz;
    }

    bool get(const K& key, V& out) const {
        int idx = bucket_index(key);
        for (const auto& [k, v] : buckets[idx]) {
            if (k == key) { out = v; return true; }
        }
        return false;
    }

    V& operator[](const K& key) {
        int idx = bucket_index(key);
        for (auto& [k, v] : buckets[idx])
            if (k == key) return v;
        if ((float)(sz + 1) / buckets.size() > max_load) {
            rehash();
            idx = bucket_index(key);
        }
        buckets[idx].emplace_back(key, V{});
        ++sz;
        return buckets[idx].back().second;
    }

    bool remove(const K& key) {
        int idx = bucket_index(key);
        auto& bucket = buckets[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) { bucket.erase(it); --sz; return true; }
        }
        return false;
    }

    bool contains(const K& key) const {
        V tmp;
        return get(key, tmp);
    }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }

    void print() const {
        for (int i = 0; i < (int)buckets.size(); ++i) {
            if (!buckets[i].empty()) {
                std::cout << "[" << i << "]: ";
                for (const auto& [k, v] : buckets[i])
                    std::cout << "{" << k << ":" << v << "} ";
                std::cout << "\n";
            }
        }
    }
};

int main() {
    HashMap<std::string, int> map;
    map.insert("apple", 5);
    map.insert("banana", 3);
    map.insert("cherry", 8);
    map["date"] = 12;

    int val;
    if (map.get("apple", val)) std::cout << "apple: " << val << "\n"; // 5
    std::cout << "Contains banana: " << map.contains("banana") << "\n"; // 1
    map.remove("banana");
    std::cout << "After remove, size: " << map.size() << "\n"; // 3
    map.print();
    return 0;
}
