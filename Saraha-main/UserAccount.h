#pragma once
#include "essentials.h"
#include "Message.h"

class UserAccount
{
private:
	int m_id;
	string m_username;
	string m_password;
public:
	set<int> Contacts;
	set<int> Blocked;
	stack<pair<int, Message>> SentMessages;
	unordered_map<int, stack<Message>> ReceivedMessages;
	queue<pair<int, Message>> Favorites;

public:
	UserAccount();
	UserAccount(int id, string username, string pw);

	void Set(int id, string username, string pw);

	bool AddContact(int User_ID);
	bool RemoveContact(int User_ID);
	bool Block(int User_ID);
	bool Unblock(int User_ID);
	char SendUserMessage(UserAccount* recipient, string content);
	bool PopMessage(UserAccount* user);
	void ViewContacts(); //ordered by most messages
	void ViewReceivedMessages();
	bool ViewReceivedMessages(int User_ID);
	void ViewSentMessages();
	bool PutFavorite(int User_ID, int Msg_Index);
	bool RemoveOldestFavorite();
	bool ViewFavorites();

	int GetMessagesFromUser(UserAccount* user);
	bool GetContact(int User_ID);
	int ID() const;
	string Username() const;
	string Password() const;
	bool IsBlocked(int User_ID);
};