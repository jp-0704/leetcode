// Two Pointers — classic problems
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// ── Two Sum (sorted array) ────────────────────────────────────────────────────
std::pair<int,int> twoSum(const std::vector<int>& arr, int target) {
    int lo = 0, hi = arr.size() - 1;
    while (lo < hi) {
        int s = arr[lo] + arr[hi];
        if (s == target) return {lo, hi};
        if (s < target) ++lo;
        else --hi;
    }
    return {-1, -1};
}

// ── 3Sum ─────────────────────────────────────────────────────────────────────
std::vector<std::vector<int>> threeSum(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    int n = nums.size();
    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i-1]) continue;
        int lo = i+1, hi = n-1;
        while (lo < hi) {
            int s = nums[i] + nums[lo] + nums[hi];
            if (s == 0) {
                result.push_back({nums[i], nums[lo], nums[hi]});
                while (lo < hi && nums[lo] == nums[lo+1]) ++lo;
                while (lo < hi && nums[hi] == nums[hi-1]) --hi;
                ++lo; --hi;
            } else if (s < 0) ++lo;
            else --hi;
        }
    }
    return result;
}

// ── Trapping Rain Water ───────────────────────────────────────────────────────
int trapWater(const std::vector<int>& height) {
    int lo = 0, hi = height.size()-1, water = 0;
    int maxLeft = 0, maxRight = 0;
    while (lo < hi) {
        if (height[lo] < height[hi]) {
            if (height[lo] >= maxLeft) maxLeft = height[lo];
            else water += maxLeft - height[lo];
            ++lo;
        } else {
            if (height[hi] >= maxRight) maxRight = height[hi];
            else water += maxRight - height[hi];
            --hi;
        }
    }
    return water;
}

// ── Container With Most Water ────────────────────────────────────────────────
int maxWater(const std::vector<int>& height) {
    int lo = 0, hi = height.size()-1, maxArea = 0;
    while (lo < hi) {
        maxArea = std::max(maxArea, std::min(height[lo], height[hi]) * (hi - lo));
        if (height[lo] < height[hi]) ++lo;
        else --hi;
    }
    return maxArea;
}

// ── Remove Duplicates from Sorted Array ──────────────────────────────────────
int removeDuplicates(std::vector<int>& arr) {
    if (arr.empty()) return 0;
    int k = 1;
    for (int i = 1; i < (int)arr.size(); ++i)
        if (arr[i] != arr[i-1]) arr[k++] = arr[i];
    return k;
}

// ── Dutch National Flag (sort 0s, 1s, 2s) ────────────────────────────────────
void sortColors(std::vector<int>& nums) {
    int lo = 0, mid = 0, hi = nums.size()-1;
    while (mid <= hi) {
        if      (nums[mid] == 0) std::swap(nums[lo++], nums[mid++]);
        else if (nums[mid] == 1) ++mid;
        else                     std::swap(nums[mid], nums[hi--]);
    }
}

// ── Valid Palindrome ──────────────────────────────────────────────────────────
bool isPalindrome(const std::string& s) {
    int lo = 0, hi = s.size()-1;
    while (lo < hi) {
        while (lo < hi && !isalnum(s[lo])) ++lo;
        while (lo < hi && !isalnum(s[hi])) --hi;
        if (tolower(s[lo]) != tolower(s[hi])) return false;
        ++lo; --hi;
    }
    return true;
}

// ── Minimum Window Substring (two pointers + frequency map) ──────────────────
#include <unordered_map>
std::string minWindow(const std::string& s, const std::string& t) {
    std::unordered_map<char,int> need, window;
    for (char c : t) ++need[c];
    int lo = 0, have = 0, required = need.size();
    int minLen = INT_MAX, start = 0;
    for (int hi = 0; hi < (int)s.size(); ++hi) {
        ++window[s[hi]];
        if (need.count(s[hi]) && window[s[hi]] == need[s[hi]]) ++have;
        while (have == required) {
            if (hi - lo + 1 < minLen) { minLen = hi - lo + 1; start = lo; }
            --window[s[lo]];
            if (need.count(s[lo]) && window[s[lo]] < need[s[lo]]) --have;
            ++lo;
        }
    }
    return (minLen == INT_MAX) ? "" : s.substr(start, minLen);
}

// ── Merge Sorted Arrays ───────────────────────────────────────────────────────
std::vector<int> mergeSorted(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    int i = 0, j = 0;
    while (i < (int)a.size() && j < (int)b.size()) {
        if (a[i] <= b[j]) result.push_back(a[i++]);
        else               result.push_back(b[j++]);
    }
    while (i < (int)a.size()) result.push_back(a[i++]);
    while (j < (int)b.size()) result.push_back(b[j++]);
    return result;
}

void print(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " "; std::cout << "\n";
}

int main() {
    std::cout << "3Sum of [-1,0,1,2,-1,-4]:\n";
    for (auto& t : threeSum({-1,0,1,2,-1,-4})) {
        for (int x : t) std::cout << x << " "; std::cout << "\n";
    }

    std::cout << "Trap water [0,1,0,2,1,0,1,3,2,1,2,1]: "
              << trapWater({0,1,0,2,1,0,1,3,2,1,2,1}) << "\n"; // 6

    std::cout << "Max water [1,8,6,2,5,4,8,3,7]: "
              << maxWater({1,8,6,2,5,4,8,3,7}) << "\n"; // 49

    std::vector<int> nums = {0,1,2,0,1,2};
    sortColors(nums);
    std::cout << "Sort colors: "; print(nums);

    std::cout << "Is palindrome 'A man a plan a canal Panama': "
              << isPalindrome("A man a plan a canal Panama") << "\n"; // 1

    std::cout << "Min window ('ADOBECODEBANC','ABC'): "
              << minWindow("ADOBECODEBANC", "ABC") << "\n"; // BANC
    return 0;
}
