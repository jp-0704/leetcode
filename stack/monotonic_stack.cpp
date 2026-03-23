// Monotonic Stack — classic applications
// 1. Next Greater Element
// 2. Largest Rectangle in Histogram
// 3. Daily Temperatures
#include <iostream>
#include <vector>
#include <stack>

// Next Greater Element: for each element, find the first greater element to its right
std::vector<int> nextGreaterElement(const std::vector<int>& nums) {
    int n = nums.size();
    std::vector<int> result(n, -1);
    std::stack<int> st; // stores indices
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[st.top()] < nums[i]) {
            result[st.top()] = nums[i];
            st.pop();
        }
        st.push(i);
    }
    return result;
}

// Largest Rectangle in Histogram
int largestRectangle(const std::vector<int>& heights) {
    std::stack<int> st;
    int maxArea = 0;
    int n = heights.size();
    for (int i = 0; i <= n; ++i) {
        int h = (i == n) ? 0 : heights[i];
        while (!st.empty() && heights[st.top()] > h) {
            int height = heights[st.top()]; st.pop();
            int width = st.empty() ? i : i - st.top() - 1;
            maxArea = std::max(maxArea, height * width);
        }
        st.push(i);
    }
    return maxArea;
}

// Daily Temperatures: days until a warmer temperature
std::vector<int> dailyTemperatures(const std::vector<int>& temps) {
    int n = temps.size();
    std::vector<int> result(n, 0);
    std::stack<int> st;
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && temps[st.top()] < temps[i]) {
            result[st.top()] = i - st.top();
            st.pop();
        }
        st.push(i);
    }
    return result;
}

int main() {
    std::vector<int> nums = {2, 1, 2, 4, 3};
    auto nge = nextGreaterElement(nums);
    std::cout << "Next Greater: ";
    for (int x : nge) std::cout << x << " "; // 4 2 4 -1 -1
    std::cout << "\n";

    std::vector<int> heights = {2, 1, 5, 6, 2, 3};
    std::cout << "Largest Rectangle: " << largestRectangle(heights) << "\n"; // 10

    std::vector<int> temps = {73, 74, 75, 71, 69, 72, 76, 73};
    auto days = dailyTemperatures(temps);
    std::cout << "Daily Temps: ";
    for (int x : days) std::cout << x << " "; // 1 1 4 2 1 1 0 0
    std::cout << "\n";
    return 0;
}
