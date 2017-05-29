#include "negamax.h"
#include <QtCore/qeventloop.h>
#include <QtWidgets/QApplication>

std::map<std::pair<int, int>, int> Negamax_search::root_search(Board& b, int depth, std::vector<std::pair<int, int>> moves)
{
	async_queue<int> as_queue(8);
	for (const auto& move : moves)
	{
		b.play(move.first, move.second, Board::computer);
		as_queue.produce(
			[=]()mutable {return -negamax(b, -Infinity, Infinity, depth, moves, Board::player); });
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

int Negamax_search::negamax(Board& b, int alpha, int beta, int depth, std::vector<std::pair<int, int>>& moves, Board::State s)
{
	auto zobrist_value = b.get_zobrist_value();
	auto& hash_tag = trans_map[zobrist_value];
	auto val = hash_tag.get_score(depth, alpha, beta, zobrist_value);
	if (val.has_value())
	{
		return val.value();
	}
	auto flag = Hash_tag::Hash_flag::Alpha;
	if (depth == 0 || b.get_winner() != Board::State::Empty)
	{
		auto score = b.evaluate();
		hash_tag.set(depth, s == Board::computer ? score : -score, Hash_tag::Hash_flag::Exact, zobrist_value);
		return s == Board::computer ? score : -score;
	}
	for (const auto& move : moves)
	{
		if (b.get_state(move.first, move.second) == Board::State::Empty)
		{
			b.play(move.first, move.second, s);
			auto score = -negamax(b, -beta, -alpha, depth - 1,
				moves, s == Board::computer ? Board::player : Board::computer);
			b.erase(move.first, move.second);
			if (score > alpha)
			{
				if (score >= beta)
				{
					hash_tag.set(depth, beta, Hash_tag::Hash_flag::Beta, zobrist_value);
					return beta;
				}
				flag = Hash_tag::Hash_flag::Exact;
				alpha = score;
			}
		}
	}
	hash_tag.set(depth, alpha, flag, zobrist_value);
	return alpha;
}

std::pair<int, int> Negamax_search::iterative_deepening_search(Board& b, int difficulty)
{
	auto moves = b.possible_moves();
	trans_map.reset();
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
	for (int depth = 0; depth < difficulty; depth += 2)
	{
		trans_map.reset();
		auto evaluation_map = root_search(b, depth, moves);
		std::sort(moves.begin(), moves.end(),
			[&](auto& lhs, auto& rhs) { return evaluation_map[lhs] >= evaluation_map[rhs]; });
	}
	return moves.front();
}
