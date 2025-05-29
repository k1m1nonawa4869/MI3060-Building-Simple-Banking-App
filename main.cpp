#include "AccountList.h"
#include "TransactionList.h"
#include "Actions.h"

int main() {
    AccountList list;
    TransactionList tlist;
    // Load persisted data
    list.load("accounts.txt");
    tlist.load("transactions.txt");

    bool running = true;
    while (running) {
        // Display menu and handle user choice
        running = actionMenu(list, tlist);
    }

    // Save data before exiting
    list.save("accounts.txt");
    tlist.save("transactions.txt");

    return 0;
}