#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::SerialPortInit(void){
    serial=new QSerialPort;//申请内存，并且设置父对象

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        serial->setPort(info);
        if(serial->open(QIODevice::ReadWrite))//以读写方式打开 串口
        {
            qDebug()<<"串口打开成功";
            serial->close();//关闭
        }
        else{
            qDebug()<<"串口打开失败,请重试";
        }
    }
    //设置波特率
    serial->setBaudRate(QSerialPort::Baud19200);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置数据位
    serial->setDataBits(QSerialPort::Data8);
    //设置校验
    serial->setParity(QSerialPort::NoParity);
    //设置流控
    serial->setFlowControl(QSerialPort::NoFlowControl);


}

void MainWindow::on_PBOpen_clicked()
{
    if(ui->PBOpen->text()==QString("Open")){
            // 检查串口是否被占用

        //串口配置
        //清空缓冲区
        serial=new QSerialPort;
        //获取计算机有效端口号，然后将端口号的名称给端口选择控件
        //QSerialPortInfo::availablePorts()的返回值为QList<QSerialPortInfo> 端口组成容器
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
            serial->setPort(info);
            if(serial->open(QIODevice::ReadWrite))//以读写方式打开 串口
            {
                qDebug()<<"串口打开成功";
                serial->close();//关闭
            }
            else{
                qDebug()<<"串口打开失败,请重试";
            }
        }
        //设置波特率
        serial->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->BaudcomboBox->currentText().toInt()));
        //设置停止位
        serial->setStopBits(static_cast<QSerialPort::StopBits>(ui->Stop_comBox->currentText().toInt()));
        //设置数据位
        serial->setDataBits(static_cast<QSerialPort::DataBits>(ui->Data_comboBox->currentText().toInt()));
        //设置校验
        serial->setParity(static_cast<QSerialPort::Parity>(ui->Parity_comboBox->currentText().toInt()));
        //设置流控
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //改变按钮上的文本
        ui->PBOpen->setText("Close");
        //输入通道的端口名字
        ui->SeriallineEdit->setText(serial->portName());
        isSerialOpen=serial->open(QIODevice::ReadWrite);
        serial->setDataTerminalReady(true);
        serial->setReadBufferSize(3);
        if(!isSerialOpen){
            qDebug()<<QString("Failed to open serial port:")<<serial->errorString();
            serial->clearError();
        }
        else{
            qDebug()<<QString("The serial port is open:");
        }

}
    else{
        ui->PBOpen->setText("Open");
        serial->close();
    }
}


void MainWindow::initConfig(){
    //创建对象，并建立信号槽

    serial=new QSerialPort(this);
    //读函数信号槽
    QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::serial_readyRead);
    QStringList baudList;
    baudList<<"115200"<<"57600"<<"9600";
    ui->BaudcomboBox->addItems(baudList);
    ui->BaudcomboBox->setCurrentText("115200");

    //添加停止位
    QStringList baudList1;
    baudList1<<"0.5"<<"1"<<"1.5"<<"2";
    ui->Stop_comBox->addItems(baudList1);
    ui->Stop_comBox->setCurrentText("1");

    //添加数据位
    QStringList baudList2;
    baudList2<<"8"<<"9";
    ui->Data_comboBox->addItems(baudList2);
    ui->Data_comboBox->setCurrentText("8");

    //奇偶校验
    QStringList baudList3;
    baudList3<<"No"<<"EVEN";
    ui->Parity_comboBox->addItems(baudList3);
    ui->Parity_comboBox->setCurrentText("NO");
}

void MainWindow::on_SendPB_clicked()
{
    if(ui->PBOpen->text()==QString("CLOSE")){
        QByteArray data=ui->SendSerialtextEdit->toPlainText().toUtf8();
        serial->write(data);
        qDebug()<<serial;

    }
    else{
        ui->SendSerialtextEdit->append("请打开串口");
    }
}

void MainWindow::serial_readyRead(){
    //从接收缓冲区中读取数据
    QByteArray buffer=serial->readAll();
    //界面中读取以前收到的数据
    QString recv=ui->Rec_text_Edit->toPlainText();
    recv+=QString(buffer);
    //清空以前的显示
    ui->Rec_text_Edit->clear();
    //重新显示
    ui->Rec_text_Edit->append(recv);
    qDebug()<<recv<<"cxc";
}

void MainWindow::on_clearPB_clicked()
{
    //清除显示
    ui->Rec_text_Edit->clear();
}
