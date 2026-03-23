// Mathematical Algorithms — GCD, primes, combinatorics, modular arithmetic
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

// ── GCD and LCM ───────────────────────────────────────────────────────────────
long long gcd(long long a, long long b) { return b ? gcd(b, a%b) : a; }
long long lcm(long long a, long long b) { return a / gcd(a, b) * b; }

// Extended GCD: ax + by = gcd(a,b)
long long extGCD(long long a, long long b, long long& x, long long& y) {
    if (!b) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extGCD(b, a%b, x1, y1);
    x = y1; y = x1 - (a/b)*y1;
    return g;
}

// ── Modular Arithmetic ────────────────────────────────────────────────────────
const long long MOD = 1e9 + 7;

long long modPow(long long base, long long exp, long long mod) {
    long long result = 1; base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

long long modInverse(long long a, long long mod) {
    return modPow(a, mod - 2, mod); // only valid if mod is prime (Fermat's little theorem)
}

// Modular inverse using extended GCD
long long modInverseExt(long long a, long long mod) {
    long long x, y;
    extGCD(a, mod, x, y);
    return (x % mod + mod) % mod;
}

// ── Prime Numbers ─────────────────────────────────────────────────────────────

// Check if n is prime
bool isPrime(long long n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n%2 == 0 || n%3 == 0) return false;
    for (long long i = 5; i*i <= n; i += 6)
        if (n%i == 0 || n%(i+2) == 0) return false;
    return true;
}

// Sieve of Eratosthenes — all primes up to n
std::vector<int> sieve(int n) {
    std::vector<bool> is_prime(n+1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i*i <= n; ++i)
        if (is_prime[i])
            for (int j = i*i; j <= n; j += i) is_prime[j] = false;
    std::vector<int> primes;
    for (int i = 2; i <= n; ++i) if (is_prime[i]) primes.push_back(i);
    return primes;
}

// Segmented Sieve — primes in [lo, hi]
std::vector<long long> segmentedSieve(long long lo, long long hi) {
    int limit = std::sqrt(hi) + 1;
    auto smallPrimes = sieve(limit);
    std::vector<bool> is_prime(hi - lo + 1, true);
    if (lo == 1) is_prime[0] = false;
    for (int p : smallPrimes) {
        long long start = std::max((long long)p*p, (lo + p - 1) / p * p);
        for (long long j = start; j <= hi; j += p) is_prime[j - lo] = false;
    }
    std::vector<long long> primes;
    for (long long i = lo; i <= hi; ++i) if (is_prime[i-lo]) primes.push_back(i);
    return primes;
}

// Prime factorization
std::vector<long long> primeFactors(long long n) {
    std::vector<long long> factors;
    for (long long p = 2; p*p <= n; ++p) {
        while (n % p == 0) { factors.push_back(p); n /= p; }
    }
    if (n > 1) factors.push_back(n);
    return factors;
}

// Number of divisors
int numDivisors(long long n) {
    int count = 0;
    for (long long i = 1; i*i <= n; ++i) {
        if (n % i == 0) { count += (i == n/i) ? 1 : 2; }
    }
    return count;
}

// ── Combinatorics ─────────────────────────────────────────────────────────────

// Pascal's triangle row (nCr for all r)
std::vector<long long> pascalRow(int n) {
    std::vector<long long> row(n+1, 1);
    for (int k = 1; k <= n; ++k) row[k] = row[k-1] * (n-k+1) / k;
    return row;
}

// nCr with modular arithmetic
std::vector<std::vector<long long>> buildFactorials(int maxN, long long mod) {
    std::vector<long long> fact(maxN+1), inv_fact(maxN+1);
    fact[0] = 1;
    for (int i = 1; i <= maxN; ++i) fact[i] = fact[i-1] * i % mod;
    inv_fact[maxN] = modPow(fact[maxN], mod-2, mod);
    for (int i = maxN-1; i >= 0; --i) inv_fact[i] = inv_fact[i+1] * (i+1) % mod;
    return {fact, inv_fact};
}

auto [fact, inv_fact] = buildFactorials(1000000, MOD);

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n-r] % MOD;
}

// ── Chinese Remainder Theorem ─────────────────────────────────────────────────
// Solve: x ≡ a1 (mod m1), x ≡ a2 (mod m2) — m1,m2 coprime
long long crt(long long a1, long long m1, long long a2, long long m2) {
    long long x, y;
    extGCD(m1, m2, x, y);
    return (a1 + m1 * ((a2 - a1) % m2 * x % m2)) % (m1 * m2);
}

// ── Number Theory ─────────────────────────────────────────────────────────────

// Euler's totient function
int eulerTotient(int n) {
    int result = n;
    for (int p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

// Miller-Rabin primality test (deterministic for small n)
bool millerRabin(long long n, long long a) {
    if (n % a == 0) return n == a;
    long long d = n-1; int r = 0;
    while (d % 2 == 0) { d /= 2; ++r; }
    long long x = modPow(a, d, n);
    if (x == 1 || x == n-1) return true;
    for (int i = 0; i < r-1; ++i) {
        x = x*x % n;
        if (x == n-1) return true;
    }
    return false;
}

bool isPrimeMR(long long n) {
    if (n < 2) return false;
    for (long long a : {2,3,5,7,11,13,17,19,23,29,31,37})
        if (n == a) return true;
        else if (!millerRabin(n, a)) return false;
    return true;
}

int main() {
    std::cout << "GCD(48,18): " << gcd(48,18) << "\n"; // 6
    std::cout << "LCM(4,6): "  << lcm(4,6)  << "\n";   // 12

    std::cout << "ModPow(2,10,1e9+7): " << modPow(2,10,MOD) << "\n"; // 1024

    auto primes = sieve(50);
    std::cout << "Primes to 50: ";
    for (int p : primes) std::cout << p << " "; std::cout << "\n";

    auto factors = primeFactors(360);
    std::cout << "Factors of 360: ";
    for (long long f : factors) std::cout << f << " "; std::cout << "\n";

    std::cout << "C(10,3): " << nCr(10,3) << "\n"; // 120

    std::cout << "Euler totient(12): " << eulerTotient(12) << "\n"; // 4

    std::cout << "Is 997 prime (Miller-Rabin): " << isPrimeMR(997) << "\n"; // 1
    std::cout << "Is 999 prime: " << isPrimeMR(999) << "\n"; // 0

    auto seg = segmentedSieve(10, 50);
    std::cout << "Primes [10,50]: ";
    for (long long p : seg) std::cout << p << " "; std::cout << "\n";
    return 0;
}
