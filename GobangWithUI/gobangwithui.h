#ifndef GOBANGWITHUI_H
#define GOBANGWITHUI_H

#include <QtWidgets/QMainWindow>
#include <qaction.h>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <array>
#include "Board.h"
#include "GeneratedFiles/ui_gobangwithui.h"

class GobangWithUI : public QMainWindow
{
	Q_OBJECT

public:
	static const int Infinity = 1000000;
	GobangWithUI(QWidget *parent = 0);
	~GobangWithUI();
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	void clear();
	void computer_win();
	void player_win();
	void player_black_win();
	void player_white_win();
	void default_first_move();
	void play(int x_pos, int y_pos);
	void tie();
private:
	Ui::GobangWithUIClass ui;
	Board board;
	int num_count;
	std::array<std::pair<int, int>, 225> point_state;
public slots:
	void set_black_computer();
	void set_black_player();
	void set_opponent_AI();
	void set_opponent_player();
	void clear_board();
};




#endif // GOBANGWITHUI_H
