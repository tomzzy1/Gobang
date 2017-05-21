#pragma once
#include "Trans_map.h"
#include "Board.h"
#include "async_queue.h"
#include <map>
#include <atomic>

class Negamax_search
{
public:
	Negamax_search() :Infinity(1000000), trans_map(48) {};
	~Negamax_search() = default;
	std::pair<int, int> search(Board& b, int difficulty)
	{
		return iterative_deepening_search(b, difficulty);
	}
private:
	const int Infinity;
	Trans_map trans_map;
	int negamax(Board& b, int alpha, int beta, int depth, std::vector<std::pair<int, int>>& moves, Board::State s);
	std::pair<int, int> iterative_deepening_search(Board& b, int difficulty);
	std::map<std::pair<int, int>, int> root_search(Board& b, int depth, std::vector<std::pair<int, int>> moves);
};

