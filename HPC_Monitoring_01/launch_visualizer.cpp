#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Launch_Visualizer() {

    QProcess *VProcess = new QProcess(this);
    QString VCommand = "/usr/bin/java";
    QStringList VArguments;
    QString _launchArg1;
    QString _launchArg2;
    QString _launchArg3;

    _launchArg1 = "-cp";
    _launchArg2 = "/nfs/hpc-visualizer_yoon/bin:/nfs/hpc-visualizer_yoon/lib/*";
    _launchArg3 = "escal.hpc.FrontendMain";

    VArguments << _launchArg1 << _launchArg2 << _launchArg3;

    if(QFile::exists(VCommand))
        VProcess->start(VCommand, VArguments);
    else {
        QMessageBox::information(this, "Error", "Visualizer not found", "OK");
        return -1;
    }

    return 0;

}
