#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Finish_Restart() {

    if (RProcess != nullptr) {
        RProcess->close();
        delete RProcess;
        RProcess = nullptr;
        RCommand.clear();
        RArguments.clear();
    } else {
        QMessageBox::information(this, "Error", "Checkpoint coordinator is NULL", "OK");
        return 1;
    }

    return 0;

}
