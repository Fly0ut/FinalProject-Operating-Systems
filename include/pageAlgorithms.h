//
// Created by fly0ut on 7/7/21.
//

#ifndef FINALPROJECT_PAGEALGORITHMS_H
#define FINALPROJECT_PAGEALGORITHMS_H
#include "typeAlias.h"
#include <vector>
#include <ranges>
#include <limits>
#include <iterator>
#include <list>
#include <sstream>
#include <iostream>
#include <algorithm>

class pageAlgorithms {
protected:
     u16* currentIndex;
     u16* physicalFrameNumber;
    std::vector<std::vector<u16>> &pageReference;
    std::vector<u16> workingPage;

public:
    pageAlgorithms( u16* setIndex,  u16* setFPN,  std::vector<std::vector<u16>> &setPageReference) : currentIndex(setIndex), physicalFrameNumber(setFPN), pageReference(setPageReference) {};

    virtual void pushPageRef();
    virtual void startUp() = 0;
    virtual void algo() = 0;
    virtual void cleanUp() = 0;
    virtual ~pageAlgorithms() = default;
};

class FIFO : public pageAlgorithms{
public:
    using pageAlgorithms::pageAlgorithms;

    void startUp()  override;
    void algo() override;
    void cleanUp()  override;
};

class OPT : public pageAlgorithms{
    std::vector<std::vector<u16>> searchSpace;
    const u16 overFlowProtect;
public:
    OPT( u16* setIndex,  u16* setFPN, std::vector<std::vector<u16>>& setPageReference) : pageAlgorithms(setIndex, setFPN, setPageReference) , overFlowProtect((this->pageReference[0].size()+*setFPN)){}

    void pushPageRef() override;
    void startUp()  override;
    void algo() override;
    void cleanUp()  override;
};

class LFU : public pageAlgorithms{
    std::vector<std::vector<u16>> searchSpace;
public:
    using pageAlgorithms::pageAlgorithms;

    void startUp()  override;
    void algo() override;
    void cleanUp()  override;

    void pushPageRef() override;
};

class LRU : public pageAlgorithms{
    std::vector<std::vector<u16>> searchSpace;
    const u16 overFlowProtect;
public:
    LRU( u16* setIndex,  u16* setFPN, std::vector<std::vector<u16>>& setPageReference) : pageAlgorithms(setIndex, setFPN, setPageReference), overFlowProtect((this->pageReference[0].size()+*setFPN)) {};

    void startUp()  override;
    void algo() override;
    void cleanUp()  override;

    void pushPageRef() override;
};

#endif //FINALPROJECT_PAGEALGORITHMS_H
