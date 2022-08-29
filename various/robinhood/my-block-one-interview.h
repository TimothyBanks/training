#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
using namespace std;
struct account_balance {
   int    account_id; ///< the name of the account
   int    balance; ///< the balance of the account
};
struct transfer {
   int from;    ///< the account to transfer from
   int to;      ///< the account to transfer to
   int amount;  ///< the amount to transfer
};
using transaction = vector<transfer>;
/**
 *
 * @param initial_balances - the initial balances in the database, see the above datastructures.
 * @return an instantiated database type of your creation which has the following member methods:
 *
 *    // push a transaction to the database
 *    //
 *    void push_transaction( const transaction&t )
 *
 *    // settle the database such that the invariant is maintained and the best state of the database is present
 *    //
 *    void settle()
 *
 *    // return a listing of all the balances in any order
 *    //
 *    vector<account_balance> get_balances() const
 *
 *    // Given the logical sequence of transactions constructed by the ordered sequence of calls
 *    // to push_transaction AND having called settle()
 *    //
 *    // return the 0-based indices of the surviving transactions in that sequence which, when
 *    // applied to the initial balances produce a state where the invariant is maintained
 *    //
 *    vector<size_t> get_applied_transactions() const
 *
 * The automated test cases will call create_database with a set of initial balances
 * They will then feed a number of transactions to your class
 * They will then call settle
 * They will then call get_applied_transactions and inspect the results
 * They will then call get_balances and inspect the results
 */
class Database final
{
public:
    // We want the parameter by value to allow the user to choose if they want
    // to copy or move their value into the type.
    Database(std::vector<account_balance> initial_balances)
    : m_balances{std::move(initial_balances)}
    {
    }
    void push_transaction(const transaction& t)
    {
        // The description mentions how transactions are atomic.  It doesn't
        // state if this method is being invoked concurrently.  Hopefully the test
        // cases will shed light on whether we need synchronization here to
        // enforce the atomic nature of the transactions.
        m_transactions.emplace_back(t, true);
        auto transaction_id = m_transactions.size() - 1;
        for (const auto& transfer : t)
        {
            do_transfer_(transfer.from, transfer.to, transfer.amount);
            m_deductions[transfer.from - 1].emplace_back(transfer, transaction_id);
        }
    }
    void settle()
    {
        auto overdrawn = std::queue<size_t>{};
        auto add_if_overdrawn_ = [&](auto account_id)
        {
            const auto& account = m_balances[account_id];
            if (account.balance >= 0)
            {
                return;
            }
            overdrawn.push(account_id);
        };
        // Find the initial overdrawn accounts.
        for (size_t i = 0; i < m_balances.size(); ++i)
        {
            add_if_overdrawn_(i);
        }
        // Keep rolling back transactions while there are overdrawn accounts.
        while (!overdrawn.empty())
        {
            auto account_id = overdrawn.front();
            overdrawn.pop();
            if (m_balances[account_id].balance >= 0)
            {
                // This account is currently ok.
                // This can happen because we check after each rollback of a transfer
                // if either account involved in the transfer is overdrawn. Follow on
                // transfer rollbacks can make an account square but the account id is
                // still within the queue to verify.
                continue;
            }
            auto max = static_cast<int64_t>(0);
            auto transfer_id = static_cast<size_t>(0);
            // Find the largest transfer and assume that the transaction it
            // came from is the one to roll back.
            for (size_t i = 0; i < m_deductions[account_id].size(); ++i)
            {
                const auto& transfer = m_deductions[account_id][i];
                if (!m_transactions[transfer.second].second)
                {
                    // This transaction has already been rolled back.
                    continue;
                }
                auto difference = std::abs(transfer.first.amount - m_balances[account_id].balance);
                if (difference >= max)
                {
                    max = difference;
                    transfer_id = i;
                }
            }
            // Rollback the transaction
            const auto& transfer = m_deductions[account_id][transfer_id];
            auto& transaction = m_transactions[transfer.second];
            transaction.second = false; // Mark it as being rolled back.
            for (const auto& transfer : transaction.first)
            {
                do_transfer_(transfer.to, transfer.from, transfer.amount);
                add_if_overdrawn_(transfer.to);
                add_if_overdrawn_(transfer.from);
            }
        }
    }
    std::vector<account_balance> get_balances() const
    {
        return m_balances;
    }
    std::vector<size_t> get_applied_transactions() const
    {
        auto applied_transactions = std::vector<size_t>{};
        applied_transactions.reserve(m_transactions.size());
        for (size_t i = 0; i < m_transactions.size(); ++i)
        {
            if (!m_transactions[i].second)
            {
                // The transaction was rolled back during the settle process.
                continue;
            }
            applied_transactions.push_back(i);
        }
        return applied_transactions;
    }
private:
    void do_transfer_(size_t from, size_t to, int64_t amount)
    {
        if (from - 1 >= m_balances.size() || to - 1 >= m_balances.size())
        {
            // What do we in this case?  We could throw an exception or just continue on
            // to the next transfer.
            return;
        }
        m_balances[from - 1].balance -= amount;
        m_balances[to - 1].balance += amount;
    }
private:
    // This represents a deduction from an account.  The second item in the pair
    // is the id of the transaction that the transfer occurred.
    using deduction = std::pair<transfer, size_t>;
    // This represents all the deductions that have occurred against an account.
    // The key is the account id.
    using deductions = std::map<size_t, std::vector<deduction>>;
    // Represents a transaction.  The second item in the pair
    // indicates if the transaction has been rolled back (false) or not (true).
    using transaction_ = std::pair<transaction, bool>;
    std::vector<account_balance> m_balances;
    std::vector<transaction_> m_transactions;
    deductions m_deductions;
};
auto create_database(const vector<account_balance>& initial_balances)
{
   // It seems like we would want to be able to pass around the database instance
   // and share it.  That could be exposed by either returning a shared pointer instance
   // or something that follows the PIMPL idiom.  But since I don't really know anything
   // beyond the usage described above, returning a simple stack based instance seems to be the best
   // approach for now.
   return Database{initial_balances};
}
template<typename DB>
static void print_transactions( const DB& db, ofstream& fout ) {
   auto trxs = db.get_applied_transactions();
   sort(trxs.begin(), trxs.end());
   fout << trxs.size() << endl;
   for( size_t cur : trxs ) {
      fout << cur << endl;
   }
}
template<typename DB>
static void print_database( const DB& db, ofstream& fout ) {
   auto balances = db.get_balances();
   sort(balances.begin(), balances.end(), [](const auto& a, const auto& b){
      return a.account_id < b.account_id;
   });
   fout << balances.size() << endl;
   for( const auto& cur : balances ) {
      fout << cur.account_id << " " << cur.balance << endl;
   }
}
#ifndef TESTING
int main( int arc, char **args)
{
    ofstream fout(getenv("OUTPUT_PATH"));
    try {
       ifstream fin;
       auto input_path = getenv("INPUT_PATH");
       if (input_path != nullptr) {
          fin.open(input_path);
       }
       istream& in =  input_path ? fin : cin;
       int remaining_accounts = 0;
       in >> remaining_accounts;
       in.ignore(numeric_limits<streamsize>::max(), '\n');
       vector<account_balance> initial_balances;
       while (remaining_accounts-- > 0) {
          int account = 0;
          int balance = 0;
          in >> account >> balance;
          in.ignore(numeric_limits<streamsize>::max(), '\n');
          initial_balances.push_back({account, balance});
       }
       auto db = create_database(initial_balances);
       int remaining_transactions = 0;
       in >> remaining_transactions;
       in.ignore(numeric_limits<streamsize>::max(), '\n');
       while (remaining_transactions-- > 0) {
          int remaining_transfers = 0;
          in >> remaining_transfers;
          in.ignore(numeric_limits<streamsize>::max(), '\n');
          transaction tx;
          while( remaining_transfers-- > 0) {
             int from = 0;
             int to = 0;
             int amount = 0;
             in >> from >> to >> amount;
             in.ignore(numeric_limits<streamsize>::max(), '\n');
             tx.push_back({from, to, amount});
          }
          db.push_transaction(tx);
       }
       db.settle();
       print_transactions(db, fout);
       print_database(db, fout);
   } catch (...) {
       return -1;
   }
   return 0;
}
#endif
