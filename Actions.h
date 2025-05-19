// File: Actions.h
#ifndef ACTIONS_H
#define ACTIONS_H

#include "AccountList.h"
#include "TransactionList.h"

// Print the main menu
void printMenu();

// Block until the user enters 0, then return to the menu
void returnMenu();

// Menu action handlers
void addAccountAction(AccountList& list);
void displayAccountAction(AccountList& list);
void updateAccountAction(AccountList& list);
void lockUnlockAction(AccountList& list);
void transactionAction(AccountList& list, TransactionList& tlist);
void transferMoneyAction(AccountList& list, TransactionList& tlist);
void displayHistoryAction(AccountList& list, TransactionList& tlist);
void undoTransactionAction(AccountList& list, TransactionList& tlist);
void deleteAccountAction(AccountList& list, TransactionList& tlist);

// Dispatch function: reads the user’s menu choice and calls the
// appropriate function above. Returns false when the user chooses Exit.
bool actionMenu(AccountList& list, TransactionList& tlist);

#endif // ACTIONS_H
