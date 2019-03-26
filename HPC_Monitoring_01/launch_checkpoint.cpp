#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Launch_Checkpoint() {

    if (CProcess_checkpoint == nullptr) {
        CProcess_checkpoint = new QProcess(this);
    } else {
        QMessageBox::information(this, "Error", "Checkpoint program already running", "OK");
        return 1;
    }

    CCommand_checkpoint = "srun";
    QString arg1 = "-n";
    QString arg2;
    arg2.sprintf("%d", NumAvailNodes);
    QString arg3 = "-N";
    QString arg4;
    arg4.sprintf("%d", NumAvailNodes);
    QString arg5 = "/nfs/HPC/WORK/checkpoint/launch_ckpt";
    QString arg6 = ui->lineEdit_5->text();
    QString arg7 = "&";
    CArguments_checkpoint << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7;
    CProcess_checkpoint->startDetached(CCommand_checkpoint, CArguments_checkpoint);
    QString log = CCommand_checkpoint + " " + arg1 + " " + arg2 + " " + arg3 + " " + arg4 + " " + arg5 + " " + arg6 + " " + arg7;
    ui->textEdit->append(log.toStdString().c_str());

    return 0;

}
