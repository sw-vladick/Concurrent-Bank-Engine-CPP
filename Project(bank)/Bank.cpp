#include "Bank.h"

Bank::Bank()
{
	accounts[1] = std::make_unique<Account>(1000.0);
	accounts[2] = std::make_unique<Account>(1000.0);
	accounts[3] = std::make_unique<Account>(1000.0);

	cycle = true;

	for (int i = 0; i < 4; ++i) {
		workers.emplace_back(&Bank::WorkerRoutine, this);
	}
}

void Bank::WorkerRoutine()
{
	while (true)
	{
		Transaction tx;
		{
			std::unique_lock<std::mutex> lock(mtxBank);

			cv.wait(lock, [=] {
				return !cycle || !transactions.empty();
				});

			if (!cycle && transactions.empty()) {
				break;
			}

			tx = transactions.front();
			transactions.pop();
		}
		
		auto toId = accounts.find(tx.toAccountId);
		auto fromId = accounts.find(tx.fromAccountId);

		if (tx.fromAccountId == tx.toAccountId) {
			continue;
		}

		if (toId != accounts.end() && fromId != accounts.end()) {
			std::scoped_lock lock(toId->second->mtx, fromId->second->mtx);
			if (fromId->second->Withdraw(tx.amount)) {
				toId->second->Deposit(tx.amount);
				Logger::Log("[SUCCESS] Transfer from ID ", fromId->first, " to ID ", toId->first, " | Amount: ", tx.amount);
				Logger::Log("          New balance ID ", fromId->first, ": ", fromId->second->GetBalance(), " | ID ", toId->first, ": ", toId->second->GetBalance(), "\n");
			}
			else {
				Logger::Log("[FAILED] Not enough money on ID ", fromId->first, " for transfer of ", tx.amount, "\n");
			}
		}
	}
}

void Bank::AddTransaction(int from, int to, double amount)
{
	{
		std::lock_guard<std::mutex> lock(mtxBank);
		transactions.push({ from, to, amount });
	}
	cv.notify_one();
}

Bank::~Bank()
{
	cycle = false;

	cv.notify_all();
	for (auto& worker : workers) {
		if (worker.joinable()) worker.join();
	}
}
