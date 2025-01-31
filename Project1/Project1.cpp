#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

enum enChoice{ShowAll = 1, Add, Delete, Update, Find, Transactions, ManageUsers, Logout};
enum enTransactionChoice{enDeposit = 1, WithDraw, TotalBalances, MainMenu};
enum enUserManagementChoice { ListUsers = 1, AddUser, DeleteUser, UpdateUser, FindUser, UsersMainMenu };

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct sClient
{
    string AccNumber;
    string PinCode;
    string Name;
    string Phone;
    double Balance;
    bool MarkForDelete = false;
};

struct stUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};

vector <sClient> LoadClientsDataFromFile(string );
void ShowAllClients(vector<sClient> );
void ReadCLients(vector<sClient>);
void PrintClientCard(sClient);
bool FindClientByAccountNumber(string,vector<sClient>, sClient&);
bool DeleteClientByAccountNumber(string,vector<sClient>& );
bool UpdateClientByAccountNumber(string,vector<sClient>& );
bool Deposit(vector<sClient>& , string , double );
string ReadClientAccountNumber();
vector <sClient> SaveClientsDataToFile(string , vector<sClient> );


bool Login(string, stUser&);
vector <stUser> LoadUsersDataFromFile(string);
void ShowAllUsers(vector<stUser>);
void ReadUsers(vector<stUser>);
bool FindUserByUserName(string, vector<stUser>, stUser&);
void PrintUserCard(stUser);
bool DeleteUserByUserName(string, vector<stUser>&);
bool UpdateUserByUserName(string, vector<stUser>&);
void StartProgram();


void ShowMainMenu()
{
    cout << "=======================================================\n";
    cout << "\t\t\tMain Menu Screen\n";
    cout << "=======================================================\n";
    cout << "\t\t[1] Show Clients List.\n";
    cout << "\t\t[2] Add New Client.\n";
    cout << "\t\t[3] Delete Client.\n";
    cout << "\t\t[4] Update Client Info.\n";
    cout << "\t\t[5] Find Client.\n";
    cout << "\t\t[6] Transactions.\n";
    cout << "\t\t[7] Manage Users.\n";
    cout << "\t\t[8] LogOut.\n";
    cout << "=======================================================\n";

}

enChoice ReadUserChoice()
{
    short Choice;
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> Choice;
    return enChoice(Choice);
}

enTransactionChoice ReadTransactionChoice()
{
    short Choice;
    cout << "Choose what do you want to do? [1 to 4]? ";
    cin >> Choice;
    return enTransactionChoice(Choice);
}

enUserManagementChoice ReadUserManagementChoice()
{
    short Choice;
    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> Choice;
    return enUserManagementChoice(Choice);
}

void PauseSystem()
{
    cout << "Press any key to go back to Main Menu...";
    system("pause>0");
}

void LogOut()
{
    StartProgram();
}


void ShowAllClientsScreen()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    ShowAllClients(vClients);
}

void ShowAddNewClientScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tAdd New Client Screen.\n";
    cout << "----------------------------------------\n";
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    ReadCLients(vClients);
    vClients = LoadClientsDataFromFile(ClientsFileName);
}

void ShowDeleteClientScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tDelete Client Screen.\n";
    cout << "----------------------------------------\n";
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);
    vClients = LoadClientsDataFromFile(ClientsFileName);
}

void ShowUpdateClientScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tUpdate Client Screen.\n";
    cout << "----------------------------------------\n";
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
    vClients = LoadClientsDataFromFile(ClientsFileName);
}

void ShowFindClientScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tFind Client Screen.\n";
    cout << "----------------------------------------\n";
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
    }
    else {
        cout << "Client with Account Number (" << AccountNumber << ") Was Not Found!\n\n";
    }
}

void ShowTransactionsMenu()
{
    cout << "=======================================================\n";
    cout << "\t\t\Transactions Menu Screen\n";
    cout << "=======================================================\n";
    cout << "\t\t[1] Deposit\n";
    cout << "\t\t[2] WithDraw.\n";
    cout << "\t\t[3] Total Balances.\n";
    cout << "\t\t[4] Main Menu.\n";
    cout << "=======================================================\n";
}


void PauseTransactionScreen()
{
    cout << "Press any key to go back to Transactions Menu...";
    system("pause>0");
}

void ShowDepositScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tDeposit Screen.\n";
    cout << "----------------------------------------\n";

}

double ReadDepositAmount()
{
    double Amount;
    cout << "Please Enter Deposit Amount: ";
    cin >> Amount;
    return Amount;
}

void PerformTransaction(enTransactionChoice Choice)
{
    ShowDepositScreen();

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        double DepositAmount = ReadDepositAmount();

        if (Choice == enTransactionChoice::WithDraw)
        {
            while (DepositAmount > Client.Balance)
            {
                cout << "Invalid Draw Amount, you can draw up to: " << Client.Balance << endl;
                DepositAmount = ReadDepositAmount();
            }
            DepositAmount = -1 * DepositAmount;

        }

        char PerformTransaction = 'n';
        cout << "Are you sure you want to perform this transaction? y/n: ";

        cin >> PerformTransaction;
        if (tolower(PerformTransaction) == 'y')
        {
            Deposit(vClients, AccountNumber, DepositAmount);

            vClients = SaveClientsDataToFile(ClientsFileName, vClients);

            cout <<"Transaction Done Successfully." << endl;

            FindClientByAccountNumber(AccountNumber, vClients, Client);

            cout << "Balance = " << Client.Balance << endl;
        }
    }
    else {
        cout << "Client with Account Number (" << AccountNumber << ") Was Not Found!\n\n";
    }

}

void PrintClientBalanceRecord(sClient Client)
{
    cout << left << "|" << setw(16) << Client.AccNumber
        << "|" << setw(10) << setw(50) << Client.Name <<"|" << setw(10) << Client.Balance << endl;
}

void ShowAllClientsBalances(vector<sClient> vClients)
{

    cout << "\t\t\t\t\t" << "ClientList(" << vClients.size() << ") Client(s)." << endl;
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    cout << left << "|" << setw(16) << "Account Number" << "|" << setw(50) << "Client Name" << "|"
         << setw(10) << "Balance\n";
    cout << "---------------------------------------------------------------------------------------------------------------\n";

    for (sClient Client : vClients)
    {
        PrintClientBalanceRecord(Client);
    }
    cout << "---------------------------------------------------------------------------------------------------------------\n";

}

double SumBalances(vector<sClient> vClients)
{
    double Sum = 0;
    for (sClient C : vClients)
    {
        Sum += C.Balance;
    }
    return Sum;
}

void ShowTotalBalances()
{
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    
    ShowAllClientsBalances(vClients);

    cout << "\n\t\t\t\t TotalBalances = " << SumBalances(vClients) << endl;
}

void PerformTransactions()
{
    enTransactionChoice Choice;
    do
    {
        system("cls");
        ShowTransactionsMenu();
        Choice = ReadTransactionChoice();

        switch (Choice)
        {
        case enDeposit:
        case WithDraw:
            system("cls");
            PerformTransaction(Choice);
            PauseTransactionScreen();
            break;
        case TotalBalances:
            system("cls");
            ShowTotalBalances();
            PauseTransactionScreen();
            break;
        case MainMenu:
            break;
        default:
            break;
        }
    } while (Choice != enTransactionChoice::MainMenu);
}




void ShowUserManagementMenu()
{
    cout << "=======================================================\n";
    cout << "\t\tUser Management Menu Screen\n";
    cout << "=======================================================\n";
    cout << "\t\t[1] List Users.\n";
    cout << "\t\t[2] Add User.\n";
    cout << "\t\t[3] Delete User.\n";
    cout << "\t\t[4] Update User.\n";
    cout << "\t\t[5] Find User.\n";
    cout << "\t\t[6] Main Menu.\n";
    cout << "=======================================================\n";
}

void PauseUserManagementScreen()
{
    cout << "Press any key to go back to User Management Menu...";
    system("pause>0");
}

string ReadUserName()
{
    string UserName;
    cout << "Enter User Name: ";
    cin >> UserName;

    return UserName;
}

void ShowAddNewUserScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tAdd New User Screen.\n";
    cout << "----------------------------------------\n";
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    ReadUsers(vUsers);
}

void ShowDeleteUserScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tDelete User Screen.\n";
    cout << "----------------------------------------\n";
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, vUsers);
}

void ShowUpdateUserScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tUpdate User Screen.\n";
    cout << "----------------------------------------\n";
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowFindUserScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\tFind User Screen.\n";
    cout << "----------------------------------------\n";
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    stUser User;

    string UserName = ReadUserName();
    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserCard(User);
    }
    else {
        cout << "User With User Name (" << UserName << ") Was Not Found!\n\n";
    }
}

void PerformUserManagement()
{
    enUserManagementChoice Choice;
    do
    {
        system("cls");
        ShowUserManagementMenu();
        Choice = ReadUserManagementChoice();
        vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

        switch (Choice)
        {
        case ListUsers:
            system("cls");
            ShowAllUsers(vUsers);
            PauseUserManagementScreen();
            break;
        case AddUser:
            system("cls");
            ShowAddNewUserScreen();
            PauseUserManagementScreen();
            break;
        case DeleteUser:
            system("cls");
            ShowDeleteUserScreen();
            PauseUserManagementScreen();
            break;
        case UpdateUser:
            system("cls");
            ShowUpdateUserScreen();
            PauseUserManagementScreen();
            break;
        case FindUser:
            system("cls");
            ShowFindUserScreen();
            PauseUserManagementScreen();
            break;

        case UsersMainMenu:
            break;
        default:
            break;
        }
    } while (Choice != enUserManagementChoice::UsersMainMenu);
}

void ShowLoginScreen()
{
    cout << "----------------------------------------\n";
    cout << "\t\Login Screen.\n";
    cout << "----------------------------------------\n";

}

void ShowAccessDeniedScreen()
{
    cout << "----------------------------------------\n";
    cout << "Access Denied,\n";
    cout << "You don't have the permission to do this,\n";
    cout << "Please Contact your admin.\n";
    cout << "----------------------------------------\n";
}




void StartProgram()
{
    stUser User;
    bool Logged = false;

    ShowLoginScreen();
    do
    {
        if (!(Login(UsersFileName, User)))
        {
            system("cls");
            ShowLoginScreen();
            cout << "Invlaid UserName/Password!" << endl;
        }
        else
            Logged = true;

    } while (!Logged);

    enChoice UserChoice; 
    do
    {
        system("cls");

        ShowMainMenu();
        UserChoice = ReadUserChoice();

        switch (UserChoice)
        {
        case ShowAll:
            system("cls");
            if ((User.Permissions & 1) == 0)
                ShowAccessDeniedScreen();
            else
                ShowAllClientsScreen();
            PauseSystem();
            break;
        case Add:
            system("cls");
            if ((User.Permissions & 2) == 0)
                ShowAccessDeniedScreen();
            else
                ShowAddNewClientScreen();
            PauseSystem();
            break;
        case Delete:
            system("cls");
            if ((User.Permissions & 4) == 0)
                ShowAccessDeniedScreen();
            else
                ShowDeleteClientScreen();
            PauseSystem();

            break;
        case Update:
            system("cls");
            if ((User.Permissions & 8) == 0)
                ShowAccessDeniedScreen();
            else
                ShowUpdateClientScreen();
            PauseSystem();

            break;
        case Find:
            system("cls");
            if ((User.Permissions & 16) == 0)
                ShowAccessDeniedScreen();
            else
                ShowFindClientScreen();
            PauseSystem();

            break;

        case Transactions:
            system("cls");
            if ((User.Permissions & 32) == 0)
            {
                ShowAccessDeniedScreen();
                PauseSystem();
            }
            else
                PerformTransactions();

            break;
        case ManageUsers:
            system("cls");
            if ((User.Permissions & 64) == 0)
            {
                ShowAccessDeniedScreen();
                PauseSystem();
            }
            else
                PerformUserManagement();
            break;

            break;
        case Logout:
            system("cls");
            LogOut();
            break;
        default:
            break;
        }

    } while (UserChoice != enChoice::Logout);

    
}

int main()
{

    StartProgram();

}

vector<string> Split(string S1, string delim)
{
    vector<string> vTokens;
    short position = 0;
    string Word;

    while ((position = S1.find(delim)) != string::npos)
    {
        Word = S1.substr(0, position);

        if (Word != "")
            vTokens.push_back(Word);

        S1 = S1.erase(0, position + delim.length());
    }

    if (S1 != "")
        vTokens.push_back(S1);

    return vTokens;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = Split(Line, Seperator);
    Client.AccNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.Balance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.Balance);
    return stClientRecord;
}

void AddDataLineToFile(string FileName, string Line)
{
    fstream File;
    File.open(FileName, ios::out | ios::app);

    if (File.is_open())
    {
        File << Line << endl;
        File.close();
    }
}

sClient ReadNewClient()
{
    sClient Record;

    cout << "Enter PinCode? ";
    getline(cin >> ws, Record.PinCode);
    cout << "Enter name? ";
    getline(cin, Record.Name);
    cout << "Enter Phone? ";
    getline(cin, Record.Phone);
    cout << "Enter account Balance? ";
    cin >> Record.Balance;

    return Record;
}

bool AddNewCLient(string AccountNumber, vector<sClient> vClients)
{
    sClient Client;
    if(FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "The Client already exist!" << endl;
        PrintClientCard(Client);
        return false;
    }
    else
    {
        Client = ReadNewClient();
        AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
        return true;
    }
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number: " << Client.AccNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.Balance;
    cout << endl;
}

void PrintClientRecord(sClient Client)
{
    cout << left << "|" << setw(16) << Client.AccNumber
        << "|" << setw(10) << Client.PinCode << "|"
        << setw(50) << Client.Name << "|" << setw(20)
        << Client.Phone << "|" << setw(10) << Client.Balance << endl;
}

string ReadClientAccountNumber()
{
    string AccountNumber;
    cout << "Enter Account Number: ";
    cin >> AccountNumber;

    return AccountNumber;
}

void ReadClientUpdatedData(sClient& Record)
{
    cout << "Enter PinCode? ";
    getline(cin >> ws, Record.PinCode);
    cout << "Enter name? ";
    getline(cin, Record.Name);
    cout << "Enter Phone? ";
    getline(cin, Record.Phone);
    cout << "Enter account Balance? ";
    cin >> Record.Balance;

}

bool UpdateClientInfo(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccNumber == AccountNumber)
        {
            ReadClientUpdatedData(C);
            break;
        }
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector <sClient> SaveClientsDataToFile(string FileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked fordelete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}





void ShowAllClients(vector<sClient> vClients)
{

    cout << "\t\t\t\t\t" << "ClientList(" << vClients.size() << ") Client(s)." << endl;
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    cout << left << "|" << setw(16) << "Account Number" << "|" << setw(10)
        << "Pin Code" << "|" << setw(50) << "Client Name" << "|"
        << setw(20) << "Phone" << "|" << setw(10) << "Balance\n";
    cout << "---------------------------------------------------------------------------------------------------------------\n";

    for (sClient Client : vClients)
    {
        PrintClientRecord(Client);
    }
    cout << "---------------------------------------------------------------------------------------------------------------\n";

}

void ReadCLients(vector<sClient> vClients)
{
    char ReadMore = 'y';

    while (toupper(ReadMore) == 'Y')
    {


        cout << "Adding new CLient:\n\n";

        string AccountNumber = ReadClientAccountNumber();
        if (AddNewCLient(AccountNumber, vClients))
        {
            cout << "client added successfully.";
        }
        cout << " Do you want to add more clients? y/n: ";
        cin >> ReadMore;
    }
}

bool FindClientByAccountNumber(string AccountNumber ,vector<sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {
        if (C.AccNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber,vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber,
                vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            //Refresh Clients
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!" << endl;
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients,
        Client))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            UpdateClientInfo(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!" << endl;
        return false;
    }
}

bool Deposit(vector<sClient> &vClients, string AccountNumber, double Amount)
{
    for (sClient& C : vClients)
    {
        if (C.AccNumber == AccountNumber)
        {
            C.Balance += Amount;
            return true;
        }
    }
    return false;
}





stUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData;
    vUserData = Split(Line, Seperator);
    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
    string stUserRecord = "";
    stUserRecord += User.UserName + Seperator;
    stUserRecord += User.Password + Seperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

vector <stUser> LoadUsersDataFromFile(string FileName)
{
    vector <stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        stUser User;
        while (getline(MyFile, Line))
        {
            User = ConvertUserLinetoRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;
    if (MyFile.is_open())
    {
        for (stUser U : vUsers)
        {
            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked fordelete.
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUsers;
}

stUser ReadUserCredentials()
{
    stUser User;
    cout << "Enter UserName: ";
    cin >> User.UserName;
    cout << "Enter Password: ";
    cin >> User.Password;

    return User;
}

bool Login(string UsersFileName, stUser& User)
{
    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    User = ReadUserCredentials();

    for (stUser U : vUsers)
    {
        if (U.UserName == User.UserName && U.Password == User.Password)
        {
            User = U;
            return true;
        }
    }

    return false;
}



void PrintUserRecord(stUser User)
{
    cout << left << "|" << setw(16) << User.UserName
        << "|" << setw(10) << User.Password << "|"
        << setw(50) << User.Permissions << endl;
}

void ShowAllUsers(vector<stUser> vUsers)
{
    cout << "\t\t\t\t\t" << "Users List(" << vUsers.size() << ") User(s)." << endl;
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    cout << left << "|" << setw(16) << "User Name" << "|" << setw(10)
        << "Password" << "|" << setw(15) << "Permissions\n";
    cout << "---------------------------------------------------------------------------------------------------------------\n";

    for (stUser User : vUsers)
    {
        PrintUserRecord(User);
    }
    cout << "---------------------------------------------------------------------------------------------------------------\n";

}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "\nUser Name : " << User.UserName;
    cout << "\nPassword : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << endl;
}

int ReadUserPermissions()
{
    char fullaccess = ' ';
    int permissions = 0;

    cout << "Dou you want to give full access, y/n?";
    cin >> fullaccess;
    if (tolower(fullaccess) == 'y')
        return -1;
    else
    {
        cout << "do you want to give acess to:\n";
        char access = ' ';
        cout << "show client list? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 1;
        cout << "add new client? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 2;
        cout << "delete client? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 4;
        cout << "update client? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 8;
        cout << "find client? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 16;
        cout << "transactions? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 32;
        cout << "user management? y/n: ";
        cin >> access;
        if (tolower(access) == 'y')
            permissions = permissions | 64;

    }

    return permissions;
}

stUser ReadNewUser()
{
    stUser Record;

    cout << "Enter Passowrd? ";
    getline(cin >> ws, Record.Password);

    Record.Permissions = ReadUserPermissions();
    return Record;
}

bool AddNewUser(string User_Name, vector<stUser> vUsers)
{
    stUser User;
    if (FindUserByUserName(User_Name, vUsers, User))
    {
        cout << "The User already exist!" << endl;
        PrintUserCard(User);
        return false;
    }
    else
    {
        User = ReadNewUser();
        User.UserName = User_Name;
        AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
        return true;
    }
}

void ReadUsers(vector<stUser> vUsers)
{
    char ReadMore = 'y';

    while (toupper(ReadMore) == 'Y')
    {


        cout << "Adding new User:\n\n";

        string UserName = ReadUserName();
        if (AddNewUser(UserName, vUsers))
        {
            cout << "user added successfully.";
        }
        cout << " Do you want to add more users? y/n: ";
        cin >> ReadMore;
    }
}

bool FindUserByUserName(string UserName, vector<stUser> vUsers, stUser& User)
{

    for (stUser U : vUsers)
    {
        if (U.UserName == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool MarkUserForDeleteByUserName(string User_Name, vector <stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == User_Name)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteUserByUserName(string UserName, vector<stUser>& vUsers)
{
    if (UserName == "Admin")
    {
        cout << "You Cannot Delete this user." << endl;
        return false;
    }

    stUser User;
    char Answer = 'n';
    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            //Refresh Users
            vUsers = LoadUsersDataFromFile(UsersFileName);
            cout << "\n\nUser Deleted Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nUser with User Name (" << UserName << ") is Not Found!" << endl;
        return false;
    }
}

void ReadUserUpdatedData(stUser& Record)
{
    cout << "Enter Password? ";
    getline(cin >> ws, Record.Password);
    Record.Permissions = ReadUserPermissions();
}

bool UpdateUserInfo(string User_Name, vector <stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == User_Name)
        {
            ReadUserUpdatedData(U);
            break;
        }
    }
    return false;
}

bool UpdateUserByUserName(string UserName, vector<stUser>& vUsers)
{
    if (UserName == "Admin")
    {
        cout << "You Cannot Update this user." << endl;
        return false;
    }

    stUser User;
    char Answer = 'n';

    if (FindUserByUserName(UserName, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\n\nAre you sure you want update this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            UpdateUserInfo(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);

            cout << "\n\nUser Updated Successfully." << endl;
            return true;
        }
    }
    else
    {
        cout << "\nUser with User Name (" << UserName << ") is Not Found!" << endl;
        return false;
    }
}