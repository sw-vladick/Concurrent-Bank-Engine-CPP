#pragma once
#include <unordered_map>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <thread>
#include "Account.h"
#include "Transaction.h"
#include <atomic>
#include <queue>
#include "Logger.h"

class Bank
{
public:
	Bank();
	void AddTransaction(int from, int to, double amount);
	~Bank();
private:
	std::unordered_map<int, std::unique_ptr<Account>> accounts;
	std::queue<Transaction> transactions;
	std::condition_variable cv;
	std::mutex mtxBank;
	std::atomic<bool> cycle;
	std::vector<std::thread> workers;

	void WorkerRoutine();
};

