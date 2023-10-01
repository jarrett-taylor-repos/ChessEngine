import chess
import chess.pgn

movesfile = open('./tests/test_files/validFENs.txt','r')
piecesfiles = open('./tests/test_files/pieceLocations.txt','w')

b = chess.Board()
print(b)
print('')
b.apply_transform(chess.flip_vertical)
print(b)
i = 0
for fen in movesfile.readlines():
  print(fen,file=piecesfiles)
  b = chess.Board(fen)
  b.apply_transform(chess.flip_vertical)
  for pieceColor in [True, False]:
    for piece in range(1,7):
      outputstring = ''
      for square in b.pieces(piece,pieceColor).tolist():
        if square:
          outputstring+='1'
        else: outputstring+='0'
      print(outputstring, file=piecesfiles)