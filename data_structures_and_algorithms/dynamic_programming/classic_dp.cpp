// Classic Dynamic Programming Problems
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

// ── 1. Fibonacci (bottom-up, O(1) space) ─────────────────────────────────────
long long fib(int n) {
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { long long c = a + b; a = b; b = c; }
    return b;
}

// ── 2. 0/1 Knapsack ──────────────────────────────────────────────────────────
int knapsack01(int W, const std::vector<int>& wt, const std::vector<int>& val) {
    int n = wt.size();
    std::vector<int> dp(W + 1, 0);
    for (int i = 0; i < n; ++i)
        for (int w = W; w >= wt[i]; --w)
            dp[w] = std::max(dp[w], dp[w - wt[i]] + val[i]);
    return dp[W];
}

// ── 3. Unbounded Knapsack (Coin Change - max value) ──────────────────────────
int unboundedKnapsack(int W, const std::vector<int>& wt, const std::vector<int>& val) {
    std::vector<int> dp(W + 1, 0);
    for (int w = 1; w <= W; ++w)
        for (int i = 0; i < (int)wt.size(); ++i)
            if (wt[i] <= w) dp[w] = std::max(dp[w], dp[w - wt[i]] + val[i]);
    return dp[W];
}

// ── 4. Coin Change (min coins) ────────────────────────────────────────────────
int coinChange(const std::vector<int>& coins, int amount) {
    std::vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i)
        for (int c : coins)
            if (c <= i) dp[i] = std::min(dp[i], dp[i - c] + 1);
    return dp[amount] > amount ? -1 : dp[amount];
}

// ── 5. Longest Common Subsequence ────────────────────────────────────────────
int lcs(const std::string& s1, const std::string& s2) {
    int m = s1.size(), n = s2.size();
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            dp[i][j] = (s1[i-1] == s2[j-1]) ? dp[i-1][j-1] + 1
                                              : std::max(dp[i-1][j], dp[i][j-1]);
    return dp[m][n];
}

// Recover LCS string
std::string lcsString(const std::string& s1, const std::string& s2) {
    int m = s1.size(), n = s2.size();
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            dp[i][j] = (s1[i-1] == s2[j-1]) ? dp[i-1][j-1] + 1
                                              : std::max(dp[i-1][j], dp[i][j-1]);
    std::string result;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (s1[i-1] == s2[j-1]) { result += s1[i-1]; --i; --j; }
        else if (dp[i-1][j] > dp[i][j-1]) --i;
        else --j;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// ── 6. Longest Increasing Subsequence (O(n log n)) ────────────────────────────
int lis(const std::vector<int>& nums) {
    std::vector<int> dp;
    for (int x : nums) {
        auto it = std::lower_bound(dp.begin(), dp.end(), x);
        if (it == dp.end()) dp.push_back(x);
        else *it = x;
    }
    return dp.size();
}

// ── 7. Edit Distance (Levenshtein) ────────────────────────────────────────────
int editDistance(const std::string& s1, const std::string& s2) {
    int m = s1.size(), n = s2.size();
    std::vector<int> prev(n+1), curr(n+1);
    std::iota(prev.begin(), prev.end(), 0);
    for (int i = 1; i <= m; ++i) {
        curr[0] = i;
        for (int j = 1; j <= n; ++j)
            curr[j] = (s1[i-1] == s2[j-1]) ? prev[j-1]
                      : 1 + std::min({prev[j], curr[j-1], prev[j-1]});
        std::swap(prev, curr);
    }
    return prev[n];
}

// ── 8. Matrix Chain Multiplication ───────────────────────────────────────────
int matrixChain(const std::vector<int>& dims) {
    int n = dims.size() - 1;
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    for (int len = 2; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; ++k)
                dp[i][j] = std::min(dp[i][j], dp[i][k] + dp[k+1][j] + dims[i]*dims[k+1]*dims[j+1]);
        }
    return dp[0][n-1];
}

// ── 9. Longest Palindromic Subsequence ───────────────────────────────────────
int lps(const std::string& s) {
    int n = s.size();
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) dp[i][i] = 1;
    for (int len = 2; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j]) dp[i][j] = dp[i+1][j-1] + 2;
            else dp[i][j] = std::max(dp[i+1][j], dp[i][j-1]);
        }
    return dp[0][n-1];
}

// ── 10. Maximum Subarray (Kadane's) ──────────────────────────────────────────
int maxSubarray(const std::vector<int>& nums) {
    int maxSum = nums[0], cur = nums[0];
    for (int i = 1; i < (int)nums.size(); ++i) {
        cur = std::max(nums[i], cur + nums[i]);
        maxSum = std::max(maxSum, cur);
    }
    return maxSum;
}

// ── 11. Rod Cutting ───────────────────────────────────────────────────────────
int rodCutting(const std::vector<int>& prices, int n) {
    std::vector<int> dp(n+1, 0);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= i; ++j)
            dp[i] = std::max(dp[i], prices[j-1] + dp[i-j]);
    return dp[n];
}

// ── 12. Partition Equal Subset Sum ────────────────────────────────────────────
bool canPartition(const std::vector<int>& nums) {
    int total = 0;
    for (int x : nums) total += x;
    if (total % 2) return false;
    int target = total / 2;
    std::vector<bool> dp(target + 1, false);
    dp[0] = true;
    for (int x : nums)
        for (int j = target; j >= x; --j)
            dp[j] = dp[j] || dp[j - x];
    return dp[target];
}

// ── 13. Egg Drop Problem ──────────────────────────────────────────────────────
int eggDrop(int eggs, int floors) {
    // dp[e][f] = min trials with e eggs and f floors
    std::vector<std::vector<int>> dp(eggs+1, std::vector<int>(floors+1, 0));
    for (int f = 1; f <= floors; ++f) dp[1][f] = f;
    for (int e = 2; e <= eggs; ++e) {
        for (int f = 1; f <= floors; ++f) {
            dp[e][f] = INT_MAX;
            for (int x = 1; x <= f; ++x)
                dp[e][f] = std::min(dp[e][f], 1 + std::max(dp[e-1][x-1], dp[e][f-x]));
        }
    }
    return dp[eggs][floors];
}

int main() {
    std::cout << "Fib(10): " << fib(10) << "\n"; // 55

    std::vector<int> wt = {2,3,4,5}, val = {3,4,5,6};
    std::cout << "0/1 Knapsack(W=5): " << knapsack01(5, wt, val) << "\n"; // 7

    std::cout << "Coin change(amount=11): " << coinChange({1,5,6,9}, 11) << "\n"; // 2

    std::cout << "LCS('ABCBDAB','BDCAB'): " << lcs("ABCBDAB", "BDCAB") << "\n"; // 4
    std::cout << "LCS string: " << lcsString("ABCBDAB", "BDCAB") << "\n"; // BCAB

    std::cout << "LIS([10,9,2,5,3,7,101,18]): " << lis({10,9,2,5,3,7,101,18}) << "\n"; // 4

    std::cout << "Edit distance('horse','ros'): " << editDistance("horse","ros") << "\n"; // 3

    std::cout << "Matrix chain [1,2,3,4]: " << matrixChain({1,2,3,4}) << "\n"; // 18

    std::cout << "LPS('bbbab'): " << lps("bbbab") << "\n"; // 4

    std::cout << "Max subarray [-2,1,-3,4,-1,2,1,-5,4]: "
              << maxSubarray({-2,1,-3,4,-1,2,1,-5,4}) << "\n"; // 6

    std::cout << "Can partition [1,5,11,5]: " << canPartition({1,5,11,5}) << "\n"; // 1

    std::cout << "Egg drop(2 eggs, 10 floors): " << eggDrop(2, 10) << "\n"; // 4
    return 0;
}
