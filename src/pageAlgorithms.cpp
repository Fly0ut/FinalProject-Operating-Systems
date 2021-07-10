//
// Created by fly0ut on 7/7/21.
//

#include "pageAlgorithms.h"

void pageAlgorithms::pushPageRef() {
    for(si i=0;i<*this->physicalFrameNumber;i++){
        this->pageReference[i+1][*currentIndex] = this->workingPage[i];
    }
}

void FIFO::startUp() {
    this->workingPage.resize(*this->physicalFrameNumber, 0);
}

void FIFO::algo()  {
    bool hit = false;
    hit = (std::ranges::find(workingPage.begin(), workingPage.end(), pageReference[0][*currentIndex]) != workingPage.end());
    //Flip failed frame status bit.
    pageReference[pageReference.size()-2][*currentIndex] = hit ? 0 : 1;
    if (!hit) {
        pageReference[pageReference.size()-1][*currentIndex] = this->workingPage.back();
        this->workingPage.pop_back();
        this->workingPage.emplace(workingPage.begin(), pageReference[0][*currentIndex]);
    }
}

void FIFO::cleanUp() {
    workingPage.clear();
}

void OPT::startUp() {
    this->searchSpace.resize(2, std::vector<u16>(*this->physicalFrameNumber, 0));
    std::ranges::fill(searchSpace[1], std::numeric_limits<u16>::max()-(this->overFlowProtect));
}

void OPT::algo() {
    bool hit = false;
    size_t minnOffset;
    using nextIter = std::vector<u16>::iterator;
    nextIter found;
    nextIter minnPos;

    found = std::ranges::find(searchSpace[0], pageReference[0][*currentIndex]);
    if(found != searchSpace[0].end()) {
        searchSpace[1][std::distance(searchSpace[0].begin(), found)] = std::ranges::distance(pageReference[0].begin()+*currentIndex,
                                                           std::ranges::find(pageReference[0].begin()+*currentIndex+1, pageReference[0].end(), pageReference[0][*currentIndex]));
        hit = true;
    }else {
        minnPos = std::ranges::max_element(searchSpace[1]);
        minnOffset = std::distance(searchSpace[1].begin(), minnPos);

        pageReference[pageReference.size()-1][*currentIndex] = searchSpace[0].at(minnOffset);

        searchSpace[0][minnOffset] = pageReference[0][*currentIndex];
        searchSpace[1][minnOffset] = std::ranges::distance(pageReference[0].begin()+*currentIndex,
                                                           std::ranges::find(pageReference[0].begin()+*currentIndex+1, pageReference[0].end(), pageReference[0][*currentIndex]));
    }
    std::ranges::transform(searchSpace[1].begin(),searchSpace[1].end(), searchSpace[1].begin(), [](u16 e){ if(e==0){exit(errno);} return e-1;});
    pageReference[pageReference.size()-2][*currentIndex] = hit ? 0 : 1;
}

void OPT::pushPageRef() {
    for(si i=0;i<*this->physicalFrameNumber;i++){
        this->pageReference[i+1][*currentIndex] = this->searchSpace[0][i];
    }
}

void OPT::cleanUp() {
    this->workingPage.clear();
}

void LFU::pushPageRef() {
    for(si i=0;i<*this->physicalFrameNumber;i++){
        this->pageReference[i+1][*currentIndex] = this->searchSpace[0][i];
    }
}

void LFU::startUp() {
    this->searchSpace.resize(2, std::vector<u16>(*this->physicalFrameNumber, 0));
}

void LFU::algo() {
    bool hit = false;
    size_t minnOffset;
    using nextIter = std::vector<u16>::iterator;
    nextIter found;
    nextIter minnPos;

    found = std::ranges::find(searchSpace[0], pageReference[0][*currentIndex]);
    if(found != searchSpace[0].end()) {
        searchSpace[1][std::distance(searchSpace[0].begin(), found)]++;
        hit = true;
    }else {
        minnPos = std::ranges::min_element(searchSpace[1]);
        minnOffset = std::distance(searchSpace[1].begin(), minnPos);

        pageReference[pageReference.size()-1][*currentIndex] = searchSpace[0].at(minnOffset);

        searchSpace[0][minnOffset] = pageReference[0][*currentIndex];
        searchSpace[1][minnOffset] = 1;
    }
    pageReference[pageReference.size()-2][*currentIndex] = hit ? 0 : 1;
}

void LFU::cleanUp() {
    this->searchSpace.clear();
}

void LRU::startUp() {
    this->searchSpace.resize(2, std::vector<u16>(*this->physicalFrameNumber, 0));
    std::ranges::fill(searchSpace[1], std::numeric_limits<u16>::max()-(this->overFlowProtect));
}

void LRU::pushPageRef() {
    for(si i=0;i<*this->physicalFrameNumber;i++){
        this->pageReference[i+1][*currentIndex] = this->searchSpace[0][i];
    }
}

void LRU::algo() {
    bool hit = false;
    using nextIter = std::vector<u16>::iterator;
    nextIter minnPos;
    nextIter found;
    u16 minnOffSet = 0;

    found = std::ranges::find(searchSpace[0], pageReference[0][*currentIndex]);

    if(found != searchSpace[0].end()) {
        hit=true;
        searchSpace[1][std::ranges::distance(searchSpace[0].begin(), found)] = 0;
    }
    else {
        minnPos = std::ranges::max_element(searchSpace[1]);
        minnOffSet = std::ranges::distance(searchSpace[1].begin(), minnPos);

        this->pageReference[pageReference.size()-1][*currentIndex] = searchSpace[0][minnOffSet];

        searchSpace[0][minnOffSet] = this->pageReference[0][*currentIndex];
        searchSpace[1][minnOffSet] = 0;
    }
    pageReference[pageReference.size()-2][*currentIndex] = hit ? 0 : 1;
    std::ranges::transform(searchSpace[1].begin(),searchSpace[1].end(), searchSpace[1].begin(), [](u16 e){return e+1;});
}

void LRU::cleanUp() {
    this->workingPage.clear();
}