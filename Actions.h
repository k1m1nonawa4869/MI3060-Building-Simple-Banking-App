#ifndef ACTIONS_H
#define ACTIONS_H

#include "AccountList.h"
#include "TransactionList.h"
#include "AccList.h"
#include "AccTranList.h"

// Print the main menu
void printMenu();

// Block until the user enters 0, then return to the menu
void returnMenu();

// Dispatch function: reads the user’s menu choice and calls the
// appropriate function above. Returns false when the user chooses Exit.
bool actionMenu(AccountList& list, TransactionList& tlist);

#endif // ACTIONS_H