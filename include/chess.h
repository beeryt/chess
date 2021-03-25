#pragma once
#include <vector>
#include <bitset>
#include <string>
#include <unordered_map>

namespace chess {
  using std::vector;
  using std::string;

  using bitboard = std::bitset<64>;

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
    bool white : 1;
    /// @brief Default constructor creates an invalid piece.
    /// @param type The Piece::Type of the Piece.
    /// @param coordinate The chess::Coordinate of the Piece.
    Piece(Type type = None, Coordinate coordinate = Coordinate(), bool white = true);
  };

  namespace FEN {
    const string Start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }

  /// A chess::Move describes a chess move.
  /// @todo revisit this brief description.
  struct Move {
    Coordinate start;
    Coordinate end;
    Piece::Type promote;
    operator bool() const;
    /// @brief Creates a chess::Move from Algebraic notation.
    /// @param an A long algebraic notation string.
    Move(const std::string& an);
    /// @brief Creates a chess::Move from chess::Coordinates.
    /// @param start The starting Coordinate.
    /// @param end The ending Coordinate.
    /// @param promote For promotions, what Piece::Type to promote to.
    Move(Coordinate start, Coordinate end, Piece::Type promote = Piece::Type::None);
  };

  /// A chess::Board manages chess::Piece%s and validates chess::Move%s.
  class Board {
    public:
      /// @brief FEN string constructor for Board (defaults to FEN::Start).
      /// @param FEN A FEN string for the Board state.
      Board(const string& FEN = FEN::Start);


      Piece GetPieceAt(Coordinate) const;
      bool IsActive(Piece) const;
      bool WhiteToMove() const;
      bool BlackToMove() const;

      Piece::Type GetPieceTypeAt(Coordinate) const;

      bool IsLegalMove(Move) const;
      bool MakeMove(Move);

      const std::vector<Piece> AllPieces() const;

    private:
      bool white_to_move = true;
      std::unordered_map<Piece::Type, bitboard> white_pieces;
      std::unordered_map<Piece::Type, bitboard> black_pieces;
  };

  std::ostream& operator<<(std::ostream& os, const Piece& p);
  std::ostream& operator<<(std::ostream& os, const Coordinate& c);
  std::ostream& operator<<(std::ostream& os, const Move& m);
}

