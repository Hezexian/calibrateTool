/**
 * 单目相机标定
 */
#ifndef MONOCALIBRATE_HPP
#define MONOCALIBRATE_HPP

#include <QObject>
#include <opencv2/opencv.hpp>
#include <vector>
#include "findchessboardcorner.hpp"
#include <QFileDialog>
#include <QString>
#include <QThread>

using namespace std;
using namespace cv;

class MonoCalibrate : public QObject
{
    Q_OBJECT
public:
    /* 类的定义 */
    struct CaliParam{
        cv::Mat cameraMatrix, distCoeffs;
        std::vector<cv::Mat> rvecs, tvecs;
        std::vector<double> stdDeviationsIntrinsics,stdDeviationsExtrinsics;
        std::vector<double> perViewErrors;
        double rms;
    };
    using result_ckbd = FindChessboardCorner::result_ckbd;

public:
    MonoCalibrate(result_ckbd res_ckbd,QObject *parent = nullptr)
        :m_res_ckbd{res_ckbd}{};
    MonoCalibrate(const MonoCalibrate &caliobj)
        :m_res_ckbd(caliobj.m_res_ckbd),m_caliparam(caliobj.m_caliparam){ };

    /** 张正友标定法功能函数
     * @brief calibrate
     * @return
     */
    CaliParam calibrate();

    /** 矫正畸变
     * @brief undistort
     * @param srcImg
     * @return 矫正后的图像
     */
    Mat undistort(const Mat& srcImg);

    /** 导出标定结果
     * @brief exportParams
     */
    static void exportParams(CaliParam &caliparam) ;



private:
    result_ckbd m_res_ckbd;
    CaliParam m_caliparam;


signals:
    /** 标定开始信号
     * @brief CaliStarted
     */
    void CaliStarted();

    /** 标定结束信号
     * @brief caliFinished
     */
    void caliFinished(CaliParam);


};

inline MonoCalibrate::CaliParam
MonoCalibrate::calibrate()
{

    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    std::vector<double> stdDeviationsIntrinsics,stdDeviationsExtrinsics;
    std::vector<double> perViewErrors;

    // calibrate camera
    // https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html#ga3207604e4b1a1758aa66acb6ed5aa65d
    TermCriteria  criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON);
    double rms  = cv::calibrateCamera(
                        m_res_ckbd.objectPoints,m_res_ckbd.imagePoints,
                        m_res_ckbd.validImgs[0].size(),
                        cameraMatrix, distCoeffs,
                        rvecs, tvecs,
                        stdDeviationsIntrinsics,stdDeviationsExtrinsics,
                        perViewErrors,0,criteria);


    CaliParam caliparam;
    caliparam.cameraMatrix = cameraMatrix;
    caliparam.distCoeffs = distCoeffs;
    caliparam.rvecs = rvecs;
    caliparam.tvecs = tvecs;
    caliparam.stdDeviationsIntrinsics = stdDeviationsIntrinsics;
    caliparam.stdDeviationsExtrinsics = stdDeviationsExtrinsics;
    caliparam.perViewErrors = perViewErrors;
    caliparam.rms = rms;
    m_caliparam = caliparam;

    emit caliFinished(m_caliparam);
    return caliparam;
}

inline Mat MonoCalibrate::undistort(const Mat &srcImg)
{
    cv::Mat undistortedImage;
    cv::undistort(srcImg, undistortedImage, m_caliparam.cameraMatrix, m_caliparam.distCoeffs);
    return undistortedImage;
}

inline void MonoCalibrate::exportParams(CaliParam &caliparam)
{


    QString folderPath = QFileDialog::getExistingDirectory(
        nullptr,
        tr("choose folder"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    QDir directory(folderPath);
    cv::FileStorage fs(directory.filePath("calibration_result.yml").toStdString(), cv::FileStorage::WRITE);
    fs << "cameraMatrix" << caliparam.cameraMatrix;
    fs << "distCoeffs" << caliparam.distCoeffs;
    fs << "rms"<<caliparam.rms;

    // 写入 rvecs 向量
    fs << "rvecs" << "["; // 开始写入向量
    for (const cv::Mat& rvec : caliparam.rvecs) {
        fs << rvec;
    }
    fs << "]"; // 结束写入向量

    // 写入 tvecs 向量
    fs << "tvecs" << "["; // 开始写入向量
    for (const cv::Mat& tvec : caliparam.tvecs) {
        fs << tvec;
    }
    fs << "]"; // 结束写入向量

    // 写入 stdDeviationsIntrinsics 向量
    fs << "stdDeviationsIntrinsics" << "["; // 开始写入向量
    for (double stdDevI : caliparam.stdDeviationsIntrinsics) {
        fs << stdDevI;
    }
    fs << "]"; // 结束写入向量

    // 写入 stdDeviationsExtrinsics 向量
    fs << "stdDeviationsExtrinsics" << "["; // 开始写入向量
    for (double stdDevE : caliparam.stdDeviationsExtrinsics) {
        fs << stdDevE;
    }
    fs << "]"; // 结束写入向量

    // 写入 perViewErrors 向量
    fs << "perViewErrors" << "["; // 开始写入向量
    for (double pError : caliparam.perViewErrors) {
        fs << pError;
    }
    fs << "]"; // 结束写入向量

    fs.release();
}





#endif // MONOCALIBRATE_HPP
