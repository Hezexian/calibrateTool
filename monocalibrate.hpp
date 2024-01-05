#ifndef MONOCALIBRATE_HPP
#define MONOCALIBRATE_HPP

#include <QObject>
#include <opencv2/opencv.hpp>

class MonoCalibrate : public QObject
{
    Q_OBJECT
public:
    explicit MonoCalibrate(QObject *parent = nullptr);

signals:

};

MonoCalibrate::MonoCalibrate(QObject *parent)
    : QObject{parent}
{

}

#endif // MONOCALIBRATE_HPP
