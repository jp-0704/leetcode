// String Matching Algorithms: KMP, Rabin-Karp, Z-Algorithm, Boyer-Moore
#include <iostream>
#include <vector>
#include <string>

// ── KMP (Knuth-Morris-Pratt) ─────────────────────────────────────────────────
// O(n + m), builds failure function to skip comparisons
std::vector<int> buildLPS(const std::string& pattern) {
    int m = pattern.size();
    std::vector<int> lps(m, 0);
    int len = 0, i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) { lps[i++] = ++len; }
        else if (len) len = lps[len - 1];
        else lps[i++] = 0;
    }
    return lps;
}

std::vector<int> kmpSearch(const std::string& text, const std::string& pattern) {
    std::vector<int> positions;
    int n = text.size(), m = pattern.size();
    auto lps = buildLPS(pattern);
    int i = 0, j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) { ++i; ++j; }
        if (j == m) { positions.push_back(i - j); j = lps[j - 1]; }
        else if (i < n && text[i] != pattern[j]) {
            if (j) j = lps[j - 1];
            else ++i;
        }
    }
    return positions;
}

// ── Rabin-Karp ────────────────────────────────────────────────────────────────
// O(n + m) average, uses rolling hash
std::vector<int> rabinKarp(const std::string& text, const std::string& pattern) {
    const long long BASE = 31, MOD = 1e9 + 9;
    int n = text.size(), m = pattern.size();
    std::vector<int> positions;

    auto charVal = [](char c) { return c - 'a' + 1; };

    long long patHash = 0, textHash = 0, power = 1;
    for (int i = 0; i < m - 1; ++i) power = power * BASE % MOD;

    for (int i = 0; i < m; ++i) {
        patHash  = (patHash  * BASE + charVal(pattern[i])) % MOD;
        textHash = (textHash * BASE + charVal(text[i]))    % MOD;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (patHash == textHash) {
            if (text.substr(i, m) == pattern) positions.push_back(i);
        }
        if (i < n - m) {
            textHash = (BASE * (textHash - charVal(text[i]) * power % MOD + MOD) + charVal(text[i+m])) % MOD;
        }
    }
    return positions;
}

// ── Z-Algorithm ───────────────────────────────────────────────────────────────
// Z[i] = length of longest substring starting at i that is a prefix of the string
// O(n)
std::vector<int> zFunction(const std::string& s) {
    int n = s.size();
    std::vector<int> z(n, 0);
    z[0] = n;
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i < r) z[i] = std::min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}

std::vector<int> zSearch(const std::string& text, const std::string& pattern) {
    std::string s = pattern + "$" + text;
    auto z = zFunction(s);
    int m = pattern.size();
    std::vector<int> positions;
    for (int i = m + 1; i < (int)s.size(); ++i)
        if (z[i] == m) positions.push_back(i - m - 1);
    return positions;
}

// ── Boyer-Moore (Bad Character heuristic) ────────────────────────────────────
std::vector<int> boyerMoore(const std::string& text, const std::string& pattern) {
    int n = text.size(), m = pattern.size();
    std::vector<int> badChar(256, -1);
    for (int i = 0; i < m; ++i) badChar[(unsigned char)pattern[i]] = i;

    std::vector<int> positions;
    int s = 0;
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j]) --j;
        if (j < 0) {
            positions.push_back(s);
            s += (s + m < n) ? m - badChar[(unsigned char)text[s + m]] : 1;
        } else {
            s += std::max(1, j - badChar[(unsigned char)text[s + j]]);
        }
    }
    return positions;
}

// ── Longest Palindromic Substring (Manacher's Algorithm) ─────────────────────
std::string manacher(const std::string& s) {
    std::string t = "#";
    for (char c : s) { t += c; t += '#'; }
    int n = t.size();
    std::vector<int> p(n, 0);
    int c = 0, r = 0;
    for (int i = 0; i < n; ++i) {
        if (i < r) p[i] = std::min(r - i, p[2*c - i]);
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < n && t[i-p[i]-1] == t[i+p[i]+1]) ++p[i];
        if (i + p[i] > r) { c = i; r = i + p[i]; }
    }
    int maxLen = 0, center = 0;
    for (int i = 0; i < n; ++i) if (p[i] > maxLen) { maxLen = p[i]; center = i; }
    return s.substr((center - maxLen) / 2, maxLen);
}

void printPositions(const std::vector<int>& pos) {
    for (int p : pos) std::cout << p << " "; std::cout << "\n";
}

int main() {
    std::string text = "aababcabcababcabc";
    std::string pat  = "ababcabc";

    std::cout << "KMP:         "; printPositions(kmpSearch(text, pat));
    std::cout << "Rabin-Karp:  "; printPositions(rabinKarp(text, pat));
    std::cout << "Z-Search:    "; printPositions(zSearch(text, pat));
    std::cout << "Boyer-Moore: "; printPositions(boyerMoore(text, pat));

    std::string s = "babad";
    std::cout << "Longest palindrome of '" << s << "': " << manacher(s) << "\n";
    return 0;
}
