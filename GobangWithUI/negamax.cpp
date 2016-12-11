#include "negamax.h"
#include "async_queue.h"
#include <QtCore/qeventloop.h>
#include <QtWidgets/QApplication>

std::map<std::pair<int, int>, int> Negamax_search::root_search(Board& b, int depth, std::vector<std::pair<int, int>> moves)
{
	async_queue<int> as_queue(10);
	for (const auto& move : moves)
	{
		b.play(move.first, move.second, Board::computer);
		as_queue.produce(
			[=]()mutable {return -negamax(b, -Infinity, Infinity, depth, update_moves(b, moves, move), Board::player); });
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		b.erase(move.first, move.second);
	}
	auto results = as_queue.consume_all();
	//std::vector<int> results;
	//for (auto& move : moves)
	//{
		//b.play(move.first, move.second, Board::computer);
		//results.push_back(negamax(b, -Infinity, Infinity, depth, update_moves(b, moves, move), Board::player));
		//b.erase(move.first, move.second);
	//}
	std::map<std::pair<int, int>, int> moves_evaluation;
	for (int i = 0; i != moves.size(); ++i)
	{
		moves_evaluation[moves[i]] = results[i];
	}
	return moves_evaluation;
}

int Negamax_search::negamax(Board& b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves, Board::State s)
{
	auto zobrist_value = b.get_zobrist_value();
	auto& hash_tag = trans_map[zobrist_value];
	auto val = hash_tag.get_score(depth, alpha, beta, zobrist_value);
	if (val != Val_unknown)
	{
		
		if (s == Board::computer)
			return val;
		else
			return -val;
	}
	auto flag = Hash_tag::Hash_flag::Alpha;
	if (depth == 0 || b.get_winner() != Board::State::Empty)
	{
		auto score = b.evaluate();++depth_;
		hash_tag.set(depth, score, Hash_tag::Hash_flag::Exact, zobrist_value);
		if (s == Board::computer)
			return -score;
		else
			return score;
	}
	for (const auto& move : moves)
	{
		b.play(move.first, move.second, s);
		auto score = -negamax(b, -beta, -alpha, depth - 1,
			update_moves(b, moves, move), s == Board::computer ? Board::player : Board::computer);
		b.erase(move.first, move.second);
		if (score > alpha)
		{
			if (score >= beta)
			{
				if (s == Board::computer)
					hash_tag.set(depth, beta, Hash_tag::Hash_flag::Beta, zobrist_value);
				else
					hash_tag.set(depth, -beta, Hash_tag::Hash_flag::Beta, zobrist_value);
				return beta;
			}
			flag = Hash_tag::Hash_flag::Exact;
			alpha = score;
		}
	}
	if (s == Board::computer)
		hash_tag.set(depth, alpha, flag, zobrist_value);
	else
		hash_tag.set(depth, -alpha, flag, zobrist_value);
	return alpha;
}

std::vector<std::pair<int, int>> Negamax_search::update_moves(const Board& b,
	const std::vector<std::pair<int, int>>& moves, const std::pair<int, int>& pos)
{
	std::vector<std::pair<int, int>> update; //possible new moves
	auto no_more_than_14 = [](int n) {return n > 14 ? 14 : n; }; //avoid out of bound
	auto no_less_than_0 = [](int n) {return n < 0 ? 0 : n; };	//avoid out of bound
	auto push_back_if_empty = [&](int x_pos, int y_pos) //push back if the position is "empty" and it's not in the "moves"
	{
		if (b.get_state(x_pos, y_pos) == Board::State::Empty)
		{
			if (std::find(moves.begin(), moves.end(), std::make_pair(x_pos, y_pos)) == moves.end())
			{
				update.push_back({ x_pos,y_pos });
			}
		}
	};
	auto& x_pos = pos.first;
	auto& y_pos = pos.second;
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
	for (auto& move : moves)
		update.emplace_back(move);
	update.erase(std::find(update.begin(), update.end(), pos));
	return update;
}

std::pair<int, int> Negamax_search::iterative_deepening_search(Board& b)
{
	using namespace std::chrono;
	auto moves = b.possible_moves();
	//trans_map.reset();
	for (auto& move : moves)
	{
		b.play(move.first, move.second, Board::computer);
		if (b.get_winner() == Board::computer)
			return move;
		b.erase(move.first, move.second);
	}
	for (auto& move : moves)
	{
		b.play(move.first, move.second, Board::player);
		if (b.get_winner() == Board::player)
			return move;
		b.erase(move.first, move.second);
	}
	int depth = 0;
	//depth_ = 0; 
	//auto now = system_clock::now();
	for (; depth < 5; ++depth)
	{
	//	auto start = system_clock::now();
		auto evaluation_map = root_search(b, depth, moves);
		std::sort(moves.begin(), moves.end(),
			[&](const std::pair<int, int>& lhs,const std::pair<int, int>& rhs) { return evaluation_map[lhs] >= evaluation_map[rhs]; });
		//if (duration_cast<milliseconds>(now - start) > milliseconds(10))
			//break;
		//depth_ = evaluation_map[moves.front()];
	}
	//depth_ = depth;
	return moves.front();

}
