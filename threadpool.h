#include <queue>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define JOB_CHECK_DELAY 100

using namespace std;

typedef void job_funct_t(void*);

typedef struct {
    void (*f)(void*);
    void* params;
} job_t;


class ThreadPool
{
private:
    /* data */
    int thread_count;
    bool join_threads = false;

    vector<int> join_ids;
    vector<int> thread_ids;
    vector<pthread_t> threads;

    pthread_mutex_t queue_mutex;

    queue<job_t> job_queue;

public:
    ThreadPool(int);
    ~ThreadPool();
    void addJob(job_funct_t, void*);
    void fetchJob();
    void joinThreads();
    void* threadLoop(void*);
};

ThreadPool::ThreadPool(int thread_count)
{
    this->thread_count = thread_count;
    this->queue_mutex = PTHREAD_MUTEX_INITIALIZER;

    this->threads.resize(thread_count);
    
    for (int i = 0; i < thread_count; i++) {
        this->thread_ids.push_back(i);
        int jid = pthread_create(&this->threads[i], NULL, threadLoop, &this->thread_ids[i]);
        this->join_ids.push_back(jid);
    }
}

void ThreadPool::addJob(void (*f)(void*), void* params) {
    job_t job;
    job.f = f;
    job.params = params;

    pthread_mutex_lock(&this->queue_mutex);

    this->job_queue.push(job);

    pthread_mutex_unlock(&this->queue_mutex);
}

void ThreadPool::fetchJob() {
    bool job_fetched = false;

    pthread_mutex_lock(&this->queue_mutex);
    
    if (this->job_queue.size > 0) {
        job_t job = this->job_queue.front();
        this->job_queue.pop();
        job_fetched = true;
    }

    pthread_mutex_unlock(&this->queue_mutex);

    if (job_fetched) job.f(job.params);
}

void ThreadPool::joinThreads(void* args) {
    this->join_threads = true;

    // For join...
}

void* ThreadPool::threadLoop(void* args) {
    int tid = *((int*)args);
    
    cout << "Thread " << tid << " started" << endl;
    while (!this->join_threads) {
        this->fetchJob();
        // Wait delay
    }
    
    cout << "Thread " << tid << " stopped" << endl;
}

ThreadPool::~ThreadPool()
{
}
