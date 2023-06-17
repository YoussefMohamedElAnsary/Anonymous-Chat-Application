#pragma once
#include "essentials.h"

class IniFile
{
	bool m_read;
	string m_data;

public:
	IniFile(bool read = true, string str = "");
	~IniFile();

	void SetData(string str);

	string ReadKey(string key);
	int ReadKeyInt(string key);

	bool WriteComment(string comment);
	bool WriteKey(string key, string value, bool newline = false);
	bool WriteKeyInt(string key, int value, bool newline = false);
	bool WriteLineBreak();

	const string operator[](string key);

	const char* Data();
};