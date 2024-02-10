#ifndef STEROCALIBRATE_HPP
#define STEROCALIBRATE_HPP

#include <QObject>

class SteroCalibrate : public QObject
{
    Q_OBJECT
public:
    explicit SteroCalibrate(QObject *parent = nullptr)
        : QObject{parent}{}

signals:

};



#endif // STEROCALIBRATE_HPP
