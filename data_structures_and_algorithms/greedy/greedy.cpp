// Greedy Algorithms — classic problems
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <numeric>

// ── Activity Selection (Interval Scheduling) ──────────────────────────────────
// Select maximum non-overlapping activities
int activitySelection(std::vector<std::pair<int,int>> activities) {
    // Sort by end time
    std::sort(activities.begin(), activities.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    int count = 1, lastEnd = activities[0].second;
    for (int i = 1; i < (int)activities.size(); ++i) {
        if (activities[i].first >= lastEnd) { ++count; lastEnd = activities[i].second; }
    }
    return count;
}

// ── Interval Merge ────────────────────────────────────────────────────────────
std::vector<std::pair<int,int>> mergeIntervals(std::vector<std::pair<int,int>> intervals) {
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::pair<int,int>> merged = {intervals[0]};
    for (auto& [s, e] : intervals) {
        if (s <= merged.back().second)
            merged.back().second = std::max(merged.back().second, e);
        else
            merged.push_back({s, e});
    }
    return merged;
}

// ── Job Scheduling (Weighted Job Scheduling) ──────────────────────────────────
// Maximize profit: each job has (start, end, profit)
struct Job { int start, end, profit; };

int weightedJobScheduling(std::vector<Job> jobs) {
    std::sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b){ return a.end < b.end; });
    int n = jobs.size();
    std::vector<int> dp(n);
    dp[0] = jobs[0].profit;

    auto latestNonConflict = [&](int i) {
        for (int j = i-1; j >= 0; --j)
            if (jobs[j].end <= jobs[i].start) return j;
        return -1;
    };

    for (int i = 1; i < n; ++i) {
        int inclProfit = jobs[i].profit;
        int l = latestNonConflict(i);
        if (l != -1) inclProfit += dp[l];
        dp[i] = std::max(dp[i-1], inclProfit);
    }
    return dp[n-1];
}

// ── Huffman Coding ────────────────────────────────────────────────────────────
struct HuffNode {
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;
    HuffNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare { bool operator()(HuffNode* a, HuffNode* b) { return a->freq > b->freq; } };

void printCodes(HuffNode* root, const std::string& code) {
    if (!root) return;
    if (!root->left && !root->right) std::cout << root->ch << ": " << code << "\n";
    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

void huffmanCoding(const std::vector<std::pair<char,int>>& chars) {
    std::priority_queue<HuffNode*, std::vector<HuffNode*>, Compare> pq;
    for (auto [c, f] : chars) pq.push(new HuffNode(c, f));
    while (pq.size() > 1) {
        HuffNode* l = pq.top(); pq.pop();
        HuffNode* r = pq.top(); pq.pop();
        HuffNode* merged = new HuffNode('\0', l->freq + r->freq);
        merged->left = l; merged->right = r;
        pq.push(merged);
    }
    std::cout << "Huffman codes:\n";
    printCodes(pq.top(), "");
}

// ── Fractional Knapsack ───────────────────────────────────────────────────────
double fractionalKnapsack(int W, std::vector<std::pair<int,int>> items) {
    // items = {weight, value}
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return (double)a.second/a.first > (double)b.second/b.first;
    });
    double total = 0;
    for (auto [w, v] : items) {
        if (W >= w) { total += v; W -= w; }
        else { total += (double)W * v / w; break; }
    }
    return total;
}

// ── Jump Game ─────────────────────────────────────────────────────────────────
bool canJump(const std::vector<int>& nums) {
    int maxReach = 0;
    for (int i = 0; i < (int)nums.size(); ++i) {
        if (i > maxReach) return false;
        maxReach = std::max(maxReach, i + nums[i]);
    }
    return true;
}

int minJumps(const std::vector<int>& nums) {
    int jumps = 0, curEnd = 0, farthest = 0;
    for (int i = 0; i < (int)nums.size()-1; ++i) {
        farthest = std::max(farthest, i + nums[i]);
        if (i == curEnd) { ++jumps; curEnd = farthest; }
    }
    return jumps;
}

// ── Gas Station ───────────────────────────────────────────────────────────────
int canCompleteCircuit(const std::vector<int>& gas, const std::vector<int>& cost) {
    int total = 0, tank = 0, start = 0;
    for (int i = 0; i < (int)gas.size(); ++i) {
        total += gas[i] - cost[i];
        tank  += gas[i] - cost[i];
        if (tank < 0) { start = i+1; tank = 0; }
    }
    return (total >= 0) ? start : -1;
}

// ── Task Scheduler ─────────────────────────────────────────────────────────────
int taskScheduler(const std::vector<char>& tasks, int n) {
    std::vector<int> freq(26, 0);
    for (char t : tasks) ++freq[t - 'A'];
    int maxFreq = *std::max_element(freq.begin(), freq.end());
    int maxCount = std::count(freq.begin(), freq.end(), maxFreq);
    return std::max((int)tasks.size(), (maxFreq-1) * (n+1) + maxCount);
}

// ── Meeting Rooms II (min meeting rooms) ─────────────────────────────────────
int minMeetingRooms(std::vector<std::pair<int,int>> intervals) {
    std::vector<int> starts, ends;
    for (auto [s, e] : intervals) { starts.push_back(s); ends.push_back(e); }
    std::sort(starts.begin(), starts.end());
    std::sort(ends.begin(), ends.end());
    int rooms = 0, maxRooms = 0, j = 0;
    for (int i = 0; i < (int)starts.size(); ++i) {
        if (starts[i] < ends[j]) ++rooms;
        else { --rooms; ++j; }
        maxRooms = std::max(maxRooms, rooms);
    }
    return maxRooms;
}

int main() {
    // Activity selection
    std::vector<std::pair<int,int>> acts = {{1,2},{3,4},{0,6},{5,7},{8,9},{5,9}};
    std::cout << "Max activities: " << activitySelection(acts) << "\n"; // 4

    // Merge intervals
    auto merged = mergeIntervals({{1,3},{2,6},{8,10},{15,18}});
    std::cout << "Merged intervals: ";
    for (auto [s,e] : merged) std::cout << "[" << s << "," << e << "] "; std::cout << "\n";

    // Fractional knapsack
    std::cout << "Fractional knapsack: "
              << fractionalKnapsack(50, {{10,60},{20,100},{30,120}}) << "\n"; // 240

    // Jump game
    std::cout << "Can jump [2,3,1,1,4]: " << canJump({2,3,1,1,4}) << "\n"; // 1
    std::cout << "Min jumps [2,3,1,1,4]: " << minJumps({2,3,1,1,4}) << "\n"; // 2

    // Gas station
    std::cout << "Gas station start: "
              << canCompleteCircuit({1,2,3,4,5},{3,4,5,1,2}) << "\n"; // 3

    // Huffman
    huffmanCoding({{'a',5},{'b',9},{'c',12},{'d',13},{'e',16},{'f',45}});

    // Task scheduler
    std::cout << "Task scheduler ['A','A','A','B','B','B'] n=2: "
              << taskScheduler({'A','A','A','B','B','B'}, 2) << "\n"; // 8

    // Meeting rooms
    std::cout << "Meeting rooms: "
              << minMeetingRooms({{0,30},{5,10},{15,20}}) << "\n"; // 2
    return 0;
}
