/********************************************************************************
** Form generated from reading UI file 'gobangwithui.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOBANGWITHUI_H
#define UI_GOBANGWITHUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GobangWithUIClass
{
public:
    QAction *action_3;
    QAction *action_4;
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu;

    void setupUi(QMainWindow *GobangWithUIClass)
    {
        if (GobangWithUIClass->objectName().isEmpty())
            GobangWithUIClass->setObjectName(QStringLiteral("GobangWithUIClass"));
        GobangWithUIClass->resize(600, 400);
        action_3 = new QAction(GobangWithUIClass);
        action_3->setObjectName(QStringLiteral("action_3"));
        action_4 = new QAction(GobangWithUIClass);
        action_4->setObjectName(QStringLiteral("action_4"));
        centralWidget = new QWidget(GobangWithUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GobangWithUIClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(GobangWithUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GobangWithUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GobangWithUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GobangWithUIClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(GobangWithUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        GobangWithUIClass->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(action_3);
        menu->addAction(action_4);

        retranslateUi(GobangWithUIClass);

        QMetaObject::connectSlotsByName(GobangWithUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *GobangWithUIClass)
    {
        GobangWithUIClass->setWindowTitle(QApplication::translate("GobangWithUIClass", "GobangWithUI", 0));
        action_3->setText(QApplication::translate("GobangWithUIClass", "\345\205\210\346\211\213", 0));
        action_4->setText(QApplication::translate("GobangWithUIClass", "\345\220\216\346\211\213", 0));
        menu->setTitle(QApplication::translate("GobangWithUIClass", "\351\200\211\351\241\271", 0));
    } // retranslateUi

};

namespace Ui {
    class GobangWithUIClass: public Ui_GobangWithUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOBANGWITHUI_H
