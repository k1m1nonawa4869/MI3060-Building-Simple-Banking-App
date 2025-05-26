// File: Actions.cpp
#include "AccountList.h"
#include "TransactionList.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <string>

// Utility to safely read an integer ID, retrying on invalid input
int restrictIDInt()
{
    int value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid character. Please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

// Print main menu
void printMenu()
{
    std::cout << "\n=== Bank Management ===\n"
              << "1. Add Account\n"
              << "2. Display Account Info\n"
              << "3. Update Account Info\n"
              << "4. Lock/Unlock Account\n"
              << "5. Transaction (Deposit/Withdraw)\n"
              << "6. Transfer Money\n"
              << "7. Transaction History\n"
              << "8. Undo Last Transaction\n"
              << "9. Delete Account\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

// Return to menu prompt
void returnMenu()
{
    std::cout << "\nEnter 0 to return to main menu: ";
    int x;
    
    while (std::cin >> x && x != 0)
    {
        std::cout << "Invalid input, please enter 0: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 1. Add Account
void addAccountAction(AccountList &list)
{
    char cont = 'y';
    while (cont == 'y')
    {
        int id;
        std::string name;
        std::string pin;
        
        int exit = 0;
        std::cout << "Enter unique account ID: "; id = restrictIDInt();
        if (id <= 0) 
        {
            std::cout << "ID must be a positive integer. Program stop." << std::endl;
            exit = 1;
        }
        if (list.findById(id)) 
        {
            std::cout << "That ID already exists. Program stop." << std::endl;
            exit = 1;
        }

        if (exit == 0) 
        {
            std::cout << "Enter account holder name: ";
            std::getline(std::cin, name);

            std::cout << "Set account PIN: ";
            std::cin >> pin;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            list.addAccount(BankAccount(id, name, pin));
        }

        std::cout << "Add another? (y/n): "; 
        std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 2. Display Account Info
void displayAccountAction(AccountList &list)
{
    while (true)
    {
        int id;
        std::string pin;

        std::cout << "Enter your account ID (-1 for admin): "; id = restrictIDInt();
        std::cout << "Enter your PIN (-1 for admin): "; std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (id == -1 && pin == "-1")
        {
            list.printAllAdmin();
            break;
        }

        int exit = 0; char cont = 'y';
        BankAccount *acct = list.findById(id);
        if (!acct)
        {
            std::cout << "Incorrect AccountID or Password." << std::endl;
            std::cout << "Try again? (y/n) ";
            std::cin >> cont; cont = std::tolower(cont);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cont == 'y')
            {
                continue;
            }
            break;
        }
        if (!acct->verifyPin(pin))
        {
            std::cout << "Incorrect AccountID or Password." << std::endl;
            std::cout << "Try again? (y/n)";
            std::cin >> cont; cont = std::tolower(cont);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cont == 'y')
            {
                continue;
            }
            break;
        }
        list.printUserNode(*acct);
        break;
    }
}

// 3. Update Account Info
void updateAccountAction(AccountList &list)
{
    char cont = 'y';
    while (cont == 'y')
    {
        int id; std::string pin;
        std::cout << "Enter account ID to update: "; id = restrictIDInt();
        std::cout << "Enter PIN: "; std::cin >> pin;

        BankAccount *acct = list.findById(id);
        int exit = 0;
        if (!acct)
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if (!(acct->verifyPin(pin)))
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }

        if (exit == 0)
        {
            int newId;
            std::string newName;
            std::string newPin;

            while (true)
            {
                std::cout << "Enter new positive ID (press -1 to keep the ID): ";
                std::cin >> newId;
                if (newId == -1)
                {
                    break;
                }
                if (list.findById(newId))
                    break;
                std::cout << "ID exists. Try again." << std::endl;
            }
            acct->id = newId;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter new name (blank to keep): ";
            std::getline(std::cin, newName);
            if (!newName.empty())
                acct->name = newName;

            char c;
            std::cout << "Change PIN? (y/n): ";
            std::cin >> c; c = std::tolower(c);
            if (c == 'y')
            {
                std::cout << "Enter new PIN: ";
                std::cin >> pin;
                acct->setPin(pin);
            }
        }

        std::cout << "Update another? (y/n): ";
        std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 4. Lock/Unlock Account
void lockUnlockAction(AccountList &list)
{
    while (true)
    {
        int id;
        std::string pin;

        std::cout << "Enter your account ID: "; id = restrictIDInt();
        std::cout << "Enter your PIN: "; std::cin >> pin;

        BankAccount *acct = list.findById(id);
        char cont = 'y';
        if (!acct)
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            std::cout << "Try again? (y/n) ";
            std::cin >> cont; cont = std::tolower(cont);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cont == 'y')
            {
                continue;
            }
            break;
        }
        if (!(acct->verifyPin(pin)))
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            std::cout << "Try again? (y/n) ";
            std::cin >> cont; cont = std::tolower(cont);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cont == 'y')
            {
                continue;
            }
            break;
        }

        std::cout << "Status: " << (acct->isLocked() ? "Locked" : "Unlocked") << std::endl;
            
        char c;
        std::cout << "Flip status? (y/n): "; 
        std::cin >> c; c = std::tolower(c);
            
        if (c == 'y')
        {
            acct->setLocked(!acct->isLocked());
            std::cout << "Now " << (acct->isLocked() ? "Locked" : "Unlocked") << std::endl;
        }

        break;
    }
}

// 5. Transaction (Deposit/Withdraw)
void transactionAction(AccountList &list, TransactionList &tlist)
{
    char cont = 'y';
    while (cont == 'y')
    {
        int id; std::string pin;
        std::cout << "Enter your AccountID: "; id = restrictIDInt();
        std::cout << "Enter your PIN: "; std::cin >> pin;

        BankAccount *acct = list.findById(id);
        int exit = 0;
        if (!acct)
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if (!(acct->verifyPin(pin)))
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if (acct->isLocked())
        {
            std::cout << "Your account has been locked. Unlock to transaction. Program stop." << std::endl;
            exit = 1;
        }

        if (exit == 0) 
        {
            char choice;
            std::cout << "Type d for Deposit,or w for Withdraw: ";
            std::cin >> choice; choice = std::tolower(choice);
            
            while (choice != 'd' && choice != 'w')
            {
                std::cin >> choice;
                choice = std::tolower(choice);
            }

            int amt;
            std::cout << "Amount: "; amt = restrictIDInt();
            
            while (amt <= 0)
            {
            std::cout << "Please enter a positive amount: ";
            amt = restrictIDInt();
            }

            //Deposit money
            if (choice == 'd')
            {
            acct->balance += amt;
            tlist.add(id, "Deposit", id, amt, acct->balance, acct->balance);
            std::cout << "Deposited Successful." << std::endl;
            }
            else //Withdraw money
            {
                if (amt > acct->balance)
                    {
                    std::cout << "Insufficient balance. Withdrawn request rejected." << std::endl;
                    }
                else
                {
                    acct->balance -= amt;
                    tlist.add(id, "Withdraw", id, amt, acct->balance, acct->balance);
                    std::cout << "Withdrawn Successful." << std::endl;
                }
            }
        }

        std::cout << "Continue action? (y/n): ";
        std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 6. Transfer Money
void transferMoneyAction(AccountList &list, TransactionList &tlist)
{
    char cont = 'y';
    while (cont == 'y')
    {
        int sid; std::string pin;
        std::cout << "Enter the Source ID: "; sid = restrictIDInt();
        std::cout << "Enter the Source ID PIN: "; std::cin >> pin;

        BankAccount *src = list.findById(sid);
        int exit = 0;
        if (!src)
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if ((exit == 0) && (!(src->verifyPin(pin))))
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if ((exit == 0) && (src->isLocked()))
        {
            std::cout << "Your account has been locked. Unlock to transaction. Program stop." << std::endl;
            exit = 1;
        }

        if (exit == 0) 
        {
            int did;
            while (true) 
            {
                std::cout << "Enter the Destination ID: "; did = restrictIDInt();
                
                BankAccount *dst = list.findById(did);
                if (!dst)
                {
                    std::cout << "Destination not found." << std::endl;
                    std::cout << "Try again? (y/n) ";
                    std::cin >> cont; cont = std::tolower(cont);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                    if (cont == 'y') {continue;}
                    break;
                }
                if (did == sid)
                {
                    std::cout << "DestID must be different from SourceID." << std::endl;
                    std::cout << "Try again? (y/n) ";
                    std::cin >> cont; cont = std::tolower(cont);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                    if (cont == 'y') {continue;}
                    break;
                }

                std::cout << "Destination account holder: " << dst->name << std::endl;
                std::cout << "Confirm to transfer money to this people (y/n): ";
                std::cin >> cont; cont = std::tolower(cont);
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (cont != 'y') {continue;}

                if (dst->isLocked())
                {
                    std::cout << "Destination locked thier account." << std::endl;
                    std::cout << "Try again? (y/n) ";
                    std::cin >> cont; cont = std::tolower(cont);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                    if (cont == 'y') {continue;}
                    break;
                }
        
                int amt;
                std::cout << "Amount: "; amt = restrictIDInt();
            
                while (amt <= 0)
                {
                    std::cout << "Please enter a positive amount: ";
                    amt = restrictIDInt();
                }

                if (amt > src->balance)
                {
                    std::cout << "Insufficient balance. Transfer request rejected." << std::endl;
                    
                    std::cout << "Try again? (y/n) ";
                    std::cin >> cont; cont = std::tolower(cont);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                    if (cont == 'y') {continue;}
                    break;
                }
                else
                {
                    src->balance -= amt;
                    dst->balance += amt;
                    tlist.add(sid, "Transfer", did, amt, src->balance, dst->balance);
                    std::cout << "Transferred Successful." << std::endl;
                    
                    std::cout << "Try again? (y/n) ";
                    std::cin >> cont; cont = std::tolower(cont);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                    if (cont == 'y') {continue;}
                    break;
                }
            }
        }

        std::cout << "Continue action? (y/n): ";
        std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 7. Transaction History
void displayHistoryAction(AccountList &list, TransactionList &tlist)
{
    while (true)
    {
        int id;
        std::string pin;

        std::cout << "Enter your account ID (-1 for admin): "; id = restrictIDInt();
        std::cout << "Enter your PIN (-1 for admin): "; std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (id == -1 && pin == "-1")
        {
            tlist.printAllAdmin();
            break;
        }

        int exit = 0; char cont = 'y';
        BankAccount *acct = list.findById(id);
        if (!acct)
        {
            std::cout << "Incorrect AccountID or Password." << std::endl;
            std::cout << "Try again? (y/n) ";
            std::cin >> cont; cont = std::tolower(cont);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cont == 'y')
            {
                continue;
            }
            break;
        }
        if (!acct->verifyPin(pin))
        {
            std::cout << "Incorrect AccountID or Password." << std::endl;
            std::cout << "Try again? (y/n)";
            std::cin >> cont; cont = std::tolower(cont);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cont == 'y')
            {
                continue;
            }
            break;
        }
        tlist.printUser(id);
        break;
    }
}

// 8. Undo Selected Between Two Accounts (require both admin and users identity)
void undoTransactionAction(AccountList& list, TransactionList& tlist) {
    //Identify
    int adminId, userId; std::string pin, adminPin;
    
    std::cout << "Enter admin ID: "; adminId = restrictIDInt();
    std::cout << "Enter admin password: "; std::cin >> adminPin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (adminId == -1 && adminPin == "-1")
    {
        std::cout << "Enter user ID: "; userId = restrictIDInt();
        std::cout << "Enter user password: "; std::cin >> pin;

        BankAccount *me = list.findById(userId);
        int exit = 0;
        if (!me)
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if (!(me->verifyPin(pin)))
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        /*if (me->isLocked())
        {
            std::cout << "Your account has been locked. Unlock to transaction. Program stop." << std::endl;
            exit = 1;
        }*/

        if (exit == 1) {
            return;
        }
    } else {
        std::cout << "Unauthorized.\n";
        return;
    }
    

    //Pick counterparty
    std::cout << "Enter counterparty ID: ";
    int destId = restrictIDInt();
    BankAccount* other = list.findById(destId);
    if (!other) { std::cout << "Account not found.\n"; returnMenu(); return; }
    std::cout << "Counterparty: " << other->name << "\n";

    //List matching transactions
    int total = tlist.printBetween(userId, destId);
    if (total == 0) {
        std::cout << "No transactions found.\n";
        return;
    }

    //Choose one to undo
    std::cout << "Select # to undo: ";
    int choice = restrictIDInt();
    if (choice < 1 || choice > total) {
        std::cout << "Invalid selection.\n";
        return;
    }

    //Remove & revert
    Transfer tr;
    if (!tlist.undoBetween(userId, destId, choice, tr)) {
        std::cout << "Undo failed.\n";
    } else {
        BankAccount* aOrig = list.findById(tr.originId);
        BankAccount* aDest = list.findById(tr.destinationId);
        if (tr.type == "Transfer") {
            if (aDest) aDest->balance -= tr.amount;  // allow negative
            if (aOrig) aOrig->balance += tr.amount;
        }
        else if (tr.type == "Deposit") {
            if (aOrig) aOrig->balance -= tr.amount;
        }
        else { // Withdraw
            if (aOrig) aOrig->balance += tr.amount;
        }
        std::cout << "Transaction undone.\n";
    }

}

// 9. Delete Account
void deleteAccountAction(AccountList &list, TransactionList &tlist)
{
    //Identify
    int adminId, userId; std::string pin, adminPin;
    
    std::cout << "Enter admin ID: "; adminId = restrictIDInt();
    std::cout << "Enter admin password: "; std::cin >> adminPin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (adminId == -1 && adminPin == "-1")
    {
        std::cout << "Enter user ID: "; userId = restrictIDInt();
        std::cout << "Enter user password: "; std::cin >> pin;

        BankAccount *me = list.findById(userId);
        int exit = 0;
        if (!me)
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        if (!(me->verifyPin(pin)))
        {
            std::cout << "Incorrect AccountID or Password. Program stop." << std::endl;
            exit = 1;
        }
        /*if (me->isLocked())
        {
            std::cout << "Your account has been locked. Unlock to transaction. Program stop." << std::endl;
            exit = 1;
        }*/

        if (exit == 1) {
            return;
        }
    } else {
        std::cout << "Unauthorized.\n";
        return;
    }

    //Remove account
    char cont = 'n';
    std::cout << "Do you really want to delete your account? (y/n) ";
    std::cin >> cont; cont = std::tolower(cont);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (cont != 'y') {return;}

    list.deleteById(userId);
    tlist.markDeleted(userId);
    std::cout << "Account " << userId << " deleted; related transactions marked." << std::endl;

    // Persist immediately
    list.save("accounts.txt");
    tlist.save("transactions.txt");
    return;
}

// Dispatch function remains unchanged
bool actionMenu(AccountList &list, TransactionList &tlist)
{
    printMenu();
    int choice;
    if (!(std::cin >> choice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (choice)
    {
    case 1:
        addAccountAction(list);
        returnMenu();
        break;
    case 2:
        displayAccountAction(list);
        returnMenu();
        break;
    case 3:
        updateAccountAction(list);
        returnMenu();
        break;
    case 4:
        lockUnlockAction(list);
        returnMenu();
        break;
    case 5:
        transactionAction(list, tlist);
        returnMenu();
        break;
    case 6:
        transferMoneyAction(list, tlist);
        returnMenu();
        break;
    case 7:
        displayHistoryAction(list, tlist);
        returnMenu();
        return true;
    case 8:
        undoTransactionAction(list, tlist);
        returnMenu();
        return true;
    case 9:
        deleteAccountAction(list, tlist);
        returnMenu();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice." << std::endl;
        returnMenu();
        return true;
    }
    return true;
}