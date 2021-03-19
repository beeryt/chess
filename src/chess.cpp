#include <chess.h>
#include <iostream>

using namespace chess;

using std::cout;
using std::endl;

Coordinate::operator bool() const { return idx < 64; }
Coordinate::Coordinate(int idx) : idx(idx) {
  if (!(*this)) { idx = -1; }
}
Coordinate::Coordinate(int x, int y) : Coordinate(x * 8 + y) {
  if (x < 0 || x > 7 || y < 0 || y > 7) { idx = -1; }
}
bool Coordinate::operator==(const Coordinate& o) const { return idx == o.idx; }
bool Coordinate::operator!=(const Coordinate& o) const { return idx != o.idx; }

Board::Board(const string& FEN) {
  cout << "FEN: " << FEN << endl;
  { // process FEN
    int field = 0;
    uint8_t idx = 0;
    for (char c : FEN) {
      if (c == ' ') { field++; continue; }
      Piece::Team color;
      Piece::Type type;
      switch (field) {
        case 0: // Piece Placement
          if (isdigit(c)) { idx += (c - '0'); continue; }
          if (c == '/') { continue; }
          color = isupper(c) ? Piece::Team::White : Piece::Team::Black;
          switch (toupper(c)) {
            case 'P': type = Piece::Type::Pawn; break;
            case 'N': type = Piece::Type::Knight; break;
            case 'B': type = Piece::Type::Bishop; break;
            case 'R': type = Piece::Type::Rook; break;
            case 'Q': type = Piece::Type::Queen; break;
            case 'K': type = Piece::Type::King; break;
            default: printf("TODO: unrecognized token: %c\n", c); break;
          }
          m_pieces.push_back({ type, color, idx++ });
          break;

        case 1: // Active Color
          if (c == 'w') { color = Piece::White; }
          else if (c == 'b') { color = Piece::Black; }
          else { printf("TODO: unrecognized token: %c\n", c); }
          active = color;
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

std::string Board::coordToStr(Coordinate c) { return Board::coordToStr(c.idx); }
std::string Board::coordToStr(int idx) { return Board::coordToStr(idx % 8, idx / 8); }
std::string Board::coordToStr(int x, int y) {
  if (x < 0 || x > 7 || y < 0 || y > 7) return "";
  std::string out;
  out += static_cast<char>('A' + x);
  out += static_cast<char>('8' - y);
  return out;
}

std::ostream& chess::operator<<(std::ostream& os, const Piece& p) {
  switch (p.team) {
    case Piece::White: os << "White "; break;
    case Piece::Black: os << "Black "; break;
  }
  switch (p.type) {
    case Piece::Pawn: os << "Pawn"; break;
    case Piece::Knight: os << "Knight"; break;
    case Piece::Bishop: os << "Bishop"; break;
    case Piece::Rook: os << "Rook"; break;
    case Piece::Queen: os << "Queen"; break;
    case Piece::King: os << "King"; break;
  }
  os << " ";
  os << static_cast<char>('A' + p.coord % 8);
  os << static_cast<char>('8' - p.coord / 8);
  return os;
}

void Board::debug() const {
  for (const auto& p : pieces) {
    cout << p << endl;
  }
}

