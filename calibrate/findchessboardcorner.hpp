/**
 * 查找标定板角点
 */
/**
 * find chessboard corners
 */
#ifndef FINDCHESSBOARDCORNER_HPP
#define FINDCHESSBOARDCORNER_HPP

#include "../ui/propertiesdialog.hpp"

#include <QObject>
#include <opencv2/opencv.hpp>
#include <vector>
#include <QDebug>

using namespace std;
using cv::Mat;
using cv::imread;
using cv::imshow;

class FindChessboardCorner : public QObject
{
    Q_OBJECT
public:
    using uint = unsigned int;
    using properties = PropertiesDialog::properties;
    // （第一种标定板）棋盘格标定结果
    typedef struct result_ckbd{
        uint totalImg = 0;
        uint addedImg = 0;
        uint partiallyDetectedPatterns = 0;
        uint rejectedImg = 0;

        vector<Mat> validImgs; // 有效图像
        vector<Mat> invalidImgs; // 无效图像
        std::vector<std::vector<cv::Point3f>> objectPoints;
        std::vector<std::vector<cv::Point2f>> imagePoints;
    }result_ckbd;

public:
    explicit FindChessboardCorner(QObject *parent = nullptr){}
    explicit FindChessboardCorner(properties props, QObject *parent = nullptr)
        : QObject{parent},m_props(props){}


    /**
     * @brief (第一种标定板的交点查找)棋盘格标定板角点查找
     * @param fileNames 图像路径合集
     * @return 角点查找结果
     */
    result_ckbd checherboard(const QStringList& fileNames);




private:
    // 长度单位换算
    int dot_per_inch = 96;  //根据自己的电脑设置，DPI：每英寸的像素点数
    double cm_to_inch = 0.3937; //1cm=0.3937inch
    double inch_to_cm = 2.54;   //1inch = 2.54cm
    double inch_per_dot = 1.0 / 96.0;
    double cm_to_mm = 10;
    double inch_to_mm = 25.4;

    // 标定板属性集合
    const properties m_props;

    // 统计角点查找结果
    uint m_totalImg = 0;
    uint m_addedImg = 0;
    uint m_partiallyDetectedPatterns = 0;//unused
    uint m_rejectedImg = 0;





    /* 第二种标定板 */
    struct result_asym{

    };


    /* 第三种标定板 */
    struct result_sym{

    };


public slots:


signals:
//    void sendCkbdRes(result_ckbd); // 发送棋盘格标定板角点查找结果
};


inline FindChessboardCorner::result_ckbd
FindChessboardCorner::checherboard(const QStringList &fileNames)
{
    /* 尺寸转化为mm */
    double chessbd_sz = m_props.square_sz;//squareSize
    if(m_props.unit == "centimeters"){
        chessbd_sz *= cm_to_mm;
    }
    else if(m_props.unit == "inches"){
        chessbd_sz *= inch_to_mm;
    }

    vector<Mat> validImgs;    //有效图像
    vector<Mat> invalidImgs; // 无效图像
    vector<Mat> cornersImgs; // 绘制角点的图像
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    /* 构建棋盘格角点的世界坐标 */
    vector<cv::Point3f> objectcorners;
    for (int i = 0; i < m_props.rows; ++i) {
        for (int j = 0; j < m_props.cols; ++j) {
            objectcorners.push_back({cv::Point3f(j * chessbd_sz, i * chessbd_sz, 0)});
        }
    }


    /* load images */
    vector<Mat> imgs;
    for(const auto &fileName:fileNames){
        Mat img = imread(fileName.toStdString());
        if(!img.empty())
            imgs.push_back(img);
    }
    /* find corners */
    for(auto &img : imgs){
        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(gray, cv::Size(m_props.cols, m_props.rows), corners);

        /* 如果找到角点，添加到标定数据中 */
        if (found) {
            Mat img_corner = img.clone();
            cv::drawChessboardCorners(img_corner, cv::Size(m_props.cols, m_props.rows), corners, found);
            cornersImgs.push_back(img_corner);

            validImgs.push_back(img);
            imagePoints.push_back(corners);
            objectPoints.push_back(objectcorners);
        }
        else{
            invalidImgs.push_back(img);
        }
    }

    /* 有效/无效图像数量统计*/
    m_totalImg = fileNames.size();
    m_addedImg = validImgs.size(); // 能够查到角点的图像数量
    m_rejectedImg = m_totalImg - m_addedImg;  // 没有查到角点的图像数量

    /* 打包角点查找结果 */
    result_ckbd res;
    res.totalImg = m_totalImg;
    res.addedImg = m_addedImg;
    res.rejectedImg = m_rejectedImg;
    res.partiallyDetectedPatterns = m_partiallyDetectedPatterns;

    res.validImgs = validImgs;
    res.invalidImgs = invalidImgs;

    res.imagePoints = imagePoints;
    res.objectPoints = objectPoints;
    return res;

}


#endif // FINDCHESSBOARDCORNER_HPP
