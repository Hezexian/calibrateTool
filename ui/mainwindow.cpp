#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include "../calibrate/imageFormat.hpp"

#include <QComboBox>
#include <QListView>
#include <QDebug>
#include <QThread>
#include <QMovie>
//#include <QOverload>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QDialog>
#include <QListWidget>
#include <QPixmap>
#include <QVBoxLayout>
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
    this->setFixedSize(1163,670);
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

    //Data Browser
    ui->tabWidget_dataBrowser->setTabText(0,tr("Data Browser"));
    connect(this,&MainWindow::foundCornersSig,this,[=](){
        this->tab_dataBrowser();
    });

    //Image (draw chessboard corners)
    ui->tabWidget_showImg->setTabText(0,tr("Image"));

    // calibrate button
    if(m_fileNames.empty())
        ui->pushButton_calibrate->setDisabled(true);
    connect(ui->pushButton_calibrate,&QPushButton::clicked,this,&MainWindow::pushButtonCalibrateClicked);

    // Show unDistorted button
    if(m_monoCaliParam.cameraMatrix.empty())
        ui->pushButton_showUndistorted->setDisabled(true);
    connect(ui->pushButton_showUndistorted,&QPushButton::clicked,this,&MainWindow::pushButtonShowUndistortedClicked);

    // export button
    if(m_monoCaliParam.cameraMatrix.empty())
        ui->pushButton_export->setDisabled(true);
    connect(ui->pushButton_export,&QPushButton::clicked,this,[=](){emit this->pushButtonExportClickedSig();});

    // Reprojection Errors
    ui->tabWidget_ReprojectonError->setTabText(0,tr("Reprojection Errors"));

    // Camera-centric
    ui->tabWidget_centric->setTabText(0,tr("Camera-centric"));

    // Pattern-centric
    ui->tabWidget_centric->setTabText(1,tr("Pattern-centric"));


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
            connect(imageAndPatternPropertiesDialog,&ImageAndPatternProperties::selectedPattern,
                    this,[=](ImageAndPatternProperties::properties props){
                        m_props = props;
                        this->findChessBoard(props);});
            imageAndPatternPropertiesDialog->exec();

        }

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

    if(!res_ckbd.validImgs.empty())
        ui->pushButton_calibrate->setDisabled(false);

    this->detectResDialog(res_ckbd);
}

void MainWindow::detectResDialog(const result_ckbd &res)
{
    QDialog *drDialog = new QDialog(this);//Detection results dialog
    drDialog->setWindowTitle(tr("Detection Results"));
    drDialog->setFixedSize(QSize(500,200));

    /* yes button*/
    QPushButton *yesbtn = new QPushButton(drDialog);
    yesbtn->setGeometry(207,153,72,26);
    yesbtn->setText(tr("yes"));
    connect(yesbtn,&QPushButton::clicked,drDialog,[=](){
        drDialog->close();
        emit foundCornersSig();
    });

    /* view images button*/
    QPushButton *viewbtn = new QPushButton(drDialog);
    viewbtn->setGeometry(370,119,106,26);
    viewbtn->setText(tr("view images"));
    connect(viewbtn,&QPushButton::clicked,drDialog,[=](){
        /* 展示没有检查到corner的标定板照片 */
        // Rejected Images
        QDialog *rejImgDlog = new QDialog(drDialog);
        rejImgDlog->setWindowTitle(tr("Rejected Images"));
        rejImgDlog->setFixedSize(QSize(1002,790));
        //有个bug，×掉后mainwindow会最小化，故暂时禁用×
        rejImgDlog->setWindowFlags(Qt::SplashScreen);
//        rejImgDlog->setWindowFlags(rejImgDlog->windowFlags() & ~Qt::WindowCloseButtonHint );
//        QLabel *tmp = new QLabel("有个bug，×掉后mainwindow\n会最小化，故暂时禁用×",rejImgDlog);
//        tmp->setGeometry(821,10,200,100);

        QLabel *label0 = new QLabel(rejImgDlog); // 展示被删除的图像
        // TODO:将res.rejectedImg合并成一张图片，转成QPixmap后在rejImgDlog(264,93)展现

        QPushButton *yesbtn = new QPushButton(rejImgDlog);
        yesbtn->setText(tr("yes"));
        yesbtn->setGeometry(464,757,72,26);
        connect(yesbtn,&QPushButton::clicked,
                rejImgDlog,[=](){rejImgDlog->close();drDialog->exec();});

        drDialog->hide();
        rejImgDlog->exec();
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


    drDialog->exec();

}

/** Data Brower 设计
 * @brief MainWindow::tab_dataBrowser
 */
void MainWindow::tab_dataBrowser()
{
    m_imgList = new QListWidget(ui->tab_dataBrowser);
    auto &imgList = m_imgList;

    int idx = 0;
    /* show draw chessboard images in IMAGE TAB*/
    connect(imgList,&QListWidget::itemClicked,this,[=](QListWidgetItem *item)mutable{
        idx = imgList->row(item);
        tab_image(idx);
    });

    /* show undistorted image */
    connect(this,&MainWindow::pushButtonShowUndistortedClickedSig,this,[=](){
        tab_image(idx);
    });


    /* add images into data browser */
    imgList->setGeometry(0,0,ui->tab_dataBrowser->width(),ui->tab_dataBrowser->height());
    imgList->setIconSize(QSize(imgList->width(),80));
    imgList->setSpacing(5);
    imgList->setResizeMode(QListView::Adjust);//适应布局调整
    imgList->setMovement(QListView::Static);//不能移动

    std::size_t cnt = 0;
    for(const auto & img : m_res_ckbd.validImgs){
        QListWidgetItem *item = new QListWidgetItem(imgList);
        item->setSizeHint(QSize(imgList->width(),80));

        // 创建图片并设置给ListWidgetItem
        QPixmap *pixmap = new QPixmap(Mat2Pixmap(img));
        pixmap->scaled(QSize(img.size().height*80/img.size().width, 80), Qt::KeepAspectRatio);
        item->setIcon(QIcon(*pixmap));
        item->setText(QString::number(cnt));
        ++cnt;
        imgList->addItem(item);

    }
    imgList->show();

    this->tab_image(idx);
}

/** show "draw chessboard corner"
 * @brief MainWindow::tab_image
 * @param index
 */
void MainWindow::tab_image(int index)
{
    Mat img;
    if(m_isUndistortShow){
        img = m_res_ckbd.validImgs[index];
        img = MonoCalibrate::undistort(img,m_monoCaliParam);
    }
    else{
        img = m_res_ckbd.cornersImgs[index];
    }
    QPixmap *pixmap = new QPixmap(Mat2Pixmap(img));
    QLabel *imglab = new QLabel(ui->tab_showImg);
    imglab->setPixmap(*pixmap);

    // TODO:将图像以合适的大小缩放
    // 暂时先填满
    imglab->setFixedSize(ui->tab_showImg->size().width(),
                         ui->tab_showImg->size().height());
    //------------------------


    imglab->show();

}

void MainWindow::pushButtonCalibrateClicked()
{
    /* 等待框 */
    QDialog *waitingDia = new QDialog(this);
    waitingDia->setWindowFlags(waitingDia->windowFlags() & ~Qt::WindowCloseButtonHint );
    waitingDia->setFixedSize(300,200);
    waitingDia->setWindowTitle(tr("waiting for calibrating ..."));

    QLabel* label = new QLabel(waitingDia);
    label->setFixedSize(300, 200);
    QMovie *movie = new QMovie(":/src/src/waiting-7579_256.gif");
    label->setMovie(movie);
    movie->start();

    QVBoxLayout* layout = new QVBoxLayout(waitingDia);
    layout->addWidget(label);

    waitingDia->setLayout(layout);
    label->show();
    waitingDia->show();

    /* 等待标定时禁用所有按钮 */
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        button->setDisabled(true);
    }
    ui->comboBox_addImages->setDisabled(true);
    ui->radioButton_standard->setDisabled(true);
    ui->radioButton_fisheye->setDisabled(true);
    ui->doubleSpinBox->setDisabled(true);

    /* 标定程序 */
    QThread *caliThread = new QThread(this);
    MonoCalibrate *monocali = new MonoCalibrate(m_res_ckbd);
    monocali->moveToThread(caliThread);
    caliThread->start();
    connect(monocali,&MonoCalibrate::CaliStarted,monocali,&MonoCalibrate::calibrate);
    emit monocali->CaliStarted();

    /* 结束标定 */
    connect(monocali,&MonoCalibrate::caliFinished,this,[=](CaliParam caliParam){
        // 保存标定结果
        m_monoCaliParam = caliParam;

        // 解禁按钮
        ui->pushButton_showUndistorted->setDisabled(false);
        ui->pushButton_export->setDisabled(false);
        ui->comboBox_addImages->setDisabled(false);
        ui->radioButton_standard->setDisabled(false);
        ui->radioButton_fisheye->setDisabled(false);
        ui->doubleSpinBox->setDisabled(false);
        for (QPushButton* button : buttons) {
            button->setDisabled(false);
        }

        // 释放资源
        waitingDia->close();
        monocali->deleteLater();
        caliThread->exit();
    });

    /* export button （导出按钮） */
    connect(this,&MainWindow::pushButtonExportClickedSig,this,[=](){
        MonoCalibrate::exportParams(m_monoCaliParam);
    });


}

void MainWindow::pushButtonShowUndistortedClicked()
{
    if(m_isUndistortShow){
        m_isUndistortShow = false;
        emit pushButtonShowUndistortedClickedSig(m_isUndistortShow);
    }else{
        m_isUndistortShow = true;
        emit pushButtonShowUndistortedClickedSig(m_isUndistortShow);
    }
}

