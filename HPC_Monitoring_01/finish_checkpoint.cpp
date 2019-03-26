#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Finish_Checkpoint() {

    if (CProcess_checkpoint != nullptr) {
        CProcess_checkpoint->close();
        delete CProcess_checkpoint;
        CProcess_checkpoint = nullptr;
        CCommand_checkpoint.clear();
        CArguments_checkpoint.clear();
    } else {
        QMessageBox::information(this, "Error", "Checkpoint is NULL", "OK");
        return 1;
    }

    return 0;

}
