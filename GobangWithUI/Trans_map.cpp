#include "Trans_map.h"

Trans_map::~Trans_map()
{
}

void Trans_map::reset()
{
	for (auto& tag : trans_table)
	{
		tag.clear();
	}
}
