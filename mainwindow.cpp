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
            if(obj.GetLogMessage(ui->checkBox->isChecked()) != "")
                ui->textEdit_log->append(obj.GetLogMessage(ui->checkBox->isChecked()));
        }
    });
    ui->label->setFont(QFont("Consolas", 8));

    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Load board configuration from...", this, SLOT(load()), QKeySequence::Open);

    info ->setKey(Qt::CTRL + Qt::Key_I);
    clear->setKey(Qt::CTRL + Qt::Key_E);
    exit ->setKey(Qt::CTRL + Qt::Key_Q);
    bind ->setKey(Qt::CTRL + Qt::Key_B);
    w_log->setKey(Qt::CTRL + Qt::Key_W);

    connect(info,  &QShortcut::activated, this, [=](){on_pushButton_info_clicked();});
    connect(clear, &QShortcut::activated, this, [=](){on_pushButton_2_clicked();});
    connect(exit,  &QShortcut::activated, this, [=](){QCoreApplication::exit();});
    connect(bind,  &QShortcut::activated, this, [=](){on_pushButton_clicked();});
    connect(w_log, &QShortcut::activated, this, [=](){ui->checkBox->setChecked(!ui->checkBox->isChecked());});

    ui->AdresslineEdit->installEventFilter(this);

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_pushButton_clicked(){
    if(socket->bind(QHostAddress::AnyIPv4, 50001)){
        ui->pushButton->hide();
        ui->statusbar->showMessage("Bind succesfull");
    }
}


void MainWindow::on_AdresslineEdit_editingFinished(){
    bool ok;
    quint16 actual_adress = ui->AdresslineEdit->text().toUInt(&ok, 16);
    ui->Adresses_textEdit->append('[' + obj.Hex(actual_adress).remove(2,4)  + ']' + " = " + obj.Hex(*(obj.Get_info() + actual_adress)));
}

void MainWindow::on_pushButton_2_clicked(){
    ui->Adresses_textEdit->clear();
}

void MainWindow::load() {
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted || !obj.set_board(dialog.selectedFiles().first()))
        statusBar()->showMessage("File not loaded");
    else{
        statusBar()->showMessage("File loaded", 2000);
    }
}


void MainWindow::on_pushButton_info_clicked(){
    bool ok;
    ui->statusbar->showMessage("[0x" + ui->AdresslineEdit->text() + "]: register" + obj.board_info((ui->AdresslineEdit->text()).toUInt(&ok, 16)));
}

bool MainWindow::eventFilter(QObject * obj, QEvent* event){
    bool ok;
    quint16 address = (ui->AdresslineEdit->text()).toUInt(&ok,16);
    if (obj == ui->AdresslineEdit){
        if (event->type() == QEvent::KeyPress){
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up){
                 ui->AdresslineEdit->setText(Emulator::Hex(address + 1).remove(0,6));
                 return true;
            }
            else if(keyEvent->key() == Qt::Key_Down){
                ui->AdresslineEdit->setText(Emulator::Hex(address - 1).remove(0,6));
                return true;
            }
        }
        return false;
    }
    return QMainWindow::eventFilter(obj, event);
}


