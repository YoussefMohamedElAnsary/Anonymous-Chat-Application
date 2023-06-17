#pragma once
#include "essentials.h"

#define DATE2STR(date) GetMessageDate(date)

vector<string> Split(const string& str, char splitter);
string DateToString(SYSTEMTIME& date);
SYSTEMTIME StringToDate(string str);
string GetMessageDate(SYSTEMTIME& date);

unsigned long long DateToULL(SYSTEMTIME& date);