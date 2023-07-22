#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <thread>

using namespace std;

void getsum(vector<int>::iterator first,vector<int>::iterator last,int &result){
    result=accumulate(first,last,0);
}
int main(){
    int result1,result2,result3,result4,result5;
    vector<int> arr;
    
    for(int i=0;i<100000000;i++){
        if(i%2==0){
            arr.push_back(i);
        }
        else{
            arr.push_back(-1*i);
        }
    }
    auto start=chrono::high_resolution_clock::now();
    thread first(getsum,arr.begin(),arr.begin()+20000000,ref(result1));
    thread second(getsum,arr.begin()+20000000,arr.begin()+40000000,ref(result2));
    thread thrid(getsum,arr.begin()+40000000,arr.begin()+60000000,ref(result3));
    thread fouth(getsum,arr.begin()+60000000,arr.begin()+80000000,ref(result4));
    thread fifth(getsum,arr.begin()+80000000,arr.begin()+100000000,ref(result5));
    first.join();
    second.join();
    thrid.join();
    fouth.join();
    fifth.join();
    int sum=result1+result2+result3+result4+result5;

    auto end=chrono::high_resolution_clock::now();
    chrono::duration<double> duration=end-start;

    cout<<"sum:"<<sum<<endl;
    std::cout << "程序运行时间: " << duration.count() << " 秒" << std::endl;
    return 0;
}