#pragma once
#include <vector>
#include <array>

class Board
{
public:

	enum class State { Black, White, Empty };
	static State computer;
	static State player;

	Board()
		:points(15, std::vector<State>(15, State::Empty)) {};
	~Board() = default;
	bool operator==(const Board& other)const
	{
		return points == other.points;
	}
	void play(int x, int y, State s)
	{
		points[x][y] = s;
	}
	State get_state(int x, int y) const
	{
		return points[x][y];
	}
	void erase(int x, int y)
	{
		points[x][y] = State::Empty;
	}
	std::vector<std::pair<int, int>> possible_moves();
	int evaluate()const
	{
		if (computer == State::Black)
			return evaluate_aux(computer) - 0.8 * evaluate_aux(player);
		return evaluate_aux(player) - 0.8 * evaluate_aux(computer); //computer plays white
	}
	void clear()
	{
		for (auto &row : points)
			for (auto &col : row)
				col = State::Empty;
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
	long long get_zobrist_value()const;
private:
	using BoardTable = std::array<std::array<std::pair<long long, long long>, 15>, 15>;
	static BoardTable random_table;
	static BoardTable random_init();
	std::vector<std::vector<State>> points;
	int evaluate_aux(const State s)const;
};




