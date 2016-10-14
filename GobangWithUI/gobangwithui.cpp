#include "gobangwithui.h"
#include <chrono>

int GobangWithUI::depth_;

Trans_map GobangWithUI::trans_map{ 48 };

GobangWithUI::GobangWithUI(QWidget *parent)
	: QMainWindow(parent), num_count(0), black(Board::State::Computer), white(Board::State::Player)
{
	setMaximumHeight(640);
	setMinimumHeight(640);
	setMaximumSize(640, 640);
	setMinimumSize(640, 640);
	ui.setupUi(this);
	connect(ui.action_3, SIGNAL(triggered()), this, SLOT(set_player_black()));
	connect(ui.action_4, SIGNAL(triggered()), this, SLOT(set_computer_black()));
	default_first_move();
}

GobangWithUI::~GobangWithUI()
{

}

void GobangWithUI::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	QPen pen;
	pen.setColor(Qt::black);
	painter.setPen(pen);
	for (int i = 40; i <= 600; i += 40)
	{
		painter.drawLine(QPoint(40, i), QPoint(600, i));
		painter.drawLine(QPoint(i, 40), QPoint(i, 600));
	}
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	for (int i = 0; i < num_count; ++i)
	{
		if (i % 2 == 0)
		{
			brush.setColor(Qt::black);
		}
		else
		{
			brush.setColor(Qt::white);
		}
		painter.setBrush(brush);
		painter.drawEllipse(point_state[i].first - 20, point_state[i].second - 20, 40, 40);
	}
}

void GobangWithUI::set_computer_black()
{
	black = Board::State::Computer;
	white = Board::State::Player;
	clear();
	default_first_move();
}

void GobangWithUI::set_player_black()
{
	black = Board::State::Player;
	white = Board::State::Computer;
	clear();
	update();
}


void GobangWithUI::mousePressEvent(QMouseEvent* mouseEvent)
{
	int x_pos = static_cast<float>(mouseEvent->x() / 40.0) + 0.5;
	int y_pos = static_cast<float>(mouseEvent->y() / 40.0) + 0.5;
	if (x_pos > 0 && x_pos <= 15 && y_pos > 0 && y_pos <= 15 && board.get_state(x_pos - 1, y_pos - 1) == Board::State::Empty)
	{
		play(x_pos, y_pos);
		auto winner = board.get_winner();
		if (winner == Board::State::Player)
		{
			auto result = QMessageBox::question(
				this,
				"Game Over",
				"You win\n Do you want to start a new game?",
				QMessageBox::Yes | QMessageBox::No);
			if (result == QMessageBox::Yes)
			{
				clear();
				if (black == Board::State::Computer)
					default_first_move();
			}	
			else
				close();
		}
		else if (winner == Board::State::Empty)
		{
			if (num_count == 225)
			{
				tie();
			}
			else
			{
				auto move = search_deeper_and_deeper(board);
				char c = static_cast<char>(depth_ + '0');
				std::string s = std::string(1, c);
				QMessageBox::question(
					this,
					"...",
					s.c_str(),
					QMessageBox::Yes | QMessageBox::No);
				play(move.first + 1, move.second + 1);
				auto winner2 = board.get_winner();
				if (winner2 == Board::State::Computer)
				{
					auto result = QMessageBox::question(
						this,
						"Game Over",
						"You lose\n Do you want to start a new game?",
						QMessageBox::Yes | QMessageBox::No);
					if (result == QMessageBox::Yes)
					{
						clear();
						if (black == Board::State::Computer)
							default_first_move();
					}
					else
						close();
				}
				else if (num_count == 225)
				{
					tie();
				}
			}
		}
	}
}

void GobangWithUI::clear()
{
	board.clear();
	num_count = 0;
	update();
}

std::pair<int, int> GobangWithUI::search_deeper_and_deeper(Board b)
{
	using namespace std::chrono;
	auto moves = b.possible_moves();
	for (auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::Computer);
		if (b.get_winner() == Board::State::Computer)
			return move;
		b.erase(move.first, move.second);
	}
	for (auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::Player);
		if (b.get_winner() == Board::State::Player)
			return move;
		b.erase(move.first, move.second);
	}
	for (int depth = 0; ; ++depth)
	{
		auto start = system_clock::now();
		auto e_map = root_search(b, depth, moves);
		std::sort(moves.begin(), moves.end(),
			[&](std::pair<int, int>&lhs, std::pair<int, int>&rhs) {return e_map[lhs] >= e_map[rhs]; });
		auto now = system_clock::now();
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		if (duration_cast<milliseconds>(now - start) > milliseconds(2000))
		{
			depth_ = depth;
			break;
		}		
	}
	return moves.front();
}

void GobangWithUI::default_first_move()
{
	board.play(7, 7, Board::State::Computer);
	point_state[num_count].first = 320;
	point_state[num_count].second = 320;
	++num_count;
	update();
}

void GobangWithUI::play(int x_pos, int y_pos)
{
	if (num_count % 2 == 0)
	{
		board.play(x_pos - 1, y_pos - 1, black);
	}
	else
	{
		board.play(x_pos - 1, y_pos - 1, white);
	}
	point_state[num_count].first = x_pos * 40;
	point_state[num_count].second = y_pos * 40;
	++num_count;
	repaint();
}

void GobangWithUI::tie()
{
	int result = QMessageBox::question(this,
		"Game Over",
		"Tie\n Do you want to start a new game?",
		QMessageBox::Yes | QMessageBox::No);
	if (result == QMessageBox::Yes)
	{
		clear();
		if (black == Board::State::Computer)
			default_first_move();
	}
	else
		close();
}

std::map<std::pair<int, int>, int> GobangWithUI::root_search(Board b, int depth, std::vector<std::pair<int, int>> moves)
{
	async_queue<int> as_queue(10);
	//auto moves = b.possible_moves();
	for (const auto &move : moves)
	{
		if (b.get_state(move.first, move.second) == Board::State::Empty)
		{
			b.play(move.first, move.second, Board::State::Computer);
			//if (b.get_winner() == computer)
			//	return move;
			as_queue.produce(min, b, -Infinity, Infinity, depth, update_moves(b, moves, move));
			b.erase(move.first, move.second);
		}
	}
	auto results = as_queue.consume_all();
	std::map<std::pair<int, int>, int> moves_evaluation;
	for (int i = 0; i != moves.size(); ++i)
	{
		moves_evaluation[moves[i]] = results[i];
	}
	return moves_evaluation/*moves[std::max_element(results.begin(), results.end()) - results.begin()]*/;
}

int GobangWithUI::min(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves)
{
	auto flag = Hash_tag::Hash_flag::Alpha;
	auto zobrist_value = b.get_zobrist_value();
	auto hash_tag = trans_map[zobrist_value];
	auto val = hash_tag.get_value(depth, alpha, beta, zobrist_value);
	if (val != ValUnknown)
		return val;
	if (depth == 0)
	{
		auto result = b.evaluate();
		hash_tag.set(depth, result, Hash_tag::Hash_flag::Exact, zobrist_value);
		return result;
		//std::unique_lock<spinlock> l(lock);
		//auto pos = m.find(zobrist_value);
		//if (pos == m.end())
		//{
		//	auto result = b.evaluate();
		//	m[zobrist_value] = result;
		//	return result;
		//}
		//return pos->second;
		//return b.evaluate();
	}
	for (const auto &move : moves)
	{
		if (b.get_state(move.first, move.second) == Board::State::Empty)
		{
			b.play(move.first, move.second, Board::State::Player);
			if (b.get_winner() == Board::State::Player)
			{
				return -Infinity;
			}
			auto score = max(b, alpha, beta, depth - 1, update_moves(b, moves, move));
			b.erase(move.first, move.second);
			if (score < beta)
			{
				flag = Hash_tag::Hash_flag::Exact;
				beta = score;
			}
			if (alpha >= beta)
			{
				hash_tag.set(depth, alpha, Hash_tag::Hash_flag::Beta, zobrist_value);
				return beta;
			}
		}
	}
	hash_tag.set(depth, alpha, flag, zobrist_value);
	return beta;
}

int GobangWithUI::max(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves)
{
	auto flag = Hash_tag::Hash_flag::Alpha;
	auto zobrist_value = b.get_zobrist_value();
	auto hash_tag = trans_map[zobrist_value];
	auto val = hash_tag.get_value(depth, alpha, beta, zobrist_value);
	if (val != ValUnknown)
		return val;
	if (depth == 0 || b.get_winner() != Board::State::Empty)
	{
		auto result = b.evaluate();
		hash_tag.set(depth, result, Hash_tag::Hash_flag::Exact, zobrist_value);
		return result;
		//std::unique_lock<spinlock> l(lock);
		//auto pos = m.find(zobrist_value);
		//if (pos == m.end())
		//{
		//	auto result = b.evaluate();
		//	m[zobrist_value] = result;
		//	return result;
		//}
		//return pos->second;
		//return b.evaluate();
	}
	for (const auto &move : moves)
	{
		if (b.get_state(move.first, move.second) == Board::State::Empty)
		{
			b.play(move.first, move.second, Board::State::Computer);
			auto score = min(b, alpha, beta, depth - 1, update_moves(b, moves, move));
			b.erase(move.first, move.second);
			if (score > alpha)
			{
				flag = Hash_tag::Hash_flag::Exact;
				alpha = score;
			}
			if (alpha >= beta)
			{
				hash_tag.set(depth, alpha, Hash_tag::Hash_flag::Beta, zobrist_value);
				return alpha;
			}
		}
	}
	hash_tag.set(depth, alpha, flag, zobrist_value);
	return alpha;
}

std::vector<std::pair<int, int>> GobangWithUI::update_moves(const Board& b,
	const std::vector<std::pair<int, int>>& moves, const std::pair<int, int>& pos)
{
	std::vector<std::pair<int, int>> update; //possible new moves
	auto no_more_than_14 = [](int n) {return n > 14 ? 14 : n; }; //avoid out of bound
	auto no_less_than_0 = [](int n) {return n < 0 ? 0 : n; };	//avoid out of bound
	auto push_back_if_empty = [&](int x_pos, int y_pos) //push back if the position is "empty" and it's not in the "moves"
	{
		if (b.get_state(x_pos, y_pos) == Board::State::Empty)
		{
			if (std::binary_search(moves.begin(), moves.end(), std::pair<int, int>{x_pos, y_pos}))
			{
				update.push_back({ x_pos,y_pos });
			}
		}
	};
	auto &x_pos = pos.first;
	auto &y_pos = pos.second;
	for (int k = 1; k <= 2; ++k)
	{
		push_back_if_empty(x_pos, no_more_than_14(y_pos + k));
		push_back_if_empty(x_pos, no_less_than_0(y_pos - k));
		push_back_if_empty(no_more_than_14(x_pos + k), y_pos);
		push_back_if_empty(no_less_than_0(x_pos - k), y_pos);
		push_back_if_empty(no_more_than_14(x_pos + k), no_more_than_14(y_pos + k));
		push_back_if_empty(no_less_than_0(x_pos - k), no_more_than_14(y_pos + k));
		push_back_if_empty(no_more_than_14(x_pos + k), no_less_than_0(y_pos - k));
		push_back_if_empty(no_less_than_0(x_pos - k), no_less_than_0(y_pos - k));
	}	
	auto result(update);
	result.resize(update.size() + moves.size());
	std::copy(moves.begin(), moves.end(), result.begin() + update.size());//make new and old moves together
	result.erase(std::find(result.begin(), result.end(), pos));
	return result;
}




