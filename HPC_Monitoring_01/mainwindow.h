#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <QProcess>
#include <nvml.h>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "qcustomplot.h"

namespace Ui {
    class MainWindow;
}

struct nodelist {

    char *NodeName;
    char *NodeStatus;
    float CPULoad;
    float MEMLoad;
    int VGACount;
    unsigned int *VGALoad_GPU;
    unsigned int *VGALoad_Mem;

    nodelist *next_node;

};

class MainWindow : public QMainWindow {

    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // NodeStatus
    int Init_NodeStatus();
    int Update_NodeStatus();

    // MemLoad
    int Init_NodeMemLoad();
    int Update_NodeMemLoad(double key);

    // VGALoad
    int Init_NodeVGALoad();
    int Update_NodeVGALoad(double key);

    int Init_Visualizer();
    int Init_PowerEstimator();

    int Init_Checkpoint();
    int Init_Restart();


private:

    Ui::MainWindow *ui;

    // Node Status
    int NodeCount;
    int NumAvailNodes;
    int NumVGANodes;
    nodelist *HPCNodes;
    QProcess *NSProcess;
    QString NSCommand;
    QStringList NSArguments;
    QGridLayout *NSGrid;
    QLabel *NodeNameLabel;
    QLabel *NodeStatusLabel;
    QLabel *NodeCPULoadLabel;
    QPushButton *NodeResumeButton;
    unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

    // NodeMemLoad
    QProcess *MLProcess;
    QString MLCommand;
    QStringList MLArguments;
    struct sysinfo memInfo;
    QVBoxLayout *MLLayout;
    QCustomPlot *MLChart;
    long long totalPhysMem;
    long long physMemUsed;

    // NodeVGALoad
    QProcess *VLProcess;
    QString VLCommand;
    QStringList VLArguments;
    QVBoxLayout *GPULoadLayout;
    QVBoxLayout *MemLoadLayout;
    QGridLayout *VLChartLayout_GPU;
    QGridLayout *VLChartLayout_Mem;
    QCustomPlot **VLChart_GPU;
    QCustomPlot **VLChart_Mem;

    // PowerEstimator
    QFormLayout *perfLayout;
    QFormLayout *powerLayout;
    QFormLayout *dataLayout;
    QFormLayout *topoLayout;

    // Checkpoint
    //int optimal_interval;
    QProcess *CProcess_coordinator;
    QString CCommand_coordinator;
    QStringList CArguments_coordinator;
    QProcess *CProcess_checkpoint;
    QString CCommand_checkpoint;
    QStringList CArguments_checkpoint;

    // Restart
    QProcess *RProcess;
    QString RCommand;
    QStringList RArguments;


private slots:

    int Update();

    int Launch_NodeResume(int NodeNumber);

    int Launch_Visualizer();

    void FINISH();
    void BROWSE_1();
    void BROWSE_2();
    void BROWSE_3();
    void BROWSE_4();

    int Launch_Checkpoint_Coordinator();
    int Launch_Checkpoint();
    int Launch_Restart();

    int Finish_Checkpoint_Coordinator();
    int Finish_Checkpoint();
    int Finish_Restart();

};

#endif // MAINWINDOW_H
