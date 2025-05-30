#ifndef ACC_TRAN_LIST_H
#define ACC_TRAN_LIST_H 

#include "AccountList.h"
#include "TransactionList.h"

// 5. Transaction (Deposit/Withdraw)
void transactionAction(AccountList &list, TransactionList &tlist);
// 6. Transfer Money
void transferMoneyAction(AccountList &list, TransactionList &tlist);
// 7. Transaction History
void displayHistoryAction(AccountList &list, TransactionList &tlist);
// 8. Undo Selected Between Two Accounts (require both admin and users identity)
void undoTransactionAction(AccountList& list, TransactionList& tlist); 
// 9. Delete Account
void deleteAccountAction(AccountList &list, TransactionList &tlist);

#endif // ACC_TRAN_LIST_H 
