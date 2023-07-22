#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>

using namespace std;

int getsum(vector<int>::iterator first,vector<int>::iterator last){
    return accumulate(first,last,0);
}
int main(){
    vector<int> arr;
    
    for(int i=0;i<100000000;i++){
        if(i%2==0){
            arr.push_back(i);
        }
        else{
            arr.push_back(-1*i);
        }
    }
    // 记录程序开始时间
    auto start = std::chrono::high_resolution_clock::now();
    int res=getsum(arr.begin(),arr.end());

    // 记录程序结束时间
    auto end = std::chrono::high_resolution_clock::now();
    // 计算运行时间，单位为秒（也可以用其他时间单位）
    std::chrono::duration<double> duration = end - start;

    cout<<"sum:"<<res<<endl;

    // 输出运行时间
    std::cout << "程序运行时间: " << duration.count() << " 秒" << std::endl;
    return 0;
}