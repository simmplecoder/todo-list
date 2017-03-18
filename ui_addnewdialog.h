/********************************************************************************
** Form generated from reading UI file 'addnewdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDNEWDIALOG_H
#define UI_ADDNEWDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AddNewDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *AddNewDialog)
    {
        if (AddNewDialog->objectName().isEmpty())
            AddNewDialog->setObjectName(QStringLiteral("AddNewDialog"));
        AddNewDialog->resize(426, 158);
        AddNewDialog->setModal(true);
        verticalLayout = new QVBoxLayout(AddNewDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lineEdit = new QLineEdit(AddNewDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMaxLength(255);

        verticalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(AddNewDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton, 0, Qt::AlignRight);


        retranslateUi(AddNewDialog);

        QMetaObject::connectSlotsByName(AddNewDialog);
    } // setupUi

    void retranslateUi(QDialog *AddNewDialog)
    {
        AddNewDialog->setWindowTitle(QApplication::translate("AddNewDialog", "Add new todo", Q_NULLPTR));
        lineEdit->setPlaceholderText(QApplication::translate("AddNewDialog", "todo description", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AddNewDialog", "Add todo", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AddNewDialog: public Ui_AddNewDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDNEWDIALOG_H
