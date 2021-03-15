/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionTemplate;
    QAction *actionTransaction;
    QWidget *centralwidget;
    QPushButton *pushButton;
    QLabel *label;
    QTreeWidget *treeWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuOpen;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(564, 515);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionTemplate = new QAction(MainWindow);
        actionTemplate->setObjectName(QString::fromUtf8("actionTemplate"));
        actionTransaction = new QAction(MainWindow);
        actionTransaction->setObjectName(QString::fromUtf8("actionTransaction"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(150, 40, 191, 23));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(380, 440, 61, 20));
        treeWidget = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(__qtreewidgetitem);
        __qtreewidgetitem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(20, 110, 361, 281));
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setGeometry(QRect(390, 110, 75, 23));
        removeButton = new QPushButton(centralwidget);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setGeometry(QRect(390, 140, 75, 23));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 564, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuOpen = new QMenu(menuFile);
        menuOpen->setObjectName(QString::fromUtf8("menuOpen"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(menuOpen->menuAction());
        menuOpen->addSeparator();
        menuOpen->addAction(actionTemplate);
        menuOpen->addAction(actionTransaction);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SortTransaction", nullptr));
        actionTemplate->setText(QApplication::translate("MainWindow", "Template", nullptr));
        actionTransaction->setText(QApplication::translate("MainWindow", "Transaction", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "Generate sorted transaction!", nullptr));
        label->setText(QApplication::translate("MainWindow", "Version", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "Keyword", nullptr));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Gruppe", nullptr));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "Lebenskosten", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(1, QApplication::translate("MainWindow", "Edeka", nullptr));
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "Einkaufen", nullptr));
        treeWidget->setSortingEnabled(__sortingEnabled);

        addButton->setText(QApplication::translate("MainWindow", "Add Item", nullptr));
        removeButton->setText(QApplication::translate("MainWindow", "Remove Item", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuOpen->setTitle(QApplication::translate("MainWindow", "Open", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
