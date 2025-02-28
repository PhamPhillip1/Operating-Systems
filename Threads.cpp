#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

// Make all names from std accessible without std
using namespace std;

// Account balances for test
int account_balance;
int account_balance2;
int account_balance3;

// Mutexes to protect shared accounts
timed_mutex mtx;  // Protect account 1
timed_mutex mtx2; // Protect account 2
timed_mutex mtx3; // Protect account 3

// Transaction Method
void transaction(int amount, int& which_account, timed_mutex& mtx, int thread_id, string account_name) {
    // Lock the account balance prevent other threads accessing same time
    mtx.lock();

    if (amount >= 0) {
        which_account += amount;
    } else {
        which_account -= amount;
    }

    cout << "Thread " << thread_id << " (Account " << account_name << "): Transaction of " 
         << (amount >= 0 ? "deposit" : "withdrawal") << " of " << abs(amount) 
         << ". New Balance: " << which_account << endl;

    mtx.unlock();
}

// Function to simulate multiple transactions on Account 1
void handle_account1_transactions(int transactions) {
    vector<thread> threads;
    int amount;
    for (int i = 0; i < transactions; ++i) {
        cout << "Enter amount for transaction " << i + 1 << " for Account 1: ";
        cin >> amount;
        threads.push_back(thread(transaction, amount, ref(account_balance), ref(mtx), i + 1, "1")); // Create thread
    }

    // Wait all threads to finish
    for (auto& threads : threads) {
        threads.join();
    }
}

// Function to simulate multiple transactions on Account 2
void handle_account2_transactions(int transactions) {
    vector<thread> threads;
    int amount;
    for (int i = 0; i < transactions; ++i) {
        cout << "Enter amount for transaction " << i + 1 << " for Account 2: ";
        cin >> amount;
        threads.push_back(thread(transaction, amount, ref(account_balance2), ref(mtx2), i + 1, "2")); // Create threads
    }

    // Wait for all threads to finish
    for (auto& threads : threads) {
        threads.join();
    }
}

// Transfer money from account 1 to account 2
void transfer_to_account2(int amount, int thread_id) {
    // Locks same order for both transfer methods to ensure no deadlock with timeouts
    // Used timeouts mechanisms to give lock 2 seconds before another thread access it
    if (mtx.try_lock_for(chrono::seconds(2))) {
        if (mtx2.try_lock_for(chrono::seconds(2))) {
            account_balance -= amount;
            account_balance2 += amount;

            this_thread::sleep_for(chrono::seconds(1));

            cout << "Thread " << thread_id << ": Transferred " << amount 
                 << " from account 1 to account 2" << endl;
            mtx2.unlock();
        }
        mtx.unlock();
    }
}

// Transfer money from account 2 to account 1
void transfer_to_account1(int amount, int thread_id) {
    if (mtx.try_lock_for(chrono::seconds(2))) {
        if (mtx2.try_lock_for(chrono::seconds(2))) {
            account_balance2 -= amount;
            account_balance += amount;

            this_thread::sleep_for(chrono::seconds(1));

            cout << "Thread " << thread_id << ": Transferred " << amount 
                 << " from account 2 to account 1" << endl;
            mtx2.unlock();
        }
        mtx.unlock();
    }
}

// Test for many threads
void stress_test(int many_threads) {
    vector<thread> threads;

    for(int i = 0; i < many_threads; i++) {
        int random_amount = rand() % 1000; // 0 to 999
        int threads_id = i + 1;

        // Create thread
        threads.push_back(thread(transaction, random_amount, ref(account_balance3), ref(mtx3), threads_id, "3"));
    }
    cout << "Final Account Balance: " << account_balance3 << endl;
}

int main() {
    int total, total2;
    int transactions_a1, transactions_a2;
    int amount, amount2, amount4;

    // Ask the user for the total amounts in Account 1 and Account 2
    cout << "Welcome to your bank account!" << endl;
    cout << "Enter total amount for Account 1: ";
    cin >> total;
    cout << "Enter total amount for Account 2: ";
    cin >> total2;

    account_balance = total;
    account_balance2 = total2;

    // Ask the user for the number of transactions they want to perform
    cout << "How many transactions do you want to perform for Account 1? ";
    cin >> transactions_a1;
    cout << "How many transactions do you want to perform for Account 2? ";
    cin >> transactions_a2;

    // Handle transactions for Account 1 and 2
    handle_account1_transactions(transactions_a1);
    handle_account2_transactions(transactions_a2);

    // Ask user for transfers between accounts
    cout << "How much do you want to transfer to Account 2? ";
    cin >> amount;
    cout << "How much do you want to transfer to Account 1? ";
    cin >> amount2;

    // Ensure threads for transfers are created based on user input
    thread thread1(transfer_to_account2, amount, 1);
    thread thread2(transfer_to_account1, amount2, 2);

    // Wait for all threads to finish
    thread1.join();
    thread2.join();

    // Final balances after all transactions
    cout << "\nFinal Balances: \n";
    cout << "Account 1 = " << account_balance << endl;
    cout << "Account 2 = " << account_balance2 << endl;
    cout << endl;

    // Stress test
    cout << "Stress Test for 100 threads" << endl;
    stress_test(100);
    return 0;
}
