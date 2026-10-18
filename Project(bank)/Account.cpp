#include "Account.h"

Account::Account(double balance): balance(balance) {}

double Account::GetBalance() const { return balance; }

void Account::Deposit(double amount) { balance += amount; }

bool Account::Withdraw(double amount)
{
	if (balance - amount < 0) {
		return false;
	}
	balance -= amount;
	return true;
}

