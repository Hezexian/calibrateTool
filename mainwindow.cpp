#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QComboBox>
#include <QListView>
#include <QDebug>
//#include <QOverload>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // add calibrate images
    QStringList addImages = {tr("add Images"),tr("From images"),tr("From camera")};
    ui->comboBox_addImages->addItems(addImages);
    QListView *view = dynamic_cast<QListView *>(ui->comboBox_addImages->view());
    view->setRowHidden(0,true);
    connect(ui->comboBox_addImages, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::addImages);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// add images to program
void MainWindow::addImages(int idx)
{
    /* add images from file*/
    if(idx == 1){
        // load images' paths
        QStringList fileNames = QFileDialog::getOpenFileNames(
            this, tr("Open File"), QDir::currentPath(),
            "Images (*.png *.jpg *.jpeg)");
        // set checkboard params

        for(const auto &filename : fileNames){
            qDebug()<<filename;
        }


    }


    /* add images from camera*/
    else if(idx == 2){

    }
}

// Get the last opened directory 获取上一次打开的目录
QString MainWindow::getLastOpenedDirectory() {
    QSettings settings("Nanchang University", "calibrateTool");
    return settings.value("lastOpenedDirectory", QDir::homePath()).toString();
}

// Save the currently open directory 保存当前打开的目录
void MainWindow::saveLastOpenedDirectory(const QString& directory) {
    QSettings settings("Nanchang University", "calibrateTool");
    settings.setValue("lastOpenedDirectory", directory);
}
