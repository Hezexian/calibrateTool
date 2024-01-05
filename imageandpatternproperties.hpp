#ifndef IMAGEANDPATTERNPROPERTIES_HPP
#define IMAGEANDPATTERNPROPERTIES_HPP

#include <QDialog>
#include <QPushButton>
#include "./ui_imageandpatternproperties.h"


namespace Ui {
class ImageAndPatternProperties;
}

class ImageAndPatternProperties : public QDialog
{
    Q_OBJECT

public:
    explicit ImageAndPatternProperties(QWidget *parent = nullptr);
    ~ImageAndPatternProperties();

private:
    Ui::ImageAndPatternProperties *ui;

public slots:
//    void pushButton_custompattern_togged(bool checked);
};



#endif // IMAGEANDPATTERNPROPERTIES_HPP

