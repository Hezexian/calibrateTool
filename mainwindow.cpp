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

#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
using cv::Mat;
using cv::imread;
using cv::imread;
using cv::write;
using cv::waitKey;


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
        m_fileNames = QFileDialog::getOpenFileNames(
            this, tr("Open File"), QDir::currentPath(),
            "Images (*.png *.jpg *.jpeg)");
        // set checkboard params
        if(m_fileNames.size() != 0){
            ImageAndPatternProperties *imageAndPatternPropertiesDialog = new ImageAndPatternProperties();
            imageAndPatternPropertiesDialog->show();
            connect(imageAndPatternPropertiesDialog,&ImageAndPatternProperties::selectedPattern,
                    this,[=](ImageAndPatternProperties::properties props){
                m_props = props;
                this->findChessBoard(props);});
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

void MainWindow::findChessBoard(const properties props)
{
    FindChessboardCorner find(props);
    result_ckbd res_ckbd;
    res_ckbd = find.checherboard(this->m_fileNames);
    m_res_ckbd = res_ckbd;

    this->detectResDialog(res_ckbd);
}

void MainWindow::detectResDialog(const result_ckbd &res)
{
    QDialog *drDialog = new QDialog(this);//Detection results dialog
    drDialog->setWindowTitle(tr("Detection Results"));
    drDialog->setFixedSize(QSize(500,200));

    QPushButton *yesbtn = new QPushButton(drDialog);
    yesbtn->setGeometry(207,153,72,26);
    yesbtn->setText(tr("yes"));
    connect(yesbtn,&QPushButton::clicked,drDialog,[=](){drDialog->close();});

    QPushButton *viewbtn = new QPushButton(drDialog);
    viewbtn->setGeometry(370,119,106,26);
    viewbtn->setText(tr("view images"));
    connect(viewbtn,&QPushButton::clicked,drDialog,[=](){
        /* 浏览标定图像的窗口 */
        // TODO: 浏览标定图像的窗口 2024/1/30

    });


    /* labels */
    QLabel *label0 = new QLabel(tr("Total images processed:"),drDialog);
    QLabel *label1 = new QLabel(tr("Added imades:"),drDialog);
    QLabel *label2 = new QLabel(tr("Partially detected patterns:"),drDialog);
    QLabel *label3 = new QLabel(tr("Rejected images:"),drDialog);

    QLabel *label4 = new QLabel(QString::number(res.totalImg),drDialog);
    QLabel *label5 = new QLabel(QString::number(res.addedImg),drDialog);
    QLabel *label6 = new QLabel(QString::number(res.partiallyDetectedPatterns),drDialog);
    QLabel *label7 = new QLabel(QString::number(res.rejectedImg),drDialog);

    label0->setGeometry(21,25,221,21);label4->setGeometry(334,25,21,21);
    label1->setGeometry(21,46,221,21);label5->setGeometry(334,46,21,21);
    label2->setGeometry(21,65,221,21);label6->setGeometry(334,65,21,21);
    label3->setGeometry(21,89,221,21);label7->setGeometry(334,89,21,21);




    drDialog->show();

}
