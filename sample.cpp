#include "threadpool.h"
#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    int i;
    double d;
} job1_params_t;

typedef struct {
    vector<int>* a;
    vector<int>* b;
    vector<int>* c;
    int start;
    int count;
} vsum_params_t;

void job1(void* p) {
    job1_params_t jp = (job1_params_t) (*((job1_params_t*)p));
    cout << "From job 1: " << jp.i << " | " << jp.d << endl;
}

void vsum(vector<int>* a, vector<int>* b, vector<int>* c, int start, int count) {
    for (int i = start; i < start + count; i++) {
        (*c)[i] = a->at(i) + b->at(i);
    }
}

int main() {

    ThreadPool pool(4);

    vector<int> a(20);
    vector<int> b(20);
    vector<int> c(20);

    for (int i = 0; i < 20; i++) {
        a[i] = i;
        b[i] = 2*i;
    }

    pool.addJob(vsum, &a, &b, &c, 0, 4);
    pool.addJob(vsum, &a, &b, &c, 5, 4);
    pool.addJob(vsum, &a, &b, &c, 10, 4);
    pool.addJob(vsum, &a, &b, &c, 15, 4);

    pool.waitComplete();

    for (int i = 0; i < 20; i++) {
        cout << a[i] << " + " << b[i] << " = " << c[i] << endl;
    }

    pool.joinThreads();
}