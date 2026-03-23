// Hash Table with Open Addressing (Linear Probing + Double Hashing)
#include <iostream>
#include <vector>
#include <functional>
#include <optional>

template <typename K, typename V>
class OpenHashMap {
private:
    enum class State { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        K key;
        V val;
        State state = State::EMPTY;
    };

    std::vector<Slot> table;
    int sz;
    int capacity;
    static const float LOAD_FACTOR;

    int hash1(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

    int hash2(const K& key) const {
        // Double hashing: must be co-prime with capacity
        return 1 + (std::hash<K>{}(key) % (capacity - 1));
    }

    int probe(const K& key, int i) const {
        return (hash1(key) + i * hash2(key)) % capacity;
    }

    void rehash() {
        int old_cap = capacity;
        capacity *= 2;
        std::vector<Slot> old_table = std::move(table);
        table.assign(capacity, Slot{});
        sz = 0;
        for (auto& slot : old_table)
            if (slot.state == State::OCCUPIED)
                insert(slot.key, slot.val);
    }

public:
    OpenHashMap(int cap = 17) : table(cap), sz(0), capacity(cap) {}

    void insert(const K& key, const V& val) {
        if ((float)sz / capacity >= LOAD_FACTOR) rehash();
        int i = 0, first_deleted = -1;
        while (i < capacity) {
            int idx = probe(key, i);
            if (table[idx].state == State::EMPTY) {
                int insert_at = (first_deleted != -1) ? first_deleted : idx;
                table[insert_at] = {key, val, State::OCCUPIED};
                ++sz;
                return;
            }
            if (table[idx].state == State::DELETED && first_deleted == -1)
                first_deleted = idx;
            if (table[idx].state == State::OCCUPIED && table[idx].key == key) {
                table[idx].val = val; return; // update
            }
            ++i;
        }
    }

    std::optional<V> get(const K& key) const {
        int i = 0;
        while (i < capacity) {
            int idx = probe(key, i);
            if (table[idx].state == State::EMPTY) return std::nullopt;
            if (table[idx].state == State::OCCUPIED && table[idx].key == key)
                return table[idx].val;
            ++i;
        }
        return std::nullopt;
    }

    bool remove(const K& key) {
        int i = 0;
        while (i < capacity) {
            int idx = probe(key, i);
            if (table[idx].state == State::EMPTY) return false;
            if (table[idx].state == State::OCCUPIED && table[idx].key == key) {
                table[idx].state = State::DELETED;
                --sz;
                return true;
            }
            ++i;
        }
        return false;
    }

    bool contains(const K& key) const { return get(key).has_value(); }
    int size() const { return sz; }

    void print() const {
        for (int i = 0; i < capacity; ++i) {
            if (table[i].state == State::OCCUPIED)
                std::cout << "[" << i << "] " << table[i].key << " -> " << table[i].val << "\n";
        }
    }
};

template <typename K, typename V>
const float OpenHashMap<K,V>::LOAD_FACTOR = 0.6f;

int main() {
    OpenHashMap<std::string, int> map;
    map.insert("one", 1);
    map.insert("two", 2);
    map.insert("three", 3);

    auto v = map.get("two");
    if (v) std::cout << "two = " << *v << "\n"; // 2

    map.remove("two");
    std::cout << "After remove, contains two: " << map.contains("two") << "\n"; // 0
    map.print();
    return 0;
}
