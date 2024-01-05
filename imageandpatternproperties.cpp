#include "imageandpatternproperties.hpp"
#include <QDebug>
#include <QSize>

ImageAndPatternProperties::ImageAndPatternProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageAndPatternProperties)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(553,350));
    ui->groupBox_patternSelect->setFixedSize(QSize(531,67));
    ui->groupBox_properties->setFixedSize(QSize(531,515));
    ui->frame_customPattern->hide();
    connect(ui->pushButton_custompattern,&QPushButton::toggled,this,[=](bool checked){
        if(checked){
            ui->frame_customPattern->show();
            this->setFixedSize(QSize(553,460));
        }
        else{
            ui->frame_customPattern->hide();
            this->setFixedSize(QSize(553,350));
        }

    });
    ui->label_customPattern->setText(
        tr("Click import Pattern Detector to import a class that contains a detector for your custom\n"
           "pattern. When you import the class,it will appear as a choice in the Choice Pattern\n"
           " drop-down menu. "));
}

ImageAndPatternProperties::~ImageAndPatternProperties()
{
    delete ui;
}

//inline void ImageAndPatternProperties::pushButton_custompattern_togged(bool checked)
//{

//}
