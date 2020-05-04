#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion());
    socket = new QUdpSocket(this);
    ui->pushButton_2->setIcon(QIcon(":/new/prefix1/Clear.png"));
    ui->pushButton->hide();
    if(socket->bind(QHostAddress::AnyIPv4, 50001)){
        ui->statusbar->showMessage("Bind succesfull");
    }
    else{
        ui->statusbar->showMessage("Bind failed");
        ui->pushButton->show();
    }
    connect(socket, &QUdpSocket::readyRead, [=](){
        if(socket->hasPendingDatagrams()){
            obj.requestSize = quint16(socket->pendingDatagramSize());
            socket->readDatagram(obj.Prequest, obj.requestSize, &senderAdress, &senderPort);
            obj.Prepare_response();
            ui->statusbar->showMessage("Connection with " + senderAdress.toString() + ':' + QString::number(senderPort, 10));
            if(obj.ready())
                socket->writeDatagram( obj.get_response(), obj.responseSize, senderAdress, senderPort);
            obj.Clear_response();
            ui->textEdit_log->append(obj.GetLogMessage());
        }
    });
    ui->label->setFont(QFont("Consolas", 8));
    ui->label_5->setFont(QFont("Consolas", 8));
    ui->pushButton->setFont(QFont("Consolas", 8));
    ui->textEdit_log->setFont(QFont("Consolas", 8));
    ui->Adresses_textEdit->setFont(QFont("Consolas", 8));

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_pushButton_clicked(){
    if(socket->bind(QHostAddress::AnyIPv4, 50001))
        ui->pushButton->hide();
}


void MainWindow::on_AdresslineEdit_editingFinished(){
    bool ok;
    quint16 actual_adress = ui->AdresslineEdit->text().toUInt(&ok, 16);
    ui->Adresses_textEdit->append('[' + obj.Hex(actual_adress)  + ']' + " = " + obj.Hex(*(obj.Get_info() + actual_adress)));
}

void MainWindow::on_pushButton_2_clicked(){
    ui->Adresses_textEdit->clear();
}
