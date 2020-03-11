import chess
import sys

def perft(depth, board):
    if depth == 1:
        return board.legal_moves.count()
    elif depth > 1:
        count = 0

        for move in board.legal_moves:
            board.push(move)
            count += perft(depth - 1, board)
            board.pop()

        return count
    else:
        return 1

s = sys.argv[2]
depth = int(sys.argv[1])

board = chess.Board(s)
print(board)
print(perft(depth, board))
