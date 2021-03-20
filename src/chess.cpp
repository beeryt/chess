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

Piece::Piece(Type type, Coordinate coord) : type(type), coord(coord.idx) {}

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
            default: printf("TODO: unrecognized token: %c\n", c); break;
          }
          if (isupper(c)) white.push_back({ type, idx++ });
          else black.push_back({ type, idx++ });
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

std::ostream& chess::operator<<(std::ostream& os, const Coordinate& c) {
  os << static_cast<char>('A' + c.idx % 8);
  os << static_cast<char>('8' - c.idx / 8);
  return os;
}

