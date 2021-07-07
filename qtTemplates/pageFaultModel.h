//
// Created by fly0ut on 7/6/21.
//

#ifndef FINALPROJECT_PAGEFAULTMODEL_H
#define FINALPROJECT_PAGEFAULTMODEL_H
#include <QtWidgets>
#include <QtGui>
#include <ranges>

class pageFaultModel : public QAbstractTableModel {
    QList<QList<QList<unsigned short>>> pageData;
    int currentIter = 0;
public:
    explicit pageFaultModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
    [[nodiscard]] int rowCount(const QModelIndex &) const override { return pageData[currentIter].count(); }
    [[nodiscard]] int columnCount(const QModelIndex &) const override { return pageData[currentIter][currentIter].count(); }

    QVariant Data(const QModelIndex &index, int role) const {//override{
        if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
        return pageData[currentIter][index.row()][index.column()];
    }

    void append(const std::vector<std::vector<unsigned int>>& nextNum){
        beginInsertRows({}, pageData[currentIter].count(), pageData[currentIter].count());
        QList<QList<unsigned short>> testing;
        int i=0;
        std::ranges::for_each(nextNum.begin(), nextNum.end(), [this, &i, &testing](const std::vector<unsigned  int>& e){std::copy(e.begin(), e.end(), std::back_inserter(pageData[currentIter][i])); i++;});
        pageData[currentIter].append(testing);
        endInsertRows();
    }

    /*QModelIndex index(int row, int column, const QModelIndex& parent) const override{
        return
    }*/

    void load(const std::vector<std::vector<std::vector<unsigned int>>>){

    }
};

#endif //FINALPROJECT_PAGEFAULTMODEL_H
