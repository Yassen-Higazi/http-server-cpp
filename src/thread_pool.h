#pragma once
#include <queue>

#include <pthread.h>

using namespace std;

class Thread
{
public:
  Thread();

  virtual ~Thread();

  void start();

  void join();

protected:
  virtual void run() = 0;

private:
  static void *threadProc(void *param)
  {
    Thread *thread = reinterpret_cast<Thread *>(param);

    thread->run();

    return NULL;
  };

private:
  enum EState
  {
    EState_None,
    EState_Started,
    EState_Joined
  };

  EState state;
  pthread_t handle;
};

class Task
{
public:
  Task() {}
  virtual ~Task() {}
  virtual void run() = 0;
  virtual void showTask() = 0;
};

class WorkQueue
{
public:
  WorkQueue();

  ~WorkQueue();

  // Retrieves the next task from the queue
  Task *nextTask();

  // Add a task
  void addTask(Task *nt);

private:
  pthread_mutex_t qmtx;
  pthread_cond_t wcond;
  queue<Task *> tasks;
};

class PoolWorkerThread : public Thread
{
public:
  PoolWorkerThread(WorkQueue &_work_queue);

protected:
  virtual void run();

private:
  WorkQueue &work_queue;
};

class ThreadPool
{
public:
  // Allocate a thread pool and set them to work trying to get tasks
  ThreadPool(int n);

  // Wait for the threads to finish, then delete them
  ~ThreadPool();

  // Add a task
  void addTask(Task *nt);

  // Asking the threads to finish, waiting for the task
  // queue to be consumed and then returning.
  void finish();

private:
  WorkQueue workQueue;
  std::vector<PoolWorkerThread *> threads;
};

// stdout is a shared resource, so protected it with a mutex
static pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;
