#pragma once
#define ValUnknown 1000001

class Hash_tag
{
public:
	enum class Hash_flag
	{
		Alpha, Beta, Exact
	};
	Hash_tag() = default;
	~Hash_tag();
	void set(int d, int v, Hash_flag type, long long zobrist);
	int get_value(int depth, int alpha, int beta, long long zobrist) const;
private:
	int depth_ = 10;
	int val;
	long long key;
	Hash_flag hash_type;
};

