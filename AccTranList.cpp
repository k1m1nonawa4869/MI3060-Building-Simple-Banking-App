#include <iostream>
#include <limits>
#include <cctype>
#include <string>
#include "AccountList.h"
#include "TransactionList.h"
#include "AccTranList.h"
#include "Actions.h" 
#include "Utils.h"


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
            if (aDest) { 
                aDest->balance -= tr.amount;
            }  // allow negative
            if (aOrig) {
                aOrig->balance += tr.amount;
            }
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
