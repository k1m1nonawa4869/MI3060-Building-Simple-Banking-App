// File: Actions.cpp
#include "AccountList.h"
#include "TransactionList.h"
#include "AccList.h"
#include "AccTranList.h"
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
