#pragma once
#include <vector>
#include <string>

namespace chess {
  using std::vector;
  using std::string;

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

  /// A Piece encodes the Piece::Type (e.g. Pawn) and chess::Coordinate in to 9 bits.
  struct Piece {
    /// Piece::Type enumerates the standard chess pieces.
    enum Type { None, Pawn, Knight, Bishop, Rook, Queen, King };
    /// The Piece::Type is encoded in 3 bits.
    Type type : 3;
    /// The chess::Coordinate is encoded in 6 bits.
    uint8_t coord : 6;
    /// @brief Default constructor creates an invalid piece.
    /// @param type The Piece::Type of the Piece.
    /// @param coordinate The chess::Coordinate of the Piece.
    Piece(Type type = None, Coordinate coordinate = Coordinate());
  };

  namespace FEN {
    const string Start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }

  struct Move {
  };

  /// A chess::Board manages chess::Piece%s and validates chess::Move%s.
  class Board {
    public:
      /// @brief FEN string constructor for Board (defaults to FEN::Start).
      /// @param FEN A FEN string for the Board state.
      Board(const string& FEN = FEN::Start);

      /// List of white pieces.
      const std::vector<Piece>& white_pieces = white;
      /// List of black pieces.
      const std::vector<Piece>& black_pieces = black;

    private:
      bool white_to_move = true;
      std::vector<Piece> white;
      std::vector<Piece> black;
  };

  std::ostream& operator<<(std::ostream& os, const Piece& p);
  std::ostream& operator<<(std::ostream& os, const Coordinate& c);
}

