#This creates a files with a list of valid FENS.  To be used for testing
#Each line is a new FEN.  
#Right now is is limited to 100k FENS

import chess
import chess.pgn

movesfile = open('./tests/test_files/validFENs.txt','w')
chessfile = open('D:/DATA/lichess_db_standard_rated_2022-01.pgn/lichess_db_standard_rated_2022-01.pgn') #this is where a massive pgn in my hard drive is


numFENs = 0
allfens = []
while numFENs < 100000:
  game = chess.pgn.read_game(chessfile)
  b = chess.Board()
  #TODO: check that this is a normal game and not chess 960?
  for move in game.mainline_moves():
    b.push(move)
    fen = b.fen()
    if fen not in allfens:
      allfens.append(fen)
      numFENs+=1
      print(b.fen(), file=movesfile)
