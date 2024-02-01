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
    ui->lineEdit_chessbd_rows->setText("6");
    ui->lineEdit_chessbd_cols->setText("4");
    QPixmap pixmap = QPixmap(":/src/src/checkboad1.png");
    ui->label_checker->setPixmap(pixmap);
    //init asym
    ui->lineEdit_asymSize->setText("25");
    ui->lineEdit_dim1->setText("3");
    ui->lineEdit_dim2->setText("5");
    ui->label_asymImg->setPixmap(QPixmap(":/src/src/checkboad2.png"));
    //init sym
    ui->lineEdit_symSize->setText("25");
    ui->lineEdit_sy_rows->setText("5");
    ui->lineEdit_sy_cols->setText("5");
    ui->label_synImg->setPixmap(QPixmap(":/src/src/checkboard3.png"));

    // Dialog button
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&ImageAndPatternProperties::dialogBtnBox);
}

ImageAndPatternProperties::~ImageAndPatternProperties()
{
    delete ui;
}

// dialog button slot
void ImageAndPatternProperties::dialogBtnBox()
{

    switch(ui->tabWidget_properties->currentIndex()){
        case 0:{
        // send checker box params
            properties props;
            props.pattern = "checkerboard";
            props.square_sz = ui->lineEdit_size->text().toDouble();
            props.unit = ui->comboBox_unit->currentText();
            props.rows = ui->lineEdit_chessbd_rows->text().toUInt();
            props.cols = ui->lineEdit_chessbd_cols->text().toUInt();
            if(ui->radioButton_low){
                props.isDistortionLow = true;
            }else{
                props.isDistortionLow = false;
            }

            emit selectedPattern(props);

            break;
        }
        case 1:{
            // send asy parms
            properties props;
            props.square_sz = ui->lineEdit_asymSize->text().toDouble();
            props.unit = ui->comboBox_unit_2->currentText();
            props.dim1 = ui->lineEdit_dim1->text().toUInt();
            props.dim2 = ui->lineEdit_dim2->text().toUInt();

            emit selectedPattern(props);
            break;
        }
        case 2:{
            // send sy parms
            properties props;
            props.square_sz = ui->lineEdit_symSize->text().toDouble();
            props.unit = ui->comboBox_unit_3->currentText();
            props.rows = ui->lineEdit_sy_rows->text().toUInt();
            props.cols = ui->lineEdit_sy_cols->text().toUInt();
            emit selectedPattern(props);

            break;
        }
        default:{}
    }

}


