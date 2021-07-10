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
#include "pageAlgorithms.h"

using std::vector;

class pageFaultSim {

private:
    bool setReferenceRow = false;


public:
    //Page reference vector layout
    //Rerefence String, Phyical frame 0.. Phyical Frame N, Page faults, Victim frames.
    std::vector< std::vector<u16> > pageReference;
    std::vector<u16> referenceRow;
    std::vector<std::vector<std::vector<u16>>> pageReferenceTime;
    u16 currentIndex=0, physicalFrameNumber=4, virtualFrameNumber=10, referenceSize=13;
    pageFaultSim();
    //API to allow external functions to call functions by passing a number, instead of creating templates and binding their own objects.
    si pageFaultAlgoStepAPI(si algoKey);
    si pageFaultAlgoPopAPI(si algoKey);

    si setReferenceString(const std::vector<u16>& newRefString);
    std::string  getReferenceString();
    //Generate pageReference
    si zeroOutPageReference();
    si resetSim();
    //Generating functions for reference strings and page references.
    void generateRandomVector(std::vector<u16>& toFill) const;
    std::string generateReferenceString(int i);
    si generatePageReference(u16 reference_length, u16 virtual_frame_number, u16 physical_frame_number);
    //Outputs
    si printReference();
    si printTallyFaults();
    //Functions that allow a tempted loop that will allow functions/classes algorithms to interact with this class.
    template<class algoFunc>
    si stepAlgoClass(algoFunc algo);
    template<class algoFunc>
    si popAlgoClass(algoFunc algo);
    //Getter for reference size.
    u16 getReferenceSize() const;
};

#endif //FINALPROJECT_PAGEFAULTSIM_H
