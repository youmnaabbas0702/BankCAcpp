#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

enum enChoice{ShowAll = 1, Add, Delete, Update, Find, Transactions, Exit};
enum enTransactionChoice{enDeposit = 1, WithDraw, TotalBalances, MainMenu};

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccNumber;
    string PinCode;
    string Name;
    string Phone;
    double Balance;
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
    cout << "\t\t[7] Exit.\n";
    cout << "=======================================================\n";

}

enChoice ReadUserChoice()
{
    short Choice;
    cout << "Choose what do you want to do? [1 to 7]? ";
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

void PauseSystem()
{
    cout << "Press any key to go back to Main Menu...";
    system("pause>0");
}

void EndProgram()
{
    cout << "----------------------------------------\n";
    cout << "\t\tProgram Ends :-)\n";
    cout << "----------------------------------------\n";
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

        DepositAmount = (Choice == enDeposit) ? DepositAmount : -1 * DepositAmount;

        char PerformTransaction = 'n';
        cout << "Are you sure you want to perform this transaction? y/n: ";

        cin >> PerformTransaction;
        if (tolower(PerformTransaction) == 'y')
        {
            Deposit(vClients, AccountNumber, DepositAmount);

            vClients = SaveClientsDataToFile(ClientsFileName, vClients);

            if(Choice == enDeposit)
                cout << "Deposit";
            if(Choice == WithDraw)
                cout << "WithDraw";

            cout <<" Done Successfully." << endl;

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

void StartProgram()
{
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
            ShowAllClientsScreen();
            PauseSystem();
            break;
        case Add:
            system("cls");
            ShowAddNewClientScreen();
            PauseSystem();

            break;
        case Delete:
            system("cls");
            ShowDeleteClientScreen();
            PauseSystem();

            break;
        case Update:
            system("cls");
            ShowUpdateClientScreen();
            PauseSystem();

            break;
        case Find:
            system("cls");
            ShowFindClientScreen();
            PauseSystem();

            break;
        
        case Transactions:
            system("cls");
            PerformTransactions();

            break;
        case Exit:
            system("cls");
            EndProgram();
            break;
        default:
            break;
        }

    } while (UserChoice != enChoice::Exit);

    
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