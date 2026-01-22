#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;
struct StUsers
{
	string Username;
	string Usernumber;
	int permission;
};
struct StClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkToUpdate = false;
	bool MarkToDelete = false;

};
enum EnPermission
{
	AllA = -1, ListA = 1, addA = 2, DeleteA = 4, UpdateA = 8, FindA = 16, TransactionA = 32, ManageA = 64
};
StUsers CurrentUser;
void showMainMinue();
void ShowTransactionScreen();
void ShowManageScreen();
string ReadAccountNumber();
void Login();
string ClientsFile = "Clients.txt";
string UsersFile = "Users.txt";
enum EnChoice
{
	Show = 1, Add = 2,
	Delete = 3, Update = 4,
	Find = 5,
	Operations = 6, Manage = 7,
	Logout = 8
};
enum operation
{
	Deposite = 1, Withdraw = 2, TotalBalance = 3, MainMenue = 4
};
enum UserOptions
{
	ListUser = 1, AddUser = 2, DeleteUser = 3, UpdateUser = 4, FindUser = 5, Exit = 6
};
short Choice(short From, short To)
{
	int n = 0;
	while (n < From || n > To)
	{
		cout << "Choice what do you want to do?" << " [" << From << " to " << To << "]? \n";
		cin >> n;
	}
	return n;
}
bool Search(vector<StClient>Clients, string AccountNumber)
{
	for (StClient Client : Clients)
	{
		if (Client.AccountNumber == AccountNumber)
			return true;
	}
	return false;
}
bool SearchAccountNumber(vector<StClient>Clients, string AccountNumber, int n)
{
	n -= 1;
	do
	{
		if (Search(Clients, AccountNumber))
			return 1;
		cout << "You have " << n << "tempt\n";
		AccountNumber = ReadAccountNumber();
	} while (--n);
	return 0;
}
vector<string>Splet(string s, string sep = " ")
{
	vector<string>Words;
	string sword = "";
	short pos = 0;
	while ((pos = s.find(sep)) != std::string::npos)
	{
		sword = s.substr(0, pos);
		if (sword != "")
		{
			Words.push_back(sword);
		}
		s.erase(0, pos + sep.length());
	}
	if (s != "")
	{
		Words.push_back(s);
	}
	return Words;

}
void BackTomainMinue()
{
	cout << "press any key to go back.............";
	system("pause");
	showMainMinue();

}
void BackToOpertaionMineu()
{
	system("pause");
	ShowTransactionScreen();
}
void BackToManageUsersMeneu()
{
	system("pause");
	ShowManageScreen();
}
void PrintClientOnScrean(StClient Client)
{
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "PIN Code : " << Client.PinCode << endl;
	cout << "Name : " << Client.Name << endl;
	cout << "Phone : " << Client.Phone << endl;
	cout << "Balance : " << Client.AccountBalance << endl;
}
void PrintAccessDediedMassage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this, \nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}
int ReadPermission()
{
	int Permission = 0;
	char Give = 'N';
	cout << "Do you want to give full Access?(y/n)";
	cin >> Give;
	if (Give == 'y' || Give == 'Y')
	{
		return -1;
	}
	else
	{
		cout << "Do you want to give access to : \n";
		cout << "Show Client List ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::ListA;
		}
		cout << "Add New Client ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::addA;
		}
		cout << "Delete Client ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::DeleteA;
		}
		cout << "Update Client ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::UpdateA;
		}
		cout << "Find Client ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::FindA;
		}
		cout << "Transaction ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::TransactionA;
		}
		cout << "Manage User ? ";
		cin >> Give;
		if (Give == 'y' || Give == 'Y')
		{
			Permission += EnPermission::ManageA;
		}
	}
	return Permission;

}
bool CheckPermissions(EnPermission Permission)
{
	if (CurrentUser.permission == EnPermission::AllA)
		return true;

	if ((Permission & CurrentUser.permission) == Permission)
		return true;
	return false;
}
StClient ReadClientInfo()
{
	StClient Client;
	cout << "Enter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);
	cout << "Enter Pin Code : ";
	getline(cin, Client.PinCode);
	cout << "Enter Name : ";
	getline(cin, Client.Name);
	cout << "Enter Phone Number : ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;
	return Client;

}
StUsers ReadUserInfo()
{
	StUsers user;
	cout << "Enter Username? ";
	cin >> user.Username;
	cout << "Enter UserNumber? ";
	cin >> user.Usernumber;
	return user;
}
StClient ReadClientInfo_toUpdate()
{
	StClient Client;
	cout << "Enter Pin Code : ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name : ";
	getline(cin, Client.Name);
	cout << "Enter Phone Number : ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;
	return Client;
}
string RecordToLine(StClient Client, string sep = "#*#")
{
	string Line = "";
	Line = Line + Client.AccountNumber + sep;
	Line = Line + Client.PinCode + sep;
	Line = Line + Client.Name + sep;
	Line = Line + Client.Phone + sep;
	Line = Line + to_string(Client.AccountBalance);
	return Line;

}
string RecordToLineForUsers(StUsers User, string sep = "#//#")
{
	string Line;
	Line = User.Username + sep + User.Usernumber + sep + to_string(User.permission);
	return Line;
}
StClient LineToRecord(string Line, string sep = "#*#")
{
	vector<string>Vinfo;
	StClient Client;
	Vinfo = Splet(Line, sep);
	Client.AccountNumber = Vinfo[0];
	Client.PinCode = Vinfo[1];
	Client.Name = Vinfo[2];
	Client.Phone = Vinfo[3];
	Client.AccountBalance = stod(Vinfo[4]);
	return Client;
}
StUsers LineToRecordForUsers(string user, string sep = "#//#")
{
	StUsers User;
	vector<string>SpletLine;
	SpletLine = Splet(user, sep);
	User.Username = SpletLine[0];
	User.Usernumber = SpletLine[1];
	User.permission = stoi(SpletLine[2]);
	return User;
}
void AddNewClientToFile(string Filename, string Line)
{
	fstream fout;
	fout.open(Filename, ios::out | ios::app);
	if (fout.is_open())
	{
		fout << Line << endl;
		fout.close();
	}

}
void AddClientsToFile(string Filename)
{
	if (!CheckPermissions(EnPermission::addA))
	{
		PrintAccessDediedMassage();
		return;
	}
	cout << "=============================================\n";
	cout << "\t\tAdd Clients Screen\n";
	cout << "=============================================\n";
	char ch = 'N';
	StClient Client;
	string line;
	do
	{
		system("cls");
		Client = ReadClientInfo();
		line = RecordToLine(Client, "#*#");
		AddNewClientToFile(Filename, line);
		cout << "The Client Added Successfully :-)\n";
		cout << "Do you want to add more client to this file(Y/N) ? ";
		cin >> ch;
	} while (ch == 'Y' || ch == 'y');

}
vector<StClient>LoadDataClientsFromFile(string Filename)
{
	fstream fin;
	string line;
	vector<StClient> Clients;
	fin.open(Filename, ios::in);
	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			StClient Client;
			Client = LineToRecord(line);
			Clients.push_back(Client);
		}
		fin.close();
	}
	return Clients;
}
vector<StUsers>LoadDataUsersFromFile(string FileName)
{
	string line;
	fstream fin;
	vector<StUsers> Users;
	fin.open(FileName, ios::in);

	if (fin.is_open())
	{

		while (getline(fin, line))
		{
			StUsers user;
			user = LineToRecordForUsers(line);
			Users.push_back(user);
		}
		fin.close();
	}
	return Users;
}
bool SearchForUserName(vector<StUsers> Users, string UserName)
{
	for (StUsers v : Users)
	{
		if (UserName == v.Username)
			return true;
	}
	return false;
}
bool searchForUserNumber(vector<StUsers>Users, string UserNumber)
{
	for (StUsers v : Users)
	{
		if (v.Usernumber == UserNumber)
			return true;
	}
	return false;
}
bool SearchForUser_UserName_UserNumber(vector<StUsers>Users, string username, string passward, StUsers& user)
{
	for (StUsers User : Users)
	{
		if (User.Username == username && User.Usernumber == passward)
		{
			user = User;
			return true;
		}
	}
	return false;
}
void ShowClientsOnScrean()
{
	if (!CheckPermissions(EnPermission::ListA))
	{
		PrintAccessDediedMassage();
		return;
	}
	vector<StClient>Clients;
	Clients = LoadDataClientsFromFile(ClientsFile);
	cout << setw(58) << "Client List (" << Clients.size() << ") Client(s)\n";
	cout << "___________________________________________________________________________________________________________________\n";
	cout << "| " << setw(17) << left << " Account Number"
		<< "| " << setw(13) << left << " Pin Code "
		<< "| " << setw(40) << left << " Client Name "
		<< "| " << setw(12) << left << " Phone "
		<< "| " << setw(12) << left << " Balance ";
	cout << "\n___________________________________________________________________________________________________________________\n";
	for (StClient Client : Clients)
	{
		cout << "| " << setw(17) << left << Client.AccountNumber
			<< "| " << setw(13) << left << Client.PinCode
			<< "| " << setw(40) << left << Client.Name
			<< "| " << setw(12) << left << Client.Phone
			<< "| " << setw(12) << left << Client.AccountBalance;
		cout << endl;
	}
	cout << "\n___________________________________________________________________________________________________________________\n";
}
bool LoadUserInfo(string username, string passward)
{
	vector<StUsers>Users = LoadDataUsersFromFile(UsersFile);
	if (SearchForUser_UserName_UserNumber(Users, username, passward, CurrentUser))
		return true;
	return false;
}
bool SearchAccountNumber_toDelete(string AccountNumber, vector<StClient>& Clients)
{
	for (StClient& Client : Clients)
	{
		if (Client.AccountNumber == AccountNumber)
		{

			PrintClientOnScrean(Client);
			char ch = 'y';
			cout << "Are you sure do you want to delete this client (Y/N)";
			cin >> ch;
			if (toupper(ch) == 'Y')
			{
				Client.MarkToDelete = true;
				return true;
			}
			return false;
		}
	}
	cout << "Not Found!\n";
	return 0;
}
void DeleteClient(vector<StClient>Clients)
{

	fstream fout;
	fout.open(ClientsFile, ios::out);
	if (fout.is_open())
	{
		for (StClient Client : Clients)
		{
			if (Client.MarkToDelete == false)
			{

				string line;
				line = RecordToLine(Client, "#*#");
				fout << line << endl;
			}
		}
	}
}
string ReadAccountNumber()
{
	string AccountNumber;
	cout << "Enter Account Number : ";
	cin >> AccountNumber;
	return AccountNumber;
}
void DeleteClientFromFile()
{
	if (!CheckPermissions(EnPermission::DeleteA))
	{
		PrintAccessDediedMassage();
		return;
	}
	cout << "=============================================\n";
	cout << "\t\tDelete Clients Screen\n";
	cout << "=============================================\n";
	vector<StClient>Clients;
	Clients = LoadDataClientsFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();
	if (SearchAccountNumber_toDelete(AccountNumber, Clients))
	{
		DeleteClient(Clients);
	}
}
bool SearchAccountNumber_ToUpdate(string AccountNumber, vector<StClient>& Clients)
{
	for (StClient& Client : Clients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			PrintClientOnScrean(Client);
			char ch = 'y';
			cout << "Are you sure do you want to update this client (Y/N)?";
			cin >> ch;
			if (toupper(ch) == 'Y')
			{
				Client.MarkToUpdate = true;
				return true;
			}
			return false;
		}
	}
	cout << "Not Found!\n";
	return false;
}
void update(vector<StClient>Clients, string AccountNumber)
{
	fstream fout;
	fout.open(ClientsFile, ios::out);
	{
		if (fout.is_open())
			for (StClient Client : Clients)
			{
				string line;
				if (Client.MarkToUpdate == false)
				{

					line = RecordToLine(Client);
					fout << line << endl;
				}
				else
				{
					Client = ReadClientInfo_toUpdate();
					line = RecordToLine(Client);
					fout << line << endl;
				}
				
			}
		fout.close();

	}
}
void UpdateClient()
{
	if (!CheckPermissions(EnPermission::UpdateA))
	{
		PrintAccessDediedMassage();
		return;
	}
	cout << "=============================================\n";
	cout << "\t\tUpdate Clients Screen\n";
	cout << "=============================================\n";
	vector<StClient>Clients;
	Clients = LoadDataClientsFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();
	if (SearchAccountNumber_ToUpdate(AccountNumber, Clients))
	{
		update(Clients, AccountNumber);
	}
}
bool IsFindClient(vector<StClient>Clients, string AccountNumber)
{
	for (StClient Client : Clients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			return true;
		}
	}
	return false;
}
void find()
{
	if (!CheckPermissions(EnPermission::FindA))
	{
		PrintAccessDediedMassage();
		return;
	}
	cout << "=============================================\n";
	cout << "\t\tFind Client Screen\n";
	cout << "=============================================\n";
	vector<StClient>Clients;
	Clients = LoadDataClientsFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();
	if (IsFindClient(Clients, AccountNumber))
	{
		cout << "YES ,This Client Is Found.";
	}
	else
	{
		cout << "No ,This Client is not found.";
	}
}
double EnterHowManyDipositeNumber()
{
	double Dep;
	cout << "Enter Your Diposite Number: ";
	cin >> Dep;
	return Dep;
}
void deposite(vector<StClient>& Clients, string AccountNumber, double Dep)
{
	fstream fout;
	fout.open(ClientsFile, ios::out);
	if (fout.is_open())
	{
		for (StClient& Client : Clients)
		{
			string line;
			if (Client.AccountNumber == AccountNumber)
			{
				if (Dep >= 0 || (Dep < 0 && Client.AccountBalance >= (Dep * -1)))
				{
					Client.AccountBalance += Dep;
					cout << "Done Successfully\n";
				}
				else
				{
					cout << "You have not enough many :-(\n";
				}
			}
			line = RecordToLine(Client, "#*#");
			fout << line << endl;
		}
	}

}
void DepositeToClient()
{
	vector<StClient>Clients;
	Clients = LoadDataClientsFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();
	if (SearchAccountNumber(Clients, AccountNumber, 3))
	{
		double Dep = EnterHowManyDipositeNumber();
		deposite(Clients, AccountNumber, Dep);
	}
	else
	{
		cout << "This Account Number Does  Not Valid\n";
	}
}
void WithDraw()
{
	vector<StClient>Clients;
	Clients = LoadDataClientsFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();
	if (SearchAccountNumber(Clients, AccountNumber, 3))
	{
		double Dep = EnterHowManyDipositeNumber();
		Dep *= -1;
		deposite(Clients, AccountNumber, Dep);
	}
	else
	{
		cout << "This Account Number Does  Not Valid\n";
	}
}
void PrintTotalBalanceLine(StClient Client)
{
	cout << "| " << setw(17) << left << Client.AccountNumber
		<< "| " << setw(40) << left << Client.Name
		<< "| " << setw(12) << left << Client.AccountBalance;
}
void ShowTotalBalance()
{
	vector<StClient> Clients = LoadDataClientsFromFile(ClientsFile);
	double Total = 0;
	cout << "\t\t\t\t" << "Total Balance for(" << Clients.size() << ") Client(s)\n";
	cout << "___________________________________________________________________________________________________________________\n";
	cout << "| " << setw(17) << left << " Account Number"
		<< "| " << setw(40) << left << " Client Name "
		<< "| " << setw(12) << left << " Balance ";
	cout << "\n___________________________________________________________________________________________________________________\n";
	if (Clients.size() == 0)
		cout << "No Clients Avalable in the system\n";
	else
	{
		for (StClient Client : Clients)
		{
			Total += Client.AccountBalance;
			PrintTotalBalanceLine(Client);
			cout << endl;
		}
		cout << "\n___________________________________________________________________________________________________________________\n";
		cout << "Total Balance = " << Total << endl;
	}

}
void Transaction(operation op)
{
	switch (op)
	{
	case operation::Deposite:
	{
		DepositeToClient();
		BackToOpertaionMineu();
		break;
	}
	case operation::Withdraw:
	{
		WithDraw();
		BackToOpertaionMineu();
		break;
	}
	case operation::TotalBalance:
	{
		ShowTotalBalance();
		BackToOpertaionMineu();
		break;
	}
	case operation::MainMenue:
		break;
	}
}
void ShowTransactionScreen()
{
	if (!CheckPermissions(EnPermission::TransactionA))
	{
		PrintAccessDediedMassage();
		return;
	}
	system("cls");
	cout << "=============================================\n";
	cout << "\t\tTransaction Menue Screen\n";
	cout << "=============================================\n";
	cout << "\t[1]Deposit.\n";
	cout << "\t[2]WithDraw.\n";
	cout << "\t[3]Total Balances.\n";
	cout << "\t[4]Main Menue.\n";
	cout << "=============================================\n";
	short Ch = Choice(1, 4);
	return Transaction(operation(Ch));
}
void PrintUsers(vector<StUsers>Users)
{
	for (StUsers User : Users)//
	{
		cout << "| " << User.Username << "\t" << "| " << User.Usernumber << "\t" << "| " << User.permission;
		cout << "\n--------------------------------------------------------------------------------------\n";
	}
}
void List()
{
	vector<StUsers>Users;
	Users = LoadDataUsersFromFile(UsersFile);
	cout << setw(58) << "Users List (" << Users.size() << ") Users(s)\n";
	cout << "___________________________________________________________________________________________________________________\n";
	cout << "| " << setw(17) << left << "Username"
		<< "| " << setw(13) << left << "Userumber"
		<< "| " << setw(40) << left << "Permission";
	cout << "\n___________________________________________________________________________________________________________________\n";
	for (StUsers User : Users)
	{
		cout << "| " << setw(17) << left << User.Username
			<< "| " << setw(13) << left << User.Usernumber
			<< "| " << setw(40) << left << User.permission;
		cout << endl;
	}
	cout << "\n___________________________________________________________________________________________________________________\n";
}
void AddNewLineToFile(string FileName, string line)
{
	fstream fout;
	fout.open(FileName, ios::out | ios::app);
	if (fout.is_open())
	{
		fout << endl << line;
	}
	fout.close();
}
void AddNewUser(string FileName)
{
	vector<StUsers>Users = LoadDataUsersFromFile(UsersFile);

	StUsers User;
	cout << "Enter User Name?";
	cin >> User.Username;
	while (SearchForUserName(Users, User.Username))
	{
		cout << "User with [" << User.Username << "] Name already exists, Enter anthor UserName : ";
		cin >> User.Username;
	}
	cout << "Enter User Number :";
	cin >> User.Usernumber;
	while (searchForUserNumber(Users, User.Usernumber))
	{
		cout << "User with [" << User.Usernumber << "] Passward already exists, Enter anthor User Number : ";
		cin >> User.Usernumber;
	}
	User.permission = ReadPermission();

	string line;
	line = RecordToLineForUsers(User, "#//#");
	AddNewLineToFile(FileName, line);
}
void AddUsers()
{
	char ch = 'N';
	do {
		AddNewUser(UsersFile);
		cout << "Do you want to add more(Y/N)?";
		cin >> ch;
	} while (ch == 'y' || ch == 'Y');
}
bool Delete_User(string FileName)
{
	vector<StUsers> Users = LoadDataUsersFromFile(UsersFile);
	StUsers User;
	User = ReadUserInfo();
	if (SearchForUserName(Users, User.Username))
	{
		if (searchForUserNumber(Users, User.Usernumber))
		{
			if (User.Username == "Admin")
			{
				cout << "You can't delete this user :-(\n";
			}
			else
			{
				char ch = 'N';
				cout << "Are you sure do want to delete this user?(y/n)";
				cin >> ch;
				if (ch == 'y' || ch == 'Y')
				{
					fstream fout;
					fout.open(FileName, ios::out);
					if (fout.is_open())
						for (StUsers U : Users)
						{
							if (U.Username == User.Username)
								continue;
							fout << RecordToLineForUsers(U) << endl;
						}
					fout.close();
					return true;
				}
			}

		}
		else
		{
			cout << "Invalid Passward:-(\n";
			return false;
		}
	}
	else
	{
		cout << "Invalid UserName\n";
		return false;
	}

	return false;
}
void DeleteUsers()
{
	char ch = 'N';
	char c = 'N';
	do {
		if (Delete_User(UsersFile))
		{
			cout << "Deleted Successfully:-)\n";
			cout << "Do you want to Delete more(Y/N)?";
			cin >> ch;
		}

	} while (ch == 'y' || ch == 'Y');

}
bool Update_User(string FileName)
{
	vector<StUsers> Users = LoadDataUsersFromFile(FileName);;
	StUsers User;
	User = ReadUserInfo();
	if (SearchForUserName(Users, User.Username))
	{
		if (searchForUserNumber(Users, User.Usernumber))
		{
			if (User.Username == "Admin")
			{
				cout << "You can't Update this user :-(\n";
			}
			else
			{
				char ch = 'N';
				cout << "Are you sure do want to Update this user?(y/n)";
				cin >> ch;
				if (ch == 'y' || ch == 'Y')
				{
					fstream fout;
					fout.open(FileName, ios::out);
					if (fout.is_open())
						for (StUsers U : Users)
						{
							if (U.Username == User.Username)
							{
								cout << "\n============Update===========\n";
								U = ReadUserInfo();
								U.permission = ReadPermission();
							}
							fout << RecordToLineForUsers(U) << endl;
						}
					fout.close();
					return true;
				}
			}

		}
		else
		{
			cout << "Invalid Passward:-(\n";
			return false;
		}
	}
	else
	{
		cout << "Invalid UserName\n";
		return false;
	}

	return false;

}
void UpdateUsers()
{
	char ch = 'N';
	char c = 'N';
	do {
		if (Update_User(UsersFile))
		{
			cout << "Updated Successfully:-)\n";
			cout << "Do you want to update more(Y/N)?";
			cin >> ch;
		}

	} while (ch == 'y' || ch == 'Y');
}
void PrintUserInfo(StUsers User)
{
	cout << "User Name :" << User.Username << endl;
	cout << "User Number :" << User.Usernumber << endl;
}
void FindUsers()
{
	vector<StUsers>Users = LoadDataUsersFromFile(UsersFile);
	StUsers User = ReadUserInfo();
	if (SearchForUserName(Users, User.Username) && searchForUserNumber(Users, User.Usernumber))
	{
		cout << "User With User Name(" << User.Username << ") Is Found :\n";
		PrintUserInfo(User);
	}
	else
	{
		cout << "Dose Not Exist:-(\n";
	}
}
void ManageUsers(UserOptions choice)
{
	switch (choice)
	{
	case UserOptions::ListUser:
		List();
		BackToManageUsersMeneu();
		break;
	case UserOptions::AddUser:
		AddUsers();
		BackToManageUsersMeneu();
		break;
	case UserOptions::DeleteUser:
		DeleteUsers();
		BackToManageUsersMeneu();
		break;
	case UserOptions::UpdateUser:
		UpdateUsers();
		BackToManageUsersMeneu();
		break;
	case UserOptions::FindUser:
		FindUsers();
		BackToManageUsersMeneu();
		break;
	default:
		showMainMinue();
		break;
	}
}
void ShowManageScreen()
{
	if (!CheckPermissions(EnPermission::ManageA))
	{
		PrintAccessDediedMassage();
		return;
	}
	system("cls");
	cout << "========================================================\n";
	cout << "\t\t\tManage Users Menue Screen \n";
	cout << "========================================================\n";
	cout << "\t\t[1] List Users.\n";
	cout << "\t\t[2] Add New User.\n";
	cout << "\t\t[3] Delete User.\n";
	cout << "\t\t[4] Update User.\n";
	cout << "\t\t[5] Find User.\n";
	cout << "\t\t[6] Main Menue.\n";
	cout << "========================================================\n";
	cout << "Chooce What do you want to do? [1 to 6]?";
	return ManageUsers(UserOptions(Choice(1, 6)));
}
void Bank(EnChoice Choice)
{

	switch (Choice)
	{
	case EnChoice::Show:
	{
		ShowClientsOnScrean();
		BackTomainMinue();
		break;
	}
	case EnChoice::Add:
	{

		AddClientsToFile(ClientsFile);
		BackTomainMinue();
		break;
	}
	case EnChoice::Delete:
	{

		DeleteClientFromFile();
		BackTomainMinue();
		break;
	}
	case EnChoice::Update:
	{

		UpdateClient();
		BackTomainMinue();
		break;
	}
	case EnChoice::Find:
	{

		find();
		BackTomainMinue();
		break;
	}
	case EnChoice::Operations:
	{

		ShowTransactionScreen();
		BackTomainMinue();
		break;
	}
	case EnChoice::Manage:
	{
		ShowManageScreen();
		BackTomainMinue();
		break;
	}
	case EnChoice::Logout:
		Login();
		break;
	}
}
void showMainMinue()
{
	system("cls");
	cout << "=========================================================================\n";
	cout << "\t\t\t\tMian Menue Screen\n";
	cout << "=========================================================================\n";
	cout << "\t\t[1]Show Client List.\n";
	cout << "\t\t[2]Add New Client.\n";
	cout << "\t\t[3]Delete Client.\n";
	cout << "\t\t[4]Update Client Info.\n";
	cout << "\t\t[5]Find Client\n";
	cout << "\t\t[6]Transaction\n";
	cout << "\t\t[7]Manage Users.\n";
	cout << "\t\t[8]Logout.\n";
	cout << "=========================================================================\n";
	EnChoice choice = EnChoice(Choice(1, 8));
	return (Bank(choice));
}
void Login()
{
	system("cls");
	cout << "\n-----------------------------------------------------\n";
	cout << "\t\t\tLogin Screen\n";
	cout << "\n-----------------------------------------------------\n";
	StUsers user;
	vector<StUsers>Users = LoadDataUsersFromFile(UsersFile);
	bool LoginFailed = false;
	do
	{
		if (LoginFailed)
		{
			cout << "Invalid UseName/Passward\n";
		}
		user = ReadUserInfo();
		LoginFailed = !LoadUserInfo(user.Username, user.Usernumber);

	} while (LoginFailed);

	showMainMinue();
}
int main()
{
	Login();
	return 0;
}
