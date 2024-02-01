/**
 * Image and pattern Propeties Dialog
 */
#ifndef IMAGEANDPATTERNPROPERTIES_HPP
#define IMAGEANDPATTERNPROPERTIES_HPP

#include "./ui_imageandpatternproperties.h"

#include <QDialog>
#include <QPushButton>


namespace Ui {
class ImageAndPatternProperties;
}

class ImageAndPatternProperties : public QDialog
{
    Q_OBJECT

public:
    explicit ImageAndPatternProperties(QWidget *parent = nullptr);
    ~ImageAndPatternProperties();

    struct properties{
        QString pattern = ""; // 三种模式共有属性.将会根据属性名称判断读取哪些属性
        double square_sz = 0;
        QString unit = ""; // 长度单位

        // checker board
        bool isDistortionLow = true;

        // checker board  & sy
        unsigned rows = 0;
        unsigned cols = 0;

        // asy
        unsigned dim1 = 0;
        unsigned dim2 = 0;
    };

private:
    Ui::ImageAndPatternProperties *ui;

public slots:
    void dialogBtnBox();

signals:
    void selectedPattern(properties);
};



#endif // IMAGEANDPATTERNPROPERTIES_HPP

