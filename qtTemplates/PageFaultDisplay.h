/********************************************************************************
** Form generated from reading UI file 'FinalProjectLnDjyE.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FINALPROJECTLNDJYE_H
#define FINALPROJECTLNDJYE_H

#include <QtCore/QVariant>
#include <QMainWindow>
#include <QtDebug>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "../include/pageFaultSim.h"
#include "pageFaultModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class PageFaultDisplay;
} // namespace Ui
QT_END_NAMESPACE

class PageFaultDisplay : public QMainWindow {
Q_OBJECT

public:
    explicit PageFaultDisplay(QWidget *parent = nullptr);

    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QTableView *tableView;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSlider *verticalSlider;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButtonFIFO;
    QRadioButton *radioButtonOPT;
    QRadioButton *radioButtonLRU;
    QRadioButton *radioButtonLFU;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menuPage_Fault_Simulation;
    QStatusBar *statusbar;
    pageFaultModel *model;
    //QAction *action;
    pageFaultSim *pageSim;

    void setupUi(PageFaultDisplay *setPageFaultDisplay);

    void retranslateUi(PageFaultDisplay *setPageFaultDisplay);

    QRadioButton* algoSwitch();
private slots:
    int algoRun();
public:
    void setPageSim(pageFaultSim *setSim);
    ~PageFaultDisplay() override;
private:
    Ui::PageFaultDisplay *ui;
};
#endif // PageFaultDisplayLNDJYE_H
