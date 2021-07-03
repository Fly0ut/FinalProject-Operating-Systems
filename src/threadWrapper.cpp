//
// Created by fly0ut on 6/8/21.
//

#include "../include/threadWrapper.h"

//Job struct code

template<typename funcation>
job<funcation>::job(si setID, si *setSig, si *setResources,
std::condition_variable *setCV, std::thread *setThread, funcation setFunc, std::condition_variable *setMasterWake){
    this->ID = setID;
    this->sig = setSig;
    this->resources = setResources;
    this->CV = setCV;
    this->thread = setThread;
    this->func = setFunc;
    this->masterWake = setMasterWake;

}

//End Job struct code

#include "../include/threadWrapper.h"

    threadBAWrapper::threadBAWrapper() {
    signal=0;
    runningProcs=0;
    //Init a  unique lock for the freeLock ot be pointed to.
    std::unique_lock<std::mutex> createFreeLock(freeMut);
    freeLock = &createFreeLock;

    si fakeRequest[threadBAWrapper::R] = {5, 2, 3, 5, 0, 2, 1 ,2, 5 ,0};
    jobQueue(fakeRequest);
}

template<typename funcation>
si threadBAWrapper::addWorker(funcation toDo) {

        this->threadMut[runningProcs].lock();
        std::unique_lock<std::mutex> newUniCV(cvMut[runningProcs]);
       uniCV[runningProcs] = &newUniCV;

        std::thread newThread(sampleWorkLoad, 1);
        this->workPool[this->runningProcs] = &newThread;

    job<funcation> createJob(this->runningProcs,
                             &this->signal,
                             this->allocation[runningProcs],
                             &this->procCV[runningProcs],
                             this->workPool[this->runningProcs],
                             toDo,
                             &this->procCV[this->runningProcs]);

        this->runningProcs++;
        this->threadMut[runningProcs].unlock();



    return 1;
}

si threadBAWrapper::delWorker() {

    this->runningProcs--;

    return 1;
}

si threadBAWrapper::handleWorkerErr() {

    return 1;
}

si threadBAWrapper::addResource() {

    return 1;
}

si threadBAWrapper::delResource() {


    return 1;
}

si threadBAWrapper::jobQueue(si newResourceRequest[threadBAWrapper::R]){
    if(this->resourceRequest(newResourceRequest) != 1) {
        std::cout << "Job request failed. Need more resources";
        return 0;
    }

    auto now = std::chrono::system_clock::now();
    //Creating local references for the lambda pred input.
    si *procs = &this->runningProcs;
    si max= this->P;
    while(freeCV.wait_for(*this->freeLock, std::chrono::microseconds(20), [procs, max](){return *procs > max;})) {

    }

    this->addWorker(0);

    return 1;
}

template<typename funcation>
void *threadBAWrapper::jobExec(job<funcation> jobExec) {
    return (void *)&(jobExec.func());
}