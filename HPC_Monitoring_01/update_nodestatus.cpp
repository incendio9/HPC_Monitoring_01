#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Update_NodeStatus() {

    // Update master node
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

    nodelist *curr_node = HPCNodes;
    curr_node->CPULoad = (float)percent;

    FILE *nStatus = fopen("/nfs/HPC/WORK/node_status.txt", "r");

    char *string = new char[1024];
    char *temp = new char[1024];
    char *token;

    int cnt_avail_node = 0;
    int cnt_vga_node = 0;

    while (fgets(string, 1024, nStatus) != nullptr) {

        strcpy(temp, string);
        token = strtok(temp, " =\n");
        if (token == nullptr) continue;

        if (strncmp(token, "NodeName", 8) == 0) {

            token = strtok(nullptr, " =\n");
            if (token == nullptr) continue;

            curr_node = HPCNodes;

            for (int i = 0; i < NodeCount; i++) {

                curr_node = curr_node->next_node;

                if (strncmp(token, curr_node->NodeName, strlen(curr_node->NodeName)) == 0) {

                    while (fgets(string, 1024, nStatus) != nullptr) {

                        strcpy(temp, string);
                        token = strtok(temp, " =\n");
                        if (token == nullptr) break;

                        while (token != nullptr) {

                            if (strncmp(token, "State", 5) == 0) {

                                token = strtok(nullptr, " =\n");

                                delete[] curr_node->NodeStatus;
                                curr_node->NodeStatus = new char[strlen(token) + 1];
                                for (int j = 0; j < (int)(strlen(token) + 1); j++) curr_node->NodeStatus[j] = '\0';
                                strcpy(curr_node->NodeStatus, token);

                                break;

                            } else if (strncmp(token, "CPULoad", 7) == 0) {

                                token = strtok(nullptr, " =\n");
                                if (token == nullptr) continue;

                                curr_node->CPULoad = (float)atof(token);

                                break;

                            }

                            token = strtok(nullptr, " =\n");

                        }

                    }

                    break;

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

    for (int i = 0; i < NodeCount; i++) {
        curr_node = curr_node->next_node;
        NodeNameLabel[i].setText(curr_node->NodeName);
        NodeStatusLabel[i].setText(curr_node->NodeStatus);
        NodeCPULoadLabel[i].setText(cpuload.sprintf("%.2f", curr_node->CPULoad));
        if ( (strcmp(curr_node->NodeStatus, "IDLE") == 0)
             || (strcmp(curr_node->NodeStatus, "ALLOCATED") == 0) ) {
            NodeResumeButton[i].setEnabled(false);
            NodeStatusLabel[i].setAutoFillBackground(true);
            NodeStatusLabel[i].setPalette(QPalette(QPalette::Base, Qt::green));
            cnt_avail_node++;
            if (curr_node->VGACount > 0) cnt_vga_node++;
        } else {
            NodeResumeButton[i].setEnabled(true);
            NodeStatusLabel[i].setAutoFillBackground(true);
            NodeStatusLabel[i].setPalette(QPalette(QPalette::Base, Qt::red));
        }
    }

    NumAvailNodes = cnt_avail_node;
    NumVGANodes = cnt_vga_node;

    return 0;

}
