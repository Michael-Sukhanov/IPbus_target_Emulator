#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <emulator.h>
#include <QShortcut>
#include <QtGui>
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

    void clear_restrictions();

    void save_values();

    void load_values();

private:
    Ui::MainWindow *ui;
    QUdpSocket* socket;
    QByteArray buffer;
    quint16 senderPort;
    QHostAddress senderAdress;
    Emulator obj;
    QShortcut *info       = new QShortcut(Qt::CTRL + Qt::Key_I, this);
    QShortcut *clear      = new QShortcut(Qt::CTRL + Qt::Key_E, this);
    QShortcut *exit       = new QShortcut(Qt::CTRL + Qt::Key_Q, this);
    QShortcut *bind       = new QShortcut(Qt::CTRL + Qt::Key_B, this);
    QShortcut *w_log      = new QShortcut(Qt::CTRL + Qt::Key_W, this);
    QShortcut *toolbar_on = new QShortcut(Qt::CTRL + Qt::Key_T, this);

    QToolBar *toolBar;

//    QMediaPlayer* notif;


};
#endif // MAINWINDOW_H
