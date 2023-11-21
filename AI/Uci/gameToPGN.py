import chess
import chess.pgn

pgnf = open('AI/pgnFile.txt','w')

with open('AI/simgames.txt') as f:
  game = chess.pgn.Game()
  # game.setup("rnb1k2r/1pqp1ppp/p3pn2/8/1b1NP3/2N1BP2/PPPQ2PP/R3KB1R b KQkq - 0 8")
  game.setup("rnbq1rk1/ppp1bppp/4pn2/3p2B1/2PP4/2N1PN2/PP3PPP/R2QKB1R b KQ - 0 6")
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