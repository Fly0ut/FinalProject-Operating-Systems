//
// Created by fly0ut on 6/16/21.
//

#include "../include/pageFaultSim.h"

    pageFaultSim::pageFaultSim(){
    pageFaultSim::zeroOutPageReference();
    this->referenceRow.resize(this->referenceSize, 0);
}

si pageFaultSim::zeroOutPageReference() {
    pageReference.clear();
    this->currentIndex = 0;
    pageReference.resize(this->physicalFrameNumber+3, std::vector<u16>(this->referenceSize, 0));
    pageReferenceTime.resize(this->referenceSize, std::vector<std::vector<u16>>(this->physicalFrameNumber+3, std::vector<u16>(this->referenceSize, 0)));
    return  0;
}

si pageFaultSim::resetSim() {
    this->setReferenceString(this->referenceRow);
    return 0;
}

void pageFaultSim::generateRandomVector(std::vector<u16>& toFill) const {
    std::random_device rd;
    std::mt19937 mtGen(rd());
    std::uniform_int_distribution<u16> dist(1, this->virtualFrameNumber);

    auto gen = [&dist, &mtGen](){
        return dist(mtGen);
    };

    std::ranges::generate(toFill, gen);
}

si pageFaultSim::setReferenceString(const std::vector<u16>& newRefString) {
    this->referenceRow = newRefString;
    this->referenceSize = this->referenceRow.size();
    this->zeroOutPageReference();
    this->pageReference[0] = this->referenceRow;

    this->currentIndex=0;

    this->setReferenceRow = true;

    return 0;
}

std::string pageFaultSim::generateReferenceString(int i) {
    this->referenceSize=i;
    this->referenceRow.resize(i, 0);
    this->zeroOutPageReference();
    this->generateRandomVector(this->referenceRow);
    this->setReferenceString(this->referenceRow);

    return this->getReferenceString();
}

std::string pageFaultSim::getReferenceString() {
    std::stringstream referenceStream;

    if(!setReferenceRow) {
        std::cout << "Error: reference row not set!\n";
        return "ERROR";
    }

    std::ranges::for_each(this->pageReference[0], [&referenceStream](const u16 &e){referenceStream << e << ", ";});

    return referenceStream.str().erase(referenceStream.str().size()-2, 2);
}

si pageFaultSim::generatePageReference(u16 reference_length,u16 virtual_frame_number, u16 physical_frame_number) {
    this->zeroOutPageReference();

    size_t height = virtual_frame_number+3;
    size_t width=reference_length;
    
    this->pageReference.resize(height, std::vector<u16>(width, 0));
    this->physicalFrameNumber = physical_frame_number;

    return 0;
}

si pageFaultSim::printReference() {
    std::stringstream u16VecOut;
    auto printU16Vec = [&u16VecOut](const std::vector<u16>& v)
        {
            std::ranges::for_each(v,
                          [&u16VecOut](const u16& n) {
                                u16VecOut << std::setw(3) << "|" << std::setw(2) << n/*-1*/ <<"|";
                         }
            );
            u16VecOut <<"\n";
        };
    std::ranges::for_each(this->pageReference, printU16Vec);
    std::cout << u16VecOut.rdbuf();

    return 0;
}

si pageFaultSim::printTallyFaults() {
    si faults = std::ranges::count(this->pageReference[pageReference.size()-2], 1);

    std::cout << "Number of faults in this sim was: " << faults << std::endl;

    return 0;
}

template<class algoFunc>
si pageFaultSim::popAlgoClass(algoFunc algo) {
    algo.startUp();
    while(this->currentIndex<this->pageReference[0].size()) {
        algo.algo();
        algo.pushPageRef();
        std::ranges::copy(this->pageReference.begin(), this->pageReference.end(), std::back_inserter(this->pageReferenceTime[this->currentIndex]));
        this->currentIndex++;
    }
    algo.cleanUp();

    return 0;
}

template<class algoClass>
si pageFaultSim::stepAlgoClass(algoClass algo) {
    std::string theVoid;

    algo.startUp();
    this->printReference();

    while(this->currentIndex<this->pageReference[0].size()) {
        std::cin >> theVoid;
        algo.algo();
        algo.pushPageRef();
        this->printReference();
        this->currentIndex++;
        theVoid.clear();
    }
    algo.cleanUp();

    this->printTallyFaults();
    this->resetSim();

    return 0;
}

u16 pageFaultSim::getReferenceSize() const {
    return this->referenceSize;
}

si pageFaultSim::pageFaultAlgoStepAPI(si algoKey) {
    if(!setReferenceRow) {
        std::cout << "Error: Reference row not set!\n";
        return 1;
    }

    class FIFO fifoClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);
    class OPT optClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);
    class LFU lfuClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);
    class LRU lruClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);

    switch (algoKey) {
        case 1:
            this->stepAlgoClass(fifoClass);
            break;
        case 2:
            this->stepAlgoClass(optClass);
            break;
        case 3:
            this->stepAlgoClass(lruClass);
            break;
        case 4:
            this->stepAlgoClass(lfuClass);
            break;
        default:
            ;
    }

    return 0;
}
si pageFaultSim::pageFaultAlgoPopAPI(si algoKey) {
    if(!setReferenceRow) {
        std::cout << "Error: Reference row not set!\n";
        return 1;
    }

    class FIFO fifoClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);
    class OPT optClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);
    class LFU lfuClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);
    class LRU lruClass(&this->currentIndex, &this->physicalFrameNumber, this->pageReference);

    switch (algoKey) {
        case 1:
            this->popAlgoClass(fifoClass);
            break;
        case 2:
            this->popAlgoClass(optClass);
            break;
        case 3:
            this->popAlgoClass(lruClass);
            break;
        case 4:
            this->popAlgoClass(lfuClass);
            break;
        default:
            ;
    }
    return 0;
}