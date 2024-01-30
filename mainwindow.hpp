#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QStringList>
#include "imageandpatternproperties.hpp"
#include "findchessboardcorner.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 获取上一次打开的目录
    QString getLastOpenedDirectory();
    // 保存当前打开的目录
    void saveLastOpenedDirectory(const QString& directory);


    // find (detect) chessboard corner
    using properties = ImageAndPatternProperties::properties;
    void findChessBoard(properties props);

    // Detection Results Dialog
    using result_ckbd = FindChessboardCorner::result_ckbd;
    void detectResDialog(const result_ckbd&);


private:
    QStringList m_fileNames;
    Ui::MainWindow *ui;

    properties m_props;
    result_ckbd m_res_ckbd;

signals:


public slots:
    void addImages(int index);

};
#endif // MAINWINDOW_HPP
