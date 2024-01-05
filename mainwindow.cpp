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
#include "imageandpatternproperties.hpp"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*mainwindow design*/
    this->setWindowTitle(tr("calibrate Tool"));
    // tabwiddet_tool design
    ui->tabWidget_tool->setTabText(0,tr("mono calibrate"));
    ui->tabWidget_tool->setTabText(1,tr("stereo calibrate"));
    ui->tabWidget_tool->setTabText(2,tr("camera"));
    ui->tabWidget_tool->setTabEnabled(2,false);
    ui->tabWidget_tool->setCurrentIndex(0);

    // button set
    ui->pushButton_defaultLayout->setText(tr("Default\nLayout"));
    ui->radioButton_standard->setChecked(true);

    // add calibrate images (setting comboBox_addImages )
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
        if(fileNames.size() != 0){
            ImageAndPatternProperties *imageAndPatternPropertiesDialog = new ImageAndPatternProperties();
            imageAndPatternPropertiesDialog->show();
        }
        /* TODO
         * get checkboard params and corner detect
         *
         * */

    }

    /* add images from camera*/
    else if(idx == 2){
        ui->tabWidget_tool->setTabEnabled(2,true);
        ui->tabWidget_tool->setCurrentIndex(2);
        ui->tabWidget_tool->setTabEnabled(0,false);
        ui->tabWidget_tool->setTabEnabled(1,false);
        /* TODO
         * camera working
         * */

        connect(ui->pushButton_closeCamera,&QPushButton::clicked,this,[=](){
            ui->tabWidget_tool->setTabEnabled(2,false);
            ui->tabWidget_tool->setTabEnabled(0,true);
            ui->tabWidget_tool->setTabEnabled(1,true);
            ui->tabWidget_tool->setCurrentIndex(0);
        });
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
