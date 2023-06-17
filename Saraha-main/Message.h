#pragma once
#include "essentials.h"

struct Message
{
	int Index;
	string Content;
	SYSTEMTIME SentDate;
	bool Seen;
	bool IsFavorite;

};

