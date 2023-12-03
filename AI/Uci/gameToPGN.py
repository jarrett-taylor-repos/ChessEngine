import chess
import chess.pgn

#need to open from here
pgnf = open('pgnFile.txt','w')

with open('simgames.txt') as f:
  game = chess.pgn.Game()
  movenum = 0
  linenum = 0
  for line in f:
    if linenum == 0:
      game.setup(line)
      linenum=1
      continue
    if line.strip()=="":
      print(game)
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
      print(game, file=pgnf)
      print("variations:", game.variations, file=pgnf)
    except AssertionError as error:
      print("HERE")
      print("variations", node.variations)
      # game.remove_variation()
      game.remove_variation(chess.Move.from_uci(line.strip()))
      # print(game, file=pgnf)
      # print(game)
      # print("ERROR: trying move ", line, " for position ", game.board().fen())
      # print("ERROR", error)
      print("-----------------------------")
      break;
  # print(game, file=pgnf)
  print(game)