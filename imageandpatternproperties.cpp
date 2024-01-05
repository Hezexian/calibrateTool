#include "imageandpatternproperties.hpp"
#include "qlineedit.h"
#include <QDebug>
#include <QSize>
#include <QTextEdit>

ImageAndPatternProperties::ImageAndPatternProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageAndPatternProperties)
{
    ui->setupUi(this);

    // setting dialog
    this->setWindowTitle(tr("Image and Pattern Propeties"));
    this->setFixedSize(QSize(553,350));
    ui->tabWidget_properties->setFixedSize(QSize(531,200));

    //setting pushButton_custompattern
    ui->frame_customPattern->hide();
    connect(ui->pushButton_customPattern,&QPushButton::toggled,this,[=](bool checked){
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

    // setting properties groupbox
    ui->tabWidget_properties->setTabText(0,tr("checkerboard"));
    ui->tabWidget_properties->setTabText(1,tr("Asymmetric Circle Grid"));
    ui->tabWidget_properties->setTabText(2,tr("Symmetric Circle Grid"));

    ui->tabWidget_properties->setCurrentIndex(0);
    //init checkerboard
    ui->lineEdit_size->setText("25");
    ui->radioButton_low->setChecked(true);
    QPixmap pixmap = QPixmap(":/src/src/checkboad1.png");
    ui->label_checker->setPixmap(pixmap);
    //init asym
    ui->lineEdit_asymSize->setText("25");
    ui->lineEdit_dim1->setText("3");
    ui->lineEdit_dim2->setText("5");
    ui->label_asymImg->setPixmap(QPixmap(":/src/src/checkboad2.png"));
    //init sym
    ui->lineEdit_symSize->setText("25");
    ui->lineEdit_rows->setText("5");
    ui->lineEdit_colums->setText("5");
    ui->label_synImg->setPixmap(QPixmap(":/src/src/checkboard3.png"));
}

ImageAndPatternProperties::~ImageAndPatternProperties()
{
    delete ui;
}


