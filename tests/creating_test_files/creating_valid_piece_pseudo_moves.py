import chess
import chess.pgn
from tools.boolListToString import boolListToString

FENsfile = open('./tests/test_files/validFENs.txt','r')
pseudomovesfile = open('./tests/test_files/pseudoMoves.txt','w')
def square_file(square) -> int:
    """Gets the file index of the square where ``0`` is the a-file."""
    return square & 7
for fen in FENsfile.readlines():
  b = chess.Board(fen)
  print(b.fen(),file=pseudomovesfile)
  b.apply_transform(chess.flip_horizontal)
  b2 = b.copy()
  b2.push(chess.Move.null())
  if b.turn:
    pseudomoves = [b2.pseudo_legal_moves,b.pseudo_legal_moves]
  else:
    pseudomoves = [b.pseudo_legal_moves,b2.pseudo_legal_moves]
  for pieceColor in [True, False]:
    for piece in range(1,7):
      squareset = chess.SquareSet()
      for move in pseudomoves[pieceColor]: #this is the slower way but whatever
        if b.is_castling(move):
          diff = square_file(move.from_square) - square_file(move.to_square)
          print(abs(diff)>1)
          print(bool(b.rooks & b.occupied_co[b.turn]))
          print(b.is_castling(move), b.is_kingside_castling(move), b.is_queenside_castling(move), '\n', b.fen(), move.uci(), move.from_square, move.to_square)
          print('\n\n')
        # print(piece, b.piece_type_at(move.from_square), pieceColor, b.color_at(move.from_square))
        if b.piece_type_at(move.from_square)==piece and b.color_at(move.from_square)==pieceColor: # and not b.is_en_passant(move):
          # print(piece, b.piece_type_at(move.from_square), pieceColor, b.color_at(move.from_square))
          squareset.add(move.to_square)  
      print(boolListToString(squareset.tolist()), file=pseudomovesfile)