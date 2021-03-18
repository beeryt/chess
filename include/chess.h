#pragma once
#include <vector>
#include <string>

namespace chess {
  using std::vector;
  using std::string;

  struct Piece {
    enum Team { White, Black };
    enum Type { Pawn, Knight, Bishop, Rook, Queen, King };
    Type type : 3;
    Team team : 1;
    uint8_t coord : 6;
  };

  namespace FEN {
    const string Start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }

  struct Coordinate {
    uint8_t idx;
    Coordinate(int x, int y);
    Coordinate(int idx = -1);
    operator bool() const;
    bool operator==(const Coordinate&) const;
    bool operator!=(const Coordinate&) const;
  };

  class Board {
    public:
      Board(const string& FEN = FEN::Start);
      void debug() const;
      const vector<Piece>& pieces = m_pieces;

      static std::string coordToStr(int x, int y);
      static std::string coordToStr(int);
      static std::string coordToStr(Coordinate);

    private:
      Piece::Team active;
      std::vector<Piece> m_pieces;
  };

  std::ostream& operator<<(std::ostream& os, const Piece& p);
}

