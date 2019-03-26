#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // Initialize
    Init_NodeStatus();
    Init_NodeMemLoad();
    Init_NodeVGALoad();

    Init_Visualizer();
    Init_PowerEstimator();

    Init_Checkpoint();
    Init_Restart();

    // Data update
    QTimer *dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(Update()));
    dataTimer->start(100);

    // NodeResume
    int i = 0;
    QSignalMapper *sigmap = new QSignalMapper(this);
    for (i = 0; i < NodeCount; i++) {
        connect(&NodeResumeButton[i], SIGNAL(clicked()), sigmap, SLOT(map()));
        sigmap->setMapping(&NodeResumeButton[i], i);
    }
    connect(sigmap, SIGNAL(mapped(int)), this, SLOT(Launch_NodeResume(int)));

    // Visualizer
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(Launch_Visualizer()));

    // PowerEstimator
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(BROWSE_1()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(BROWSE_2()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(BROWSE_3()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(BROWSE_4()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(FINISH()));

    // Checkpoint
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(Launch_Checkpoint_Coordinator()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(Launch_Checkpoint()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(Launch_Restart()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(Finish_Checkpoint_Coordinator()));
    connect(ui->pushButton_11, SIGNAL(clicked()), this, SLOT(Finish_Checkpoint()));
    connect(ui->pushButton_12, SIGNAL(clicked()), this, SLOT(Finish_Restart()));


}

MainWindow::~MainWindow()
{

    delete ui;

    if (NSProcess != NULL) delete NSProcess;
    if (MLProcess != NULL) delete MLProcess;
    if (VLProcess != NULL) delete VLProcess;
    if (CProcess_coordinator != NULL) delete CProcess_coordinator;
    if (CProcess_checkpoint != NULL) delete CProcess_checkpoint;
    if (RProcess != NULL) delete RProcess;

    delete[] NodeNameLabel;
    delete[] NodeStatusLabel;
    delete[] NodeCPULoadLabel;
    delete[] NodeResumeButton;

    delete[] MLChart;
    delete MLLayout;

    delete perfLayout;
    delete powerLayout;
    delete dataLayout;
    delete topoLayout;

    nodelist *curr_node = HPCNodes;
    nodelist *prev_node = HPCNodes;
    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        delete[] prev_node->NodeName;
        delete[] prev_node->NodeStatus;
        delete[] prev_node->VGALoad_GPU;
        delete[] prev_node->VGALoad_Mem;
        if (curr_node->VGACount > 0) {
                delete[] VLChart_GPU[i];
                delete[] VLChart_Mem[i];
        }
        delete prev_node;
        prev_node = curr_node;
    }

    delete[] VLChart_GPU;
    delete[] VLChart_Mem;
    delete[] VLChartLayout_GPU;
    delete[] VLChartLayout_Mem;
    delete GPULoadLayout;
    delete MemLoadLayout;

}
