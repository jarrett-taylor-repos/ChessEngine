#This creates a files with a list of valid moves.  To be used for testing
#The files will have the move in uci notation
  #i.e. e2e4 and a7a8q for promotion
#Each line is a new move.  Whenever a new game is started, `newgame` is printed in the file
#Right now is is limited to 100k moves

import chess
import chess.pgn

movesfile = open('./tests/test_files/validmoves1.txt','w')
chessfile = open('D:/DATA/lichess_db_standard_rated_2022-01.pgn/lichess_db_standard_rated_2022-01.pgn')


nummoves = 0

while nummoves < 1000000:
  game = chess.pgn.read_game(chessfile)
  if "Variant" not in game.headers:
    for move in game.mainline_moves():
      nummoves+=1
      print(move.uci(), file=movesfile)
    print('newgame', file=movesfile)
