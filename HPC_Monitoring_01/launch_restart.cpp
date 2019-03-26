#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Launch_Restart() {

    if (RProcess == nullptr) {
        RProcess = new QProcess(this);
    } else {
        QMessageBox::information(this, "Error", "Restarted program already running", "OK");
        return 1;
    }

    RCommand = "srun";
    QString arg1 = "-n";
    QString arg2;
    arg2.sprintf("%d", NumAvailNodes);
    QString arg3 = "-N";
    QString arg4;
    arg4.sprintf("%d", NumAvailNodes);
    QString arg5 = "/nfs/HPC/WORK/checkpoint/launch_restart";
    QString arg6 = "&";
    RArguments << arg1 << arg2 << arg3 << arg4 << arg5 << arg6;
    RProcess->startDetached(RCommand, RArguments);
    QString log = RCommand + " " + arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " " + arg5 + " " + arg6;
    ui->textEdit->append(log.toStdString().c_str());

    return 0;

}
