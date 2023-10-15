import chess
import chess.pgn

pgnf = open('AI/pgnFile.txt','w')

with open('AI/simgames.txt') as f:
  game = chess.pgn.Game()
  movenum = 0
  for line in f:
    if line.strip()=="":
      print(game, file=pgnf)
      game = chess.pgn.Game()
      movenum = 0
      continue
    try:
      if movenum==0:
        node = game.add_variation(chess.Move.from_uci(line.strip()))
        movenum=1
      else:
        node = node.add_variation(chess.Move.from_uci(line.strip()))
    except:
      print(game, file=pgnf)
      print("ERROR: trying move ", line, " for position ", game.board().fen())
  print(game, file=pgnf)