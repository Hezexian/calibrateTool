#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QStringList>

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


private:
    Ui::MainWindow *ui;

signals:


public slots:
    void addImages(int index);

};
#endif // MAINWINDOW_HPP
