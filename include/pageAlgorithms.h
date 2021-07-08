//
// Created by fly0ut on 7/7/21.
//

#ifndef FINALPROJECT_PAGEALGORITHMS_H
#define FINALPROJECT_PAGEALGORITHMS_H
#include "typeAlias.h"
#include <vector>
#include <ranges>
#include <iterator>
#include <list>
#include <sstream>
#include <iostream>
#include <algorithm>

class pageAlgorithms {
protected:
const u16* currentIndex;
const std::vector<std::vector<u16>> *pageReference;
const u16* physicalFrameNumber;

public:
    pageAlgorithms(const u16* setIndex, const u16* setFPN, const std::vector<std::vector<u16>> *setPageReference) : currentIndex(setIndex), physicalFrameNumber(setFPN), pageReference(setPageReference) {};

    virtual void startUp() const;
    virtual void algo() const;
    virtual void cleanUp() const;
};

class FIFO : public pageAlgorithms{

public:
    FIFO(const u16* setIndex,const std::vector<std::vector<u16>>* setPageReference) : pageAlgorithms(setIndex, setPageReference) {};

    void startUp() const override;
    void algo() const override;
    void cleanUp() const override;
};

class OPT : public pageAlgorithms{

public:
    OPT(const u16* setIndex,const std::vector<std::vector<u16>>* setPageReference) : pageAlgorithms(setIndex, setPageReference) {};

    void startUp() const override;
    void algo() const override;
    void cleanUp() const override;
};

class LFU : public pageAlgorithms{

public:
    LFU(const u16* setIndex,const std::vector<std::vector<u16>>* setPageReference) : pageAlgorithms(setIndex, setPageReference) {};

    void startUp() const override;
    void algo() const override;
    void cleanUp() const override;
};

class LRU : public pageAlgorithms{

public:
    LRU(const u16* setIndex,const std::vector<std::vector<u16>>* setPageReference) : pageAlgorithms(setIndex, setPageReference) {};

    void startUp() const override;
    void algo() const override;
    void cleanUp() const override;
};

#endif //FINALPROJECT_PAGEALGORITHMS_H
