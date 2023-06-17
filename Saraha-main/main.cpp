#include "main.h"
#include <chrono>
#include <ctime>
#include <conio.h>

#define CONFIG_FILE "data.ini"

Config cfg(CONFIG_FILE);
UserAccount* current_user = NULL;

void TestingViewContact()
{
	UserAccount acc(11111, "Wael", "12345");
	set<int>temp = { 1,2,3,4,5 };
	acc.Contacts = temp;
	Message m = { 0,"Test",time(NULL),false};
	Message m1 = { 1,"Test",time(NULL),false };
	Message m2 = { 2,"Test",time(NULL),false };

	/*acc.Messages[1].push(m);
	acc.Messages[1].push(m1);
	acc.Messages[1].push(m2);
	acc.Messages[2].push(m);
	acc.Messages[2].push(m1);
	acc.Messages[3].push(m);*/
	acc.ViewContacts();
}

void TestingFavorites()
{
	if (current_user)
	{
		cout << "Saving favorites..\n";

		SYSTEMTIME time;

		GetSystemTime(&time);
		current_user->Favorites.push(pair<int, Message>(3, Message{ 1, "uwuuu", time, true }));

		Sleep(5 * 1000);
		GetSystemTime(&time);
		current_user->Favorites.push(pair<int, Message>(3, Message{ 2, "adeni gram m7ba", time, true }));

		cfg.Save();
		cout << "Finished saving favorites.\n";
	}
}

// Main functions
bool Register(string Username, string Password);
UserAccount* Login(string Username, string Password);
void LoginRegisterMenu();
void HomeScreen();
void MessageScreen();
void ViewMessageFromContactScreen();
void ContactScreen();
void FavoriteSettingScreen();
string PasswordCensoring();
bool PasswordTypoChecker(string pass1, string pass2);
void OnExit();


//--------------------------------------------------------------------------

int main()
{
	// Load configuration on startup
	cfg.Load();

	// Save configuration on exit (not finished yet)
	LoginRegisterMenu();

	atexit(OnExit);
	return 0;
}

void OnExit()
{
	cfg.Save();
}


//--------------------------------------------------------------------------

string PasswordCensoring()
{
	string passw = "";
	char ch;
	ch = _getch();
	while (ch != 13) //Equivalent of Pressing "Enter"
	{
		if (ch != 8) //Equivalent of Pressing "Backspace"
		{
			passw.push_back(ch);
			cout << "*";
		}
		else
		{
			if (!passw.empty())
			{
				passw.pop_back();
				cout << "\b" << " " << "\b";
			}
		}
		ch = _getch();
	}
	return passw;
}
bool PasswordTypoChecker(string pass1, string pass2) 
{
	if (pass1.compare(pass2) == 0)
		return true;
	else
		return false;
}

bool Register(string Username, string Password)
{
	if (cfg.AccountExists(Username) == NULL)
	{
		int new_id = cfg.PopNextAccountID();
		UserAccount new_acc(new_id, Username, Password);
		cfg.UserAccounts[new_acc.ID()] = new_acc;
		return true;
	}
	else
		return false;
}

UserAccount* Login(string Username, string Password)
{
	if (cfg.AccountExists(Username, Password) == NULL)
		return NULL;
	else
		return cfg.AccountExists(Username, Password);
}

void ClearConsole()
{
	system("cls");
}

void SystemPause()
{
	system("pause");
}

void LoginRegisterMenu()
{
	ClearConsole();

	cout << "----------------- Welcome to Saraha -----------------\n";
	cout << "1. LOGIN\n2. REGISTER\n3. EXIT\n\nYour choice: ";
	char choice;
	cin >> choice;
	string username, password, password_check;
	switch (choice)
	{
	case '1':
	{
		ClearConsole();
		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter password: ";
		password = PasswordCensoring();

		if (Login(username, password) == NULL)
		{
			cout << "\n\nError: Invalid credentials, please try again. \n";
			SystemPause();
			LoginRegisterMenu();
		}
		else
		{
			current_user = Login(username, password);
			cout << "\n\nLogin Successful\n";
			SystemPause();
			HomeScreen();
		}
		break;
	}

	case '2':
	{
		ClearConsole();

		cout << "Enter Username: ";
		cin >> username;
		cout << "Enter Password: ";
		password = PasswordCensoring();
		cout << "\nRe-enter your password: ";
		password_check = PasswordCensoring();

		if (!PasswordTypoChecker(password, password_check))
		{
			cout << "\n\nError: Passwords do not match\n";
			SystemPause();
			LoginRegisterMenu();
		}
		else
		{
			if (Register(username, password))
			{
				cout << "\n\nAccount Created\n";
				cout << "Please login.\n";
				SystemPause();
				LoginRegisterMenu();
				;
			}
			else
			{
				cout << "\n\nError: Account already exists\n";
				SystemPause();
				LoginRegisterMenu();
			}
		}
		break;
	}

	case '3':
		return;

	default:
		cout << "\n\nError: Please enter a valid choice\n";
		SystemPause();
		LoginRegisterMenu();
	}
}

void HomeScreen()
{	
	ClearConsole();
	cout << "1. View messages.\n2. View contacts.\n3. View favorite messages.\n4. Logout\n5. Exit.\n\nYour choice: ";
	char choice;
	cin >> choice;
	switch (choice)
	{
	case '1':
		MessageScreen();
		break;
	case '2':
		ContactScreen();
		break;
	case '3':
		ClearConsole();
		current_user->ViewFavorites();
		system("pause");
		HomeScreen();
		break;
	case '4':
		LoginRegisterMenu();
		break;
	case '5':
		return;
	default:
		cout << "Please enter a valid choice\n";
		SystemPause();
		HomeScreen();
	}
}
void MessageScreen() 
{	
	ClearConsole();
	cout << "1. Send a message.\n2. Pop last message.\n3. View all sent messages\n4. View all received messages.\n5. View received messages from a specific user.\n6. View contacts.\n7. Return home screen.\n\nYour choice: ";
	char choice;
	cin >> choice;

	switch (choice)
	{
	case '1':
	{
		cout << "Please enter the user ID to send him/her a message\n";
		int ID;
		cin >> ID;

		cout << "Please enter the message you'd like to send\n";
		cin.ignore();
		string msg;
		getline(cin, msg, '\n');

		if (UserAccount* receiver = cfg.GetUserAccount(ID))
		{
			switch (current_user->SendUserMessage(receiver, msg))
			{
			case 0:
				cout << "Message has been sent.\n";
				break;

			case 1:
				cout << "User was not found.\n";
				break;

			case 2:
				cout << "Could not deliver message, you may have blocked this user.\n";
				break;

			case 3:
				cout << "Could not deliver message, you may have been blocked by this user.\n";
				break;

			default:
				cout << "Could not deliver message for unknown reason.\n";
			}
		}
		else
			cout << "User was not found.\n";
		system("pause");
		MessageScreen();
		break;
	}

	case '2':
	{
		cout << "Please enter the user ID to pop last message you sent him/her\n";
		int ID;
		cin >> ID;
		UserAccount* receiver = cfg.GetUserAccount(ID);
		if (receiver != NULL)
		{
			bool result = current_user->PopMessage(receiver);
			if (result) {
				cout << "Message has been popped.\n";
			}
			else {
				cout << "No messages to delete.\n";

			}
		}
		else
			cout << "User was not found.\n";
		system("pause");
		MessageScreen();
		break;
	}

	case '3':
		ClearConsole();
		cout << "Here is the messages you sent, from most recent to oldest:\n";
		current_user->ViewSentMessages();
		system("pause");
		MessageScreen();
		break;
	case '4':
		ClearConsole();
		cout << "Here is your inbox from most recent to oldest:\n";
		current_user->ViewReceivedMessages();
		//Add Adding to Favorite Logic
		FavoriteSettingScreen();
		system("pause");
		MessageScreen();
		break;

	case '5':
	{
		cout << "Please enter the user ID to display their messages\n";
		int ID;
		cin >> ID;
		if (current_user->ViewReceivedMessages(ID))
		{
			cout << "------------------------------------" << endl;
		}
		else
		{
			cout << "No messages are found" << endl;
		}
		//Add Adding to Favorite Logic

		system("pause");
		MessageScreen();
		break;
	}
	case '6':
		ContactScreen();
		break;
	case '7':
		HomeScreen();
		break;

	default:
		cout << "Please enter a valid choice\n";
		SystemPause();
		MessageScreen();
	}
}

void ViewMessageFromContactScreen()
{	
	ClearConsole();
	current_user->ViewContacts();
	cout << "------------------------------------------------------\n\n";
	cout << "Enter User ID to view their messages.\nTo return home, press 0\n\nYour choice: ";
	int ID;
	cin >> ID;
	if (!ID)
		HomeScreen();
	else {
		if (current_user->ViewReceivedMessages(ID)) // Momkn n5liha boolean;
		{

		}
		else {
			cout << "Invalid user ID. Please, try again.\n";
			SystemPause();
			ViewMessageFromContactScreen();
		}
	}
}



void ContactScreen()
{
	ClearConsole();
	current_user->ViewContacts();
	cout << "------------------------------------------------------\n\n";
	cout << "1. Add new contact\n2. Remove a contact.\n3. Block user.\n4. Unblock user.\n5. Return Home\n\nYour choice: ";
	char choice;
	int ID;

	cin >> choice;
	switch (choice)
	{
	case '1':
		cout << "Please enter the User ID your want to add to your contacts list\n";
		cin >> ID;
		if (!current_user->AddContact(ID))
		{
			cout << ID << " should send at least one message to be added to the contacts list.\n";

		}
		SystemPause();
		ContactScreen();
		break;
	case '2':
		cout << "Please enter the User ID your want to remove from your contacts list\n";
		cin >> ID;
		if (!current_user->RemoveContact(ID))
			cout << ID << " isn't on your contacts list.\n";

		SystemPause();
		ContactScreen();
		break;

	case '3':
	{
		cout << "Please enter the User ID your want to block\n";
		cin >> ID;

		if (current_user->Block(ID))
			cout << ID << " has been blocked.\n";
		else
			cout << ID << " is already blocked!\n";

		SystemPause();
		ContactScreen();
		break;
	}

	case '4':
	{
		cout << "Please enter the User ID your want to unblock\n";
		cin >> ID;

		if (current_user->Unblock(ID))
			cout << ID << " has been unblocked.\n";
		else
			cout << ID << " is not blocked!\n";

		SystemPause();
		ContactScreen();
		break;
	}

	case '5':
		HomeScreen();
		break;

	default:
		cout << "Please enter a valid choice.\n";
		SystemPause();
		ContactScreen();
	}

}

void FavoriteSettingScreen() {
	cout << "Press 1 to add a message to favorites.\n";
	char choice;
	cin >> choice;
	if (choice == '1')
	{
		int ID, Ind;
		cout << "Please enter the user id and message index you wish to add to your favorite list, and any key to go back.\n";
		cin >> ID >> Ind;
		if (current_user->PutFavorite(ID, Ind)) {
			cout << "Message "<<Ind<< "from user "<<ID<<" has been added to favorite.\n";

		}
		else {
			cout << "Something went wrong try again.\n";
			system("pause");
			ClearConsole();
			cout << "Here is your inbox from most recent to oldest:\n";
			current_user->ViewReceivedMessages();
			//Add Adding to Favorite Logic
			FavoriteSettingScreen();
			system("pause");
			MessageScreen();

		}
	}
	else {
		system("pause");
		MessageScreen();
	}
}
 