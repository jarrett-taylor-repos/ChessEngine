import chess
import chess.pgn
from tools.boolListToString import boolListToString

movesfile = open('./tests/test_files/validFENs.txt','r')
piecesfiles = open('./tests/test_files/pieceLocations.txt','w')

for fen in movesfile.readlines():
  b = chess.Board(fen)
  print(b.fen(),file=piecesfiles)
  b.apply_transform(chess.flip_horizontal)
  for pieceColor in [True, False]:
    for piece in range(1,7):
      print(boolListToString(b.pieces(piece,pieceColor).tolist()), file=piecesfiles)