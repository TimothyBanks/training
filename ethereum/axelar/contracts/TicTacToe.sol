// SPDX-License-Identifier: Unlicense

pragma solidity ^0.8.7;

contract TicTacToe {
    enum BoardState {Unset, Player1, Player2}
    enum GameState {New, Active, Draw, Winner}

    event GameNew(uint64 game);
    event GameStarted(uint64 game);
    event GameMoveMade(uint64 game);
    event GameCompleted(uint64 game);

    struct Game {
        address p1;           // Address of player 1
        address p2;           // Address of player 2  
        GameState state;      // The state of the game
        bool turn;            // Whose turn it is.  0 - Player 1, 1 - Player 2
        bool winner;          // Who won if state is GameState.Winner.  0 - Player 1, 1 - Player 2
        BoardState[9] board;  // The board
    }

    mapping(uint64 => Game) private games;
    uint64 private gameCount;
    uint8 private n;

    constructor() {
        gameCount = 0;
        n = 3;
    }

    modifier validGame(uint64 game) {
        require(game < gameCount, "Invalid game.");
        _;
    }

    modifier gameAvailable(uint64 game) {
        require(games[game].state == GameState.New, "Game is no longer available.");
        _;
    }

    modifier gameActive(uint64 game) {
        require(games[game].state == GameState.Active, "Game is no longer active.");
        _;
    }

    modifier myTurn(uint64 game, address player) {
        Game memory g = games[game];
        require(g.p1 == player || g.p2 == player, "Invalid player.");
        require((g.p1 == player && !g.turn) || (g.p2 == player && g.turn), "Invalid turn.");
        _;
    }

    modifier validMove(uint64 game, uint8 x, uint y) {
        require(x < n && y < n, "Invalid move.");
        Game memory g = games[game];
        require(g.board[y * n + x] == BoardState.Unset, "Invalid move.  Position already occupied.");
        _;
    }

    function Create(address player) public returns (uint64) {
        Game memory g = Game(
            player, 
            address(0), 
            GameState.New, 
            false, 
            false, 
            [BoardState.Unset, BoardState.Unset, BoardState.Unset,
             BoardState.Unset, BoardState.Unset, BoardState.Unset,
             BoardState.Unset, BoardState.Unset, BoardState.Unset]);
        games[gameCount] = g;
        gameCount++;
        emit GameNew(gameCount - 1);
        return gameCount - 1;
    }

    function Join(uint64 game, address player) public validGame(game) gameAvailable(game) {
        Game storage g = games[game];
        g.p2 = player;
        g.state = GameState.Active;

        // There is a bit of randomness to who goes first.  Basically,
        // we concatenate the two addresses, hash that and check what the first
        // bit is.  If that bit is 0, then player 2 goes first, otherwise player 1 goes.
        bytes32 b = keccak256(abi.encodePacked(g.p1, g.p2));
        if ((uint8(b[0]) >> 7) & 0x1 == 0) {
            g.turn = true;
        } else {
            g.turn = false;
        }

        emit GameStarted(game);
    }

    function NextGameState(BoardState[9] memory board) private pure returns (GameState, bool) {
        bool full_board = true;
        for (uint8 i = 0; i < 9; ++i) {
            // Check if the board is full. If any position is still Unset, then the board is not full.
            full_board = (board[i] == BoardState.Unset ? false : full_board);
            
            if (board[i] == BoardState.Unset) {
                continue;
            }
            
            if (i % 3 == 0 && board[i] == board[i+1] && board[i] == board[i+2]) {
                // Beginning of a row
                return (GameState.Winner, board[i] == BoardState.Player1 ? false : true);
            }

            if (i < 3 && board[i] == board[i + 3] && board[i] == board[i + 6]) {
                // Beginning of a column
                return (GameState.Winner, board[i] == BoardState.Player1 ? false : true);
            }

            if (i == 0 && board[i] == board[4] && board[i] == board[8]) {
                // Diagonal.
                return (GameState.Winner, board[i] == BoardState.Player1 ? false : true);
            }

            if (i == 2 && board[i] == board[4] && board[i] == board[6]) {
                // The other Diagonal.
                return (GameState.Winner, board[i] == BoardState.Player1 ? false : true);
            }
        }

        // If we got to this point, there is no winner but we might be at a draw.
        return (!full_board ? GameState.Active : GameState.Draw, false);
    }

    function Move(uint64 game, address player, uint8 x, uint8 y) public validGame(game) gameActive(game) myTurn(game, player) validMove(game, x, y) {
        Game storage g = games[game];
        g.board[y * n + x] = g.turn ? BoardState.Player2 : BoardState.Player1;
        g.turn = !g.turn; // It is the other players turn.
        (g.state, g.winner) = NextGameState(g.board);
        emit GameMoveMade(game);
        if (g.state != GameState.Active) {
            emit GameCompleted(game);
        }
    }

    function State(uint64 game) public view validGame(game) returns (Game memory) {
        return games[game];
    }
}