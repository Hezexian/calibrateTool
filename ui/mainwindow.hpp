#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "../calibrate/findchessboardcorner.hpp"
#include "../calibrate/monocalibrate.hpp"
#include "propertiesdialog.hpp"


#include <QMainWindow>
#include <QStringList>
#include <QListWidgetItem>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 获取上一次打开的目录
    QString getLastOpenedDirectory();
    // 保存当前打开的目录
    void saveLastOpenedDirectory(const QString& directory);


    // find (detect) chessboard corner
    using properties = PropertiesDialog::properties; // 标定板属性
    void findChessBoard(properties props);  
    using result_ckbd = FindChessboardCorner::result_ckbd;// 角点查找结果
    void detectResDialog(const result_ckbd&); // 展示角点检测结果的对话框

    // 标定图像列表(data browser)
    void tab_dataBrowser();

    // Image tab(image 标签)
    void tab_image(int index);

    // calibrate btn
    using CaliParam = MonoCalibrate::CaliParam;


private:
    QStringList m_fileNames;
    Ui::MainWindow *ui;

    properties m_props; // 标定板属性
    result_ckbd m_res_ckbd;//角点查找结果
    CaliParam m_monoCaliParam; //标定的相机参数
    QListWidget *m_imgList; //左侧图像列表

    bool m_isUndistortShow = false; //image tab中是否展示矫正后的图像
    int m_dataBrowserIdx = 0; // dataBrowser tab的图像列表序号


signals:
    // find chessboard corner 任务完成
    void foundCornersSig();
    void pushButtonExportClickedSig();
    void pushButtonShowUndistortedClickedSig(bool);



public slots:
    void addImages(int index);
    void pushButtonCalibrateClicked();
    void pushButtonOptionsClicked();

};
#endif // MAINWINDOW_HPP
