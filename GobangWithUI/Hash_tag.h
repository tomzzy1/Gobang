#pragma once
#define Val_unknown 1000001
#include "spinlock.h"

class Hash_tag
{
public:
	enum class Hash_flag
	{
		Alpha, Beta, Exact, Empty
	};
	Hash_tag() = default;
	~Hash_tag();
	void set(int d, int v, Hash_flag type, long long zobrist);
	void clear();
	int get_score(int depth, int& alpha, int& beta, long long zobrist) const;
private:
	Spinlock lock;
	int depth_;
	int score;
	long long key;
	Hash_flag hash_type = Hash_flag::Empty;
};

