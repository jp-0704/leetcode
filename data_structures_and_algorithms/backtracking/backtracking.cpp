// Backtracking — classic problems
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

// ── N-Queens ──────────────────────────────────────────────────────────────────
class NQueens {
    int n;
    std::vector<std::vector<std::string>> solutions;
    std::vector<bool> col, diag1, diag2;

    void solve(int row, std::vector<std::string>& board) {
        if (row == n) { solutions.push_back(board); return; }
        for (int c = 0; c < n; ++c) {
            if (col[c] || diag1[row-c+n] || diag2[row+c]) continue;
            board[row][c] = 'Q';
            col[c] = diag1[row-c+n] = diag2[row+c] = true;
            solve(row+1, board);
            board[row][c] = '.';
            col[c] = diag1[row-c+n] = diag2[row+c] = false;
        }
    }

public:
    NQueens(int n) : n(n), col(n,false), diag1(2*n,false), diag2(2*n,false) {}

    std::vector<std::vector<std::string>> solve() {
        std::vector<std::string> board(n, std::string(n, '.'));
        solve(0, board);
        return solutions;
    }
};

// ── Sudoku Solver ─────────────────────────────────────────────────────────────
bool isValidSudoku(std::vector<std::vector<char>>& board, int r, int c, char num) {
    for (int i = 0; i < 9; ++i) {
        if (board[r][i] == num) return false;
        if (board[i][c] == num) return false;
        if (board[3*(r/3) + i/3][3*(c/3) + i%3] == num) return false;
    }
    return true;
}

bool solveSudoku(std::vector<std::vector<char>>& board) {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (board[r][c] == '.') {
                for (char num = '1'; num <= '9'; ++num) {
                    if (isValidSudoku(board, r, c, num)) {
                        board[r][c] = num;
                        if (solveSudoku(board)) return true;
                        board[r][c] = '.';
                    }
                }
                return false;
            }
    return true;
}

// ── Subsets ───────────────────────────────────────────────────────────────────
void subsetsHelper(const std::vector<int>& nums, int start,
                   std::vector<int>& curr, std::vector<std::vector<int>>& result) {
    result.push_back(curr);
    for (int i = start; i < (int)nums.size(); ++i) {
        curr.push_back(nums[i]);
        subsetsHelper(nums, i+1, curr, result);
        curr.pop_back();
    }
}

std::vector<std::vector<int>> subsets(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    std::vector<int> curr;
    subsetsHelper(nums, 0, curr, result);
    return result;
}

// Subsets with duplicates
std::vector<std::vector<int>> subsetsWithDup(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    std::vector<int> curr;
    std::function<void(int)> bt = [&](int start) {
        result.push_back(curr);
        for (int i = start; i < (int)nums.size(); ++i) {
            if (i > start && nums[i] == nums[i-1]) continue;
            curr.push_back(nums[i]);
            bt(i+1);
            curr.pop_back();
        }
    };
    bt(0);
    return result;
}

// ── Permutations ──────────────────────────────────────────────────────────────
std::vector<std::vector<int>> permutations(std::vector<int> nums) {
    std::vector<std::vector<int>> result;
    std::sort(nums.begin(), nums.end());
    do { result.push_back(nums); } while (std::next_permutation(nums.begin(), nums.end()));
    return result;
}

// Permutations with backtracking
void permuteHelper(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result) {
    if (start == (int)nums.size()) { result.push_back(nums); return; }
    for (int i = start; i < (int)nums.size(); ++i) {
        std::swap(nums[start], nums[i]);
        permuteHelper(nums, start+1, result);
        std::swap(nums[start], nums[i]);
    }
}

// ── Combination Sum ───────────────────────────────────────────────────────────
void combinationSumHelper(const std::vector<int>& candidates, int target, int start,
                           std::vector<int>& curr, std::vector<std::vector<int>>& result) {
    if (target == 0) { result.push_back(curr); return; }
    for (int i = start; i < (int)candidates.size() && candidates[i] <= target; ++i) {
        curr.push_back(candidates[i]);
        combinationSumHelper(candidates, target - candidates[i], i, curr, result);
        curr.pop_back();
    }
}

std::vector<std::vector<int>> combinationSum(std::vector<int> candidates, int target) {
    std::sort(candidates.begin(), candidates.end());
    std::vector<std::vector<int>> result;
    std::vector<int> curr;
    combinationSumHelper(candidates, target, 0, curr, result);
    return result;
}

// ── Word Search ───────────────────────────────────────────────────────────────
bool wordSearchHelper(std::vector<std::vector<char>>& board, const std::string& word,
                       int r, int c, int idx) {
    if (idx == (int)word.size()) return true;
    if (r < 0 || r >= (int)board.size() || c < 0 || c >= (int)board[0].size()
        || board[r][c] != word[idx]) return false;
    char tmp = board[r][c]; board[r][c] = '#';
    bool found = wordSearchHelper(board,word,r+1,c,idx+1) ||
                 wordSearchHelper(board,word,r-1,c,idx+1) ||
                 wordSearchHelper(board,word,r,c+1,idx+1) ||
                 wordSearchHelper(board,word,r,c-1,idx+1);
    board[r][c] = tmp;
    return found;
}

bool wordSearch(std::vector<std::vector<char>>& board, const std::string& word) {
    for (int r = 0; r < (int)board.size(); ++r)
        for (int c = 0; c < (int)board[0].size(); ++c)
            if (wordSearchHelper(board, word, r, c, 0)) return true;
    return false;
}

// ── Generate Parentheses ──────────────────────────────────────────────────────
std::vector<std::string> generateParentheses(int n) {
    std::vector<std::string> result;
    std::function<void(std::string, int, int)> bt = [&](std::string cur, int open, int close) {
        if ((int)cur.size() == 2*n) { result.push_back(cur); return; }
        if (open < n)    bt(cur + '(', open+1, close);
        if (close < open) bt(cur + ')', open, close+1);
    };
    bt("", 0, 0);
    return result;
}

int main() {
    // N-Queens
    NQueens nq(4);
    auto queens = nq.solve();
    std::cout << "4-Queens solutions: " << queens.size() << "\n"; // 2
    for (const auto& row : queens[0]) std::cout << row << "\n";

    // Subsets
    auto ss = subsets({1,2,3});
    std::cout << "\nSubsets of {1,2,3}: " << ss.size() << " subsets\n"; // 8

    // Combination sum
    auto cs = combinationSum({2,3,6,7}, 7);
    std::cout << "Combination sum 7:\n";
    for (auto& c : cs) { for (int x : c) std::cout << x << " "; std::cout << "\n"; }

    // Generate parentheses
    auto parens = generateParentheses(3);
    std::cout << "\nParentheses n=3: ";
    for (const auto& p : parens) std::cout << p << " ";
    std::cout << "\n";

    // Word search
    std::vector<std::vector<char>> board = {
        {'A','B','C','E'}, {'S','F','C','S'}, {'A','D','E','E'}
    };
    std::cout << "Word 'ABCCED': " << wordSearch(board, "ABCCED") << "\n"; // 1
    std::cout << "Word 'SEE': "    << wordSearch(board, "SEE")    << "\n"; // 1
    std::cout << "Word 'ABCB': "   << wordSearch(board, "ABCB")   << "\n"; // 0
    return 0;
}
