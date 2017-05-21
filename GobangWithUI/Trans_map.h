#pragma once
#include <vector>
#include "Hash_tag.h"

class Trans_map
{
public:
	Trans_map() = default;
	Trans_map(size_t size = 48)
		:trans_table(size * 1024 * 1024 / sizeof(Hash_tag))
		, table_size_mask(trans_table.size() - 1) {}//size should be 6,12,24,48...
	~Trans_map();
	Hash_tag& operator[](long long zobrist_value)
	{
		return trans_table[zobrist_value & table_size_mask];
	}
	void reset();
private:
	std::vector<Hash_tag> trans_table;
	const size_t table_size_mask;
};

