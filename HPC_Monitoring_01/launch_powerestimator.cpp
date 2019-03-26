#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::BROWSE_1()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->lineEdit_1->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::BROWSE_2()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->lineEdit_2->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::BROWSE_3()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->lineEdit_3->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::BROWSE_4()
{
    QFileDialog *fd = new QFileDialog(this);
    QString file_name;
    QStringList list_file_name;
    int ret = fd->exec();
    if (ret == (int)QDialog::Accepted)
    {
        list_file_name = fd->selectedFiles();
        file_name = list_file_name.at(0);
        ui->lineEdit_4->setText(file_name);
    }
    else if (ret == (int)QDialog::Rejected)
    {
        QMessageBox::information(this, "", "File dir rejected!\n");
        return;
    }
    delete fd;
}

void MainWindow::FINISH()
{
    system("/bin/ls");
    QFile *file = new QFile("tmp_result.txt");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream *in = new QTextStream(file);

    for(int i = 0; i < perfLayout->rowCount(); i++)
        perfLayout->removeRow(i);

    while (!in->atEnd())
    {
        QString line = in->readLine();
        QLabel *label = new QLabel();
        perfLayout->addRow(line, label);
        delete label;
    }

    file->close();

    delete file;
    delete in;
}
