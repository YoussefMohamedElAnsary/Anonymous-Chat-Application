#include "IniFile.h"

#define KEY_DELIMITER   '\n'
#define VALUE_DELIMITER '='
#define COMMENT_PREFIX  "#"

IniFile::IniFile(bool read, string str) : m_read(read), m_data(str)
{
}

IniFile::~IniFile()
{
}

void IniFile::SetData(string str)
{
	m_data = str;
}

string IniFile::ReadKey(string key)
{
	string res;

	if (m_read)
	{
		key.push_back(VALUE_DELIMITER);

		size_t pos = m_data.find(key);
		if (pos != string::npos)
		{
			pos += key.length();

			size_t endpos = m_data.find(KEY_DELIMITER, pos + 1);
			if (endpos == string::npos)
				endpos = m_data.length();

			res.assign(m_data.substr(pos, endpos - pos));
		}
	}

	return res;
}

int IniFile::ReadKeyInt(string key)
{
	return atoi(ReadKey(key).c_str());
}

bool IniFile::WriteComment(string comment)
{
	if (!m_read)
	{
		m_data += COMMENT_PREFIX " " + comment + KEY_DELIMITER;
		return true;
	}
	return false;
}

bool IniFile::WriteKey(string key, string value, bool newline)
{
	if (!m_read)
	{
		key.push_back(VALUE_DELIMITER);

		size_t pos = m_data.find(key);
		if (pos != string::npos)
		{
			pos += key.length();

			size_t endpos = m_data.find(KEY_DELIMITER, pos + 1);
			if (endpos == string::npos)
				endpos = m_data.length();

			m_data.replace(pos, endpos - pos, value);
			return true;
		}

		m_data += key + value + KEY_DELIMITER;
		if (newline)
			WriteLineBreak();
		return true;
	}
	return false;
}

bool IniFile::WriteKeyInt(string key, int value, bool newline)
{
	return WriteKey(key, to_string(value), newline);
}

bool IniFile::WriteLineBreak(void)
{
	if (!m_read)
	{
		m_data.push_back(KEY_DELIMITER);
		return true;
	}
	return false;
}

const string IniFile::operator[](string key)
{
	return ReadKey(key);
}

const char* IniFile::Data()
{
	return !m_read ? m_data.c_str() : NULL;
}
