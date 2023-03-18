#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qserial= new QSerialPort;
    Widget::fill_port_settings();
    connect(qserial,&QSerialPort::readyRead,this,&Widget::serial_read);
    connect(qserial,&QSerialPort::errorOccurred,this, & Widget::error_message);
}


Widget::~Widget()
{
    delete ui;
    qserial->close();
    qDebug()<<"Com closed";
}


void Widget::fill_port_settings(){

    foreach ( const QSerialPortInfo & serialport_info, QSerialPortInfo::availablePorts()) {
        ui->portList->addItem(serialport_info.portName());
    }

    foreach (quint32 baud, QSerialPortInfo::standardBaudRates()) {
            ui->baudRate->addItem(QString::number(baud), baud);
        }

        ui->parity->addItem(QStringLiteral("None"), QSerialPort::NoParity);
        ui->parity->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
        ui->parity->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
        ui->parity->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
        ui->parity->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

        ui->dataBits->addItem(QStringLiteral("5"),QSerialPort::Data5);
        ui->dataBits->addItem(QStringLiteral("6"),QSerialPort::Data6);
        ui->dataBits->addItem(QStringLiteral("7"),QSerialPort::Data7);
        ui->dataBits->addItem(QStringLiteral("8"),QSerialPort::Data8);

        ui->stopBits->addItem(QStringLiteral("1"),QSerialPort::OneStop);
        ui->stopBits->addItem(QStringLiteral("1.5"),QSerialPort::OneAndHalfStop);
        ui->stopBits->addItem(QStringLiteral("2"),QSerialPort::TwoStop);
}


void Widget::on_pushOpen_clicked()
{
    Widget::open_port();
    qserial->open(QIODevice::ReadWrite);

    if (qserial->isOpen()) {
            ui->portState->setText(qserial->portName()+" is now open");
            return;
    }
    ui->portState->setText(qserial->portName()+" cannot open\n");
}


void Widget::on_terminal_w_button_clicked()
{
    if (!qserial->isOpen()) {
        ui->portState->setText("No open ports\n");
        return;
    }
        Widget::serial_write();
}


void Widget::open_port(){

    QSerialPort::BaudRate baud = static_cast<QSerialPort::BaudRate>(ui->baudRate->currentData().toInt());
    QSerialPort::DataBits databits = static_cast<QSerialPort::DataBits>(ui->dataBits->currentData().toInt());
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(ui->parity->currentData().toInt());
    QSerialPort::StopBits stop = static_cast<QSerialPort::StopBits>(ui->stopBits->currentData().toInt());

    if (!qserial->isOpen()) {
        qserial->setPortName(ui->portList->currentText());
        qserial->setBaudRate(baud);
        qserial->setDataBits(databits);
        qserial->setParity(parity);
        qserial->setStopBits(stop);
    }
}


void Widget::serial_write(){
    QByteArray str=ui->terminal_write->displayText().toUtf8();
    qserial->write(str);
    ui->terminal_write->clear();
}


void Widget::on_pushButton_closeTerminal_clicked()
{
    if (qserial->isOpen()) {
        qserial->close();
        ui->portState->setText(qserial->portName()+" Closed\n");
        return;
    }
         ui->portState->setText("Open port not found\n");
}


void Widget::on_refresh_ports_clicked()
{
   ui->portList->clear();
    foreach ( const QSerialPortInfo & serialport_info, QSerialPortInfo::availablePorts()) {  
        ui->portList->addItem(serialport_info.portName());
    }
}


void Widget::serial_read(){
    QString rec_data=qserial->readAll();
    ui->terminal->insertPlainText(rec_data);
}


void Widget::on_cleanTerminal_clicked()
{
    ui->terminal->clear();
}


void Widget::error_message()
{
    qDebug()<<qserial->error();
}
