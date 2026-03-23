// String Algorithms — Aho-Corasick, Rolling Hash, Suffix Automaton
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

// ── Aho-Corasick — Multi-pattern search O(n + m + z) ─────────────────────────
class AhoCorasick {
    struct State {
        std::unordered_map<char,int> next;
        int fail = 0;
        std::vector<int> output; // pattern indices that end here
    };

    std::vector<State> trie;
    std::vector<std::string> patterns;

    void build() {
        std::queue<int> q;
        for (auto& [c, s] : trie[0].next) { trie[s].fail = 0; q.push(s); }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& [c, v] : trie[u].next) {
                int f = trie[u].fail;
                while (f && !trie[f].next.count(c)) f = trie[f].fail;
                trie[v].fail = (trie[f].next.count(c) && trie[f].next[c] != v)
                               ? trie[f].next[c] : 0;
                auto& fo = trie[trie[v].fail].output;
                trie[v].output.insert(trie[v].output.end(), fo.begin(), fo.end());
                q.push(v);
            }
        }
    }

public:
    AhoCorasick() { trie.push_back({}); }

    void addPattern(const std::string& pattern) {
        int cur = 0;
        for (char c : pattern) {
            if (!trie[cur].next.count(c)) {
                trie[cur].next[c] = trie.size();
                trie.push_back({});
            }
            cur = trie[cur].next[c];
        }
        trie[cur].output.push_back(patterns.size());
        patterns.push_back(pattern);
    }

    void buildAutomaton() { build(); }

    // Returns {position, pattern_index} for all matches
    std::vector<std::pair<int,int>> search(const std::string& text) {
        std::vector<std::pair<int,int>> results;
        int cur = 0;
        for (int i = 0; i < (int)text.size(); ++i) {
            char c = text[i];
            while (cur && !trie[cur].next.count(c)) cur = trie[cur].fail;
            if (trie[cur].next.count(c)) cur = trie[cur].next[c];
            for (int pi : trie[cur].output)
                results.push_back({i - (int)patterns[pi].size() + 1, pi});
        }
        return results;
    }
};

// ── Rolling Hash (Polynomial Hash) ───────────────────────────────────────────
struct RollingHash {
    static const long long MOD1 = 1e9+7, MOD2 = 1e9+9;
    static const long long BASE1 = 31, BASE2 = 37;
    std::vector<long long> h1, h2, p1, p2;

    RollingHash(const std::string& s) {
        int n = s.size();
        h1.resize(n+1,0); h2.resize(n+1,0);
        p1.resize(n+1,1); p2.resize(n+1,1);
        for (int i = 0; i < n; ++i) {
            h1[i+1] = (h1[i]*BASE1 + s[i]-'a'+1) % MOD1;
            h2[i+1] = (h2[i]*BASE2 + s[i]-'a'+1) % MOD2;
            p1[i+1] = p1[i]*BASE1 % MOD1;
            p2[i+1] = p2[i]*BASE2 % MOD2;
        }
    }

    std::pair<long long,long long> getHash(int l, int r) const {
        long long v1 = (h1[r+1] - h1[l]*p1[r-l+1] % MOD1 + MOD1) % MOD1;
        long long v2 = (h2[r+1] - h2[l]*p2[r-l+1] % MOD2 + MOD2) % MOD2;
        return {v1, v2};
    }
};

// Longest common substring using rolling hash + binary search
int longestCommonSubstrBinSearch(const std::string& s1, const std::string& s2) {
    int lo = 0, hi = std::min(s1.size(), s2.size()), ans = 0;
    RollingHash rh1(s1), rh2(s2);
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        std::unordered_map<long long, std::vector<int>> seen;
        bool found = false;
        for (int i = 0; i + mid <= (int)s1.size(); ++i) {
            auto [h1, h2] = rh1.getHash(i, i+mid-1);
            seen[h1].push_back(i);
        }
        for (int j = 0; j + mid <= (int)s2.size() && !found; ++j) {
            auto [h1, h2] = rh2.getHash(j, j+mid-1);
            if (seen.count(h1)) found = true;
        }
        if (found) { ans = mid; lo = mid+1; } else hi = mid-1;
    }
    return ans;
}

// ── String Hashing — find duplicate substrings ────────────────────────────────
bool hasDuplicateSubstring(const std::string& s, int len) {
    RollingHash rh(s);
    std::unordered_map<long long, int> seen;
    for (int i = 0; i + len <= (int)s.size(); ++i) {
        auto [h, _] = rh.getHash(i, i+len-1);
        if (seen.count(h)) return true;
        seen[h] = i;
    }
    return false;
}

// ── Longest Duplicate Substring (Binary search + rolling hash) ────────────────
std::string longestDupSubstring(const std::string& s) {
    int lo = 1, hi = s.size()-1, start = -1, len = 0;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        RollingHash rh(s);
        std::unordered_map<long long, int> seen;
        bool found = false;
        for (int i = 0; i + mid <= (int)s.size(); ++i) {
            auto [h, _] = rh.getHash(i, i+mid-1);
            if (seen.count(h)) { found = true; start = seen[h]; len = mid; break; }
            seen[h] = i;
        }
        if (found) lo = mid+1;
        else hi = mid-1;
    }
    return (start == -1) ? "" : s.substr(start, len);
}

// ── Regular Expression Matching (DP) ─────────────────────────────────────────
bool regexMatch(const std::string& s, const std::string& p) {
    int m = s.size(), n = p.size();
    std::vector<std::vector<bool>> dp(m+1, std::vector<bool>(n+1, false));
    dp[0][0] = true;
    for (int j = 1; j <= n; j += 2) {
        if (p[j] == '*') dp[0][j+1] = dp[0][j-1];
        else break;
    }
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) {
            if (p[j-1] == '*') {
                dp[i][j] = dp[i][j-2]; // zero occurrences
                if (p[j-2] == '.' || p[j-2] == s[i-1])
                    dp[i][j] = dp[i][j] || dp[i-1][j];
            } else {
                dp[i][j] = dp[i-1][j-1] && (p[j-1] == '.' || p[j-1] == s[i-1]);
            }
        }
    return dp[m][n];
}

int main() {
    // Aho-Corasick
    AhoCorasick ac;
    ac.addPattern("he"); ac.addPattern("she"); ac.addPattern("his"); ac.addPattern("hers");
    ac.buildAutomaton();
    auto matches = ac.search("ushers");
    std::cout << "Aho-Corasick matches in 'ushers':\n";
    for (auto [pos, pi] : matches)
        std::cout << "  Pattern found at pos " << pos << ": " << pi << "\n";

    // Rolling hash
    std::string s1 = "abcde", s2 = "cdefg";
    std::cout << "\nLongest common substring: "
              << longestCommonSubstrBinSearch(s1, s2) << "\n"; // 3 (cde)

    std::string s = "banana";
    std::cout << "Longest duplicate substring of '" << s << "': "
              << longestDupSubstring(s) << "\n"; // ana

    // Regex
    std::cout << "\nRegex 'aa' matches 'a*': " << regexMatch("aa","a*") << "\n"; // 1
    std::cout << "Regex 'ab' matches '.*': " << regexMatch("ab",".*") << "\n"; // 1
    return 0;
}
