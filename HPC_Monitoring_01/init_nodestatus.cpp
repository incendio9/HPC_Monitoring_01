#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Init_NodeStatus() {

    NSProcess = new QProcess(this);
    NSCommand = "scontrol";
    QString NSScript1 = "show";
    QString NSScript2 = "nodes";
    NSArguments << NSScript1 << NSScript2;
    NSProcess->setStandardOutputFile("/nfs/HPC/WORK/node_status.txt");
    NSProcess->start(NSCommand, NSArguments);
    NSProcess->waitForFinished();

    NodeCount = 0;
    NumAvailNodes = 0;
    NumVGANodes = 0;

    nvmlInit();
    unsigned int device_count = 0;
    nvmlDeviceGetCount(&device_count);

    HPCNodes = new nodelist;
    HPCNodes->NodeName = new char[strlen(qgetenv("USER"))];
    strcpy(HPCNodes->NodeName, qgetenv("USER"));
    HPCNodes->NodeStatus = nullptr;
    HPCNodes->CPULoad = 0.0;
    HPCNodes->MEMLoad = 0.0;
    HPCNodes->VGACount = (int)device_count;
    HPCNodes->VGALoad_GPU = new unsigned int[device_count];
    HPCNodes->VGALoad_Mem = new unsigned int[device_count];
    HPCNodes->next_node = nullptr;

    nvmlShutdown();

    nodelist *curr_node = HPCNodes;
    FILE *cLoad = fopen("/proc/stat", "r");
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;
    fscanf(cLoad, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
    fclose(cLoad);
    double percent = 0.00;
    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle) {
            percent = 0.0;
    }
    else {
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    curr_node->CPULoad = (float)percent;

    FILE *nStatus = fopen("/nfs/HPC/WORK/node_status.txt", "r");

    char *string = new char[1024];
    char *temp = new char[1024];
    char *token;

    while (fgets(string, 1024, nStatus) != nullptr) {

        strcpy(temp, string);
        token = strtok(temp, " =\n");
        if (token == nullptr) continue;

        if (strncmp(token, "NodeName", 8) == 0) {

            NodeCount++;

            token = strtok(nullptr, " =\n");
            if (token == nullptr) continue;

            curr_node->next_node = new nodelist;
            curr_node = curr_node->next_node;
            curr_node->NodeName = new char[strlen(token) + 1];
            for (int i = 0; i < (int)(strlen(token) + 1); i++) curr_node->NodeName[i] = '\0';
            strcpy(curr_node->NodeName, token);
            curr_node->MEMLoad = 0.0;
            curr_node->VGACount = 0;
            curr_node->VGALoad_GPU = nullptr;
            curr_node->VGALoad_Mem = nullptr;
            curr_node->next_node = nullptr;

            while (fgets(string, 1024, nStatus) != nullptr) {

                strcpy(temp, string);
                token = strtok(temp, " =\n");
                if (token == nullptr) break;

                while (token != nullptr) {

                    if (strncmp(token, "State", 5) == 0) {

                        token = strtok(nullptr, " =\n");
                        if (token == nullptr) continue;

                        curr_node->NodeStatus = new char[strlen(token) + 1];
                        for (int i = 0; i < (int)(strlen(token) + 1); i++) curr_node->NodeStatus[i] = '\0';
                        strcpy(curr_node->NodeStatus, token);

                        break;

                    } else if (strncmp(token, "CPULoad", 7) == 0) {

                        token = strtok(nullptr, " =\n");
                        if (token == nullptr) continue;

                        curr_node->CPULoad = (float)atof(token);

                        break;

                    } else if (strncmp(token, "Gres", 4) == 0) {

                        token = strtok(nullptr, " =\n:");
                        if (token == nullptr) continue;

                        if (strncmp(token, "gpu", 3) == 0) {
                            NumVGANodes++;
                            token = strtok(nullptr, " =\n:");
                            curr_node->VGACount = atoi(token);
                            curr_node->VGALoad_GPU = new unsigned int[curr_node->VGACount];
                            curr_node->VGALoad_Mem = new unsigned int[curr_node->VGACount];
                            //printf("%d\n", curr_node->VGACount);
                        }

                        break;

                    }

                    token = strtok(nullptr, " =\n");

                }

            }

        }

    }

    fclose(nStatus);
    delete[] string;
    delete[] temp;

    curr_node = HPCNodes;

    QString cpuload;
    ui->label_06->setText(curr_node->NodeName);
    ui->label_08->setText(cpuload.sprintf("%.2f", curr_node->CPULoad));

    NSGrid = new QGridLayout;
    NodeNameLabel = new QLabel[NodeCount];
    NodeStatusLabel = new QLabel[NodeCount];
    NodeCPULoadLabel = new QLabel[NodeCount];
    NodeResumeButton = new QPushButton[NodeCount];

    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        NodeNameLabel[i].setText(curr_node->NodeName);
        NodeStatusLabel[i].setText(curr_node->NodeStatus);
        NodeCPULoadLabel[i].setText(cpuload.sprintf("%.2f", curr_node->CPULoad));
        NodeResumeButton[i].setText("RESUME NODE");
        if ( (strcmp(curr_node->NodeStatus, "IDLE") == 0)
             || (strcmp(curr_node->NodeStatus, "ALLOCATED") == 0) ) {
            NodeResumeButton[i].setEnabled(false);
            NodeStatusLabel[i].setAutoFillBackground(true);
            NodeStatusLabel[i].setPalette(QPalette(QPalette::Base, Qt::green));
            NumAvailNodes++;
        } else {
            NodeResumeButton[i].setEnabled(true);
            NodeStatusLabel[i].setAutoFillBackground(true);
            NodeStatusLabel[i].setPalette(QPalette(QPalette::Base, Qt::red));
        }
        NSGrid->addWidget(&NodeNameLabel[i], i, 0);
        NSGrid->addWidget(&NodeStatusLabel[i], i, 1);
        NSGrid->addWidget(&NodeCPULoadLabel[i], i, 2);
        NSGrid->addWidget(&NodeResumeButton[i], i, 3);
    }

    ui->scrollAreaWidgetContents->setLayout(NSGrid);

    return 0;

}
