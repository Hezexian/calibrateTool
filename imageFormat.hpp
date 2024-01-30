/**
 * 图像格式转换函数
 */

#ifndef IMAGEFORMAT_HPP
#define IMAGEFORMAT_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <QImage>
#include <QPixmap>
#include <QBuffer>
using namespace std;
using namespace cv;

QImage Mat2QImage(const Mat& cvImage) {
    vector<uchar> imgBuf;
    imencode(".bmp", cvImage, imgBuf);

    QByteArray baImg((char*)imgBuf.data(), static_cast<int>(imgBuf.size()));
    QImage image;
    image.loadFromData(baImg, "BMP");
    return image;
}

QPixmap Mat2Pixmap(const Mat& cvImage)
{
    return QPixmap::fromImage(Mat2QImage(cvImage));
}

Mat QImage2Mat(const QImage image) {
    // 将 QImage 保存为 BMP 格式的二进制数据
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "BMP");

    // 将二进制数据转换为 cv::Mat
    std::vector<uchar> imgBuf(ba.begin(), ba.end());
    cv::Mat mat = cv::imdecode(imgBuf, cv::IMREAD_COLOR);
    return mat;
}

Mat Pixmap2Mat(const QPixmap& image) {
    return QImage2Mat(image.toImage());
}

#endif // IMAGEFORMAT_HPP
