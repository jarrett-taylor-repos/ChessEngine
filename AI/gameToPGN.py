import chess
import chess.pgn

pgnf = open('AI/pgnFile.txt','w')

with open('AI/simgames.txt') as f:
  game = chess.pgn.Game()
  # game.setup("r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R w KQkq - 4 4")
  movenum = 0
  for line in f:
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
    except Exception as error:
      print(game, file=pgnf)
      print(game)
      # print("ERROR: trying move ", line, " for position ", game.board().fen())
      print(error)
  print(game, file=pgnf)
  print(game)