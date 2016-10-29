#include "Hash_tag.h"

Hash_tag::~Hash_tag()
{
}

void Hash_tag::set(int d, int v, Hash_flag type, long long zobrist)
{
	depth_ = d;
	v = val;
	hash_type = type;
	key = zobrist;
}

int Hash_tag::get_value(int depth, int alpha, int beta, long long zobrist) const
{
	if (depth >= depth_ && zobrist == key)
	{
		if (hash_type == Hash_flag::Alpha && val <= alpha)
		{
			return alpha;
		}
		if (hash_type == Hash_flag::Beta && val >= beta)
		{
			return beta;
		}
		if (hash_type == Hash_flag::Exact)
		{
			return val;
		}
	}
	return ValUnknown;
}
