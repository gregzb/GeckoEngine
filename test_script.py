import chess

fen = "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2"
board = chess.Board(fen)

legal_move_count = len(list(board.legal_moves))

print(legal_move_count)