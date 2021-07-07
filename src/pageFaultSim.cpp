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
}

si pageFaultSim::stdTemplateHW5() {
    this->zeroOutPageReference();
    this->pageReference[0] = {2, 4, 5, 6, 1, 5, 3 ,4 ,5, 2 ,3 ,6 ,5 ,3, 4, 7, 3, 5, 6};
    std::fill_n(this->pageReference[1].begin(), 4,2);
    std::fill_n(this->pageReference[2].begin()+1, 3, 4);
    std::fill_n(this->pageReference[3].begin()+2, 2, 5);
    std::fill_n(this->pageReference[4].begin()+3, 1,6);
    std::fill_n(this->pageReference[5].begin(), 4, 1);

    currentIndex = 4;

    return 0;
}


void pageFaultSim::generateRandomVector(std::vector<u16>& toFill){
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

    return 0;
}

std::string pageFaultSim::generateReferenceString() {
    this->generateRandomVector(this->referenceRow);
    this->setReferenceString(this->referenceRow);

    return this->getReferenceString();
}

std::string pageFaultSim::getReferenceString() {
    std::stringstream referenceStream;
    std::ranges::for_each(this->pageReference[0], [&referenceStream](const u16 &e){referenceStream << e << ", ";});

    return referenceStream.str().erase(referenceStream.str().size()-2, 2);
}

si pageFaultSim::generatePageReference(u16 reference_length,u16 virtual_frame_number, u16 phyical_frame_number) {
    this->zeroOutPageReference();

    size_t height = virtual_frame_number+3;
    size_t width=reference_length;
    
    this->pageReference.resize(height, std::vector<u16>(width, 0));
    this->physicalFrameNumber = phyical_frame_number;

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

template<class algoFunc>
si pageFaultSim::stepAlgo(algoFunc algo) {


    this->printReference();

    std::string theVoid;

    while(this->currentIndex<this->pageReference[0].size()) {
        algo();
        this->printReference();
        std::cin >> theVoid;
        std::ranges::copy(this->pageReference.begin(), this->pageReference.end(), std::back_inserter(this->pageReferenceTime[this->currentIndex]));
        this->currentIndex++;
        theVoid.clear();
    }

    this->zeroOutPageReference();

    return 0;
}

template<class algoFunc>
si pageFaultSim::algoPopulate(algoFunc algo) {

    while(this->currentIndex<this->pageReference[0].size()) {
        algo();
        std::ranges::copy(this->pageReference.begin(), this->pageReference.end(), std::back_inserter(this->pageReferenceTime[this->currentIndex]));
        this->currentIndex++;
    }

    this->zeroOutPageReference();

    return 0;
}

/*
//////////////////////////
//Start Algorithms////////
//////////////////////////
*/

si pageFaultSim::FIFO() {
    bool hit = false;
    si ii=0;
    std::list<u16> line;

    for(si i=0;i<pageReference.size()-3; i++) {
        line.push_front(pageReference[i+1][currentIndex - 1]);
    }

    for(; currentIndex<this->pageReference[0].size();currentIndex++) {
        hit  = (std::ranges::find(line, pageReference[0][currentIndex]) != line.end());
        //Flip failed frame status bit.
        pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;
        if(!hit) {
            pageReference[pageReference.size()-1][currentIndex] = line.back();
            line.pop_back();
            line.push_front(pageReference[0][currentIndex]);
        }

        ii=0;
        for(u16 lit : line) {
            pageReference[ii+1][currentIndex] = lit;
            ii++;
        }
    }


    return 0;
}

si pageFaultSim::FIFOSt() {
    bool hit = false;
    si ii=0;
    static std::list<u16> line;

    if(currentIndex==0){
        line.clear();
    for(si i=0;i<pageReference.size()-3; i++) {
        line.push_front(pageReference[i+1][currentIndex-1]);
    }
    }
        hit  = (std::ranges::find(line.begin(),line.end(),pageReference[0][currentIndex])!=line.end());
        //Flip failed frame status bit.
        pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;
        if(!hit) {
            pageReference[pageReference.size()-1][currentIndex] = line.back();
            line.pop_back();
            line.push_front(pageReference[0][currentIndex]);
        }

        ii=0;
        for(u16 lit : line) {
            pageReference[ii+1][currentIndex] = lit;
            ii++;
        }

        return 0;
}



si pageFaultSim::OPT() {

    u16 replaced=0;
    bool hit = false;
    std::vector<u16> searchSpace(pageReference.size()-3, 0);
    std::vector<std::vector<u16>::iterator> nextUses;
    nextUses.resize(searchSpace.size());
    std::vector<u16> distance(searchSpace.size(), 0);
    u16 distanceBug = 0;
    u16 toPopDis = 0;
    u16 toPopIndex = 0;



        for(; currentIndex<this->pageReference[0].size();currentIndex++) {

            //Iter thought physical frames to find frame to replace
            for(si i=0;i<searchSpace.size(); i++) {
                //Checks if it is the same last time, to save on computation.
                    searchSpace[i] = pageReference[i+1][currentIndex-1];
                    nextUses[i] = std::ranges::find(pageReference[0].begin()+currentIndex, pageReference[0].end(), searchSpace[i]);
                    distance[i] = std::ranges::distance(pageReference[0].begin()+currentIndex, nextUses[i]);
                    distanceBug = distance[i];
                //If next distance is larger mark to replace that index and store new distance, if not keep the same.
                if(distance[i] > toPopDis) {
                    toPopDis = distance[i];
                    toPopIndex = i;
                }

                //If distance is zero it is hit and if it has been set true for this loop it has already hit and should stay true.
                hit = (distance[i] == 0) || hit;

                pageReference[i+1][currentIndex] = searchSpace[i];
            }
            //Offset toPopIndex by 1 since the first vector is the reference string. set it to missing frame.
            replaced = (!hit) ? pageReference[toPopIndex+1][currentIndex]: 0;
            if(!hit){
                pageReference[pageReference.size()-1][currentIndex] = pageReference[toPopIndex+1][currentIndex];
            }
            pageReference[toPopIndex+1][currentIndex] = (!hit) ? pageReference[0][currentIndex] : pageReference[toPopIndex+1][currentIndex];
                //printf("PD: %i, RI: %i, RN: %i, NN%i, SN: %i, CI: %i\n",toPopDis, toPopIndex, replaced, pageReference[toPopIndex + 1][currentIndex],
                 //      pageReference[0][currentIndex], currentIndex);
            //Flip failed frame status bit.
            pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;
            toPopDis = 0;
            toPopIndex = 0;
            hit = false;
        }

    return 0;
}

si pageFaultSim::OPTSt() {
    std::cout << "testing" << this->currentIndex << "\n";

    u16 checkNull = 1337;
    u16 target=0;
    u16 *currentNum = &checkNull;
    u16 replaced=0;
    bool hit = false;
    bool fill = false;
    u16 searchSize = this->pageReference.size()-3;
    std::vector<std::vector<u16>::iterator> nextUses;
    nextUses.resize(searchSize);
    std::vector<u16> distance(searchSize, 0);
    u16 distanceBug = 0;
    u16 toPopDis = 0;
    u16 toPopIndex = 0;

        target = this->pageReference[0][currentIndex];
        if(currentIndex==0) {
            pageReference[1][currentIndex] = target;
        }
        else {
        for(si i=0;i<searchSize; i++) {
            currentNum = &pageReference[i+1][currentIndex-1];
            if(*currentNum==0&&!hit){pageReference[i+1][currentIndex] = target;hit=true;fill=true;}
            nextUses[i] = std::ranges::find(pageReference[0].begin()+currentIndex, pageReference[0].end(), *currentNum);
            distance[i] = std::ranges::distance(pageReference[0].begin()+currentIndex, nextUses[i]);
            distanceBug = distance[i];
            //If next distance is larger mark to replace that index and store new distance, if not keep the same.
            if(distance[i] > toPopDis) {
                toPopDis = distance[i];
                toPopIndex = i;
            }
            //If distance is zero it is hit and if it has been set true for this loop it has already hit and should stay true.
            hit = (distance[i] == 0) || hit;

            if(!fill){pageReference[i+1][currentIndex] = *currentNum;}
        }
        }
        replaced = pageReference[toPopIndex+1][currentIndex];
        if(!hit) {
            pageReference[pageReference.size()-1][currentIndex] = pageReference[toPopIndex+1][currentIndex];
            pageReference[toPopIndex+1][currentIndex] = pageReference[0][currentIndex];
        }
        printf("PD: %i, RI: %i, RN: %i, NN%i, SN: %i, CI: %i\n",toPopDis, toPopIndex, replaced, pageReference[toPopIndex+1][currentIndex],
              pageReference[0][currentIndex], currentIndex);
        //Flip failed frame status bit.
        pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;

        return 0;
}


si pageFaultSim::LRU() {

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

    for(; currentIndex<this->pageReference[0].size();currentIndex++) {
        //Iter thought physical frames to find frame to replace
        for(si i=0;i<searchSpace.size(); i++) {
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
        //Offset toPopIndex by 1 since the first vector is the reference string. set it to missing frame.
        if(!hit){
            pageReference[pageReference.size()-1][currentIndex] = pageReference[toPopIndex+1][currentIndex];
        }
        pageReference[toPopIndex+1][currentIndex] = (!hit) ? pageReference[0][currentIndex] : pageReference[toPopIndex+1][currentIndex];
           // printf("PD: %i, PI: %i, NN%i, SN: %i, CI: %i\n",toPopDis, toPopIndex, pageReference[toPopIndex + 1][currentIndex],
           //        pageReference[0][currentIndex], currentIndex);
        //Flip failed frame status bit.
        pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;
        toPopDis = 0;
        toPopIndex = 0;
        hit = false;
    }

    return 0;
}

si pageFaultSim::LRUSt() {
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

        return 0;
}

si pageFaultSim::LFU() {
        bool hit = false;
        size_t minnOffset;
        //3 vectors, pos 0 has the number, post 1 has the count of number, and pos 2 has the index of the number in the paging space.
        std::vector<std::vector<u16>> search(3, std::vector<u16>(0, 0));
        using nextIter = std::vector<u16>::iterator;
        nextIter found;
        nextIter minnPos;
        u16 minn;

        for(; currentIndex<this->pageReference[0].size();currentIndex++) {
            //Iter thought physical frames to find frame to replace
            for(si i=0;i<pageReference.size()-3; i++) {
                //If next distance is larger mark to replace that index and store new distance, if not keep the same.
                pageReference[i+1][currentIndex] = pageReference[i+1][currentIndex-1];
                found = std::ranges::find(search[0], pageReference[i+1][currentIndex]);
                if(found != search[0].end()) {
                    search[1].at(std::distance(search[0].begin(), found))++;
                } else {
                    search[0].push_back(pageReference[i+1][currentIndex]);
                    search[1].push_back(1);
                    search[2].push_back(i);
                }
                //If distance is zero it is hit and if it has been set true for this loop it has already hit and should stay true.
                hit = (pageReference[i+1][currentIndex] == pageReference[0][currentIndex]) || hit;
            }
            //Offset toPopIndex by 1 since the first vector is the reference string. set it to missing frame.
            if(!hit) {
                minnPos = std::ranges::min_element(search[1]);
                minnOffset = std::distance(search[1].begin(), minnPos);
                search[1].erase(minnPos);
                minn = search[0].at(minnOffset);
                pageReference[pageReference.size()-1][currentIndex] = minn;
                search[0].erase(search[0].begin()+minnOffset);
                pageReference[search[2].at(minnOffset)][currentIndex] = pageReference[0][currentIndex];
                search[2].erase(search[2].begin()+minnOffset);
                }
            // printf("PD: %i, PI: %i, RN: %i, NN%i, SN: %i, CI: %i\n",toPopDis, toPopIndex, replaced, pageReference[toPopIndex + 1][currentIndex],
            //        pageReference[0][currentIndex], currentIndex);
            //Flip failed frame status bit.
            pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;
        }

    return 0;
}

//ToDo: make special case for zero and done.
si pageFaultSim::LFUSt() {
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

            for (int i = 0; i < searchSpace.size(); i++) {
                std::cout << "i: " << i << ", at i: " << pageReference[i + 1][currentIndex] << ", minn: " << minn
                          << "\n";
                if (pageReference[i + 1][currentIndex] == minn) {
                    pageReference[i + 1][currentIndex] = pageReference[0][currentIndex];
                }
            }

            std::cout << "Min: " << minn << ",  min offset: " << minnOffset << ", min count: " << minnCount
                      << ", searchSpace size: " << searchSpace.size() << "\n";
        }
    }

        pageReference[pageReference.size()-2][currentIndex] = hit ? 0 : 1;

        return 0;
}

/*
//////////////////////
//End Algorithms//////
/////////////////////
*/

u16 pageFaultSim::getReferenceSize() const {
    return this->referenceSize;
}

si pageFaultSim::pageFaultAlgoStepAPI(si algoKey) {

    std::function<si()> test;
    switch (algoKey) {
        case 1:
            test = std::bind(&pageFaultSim::FIFOSt, this);
            this->stepAlgo(test);
            break;
        case 2:
            test = std::bind(&pageFaultSim::OPTSt, this);
            this->stepAlgo(test);
            break;
        case 3:
            test = std::bind(&pageFaultSim::LRUSt, this);
            this->stepAlgo(test);
            break;
        case 4:
            test = std::bind(&pageFaultSim::LFUSt, this);
            this->stepAlgo(test);
            break;
        default:
            ;
    }

    return 0;
}

si pageFaultSim::pageFaultAlgoPopAPI(si algoKey) {

    std::function<si()> test;
    switch (algoKey) {
        case 1:
            test = std::bind(&pageFaultSim::FIFOSt, this);
            this->algoPopulate(test);
            break;
        case 2:
            test = std::bind(&pageFaultSim::OPTSt, this);
            this->algoPopulate(test);
            break;
        case 3:
            test = std::bind(&pageFaultSim::LRUSt, this);
            this->algoPopulate(test);
            break;
        case 4:
            test = std::bind(&pageFaultSim::LFUSt, this);
            this->algoPopulate(test);
            break;
        default:
            ;
    }

    return 0;
}

si pageFaultSim::homework5Demo() {
    //Start FIFO Demo
    std::cout << "1. TextBook Reading FIFO algorithm\n";
    this->stdTemplateHW5();
    std::cout << "Before:\n";
    this->printReference();
    this->FIFO();
    std::cout << "After:\n";
    this->printReference();
    std::cout << "\n********************\n";
    //End FIFO Demo

    //OPT Demo
    std::cout << "2. OPT page replacement algorithm.\n";
    this->stdTemplateHW5();
    std::cout << "Before:\n";
    this->printReference();
    this->OPT();
    std::cout << "After:\n";
    this->printReference();
    std::cout << "\n*************************************\n";
    //End OPT Demo

    //Start LRU Demo
    std::cout << "3. LRU page replacement algorithm.\n";
    this->stdTemplateHW5();
    std::cout << "Before:\n";
    this->printReference();
    this->LRU();
    std::cout << "After:\n";
    this->printReference();
    std::cout << "\n*******************\n";
    //End LRU Demo
    //Start LFU Demo
    std::cout << "4. LFU page replacement algorithm.\n";
    this->stdTemplateHW5();
    std::cout << "Before:\n";
    this->printReference();
    this->LFU();
    std::cout << "After:\n";
    this->printReference();
    std::cout << "\n*********************\n";
    //End LFU Demo

    return 1;
}