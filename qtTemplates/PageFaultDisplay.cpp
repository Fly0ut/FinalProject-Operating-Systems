//
// Created by fly0ut on 7/4/21.
//

#include "PageFaultDisplay.h"
#include "ui_PageFaultDisplay.h"

PageFaultDisplay::PageFaultDisplay(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::PageFaultDisplay) {
    setWindowTitle("Page Fault Simulation");
    ui->setupUi(this);

    //Run button setup to trigger the page algorithm simulation.
    QObject::connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(algoRun()));
    //ToDo: Go back in the simulation.
    //QObject::connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(void));
    //ToDo: Go forward in the simulation.
    //QObject::connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(void));
}

PageFaultDisplay::~PageFaultDisplay() {
    delete ui;
}

void PageFaultDisplay::setupUi(PageFaultDisplay *setPageFaultDisplay)
{
    if (setPageFaultDisplay->objectName().isEmpty())
        setPageFaultDisplay->setObjectName(QString::fromUtf8("PageFaultDisplay"));
    setPageFaultDisplay->resize(800, 600);
    setPageFaultDisplay->setCursor(QCursor(Qt::ArrowCursor));
    centralwidget = new QWidget(setPageFaultDisplay);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayoutWidget = new QWidget(centralwidget);
    gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
    gridLayoutWidget->setGeometry(QRect(-1, -1, 791, 551));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    tableView = new QTableView(gridLayoutWidget);
    tableView->setObjectName(QString::fromUtf8("tableView"));

    model = new pageFaultModel;
    model->setPageSim(this->pageSim);

    tableView->setModel(model);

    gridLayout->addWidget(tableView, 0, 1, 1, 1);

    verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setSpacing(5);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    pushButton_2 = new QPushButton(gridLayoutWidget);
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    horizontalLayout_3->addWidget(pushButton_2);

    pushButton_3 = new QPushButton(gridLayoutWidget);
    pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

    horizontalLayout_3->addWidget(pushButton_3, 0, Qt::AlignLeft);


    verticalLayout_2->addLayout(horizontalLayout_3);

    verticalSlider = new QSlider(gridLayoutWidget);
    verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
    verticalSlider->setOrientation(Qt::Vertical);

    verticalLayout_2->addWidget(verticalSlider);


    gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    radioButtonFIFO = new QRadioButton(gridLayoutWidget);
    radioButtonFIFO->setObjectName(QString::fromUtf8("radioButtonFIFO"));
    radioButtonFIFO->setCursor(QCursor(Qt::PointingHandCursor));
    radioButtonFIFO->setChecked(true);

    horizontalLayout->addWidget(radioButtonFIFO);

    radioButtonOPT = new QRadioButton(gridLayoutWidget);
    radioButtonOPT->setObjectName(QString::fromUtf8("radioButtonOPT"));
    radioButtonOPT->setCursor(QCursor(Qt::PointingHandCursor));

    horizontalLayout->addWidget(radioButtonOPT);

    radioButtonLRU = new QRadioButton(gridLayoutWidget);
    radioButtonLRU->setObjectName(QString::fromUtf8("radioButtonLRU"));
    radioButtonLRU->setCursor(QCursor(Qt::PointingHandCursor));

    horizontalLayout->addWidget(radioButtonLRU);

    radioButtonLFU = new QRadioButton(gridLayoutWidget);
    radioButtonLFU->setObjectName(QString::fromUtf8("radioButtonLFU"));
    radioButtonLFU->setCursor(QCursor(Qt::PointingHandCursor));

    horizontalLayout->addWidget(radioButtonLFU);

    pushButton = new QPushButton(gridLayoutWidget);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setCursor(QCursor(Qt::PointingHandCursor));

    horizontalLayout->addWidget(pushButton);


    gridLayout->addLayout(horizontalLayout, 1, 0, 1, 2);

    //setPageFaultDisplay->setCentralWidget(centralwidget);
    menubar = new QMenuBar(setPageFaultDisplay);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 25));
    menuPage_Fault_Simulation = new QMenu(menubar);
    menuPage_Fault_Simulation->setObjectName(QString::fromUtf8("menuPage_Fault_Simulation"));
    //setPageFaultDisplay->setMenuBar(menubar);
    statusbar = new QStatusBar(setPageFaultDisplay);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    //setPageFaultDisplay->setStatusBar(statusbar);

    menubar->addAction(menuPage_Fault_Simulation->menuAction());

    retranslateUi(setPageFaultDisplay);

    QMetaObject::connectSlotsByName(setPageFaultDisplay);
} // setupUi

void PageFaultDisplay::retranslateUi(PageFaultDisplay *setPageFaultDisplay)
{
    setPageFaultDisplay->setWindowTitle(QCoreApplication::translate("PageFaultDisplay", "PageFaultDisplay", nullptr));
    pushButton_2->setText(QCoreApplication::translate("PageFaultDisplay", "Back", nullptr));
    pushButton_3->setText(QCoreApplication::translate("PageFaultDisplay", "Forward", nullptr));
    radioButtonFIFO->setText(QCoreApplication::translate("PageFaultDisplay", "FIFO", nullptr));
    radioButtonOPT->setText(QCoreApplication::translate("PageFaultDisplay", "OPT", nullptr));
    radioButtonLRU->setText(QCoreApplication::translate("PageFaultDisplay", "LRU", nullptr));
    radioButtonLFU->setText(QCoreApplication::translate("PageFaultDisplay", "LFU", nullptr));
    pushButton->setText(QCoreApplication::translate("PageFaultDisplay", "PushButton", nullptr));
    menuPage_Fault_Simulation->setTitle(QCoreApplication::translate("PageFaultDisplay", "File", nullptr));
} // retranslateUi

QRadioButton* PageFaultDisplay::algoSwitch() {
    QList<QRadioButton *> algoList= this->findChildren<QRadioButton *>();

    if(algoList.empty()) {
        qDebug() << "Error: not children radio buttons found.\n";
        return nullptr;
        //May want to exit with error instead of returning a null pointer instead of a non-valid pointer.
        //This is why rust is soo much better.  If it was rust I could just return a result<QRadioButton, Error>.
        //exit(errno);
    }
    else {
        qDebug() << algoList.size() << "\n";
    }
    //Make sure this works. Doesn't look like it works.
    QList<QRadioButton *>::iterator algoButIter = std::find_if(algoList.begin(), algoList.end(), [](QRadioButton *e){ return e->isChecked();});
    return *algoButIter;
}

int PageFaultDisplay::algoRun() {
    QRadioButton* seletedRadioOpt =  algoSwitch();
    if(seletedRadioOpt == nullptr){
        qDebug() << "Error: QRadiobutton from switch is  a Null pointer\n";
        return errno;
    }
    if(this->pageSim == nullptr) {
        qDebug() << "Error: page simulation is a null pointer";
        return errno;
    }

    qDebug() << "Pointing to: " << seletedRadioOpt << "\n";

    std::string seletedOpt = seletedRadioOpt->text().toStdString();

    if(seletedOpt == "FIFO") {
        this->pageSim->pageFaultAlgoPopAPI(1);
        qDebug() << "triggering FIFO\n";
    }
    else if(seletedOpt == "OPT") {
        this->pageSim->pageFaultAlgoStepAPI(2);
        qDebug() << "triggering OPT\n";
    }
    else if(seletedOpt == "LRU") {
        this->pageSim->pageFaultAlgoStepAPI(3);
        qDebug() << "triggering LRU\n";
    }
    else if(seletedOpt == "LFU") {
        this->pageSim->pageFaultAlgoStepAPI(4);
        qDebug() << "triggering LFU\n";
    } else{
        qDebug() << "Error: not valid input for gui select: " << seletedOpt.c_str()<< "\n" << seletedRadioOpt->text();
        return 0;
    }

    model->loadFromPageSim();

    return 0;
}

void PageFaultDisplay::setPageSim(pageFaultSim *setSim) {
    this->pageSim = setSim;
}
