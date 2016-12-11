#pragma once
#include "Trans_map.h"
#include "Board.h"
#include <map>

class Minimax_search
{
public:
	Minimax_search();
	~Minimax_search();
	static std::pair<int,int> minimax_search(Board& b)
	{
		return search_deeper_and_deeper(b);
	}
private:
	static const int Infinity = 1000000;
	static Trans_map trans_map;
	static std::pair<int, int> search_deeper_and_deeper(Board b);
	static std::map<std::pair<int, int>, int> root_search(Board b, int depth, std::vector<std::pair<int, int>> moves);
	static int min(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves);
	static int max(Board b, int alpha, int beta, int depth, std::vector<std::pair<int, int>> moves);
	static std::vector<std::pair<int, int>> update_moves(const Board &b,
		const std::vector<std::pair<int, int>> &moves, const std::pair<int, int> &pos);
};

