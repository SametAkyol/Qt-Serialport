#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr);
    ~Widget();

    QSerialPort *qserial;
    void fill_port_settings();
    void open_port();
    void serial_write();
    void serial_read();
    void error_message();
    bool isOpen=false;





private slots:

    void on_pushOpen_clicked();
    void on_terminal_w_button_clicked();
    void on_pushButton_closeTerminal_clicked();
    void on_refresh_ports_clicked();

    void on_cleanTerminal_clicked();

private:
    Ui::Widget *ui;

};



#endif // WIDGET_H
