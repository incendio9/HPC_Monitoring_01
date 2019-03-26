#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Init_PowerEstimator() {

    perfLayout = new QFormLayout();
    //Qt::Alignment align = Qt::AlignVCenter;
    //perfLayout->setFormAlignment(align);
    QGroupBox *perfBox = new QGroupBox("Performance");
    perfBox->setLayout(perfLayout);

    powerLayout = new QFormLayout();
    QGroupBox *powerBox = new QGroupBox("Power");
    powerBox->setLayout(powerLayout);

    dataLayout = new QFormLayout();
    QGroupBox *dataBox = new QGroupBox("Data Access");
    dataBox->setLayout(dataLayout);

    topoLayout = new QFormLayout();
    QGroupBox *topoBox = new QGroupBox("Topology");
    topoBox->setLayout(topoLayout);

    QGroupBox *perfpowerBox = new QGroupBox();
    QVBoxLayout *perfpowerLayout = new QVBoxLayout();
    perfpowerLayout->addWidget(perfBox);
    perfpowerLayout->addWidget(powerBox);
    perfpowerBox->setLayout(perfpowerLayout);
    //perfpowerBox->setFixedWidth(this->ui->frame->width()/2);

    QGroupBox *datatopoBox = new QGroupBox();
    QVBoxLayout *datatopoLayout = new QVBoxLayout();
    datatopoLayout->addWidget(dataBox);
    datatopoLayout->addWidget(topoBox);
    datatopoBox->setLayout(datatopoLayout);
    //datatopoBox->setFixedWidth(this->ui->frame->width()/2);

    QHBoxLayout *totalLayout = new QHBoxLayout();
    totalLayout->addWidget(perfpowerBox);
    totalLayout->addWidget(datatopoBox);

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(totalLayout, 0, 0);

    ui->frame->setLayout(baseLayout);
    //ui->Results->setLayout(baseLayout);

    return 0;

}
