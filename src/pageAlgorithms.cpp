//
// Created by fly0ut on 7/7/21.
//

#include "pageAlgorithms.h"


void FIFO::startUp() const{

}
void FIFO::algo() const{

}
void FIFO::cleanUp() const{

}

void OPT::startUp() const{

}
void OPT::algo() const{

}
void OPT::cleanUp() const{

}

void LFU::startUp() const{

}
void LFU::algo() const{
    bool hit = false;
    size_t minnOffset;
    static std::vector<u16> searchSpace;
    static std::vector<u16> searchCount;
    using nextIter = std::vector<u16>::iterator;
    std::vector<std::vector<u16>> search(3, std::vector<u16>(0, 0));
    nextIter found;
    nextIter minnPos;
    u16 minn;

    if(currentIndex==0){
        searchSpace.clear();
        searchCount.clear();
    }

    for(si i=0;i<pageReference.size()-3; i++) {
        pageReference[i+1][currentIndex] = pageReference[i+1][currentIndex-1];
        found = std::ranges::find(searchSpace, pageReference[i+1][currentIndex]);
        if(found != searchSpace.end()) {
            searchCount.at(std::distance(searchSpace.begin(), found))++;
        } else if(pageReference[i+1][currentIndex] != 0){
            searchSpace.push_back(pageReference[i+1][currentIndex]);
            searchCount.push_back(1);
        }
        hit = (pageReference[i+1][currentIndex] == pageReference[0][currentIndex]) || hit;
    }
    if(searchSpace.size() < physicalFrameNumber) {
        pageReference[searchSpace.size()+1][currentIndex] = pageReference[0][currentIndex];
    }
    else {
        if (!hit) {
            minnPos = std::ranges::min_element(searchCount);
            int minnCount = *minnPos;
            minnOffset = std::distance(searchCount.begin(), minnPos);
            searchCount.erase(minnPos);
            minn = searchSpace.at(minnOffset);
            pageReference[pageReference.size() - 1][currentIndex] = minn;
            searchSpace.erase(searchSpace.begin() + minnOffset);

            for (int i = 0; i < searchSpace.size()+1; i++) {
                if (pageReference[i + 1][currentIndex] == minn) {
                    pageReference[i + 1][currentIndex] = pageReference[0][currentIndex];
                    searchSpace.push_back(pageReference[i+1][currentIndex]);
                    searchCount.push_back(1);
                }
            }

            std::cout << "Min: " << minn << ",  min offset: " << minnOffset << ", min count: " << minnCount
                      << ", searchSpace size: " << searchSpace.size() << "\n";
        }
    }

    pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;

}
void LFU::cleanUp() const{

}

void LRU::startUp() const{

}
void LRU::algo() const{
    bool hit = false;
    std::vector<u16> searchSpace(pageReference.size()-3, 0);
    using nextIter = std::vector<u16>::iterator;
    using testNextIter = std::vector<u16>::reverse_iterator;
    std::vector<nextIter> nextUses;
    std::vector<testNextIter> testNextUses;
    nextUses.resize(searchSpace.size());
    testNextUses.resize(searchSpace.size());
    std::vector<u16> distance(searchSpace.size(), 0);
    si distanceBug = 0;
    nextIter nextUseBug;
    u16 toPopDis = 0;
    u16 toPopIndex = 0;

    //Iter thought physical frames to find frame to replace
    for(si i=0;i<searchSpace.size(); i++) {
        //Checks if it is the same last time, to save on computation.
        searchSpace[i] = pageReference[i+1][currentIndex-1];
        nextUses[i] = (std::ranges::find(pageReference[0].rbegin()+(pageReference[0].size()-currentIndex),pageReference[0].rend(), searchSpace[i])).base();
        testNextUses[i] = (std::find((pageReference[0].rbegin()+((int)pageReference[0].size()-(int)currentIndex)),pageReference[0].rend(), searchSpace[i]));
        nextUseBug = nextUses[i];
        distanceBug = std::distance(pageReference[0].rend()-currentIndex-1,testNextUses[i]);
        distance[i] = abs(distanceBug);
        //If next distance is larger mark to replace that index and store new distance, if not keep the same.
        if(distance[i] > toPopDis) {
            toPopDis = distance[i];
            toPopIndex = i;
        }
        //If distance is zero it is hit and if it has been set true for this loop it has already hit and should stay true.
        hit = (searchSpace[i] == pageReference[0][currentIndex]) || hit;

        pageReference[i+1][currentIndex] = searchSpace[i];
    }
    if(!hit){
        pageReference[pageReference.size()-1][currentIndex] = pageReference[toPopIndex+1][currentIndex];
    }
    pageReference[toPopIndex+1][currentIndex] = (!hit) ? pageReference[0][currentIndex] : pageReference[toPopIndex+1][currentIndex];
    // printf("PD: %i, PI: %i, RN: %i, NN%i, SN: %i, CI: %i\n",toPopDis, toPopIndex, replaced, pageReference[toPopIndex + 1][currentIndex],
    //        pageReference[0][currentIndex], currentIndex);
    //Flip failed frame status bit.
    pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;

}
void LRU::cleanUp() const{

}