import chess
import chess.pgn

jert_file = open("tests/test_files/jert_possible_moves.txt")
errfile = open("tests/possible_moves_errors.txt",'w')

lines = jert_file.readlines()
for line in lines:
  [FEN, jertFEN, movesString] = line.split("|")

    
  if FEN!=jertFEN:
    print(f"when inputting FEN: {FEN}, program outputed FEN {jertFEN}", file=errfile) 

  jertMoves = movesString.split(",")
  jertMoves.pop() # gets rid of \n at the end
  jertMoves = [move.strip() for move in jertMoves]

  
  b = chess.Board(FEN)
  pyMoves = b.legal_moves
  rightMoves = []
  for move in pyMoves:
    rightMoves.append(move.uci())
  
  extraMoves = list(set(jertMoves)-set(rightMoves))
  missingMoves = list(set(rightMoves)-set(jertMoves))

  if extraMoves: print(f"For FEN {FEN}, there are extra move(s): {extraMoves}", file=errfile)
  if missingMoves: print(f"For FEN {FEN}, there are missing move(s): {missingMoves}", file=errfile)

