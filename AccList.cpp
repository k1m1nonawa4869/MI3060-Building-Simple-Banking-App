#include <iostream>
#include <limits>
#include <cctype>
#include <string>
#include "AccList.h"
#include "Actions.h"
#include "Utils.h"

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
