/********************************************************************************
** Form generated from reading UI file 'GUI.ui'
**
** Created by: Qt User Interface Compiler version 6.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GUIClass
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *textEdit;
    QPushButton *sendButton;

    void setupUi(QWidget *GUIClass)
    {
        if (GUIClass->objectName().isEmpty())
            GUIClass->setObjectName("GUIClass");
        GUIClass->resize(600, 400);
        widget = new QWidget(GUIClass);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 10, 571, 371));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(widget);
        listWidget->setObjectName("listWidget");

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        textEdit = new QLineEdit(widget);
        textEdit->setObjectName("textEdit");

        horizontalLayout->addWidget(textEdit);

        sendButton = new QPushButton(widget);
        sendButton->setObjectName("sendButton");

        horizontalLayout->addWidget(sendButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(GUIClass);

        QMetaObject::connectSlotsByName(GUIClass);
    } // setupUi

    void retranslateUi(QWidget *GUIClass)
    {
        GUIClass->setWindowTitle(QCoreApplication::translate("GUIClass", "GUI", nullptr));
        sendButton->setText(QCoreApplication::translate("GUIClass", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GUIClass: public Ui_GUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
