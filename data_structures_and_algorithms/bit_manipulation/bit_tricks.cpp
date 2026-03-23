// Bit Manipulation — tricks, classic problems, XOR applications
#include <iostream>
#include <vector>
#include <string>

// ── Basic Bit Operations ──────────────────────────────────────────────────────
bool getBit(int n, int i)   { return (n >> i) & 1; }
int  setBit(int n, int i)   { return n | (1 << i); }
int  clearBit(int n, int i) { return n & ~(1 << i); }
int  toggleBit(int n, int i){ return n ^ (1 << i); }
bool isPowerOf2(int n)       { return n > 0 && (n & (n-1)) == 0; }
int  lsbPosition(int n)      { return n & (-n); }           // lowest set bit
int  clearLSB(int n)         { return n & (n-1); }          // clear lowest set bit
int  countBits(int n)        { return __builtin_popcount(n); } // # set bits

// Count set bits (Brian Kernighan's algorithm)
int countSetBits(int n) {
    int count = 0;
    while (n) { n &= (n-1); ++count; }
    return count;
}

// Count set bits in [0..n] cumulatively
std::vector<int> countBitsRange(int n) {
    std::vector<int> dp(n+1, 0);
    for (int i = 1; i <= n; ++i)
        dp[i] = dp[i >> 1] + (i & 1);
    return dp;
}

// ── Classic XOR Problems ──────────────────────────────────────────────────────

// Find single number (every other appears twice)
int singleNumber(const std::vector<int>& nums) {
    int xorSum = 0;
    for (int x : nums) xorSum ^= x;
    return xorSum;
}

// Find two unique numbers (every other appears twice)
std::pair<int,int> twoUniqueNumbers(const std::vector<int>& nums) {
    int xorAll = 0;
    for (int x : nums) xorAll ^= x;
    int diffBit = xorAll & (-xorAll); // rightmost differing bit
    int a = 0, b = 0;
    for (int x : nums) {
        if (x & diffBit) a ^= x;
        else             b ^= x;
    }
    return {a, b};
}

// Single number appearing once (all others appear 3 times) — bitwise counting
int singleNumber3Times(const std::vector<int>& nums) {
    int ones = 0, twos = 0;
    for (int x : nums) {
        ones = (ones ^ x) & ~twos;
        twos = (twos ^ x) & ~ones;
    }
    return ones;
}

// ── Bit Tricks ────────────────────────────────────────────────────────────────

// Swap without temporary
void swapBits(int& a, int& b) { a ^= b; b ^= a; a ^= b; }

// Check if two integers have opposite signs
bool oppositeSigns(int a, int b) { return (a ^ b) < 0; }

// Multiply by power of 2
int multiplyBy2k(int n, int k) { return n << k; }

// Divide by power of 2
int divideBy2k(int n, int k) { return n >> k; }

// Turn off n-th bit from right
int turnOffKthBit(int n, int k) { return n & ~(1 << (k-1)); }

// Check if n is divisible by 2
bool evenNumber(int n) { return !(n & 1); }

// Next power of 2
int nextPowerOf2(int n) {
    if (n <= 1) return 1;
    --n;
    n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8; n |= n >> 16;
    return ++n;
}

// Reverse bits of a 32-bit integer
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}

// Hamming distance (number of differing bits)
int hammingDistance(int x, int y) { return __builtin_popcount(x ^ y); }

// ── Generate all subsets using bitmask ────────────────────────────────────────
std::vector<std::vector<int>> allSubsets(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<std::vector<int>> result;
    for (int mask = 0; mask < (1 << n); ++mask) {
        std::vector<int> subset;
        for (int i = 0; i < n; ++i)
            if (mask & (1 << i)) subset.push_back(nums[i]);
        result.push_back(subset);
    }
    return result;
}

// Enumerate all subsets of a given mask (useful in bitmask DP)
void enumerateSubsets(int mask) {
    for (int sub = mask; sub > 0; sub = (sub-1) & mask)
        std::cout << sub << " ";
    std::cout << "\n";
}

// ── Gray Code ─────────────────────────────────────────────────────────────────
std::vector<int> grayCode(int n) {
    std::vector<int> result;
    for (int i = 0; i < (1 << n); ++i)
        result.push_back(i ^ (i >> 1));
    return result;
}

// ── Bit manipulation for strings ─────────────────────────────────────────────
// Check if string has all unique characters (for a-z only)
bool allUnique(const std::string& s) {
    int checker = 0;
    for (char c : s) {
        int bit = 1 << (c - 'a');
        if (checker & bit) return false;
        checker |= bit;
    }
    return true;
}

int main() {
    // Basic operations
    int n = 12; // 1100
    std::cout << "n = " << n << " (binary: 1100)\n";
    std::cout << "Bit 2: " << getBit(n, 2) << "\n";     // 1
    std::cout << "Set bit 1: " << setBit(n, 1) << "\n"; // 14
    std::cout << "Clear bit 2: " << clearBit(n, 2) << "\n"; // 8
    std::cout << "Count set bits: " << countSetBits(n) << "\n"; // 2
    std::cout << "Is power of 2 (16): " << isPowerOf2(16) << "\n"; // 1

    // XOR problems
    std::cout << "\nSingle number in [2,2,1]: " << singleNumber({2,2,1}) << "\n"; // 1
    auto [a, b] = twoUniqueNumbers({1,2,1,3,2,5});
    std::cout << "Two unique: " << a << " " << b << "\n"; // 3 5

    // Bit tricks
    std::cout << "\nNext power of 2 after 100: " << nextPowerOf2(100) << "\n"; // 128
    std::cout << "Reverse bits of 1: " << reverseBits(1) << "\n"; // 2147483648
    std::cout << "Hamming(1,4): " << hammingDistance(1, 4) << "\n"; // 2

    // Subsets of {1,2,3}
    auto subsets = allSubsets({1, 2, 3});
    std::cout << "\nAll subsets of {1,2,3}: " << subsets.size() << " subsets\n"; // 8

    // Gray code
    auto gc = grayCode(3);
    std::cout << "Gray code (n=3): ";
    for (int x : gc) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "All unique 'abc': " << allUnique("abc") << "\n"; // 1
    std::cout << "All unique 'aab': " << allUnique("aab") << "\n"; // 0
    return 0;
}
