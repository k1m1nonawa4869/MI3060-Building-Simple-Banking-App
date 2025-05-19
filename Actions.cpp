// File: Actions.cpp
#include "AccountList.h"
#include "TransactionList.h"
#include <iostream>
#include <limits>
#include <cctype>

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
        int id; std::string name; int pin;
        while (true) {
            std::cout << "Enter unique account ID: "; std::cin >> id;
            if (!list.findById(id)) break;
            std::cout << "That ID already exists. Try again." << std::endl;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter account holder name: "; std::getline(std::cin, name);
        std::cout << "Set account PIN: "; std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        list.addAccount(BankAccount(id, name, pin));
        std::cout << "Add another? (y/n): "; std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 2. Display Account Info
void displayAccountAction(AccountList& list) {
    while (true) {
        int id, pin;
        std::cout << "Enter your account ID (-1 admin): "; std::cin >> id;
        std::cout << "Enter your PIN: "; std::cin >> pin;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (id == -1 && pin == -1) {
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
        int id; std::cout << "Enter account ID to update: "; std::cin >> id;
        BankAccount* acct = list.findById(id);
        if (!acct) {
            std::cout << "Account not found." << std::endl;
        } else {
            int pin;
            while (true) {
                std::cout << "Enter PIN (or -1 cancel): "; std::cin >> pin;
                if (pin == -1) { std::cout << "Cancelled." << std::endl; break; }
                if (acct->verifyPin(pin)) break;
                std::cout << "Incorrect PIN." << std::endl;
            }
            if (!acct->verifyPin(pin)) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Press any key to continue." << std::endl;
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
                int np; std::cout << "New PIN: "; std::cin >> np;
                acct->setPin(np);
            }
        }
        std::cout << "Update another? (y/n): "; std::cin >> cont; cont = std::tolower(cont);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// 4. Lock/Unlock Account
void lockUnlockAction(AccountList& list) {
    int id, pin;
    std::cout << "Enter your account ID: "; std::cin >> id;
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
    returnMenu();
}

// 5. Transaction (Deposit/Withdraw)
void transactionAction(AccountList& list, TransactionList& tlist) {
    char cont = 'y';
    while (cont=='y') {
        int id; std::cout<<"ID: "; std::cin>>id;
        BankAccount* acct = list.findById(id);
        if (!acct) { std::cout<<"Not found."<<std::endl; continue; }
        if (acct->isLocked()) { std::cout<<"Account locked."<<std::endl; continue; }
        int pin;
        while (true) {
            std::cout<<"PIN(-1 cancel): "; std::cin>>pin;
            if (pin==-1) break;
            if (acct->verifyPin(pin)) break;
            std::cout<<"Wrong PIN."<<std::endl;
        }
        if (!acct->verifyPin(pin)) continue;
        char choice; std::cout<<"d=Deposit,w=Withdraw: "; std::cin>>choice; choice=std::tolower(choice);
        while(choice!='d'&&choice!='w') {std::cin>>choice; choice=std::tolower(choice);}        
        int amt; std::cout<<"Amount: "; std::cin>>amt;
        if(choice=='d'){
            acct->balance+=amt;
            tlist.add(id,"Deposit",id,amt,acct->balance);
            std::cout<<"Deposited."<<std::endl;
        } else {
            if(amt>acct->balance) std::cout<<"Insufficient."<<std::endl;
            else {acct->balance-=amt; tlist.add(id,"Withdraw",id,amt,acct->balance); std::cout<<"Withdrawn."<<std::endl;}
        }
        std::cout<<"Again? (y/n): ";std::cin>>cont; cont=std::tolower(cont);
    }
}

// 6. Transfer Money
void transferMoneyAction(AccountList& list, TransactionList& tlist) {
    char cont='y';
    while(cont=='y'){
        int sid; std::cout<<"Source ID: "; std::cin>>sid;
        BankAccount* src=list.findById(sid);
        if(!src){std::cout<<"Not found."<<std::endl;continue;}
        if(src->isLocked()){std::cout<<"Locked."<<std::endl;continue;}
        int pin; std::cout<<"PIN(-1 cancel): "; std::cin>>pin;
        if(pin==-1) continue;
        if(!src->verifyPin(pin)){std::cout<<"Wrong PIN."<<std::endl;continue;}
        int did; std::cout<<"Dest ID: "; std::cin>>did;
        BankAccount* dst=list.findById(did);
        if(!dst){std::cout<<"Dest not found."<<std::endl;continue;}
        if(dst->isLocked()){std::cout<<"Dest locked."<<std::endl;continue;}
        int amt; std::cout<<"Amount: ";std::cin>>amt;
        if(amt>src->balance) std::cout<<"Insufficient."<<std::endl;
        else {src->balance-=amt; dst->balance+=amt; tlist.add(sid,"Transfer",did,amt,src->balance); std::cout<<"Transferred."<<std::endl;}
        std::cout<<"Again? (y/n): ";std::cin>>cont;cont=std::tolower(cont);
    }
}

// 7. Transaction History
void displayHistoryAction(AccountList& list, TransactionList& tlist) {
    while(true){
        int id,pin; std::cout<<"ID(-1 admin): "; std::cin>>id; std::cout<<"PIN: "; std::cin>>pin;
        if(id==-1&&pin==-1){tlist.printAllAdmin();break;}
        BankAccount* acct=list.findById(id);
        if(!acct||!acct->verifyPin(pin)){std::cout<<"Invalid."<<std::endl;continue;}
        tlist.printUser(id);
        break;
    }
}

// 8. Undo Last Transaction
void undoTransactionAction(AccountList& list, TransactionList& tlist) {
    int id,pin; std::cout<<"Admin ID: ";std::cin>>id;std::cout<<"PIN: ";std::cin>>pin;
    if(id!=-1||pin!=-1){std::cout<<"Unauthorized."<<std::endl;returnMenu();return;}
    Transfer tr;
    if(!tlist.popLast(tr)){std::cout<<"None."<<std::endl;returnMenu();return;}
    if(tr.type=="Deposit"){BankAccount* a=list.findById(tr.originId);
        if(a->balance<tr.amount) std::cout<<"Undo failed."<<std::endl;
        else {a->balance-=tr.amount; std::cout<<"Undo Deposit."<<std::endl;}}
    else if(tr.type=="Withdraw"){BankAccount* a=list.findById(tr.originId);
        a->balance+=tr.amount; std::cout<<"Undo Withdraw."<<std::endl;}
    else if(tr.type=="Transfer"){BankAccount* s=list.findById(tr.originId);
        BankAccount* d=list.findById(tr.destinationId);
        if(d->balance<tr.amount) std::cout<<"Undo failed."<<std::endl;
        else {d->balance-=tr.amount; s->balance+=tr.amount; std::cout<<"Undo Transfer."<<std::endl;}}
    returnMenu();
}

// 9. Delete Account
void deleteAccountAction(AccountList& list, TransactionList& tlist) {
    int id,pin; std::cout<<"Admin ID: ";std::cin>>id;std::cout<<"PIN: ";std::cin>>pin;
    if(id!=-1||pin!=-1){std::cout<<"Unauthorized."<<std::endl;returnMenu();return;}
    int delId; std::cout<<"Delete ID: ";std::cin>>delId;
    if(!list.deleteById(delId)) std::cout<<"Not found."<<std::endl;
    else {tlist.markDeleted(delId); std::cout<<"Deleted."<<std::endl;}
    returnMenu();
}

// Dispatch
bool actionMenu(AccountList& list, TransactionList& tlist) {
    printMenu();
    int choice;
    if(!(std::cin>>choice)){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');return true;}
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch(choice) {
        case 1: addAccountAction(list); returnMenu(); return true;
        case 2: displayAccountAction(list); returnMenu(); return true;
        case 3: updateAccountAction(list); returnMenu(); return true;
        case 4: lockUnlockAction(list); returnMenu(); return true;
        case 5: transactionAction(list, tlist); returnMenu(); return true;
        case 6: transferMoneyAction(list, tlist); returnMenu(); return true;
        case 7: displayHistoryAction(list, tlist); returnMenu(); return true;
        case 8: undoTransactionAction(list, tlist); returnMenu(); return true;
        case 9: deleteAccountAction(list, tlist); returnMenu(); return true;
        case 0: return false;
        default: std::cout<<"Invalid choice."<<std::endl; returnMenu(); return true;
    }
}
