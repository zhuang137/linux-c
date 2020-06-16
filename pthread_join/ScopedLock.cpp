#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;
class ScopedLock {
public:
    ScopedLock(pthread_mutex_t &mutex):_mutex(mutex) {
        pthread_mutex_lock(&_mutex);
    }
    ~ScopedLock() {
        pthread_mutex_unlock(&_mutex);
    }

private:
    pthread_mutex_t& _mutex;

    ScopedLock(const ScopedLock&) = delete;
    void operator=(const ScopedLock&) = delete;
};

typedef void* (*pthread_entry)(void*);
static int createDetachThread(pthread_t* new_thread, pthread_entry entry, void* params) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(new_thread, &attr, entry, params);
    pthread_attr_destroy(&attr);
    if (ret != 0) {
        //LOGE("createDetachThread failed for: %s", strerror(errno));
        printf("createDetachThread failed for:%s\n", strerror(errno));
        return 0;
    }
    printf("createDetachThread success %d\n", gettid());
    return ret;
}

static void* testThread(void* params) {
    ScopedLock _lock(gMutex);
    printf("tid->%d testThread begin\n", gettid());

    for (int i=0; i<10; i++) {
        sleep(1);
        printf("sleep-------for tid->%d\n", gettid());
    }
    printf("tid testThread exit\n");
}

void onMainExit() {
    printf("main exit....\n");
}

int main() {
    atexit(onMainExit);
    pthread_t xx1{0}, xx2{0}, xx3{3};
    createDetachThread(&xx1, testThread, NULL);
    createDetachThread(&xx2, testThread, NULL);
    createDetachThread(&xx3, testThread, NULL);
    
    pthread_join(xx1, NULL);
    pthread_join(xx2, NULL);
    pthread_join(xx3, NULL);
    
    return 0;
}
