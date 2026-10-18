#include <iostream>
#include <thread>
#include "Bank.h"

int main() {
    Bank myBank;

    myBank.AddTransaction(1, 2, 500.0);
    myBank.AddTransaction(2, 3, 200.0);
    myBank.AddTransaction(3, 1, 100.0);
    myBank.AddTransaction(1, 3, 300.0);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}

