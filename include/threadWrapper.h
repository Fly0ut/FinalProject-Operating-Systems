//
// Created by fly0ut on 6/8/21.
//

#ifndef UNTITLED_THREADWRAPPER_H
#define UNTITLED_THREADWRAPPER_H
#include <future>
#include <iostream>
#include <fstream>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <ctime>
#include <condition_variable>
#include <functional>
#include <chrono>
#include "typeAlias.h"

using namespace std::chrono_literals;

template<typename  funcation>
struct job {
    si ID;
    si *sig;
    //read only
    si *resources;
    std::condition_variable *CV;
    std::thread *thread;
    std::condition_variable *masterWake;

    job(si setID, si *setSig, si *setResources,
        std::condition_variable *setCV, std::thread *setThread, funcation setFunc,
        std::condition_variable *setMasterWake);

    //Actually function for the job to compute
    funcation func;
};

class threadBAWrapper {
private:
    //Resources type count
    const static si R=10;
    //Process count
    const static si P=10;
    //signal for threads to communicate global states.
    si signal;
    //Real running processes
    si runningProcs;
    //Active/queued jobs
    si jobsCount;
    //CV for sleep for resource free
    std::mutex freeMut;
    std::unique_lock<std::mutex> *freeLock;
    std::condition_variable freeCV;
    //Condition variable locks for worker threads to use. Can be modified by
    std::condition_variable procCV[P];
    std::mutex cvMut[P];
    std::unique_lock<std::mutex> *uniCV[P];
    //Mutexes for the scheduler to handle the threads. Will not be touched by worker threads
    std::mutex threadMut[P];
    //Limit on resources

    //Mine
    std::thread *workPool[P];

    std::vector<si> inputs;


public:
    threadBAWrapper();

private:

    template<typename funcation>
    si addWorker(funcation toDo);
    si delWorker();
    si handleWorkerErr();

    si jobQueue(si newResourceRequest[threadBAWrapper::R]);
    template<typename funcation>
    void *jobExec(job<funcation> jobExec);
/*
    si jobFinish(job jobFinish);
    si jobWait(job jobWait);
    si jobAbort(job jobAbort);
    si jobHandleErr(job jobErr);
*/
    bool safetyBA();
};

#endif //UNTITLED_THREADWRAPPER_H
