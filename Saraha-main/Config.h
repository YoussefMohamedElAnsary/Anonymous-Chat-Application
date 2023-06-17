#pragma once
#include "essentials.h"
#include "IniFile.h"
#include "UserAccount.h"

class Config
{
private:
	bool m_loaded;
	string m_file;

	int m_nextaccountid;
public:
	map<int, UserAccount> UserAccounts;

public:
	Config(const char* filename);
	~Config();

	bool Load();
	bool Save();

private:
	bool GetConfigFile(IniFile& ini) const;
	bool SaveConfigFile(IniFile& ini);

	void LoadAccounts(IniFile& cfg);
	void LoadContacts(UserAccount& acc, int idx, IniFile& cfg);
	void LoadBlocks(UserAccount& acc, int idx, IniFile& cfg);
	void LoadMessages(UserAccount& acc, int idx, IniFile& cfg);
	void LoadFavoriteMessages(UserAccount& acc, int idx, IniFile& cfg);

	void WriteAccounts(IniFile& cfg);
	void WriteContacts(UserAccount& acc, int idx, IniFile& cfg);
	void WriteBlocks(UserAccount& acc, int idx, IniFile& cfg);
	void WriteMessages(UserAccount& acc, int idx, IniFile& cfg);
	void WriteFavoriteMessages(UserAccount& acc, int idx, IniFile& cfg);

public:
	UserAccount* AccountExists(string username);
	UserAccount* AccountExists(string username, const string& pw);

	UserAccount* GetUserAccount(int id);
	int PopNextAccountID();

	static void AppendMessage(UserAccount& acc, int senderid, Message& msg, bool sent);
};