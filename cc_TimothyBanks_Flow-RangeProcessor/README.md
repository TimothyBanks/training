# Build instructions
1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./dapperlabs -n 50 -t 22 -i ../testdata/blocks_1 -i ../testdata/blocks_2 

# Range request processing problem - Senior/Staff

Each new node which joins a blockchain network starts from a blank slate (i.e. at block height 0), and must catch up to the latest height of the chain by downloading past blocks from other nodes on the network. It does this using a synchronization engine, which sends requests for ranges of blocks to various nodes on the network and tracks the responses.

Because a blockchain network is a "trustless" network, we cannot blindly accept the responses we receive. To increase the likelihood that we receive the correct block for each height, we consider a given height to be **fulfilled** only once we've received at least `n` responses agreeing on the same `Block` at that height. The first `Block` to reach `n` responses is accepted.

### Task:

The task for this assignment is to implement a `RangeResponseProcessor` to help track the range of block heights that the synchronization engine should actively be requesting. We will refer to this as the **active range**. The `RangeResponseProcessor` should have the following methods:

- `ProcessRange(startHeight uint64, blocks []Block)`
    - This is called by the synchronization engine to process each range response it receives.
    - You may assume that the input represents a consecutive range of blocks starting at height `startHeight`. For example, the first element of `blocks` has height `startHeight`, the second element has height `startHeight + 1`, and so on.
    - If any blocks in the input fall outside the current active range, you should ignore them.
    - Once a `Block` receives `n` responses for a given height, it is accepted and you can update the active range.
- `GetActiveRange() (minHeight uint64, maxHeight uint64)`
    - This should return the current active block height range represented as `(h, h+s)`, where `h` is the minimum height such that fewer than `n` range responses agreeing on the `Block` at height `h` have been received so far. In other words, `h` is the minimum height that has yet to be fulfilled.
    - `s` represents the size of the active range.
    - `s` and `n` are parameters that should be provided to the `RangeResponseProcessor` upon instantiation.

The instantiator of `RangeResponseProcessor` provides a callback, which must be called when a `Block` is accepted for a height. The callback should take 2 arguments: `height uint64` and `block Block`.

For simplicity, you may assume that the underlying representation of a `Block` is simply a string:

```go
type Block string
```

## General guidance

- Do not write low-level, networking or other IPC, multi-process or container centric solutions. This is a local development coding test meant to only take 1-2 hours maximum.
- We prefer candidates to use GoLang - our in house language, however, please feel free to use any language of your preference.
- You should ensure that your implementation is concurrency-safe.
- Please approach this as you would a real-world problem. We are not only assessing your ability to solve the problem but also trade-offs/edge cases considered and your holistic approach to quality.
- If anything remains unclear about this problem don’t hesitate to ask your Talent team associate who can get follow-ups from engineering.
- When submitting, please include a section in your response, or within your code, to summarize any assumptions, or other matters to share to us in considering your submission.
