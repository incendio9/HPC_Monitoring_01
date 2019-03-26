#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Finish_Checkpoint_Coordinator() {

    if (CProcess_coordinator != nullptr) {
        CProcess_coordinator->close();
        delete CProcess_coordinator;
        CProcess_coordinator = nullptr;
        CCommand_coordinator.clear();
        CArguments_coordinator.clear();
    } else {
        QMessageBox::information(this, "Error", "Checkpoint coordinator is NULL", "OK");
        return 1;
    }

    return 0;

}
