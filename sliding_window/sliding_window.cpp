// Sliding Window — fixed and variable size window problems
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <deque>
#include <climits>

// ── Fixed Window: Max Sum Subarray of size k ──────────────────────────────────
int maxSumSubarray(const std::vector<int>& nums, int k) {
    int n = nums.size(), windowSum = 0, maxSum = INT_MIN;
    for (int i = 0; i < n; ++i) {
        windowSum += nums[i];
        if (i >= k - 1) {
            maxSum = std::max(maxSum, windowSum);
            windowSum -= nums[i - k + 1];
        }
    }
    return maxSum;
}

// ── Fixed Window: Sliding Window Maximum ─────────────────────────────────────
std::vector<int> slidingWindowMax(const std::vector<int>& nums, int k) {
    std::deque<int> dq; // stores indices, decreasing by value
    std::vector<int> result;
    for (int i = 0; i < (int)nums.size(); ++i) {
        while (!dq.empty() && dq.front() < i - k + 1) dq.pop_front();
        while (!dq.empty() && nums[dq.back()] < nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) result.push_back(nums[dq.front()]);
    }
    return result;
}

// ── Variable Window: Longest Substring Without Repeating Characters ───────────
int lengthOfLongestSubstring(const std::string& s) {
    std::unordered_map<char, int> lastSeen;
    int maxLen = 0, lo = 0;
    for (int hi = 0; hi < (int)s.size(); ++hi) {
        if (lastSeen.count(s[hi]) && lastSeen[s[hi]] >= lo)
            lo = lastSeen[s[hi]] + 1;
        lastSeen[s[hi]] = hi;
        maxLen = std::max(maxLen, hi - lo + 1);
    }
    return maxLen;
}

// ── Variable Window: Longest Substring with at Most K Distinct Characters ─────
int longestKDistinct(const std::string& s, int k) {
    std::unordered_map<char,int> freq;
    int lo = 0, maxLen = 0;
    for (int hi = 0; hi < (int)s.size(); ++hi) {
        ++freq[s[hi]];
        while ((int)freq.size() > k) {
            if (--freq[s[lo]] == 0) freq.erase(s[lo]);
            ++lo;
        }
        maxLen = std::max(maxLen, hi - lo + 1);
    }
    return maxLen;
}

// ── Variable Window: Minimum Size Subarray Sum >= target ─────────────────────
int minSubarrayLen(int target, const std::vector<int>& nums) {
    int lo = 0, sum = 0, minLen = INT_MAX;
    for (int hi = 0; hi < (int)nums.size(); ++hi) {
        sum += nums[hi];
        while (sum >= target) {
            minLen = std::min(minLen, hi - lo + 1);
            sum -= nums[lo++];
        }
    }
    return (minLen == INT_MAX) ? 0 : minLen;
}

// ── Variable Window: Longest Subarray with Sum <= k ──────────────────────────
int longestSubarraySum(const std::vector<int>& nums, int k) {
    int lo = 0, sum = 0, maxLen = 0;
    for (int hi = 0; hi < (int)nums.size(); ++hi) {
        sum += nums[hi];
        while (sum > k) sum -= nums[lo++];
        maxLen = std::max(maxLen, hi - lo + 1);
    }
    return maxLen;
}

// ── Fixed Window: Find All Anagrams ───────────────────────────────────────────
std::vector<int> findAnagrams(const std::string& s, const std::string& p) {
    std::vector<int> result;
    if (s.size() < p.size()) return result;
    std::vector<int> pCount(26, 0), wCount(26, 0);
    for (char c : p) ++pCount[c - 'a'];
    int k = p.size();
    for (int i = 0; i < (int)s.size(); ++i) {
        ++wCount[s[i] - 'a'];
        if (i >= k) --wCount[s[i-k] - 'a'];
        if (pCount == wCount) result.push_back(i - k + 1);
    }
    return result;
}

// ── Variable Window: Max Consecutive Ones III (flip at most k zeros) ──────────
int longestOnes(const std::vector<int>& nums, int k) {
    int lo = 0, zeros = 0, maxLen = 0;
    for (int hi = 0; hi < (int)nums.size(); ++hi) {
        if (nums[hi] == 0) ++zeros;
        while (zeros > k) { if (nums[lo++] == 0) --zeros; }
        maxLen = std::max(maxLen, hi - lo + 1);
    }
    return maxLen;
}

// ── Variable Window: Permutation in String ────────────────────────────────────
bool checkInclusion(const std::string& s1, const std::string& s2) {
    if (s1.size() > s2.size()) return false;
    std::vector<int> count(26, 0);
    for (char c : s1) ++count[c - 'a'];
    int k = s1.size(), match = 0;
    for (int i = 0; i < (int)s2.size(); ++i) {
        if (--count[s2[i] - 'a'] >= 0) ++match;
        if (i >= k && ++count[s2[i-k] - 'a'] > 0) --match;
        if (match == k) return true;
    }
    return false;
}

void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " "; std::cout << "\n";
}

int main() {
    std::cout << "Max sum subarray k=3: "
              << maxSumSubarray({2,1,5,1,3,2}, 3) << "\n"; // 9

    auto swmax = slidingWindowMax({1,3,-1,-3,5,3,6,7}, 3);
    std::cout << "Sliding window max: "; print(swmax); // 3 3 5 5 6 7

    std::cout << "Longest substring no repeat 'abcabcbb': "
              << lengthOfLongestSubstring("abcabcbb") << "\n"; // 3

    std::cout << "Longest 2-distinct 'eceba': "
              << longestKDistinct("eceba", 2) << "\n"; // 3

    std::cout << "Min subarray sum>=7 [2,3,1,2,4,3]: "
              << minSubarrayLen(7, {2,3,1,2,4,3}) << "\n"; // 2

    auto anagrams = findAnagrams("cbaebabacd", "abc");
    std::cout << "Anagram positions: "; print(anagrams); // 0 6

    std::cout << "Longest ones k=2 [1,1,1,0,0,0,1,1,1,1,0]: "
              << longestOnes({1,1,1,0,0,0,1,1,1,1,0}, 2) << "\n"; // 6

    std::cout << "Permutation 'ab' in 'eidbaooo': "
              << checkInclusion("ab", "eidbaooo") << "\n"; // 1
    return 0;
}
