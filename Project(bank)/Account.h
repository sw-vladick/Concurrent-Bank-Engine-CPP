#pragma once
#include <mutex>

class Account
{
public:
	Account(double balance);
	double GetBalance() const;
	void Deposit(double amount);
	bool Withdraw(double amount);
	std::mutex mtx;
private:
	double balance;
};

