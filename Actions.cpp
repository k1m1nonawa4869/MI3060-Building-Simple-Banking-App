// File: Actions.cpp
#include "AccountList.h"
#include "TransactionList.h"
#include <iostream>
#include <limits>
#include <cctype>
#include <string>

// Utility to safely read an integer ID, retrying on invalid input
int restrictIDInt() {
    int value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input; please enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

// Print main menu
void printMenu() {
    std::cout << "=== Bank Management ===\n"
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
void returnMenu() {
    std::cout << "\nEnter 0 to return to main menu: ";
    int x;
    while (std::cin >> x && x != 0) {
        std::cout << "Invalid input, please enter 0: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 1. Add Account
void addAccountAction(AccountList& list) {
    char cont = 'y';
    while (cont == 'y') {
        int id; std::string name; std::string pin;
        while (true) {
            std::cout << "Enter unique account ID: "; id = restrictIDInt();
            if (id <= 0) {
                std::cout << "ID must be a positive integer. Try again." << std::endl;
                continue;
            }
            if (!list.findById(id)) break;
            std::cout << "That ID already exists. Try again." << std::endl;
        }
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "Enter account holder name: "; std::getline(std::cin, name);
        
        std::cout << "Set account PIN: "; std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        list.addAccount(BankAccount(id, name, pin));\

        std::cout << "Add another? (y/n): "; std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 2. Display Account Info
void displayAccountAction(AccountList& list) {
    while (true) {
        int id; std::string pin;
        std::cout << "Enter your account ID (-1 admin): "; id = restrictIDInt();
        std::cout << "Enter your PIN: "; std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (id == -1 && pin == "-1") {
            list.printAllAdmin();
            break;
        }
        BankAccount* acct = list.findById(id);
        if (!acct) {
            std::cout << "Invalid ID. Try again." << std::endl;
            continue;
        }
        if (!acct->verifyPin(pin)) {
            std::cout << "Incorrect PIN. Try again." << std::endl;
            continue;
        }
        list.printUserNode(*acct);
        break;
    }
}

// 3. Update Account Info
void updateAccountAction(AccountList& list) {
    char cont = 'y';
    while (cont == 'y') {
        int id; std::cout << "Enter account ID to update: "; id = restrictIDInt();
        BankAccount* acct = list.findById(id);
        if (!acct) {
            std::cout << "Account not found." << std::endl;
        } else {
            std::string pin;
            while (true) {
                std::cout << "Enter PIN (or -1 cancel): "; std::cin >> pin;
                if (pin == "-1") { std::cout << "Cancelled." << std::endl; break; }
                if (acct->verifyPin(pin)) break;
                std::cout << "Incorrect PIN." << std::endl;
            }

            if (!acct->verifyPin(pin)) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            int newId; std::string newName;
            while (true) {
                std::cout << "Enter new ID (or same): "; std::cin >> newId;
                if (newId == acct->id || !list.findById(newId)) break;
                std::cout << "ID exists. Try again." << std::endl;
            }

            acct->id = newId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter new name (blank to keep): "; std::getline(std::cin, newName);

            if (!newName.empty()) acct->name = newName;
            char c;
            std::cout << "Change PIN? (y/n): "; std::cin >> c; c = std::tolower(c);
            if (c == 'y') {
                std::cout << "Enter new PIN: "; std::cin >> pin;
                acct->setPin(pin);
            }
        }
        std::cout << "Update another? (y/n): "; std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 4. Lock/Unlock Account
void lockUnlockAction(AccountList& list) {
    int id; std::string pin;
    std::cout << "Enter your account ID: "; id = restrictIDInt();
    std::cout << "Enter your PIN: "; std::cin >> pin;

    BankAccount* acct = list.findById(id);
    if (!acct || !acct->verifyPin(pin)) {
        std::cout << "Invalid credentials." << std::endl;
        returnMenu();
        return;
    }

    std::cout << "Status: " << (acct->isLocked()?"Locked":"Unlocked") << std::endl;
    char c;
    std::cout << "Flip status? (y/n): "; std::cin >> c; c = std::tolower(c);
    if (c == 'y') {
        acct->setLocked(!acct->isLocked());
        std::cout << "Now " << (acct->isLocked()?"Locked":"Unlocked") << std::endl;
    }
}

// 5. Transaction (Deposit/Withdraw)
void transactionAction(AccountList& list, TransactionList& tlist) {
    char cont = 'y';
    while (cont=='y') {
        int id; std::cout<<"ID: "; id = restrictIDInt();
        BankAccount* acct = list.findById(id);

        if (!acct) { std::cout<<"Not found."<<std::endl; continue; }
        if (acct->isLocked()) { std::cout<<"Account locked."<<std::endl; continue; }

        std::string pin; std::cout<<"Enter PIN (-1 cancel): "; std::cin>>pin;
        if (pin=="-1") continue;
        if (!acct->verifyPin(pin)) { std::cout<<"Wrong PIN."<<std::endl; continue; }
        
        char choice; std::cout<<"d=Deposit,w=Withdraw: "; std::cin>>choice; choice=std::tolower(choice);
        while(choice!='d'&&choice!='w') {std::cin>>choice; choice=std::tolower(choice);}        
        
        int amt;
        std::cout << "Amount: ";
        amt = restrictIDInt();
        while (amt <= 0) {
            std::cout << "Please enter a positive amount: ";
            amt = restrictIDInt();
        }

        if(choice=='d'){
            acct->balance+=amt;
            tlist.add(id,"Deposit",id,amt,acct->balance);
            std::cout<<"Deposited."<<std::endl;
        } 
        else {
            if(amt>acct->balance) std::cout<<"Insufficient."<<std::endl;
            else {acct->balance-=amt; tlist.add(id,"Withdraw",id,amt,acct->balance); std::cout<<"Withdrawn."<<std::endl;}
        }
        std::cout<<"Again? (y/n): ";std::cin>>cont;cont=std::tolower(cont);
    }
}

// 6. Transfer Money
void transferMoneyAction(AccountList& list, TransactionList& tlist) {
    char cont='y';
    while(cont=='y'){
        int sid; std::cout<<"Source ID: "; sid = restrictIDInt();

        BankAccount* src=list.findById(sid);
        if(!src){std::cout<<"Not found."<<std::endl;continue;}
        if(src->isLocked()){std::cout<<"Locked."<<std::endl;continue;}

        std::string pin; std::cout<<"Enter PIN (-1 cancel): "; std::cin>>pin;
        if(pin=="-1") continue;
        if(!src->verifyPin(pin)){std::cout<<"Wrong PIN."<<std::endl;continue;}

        int did; std::cout<<"Dest ID: "; did = restrictIDInt();
        BankAccount* dst=list.findById(did);
        if(!dst){std::cout<<"Dest not found."<<std::endl;continue;}
        if(dst->isLocked()){std::cout<<"Dest locked."<<std::endl;continue;}
        if(did == sid) {std::cout<<"DestID must be different from SourceID."<<std::endl;continue;}

        int amt;
        std::cout << "Amount: ";
        amt = restrictIDInt();
        while (amt <= 0) {
            std::cout << "Please enter a positive amount: ";
            amt = restrictIDInt();
        }

        if(amt>src->balance) std::cout<<"Insufficient."<<std::endl;
        else {src->balance-=amt; dst->balance+=amt; tlist.add(sid,"Transfer",did,amt,src->balance); std::cout<<"Transferred."<<std::endl;}
        
        std::cout<<"Again? (y/n): ";std::cin>>cont;cont=std::tolower(cont);
    }
}


// 7. Transaction History (admin only)
void displayHistoryAction(AccountList& list, TransactionList& tlist) {
    while (true) {
        int id;
        std::string pin;
        std::cout << "Enter account ID (-1 for admin): ";
        id = restrictIDInt();
        std::cout << "Enter your PIN: ";
        std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (id == -1 && pin == "-1") {
            tlist.printAllAdmin();
            break;
        }
        BankAccount* acct = list.findById(id);
        if (!acct) {
            std::cout << "Invalid ID. Please try again." << std::endl;
            continue;
        }
        if (!acct->verifyPin(pin)) {
            std::cout << "Incorrect PIN. Please try again." << std::endl;
            continue;
        }
        tlist.printUser(id);
        break;
    }
}
// 8. Undo Last Transaction (admin only)
void undoTransactionAction(AccountList& list, TransactionList& tlist) {
    int id;
    std::string pin;
    std::cout << "Enter admin ID: ";
    id = restrictIDInt();
    std::cout << "Enter admin PIN: ";
    std::cin >> pin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (id != -1 || pin != "-1") {
        std::cout << "Unauthorized. Only admin can undo." << std::endl;
        return;
    }
    Transfer tr;
    if (!tlist.popLast(tr)) {
        std::cout << "No transactions to undo." << std::endl;
        return;
    }
    if (tr.type == "Deposit") {
        BankAccount* a = list.findById(tr.originId);
        if (a->balance < tr.amount) {
            std::cout << "Undo failed. Account " << tr.originId << " has insufficient funds." << std::endl;
        } else {
            a->balance -= tr.amount;
            std::cout << "Undo Deposit: " << tr.amount << " removed from " << tr.originId << std::endl;
        }
    } else if (tr.type == "Withdraw") {
        BankAccount* a = list.findById(tr.originId);
        a->balance += tr.amount;
        std::cout << "Undo Withdraw: " << tr.amount << " returned to " << tr.originId << std::endl;
    } else if (tr.type == "Transfer") {
        BankAccount* src = list.findById(tr.originId);
        BankAccount* dst = list.findById(tr.destinationId);
        if (dst->balance < tr.amount) {
            std::cout << "Undo failed. Account " << tr.destinationId << " has insufficient funds." << std::endl;
        } else {
            dst->balance -= tr.amount;
            src->balance += tr.amount;
            std::cout << "Undo Transfer: " << tr.amount << " moved back from " << tr.destinationId << " to " << tr.originId << std::endl;
        }
    }
    std::cout << "Press 0 to return to menu." << std::endl;
}
// 9. Delete Account
void deleteAccountAction(AccountList& list, TransactionList& tlist) {
    int id;
    std::string pin;
    std::cout << "Enter admin ID: ";
    id = restrictIDInt();
    std::cout << "Enter admin PIN: ";
    std::cin >> pin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (id != -1 || pin != "-1") {
        std::cout << "Unauthorized. Only admin can delete accounts." << std::endl;
        return;
    }
    int delId;
    std::cout << "Enter Account ID to delete: ";
    delId = restrictIDInt();
    if (!list.deleteById(delId)) {
        std::cout << "Account " << delId << " not found." << std::endl;
    } else {
        tlist.markDeleted(delId);
        std::cout << "Account " << delId << " deleted; related transactions marked." << std::endl;
    }
    std::cout << "Press 0 to return to menu." << std::endl;
}


// Dispatch function remains unchanged
bool actionMenu(AccountList& list, TransactionList& tlist) {
    printMenu();
    int choice;
    if(!(std::cin>>choice)){
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return true;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch(choice) {
        case 1: addAccountAction(list); returnMenu(); break;
        case 2: displayAccountAction(list); returnMenu(); break;
        case 3: updateAccountAction(list); returnMenu(); break;
        case 4: lockUnlockAction(list); returnMenu(); break;
        case 5: transactionAction(list,tlist); returnMenu(); break;
        case 6: transferMoneyAction(list,tlist); returnMenu(); break;
        case 7: displayHistoryAction(list, tlist); returnMenu(); return true;
        case 8: undoTransactionAction(list, tlist); returnMenu(); return true;
        case 9: deleteAccountAction(list, tlist); returnMenu(); return true;
        case 0: return false;
        default: std::cout<<"Invalid choice."<<std::endl; returnMenu(); return true;
    }
    return true;
}