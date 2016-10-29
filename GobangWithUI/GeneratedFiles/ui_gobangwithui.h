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
    QAction *play_black;
    QAction *play_white;
    QAction *AI_opponent;
    QAction *human_oppenent;
    QAction *clear_board;
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;

    void setupUi(QMainWindow *GobangWithUIClass)
    {
        if (GobangWithUIClass->objectName().isEmpty())
            GobangWithUIClass->setObjectName(QStringLiteral("GobangWithUIClass"));
        GobangWithUIClass->resize(600, 400);
        play_black = new QAction(GobangWithUIClass);
        play_black->setObjectName(QStringLiteral("play_black"));
        play_white = new QAction(GobangWithUIClass);
        play_white->setObjectName(QStringLiteral("play_white"));
        AI_opponent = new QAction(GobangWithUIClass);
        AI_opponent->setObjectName(QStringLiteral("AI_opponent"));
        human_oppenent = new QAction(GobangWithUIClass);
        human_oppenent->setObjectName(QStringLiteral("human_oppenent"));
        clear_board = new QAction(GobangWithUIClass);
        clear_board->setObjectName(QStringLiteral("clear_board"));
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
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        GobangWithUIClass->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(play_black);
        menu->addAction(play_white);
        menu->addAction(clear_board);
        menu_2->addSeparator();
        menu_2->addAction(AI_opponent);
        menu_2->addAction(human_oppenent);

        retranslateUi(GobangWithUIClass);

        QMetaObject::connectSlotsByName(GobangWithUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *GobangWithUIClass)
    {
        GobangWithUIClass->setWindowTitle(QApplication::translate("GobangWithUIClass", "GobangWithUI", 0));
        play_black->setText(QApplication::translate("GobangWithUIClass", "\345\205\210\346\211\213", 0));
        play_white->setText(QApplication::translate("GobangWithUIClass", "\345\220\216\346\211\213", 0));
        AI_opponent->setText(QApplication::translate("GobangWithUIClass", "\347\224\265\350\204\221", 0));
        human_oppenent->setText(QApplication::translate("GobangWithUIClass", "\347\216\251\345\256\266", 0));
        clear_board->setText(QApplication::translate("GobangWithUIClass", "\346\270\205\347\251\272\346\243\213\345\261\200", 0));
        menu->setTitle(QApplication::translate("GobangWithUIClass", "\351\200\211\351\241\271", 0));
        menu_2->setTitle(QApplication::translate("GobangWithUIClass", "\345\257\271\346\211\213", 0));
    } // retranslateUi

};

namespace Ui {
    class GobangWithUIClass: public Ui_GobangWithUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOBANGWITHUI_H
