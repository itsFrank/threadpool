#include "threadpool.h"
#include<iostream>

using namespace std;

typedef struct {
    int i;
    double d;
} job1_params_t;


void job1(void* p) {
    job1_params_t jp = (job1_params_t) (*((job1_params_t*)p));
    cout << "From job 1: " << jp.i << " | " << jp.d << endl;
}

int main() {

    ThreadPool pool(4);

    job1_params_t jp = {12, 10.3};
    pool.addJob(job1, &jp);

    pool.joinThreads();

}