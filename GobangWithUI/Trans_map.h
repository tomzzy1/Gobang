#pragma once
#include <vector>
#include "Hash_tag.h"

class Trans_map
{
public:
	Trans_map() = default;
	Trans_map(size_t size = 48)
		:table_size_mask(size / 24 * 100 - 1)
		,trans_table(size * 1024 * 1024 / sizeof(Hash_tag)) {}//size should be 3,6,12,24,48...
	~Trans_map();
	Hash_tag& operator[](long long zobrist_value)
	{
		return trans_table[zobrist_value & table_size_mask];
	}
private:
	const int table_size_mask;
	std::vector<Hash_tag> trans_table;
};

