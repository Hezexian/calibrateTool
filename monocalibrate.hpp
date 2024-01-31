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

using namespace std;
using namespace cv;

class MonoCalibrate : public QObject
{
    Q_OBJECT
public:
    /* 类的定义 */
    typedef struct CaliParam{
        cv::Mat cameraMatrix, distCoeffs;
        std::vector<cv::Mat> rvecs, tvecs;
        std::vector<double> stdDeviationsIntrinsics,stdDeviationsExtrinsics;
        std::vector<double> perViewErrors;
        double rms;
    }CaliParam;
    using result_ckbd = FindChessboardCorner::result_ckbd;

public:
    explicit MonoCalibrate(result_ckbd res_ckbd,QObject *parent = nullptr)
        :m_res_ckbd{res_ckbd}{};

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
    void exportParams() const;



private:
    result_ckbd m_res_ckbd;
    CaliParam m_caliparam;


signals:

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
    return caliparam;
}

inline Mat MonoCalibrate::undistort(const Mat &srcImg)
{
    cv::Mat undistortedImage;
    cv::undistort(srcImg, undistortedImage, m_caliparam.cameraMatrix, m_caliparam.distCoeffs);
    return undistortedImage;
}

inline void MonoCalibrate::exportParams() const
{
    // 保存标定结果
    QString folderPath = QFileDialog::getExistingDirectory(
        nullptr,
        tr("choose folder"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    QDir directory(folderPath);
    cv::FileStorage fs(directory.filePath("calibration_result.yml").toStdString(), cv::FileStorage::WRITE);
    fs << "cameraMatrix" << m_caliparam.cameraMatrix;
    fs << "distCoeffs" << m_caliparam.distCoeffs;
    fs << "rvecs"<<m_caliparam.rvecs;
    fs << "tvecs"<<m_caliparam.tvecs;
    fs << "stdDeviationsIntrinsics"<<m_caliparam.stdDeviationsIntrinsics;
    fs << "stdDeviationsExtrinsics"<<m_caliparam.stdDeviationsExtrinsics;
    fs << "perViewErrors"<<m_caliparam.perViewErrors;
    fs << "rms"<<m_caliparam.rms;
    fs << "rvecs"<<m_caliparam.rvecs;

    fs.release();
}





#endif // MONOCALIBRATE_HPP
