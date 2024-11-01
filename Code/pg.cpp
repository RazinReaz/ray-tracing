#include <bits/stdc++.h>

using namespace std;
long long total_elapsed_time = 0;

int main() {
    vector<int> v;
    for (int i = 0; i < 10000; i++) {
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        v.push_back(i);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    total_elapsed_time += chrono::duration_cast<chrono::microseconds>(end - begin).count();
    }
    cout << "Time taken in total = " << total_elapsed_time << "[micro sec]" << endl;
    return 0;
}