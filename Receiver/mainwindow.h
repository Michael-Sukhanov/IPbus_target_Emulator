#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <emulator.h>
//#include <QMediaPlayer>
//#include <QMediaPlaylist>

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

    void on_AdresslineEdit_editingFinished();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket* socket;
    QByteArray buffer;
    quint16 senderPort;
    QHostAddress senderAdress;
    Emulator obj;
//    QMediaPlayer* notif;


};
#endif // MAINWINDOW_H
