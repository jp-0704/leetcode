// Matrix Operations — multiplication, rotation, spiral, search
#include <iostream>
#include <vector>
#include <algorithm>

using Matrix = std::vector<std::vector<int>>;

// ── Matrix Multiplication — O(n^3) / Strassen O(n^2.807) ─────────────────────
Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size(), m = B[0].size(), k = B.size();
    Matrix C(n, std::vector<int>(m, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            for (int p = 0; p < k; ++p)
                C[i][j] += A[i][p] * B[p][j];
    return C;
}

// Matrix power in O(n^3 log p)
Matrix matPow(Matrix A, long long p) {
    int n = A.size();
    Matrix result(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) result[i][i] = 1; // identity
    while (p > 0) {
        if (p & 1) result = multiply(result, A);
        A = multiply(A, A);
        p >>= 1;
    }
    return result;
}

// ── Rotate Matrix 90° clockwise in-place ──────────────────────────────────────
void rotate90(Matrix& mat) {
    int n = mat.size();
    // Transpose
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            std::swap(mat[i][j], mat[j][i]);
    // Reverse each row
    for (int i = 0; i < n; ++i)
        std::reverse(mat[i].begin(), mat[i].end());
}

// ── Spiral Order traversal ────────────────────────────────────────────────────
std::vector<int> spiralOrder(const Matrix& mat) {
    std::vector<int> result;
    if (mat.empty()) return result;
    int top = 0, bot = mat.size()-1, left = 0, right = mat[0].size()-1;
    while (top <= bot && left <= right) {
        for (int c = left; c <= right; ++c) result.push_back(mat[top][c]);
        ++top;
        for (int r = top; r <= bot; ++r) result.push_back(mat[r][right]);
        --right;
        if (top <= bot) {
            for (int c = right; c >= left; --c) result.push_back(mat[bot][c]);
            --bot;
        }
        if (left <= right) {
            for (int r = bot; r >= top; --r) result.push_back(mat[r][left]);
            ++left;
        }
    }
    return result;
}

// ── Search in Row-wise and Column-wise Sorted Matrix — O(n+m) ─────────────────
bool searchMatrix(const Matrix& mat, int target) {
    if (mat.empty()) return false;
    int row = 0, col = mat[0].size() - 1;
    while (row < (int)mat.size() && col >= 0) {
        if      (mat[row][col] == target) return true;
        else if (mat[row][col] >  target) --col;
        else                              ++row;
    }
    return false;
}

// ── Set Zeroes in-place ───────────────────────────────────────────────────────
void setZeroes(Matrix& mat) {
    int m = mat.size(), n = mat[0].size();
    bool firstRow = false, firstCol = false;
    for (int j = 0; j < n; ++j) if (mat[0][j] == 0) firstRow = true;
    for (int i = 0; i < m; ++i) if (mat[i][0] == 0) firstCol = true;
    for (int i = 1; i < m; ++i)
        for (int j = 1; j < n; ++j)
            if (mat[i][j] == 0) { mat[i][0] = 0; mat[0][j] = 0; }
    for (int i = 1; i < m; ++i)
        for (int j = 1; j < n; ++j)
            if (!mat[i][0] || !mat[0][j]) mat[i][j] = 0;
    if (firstRow) for (int j = 0; j < n; ++j) mat[0][j] = 0;
    if (firstCol) for (int i = 0; i < m; ++i) mat[i][0] = 0;
}

// ── Maximal Rectangle in Binary Matrix ───────────────────────────────────────
// Uses largest rectangle in histogram technique
int largestRectHistogram(const std::vector<int>& h) {
    std::vector<int> st;
    int maxArea = 0, n = h.size();
    for (int i = 0; i <= n; ++i) {
        int height = (i == n) ? 0 : h[i];
        while (!st.empty() && h[st.back()] > height) {
            int ht = h[st.back()]; st.pop_back();
            int w = st.empty() ? i : i - st.back() - 1;
            maxArea = std::max(maxArea, ht * w);
        }
        st.push_back(i);
    }
    return maxArea;
}

int maximalRectangle(Matrix& mat) {
    if (mat.empty()) return 0;
    int m = mat.size(), n = mat[0].size(), maxRect = 0;
    std::vector<int> heights(n, 0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            heights[j] = (mat[i][j] == 1) ? heights[j] + 1 : 0;
        maxRect = std::max(maxRect, largestRectHistogram(heights));
    }
    return maxRect;
}

// ── Number of Islands (DFS) ───────────────────────────────────────────────────
void dfs(std::vector<std::vector<char>>& grid, int r, int c) {
    if (r < 0 || r >= (int)grid.size() || c < 0 || c >= (int)grid[0].size() || grid[r][c] != '1') return;
    grid[r][c] = '0';
    dfs(grid,r+1,c); dfs(grid,r-1,c); dfs(grid,r,c+1); dfs(grid,r,c-1);
}

int numIslands(std::vector<std::vector<char>> grid) {
    int count = 0;
    for (int r = 0; r < (int)grid.size(); ++r)
        for (int c = 0; c < (int)grid[0].size(); ++c)
            if (grid[r][c] == '1') { dfs(grid, r, c); ++count; }
    return count;
}

void print(const Matrix& mat) {
    for (const auto& row : mat) {
        for (int x : row) std::cout << x << " ";
        std::cout << "\n";
    }
}

int main() {
    Matrix A = {{1,2},{3,4}}, B = {{5,6},{7,8}};
    auto C = multiply(A, B);
    std::cout << "Matrix multiply:\n"; print(C);

    Matrix mat = {{1,2,3},{4,5,6},{7,8,9}};
    rotate90(mat);
    std::cout << "\nAfter 90° rotation:\n"; print(mat);

    Matrix mat2 = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    auto spiral = spiralOrder(mat2);
    std::cout << "\nSpiral: ";
    for (int x : spiral) std::cout << x << " "; std::cout << "\n";

    Matrix sorted = {{1,4,7,11},{2,5,8,12},{3,6,9,16},{10,13,14,17}};
    std::cout << "Search 5: " << searchMatrix(sorted, 5) << "\n";
    std::cout << "Search 20: " << searchMatrix(sorted, 20) << "\n";

    // Fibonacci via matrix exponentiation
    Matrix fib = {{1,1},{1,0}};
    auto res = matPow(fib, 10);
    std::cout << "\nFib(10) via matrix pow: " << res[0][1] << "\n"; // 55
    return 0;
}
