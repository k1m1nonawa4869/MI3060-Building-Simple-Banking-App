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
