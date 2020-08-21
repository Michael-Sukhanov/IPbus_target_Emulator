#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//     qDebug()<<"Error1";
    setWindowTitle(QCoreApplication::applicationName() + " v" + QCoreApplication::applicationVersion());
    socket = new QUdpSocket(this);
//    ui->pushButton_2->setIcon(QIcon(":/new/prefix1/Clear.png"));
    ui->pushButton->hide();
    if(socket->bind(QHostAddress::AnyIPv4, 50001)){
        ui->statusbar->showMessage("Bind succesfull");
//        qDebug()<<"Bind succesfull";
    }
    else{
        ui->statusbar->showMessage("Bind failed");
        ui->pushButton->show();
//        qDebug()<<"Bind failed";
    }
    connect(socket, &QUdpSocket::readyRead, [=](){
        if(socket->hasPendingDatagrams()){
//            qDebug()<<"Got a packet";
            obj.requestSize = quint16(socket->pendingDatagramSize());
            socket->readDatagram(obj.Prequest, obj.requestSize, &senderAdress, &senderPort);
//            qDebug()<<"Processing packet";
            obj.Prepare_response();
            ui->statusbar->showMessage("Connection with " + senderAdress.toString() + ':' + QString::number(senderPort, 10));
//            qDebug()<<"Sending datagram";
            if(obj.ready())
                socket->writeDatagram( obj.get_response(), obj.responseSize, senderAdress, senderPort);
//            qDebug()<<"Clearing response";
            obj.Clear_response();
            if(obj.GetLogMessage(ui->checkBox->isChecked()) != "")
                ui->textEdit_log->append(obj.GetLogMessage(ui->checkBox->isChecked()));
        }
    });
//    qDebug()<<"Error";
    ui->label->setFont(QFont("Consolas", 8));


    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *ConfigMenu = menuBar()->addMenu("&Config");
    ConfigMenu->addAction(QIcon(":/images/Configs.png"),"&Load board configuration from...", this, SLOT(load()), QKeySequence::Open);
    fileMenu->addAction(QIcon(":/images/Save.png"),"Save state", this, SLOT(save_values()), Qt::CTRL + Qt::Key_S);
    fileMenu->addAction(QIcon(":/images/Load.png"),"Load state", this, SLOT(load_values()), Qt::CTRL + Qt::Key_L);
    ConfigMenu->addAction(QIcon(":/images/Remove restrictions.png"),"&Clear restrictions", this, SLOT(clear_restrictions()), Qt::CTRL + Qt::Key_Q + Qt::SHIFT);

    toolBar = new QToolBar("Tool bar", this);
    toolBar->addAction(QIcon(":/images/Save.png"),"Save state", this, SLOT(save_values()));
    toolBar->addAction(QIcon(":/images/Load.png"),"Load state", this, SLOT(load_values()));
    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/images/Configs.png"),"&Load board configuration from...", this, SLOT(load()));
    toolBar->addAction(QIcon(":/images/Remove restrictions.png"),"&Clear restrictions", this, SLOT(clear_restrictions()));


    addToolBar(Qt::TopToolBarArea, toolBar);


    connect(info,  &QShortcut::activated, this, [=](){on_pushButton_info_clicked();});
    connect(clear, &QShortcut::activated, this, [=](){on_pushButton_2_clicked();});
    connect(exit,  &QShortcut::activated, this, [=](){QCoreApplication::exit();});
    connect(bind,  &QShortcut::activated, this, [=](){on_pushButton_clicked();});
    connect(w_log, &QShortcut::activated, this, [=](){ui->checkBox->setChecked(!ui->checkBox->isChecked());});
    connect(toolbar_on,  &QShortcut::activated, this, [=](){if(menuBar()->isHidden()){
           menuBar()->show();
           toolBar->hide();
        }else{
           menuBar()->hide();
           toolBar->show();
     }});
    connect(&obj, &Emulator::Message, this, [=](QString Message){
        QMessageBox::warning(this, QCoreApplication::applicationName(),
                                        Message);

    });

    ui->AdresslineEdit->installEventFilter(this);
    toolBar->hide();

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
    ui->Adresses_textEdit->append("[0x" + obj.Hex(actual_adress).right(4)  + ']' + " = " + obj.Hex(obj.Get_info()[actual_adress]));
}

void MainWindow::on_pushButton_2_clicked(){
    ui->Adresses_textEdit->clear();
}

void MainWindow::load() {
//    qDebug()<<"Loading file";
    obj.remove_board();
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted || !obj.set_board(dialog.selectedFiles().first()))
        statusBar()->showMessage("File not loaded");
    else{
        statusBar()->showMessage("Configurations loaded " + QFileInfo(dialog.selectedFiles().first()).filePath(), 2000);
    }
}

void MainWindow::clear_restrictions(){
    obj.remove_board();
    ui->statusbar->showMessage("Restrictions removed", 2000);
    }


void MainWindow::on_pushButton_info_clicked(){
    bool ok;
    ui->statusbar->showMessage("[0x" + ui->AdresslineEdit->text() + "]: register" + obj.board_info((ui->AdresslineEdit->text()).toUInt(&ok, 16)));
}

bool MainWindow::eventFilter(QObject * obj, QEvent* event){
    bool ok;
    quint16 address = (ui->AdresslineEdit->text()).toUtf8().toUInt(&ok, 16);
    if (obj == ui->AdresslineEdit){
        if (event->type() == QEvent::KeyPress){
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Up){
                 ui->AdresslineEdit->setText(Emulator::Hex(address + 1).right(4));
                 return true;
            }
            else if(keyEvent->key() == Qt::Key_Down){
                ui->AdresslineEdit->setText(Emulator::Hex(address - 1).right(4));
                return true;
            }
        }
        return false;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::save_values(){
    QString saveFilename = QFileDialog::getSaveFileName(this,
             "Save state", "Saves/" + QDate::currentDate().toString("yyyy-MM-dd") + " state",
             "Text files(*.txt  *.dat)");
    if (saveFilename.isEmpty())
             return;
    else{
        QFile file(saveFilename);
        if(!file.open(QIODevice::WriteOnly)) {
            ui->statusbar->showMessage("Unable to open file", 2000);
            return;
         }else{
            QTextStream out(&file);
            for (quint32 address = 0; address < 0x10000; ++ address){
                if(obj.Get_info()[address] != 0x0)
                    out << Emulator::Hex(address) +'\t' << Emulator::Hex(obj.Get_info()[address]) + '\n';
            ui->statusbar->showMessage("State saved successfully to " + QFileInfo(file).filePath(), 2000);
            }
            file.close();
        }
    }
}

void MainWindow::load_values(){
    QString loadFilename = QFileDialog::getOpenFileName(this, "Load state", "",
                                                        "Text files(*.txt  *.dat)");
    if(loadFilename.isEmpty())
            return;
    else{
        QFile file(loadFilename);
        if(!file.open(QIODevice::ReadOnly)){
            ui->statusbar->showMessage("Unable to open file", 2000);
        }else{
            QTextStream in(&file);
            if(!obj.load_state(&in))
                ui->statusbar->showMessage("ERROR: Invalid format", 2000);
            else
                ui->statusbar->showMessage("State loaded successfully from " + QFileInfo(file).filePath(), 2000);
        }
        file.close();
    }

}


