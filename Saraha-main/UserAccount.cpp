#include "UserAccount.h"
#include "Misc.h"

void PrintDate(SYSTEMTIME d)
{
	cout << d.wDay << "-" << d.wMonth << "-" << d.wYear << " " << d.wHour << ":" << d.wMinute << endl;
}
bool compareByTime(const pair<Message, int>& a, const pair<Message, int>& b) {
	FILETIME aFileTime, bFileTime;
	SystemTimeToFileTime(&a.first.SentDate, &aFileTime);
	SystemTimeToFileTime(&b.first.SentDate, &bFileTime);
	return CompareFileTime(&aFileTime, &bFileTime) < 0;
}
UserAccount::UserAccount(void)
{
}

UserAccount::UserAccount(int id, string username, string pw)
{
	Set(id, username, pw);
}

void UserAccount::Set(int id, string username, string pw)
{
	m_id = id;
	m_username = username;
	m_password = pw;
}

int UserAccount::ID(void) const
{
	return m_id;
}

string UserAccount::Username(void) const
{
	return m_username;
}

string UserAccount::Password(void) const
{
	return m_password;
}

bool UserAccount::AddContact(int User_ID) {

	if (ReceivedMessages.find(User_ID) != ReceivedMessages.end()) {

		if (Contacts.find(User_ID) != Contacts.end()) {

			cout << User_ID<<" is already in your contacts list.\n";
		}
		else {

			Contacts.insert(User_ID);
			cout << User_ID << " has been added to your contacts list.\n";

		}
		return true;
	}

	return false;

}

bool UserAccount::RemoveContact(int User_ID) {

	if (Contacts.find(User_ID) != Contacts.end()) {
		Contacts.erase((User_ID));
		cout << User_ID << " has been removed from your contacts list.\n";
		return true;
	}
	return false;
}

bool UserAccount::Block(int User_ID)
{
	if (!IsBlocked(User_ID))
	{
		Blocked.insert(User_ID);
		return true;
	}
	return false;
}

bool UserAccount::Unblock(int User_ID)
{
	return Blocked.erase(User_ID);
}

char UserAccount::SendUserMessage(UserAccount* recipient, string content)
{
	if (!recipient)
		return 1;
	else if (IsBlocked(recipient->m_id))
		return 2;
	else if (recipient->IsBlocked(m_id))
		return 3;

	SYSTEMTIME time;
	GetSystemTime(&time);
	Message msg{ 0, content, time };
	
	// >> SENDER SIDE <<
	msg.Index = (!SentMessages.empty() ? SentMessages.top().second.Index : 0) + 1;
	SentMessages.push(pair<int, Message>(recipient->m_id, msg));
	
	// >> RECIPIENT/RECEIVER SIDE <<
	auto it = recipient->ReceivedMessages.find(m_id);

	// if received msgs from this user before
	if (it != recipient->ReceivedMessages.end())
	{
		msg.Index = (!it->second.empty() ? it->second.top().Index : 0) + 1;
		it->second.push(msg);
	}

	// first time to receive msg from this user
	else
	{
		stack<Message> msgs;
		msgs.push(msg);

		recipient->ReceivedMessages[m_id] = msgs;
	}
	return 0;
}

bool UserAccount::PopMessage(UserAccount* user) {

	// Pop from sender side
	if (SentMessages.empty())
		return false;
	SentMessages.pop();

	// Pop from receiver side
	auto it = user->ReceivedMessages.find(m_id);
	if (it == user->ReceivedMessages.end() || it->second.empty()) {
		return false;
	}
	else {
		it->second.pop();
		return true;
	}
}

void UserAccount::ViewContacts() { 
	set<int>::iterator itr;		   	
	vector<pair<int,int>>ContactTotalMessages;

	for (itr = Contacts.begin();itr != Contacts.end(); itr++) // Retrieving Contact's Sent Messages
	{
		int ContactID = *itr;
		int ContactMessages = ReceivedMessages[ContactID].size();
		ContactTotalMessages.push_back(make_pair(ContactMessages, ContactID)); // Storing as (Messages,ID) for easier sort lol :')
	}
	if (ContactTotalMessages.empty()) {
		cout << "Oh no, you don't have any contacts. :(" << endl;
	}
	else {
		sort(ContactTotalMessages.begin(), ContactTotalMessages.end(), greater<>());
		for (auto i : ContactTotalMessages) {
			cout << "Contact with ID " << i.second << ": " << i.first << "  Message" << endl;
		}
	}
}

void UserAccount::ViewReceivedMessages() {
	if (ReceivedMessages.empty() == true)
	{
		cout << "No messages to be displayed" << endl;
	}
	else
	{
		vector <pair<Message, int>> AllMessages;
		stack<Message> UserMessages;
		unordered_map<int, stack<Message>>::iterator it;

		for (it = ReceivedMessages.begin(); it != ReceivedMessages.end(); it++)
		{
			UserMessages = it->second;
			while (!UserMessages.empty())
			{
				AllMessages.emplace_back(UserMessages.top(), it->first);
				UserMessages.pop();
			}
		}
		std::sort(AllMessages.begin(), AllMessages.end(), compareByTime);

		for (int i = 0; i < AllMessages.size(); i++)
		{
			cout << AllMessages[i].first.Index << "| User ID: " << AllMessages[i].second << "| " << AllMessages[i].first.Content << "  ";
			PrintDate(AllMessages[i].first.SentDate);
		}
	}
}
bool UserAccount::ViewReceivedMessages(int User_ID) {
	stack<Message> UserMessages;

	if (ReceivedMessages.find(User_ID) == ReceivedMessages.end())
	{
		return false;
	}
	else
	{
		UserMessages = ReceivedMessages[User_ID];
		while (!UserMessages.empty())
		{

			cout << UserMessages.top().Index << "| " << UserMessages.top().Content<<"  ";
			PrintDate(UserMessages.top().SentDate);
			UserMessages.pop();
		}
		return true;
	}

}
void UserAccount::ViewSentMessages()
{
	stack<pair<int, Message>>Sent;
	Sent = SentMessages;
	while (!Sent.empty())
	{
		cout << Sent.top().second.Index << "| User ID: " << Sent.top().first << "| " << Sent.top().second.Content << "  ";
		PrintDate(Sent.top().second.SentDate);
		Sent.pop();
	}
	
}

bool UserAccount::PutFavorite(int User_ID, int Msg_Index) {
	if (ReceivedMessages.find(User_ID) == ReceivedMessages.end())
	{
		return false;
	}
	else
	{
		stack<Message> chat = ReceivedMessages[User_ID];
		while (!chat.empty())
		{
			if (chat.top().Index != Msg_Index)
			{
				chat.pop();
			}
			else
			{
				if (chat.top().IsFavorite == true)
				{
					cout << "This message is already a favourite";
					return false;
				}
				else
				{
					chat.top().IsFavorite = true;
					Favorites.emplace(User_ID, chat.top());
					return true;
				}
			}
		}
	}
}
bool UserAccount::RemoveOldestFavorite() {
	if (Favorites.empty())
		return false;
	Favorites.pop();
	return true;

}

bool UserAccount::ViewFavorites(void)
{
	if (Favorites.empty())
	{
		cout << "No favorite messages to display.\n";
		return false;
	}

	queue<pair<int, Message>> msgs = Favorites;
	pair<int, Message> msg;

	cout << "\n\n>>>>>>>>>> YOUR FAVORITE MESSEAGES <<<<<<<<<<\n\n";

	while (!msgs.empty())
	{
		msg = msgs.front();

		cout << "[" << DATE2STR(msg.second.SentDate) << "] "
			<< to_string(msg.first)
			<< (GetContact(msg.first) ? " (contact): " : ": ")
			<< msg.second.Content << '\n';

		msgs.pop();
	}
	return true;
}

int UserAccount::GetMessagesFromUser(UserAccount* user) {
	return ReceivedMessages[user->m_id].size();

}
bool UserAccount::GetContact(int User_ID) {
	auto it = Contacts.find(User_ID);
	if (it != Contacts.end()) {
		return true;
	}
	return NULL;
}

bool UserAccount::IsBlocked(int User_ID)
{
	return Blocked.find(User_ID) != Blocked.end();
}
