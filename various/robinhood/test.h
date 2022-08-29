ostream& operator<<(ostream& os, const account_balance& ab)
{
   os << "(id=" << ab.account_id << ", balance=" << ab.balance << ")";
   return os;
}
ostream& operator<<(ostream& os, const transfer& trans)
{
   os << "(from=" << trans.from << ", to=" << trans.to << ", amount=" << trans.amount << ")";
   return os;
}
class CAccountsDB
{
protected:
   unordered_map<int, account_balance> account_cache;
   vector<transaction>                 appplied_tranactions;
   vector<size_t>                      appplied_tranactions_idx;
   vector<size_t>                      pending_tranactions_idx;

public:
   // TIM
   // Why do we need to declare the default constructor and destructor?
   // Could we also just default them?
   CAccountsDB() {}
   // TIM
   // What about move contructor?
   CAccountsDB(const CAccountsDB& c) : account_cache(c.account_cache) {}
   ~CAccountsDB() {}

public:
   void initialize_accounts(const vector<account_balance>& initial_balances_)
   {
      // TIM
      // use auto&
      for (account_balance acct : initial_balances_) {
         account_cache[acct.account_id] = acct;
      }
   }

protected:
    
   // TIM
   // Why not allow the transaction to always be applied?
   // and then through the settlement process, rollback transactions until the balance is non negative again?
   // The problem description does indicate that the account can overdrawn at any intermediate state.
   // So if you allowed the apply_transaction to alwasy succeed, how do you think that would change your algorithm.
   void apply_transaction(const size_t&      trx_idx,
                          const transaction& trx,
                          vector<size_t>&    applied_idx,
                          vector<size_t>&    rolledback_idx)
   {
      try {
         vector<transfer> trans_applied;
         // TIM
         // Use auto&
         for (transfer trans : trx) {
             
            // TIM
            // Could just use an auto variable here.
            unordered_map<int, account_balance>::iterator from_iter = account_cache.find(trans.from);
            unordered_map<int, account_balance>::iterator to_iter   = account_cache.find(trans.to);
             
            //
            // discarded will go into a pending list for retry
            // during settle.
            //
             
            // ensure we know accounts
            if ((from_iter == account_cache.end()) | (to_iter == account_cache.end())) {
               throw runtime_error("unknown accounts");
            }
            //
            // not overdrawn
            //
            if ((from_iter->second.balance - trans.amount) < 0) {
               // discard transaction
               throw runtime_error("overdrawn");
            }
             
            // apply transfer
            from_iter->second.balance -= trans.amount;
            to_iter->second.balance += trans.amount;
            trans_applied.push_back(trans);
         }
         applied_idx.push_back(trx_idx);
      } catch (exception& err) {
         rolledback_idx.push_back(appplied_tranactions.size() - 1);
         // roll back transaction transfers
         for (transfer trans : trans_applied) {
            unordered_map<int, account_balance>::iterator from_iter = account_cache.find(trans.from);
            unordered_map<int, account_balance>::iterator to_iter   = account_cache.find(trans.to);

            // unapply transfer
            from_iter->second.balance += trans.amount;
            to_iter->second.balance -= trans.amount;
         }
      }
   }

public:
   void push_transaction(const transaction& trx)
   {
      vector<size_t> applied_trx_idx;
      vector<size_t> rolledback_trx_idx;
       
      // TIM
      // It would be more efficient to move trx into this vector (and emplace it).
      // We could then pass a reference to the transaction in the vector into apply_transactions.
      appplied_tranactions.push_back(trx);
      apply_transaction(appplied_tranactions.size() - 1, trx, applied_trx_idx, rolledback_trx_idx);
       
      // TIM
      // We know in this case that there is only one transaction being worked on
      // so one of these vectors will be empty and the other will only have one element.
      // The for loops seem a bit overkill for handling that.
      for (size_t trx_idx : applied_trx_idx)
         appplied_tranactions_idx.push_back(trx_idx);
      for (size_t trx_idx : rolledback_trx_idx)
         pending_tranactions_idx.push_back(trx_idx);
   }
   void settle()
   {
      vector<size_t> applied_trx_idx;
      vector<size_t> rolledback_trx_idx;
      //
      // keep trying to apply rolled back transactions until
      // no more succeed.
      //
      while (pending_tranactions_idx.size() > 0) {
         applied_trx_idx.clear();
         rolledback_trx_idx.clear();
         for (size_t trx_idx : pending_tranactions_idx) {
            apply_transaction(trx_idx, appplied_tranactions[trx_idx], applied_trx_idx, rolledback_trx_idx);
         }
         for (size_t trx_idx : applied_trx_idx)
            appplied_tranactions_idx.push_back(trx_idx);
         //
         // if no more to settle or no more can be applied, quit
         //
        
         // TIM
         // It is idiomatic to check against empty instead of size == 0
         // Does the second part of the conditional indicate that it is impossible to apply any of the pending transactions?
         if ((rolledback_trx_idx.size() == 0) || (rolledback_trx_idx.size() == pending_tranactions_idx.size())) {
            break;
         }
         //
         // otherwise, keep trying
         //
          
         // TIM
         // Would be better to move the rolledback vector into the pending transactions.
         pending_tranactions_idx = rolledback_trx_idx;
      }
      sort(appplied_tranactions_idx.begin(), appplied_tranactions_idx.end());
   }
   vector<account_balance> get_balances() const
   {
      // TIM
      // You could also reserve the size of this vector.
      vector<account_balance>                             account_balances;
    
      // TIM
      // Curious why you chose a while loop instead of range based loop?
      unordered_map<int, account_balance>::const_iterator acct_iter = account_cache.begin();
      while (acct_iter != account_cache.end()) {
         account_balances.push_back(acct_iter->second);
         ++acct_iter;
      }
      return account_balances;
   }
   vector<size_t> get_applied_transactions() const
   {
      cout << "{";
      for (size_t i : appplied_tranactions_idx) {
         cout << i << ",";
      }
      cout << "}";
      return appplied_tranactions_idx;
   }
};
