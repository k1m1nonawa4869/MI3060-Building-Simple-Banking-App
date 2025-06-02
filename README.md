
#  MI3060-Building-Simple-Banking-App

This is our large project to build a simple banking apps in C++, with core functionalities such as create/delete account, query balance, deposit/transfer/withdraw money,... Using only DSA-related stuffs.

## Prerequisites:

- C++11–compatible compiler (e.g. g++ 7+, clang 5+, MSVC 2017+)
- CMake 3.10+
- (Optional) Ninja or Make 

##  What to do:

 -  First Clone this project to your local device. Use `Main` branch, select `Code -> Download ZIP` or use with your Github Desktop.

 -  If you are just a beginner wants to try our project, go to `build\CMakeFiles` and run `bank_management.exe`.

 -  If you want to explore more: Should you change the code, you must rebuild the exe using

	- `cd build` if you are not in `build` folder

	- add new .cpp file to `CMakeLists.txt ` if you create new .cpp file

	- use CMakeTools `cmake --build .` or `ninja` to rebuild.
	
- CSV can be manually created if you are lazy to create through GUI. Following the syntax:
	- `id,name,balance,pin,locked,created,modified` for accounts.txt
	- `originId,type,destId,amount,origBal,destBal,timestamp` for transactions.txt


  

##  What you can do:

-  Create/update/lock account, query balance and information, deposit/transfer/withdraw money.

-  Admin account has previlege to see all accounts, transactions; has the rights to delete account and requested transactions.

-  Program has an easy-to-use GUI so you can just pick the action you want to do.

- Quick Demo:

     === Bank Management ===
    1. Add Account
    2. Display Account Info
    ...
    Choose an option: 1
    
    Enter unique account ID: 101
    Enter account holder name: Alice
    Set account PIN: 1234
    Add another? (y/n): n
    
    Enter 0 to return to main menu: 0
    …



  

##  What have we used:

-  Account and Transaction Class.

-  Structures: List (to store AccountID in order), Linked-list (to manage Account and Transaction hitsory).

-  Algorithm: Quick Sort, Binary Search (to find appropriate AccountID), functions on linked-list.

## Roadmap

What we wants to implement if having more time:

-   CSV external import/export
    
-   Role-based access control
    
-   Automated test suite
    
-   Integration with a real database

  

##  Credits:

Special thanks to @Letatducmanh (initial Account and Transaction structures) and @yuhle-qug (Binary Search contributors) for the project.

  

##  Contributions:

Please open an issue or submit a pull request.

Feel free to suggest new features or report bugs!

## License:
I dont't know