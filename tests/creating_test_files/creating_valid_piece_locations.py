import chess
import chess.pgn

movesfile = open('./tests/test_files/validFENs.txt','r')
piecesfiles = open('./tests/test_files/pieceLocations.txt','w')

for fen in movesfile.readlines():
  b = chess.Board(fen)
  print(b.fen(),file=piecesfiles)
  b.apply_transform(chess.flip_horizontal)
  for pieceColor in [True, False]:
    for piece in range(1,7):
      outputstring = ''
      for square in b.pieces(piece,pieceColor).tolist():
        if square:
          outputstring+='1'
        else: outputstring+='0'
      print(outputstring, file=piecesfiles)