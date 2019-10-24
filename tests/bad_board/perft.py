import chess
import sys

def perft(board, depth):
	n = 0;
	if (depth == 0):
		return 1
	f = open("kek", "a")
	for i in board.legal_moves:
		f.write(str(i) + "\n")
		board.push(i)
		n += perft(board, depth - 1)
		board.pop()
	return n


s = sys.argv[1]
board = chess.Board(s)
number_move_expected = perft(board, int(sys.argv[2]))
print(number_move_expected)
