#include "Hash_tag.h"

Hash_tag::~Hash_tag()
{
}

int Hash_tag::get_value(int depth, int alpha, int beta, long long zobrist) const
{
	if (zobrist == key && depth >= depth_)
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
