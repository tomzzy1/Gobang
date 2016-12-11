#pragma once
#include "Trans_map.h"
#include "Board.h"
#include <map>
#include <atomic>

class Negamax_search
{
public:
	Negamax_search() :Infinity(1000000), trans_map(64) {};
	//Negamax_search() = default;
	~Negamax_search() = default;
	std::pair<int, int> search(Board& b)
	{
		return iterative_deepening_search(b);
	}
	std::atomic_int depth_ = 0;
private:
	const int Infinity;
	Trans_map trans_map;
	int negamax(Board& b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves, Board::State s);
	std::pair<int, int> iterative_deepening_search(Board& b);
	std::map<std::pair<int, int>, int> root_search(Board& b, int depth, std::vector<std::pair<int, int>> moves);
	std::vector<std::pair<int, int>> update_moves(const Board &b,
		const std::vector<std::pair<int, int>>& moves, const std::pair<int, int>& pos);
};

