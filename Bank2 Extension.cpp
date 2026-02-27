#include<string>
#include<vector>
#include <iostream>
#include<iomanip>
#include <fstream>
using namespace std;

const string ClientsFileName = "Bank Clients.txt";
const string UsersFileName = "Users.txt";

enum enMainMenueList {
    enShowAllClients = 1, enAddClient = 2, enDeleteClient = 3,
    enUpdateClient = 4, enFindClient = 5,enTransaction=6,enManageUsers=7, enLogout= 8
};

enum enTransactionsMenue{
    enDeposit=1,enWthdraw=2,
    enTotalBalances=3,enMainMenue=4
};

enum enManageUsersMenue{
    enListUsers=1,enAddNewUsers=2,enDeleteUsers=3,
    enUpdateUsers=4,enFindUsers=5,enMainMenueScreen=6
};

enum enMainMenuePermissions{
    eAll=-1,PlistClient=1,pAddNewClient=2,pDeleteClient=4,
    pUpdateClient=8,pFindClient=16,pTranactions=32,pManageUsers=64
};

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance=0;
    bool MarkForDelete = false;
};

struct stUser
{
    string Username;
    string Password;
    int Permissions=0;
    bool MarkForDelete = false;
};

stUser CurrentUser;

void GoBackToManageUsersMenue(vector<stClient>& vClient, vector<stUser>& vUser);
void PerformTransactionsMenueOption(vector<stClient>& vClient, vector<stUser>& vUser);
void GoBackToMainMenue();
void GoBackToTransactionsMenue(vector<stClient>& vClient, vector<stUser>& vUser);
void ShowMainMenueScreen();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void Login();

vector<string> SplitString(string S1, string delim)
{
    vector<string>vString;

    size_t pos = 0;
    string sword;//define a string variable

    //use find() function to get the position of the delimiters
    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sword = S1.substr(0, pos);//store the word
        if (sword != "")
        {
            vString.push_back(sword);
        }
        S1.erase(0, pos + delim.length());//erase() until position and move to next word
    }
    if (S1 != "")
    {
        vString.push_back(S1);//it print last word of the string
    }
    return vString;
}

stClient ConvertLineToRecord(string Line, string sperator = "#//#")
{
    stClient Client;
    vector<string>vString;

    vString = SplitString(Line, sperator);

    if (vString.size() < 5)
        return Client;

    Client.AccountNumber = vString[0];
    Client.PinCode = vString[1];
    Client.Name = vString[2];
    Client.Phone = vString[3];
    Client.AccountBalance = stod(vString[4]);//cast string to double

    return Client;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
    vector<stClient>vClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return vClient;
}

stUser ConvertUserLineToRecord(string Line, string sperator = "#//#")
{
    stUser User;
    vector<string>vUserData;

    vUserData = SplitString(Line, sperator);

    if (vUserData.size() < 3)
        return User;

    User.Username = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

vector<stUser> LoadUserDataFromFile(string FileName)
{
    vector<stUser>vUser;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            vUser.push_back(User);
        }
        MyFile.close();
    }
    return vUser;
}

void ShowAccessDeniedMessage()
{
    cout << "\n---------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Contact your Admin.";
    cout << "\n---------------------------------------\n";
    GoBackToMainMenue();
}

void PrintClientRecord(stClient Client)
{
    cout << "| " << setw(18) << left << Client.AccountNumber;
    cout << "| " << setw(14) << left << Client.PinCode;
    cout << "| " << setw(33) << left << Client.Name;
    cout << "| " << setw(17) << left << Client.Phone;
    printf("| %.f", Client.AccountBalance);
}

void ShowAllClientScreen()
{
    vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client (s).";
    cout << "\n========================================================";
    cout << "========================================================\n";

    cout << "| " << left << setw(18) << "Account Number";
    cout << "| " << left << setw(14) << "Pin Code";
    cout << "| " << left << setw(33) << "Client Name";
    cout << "| " << left << setw(17) << "Phone";
    cout << "| " << left << setw(15) << "Balance";

    cout << "\n========================================================";
    cout << "========================================================\n";

    if (vClients.size() == 0)
    {
        cout << "\n\t\t\tNo Clients Available In the System!\n";
    }
    else
        for (stClient &Client : vClients)
        {
            PrintClientRecord(Client);
            cout << endl;
        }
    cout << "--------------------------------------------------";
    cout << "--------------------------------------------------------------\n";

    cout << "\n";
}

string ConvertRecordToLine(stClient Client, string sperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + sperator;
    stClientRecord += Client.PinCode + sperator;
    stClientRecord += Client.Name + sperator;
    stClientRecord += Client.Phone + sperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

string ConvertUserRecordToLine(stUser User, string sperator = "#//#")
{
    string stUserRecord = "";

    stUserRecord += User.Username + sperator;
    stUserRecord += User.Password + sperator;
    stUserRecord += to_string(User.Permissions);

    return stUserRecord;
}

void AddClientToFile(string FileName, string DataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << DataLine << endl;
        MyFile.close();
    }
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<stClient>vClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}

bool UserExistsByUsername(string Username, string FileName)
{
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            if (User.Username==Username)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

stClient ReadClientData()
{
    stClient Client;

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Account Number : ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number : ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "--------------------------------------------------------------\n";

    cout << "Enter PinCode        : ";
    getline(cin >> ws, Client.PinCode);

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Name           : ";
    getline(cin, Client.Name);

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Phone          : ";
    getline(cin, Client.Phone);

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    cout << "--------------------------------------------------------------\n";

    return Client;
}

void AddNewClient()
{
    cout << "================================================================================\n";
    cout << "\n\t\t\tShow Add New Client Screen\n\n";
    cout << "================================================================================\n";

    stClient Client = ReadClientData();
    AddClientToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void ShowAddNewClientsScreen(vector<stClient>& vClient)
{
    char AddMore = 'Y';

    do
    {
        system("cls");

        AddNewClient();
        vClient = LoadClientsDataFromFile(ClientsFileName);

        cout << "\nClient Added Successfully !\nDo you want to add more clients? Y/N ? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void PrintClientCard(stClient& Client)
{
    cout << "\n==============================================================\n";
    cout << "\n\tThe following are the client details : \n";
    cout << "\n==============================================================\n";

    cout << "# Account number : " << Client.AccountNumber;
    cout << "\n--------------------------------------------------------------\n";
    cout << "# Pin Code       : " << Client.PinCode;
    cout << "\n--------------------------------------------------------------\n";
    cout << "# Client Name    : " << Client.Name;
    cout << "\n--------------------------------------------------------------\n";
    cout << "# Phone          : " << Client.Phone;
    cout << "\n--------------------------------------------------------------\n";
    printf("# Account Balance: %.f", Client.AccountBalance);
    cout << "\n--------------------------------------------------------------\n";
}

bool FindClientDataByAccountNumber(string AccountNumber, vector<stClient>& vClient, stClient& Client)
{
    for (const stClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindUserDataByUsername(string Username, vector<stUser>& vUsers, stUser& User)
{
    for (const stUser& U : vUsers)
    {
        if (U.Username == Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, stUser& User)
{
    vector<stUser>vUsers = LoadUserDataFromFile(UsersFileName);
    for (const stUser U : vUsers)
    {
        if (U.Username == Username&&U.Password==Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    for (stClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<stClient>SaveClientDataToFile(string FileName, vector<stClient>& vClient)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overWrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClient C : vClient)
        {
            if (C.MarkForDelete == false) {
                //we only write records that are not marked for delete

                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    stClient Client;
    char Answer = 'n';

    if (FindClientDataByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre sure you want delete this client ? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
            SaveClientDataToFile(ClientsFileName, vClient);

            //refresh Clints
            vClient = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Succefully . \n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found ! ";
        return false;
    }
    return false;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber ? ";
    cin >> AccountNumber;

    return AccountNumber;
}

void ShowDeleteClientScreen(vector<stClient>& vClient)
{
    cout << "================================================================================\n";
    cout << "\n\t\t\t\tShow Delete Screen\n\n";
    cout << "================================================================================\n";

    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClient);
}

void DeleteClients(vector<stClient>& vClient)
{
    stClient Client;
    char DeleteMore = 'Y';

    do
    {
        ShowDeleteClientScreen(vClient);
        cout << "\n\nDo you want to delete another client? Y/N ? ";
        cin >> DeleteMore;
    } while (toupper(DeleteMore) == 'Y');
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient>& vClient, stClient& Client)
{
    if (FindClientDataByAccountNumber(AccountNumber, vClient, Client))
    {
        return true;
    }
    else
        return false;
}

void ShowFindClientScreen(vector<stClient>& vClient)
{
    cout << "=============================================================================\n";
    cout << "\n\t\t\t\tShow Find Screen\n\n";
    cout << "=============================================================================\n";

    stClient Client;

    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found ! ";
}

void FindClients(vector<stClient>& vClient)
{
    char FindMore = 'Y';

    do
    {
        system("cls");
        ShowFindClientScreen(vClient);

        cout << "\n\nDo you want to find another client? Y/N ? ";
        cin >> FindMore;
    } while (toupper(FindMore) == 'Y');
}

stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n--------------------------------------------------------------\n";

    cout << "Enter PinCode        : ";
    getline(cin >> ws, Client.PinCode);

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Name           : ";
    getline(cin, Client.Name);

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Phone          : ";
    getline(cin, Client.Phone);

    cout << "--------------------------------------------------------------\n";

    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    cout << "--------------------------------------------------------------\n";

    return Client;
}

int ReadPermissionToSet()
{
    int Permission = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n";

    cout << "\nShow Client List?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::PlistClient;
    }

    cout << "\nAdd New Client?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::pAddNewClient;
    }
    cout << "\nDelete Client?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::pDeleteClient;
    }
    cout << "\nUpdate Client?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::pUpdateClient;
    }
    cout << "\nFind Client?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::pFindClient;
    }

    cout << "\nTransactions?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::pTranactions;
    }

    cout << "\nManage Users?y/n ? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permission += enMainMenuePermissions::pManageUsers;
    }
    return Permission;
}

stUser ChangeUserRecord(string Username)
{
    stUser User;

    User.Username = Username;

    cout << "\nEnter Password : ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionToSet();

    return User;
}

bool MarkClientForDeleteByUsername(string Username, vector<stUser>& vUser)
{
    for (stUser& U : vUser)
    {
        if (U.Username==Username)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}


vector<stUser>SaveUserDataToFile(string FileName, vector<stUser>& vUser)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overWrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (stUser U : vUser)
        {
            if (U.MarkForDelete == false) {
                //we only write records that are not marked for delete

                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUser;
}


bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    stClient Client;
    char Answer = 'n';

    if (FindClientDataByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nAre sure you want update this client ? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (stClient& C : vClient)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientDataToFile(ClientsFileName, vClient);
            vClient = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Updated Succefully . \n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found ! ";
        return false;
    }

    return false;
}

void ShowUpdateClientScreen(vector<stClient>&vClient)
{
    cout << "==============================================================================\n";
    cout << "\n\t\t\t\tShow Update Screen\n\n";
    cout << "==============================================================================\n";

    string AccountNumber = ReadClientAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClient);
}

bool DepositByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    stClient Client;
    double Amount;
    char Answer = 'Y';

    if (FindClientDataByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nPlease enter deposit amount ? ";
        cin >> Amount;

        cout << "\n\nAre you sure you want to perform this transaction ? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (stClient& C : vClient)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    Amount += C.AccountBalance;
                    C.AccountBalance = Amount;
                    break;
                }
            }

            SaveClientDataToFile(ClientsFileName, vClient);
            vClient = LoadClientsDataFromFile(ClientsFileName);

            printf("\nNew Balances become : %.f\n", Amount);

            return true;
        }
        else
            return false;
    }
    else 
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found ! ";
     
        return false;
    }
    return false;
}

void ShowDepositScreen(vector<stClient>&vClient)
{
    cout << "==============================================================================\n";
    cout << "\n\t\t\t\t Deposit Screen\n\n";
    cout << "==============================================================================\n";

    string AccountNumber = ReadClientAccountNumber();
   
   DepositByAccountNumber(AccountNumber, vClient);
}

bool WithdrawByAccountNumber(string AccountNumber, vector<stClient>& vClient)
{
    stClient Client;
    double Amount;
    char Answer = 'Y';
    if (FindClientDataByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

        cout << "\n\nPlease enter withdraw amount ? ";
        cin >> Amount;

        while (Amount > Client.AccountBalance)
        {
            printf("\nAmount Exceeds the balance, you can withdraw up to : %.f", Client.AccountBalance);
            cout << "\nPlease enter another amount ? ";
            cin >> Amount;
        }

            cout << "\n\nAre you sure you want to perform this transaction ? ";
            cin >> Answer;

            if (toupper(Answer) == 'Y')
            {
                for (stClient& C : vClient)
                {
                    if (C.AccountNumber == AccountNumber)
                    {
                        Amount = C.AccountBalance - Amount;
                        C.AccountBalance = Amount;
                        break;
                    }
                }

                SaveClientDataToFile(ClientsFileName, vClient);
                vClient = LoadClientsDataFromFile(ClientsFileName);

                printf("\nNew Balances become : %.f\n", Amount);

                return true;
            }
            else
      return false;
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found ! ";
        return false;
    }
    return false;
}

void ShowWithdrawScreen(vector<stClient>& vClient)
{
    cout << "==============================================================================\n";
    cout << "\n\t\t\t\t Withdraw Screen\n\n";
    cout << "==============================================================================\n";

    string AccountNumber = ReadClientAccountNumber();

    WithdrawByAccountNumber(AccountNumber, vClient);
}

void PrintBalanceRecord(stClient Client)
{
    cout << "| " << setw(20) << left << Client.AccountNumber;
    cout << "| " << setw(20) << left << Client.PinCode;
    cout << "| " << setw(38) << left << Client.Name;
    printf("| %.f", Client.AccountBalance);
}

void ShowTotalBalances(vector<stClient>& vClient)
{
    stClient Client;
    double Total = 0;

    cout << "\n\t\t\t\t\tBalances List (" << vClient.size() << ") Client (s).";
    cout << "\n========================================================";
    cout << "========================================================\n";

    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(20) << "Pin Code";
    cout << "| " << left << setw(38) << "Client Name";
    cout << "| " << left << setw(15) << "Balance";

    cout << "\n========================================================";
    cout << "========================================================\n";

    if (vClient.size() == 0)
    {
        cout << "\n\t\t\tNo Clients Available In the System!\n";
    }
    else
        for (stClient Client : vClient)
        {
            PrintBalanceRecord(Client);
            Total += Client.AccountBalance;
            cout << endl;
        }
    cout << "--------------------------------------------------";
    cout << "--------------------------------------------------------------\n";
    printf("\t\t\t\t\tTotal Balances : %.f\n", Total);

    cout << "--------------------------------------------------";
    cout << "--------------------------------------------------------------\n";

    cout << "\n";
}

void PrintUsersRecord(stUser User)
{
    cout << "| " << setw(25) << left << User.Username;
    cout << "| " << setw(25) << left << User.Password;
    cout << "| " << setw(40) << left << User.Permissions; 
}

void ShowAllUsersScreen()
{
    vector<stUser> vUser = LoadUserDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tList Users List (" << vUser.size() << ") Client (s).";
    cout << "\n========================================================";
    cout << "===========================================\n";

    cout << "| " << left << setw(25) << "Account Number";
    cout << "| " << left << setw(25) << "Password";
    cout << "| " << left << setw(40) << "Permissions";

    cout << "\n========================================================";
    cout << "===========================================\n";

    for (stUser User : vUser)
    {
        PrintUsersRecord(User);
        cout << endl;
    }
    cout << "---------------------------------------------------------";
    cout << "------------------------------------------";
    cout << "\n";
}

stUser ReadNewUser()
{
    stUser User;

    cout << "--------------------------------------------------------\n";
    cout << "\n\tAdd New User Screen\n";
    cout << "\n--------------------------------------------------------\n";

    cout << "\nEnter Username : ";
    getline(cin >> ws, User.Username);
   
    while (UserExistsByUsername(User.Username, UsersFileName))
    {
        cout << "\nUser with [" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }

    cout << "\nEnter Password : ";
    getline(cin >> ws, User.Password);
   
    User.Permissions = ReadPermissionToSet();

    return User;
}

void AddNewUser()
{
    stUser User = ReadNewUser();
    AddClientToFile(UsersFileName, ConvertUserRecordToLine(User));
    LoadUserDataFromFile(UsersFileName);
}

void ShowAddNewUsersScreen(vector<stUser>& vUser)
{
    char AddMore = 'Y';

    do
    {
        system("cls");
        AddNewUser();
        cout << "\nUser Added Successfully !\nDo you want to add more Users? Y/N ? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void PrintUserCard(stUser& User)
{
    cout << "\n==============================================================\n";
    cout << "\n\tThe following are the user details : \n";
    cout << "\n==============================================================\n";
    cout << "# Username   : " << User.Username;
    cout << "\n--------------------------------------------------------------\n";
    cout << "# Password   : " << User.Password;
    cout << "\n--------------------------------------------------------------\n";
    cout << "# Permissions: " << User.Permissions;
    cout << "\n--------------------------------------------------------------\n";
}

bool DeleteUserByUsername(string Username, vector<stUser>& vUser)
{
    if (Username == "Admin")
    {
        cout << "\n\nYou cannot Delete This User. ";
        return false;
    }

    stUser User;
    char Answer = 'n';

    if (FindUserDataByUsername(Username, vUser, User))
    {
        PrintUserCard(User);

        cout << "\n\nAre sure you want delete this User ? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
           MarkClientForDeleteByUsername(Username, vUser);
            SaveUserDataToFile(UsersFileName, vUser);

            //refresh Clints
            vUser = LoadUserDataFromFile(UsersFileName);

            cout << "\n\nUser Deleted Succefully . \n";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found ! ";
        return false;
    }
    return false;
}

bool UpdateUserByUsername(string Username, vector<stUser>& vUser)
{
    stUser User;
    char Answer = 'n';

    if (FindUserDataByUsername(Username, vUser, User))
    {
        PrintUserCard(User);

        cout << "\n\nAre sure you want update this user ? y/n ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            for (stUser& U : vUser)
            {
                if (U.Username==Username)
                {
                    U = ChangeUserRecord(Username);
                    break;
                }
            }

            SaveUserDataToFile(UsersFileName, vUser);
            vUser = LoadUserDataFromFile(UsersFileName);

            cout << "\n\nUser Updated Succefully . \n";
            return true;
        }
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found ! ";
        return false;
    }
    return false;
}

string ReadUsername()
{
    string Username = "";

    cout << "\nPlease enter Username ? ";
    cin >> Username;
    return Username;
}

void ShowListUsersScreen()
{
    ShowAllUsersScreen();
}

void ShowAddNewUsersScreen()
{
    cout << "\n==============================================================\n";
    cout << "\t\t  Add New User Screen";
    cout << "\n==============================================================\n";

    AddNewUser();
}

void ShowDeleteUsersScreen()
{
    cout << "\n==============================================================\n";
    cout << "\t\t  Delete User Screen";
    cout << "\n==============================================================\n";

    vector<stUser>vUser = LoadUserDataFromFile(UsersFileName);

    string Username = ReadUsername();
    DeleteUserByUsername(Username, vUser);
}

void ShowUpdateUsersScreen()
{
    cout << "\n==============================================================\n";
    cout << "\t\t  Update User Screen";
    cout << "\n==============================================================\n";

    vector<stUser>vUser = LoadUserDataFromFile(UsersFileName);

    string Username = ReadUsername();
    UpdateUserByUsername(Username, vUser);
}

void ShowAddNewClientScreen()
{
    cout << "\n==============================================================\n";
    cout << "\t\tAdd New Clients Screen";
    cout << "\n==============================================================\n";

    AddNewClient();
}

void ShowFindClientScreen()
{
    cout << "\n==============================================================\n";
    cout << "\t\tFind Client Screen";
    cout << "\n==============================================================\n";

    vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);
    stClient Client;
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

    }
    else
        cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}

void ShowFindUserScreen()
{
    cout << "\n==============================================================\n";
    cout << "\t\tFind User Screen";
    cout << "\n==============================================================\n";

    vector<stUser>vUsers = LoadUserDataFromFile(UsersFileName);
    stUser User;
    string Username = ReadUsername();

    if (FindUserDataByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
    }
    else
        cout << "\nUser with Username [" << Username << "] is not found!";
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
        return true;
    
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
    return false;
}

void PerformManageUsersMenue(vector<stClient>& vClient,vector<stUser>& vUser)
{
    short Choose;

    cout << "\nChoose what do you want to do ? [1 to 6] ?   ";
    cin >> Choose;

    switch (Choose)
    {
    case enManageUsersMenue::enListUsers:
        system("cls");
        ShowAllUsersScreen();
        GoBackToManageUsersMenue(vClient,vUser);
        break;

    case enManageUsersMenue::enAddNewUsers:
        system("cls");
        ShowAddNewUsersScreen(vUser);
        GoBackToManageUsersMenue(vClient,vUser);
        break;

    case enManageUsersMenue::enDeleteUsers:
        system("cls");
        ShowDeleteUsersScreen();
        GoBackToManageUsersMenue(vClient,vUser);
        break;

    case enManageUsersMenue::enUpdateUsers:
        system("cls");
        ShowUpdateUsersScreen();
        GoBackToManageUsersMenue(vClient, vUser);
        break;

    case enManageUsersMenue::enFindUsers:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue(vClient,vUser);
        break;

    case enManageUsersMenue::enMainMenueScreen:
        system("cls");
        ShowMainMenueScreen();
        GoBackToMainMenue();
        break;

    default:
    {
        cout << "\nSorry, this choice does not exist.\n";
        PerformManageUsersMenue(vClient,vUser);
    }
    }
}

void ShowManageUsersScreen(vector<stClient>& vClient, vector<stUser>& vUser)
{
    cout << "\n========================================================";
    cout << "\n\t\tManage Users Menue Screen\n";
    cout << "========================================================\n";
    cout << " [1] List Users.\n";
    cout << " [2] Add new Users.\n";
    cout << " [3] Delete Users.\n";
    cout << " [4] Update users.\n";
    cout << " [5] Find Users.\n";
    cout << " [6] Main Menue.\n";
    cout << "========================================================\n";

    PerformManageUsersMenue(vClient,vUser);
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFailed = false;

    string Username, Password;
    do
    {
        system("cls");

        cout << "\n==============================================================\n";
        cout << "\n\t\t   Login Screen\n";
        cout << "\n==============================================================\n";

        if (LoginFailed)
        {
            cout << "\nInvalid Username/Password!\n";
        }
        cout << "\nEnter Username ? ";
        cin >> Username;
        cout << "\nEnter Password ? ";
        cin >> Password;

        LoginFailed = !LoadUserInfo(Username, Password);
    } while (LoginFailed);

    system("cls");
        ShowMainMenueScreen();
}

void ShowTransactionsMenueScreen(vector<stClient>& vClient, vector<stUser>& vUser)
{
    cout << "================================================================\n";
    cout << "\t\t\tTransactions Menue Screen\n";
    cout << "================================================================\n";
    cout << "\t\t[1] Deposit.\n";
    cout << "\t\t[2] Withdraw.\n";
    cout << "\t\t[3] Total Balances.\n";
    cout << "\t\t[4] Main Menue.\n";
    cout << "================================================================\n\n";

    PerformTransactionsMenueOption(vClient,vUser);
}

void PerformMainMenueOption(vector<stClient>& vClient, vector<stUser>& vUser)
{
    short Choose;

    cout << "\nChoose what do you want to do ? [1 to 8] ?   ";
    cin >> Choose;

    switch (Choose)
    {

    case enMainMenueList::enShowAllClients:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::PlistClient))
        {
           ShowAllClientScreen();
            GoBackToMainMenue();
            break;
        }

        else
            ShowAccessDeniedMessage();
        break;
        
    case enMainMenueList::enAddClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
        {
            ShowAddNewClientsScreen(vClient);
            GoBackToMainMenue();
            break;
        }

        else
            ShowAccessDeniedMessage();
        break;

    case enMainMenueList::enDeleteClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
        {
            DeleteClients(vClient);
            GoBackToMainMenue();
            break;
        }
        else
            ShowAccessDeniedMessage();
        break;

    case enMainMenueList::enUpdateClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::pUpdateClient))
        {
            ShowUpdateClientScreen(vClient);
            GoBackToMainMenue();
            break;
        }
        else
            ShowAccessDeniedMessage();
        break;

    case enMainMenueList::enFindClient:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::pFindClient))
        {
           FindClients(vClient);
            GoBackToMainMenue();
            break;
        }
        else
            ShowAccessDeniedMessage();
        break;

    case enMainMenueList::enTransaction:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::pTranactions))
        {
            ShowTransactionsMenueScreen(vClient,vUser);
            break;
        }
        else
            ShowAccessDeniedMessage();
        break;

    case enMainMenueList::enManageUsers:
        system("cls");
        if (CheckAccessPermission(enMainMenuePermissions::pManageUsers))
        {
            ShowManageUsersScreen(vClient,vUser);
            break;
        }
        else
            ShowAccessDeniedMessage();
        break;

    case enMainMenueList::enLogout:
        system("cls");
        Login();
        break;

    default:
    {
        cout << "\nSorry, this choice does not exist.\n";
        PerformMainMenueOption(vClient,vUser);
    }
    }
}

void ShowMainMenueScreen()
{
    vector<stClient> vClient = LoadClientsDataFromFile(ClientsFileName);
    vector<stUser> vUser = LoadUserDataFromFile(UsersFileName);

    cout << "================================================================\n";
    cout << "\t\t\tMain Menue Screen\n";
    cout << "================================================================\n";
    cout << "\t\t[1] Show All Clients List.\n";
    cout << "\t\t[2] Add New Client.\n";
    cout << "\t\t[3] Delete Client.\n";
    cout << "\t\t[4] Update Client Info.\n";
    cout << "\t\t[5] Find Client.\n";
    cout << "\t\t[6] Transactions.\n";
    cout << "\t\t[7] Manage Users.\n";
    cout << "\t\t[8] Logout.\n";
    cout << "================================================================\n\n";

    PerformMainMenueOption(vClient,vUser);
}

void PerformTransactionsMenueOption(vector<stClient>& vClient, vector<stUser>& vUser)
{
    int Choose;

    cout << "\nChoose what do you want to do ? [1 to 4] ?   ";
    cin >> Choose;

    switch (Choose)
    {
    case enTransactionsMenue::enDeposit:
        system("cls");
        ShowDepositScreen(vClient);
        GoBackToTransactionsMenue(vClient,vUser);
        break;

    case enTransactionsMenue::enWthdraw:
        system("cls");
        ShowWithdrawScreen(vClient);
        GoBackToTransactionsMenue(vClient,vUser);
        break;

    case enTransactionsMenue::enTotalBalances:
        system("cls");
        ShowTotalBalances(vClient);
        GoBackToTransactionsMenue(vClient,vUser);
        break;

    case enTransactionsMenue::enMainMenue:
        system("cls");
        ShowMainMenueScreen();
        break;

    default:
    {
        cout << "\nSorry, this choice does not exist.\n";
        PerformTransactionsMenueOption(vClient,vUser);
    }
    }
}

void GoBackToManageUsersMenue(vector<stClient>&vClient,vector<stUser>& vUser)
{
    cout << "\n\nPress any Key go back to Users menue ...";
    system("pause>0");
    system("cls");
    ShowManageUsersScreen(vClient,vUser);
}

void GoBackToTransactionsMenue(vector<stClient>& vClient, vector<stUser>& vUser)
{
    cout << "\n\nPress any Key go back to transactions menue ...";
    system("pause>0");
    system("cls");
    ShowTransactionsMenueScreen(vClient,vUser);
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any Key go back to main menue ...";
    system("pause>0");
    system("cls");
    ShowMainMenueScreen();
}


int main()
{
    Login();

    system("pause>0");
}

