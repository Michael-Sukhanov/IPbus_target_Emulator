/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTextEdit *textEdit_log;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QCheckBox *checkBox;
    QPushButton *pushButton;
    QTextEdit *Adresses_textEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QLineEdit *AdresslineEdit;
    QPushButton *pushButton_info;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(758, 434);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textEdit_log = new QTextEdit(centralwidget);
        textEdit_log->setObjectName(QString::fromUtf8("textEdit_log"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit_log->sizePolicy().hasHeightForWidth());
        textEdit_log->setSizePolicy(sizePolicy);
        textEdit_log->setMinimumSize(QSize(400, 180));
        textEdit_log->setReadOnly(true);

        gridLayout->addWidget(textEdit_log, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Droid Sans"));
        font.setPointSize(8);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8(""));
        label->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        label->setTextFormat(Qt::PlainText);

        horizontalLayout_2->addWidget(label);

        checkBox = new QCheckBox(centralwidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setEnabled(true);
        checkBox->setLayoutDirection(Qt::RightToLeft);
        checkBox->setChecked(true);
        checkBox->setTristate(false);

        horizontalLayout_2->addWidget(checkBox);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        Adresses_textEdit = new QTextEdit(centralwidget);
        Adresses_textEdit->setObjectName(QString::fromUtf8("Adresses_textEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Adresses_textEdit->sizePolicy().hasHeightForWidth());
        Adresses_textEdit->setSizePolicy(sizePolicy1);
        Adresses_textEdit->setMinimumSize(QSize(200, 180));
        Adresses_textEdit->setMaximumSize(QSize(200, 16777215));
        Adresses_textEdit->setReadOnly(true);

        gridLayout->addWidget(Adresses_textEdit, 2, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, -1, 0, -1);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(130, 16777215));
        label_5->setStyleSheet(QString::fromUtf8(""));
        label_5->setTextFormat(Qt::PlainText);

        horizontalLayout->addWidget(label_5);

        AdresslineEdit = new QLineEdit(centralwidget);
        AdresslineEdit->setObjectName(QString::fromUtf8("AdresslineEdit"));
        AdresslineEdit->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(AdresslineEdit->sizePolicy().hasHeightForWidth());
        AdresslineEdit->setSizePolicy(sizePolicy2);
        AdresslineEdit->setMinimumSize(QSize(30, 20));
        AdresslineEdit->setMaximumSize(QSize(35, 20));
        AdresslineEdit->setStyleSheet(QString::fromUtf8("font: 8pt \"Consolas\";"));
        AdresslineEdit->setFrame(true);
        AdresslineEdit->setEchoMode(QLineEdit::Normal);
        AdresslineEdit->setAlignment(Qt::AlignCenter);
        AdresslineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);
        AdresslineEdit->setClearButtonEnabled(false);

        horizontalLayout->addWidget(AdresslineEdit);

        pushButton_info = new QPushButton(centralwidget);
        pushButton_info->setObjectName(QString::fromUtf8("pushButton_info"));
        pushButton_info->setMinimumSize(QSize(20, 20));
        pushButton_info->setMaximumSize(QSize(20, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/info.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_info->setIcon(icon);

        horizontalLayout->addWidget(pushButton_info);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(20, 20));
        pushButton_2->setMaximumSize(QSize(20, 20));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/Clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon1);

        horizontalLayout->addWidget(pushButton_2);


        gridLayout->addLayout(horizontalLayout, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 758, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        textEdit_log->setStyleSheet(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Current processes:", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "Write transactions only", nullptr));
#if QT_CONFIG(whatsthis)
        pushButton->setWhatsThis(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Start to listen to the 500001 port</p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushButton->setText(QCoreApplication::translate("MainWindow", "Bind", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Register's content", nullptr));
        AdresslineEdit->setInputMask(QCoreApplication::translate("MainWindow", ">HHHH", nullptr));
        AdresslineEdit->setText(QCoreApplication::translate("MainWindow", "0000", nullptr));
        pushButton_info->setText(QString());
        pushButton_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
