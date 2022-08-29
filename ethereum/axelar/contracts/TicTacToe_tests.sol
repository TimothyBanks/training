// SPDX-License-Identifier: GPL-3.0
        
pragma solidity >=0.4.22 <0.9.0;

// This import is automatically injected by Remix
import "remix_tests.sol"; 

// This import is required to use custom transaction context
// Although it may fail compilation in 'Solidity Compiler' plugin
// But it will work fine in 'Solidity Unit Testing' plugin
import "remix_accounts.sol";
import "../contracts/tictactoe.sol";

// File name has to end with '_test.sol', this file can contain more than one testSuite contracts
contract testSuite {
    function checkCreateGame() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        uint64 game_id_2 = ttt.Create(TestsAccounts.getAccount(3));

        TicTacToe.Game memory game = ttt.State(game_id_1);
        Assert.equal(game.p1, TestsAccounts.getAccount(0), "Player 1 is not TestsAccounts.getAccount(0)");
        Assert.equal(game.p2, address(0), "Player 2 is not address(0)");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.New), "Game state should be new.");
        Assert.equal(game.turn, false, "Turn should be false");
        Assert.equal(game.winner, false, "Winner should be false");
        for (uint8 i = 0; i < 9; ++i) {
            Assert.equal(uint8(game.board[i]), uint8(TicTacToe.BoardState.Unset), "Board should be unset");
        }

        game = ttt.State(game_id_2);
        Assert.equal(game.p1, TestsAccounts.getAccount(3), "Player 1 is not TestsAccounts.getAccount(3)");
        Assert.equal(game.p2, address(0), "Player 2 is not address(0)");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.New), "Game state should be new.");
        Assert.equal(game.turn, false, "Turn should be false");
        Assert.equal(game.winner, false, "Winner should be false");
        for (uint8 i = 0; i < 9; ++i) {
            Assert.equal(uint8(game.board[i]), uint8(TicTacToe.BoardState.Unset), "Board should be unset");
        }

        try ttt.State(3) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "State(3) should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid game.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkJoinGamePlayer1Turn() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));

        TicTacToe.Game memory game = ttt.State(game_id_1);
        Assert.equal(game.p1, TestsAccounts.getAccount(0), "Player 1 is not TestsAccounts.getAccount(0)");
        Assert.equal(game.p2, TestsAccounts.getAccount(3), "Player 2 is not TestsAccounts.getAccount(3)");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.Active), "Game state should be active.");
        Assert.equal(game.turn, false, "Turn should be false");
        Assert.equal(game.winner, false, "Winner should be false");
        for (uint8 i = 0; i < 9; ++i) {
            Assert.equal(uint8(game.board[i]), uint8(TicTacToe.BoardState.Unset), "Board should be unset");
        }
    }

    function checkJoinGamePlayer2Turn() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(2));

        TicTacToe.Game memory game = ttt.State(game_id_1);
        Assert.equal(game.p1, TestsAccounts.getAccount(0), "Player 1 is not TestsAccounts.getAccount(0)");
        Assert.equal(game.p2, TestsAccounts.getAccount(2), "Player 2 is not TestsAccounts.getAccount(2)");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.Active), "Game state should be active.");
        Assert.equal(game.turn, true, "Turn should be true");
        Assert.equal(game.winner, false, "Winner should be false");
        for (uint8 i = 0; i < 9; ++i) {
            Assert.equal(uint8(game.board[i]), uint8(TicTacToe.BoardState.Unset), "Board should be unset");
        }
    }

    function checkJoinInProgressGame() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));
        TicTacToe.Game memory game = ttt.State(game_id_1);
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.Active), "Game state should be active.");

        try ttt.Join(game_id_1, TestsAccounts.getAccount(2)) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Join should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Game is no longer available.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkJoinInvalidGame() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));

        try ttt.Join(game_id_1 + 1, TestsAccounts.getAccount(3)) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Join should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid game.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkMove() public {        
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 0);
        TicTacToe.Game memory game = ttt.State(game_id_1);       
        Assert.equal(game.turn, true, "Turn should be false");
        Assert.equal(game.winner, false, "Winner should be false");
        Assert.equal(uint8(game.board[0]), uint8(TicTacToe.BoardState.Player1), "Position should be Player1");
        for (uint8 i = 1; i < 9; ++i) {
            Assert.equal(uint8(game.board[i]), uint8(TicTacToe.BoardState.Unset), "Board should be unset");
        }

        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 2, 1);
        game = ttt.State(game_id_1);       
        Assert.equal(game.turn, false, "Turn should be false");
        Assert.equal(game.winner, false, "Winner should be false");
        Assert.equal(uint8(game.board[5]), uint8(TicTacToe.BoardState.Player2), "Position should be Player2");
        for (uint8 i = 0; i < 9; ++i) {
            if (i == 0 || i == 5) {
                continue;
            }
            Assert.equal(uint8(game.board[i]), uint8(TicTacToe.BoardState.Unset), "Board should be unset");
        }
    }

    function checkMoveInvalidGame() public {
        TicTacToe ttt = new TicTacToe();
        try ttt.Move(1, TestsAccounts.getAccount(0), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid game.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkMoveNewGame() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        try ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Game is no longer active.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkMoveWrongGame() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));
        try ttt.Move(game_id_1, TestsAccounts.getAccount(2), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid player.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkMoveWrongTurn() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));
        try ttt.Move(game_id_1, TestsAccounts.getAccount(3), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid turn.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkMoveInvalidPosition() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));

        try ttt.Move(game_id_1, TestsAccounts.getAccount(0), 5, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid move.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }

        try ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 5) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid move.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkMoveOccupiedPosition() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 0);

        try ttt.Move(game_id_1, TestsAccounts.getAccount(3), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Invalid move.  Position already occupied.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkPlayToDraw() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 0);
        TicTacToe.Game memory game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[0]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");

        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 1, 0);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[1]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 2, 0);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[2]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 0, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[3]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 2, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[5]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 1, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[4]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 2);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[6]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 2, 2);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[8]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 1, 2);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[7]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        Assert.equal(game.turn, true, "Turn should be true");
        Assert.equal(game.winner, false, "Winner should be false");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.Draw), "Game state should be draw.");

        try ttt.Join(game_id_1, TestsAccounts.getAccount(3)) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Game is no longer available.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }

        try ttt.Move(game_id_1, TestsAccounts.getAccount(3), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Game is no longer active.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkPlayer1Wins() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 0);
        TicTacToe.Game memory game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[0]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");

        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 1, 0);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[1]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 2, 0);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[2]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 0, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[3]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 1, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[4]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 2, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[5]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 2);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[6]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        Assert.equal(game.winner, false, "Winner should be false");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.Winner), "Game state should be draw.");        
        
        try ttt.Join(game_id_1, TestsAccounts.getAccount(3)) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Game is no longer available.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }

        try ttt.Move(game_id_1, TestsAccounts.getAccount(3), 0, 0) /*returns (TicTacToe.Game memory g)*/ {
            Assert.ok(false, "Move should fail.");
        } catch Error(string memory reason) {
            Assert.equal(reason, "Game is no longer active.", "Failed with unexpected reason");
        } catch (bytes memory /*lowLevelData*/) {
            Assert.ok(false, "Unexpected failure.");
        }
    }

    function checkPlayer2Wins() public {
        TicTacToe ttt = new TicTacToe();
        uint64 game_id_1 = ttt.Create(TestsAccounts.getAccount(0));
        ttt.Join(game_id_1, TestsAccounts.getAccount(3));

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 0);
        TicTacToe.Game memory game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[0]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");

        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 1, 0);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[1]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");

        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 2, 0);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[2]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 1, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[4]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 0, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[3]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 2, 1);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[5]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(0), 2, 2);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[8]), uint8(TicTacToe.BoardState.Player1), "Board set incorrectly");
        
        ttt.Move(game_id_1, TestsAccounts.getAccount(3), 1, 2);
        game = ttt.State(game_id_1);  
        Assert.equal(uint8(game.board[7]), uint8(TicTacToe.BoardState.Player2), "Board set incorrectly");

        Assert.equal(game.winner, true, "Winner should be false");
        Assert.equal(uint8(game.state), uint8(TicTacToe.GameState.Winner), "Game state should be draw.");   
    }
}
    