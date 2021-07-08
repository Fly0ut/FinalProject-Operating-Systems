//
// Created by fly0ut on 6/16/21.
//

#ifndef FINALPROJECT_PAGEFAULTSIM_H
#define FINALPROJECT_PAGEFAULTSIM_H
#include "typeAlias.h"
#include <vector>
#include <iomanip>
#include <list>
#include <iostream>
#include <functional>
#include <algorithm>
#include <ranges>
#include <sstream>
#include <random>

using std::vector;

class pageFaultSim {

private:

    //Page reference vector layout
    //Rerefence String, Phyical frame 0.. Phyical Frame N, Page faults, Victim frames.
    std::vector< std::vector<u16> > pageReference;
    std::vector<u16> referenceRow;
    std::vector<std::vector<std::vector<u16>>> pageReferenceTime;
    u16 currentIndex=0, physicalFrameNumber=4, virtualFrameNumber=10, referenceSize=13;

public:
    pageFaultSim();

    //Homework5 demo: runs a demo of the alogrims as stated in homework5.
    si homework5Demo();

    si pageFaultAlgoStepAPI(si algoKey);
    si pageFaultAlgoPopAPI(si algoKey);

    si setReferenceString(const std::vector<u16>& newRefString);
    std::string  getReferenceString();

    //Generate pageReference
    si zeroOutPageReference();
    si resetSim();

    si stdTemplateHW5();

    void generateRandomVector(std::vector<u16>& toFill);

    std::string generateReferenceString(int i);

    si generatePageReference(u16 reference_length, u16 virtual_frame_number, u16 phyical_frame_number);

    //Outputs
    si printReference();



    template<class algoFunc>
    si stepAlgo(algoFunc algo);
    template<class algoFunc>
    si algoPopulate(algoFunc algo);


    //Different page replacement algorithms
    si FIFO();
    si FIFOSt();
    si OPT();
    si OPTSt();
    si LRU();
    si LRUSt();
    si LFU();
    si LFUSt();

    u16 getReferenceSize() const;

};

#endif //FINALPROJECT_PAGEFAULTSIM_H
