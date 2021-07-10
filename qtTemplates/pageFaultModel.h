//
// Created by fly0ut on 7/6/21.
//

#ifndef FINALPROJECT_PAGEFAULTMODEL_H
#define FINALPROJECT_PAGEFAULTMODEL_H
#include <QtWidgets>
#include <QtGui>
#include <ranges>
#include "../include/pageFaultSim.h"

class pageFaultModel : public QAbstractTableModel {
    QList<QList<QList<unsigned short>>> pageData;
    int currentIter = 0;
    pageFaultSim *pageSim;

public:
    explicit pageFaultModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
    int rowCount(const QModelIndex &) const override { return pageData.at(currentIter).count();}
    int columnCount(const QModelIndex &) const override { return pageData.at(currentIter).at(0).count();}

    QVariant data(const QModelIndex &index, int role) const override{
        if (!index.isValid())
            return QVariant();
        if (index.row() >= pageData.at(currentIter).size())
            return QVariant();
        if (index.column() >= pageData.at(currentIter).at(index.row()).size())
            return QVariant();
        if (role == Qt::DisplayRole)
            return pageData.at(currentIter).at(index.row()).at(index.column());
        else
            return QVariant();
    }

    /*QVariant headerData(int section, Qt::Orientation orientation, int role) const override{

    }*/

    void appendTime(const std::vector<std::vector<unsigned int>>& nextNum) {
        beginInsertRows({}, pageData[currentIter].count(), pageData[currentIter].count());
        QList<QList<unsigned short>> testing;
        int i=0;
        std::ranges::for_each(nextNum.begin(), nextNum.end(), [this, &i, &testing](const std::vector<unsigned  int>& e){std::copy(e.begin(), e.end(), std::back_inserter(pageData[currentIter][i])); i++;});
        pageData[currentIter].append(testing);
        endInsertRows();
    }

    QModelIndex index(int row, int column, const QModelIndex& parent) const override{
        return createIndex(row, column);
    }

    void setCurrentIter(int setIter){
        this->currentIter=setIter;
    }

    void setPageSim(pageFaultSim *setPageSim){
        if(setPageSim == nullptr) {
            qDebug() << "Error: page simulation can't be set to a null pointer";
        }
        this->pageSim=setPageSim;
    }

    void load(const std::vector<std::vector<std::vector<unsigned int>>>){

    }

    void loadFromPageSim(){
        if(this->pageSim == nullptr) {
            qDebug() << "Error: page simulation is a null pointer";
        }
        beginInsertRows(QModelIndex(), 0, pageSim->pageReferenceTime[0].size());
        int i=0;
        int ii=0;
        std::for_each(pageSim->pageReferenceTime.begin(), pageSim->pageReferenceTime.end(),
              [this, &i, &ii](const vector<vector<unsigned short>> e)
                    {std::ranges::for_each(e,
                       [this, &i, &ii](const vector<unsigned short> ee)
                            {qDebug() << ee[0];
                             std::ranges::copy(ee, std::back_inserter(pageData[i][ii])); ii++
                        ;}); i++
                    ;});
        endInsertRows();
    }
};

#endif //FINALPROJECT_PAGEFAULTMODEL_H
