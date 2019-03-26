#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Launch_NodeResume(int NodeNumber) {

    nodelist *curr_node = HPCNodes;
    curr_node = curr_node->next_node;
    for (int j = 0; j < NodeNumber; j++) curr_node = curr_node->next_node;

    QProcess *NRProcess = new QProcess(this);
    QString NRcommand = "sudo";
    //QString NRcommand = "scontrol";
    QStringList NRarguments;

    QString arg0 = "-S";
    QString arg1 = "scontrol";
    QString arg2 = "update";
    QString arg3;
    arg3.sprintf("NodeName=%s", curr_node->NodeName);
    QString arg4 = "State=RESUME";

    NRarguments << arg0 << arg1 << arg2 << arg3 << arg4;
    //NRarguments << arg1 << arg2 << arg3;

    /*QString NRcommand = "/bin/bash";

    QStringList NRarguments;

    QString arg0 = "-c";
    QString arg1 = "echo";
    QString arg2 = "comsys2775";
    QString arg3 = "|";
    QString arg4 = "sudo";
    QString arg5 = "-S";
    QString arg6 = "scontrol";
    QString arg7 = "update";
    QString arg8;
    arg8.sprintf("NodeName=%s", curr_node->NodeName);
    QString arg9 = "State=RESUME";

    NRarguments << arg0 << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9;*/

    //NRProcess->open(QIODevice::ReadWrite);
    //NRProcess->waitForReadyRead();
    NRProcess->startDetached(NRcommand, NRarguments);
    //NRProcess->waitForReadyRead();
    //NRProcess->write("comsys2775");
    NRProcess->waitForFinished();

    return 0;

}
