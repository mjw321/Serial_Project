#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_PBOpen_clicked();

    void on_SendPB_clicked();

    void on_clearPB_clicked();

private:
    Ui::MainWindow *ui;

    void SerialPortInit(void);
    void initConfig();
    void serial_readyRead();
    void configSetEnable(bool b);


    bool isSerialOpen;


    QSerialPort *serial;//定义全局的串口对象
    QStringList baudList;//波特率
    QStringList parityList;//校验位
    QStringList dataBitsList;//数据位
    QStringList stopBitsList;//停止位
    QStringList flowControlList;//控制流

};
#endif // MAINWINDOW_H
