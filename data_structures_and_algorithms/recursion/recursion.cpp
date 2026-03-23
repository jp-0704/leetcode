// Recursion — classic problems and patterns
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// ── Tower of Hanoi ────────────────────────────────────────────────────────────
void hanoi(int n, char from, char to, char aux) {
    if (n == 1) { std::cout << "Move disk 1 from " << from << " to " << to << "\n"; return; }
    hanoi(n-1, from, aux, to);
    std::cout << "Move disk " << n << " from " << from << " to " << to << "\n";
    hanoi(n-1, aux, to, from);
}

// ── Recursive Binary Search ───────────────────────────────────────────────────
int binarySearch(const std::vector<int>& arr, int target, int lo, int hi) {
    if (lo > hi) return -1;
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) return binarySearch(arr, target, mid+1, hi);
    return binarySearch(arr, target, lo, mid-1);
}

// ── Flatten Nested List ───────────────────────────────────────────────────────
// Simulated with vector<variant> — using a simple recursive array flatten
void flatten(const std::vector<std::vector<int>>& nested, std::vector<int>& result) {
    for (const auto& v : nested) for (int x : v) result.push_back(x);
}

// ── Power Set ────────────────────────────────────────────────────────────────
void powerSet(const std::vector<int>& nums, int idx, std::vector<int>& curr,
              std::vector<std::vector<int>>& result) {
    result.push_back(curr);
    for (int i = idx; i < (int)nums.size(); ++i) {
        curr.push_back(nums[i]);
        powerSet(nums, i+1, curr, result);
        curr.pop_back();
    }
}

// ── String Permutations ───────────────────────────────────────────────────────
void stringPerms(std::string s, int l, int r, std::vector<std::string>& result) {
    if (l == r) { result.push_back(s); return; }
    for (int i = l; i <= r; ++i) {
        std::swap(s[l], s[i]);
        stringPerms(s, l+1, r, result);
        std::swap(s[l], s[i]);
    }
}

// ── Count ways to climb n stairs (1 or 2 steps) ──────────────────────────────
std::vector<long long> memo;
long long climbStairs(int n) {
    if (n <= 1) return 1;
    if (memo[n] != -1) return memo[n];
    return memo[n] = climbStairs(n-1) + climbStairs(n-2);
}

// ── Exponentiation (recursive, O(log n)) ─────────────────────────────────────
double fastPow(double x, int n) {
    if (n == 0) return 1;
    if (n < 0) { x = 1/x; n = -n; }
    if (n % 2 == 0) { double half = fastPow(x, n/2); return half * half; }
    return x * fastPow(x, n-1);
}

// ── Reverse a String Recursively ─────────────────────────────────────────────
void reverseStr(std::string& s, int lo, int hi) {
    if (lo >= hi) return;
    std::swap(s[lo], s[hi]);
    reverseStr(s, lo+1, hi-1);
}

// ── GCD (Euclidean) ───────────────────────────────────────────────────────────
int gcd(int a, int b) { return b == 0 ? a : gcd(b, a%b); }

// ── Count digits in number ────────────────────────────────────────────────────
int countDigits(int n) { return (n < 10) ? 1 : 1 + countDigits(n/10); }

// ── Sum of digits ─────────────────────────────────────────────────────────────
int sumDigits(int n) { return (n == 0) ? 0 : n%10 + sumDigits(n/10); }

// ── Check if palindrome recursively ──────────────────────────────────────────
bool isPalin(const std::string& s, int lo, int hi) {
    if (lo >= hi) return true;
    return (s[lo] == s[hi]) && isPalin(s, lo+1, hi-1);
}

// ── Flood Fill ────────────────────────────────────────────────────────────────
void floodFill(std::vector<std::vector<int>>& grid, int r, int c, int oldColor, int newColor) {
    if (r < 0 || r >= (int)grid.size() || c < 0 || c >= (int)grid[0].size()) return;
    if (grid[r][c] != oldColor) return;
    grid[r][c] = newColor;
    floodFill(grid, r+1, c, oldColor, newColor);
    floodFill(grid, r-1, c, oldColor, newColor);
    floodFill(grid, r, c+1, oldColor, newColor);
    floodFill(grid, r, c-1, oldColor, newColor);
}

// ── Print all binary strings of length n ──────────────────────────────────────
void binaryStrings(int n, std::string& current) {
    if (n == 0) { std::cout << current << " "; return; }
    current.push_back('0'); binaryStrings(n-1, current); current.pop_back();
    current.push_back('1'); binaryStrings(n-1, current); current.pop_back();
}

int main() {
    // Hanoi
    std::cout << "Tower of Hanoi (2 disks):\n";
    hanoi(2, 'A', 'C', 'B');

    // Binary search
    std::vector<int> arr = {1,3,5,7,9,11,13};
    std::cout << "\nBinary search 7: " << binarySearch(arr, 7, 0, arr.size()-1) << "\n"; // 3

    // Stairs
    memo.assign(50, -1);
    std::cout << "Climb 5 stairs: " << climbStairs(5) << "\n"; // 8

    // Power
    std::cout << "2^10 = " << fastPow(2, 10) << "\n"; // 1024

    // String reverse
    std::string s = "hello";
    reverseStr(s, 0, s.size()-1);
    std::cout << "Reversed: " << s << "\n"; // olleh

    // Palindrome
    std::cout << "isPalin 'racecar': " << isPalin("racecar", 0, 6) << "\n"; // 1

    // Binary strings
    std::cout << "Binary strings of length 3: ";
    std::string cur;
    binaryStrings(3, cur);
    std::cout << "\n";

    // Power set
    std::vector<std::vector<int>> ps;
    std::vector<int> tmp;
    powerSet({1,2,3}, 0, tmp, ps);
    std::cout << "Power set size of {1,2,3}: " << ps.size() << "\n"; // 8
    return 0;
}
