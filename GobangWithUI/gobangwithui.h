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
#include <async_queue.h>
#include "GeneratedFiles/ui_gobangwithui.h"
#include "Trans_map.h"


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
	static std::pair<int, int> search_deeper_and_deeper(Board b);
	static std::map<std::pair<int, int>, int> root_search(Board b, int depth, std::vector<std::pair<int, int>> moves);
	static int min(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves);
	static int max(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves);
	static std::vector<std::pair<int, int>> update_moves(const Board &b,
		const std::vector<std::pair<int, int>> &moves, const std::pair<int, int> &pos);
	void default_first_move();
	void play(int x_pos, int y_pos);
	void tie();
private:
	static int depth_;
	static Trans_map trans_map;
	Ui::GobangWithUIClass ui;
	Board board;
	int num_count;
	Board::State black;
	Board::State white;
	std::array<std::pair<int, int>, 225> point_state;
public slots:
	void set_computer_black();
	void set_player_black();
};




#endif // GOBANGWITHUI_H
