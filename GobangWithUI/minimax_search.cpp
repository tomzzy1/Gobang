#include "minimax_search.h"
#include "async_queue.h"
#include <QtCore/qeventloop.h>
#include <QtWidgets/QApplication>

Trans_map Minimax_search::trans_map{ 48 };

Minimax_search::Minimax_search()
{
}


Minimax_search::~Minimax_search()
{
}

std::map<std::pair<int, int>, int> Minimax_search::root_search(Board b, int depth, std::vector<std::pair<int, int>> moves)
{
	async_queue<int> as_queue(1);
	for (const auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::Black);
		as_queue.produce(min, b, -Infinity, Infinity, depth, update_moves(b, moves, move));
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		b.erase(move.first, move.second);
	}
	auto results = as_queue.consume_all();
	std::map<std::pair<int, int>, int> moves_evaluation;
	for (int i = 0; i != moves.size(); ++i)
	{
		moves_evaluation[moves[i]] = results[i];
	}
	return moves_evaluation;
}

int Minimax_search::min(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves)
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
	}
	for (const auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::White);
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
	hash_tag.set(depth, alpha, flag, zobrist_value);
	return beta;
}

int Minimax_search::max(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves)
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
	}
	for (const auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::Black);
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
	hash_tag.set(depth, alpha, flag, zobrist_value);
	return alpha;
}

std::vector<std::pair<int, int>> Minimax_search::update_moves(const Board& b,
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
	std::vector<std::pair<int, int>> result(update.size() + moves.size());
	std::copy(update.begin(), update.end(), result.begin());
	std::copy(moves.begin(), moves.end(), result.begin() + update.size());//make new and old moves together
	result.erase(std::find(result.begin(), result.end(), pos));
	return result;
}

std::pair<int, int> Minimax_search::search_deeper_and_deeper(Board b)
{
	using namespace std::chrono;
	auto moves = b.possible_moves();
	for (auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::Black);
		if (b.get_winner() == Board::State::Black)
			return move;
		b.erase(move.first, move.second);
	}
	for (auto &move : moves)
	{
		b.play(move.first, move.second, Board::State::White);
		if (b.get_winner() == Board::State::White)
			return move;
		b.erase(move.first, move.second);
	}
	for (int depth = 0; ; ++depth)
	{
		auto start = system_clock::now();
		auto evaluation_map = root_search(b, depth, moves);
		std::sort(moves.begin(), moves.end(),
			[&](std::pair<int, int>&lhs, std::pair<int, int>&rhs) {return evaluation_map[lhs] >= evaluation_map[rhs]; });
		auto now = system_clock::now();
		if (duration_cast<milliseconds>(now - start) > milliseconds(2000))
			break;
	}
	return moves.front();
}