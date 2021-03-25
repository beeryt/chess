#include <chess.h>
#include <iostream>

using namespace chess;

using std::cout;
using std::endl;

Coordinate::operator bool() const { return idx < 64; }
Coordinate::Coordinate(int idx) : idx(idx) {
  if (!(*this)) { idx = -1; }
}
Coordinate::Coordinate(int x, int y) : Coordinate(x + y * 8) {
  if (x < 0 || x > 7 || y < 0 || y > 7) { idx = -1; }
}
bool Coordinate::operator==(const Coordinate& o) const { return idx == o.idx; }
bool Coordinate::operator!=(const Coordinate& o) const { return idx != o.idx; }

Piece::Piece(Type type, Coordinate coord, bool white) :
  type(type),
  coord(coord.idx),
  white(white) {}

Move::operator bool() const {
  return start && end;
}

Move::Move(Coordinate start, Coordinate end, Piece::Type promote) :
  start(start),
  end(end),
  promote(promote) {}

Board::Board(const string& FEN) {
  cout << "FEN: " << FEN << endl;
  { // process FEN
    int field = 0;
    uint8_t idx = 0;
    for (char c : FEN) {
      if (c == ' ') { field++; continue; }
      auto type = Piece::Type();
      switch (field) {
        case 0: // Piece Placement
          if (isdigit(c)) { idx += (c - '0'); continue; }
          if (c == '/') { continue; }
          switch (toupper(c)) {
            case 'P': type = Piece::Type::Pawn; break;
            case 'N': type = Piece::Type::Knight; break;
            case 'B': type = Piece::Type::Bishop; break;
            case 'R': type = Piece::Type::Rook; break;
            case 'Q': type = Piece::Type::Queen; break;
            case 'K': type = Piece::Type::King; break;
            default: break;
          }
          if (type != Piece::None) {
            auto& pieces = isupper(c) ? white_pieces : black_pieces;
            pieces[type].set(idx++);
          }
          break;

        case 1: // Active Color
          if (c == 'w') white_to_move = true;
          else if (c == 'b') white_to_move = false;
          else { printf("TODO: unrecognized token: %c\n", c); }
          break;

        case 2: // Castling Availability
          break;

        case 3: // En passant Target Square
          break;

        case 4: // Halfmove Clock
          break;

        case 5: // Fullmove Clock
          break;
      }
    }
  }
}

bool Board::WhiteToMove() const { return white_to_move; }
bool Board::BlackToMove() const { return !white_to_move; }

Piece Board::GetPieceAt(Coordinate c) const {
  for (const auto& p : AllPieces()) {
    if (p.coord == c.idx) return p;
  }
  return Piece{};
}

bool Board::IsActive(Piece p) const {
  return white_to_move == p.white;
}

bool Board::IsLegalMove(Move m) const {
  if (!m) return false;

  Piece::Type occupied = Piece::None;
  Piece::Type type = Piece::None;
  auto& pieces = white_to_move ? white_pieces : black_pieces;
  for (auto map : pieces) {
    if (map.second.test(m.start.idx)) type = map.first;
    if (map.second.test(m.end.idx)) occupied = map.first;
  }

  // Could not find the starting piece. Not a legal move.
  if (type == Piece::None) return false;

  // Attempting to capture own piece. Not a legal move.
  // @todo castling may be a false positive here.
  if (occupied != Piece::None) return false;

  return true;
}

using PieceMap = std::unordered_map<Piece::Type, bitboard>;
const std::vector<Piece> Board::AllPieces() const {
  std::vector<Piece> out;

  auto convert = [&out](bool white, PieceMap pieces) {
    for (const auto& map : pieces) {
      for (uint8_t i = 0; i < map.second.size(); ++i) {
        if (!map.second.test(i)) continue;
        out.push_back({ map.first, i, white });
      }
    }
  };

  convert(true, white_pieces);
  convert(false, black_pieces);
  return out;
}

Piece::Type Board::GetPieceTypeAt(Coordinate c) const {
  for (auto p : AllPieces()) {
    /// @note assumes single piece is at c (valid for valid games)
    if (p.coord == c.idx) { return p.type; }
  }
  return Piece::Type::None;
}

bool Board::MakeMove(Move m) {
  if (!IsLegalMove(m)) return false;

  auto& pieces = white_to_move ? white_pieces : black_pieces;
  auto& enemy = white_to_move ? black_pieces : white_pieces;
  Piece::Type type = GetPieceTypeAt(m.start);

  // move piece to new square
  pieces[type] &= ~bitboard{ 1ul << m.start.idx };
  pieces[type] |= bitboard{ 1ul << m.end.idx };

  // capture enemy piece
  for (auto& map : enemy) {
    map.second &= ~bitboard{ 1ul << m.end.idx };
  }

  white_to_move = !white_to_move;
  return true;
}

std::ostream& chess::operator<<(std::ostream& os, const Coordinate& c) {
  os << static_cast<char>('A' + c.idx % 8);
  os << static_cast<char>('8' - c.idx / 8);
  return os;
}

std::ostream& chess::operator<<(std::ostream& os, const Piece& p) {
  switch (p.type) {
    /// @todo re-order to match enum
    case Piece::Pawn: os << "Pawn"; break;
    case Piece::Bishop: os << "Bishop"; break;
    case Piece::Knight: os << "Knight"; break;
    case Piece::Rook: os << "Rook"; break;
    case Piece::Queen: os << "Queen"; break;
    case Piece::King: os << "King"; break;
    default: break;
  }
  return os;
}

