#pragma once
#include <vector>
#include <array>

class Board
{
public:

	enum class State { Black, White, Empty };
	static State computer;
	static State player;

	Board() :zobrist_value(0)
	{
		for (auto& row : points)
			for (auto& point : row)
			{
				point = State::Empty;
			}
	}
	~Board() = default;
	void play(int x, int y, State s)
	{
		points[x][y] = s;
		if (s == State::Black)
			zobrist_value ^= random_table[x][y].first;
		else
			zobrist_value ^= random_table[x][y].second;
	}
	State get_state(int x, int y) const
	{
		return points[x][y];
	}
	void erase(int x, int y)
	{
		if (points[x][y] == State::Black)
			zobrist_value ^= random_table[x][y].first;
		else
			zobrist_value ^= random_table[x][y].second;
		points[x][y] = State::Empty;
	}
	std::vector<std::pair<int, int>> possible_moves();
	int evaluate()
	{
		return evaluate_aux(computer) - evaluate_aux(player);
	}
	void clear()
	{
		for (auto& row : points)
			for (auto& point : row)
				point = State::Empty;
	}
	static void set_black_computer()
	{
		computer = State::Black;
		player = State::White;
	}
	static void set_black_player()
	{
		computer = State::White;
		player = State::Black;
	}
	static void set_opponent_player()
	{
		computer = State::Empty;
	}
	static bool is_opponent_AI()
	{
		return computer != State::Empty;
	}
	State get_winner()const;
	long long get_zobrist_value()const
	{
		return zobrist_value;
	}
	using BoardTable = std::array<std::array<std::pair<long long, long long>, 15>, 15>;
	void init_table(BoardTable& b)
	{
		random_table = b;
	}
private:
	static BoardTable random_table;
	std::array<std::array<State, 15>, 15> points;
	long long zobrist_value;
	int evaluate_aux(State s);
};




