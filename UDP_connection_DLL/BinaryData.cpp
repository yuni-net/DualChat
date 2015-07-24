#include "BinaryData.h"
#include <cstring>

BinaryData::BinaryData(const int byte)
{
	data = new char[byte];
	next_head = 0;
}

void BinaryData::add(const void * value, const int byte)
{
	memcpy(data + next_head, value, byte);
	next_head += byte;
}

void BinaryData::add(const char * text)
{
	add(text, strlen(text) + 1);
}

const char * BinaryData::get_buffer() const
{
	return data;
}

bool BinaryData::is_same_beginning(void * target) const
{
	return memcmp(data, target, next_head) == 0;
}

BinaryData::~BinaryData()
{
	delete [] data;
}
