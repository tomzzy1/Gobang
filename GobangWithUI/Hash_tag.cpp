#include "Hash_tag.h"

Hash_tag::~Hash_tag()
{
}

void Hash_tag::set(int d, int v, Hash_flag type, long long zobrist)
{
	if (d <= depth_ || hash_type == Hash_flag::Empty)
	{
		depth_ = d;
		score = v;
		hash_type = type;
		key = zobrist;
	}
}

void Hash_tag::clear()
{
	hash_type = Hash_flag::Empty;
}

std::optional<int> Hash_tag::get_score(int depth, int& alpha, int& beta, long long zobrist) const
{
	if (depth >= depth_ && zobrist == key)
	{
		if (hash_type == Hash_flag::Alpha)
		{
			if (score <= alpha)
				return alpha;
			if (score < beta)
				beta = score;
		}
		else if (hash_type == Hash_flag::Beta)
		{
			if (score >= beta)
				return beta;
			if (score > alpha)
				alpha = score;
		}
		else if (hash_type == Hash_flag::Exact)
		{
			return score;
		}
	}
	return {};
}
