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

void vsum(void* p) {
    vsum_params_t* vp = (vsum_params_t*) p;

    for (int i = vp->start; i < vp->start + vp->count; i++) {
        (*vp->c)[i] = vp->a->at(i) + vp->b->at(i);
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

    vsum_params_t vp1 = {&a, &b, &c, 0, 4};
    vsum_params_t vp2 = {&a, &b, &c, 5, 4};
    vsum_params_t vp3 = {&a, &b, &c, 10, 4};
    vsum_params_t vp4 = {&a, &b, &c, 15, 4};
    
    pool.addJob(vsum, &vp1);
    pool.addJob(vsum, &vp2);
    pool.addJob(vsum, &vp3);
    pool.addJob(vsum, &vp4);

    while(pool.jobsRemaining());

    pool.joinThreads();


    for (int i = 0; i < 20; i++) {
        cout << a[i] << " + " << b[i] << " = " << c[i] << endl;
    }
}