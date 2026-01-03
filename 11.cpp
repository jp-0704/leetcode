//11. Container With Most Water

#include <iostream>
#include <vector>
#include <cmath>


int maxArea(std::vector<int>& height) {
        
    //Define an array
    int left = 0;
    int right = height.size()-1;
    int max_i = 0; // Calculate volume

    for(int i = 0; i<height.size();i++){
        int width = right - left;
        int h = std::min(height[right],height[left]);

        max_i = std::max(max_i,width * h);

        if(height[left] < height[right]){
            left++;
        }
        else{
            right--;
        }

    }

    return max_i;
}

int main() {
    std::vector<int> height = {1,8,6,2,5,4,8,3,7};
    std::cout << "Maximum area: " << maxArea(height) << std::endl;
    return 0;
}