// Trie (Prefix Tree) — insert, search, prefix, delete, autocomplete
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

// ── Array-based Trie (fixed alphabet 'a'-'z') ─────────────────────────────────
struct TrieNode {
    TrieNode* children[26];
    bool isEnd;
    int count; // words passing through this node

    TrieNode() : isEnd(false), count(0) {
        std::fill(children, children + 26, nullptr);
    }
};

class Trie {
private:
    TrieNode* root;

    void collectWords(TrieNode* node, std::string& current, std::vector<std::string>& results) {
        if (node->isEnd) results.push_back(current);
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                current.push_back('a' + i);
                collectWords(node->children[i], current, results);
                current.pop_back();
            }
        }
    }

    bool deleteHelper(TrieNode* node, const std::string& word, int depth) {
        if (!node) return false;
        if (depth == (int)word.size()) {
            if (node->isEnd) { node->isEnd = false; --node->count; }
            return true;
        }
        int idx = word[depth] - 'a';
        deleteHelper(node->children[idx], word, depth + 1);
        --node->count;
        return true;
    }

    void destroy(TrieNode* node) {
        if (!node) return;
        for (auto* child : node->children) destroy(child);
        delete node;
    }

public:
    Trie() : root(new TrieNode()) {}
    ~Trie() { destroy(root); }

    void insert(const std::string& word) {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->children[idx]) cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
            ++cur->count;
        }
        cur->isEnd = true;
    }

    bool search(const std::string& word) const {
        TrieNode* cur = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return cur->isEnd;
    }

    bool startsWith(const std::string& prefix) const {
        TrieNode* cur = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!cur->children[idx]) return false;
            cur = cur->children[idx];
        }
        return true;
    }

    // Count words with given prefix
    int countWithPrefix(const std::string& prefix) const {
        TrieNode* cur = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!cur->children[idx]) return 0;
            cur = cur->children[idx];
        }
        return cur->count;
    }

    // Autocomplete: all words with prefix
    std::vector<std::string> autocomplete(const std::string& prefix) {
        TrieNode* cur = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!cur->children[idx]) return {};
            cur = cur->children[idx];
        }
        std::vector<std::string> results;
        std::string word = prefix;
        collectWords(cur, word, results);
        return results;
    }

    void deleteWord(const std::string& word) { deleteHelper(root, word, 0); }
};

// ── XOR Trie (for maximum XOR) ────────────────────────────────────────────────
class XORTrie {
    struct XNode { XNode* ch[2] = {nullptr, nullptr}; };
    XNode* root;
    static const int BITS = 30;

public:
    XORTrie() : root(new XNode()) {}

    void insert(int num) {
        XNode* cur = root;
        for (int i = BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (!cur->ch[bit]) cur->ch[bit] = new XNode();
            cur = cur->ch[bit];
        }
    }

    int maxXOR(int num) const {
        XNode* cur = root;
        int result = 0;
        for (int i = BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            int want = 1 - bit;
            if (cur->ch[want]) { result |= (1 << i); cur = cur->ch[want]; }
            else if (cur->ch[bit]) cur = cur->ch[bit];
        }
        return result;
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    trie.insert("apply");
    trie.insert("banana");

    std::cout << "Search 'app': "    << trie.search("app")    << "\n"; // 1
    std::cout << "Search 'appl': "   << trie.search("appl")   << "\n"; // 0
    std::cout << "Prefix 'appl': "   << trie.startsWith("appl") << "\n"; // 1
    std::cout << "Count 'app': "     << trie.countWithPrefix("app") << "\n";

    auto suggestions = trie.autocomplete("app");
    std::cout << "Autocomplete 'app': ";
    for (const auto& s : suggestions) std::cout << s << " ";
    std::cout << "\n";

    XORTrie xorTrie;
    std::vector<int> nums = {3, 10, 5, 25, 2, 8};
    for (int x : nums) xorTrie.insert(x);
    int maxXor = 0;
    for (int x : nums) maxXor = std::max(maxXor, xorTrie.maxXOR(x));
    std::cout << "Max XOR: " << maxXor << "\n"; // 28
    return 0;
}
