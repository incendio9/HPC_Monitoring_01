#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Launch_Checkpoint_Coordinator() {

    if (CProcess_coordinator == nullptr) {
        CProcess_coordinator = new QProcess(this);
    } else {
        QMessageBox::information(this, "Error", "Checkpoint coordinator already running", "OK");
        return 1;
    }

    QString app_dir = ui->lineEdit_5->text();
    int app_execution_time = ui->lineEdit_6->text().toInt();
    float ckpt_execution_time = ui->lineEdit_7->text().toFloat();
    float fault_rate = ui->lineEdit_8->text().toFloat();
    int optimal_interval = 0;

    // Launch Coordinator
    if (CProcess_coordinator == nullptr)
        CProcess_coordinator = new QProcess(this);

    optimal_interval = (int)sqrt( ( (fault_rate * ckpt_execution_time * ckpt_execution_time)
                                    + ckpt_execution_time
                                    + (app_execution_time * ckpt_execution_time) )
                                  / fault_rate );

    CCommand_coordinator = "/usr/local/bin/dmtcp_coordinator";
    //QString arg1 = "--daemon";
    QString arg2 = "--exit-on-last";
    //QString arg3 = "--ckptdir";
    //QString arg4 = "/nfs/HPC/WORK/checkpoint/";
    QString arg5 = "-i";
    QString arg6;
    arg6.sprintf("%d", optimal_interval);
    CArguments_coordinator << arg2 << arg5 << arg6;
    //CArguments_coordinator << arg1 << arg2 << arg5 << arg6;
    CProcess_coordinator->startDetached(CCommand_coordinator, CArguments_coordinator);
    QString log = CCommand_coordinator + " " + arg2 + " " + arg5 + " " + arg6;
    ui->textEdit->append(log.toStdString().c_str());

    return 0;

}
