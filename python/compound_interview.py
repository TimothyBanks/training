# Your previous Plain Text content is preserved below:


# # FAILURE -> SUCCESS

# # Queue

# { id: 4, user: Tom, action: withdrawal, amount: 2 } SUCCESS
# ##{ id: 3, user: Tom, action: withdrawal, amount: 8 } SUCCESS##
# { id: 4, user: Tom, action: withdrawal, amount: 4 } FAILURE -> SUCCESS
# { id: 5, user: Tom, action: withdrawal, amount: 5 } FAILURE -> SUCCESS
# { id: 4, user: Tom, action: withdrawal, amount: 1 } SUCCESS -> FAILURE

# { id: 1, user: Tom, action: deposit, amount: 10 }
# { id: 4, user: Tom, action: withdrawal, amount: 2 }

# { id: 2, user: John, action: deposit, amount: 5 }
# { id: 5, user: John, action: withdrawal, amount: 2 }
# { action: yank, yank_id: 3 }
# { id: 6, user: John, action: withdrawal, amount: 1 }

# { id: 7, user: Tom, to: John, action: transfer, amount: 5 }

# # API
 
#  user | bal
#  ----------
#    tom|   6
#   john|   2


#   Queue Table
#   - Id
#   - From
#   - To
#   - Action {Deposit/Withdraw/yank}
#   - Amount
#   - Yank ID
#   - State (SUCCESS/REJECT/YANKED)
#   - From Balance
#   - To Balance

#   Triggers/Stored Procedure

#   User Total
#   - User
#   - Total

# 3 Actions
# 1. Deposit:  It's just allowed to happen.  We insert a new record in the queue and set its state to success.
# 2. Withdrawal: 
# 2.1 Insert a new record into the queue table.
# 2.2 Ask for the summation of all the successful deposits and withdraws. -- O(log(n))
# 2.3 If that total is at least as much as the withdraw, set state to success
# 2.4 Otherwise set state to reject.
# 3. Yank
# 3.1 Calculate user total up to that point in the queue. -- O(log(n))
# 3.2 For each item in the queue that is for the user, reset the state of that transaction. `O(yank_id -> present)`


# a1 = uniswap(symbol).getPriceAccumulator()
# a2 = uniswap(symbol).getPriceAccumulator()
# TWAP = (a2 - a1) / (t2 - t1)
# compute TWAP over at least last 30 minutes 

# function postPrice(string symbol, uint price)
#  ...
#  uint anchorPrice = fetchAnchorPrice(symbol)
#  ...

class entry:
    def __init__(self):
        self.accumlator = 0
        self.timestamp = 0

entries = {}

def init(symbol):
    e = entry()
    e.accumulator = uniswap(symbol).getPriceAccumulator()
    e.timestamp = time.time()
    entries[symbol] = e
    
def fetchAnchorPrice(symbol):
    symbol_entries = entries[symbol]

    e = entry()
    e.accumulator = uniswap(symbol).getPriceAccumulator()
    e.timestamp = time.time()
    search_time = e.timestamp - (30 * 60)

    index = 0
    for i in range(len(symbol_entries)):
        entry = symbol_entries[i]
        if entry.timestamp >= search_time:
            index = i
        else:
            break

    symbol_entries = symbol_entries[index:]
    symbol_entries.append(e)

    return (e.accumlator - symbol_entries[index].accumulator) / (e.timestamp - symbol_entries[index].timestamp) 
