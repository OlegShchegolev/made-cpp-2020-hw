#include <iostream>
using namespace std;

int main(void) {
    int k;
    int t;
    ptrdiff_t diff = &t - &k;
    cout << &k << " " << &t << " " << diff<<endl;
}