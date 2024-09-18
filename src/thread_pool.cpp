#include <stdio.h>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#include "headers/thread_pool.h"

// Reusable thread class
Thread::Thread()
{
  state = EState_None;
  handle = 0;
}

Thread::~Thread()
{
  assert(state != EState_Started);
}

void Thread::start()
{
  assert(state == EState_None);

  // in case of thread create error I usually FatalExit...
  if (pthread_create(&handle, NULL, threadProc, this))
    abort();

  state = EState_Started;
}

void Thread::join()
{
  // A started thread must be joined exactly once!
  // This requirement could be eliminated with an alternative implementation but it isn't needed.
  assert(state == EState_Started);

  pthread_join(handle, NULL);

  state = EState_Joined;
}

// Wrapper around std::queue with some mutex protection
WorkQueue::WorkQueue()
{
  pthread_mutex_init(&qmtx, 0);

  // wcond is a condition variable that's signaled
  // when new work arrives
  pthread_cond_init(&wcond, 0);
}

WorkQueue::~WorkQueue()
{
  // Cleanup pthreads
  pthread_mutex_destroy(&qmtx);
  pthread_cond_destroy(&wcond);
}

// Retrieves the next task from the queue
Task *WorkQueue::nextTask()
{
  // The return value
  Task *nt = 0;

  // Lock the queue mutex
  pthread_mutex_lock(&qmtx);

  while (tasks.empty())
    pthread_cond_wait(&wcond, &qmtx);

  nt = tasks.front();

  tasks.pop();

  // Unlock the mutex and return
  pthread_mutex_unlock(&qmtx);

  return nt;
}

// Add a task
void WorkQueue::addTask(Task *nt)
{
  // Lock the queue
  pthread_mutex_lock(&qmtx);

  // Add the task
  tasks.push(nt);

  // signal there's new work
  pthread_cond_signal(&wcond);

  // Unlock the mutex
  pthread_mutex_unlock(&qmtx);
}

// Thanks to the reusable thread class implementing threads is
// simple and free of pthread api usage.

PoolWorkerThread::PoolWorkerThread(WorkQueue &_work_queue) : work_queue(_work_queue) {}

void PoolWorkerThread::run()
{
  while (Task *task = work_queue.nextTask())
    task->run();
}

// Allocate a thread pool and set them to work trying to get tasks
ThreadPool::ThreadPool(int n)
{
  printf("Creating a thread pool with %d threads\n", n);
  for (int i = 0; i < n; ++i)
  {
    threads.push_back(new PoolWorkerThread(workQueue));
    threads.back()->start();
  }
}

// Wait for the threads to finish, then delete them
ThreadPool::~ThreadPool()
{
  finish();
}

// Add a task
void ThreadPool::addTask(Task *nt)
{
  workQueue.addTask(nt);
}

// Asking the threads to finish, waiting for the task
// queue to be consumed and then returning.
void ThreadPool::finish()
{
  for (size_t i = 0, e = threads.size(); i < e; ++i)
    workQueue.addTask(NULL);
  for (size_t i = 0, e = threads.size(); i < e; ++i)
  {
    threads[i]->join();
    delete threads[i];
  }
  threads.clear();
}