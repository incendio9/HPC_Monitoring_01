#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::Init_Visualizer() {

    QPixmap img;
    img.load(":/new/prefix1/escal-logo-colored.png");
    img.scaled(ui->label_17->width(), ui->label_17->height(), Qt::KeepAspectRatio);
    ui->label_17->setPixmap(img);

    ui->label_17->setAlignment(Qt::AlignCenter);

    return 0;

}
