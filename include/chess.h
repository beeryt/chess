#pragma once
#include <vector>
#include <string>

namespace chess {
  using std::vector;
  using std::string;

  /// A Piece encodes the Type (e.g. Pawn) and Coordinate in a 9 bits.
  struct Piece {
    /// @todo Remove since Piece does not need to store color.
    enum Team { White, Black };
    /// Piece::Type enumerates the standard chess pieces.
    enum Type { Pawn, Knight, Bishop, Rook, Queen, King };
    /// The Piece::Type can be encoded into 3 bits.
    Type type : 3;
    /// @todo Remove since it is not necessary to store color.
    Team team : 1;
    /// The Coordinate can be encoded into 6 bits.
    uint8_t coord : 6;
  };

  namespace FEN {
    const string Start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }

  /// A Coordinate encodes the x- and y-coordinates as a single index.
  struct Coordinate {
    /// The x- and y-coordinates are encoded into a single index.
    uint8_t idx;
    /// @brief Constructs a Coordinate from two x- and y-coordinates.
    /// @note If coordinates are out-of-range, idx will be -1.
    /// @param x The x-coordinate with valid range [0..7].
    /// @param y The y-coordinate with valid range [0..7].
    Coordinate(int x, int y);
    /// @brief Constructs a Coordinate from a single index.
    /// @note An out-of-range idx will clip to -1.
    /// @param idx An encoded coordinate with valid range [0..63].
    Coordinate(int idx = -1);
    /// Evaluates as True if Coordinate is valid, False otherwise.
    operator bool() const;
    /// @param o A Coordinate to compare.
    /// @returns True if equal, False otherwise.
    bool operator==(const Coordinate&) const;
    /// @param o A Coordinate to compare.
    /// @returns False if equal, True otherwise.
    bool operator!=(const Coordinate&) const;
  };

  struct Move {
  };

  /// A chess::Board manages chess::Piece%s and validates chess::Move%s.
  class Board {
    public:
      /// @brief FEN string constructor for Board (defaults to FEN::Start).
      /// @param FEN A FEN string for the Board state.
      Board(const string& FEN = FEN::Start);
      /// @brief Prints useful information for debugging the Board state.
      /// @todo Remove this function.
      void debug() const;
      /// @todo Consider removing
      const vector<Piece>& pieces = m_pieces;

      /// @todo Consider removing
      /// @param x,y A Coordinate.
      /// @returns A formatted Coordinate string.
      static std::string coordToStr(int x, int y);
      /// @todo Consider removing
      /// @param index A Coordinate index.
      /// @returns A formatted Coordinate string.
      static std::string coordToStr(int index);
      /// @todo Consider removing
      /// @param coord A Coordinate.
      /// @returns A formatted Coordinate string.
      static std::string coordToStr(Coordinate coord);

    private:
      Piece::Team active;
      std::vector<Piece> m_pieces;
  };

  std::ostream& operator<<(std::ostream& os, const Piece& p);
}

