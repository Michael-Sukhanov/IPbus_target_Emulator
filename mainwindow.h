#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <emulator.h>
#include <QShortcut>
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

protected:
    bool eventFilter(QObject* , QEvent*);

private slots:
    void on_pushButton_clicked();

    void on_AdresslineEdit_editingFinished();

    void on_pushButton_2_clicked();

    void load();

    void on_pushButton_info_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket* socket;
    QByteArray buffer;
    quint16 senderPort;
    QHostAddress senderAdress;
    Emulator obj;
    QShortcut *info = new QShortcut(this);
    QShortcut *clear= new QShortcut(this);
    QShortcut *exit = new QShortcut(this);
    QShortcut *bind = new QShortcut(this);
    QShortcut *w_log= new QShortcut(this);

//    QMediaPlayer* notif;


};
#endif // MAINWINDOW_H
