// Suffix Array + LCP Array — O(n log^2 n) construction, O(n) LCP (Kasai)
// Used for: pattern matching, longest repeated substring, longest common substring
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// ── Suffix Array (O(n log^2 n)) ───────────────────────────────────────────────
std::vector<int> buildSuffixArray(const std::string& s) {
    int n = s.size();
    std::vector<int> sa(n), rank_(n), tmp(n);
    std::iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) rank_[i] = s[i];

    for (int gap = 1; gap < n; gap <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rank_[a] != rank_[b]) return rank_[a] < rank_[b];
            int ra = (a + gap < n) ? rank_[a + gap] : -1;
            int rb = (b + gap < n) ? rank_[b + gap] : -1;
            return ra < rb;
        };
        std::sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i)
            tmp[sa[i]] = tmp[sa[i-1]] + (cmp(sa[i-1], sa[i]) ? 1 : 0);
        rank_ = tmp;
    }
    return sa;
}

// ── LCP Array (Kasai's Algorithm) — O(n) ─────────────────────────────────────
std::vector<int> buildLCP(const std::string& s, const std::vector<int>& sa) {
    int n = s.size();
    std::vector<int> rank_(n), lcp(n, 0);
    for (int i = 0; i < n; ++i) rank_[sa[i]] = i;

    int h = 0;
    for (int i = 0; i < n; ++i) {
        if (rank_[i] > 0) {
            int j = sa[rank_[i] - 1];
            while (i + h < n && j + h < n && s[i+h] == s[j+h]) ++h;
            lcp[rank_[i]] = h;
            if (h > 0) --h;
        }
    }
    return lcp;
}

// ── Pattern matching using suffix array (O(m log n)) ──────────────────────────
std::pair<int,int> searchPattern(const std::string& text, const std::string& pattern,
                                  const std::vector<int>& sa) {
    int n = text.size(), m = pattern.size();
    // Find leftmost occurrence
    int lo = 0, hi = n - 1, left = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        auto cmp = text.compare(sa[mid], m, pattern);
        if (cmp < 0) lo = mid + 1;
        else if (cmp > 0) hi = mid - 1;
        else { left = mid; hi = mid - 1; }
    }
    if (left == -1) return {-1, -1};
    lo = left; hi = n - 1;
    int right = left;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        auto cmp = text.compare(sa[mid], m, pattern);
        if (cmp <= 0) { if (cmp == 0) right = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return {left, right};
}

// ── Longest Repeated Substring ────────────────────────────────────────────────
std::string longestRepeatedSubstring(const std::string& s) {
    auto sa  = buildSuffixArray(s);
    auto lcp = buildLCP(s, sa);
    int maxLen = *std::max_element(lcp.begin(), lcp.end());
    int idx = std::max_element(lcp.begin(), lcp.end()) - lcp.begin();
    return s.substr(sa[idx], maxLen);
}

// ── Longest Common Substring of two strings ──────────────────────────────────
std::string longestCommonSubstring(const std::string& s1, const std::string& s2) {
    std::string combined = s1 + "$" + s2 + "#";
    int n1 = s1.size(), n = combined.size();
    auto sa  = buildSuffixArray(combined);
    auto lcp = buildLCP(combined, sa);

    int best = 0, bestIdx = -1;
    for (int i = 1; i < n; ++i) {
        bool fromS1 = (sa[i] < n1), prevFromS1 = (sa[i-1] < n1);
        if (fromS1 != prevFromS1 && lcp[i] > best) {
            best = lcp[i]; bestIdx = sa[i];
        }
    }
    return (bestIdx == -1) ? "" : combined.substr(bestIdx, best);
}

int main() {
    std::string s = "banana";
    auto sa  = buildSuffixArray(s);
    auto lcp = buildLCP(s, sa);

    std::cout << "String: " << s << "\n";
    std::cout << "Suffix Array:\n";
    for (int i = 0; i < (int)sa.size(); ++i)
        std::cout << "  sa[" << i << "] = " << sa[i] << "  -> \"" << s.substr(sa[i]) << "\"\n";

    std::cout << "LCP Array: ";
    for (int x : lcp) std::cout << x << " "; std::cout << "\n";

    auto [l, r] = searchPattern(s, "an", sa);
    std::cout << "Pattern 'an' found at sa[" << l << ".." << r << "]: positions ";
    for (int i = l; i <= r; ++i) std::cout << sa[i] << " ";
    std::cout << "\n";

    std::cout << "Longest repeated: " << longestRepeatedSubstring("abcabc") << "\n"; // abc
    std::cout << "Longest common substring(abcde, cdegh): "
              << longestCommonSubstring("abcde", "cdegh") << "\n"; // cde
    return 0;
}
