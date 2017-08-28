#include<iostream>
#include<vector>
#include<sys/time.h>
using namespace std;
int main() {
    vector<int> intVec;
    cout << "vec size " << intVec.size() << ",capacity size " << intVec.capacity() << endl;
    intVec.push_back(1);
    cout << "vec size " << intVec.size() << ",capacity size " << intVec.capacity() << endl;
    intVec.push_back(2);
    cout << "vec size " << intVec.size() << ",capacity size " << intVec.capacity() << endl;
    intVec.push_back(3);
    cout << "vec size " << intVec.size() << ",capacity size " << intVec.capacity() << endl;
    intVec.push_back(4);
    cout << "vec size " << intVec.size() << ",capacity size " << intVec.capacity() << endl;
    intVec.push_back(5);
    cout << "vec size " << intVec.size() << ",capacity size " << intVec.capacity() << endl;
    vector<int> v1;
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);
    cout << "before use reserve" << endl;
    cout << "t1.tv_sec " << t1.tv_sec << ",t1.tv_usec " << t1.tv_usec << endl;
    for(int i = 0;i < 1000; ++i) {
       v1.push_back(i);
    }
    gettimeofday(&t2,NULL);
    cout << "t2.tv_sec " << t2.tv_sec << ",t2.tv_usec " << t2.tv_usec << endl;
    int timesub = 0;
    timesub = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
    cout << "timesub(us) " << timesub <<endl;
    vector<int> v2;
    v2.reserve(1000);
    cout << "after use reserve" << endl;
    gettimeofday(&t1,NULL);
    cout << "t1.tv_sec " << t1.tv_sec << ",t1.tv_usec " << t1.tv_usec << endl;
    for(int i = 0;i < 1000; ++i) {
       v1.push_back(i);
    }
    gettimeofday(&t2,NULL);
    cout << "t2.tv_sec " << t2.tv_sec << ",t2.tv_usec " << t2.tv_usec << endl;
    timesub = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
    cout << "timesub(us) " << timesub <<endl;
    return 0;
}
