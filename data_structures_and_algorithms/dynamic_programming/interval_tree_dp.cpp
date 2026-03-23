// Interval DP, Tree DP, Digit DP, Bitmask DP
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

// ── Interval DP ───────────────────────────────────────────────────────────────

// Burst Balloons — multiply adjacent balloons
int maxCoins(std::vector<int> nums) {
    nums.insert(nums.begin(), 1);
    nums.push_back(1);
    int n = nums.size();
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));
    for (int len = 2; len < n; ++len)
        for (int left = 0; left < n - len; ++left) {
            int right = left + len;
            for (int k = left + 1; k < right; ++k)
                dp[left][right] = std::max(dp[left][right],
                    dp[left][k] + nums[left]*nums[k]*nums[right] + dp[k][right]);
        }
    return dp[0][n-1];
}

// Palindrome Partitioning (min cuts)
int minPalindromeCuts(const std::string& s) {
    int n = s.size();
    std::vector<std::vector<bool>> isPalin(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; ++i) isPalin[i][i] = true;
    for (int len = 2; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            isPalin[i][j] = (s[i] == s[j]) && (len == 2 || isPalin[i+1][j-1]);
        }
    std::vector<int> dp(n, INT_MAX);
    for (int i = 0; i < n; ++i) {
        if (isPalin[0][i]) { dp[i] = 0; continue; }
        for (int j = 1; j <= i; ++j)
            if (isPalin[j][i]) dp[i] = std::min(dp[i], dp[j-1] + 1);
    }
    return dp[n-1];
}

// ── Bitmask DP ────────────────────────────────────────────────────────────────

// Travelling Salesman Problem (TSP) — O(2^n * n^2)
int tsp(const std::vector<std::vector<int>>& dist) {
    int n = dist.size();
    std::vector<std::vector<int>> dp(1<<n, std::vector<int>(n, INT_MAX/2));
    dp[1][0] = 0; // start at city 0
    for (int mask = 1; mask < (1<<n); ++mask)
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1<<u)) || dp[mask][u] == INT_MAX/2) continue;
            for (int v = 0; v < n; ++v) {
                if (mask & (1<<v)) continue;
                int next = mask | (1<<v);
                dp[next][v] = std::min(dp[next][v], dp[mask][u] + dist[u][v]);
            }
        }
    int ans = INT_MAX;
    int full = (1<<n) - 1;
    for (int u = 1; u < n; ++u)
        if (dp[full][u] != INT_MAX/2) ans = std::min(ans, dp[full][u] + dist[u][0]);
    return ans;
}

// Minimum Cost to Cover All Points (bitmask DP)
int assignmentProblem(const std::vector<std::vector<int>>& cost) {
    int n = cost.size();
    std::vector<int> dp(1<<n, INT_MAX);
    dp[0] = 0;
    for (int mask = 0; mask < (1<<n); ++mask) {
        if (dp[mask] == INT_MAX) continue;
        int row = __builtin_popcount(mask); // number of assigned tasks
        if (row == n) continue;
        for (int col = 0; col < n; ++col)
            if (!(mask & (1<<col)))
                dp[mask|(1<<col)] = std::min(dp[mask|(1<<col)], dp[mask] + cost[row][col]);
    }
    return dp[(1<<n)-1];
}

// ── Tree DP ───────────────────────────────────────────────────────────────────
// Maximum sum of non-adjacent nodes in a tree (House Robber III)
struct TNode { int val; std::vector<TNode*> children; TNode(int v) : val(v) {} };

std::pair<int,int> treeDP(TNode* root) {
    if (!root) return {0, 0};
    int withRoot = root->val, withoutRoot = 0;
    for (TNode* child : root->children) {
        auto [cWith, cWithout] = treeDP(child);
        withRoot    += cWithout;
        withoutRoot += std::max(cWith, cWithout);
    }
    return {withRoot, withoutRoot};
}

// ── Digit DP ──────────────────────────────────────────────────────────────────
// Count numbers from 1 to N with no repeated digits
std::string numStr;
std::vector<std::vector<std::vector<int>>> memo;

int digitDP(int pos, int usedMask, bool tight, bool started) {
    if (pos == (int)numStr.size()) return started ? 1 : 0;
    if (memo[pos][usedMask][tight] != -1) return memo[pos][usedMask][tight];

    int limit = tight ? (numStr[pos] - '0') : 9;
    int result = 0;

    for (int d = 0; d <= limit; ++d) {
        if (started && (usedMask >> d & 1)) continue; // already used
        bool newTight = tight && (d == limit);
        bool newStarted = started || (d > 0);
        int newMask = newStarted ? (usedMask | (1<<d)) : usedMask;
        result += digitDP(pos+1, newMask, newTight, newStarted);
    }
    return memo[pos][usedMask][tight] = result;
}

int countNoRepeatDigits(int N) {
    numStr = std::to_string(N);
    int n = numStr.size();
    memo.assign(n, std::vector<std::vector<int>>(1<<10, std::vector<int>(2, -1)));
    return digitDP(0, 0, true, false);
}

int main() {
    std::cout << "Burst balloons [3,1,5,8]: " << maxCoins({3,1,5,8}) << "\n"; // 167
    std::cout << "Min palindrome cuts 'aab': " << minPalindromeCuts("aab") << "\n"; // 1

    std::vector<std::vector<int>> dist = {
        {0,10,15,20},{10,0,35,25},{15,35,0,30},{20,25,30,0}
    };
    std::cout << "TSP cost: " << tsp(dist) << "\n"; // 80

    // Tree DP: rob a tree
    TNode* root = new TNode(3);
    root->children.push_back(new TNode(2));
    root->children.push_back(new TNode(3));
    root->children[0]->children.push_back(new TNode(3));
    root->children[1]->children.push_back(new TNode(1));
    auto [w, wo] = treeDP(root);
    std::cout << "Max non-adjacent sum: " << std::max(w, wo) << "\n"; // 7

    std::cout << "Numbers <=20 with no repeat digits: " << countNoRepeatDigits(20) << "\n"; // 19
    return 0;
}
