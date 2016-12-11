#include "Board.h"
#include <set>

Board::BoardTable Board::random_table;

Board::State Board::computer = State::Black;

Board::State Board::player = State::White;

std::vector<std::pair<int, int>> Board::possible_moves()
{
	auto no_more_than_14 = [](int n) {return n > 14 ? 14 : n; };
	auto no_less_than_0 = [](int n) {return n < 0 ? 0 : n; };
	std::set<std::pair<int, int>> moves;
	auto insert_if_empty = [&](int x_pos, int y_pos)
	{
		if (points[x_pos][y_pos] == State::Empty)
			moves.insert({ x_pos,y_pos });
	};
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 15; ++j)
		{
			if (points[i][j] != State::Empty) //position around a not empty position is meaningful
			{
				for (int k = 1; k <= 2; ++k)
				{
					insert_if_empty(i, no_more_than_14(j + k));
					insert_if_empty(i, no_less_than_0(j - k));
					insert_if_empty(no_more_than_14(i + k), j);
					insert_if_empty(no_less_than_0(i - k), j);
					insert_if_empty(no_more_than_14(i + k), no_more_than_14(j + k));
					insert_if_empty(no_less_than_0(i - k), no_more_than_14(j + k));
					insert_if_empty(no_more_than_14(i + k), no_less_than_0(j - k));
					insert_if_empty(no_less_than_0(i - k), no_less_than_0(j - k));
				}
			}
		}
	std::vector<std::pair<int, int>> temp(moves.size());
	std::copy(moves.begin(), moves.end(), temp.begin());
	return temp;
}

Board::State Board::get_winner() const
{
	for (int i = 0; i < 15; ++i)
		for (int j = 0; j < 11; ++j)
		{
			if (points[i][j] == State::Black
				&& points[i][j + 1] == State::Black && points[i][j + 2] == State::Black
				&& points[i][j + 3] == State::Black && points[i][j + 4] == State::Black)
				return State::Black;
			if (points[i][j] == State::White
				&& points[i][j + 1] == State::White && points[i][j + 2] == State::White
				&& points[i][j + 3] == State::White && points[i][j + 4] == State::White)
				return State::White;
		}
	for (int i = 0; i < 11; ++i)
		for (int j = 0; j < 15; ++j)
		{
			if (points[i][j] == State::Black
				&& points[i + 1][j] == State::Black && points[i + 2][j] == State::Black
				&& points[i + 3][j] == State::Black && points[i + 4][j] == State::Black)
				return State::Black;
			if (points[i][j] == State::White
				&& points[i + 1][j] == State::White && points[i + 2][j] == State::White
				&& points[i + 3][j] == State::White && points[i + 4][j] == State::White)
				return State::White;
		}
	for (int i = 0; i < 11; ++i)
		for (int j = 0; j < 11; ++j)
		{
			if (points[i][j] == State::Black
				&& points[i + 1][j + 1] == State::Black && points[i + 2][j + 2] == State::Black
				&& points[i + 3][j + 3] == State::Black && points[i + 4][j + 4] == State::Black)
				return State::Black;
			if (points[i][j] == State::White
				&& points[i + 1][j + 1] == State::White && points[i + 2][j + 2] == State::White
				&& points[i + 3][j + 3] == State::White && points[i + 4][j + 4] == State::White)
				return State::White;
		}
	for (int i = 0; i < 11; ++i)
		for (int j = 14; j >= 4; --j)
		{
			if (points[i][j] == State::Black
				&& points[i + 1][j - 1] == State::Black && points[i + 2][j - 2] == State::Black
				&& points[i + 3][j - 3] == State::Black && points[i + 4][j - 4] == State::Black)
				return State::Black;
			if (points[i][j] == State::White
				&& points[i + 1][j - 1] == State::White && points[i + 2][j - 2] == State::White
				&& points[i + 3][j - 3] == State::White && points[i + 4][j - 4] == State::White)
				return State::White;
		}
	return State::Empty;
}

int Board::evaluate_aux(const State s)const
{
	//using namespace std::chrono;
	//auto start = system_clock::now();
	int sum = 0;
	int cnt_four = 0;
	int cnt_three = 0;
	int cnt_four_with_hole = 0;
	int cnt_two = 0;
	int cnt_three_with_hole = 0;
	int cnt_one = 0;
	int cnt_five = 0;
	int cnt_bad_four = 0;
	int cnt_two_two = 0;
	int cnt_one_three = 0;
	//horizonal
	// loop for six
	for (int i = 0; i <= 14; ++i)
		for (int j = 0; j <= 9; ++j)
		{
			//cnt_four -0000-
			if (points[i][j] == State::Empty && points[i][j + 1] == s
				&& points[i][j + 2] == s && points[i][j + 3] == s
				&& points[i][j + 4] == s && points[i][j + 5] == State::Empty)
				++cnt_four;
			//cnt_three -000-- or --000-
			if (points[i][j] == State::Empty && points[i][j + 1] == s
				&& points[i][j + 2] == s && points[i][j + 3] == s
				&& points[i][j + 4] == State::Empty && points[i][j + 5] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i][j + 1] == State::Empty
				&& points[i][j + 2] == s && points[i][j + 3] == s
				&& points[i][j + 4] == s && points[i][j + 5] == State::Empty)
				++cnt_three;
			//cnt_four_with_hole -0-00- or -00-0-
			if (points[i][j] == State::Empty && points[i][j + 1] == s
				&& points[i][j + 2] == s && points[i][j + 3] == State::Empty
				&& points[i][j + 4] == s && points[i][j + 5] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i][j + 1] == s
				&& points[i][j + 2] == State::Empty && points[i][j + 3] == s
				&& points[i][j + 4] == s && points[i][j + 5] == State::Empty)
				++cnt_four_with_hole;
			//cnt_two --00--
			if (points[i][j] == State::Empty && points[i][j + 1] == State::Empty
				&& points[i][j + 2] == s && points[i][j + 3] == s
				&& points[i][j + 4] == State::Empty && points[i][j + 5] == State::Empty)
				++cnt_two;
			//cnt_three_with_hole -0-0-- or --0-0-
			if (points[i][j] == State::Empty && points[i][j + 1] == State::Empty
				&& points[i][j + 2] == s && points[i][j + 3] == State::Empty
				&& points[i][j + 4] == s && points[i][j + 5] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i][j + 1] == s
				&& points[i][j + 2] == State::Empty && points[i][j + 3] == s
				&& points[i][j + 4] == State::Empty && points[i][j + 5] == State::Empty)
				++cnt_three_with_hole;
			//cnt_one --0--- ---0--
			if (points[i][j] == State::Empty && points[i][j + 1] == State::Empty
				&& points[i][j + 2] == s && points[i][j + 3] == State::Empty
				&& points[i][j + 4] == State::Empty && points[i][j + 5] == State::Empty)
				++cnt_one;
			if (points[i][j] == State::Empty && points[i][j + 1] == State::Empty
				&& points[i][j + 2] == State::Empty && points[i][j + 3] == s
				&& points[i][j + 4] == State::Empty && points[i][j + 5] == State::Empty)
				++cnt_one;
		}
	//loop for five
	for (int i = 0; i <= 14; ++i)
		for (int j = 0; j <= 10; ++j)
		{
			if (points[i][j] == s
				&& points[i][j + 1] == s && points[i][j + 2] == s
				&& points[i][j + 3] == s && points[i][j + 4] == s)
				++cnt_five;
			if (points[i][j] == State::Empty
				&& points[i][j + 1] == s && points[i][j + 2] == s
				&& points[i][j + 3] == s && points[i][j + 4] == s)
				++cnt_bad_four;
			if (points[i][j] == s
				&& points[i][j + 1] == s && points[i][j + 2] == s
				&& points[i][j + 3] == s && points[i][j + 4] == State::Empty)
				++cnt_bad_four;
			if (points[i][j] == s
				&& points[i][j + 1] == s && points[i][j + 2] == State::Empty
				&& points[i][j + 3] == s && points[i][j + 4] == s)
				++cnt_two_two;
			if (points[i][j] == s
				&& points[i][j + 1] == State::Empty && points[i][j + 2] == s
				&& points[i][j + 3] == s && points[i][j + 4] == s)
				++cnt_one_three;
			if (points[i][j] == s
				&& points[i][j + 1] == s && points[i][j + 2] == s
				&& points[i][j + 3] == State::Empty && points[i][j + 4] == s)
				++cnt_one_three;
		}
	//vertical
	//loop for six
	for (int i = 0; i <= 9; ++i)
		for (int j = 0; j <= 14; ++j)
		{
			if (points[i][j] == State::Empty && points[i + 1][j] == s
				&& points[i + 2][j] == s && points[i + 3][j] == s
				&& points[i + 4][j] == s && points[i + 5][j] == State::Empty)
				++cnt_four;
			if (points[i][j] == State::Empty && points[i + 1][j] == s
				&& points[i + 2][j] == s && points[i + 3][j] == s
				&& points[i + 4][j] == State::Empty && points[i + 5][j] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i + 1][j] == State::Empty
				&& points[i + 2][j] == s && points[i + 3][j] == s
				&& points[i + 4][j] == s && points[i + 5][j] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i + 1][j] == s
				&& points[i + 2][j] == s && points[i + 3][j] == State::Empty
				&& points[i + 4][j] == s && points[i + 5][j] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j] == s
				&& points[i + 2][j] == State::Empty && points[i + 3][j] == s
				&& points[i + 4][j] == s && points[i + 5][j] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j] == State::Empty
				&& points[i + 2][j] == s && points[i + 3][j] == s
				&& points[i + 4][j] == State::Empty && points[i + 5][j] == State::Empty)
				++cnt_two;
			if (points[i][j] == State::Empty && points[i + 1][j] == State::Empty
				&& points[i + 2][j] == s && points[i + 3][j] == State::Empty
				&& points[i + 4][j] == s && points[i + 5][j] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j] == s
				&& points[i + 2][j] == State::Empty && points[i + 3][j] == s
				&& points[i + 4][j] == State::Empty && points[i + 5][j] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j] == State::Empty
				&& points[i + 2][j] == s && points[i + 3][j] == State::Empty
				&& points[i + 4][j] == State::Empty && points[i + 5][j] == State::Empty)
				++cnt_one;
			if (points[i][j] == State::Empty && points[i + 1][j] == State::Empty
				&& points[i + 2][j] == State::Empty && points[i + 3][j] == s
				&& points[i + 4][j] == State::Empty && points[i + 5][j] == State::Empty)
				++cnt_one;
		}
	//loop for five
	for (int i = 0; i <= 10; ++i)
		for (int j = 0; j <= 14; ++j)
		{
			if (points[i][j] == s
				&& points[i + 1][j] == s && points[i + 2][j] == s
				&& points[i + 3][j] == s && points[i + 4][j] == s)
				++cnt_five;
			if (points[i][j] == State::Empty
				&& points[i + 1][j] == s && points[i + 2][j] == s
				&& points[i + 3][j] == s && points[i + 4][j] == s)
				++cnt_bad_four;
			if (points[i][j] == s
				&& points[i + 1][j] == s && points[i + 2][j] == s
				&& points[i + 3][j] == s && points[i + 4][j] == State::Empty)
				++cnt_bad_four;
			if (points[i][j] == s
				&& points[i + 1][j] == s && points[i + 2][j] == State::Empty
				&& points[i + 3][j] == s && points[i + 4][j] == s)
				++cnt_two_two;
			if (points[i][j] == s
				&& points[i + 1][j] == State::Empty && points[i + 2][j] == s
				&& points[i + 3][j] == s && points[i + 4][j] == s)
				++cnt_one_three;
			if (points[i][j] == s
				&& points[i + 1][j] == s && points[i + 2][j] == s
				&& points[i + 3][j] == State::Empty && points[i + 4][j] == s)
				++cnt_one_three;
		}
	//diagonal
	//loop for six
	for (int i = 0; i <= 9; ++i)
		for (int j = 0; j <= 9; ++j)
		{
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == s
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == s && points[i + 5][j + 5] == State::Empty)
				++cnt_four;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == s //cnt_three --000- or -000--
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == State::Empty && points[i + 5][j + 5] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == State::Empty
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == s && points[i + 5][j + 5] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == s //cnt four with hole -00-0- or -0-00-
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == State::Empty
				&& points[i + 4][j + 4] == s && points[i + 5][j + 5] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == s
				&& points[i + 2][j + 2] == State::Empty && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == s && points[i + 5][j + 5] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == State::Empty //cnt_two --00--
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == State::Empty && points[i + 5][j + 5] == State::Empty)
				++cnt_two;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == State::Empty //cnt three with hole --0-0- or -0-0--
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == State::Empty
				&& points[i + 4][j + 4] == s && points[i + 5][j + 5] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == s
				&& points[i + 2][j + 2] == State::Empty && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == State::Empty && points[i + 5][j + 5] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == State::Empty //cnt one --0--- or ---0--
				&& points[i + 2][j + 2] == s && points[i + 3][j + 3] == State::Empty
				&& points[i + 4][j + 4] == State::Empty && points[i + 5][j + 5] == State::Empty)
				++cnt_one;
			if (points[i][j] == State::Empty && points[i + 1][j + 1] == State::Empty
				&& points[i + 2][j + 2] == State::Empty && points[i + 3][j + 3] == s
				&& points[i + 4][j + 4] == State::Empty && points[i + 5][j + 5] == State::Empty)
				++cnt_one;
		}
	//loop for five
	for (int i = 0; i <= 10; ++i)
		for (int j = 0; j <= 10; ++j)
		{
			if (points[i][j] == s //cnt_five 00000
				&& points[i + 1][j + 1] == s && points[i + 2][j + 2] == s
				&& points[i + 3][j + 3] == s && points[i + 4][j + 4] == s)
				++cnt_five;
			if (points[i][j] == State::Empty //cnt bad four -0000 or 0000-
				&& points[i + 1][j + 1] == s && points[i + 2][j + 2] == s
				&& points[i + 3][j + 3] == s && points[i + 4][j + 4] == s)
				++cnt_bad_four;
			if (points[i][j] == s
				&& points[i + 1][j + 1] == s && points[i + 2][j + 2] == s
				&& points[i + 3][j + 3] == s && points[i + 4][j + 4] == State::Empty)
				++cnt_bad_four;
			if (points[i][j] == s//cnt two two 00-00
				&& points[i + 1][j + 1] == s && points[i + 2][j + 2] == State::Empty
				&& points[i + 3][j + 3] == s && points[i + 4][j + 4] == s)
				++cnt_two_two;
			if (points[i][j] == s//cnt one three 000-0 or 0-000
				&& points[i + 1][j + 1] == State::Empty && points[i + 2][j + 2] == s
				&& points[i + 3][j + 3] == s && points[i + 4][j + 4] == s)
				++cnt_one_three;
			if (points[i][j] == s
				&& points[i + 1][j + 1] == s && points[i + 2][j + 2] == s
				&& points[i + 3][j + 3] == State::Empty && points[i + 4][j + 4] == s)
				++cnt_one_three;
		}
	//another direction
	//loop for six
	for (int i = 0; i <= 9; ++i)
		for (int j = 14; j >= 5; --j)
		{
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == s
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == s && points[i + 5][j - 5] == State::Empty)
				++cnt_four;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == s //cnt_three --000- or -000--
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == State::Empty && points[i + 5][j - 5] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == State::Empty
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == s && points[i + 5][j - 5] == State::Empty)
				++cnt_three;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == s //cnt four with hole -00-0- or -0-00-
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == State::Empty
				&& points[i + 4][j - 4] == s && points[i + 5][j - 5] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == s
				&& points[i + 2][j - 2] == State::Empty && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == s && points[i + 5][j - 5] == State::Empty)
				++cnt_four_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == State::Empty //cnt_two --00--
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == State::Empty && points[i + 5][j - 5] == State::Empty)
				++cnt_two;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == State::Empty //cnt three with hole --0-0- or -0-0--
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == State::Empty
				&& points[i + 4][j - 4] == s && points[i + 5][j - 5] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == s
				&& points[i + 2][j - 2] == State::Empty && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == State::Empty && points[i + 5][j - 5] == State::Empty)
				++cnt_three_with_hole;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == State::Empty //cnt one --0--- or ---0--
				&& points[i + 2][j - 2] == s && points[i + 3][j - 3] == State::Empty
				&& points[i + 4][j - 4] == State::Empty && points[i + 5][j - 5] == State::Empty)
				++cnt_one;
			if (points[i][j] == State::Empty && points[i + 1][j - 1] == State::Empty
				&& points[i + 2][j - 2] == State::Empty && points[i + 3][j - 3] == s
				&& points[i + 4][j - 4] == State::Empty && points[i + 5][j - 5] == State::Empty)
				++cnt_one;
		}
	//loop for five
	for (int i = 0; i <= 10; ++i)
		for (int j = 14; j >= 4; --j)
		{
			if (points[i][j] == s //cnt_five 00000
				&& points[i + 1][j - 1] == s && points[i + 2][j - 2] == s
				&& points[i + 3][j - 3] == s && points[i + 4][j - 4] == s)
				++cnt_five;
			if (points[i][j] == State::Empty //cnt bad four -0000 or 0000-
				&& points[i + 1][j - 1] == s && points[i + 2][j - 2] == s
				&& points[i + 3][j - 3] == s && points[i + 4][j - 4] == s)
				++cnt_bad_four;
			if (points[i][j] == s
				&& points[i + 1][j - 1] == s && points[i + 2][j - 2] == s
				&& points[i + 3][j - 3] == s && points[i + 4][j - 4] == State::Empty)
				++cnt_bad_four;
			if (points[i][j] == s//cnt two two 00-00
				&& points[i + 1][j - 1] == s && points[i + 2][j - 2] == State::Empty
				&& points[i + 3][j - 3] == s && points[i + 4][j - 4] == s)
				++cnt_two_two;
			if (points[i][j] == s//cnt one three 000-0 or 0-000
				&& points[i + 1][j - 1] == State::Empty && points[i + 2][j - 2] == s
				&& points[i + 3][j - 3] == s && points[i + 4][j - 4] == s)
				++cnt_one_three;
			if (points[i][j] == s
				&& points[i + 1][j - 1] == s && points[i + 2][j - 2] == s
				&& points[i + 3][j - 3] == State::Empty && points[i + 4][j - 4] == s)
				++cnt_one_three;
		}
	sum += cnt_five * 50000;
	sum += cnt_four * 4320;
	sum += cnt_three * 720;
	sum += cnt_four_with_hole * 720;
	sum += cnt_bad_four * 720;
	sum += cnt_two_two * 720;
	sum += cnt_one_three * 720;
	sum += cnt_two * 120;
	sum += cnt_three_with_hole * 120;
	sum += cnt_one * 20;
	return sum;
}



