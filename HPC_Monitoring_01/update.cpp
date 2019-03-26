#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

int MainWindow::Update() {

    static QTime time(QTime::currentTime());
    double key = time.elapsed() / 1000.0;
    static double lastPointKey = 0;

    if (key - lastPointKey >= 5.000) { // at most add point every 1 ms

        Update_NodeStatus();
        Update_NodeMemLoad(key);
        Update_NodeVGALoad(key);

        NSProcess->start(NSCommand, NSArguments);
        MLProcess->start(MLCommand, MLArguments);
        VLProcess->start(VLCommand, VLArguments);

        NSProcess->waitForFinished();
        MLProcess->waitForFinished();
        VLProcess->waitForFinished();

        lastPointKey = key;

    }

    return 0;

}
