import chess
import sys

def perft(board):
    return board.legal_moves.count()


s = sys.argv[1]
number_move_prog = int(sys.argv[2])
board = chess.Board(s)
number_move_expected = perft(board)
if number_move_expected != number_move_prog:
    with open("bad_board", "a") as fichier:
        fichier.write("-------------------------\n")
        fichier.write(s + "\n")
        fichier.write("our number of move : " + str(number_move_prog) + "\n")
        fichier.write("expected number of move : " + str(number_move_expected) + "\n")
        fichier.write("-------------------------\n")

