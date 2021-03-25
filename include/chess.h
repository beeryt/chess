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
    /// @brief Equality operator for two coordinates.
    /// @param o A Coordinate to compare.
    /// @returns True if equal, False otherwise.
    bool operator==(const Coordinate&) const;
    /// @brief Inequality operator for two coordinates.
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
    /// Whether this piece belongs to white.
    bool white : 1;
    /// @brief Default constructor creates an invalid piece.
    /// @param type The Piece::Type of the Piece.
    /// @param coordinate The chess::Coordinate of the Piece.
    /// @param white Whether this Piece belongs to white.
    Piece(Type type = None, Coordinate coordinate = Coordinate(), bool white = true);
  };

  namespace FEN {
    const string Start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }

  /// A chess::Move describes a chess move in long algebraic notation.
  struct Move {
    /// @todo Add attributes for check, checkmate, and any other LAN terms missed.
    /// The starting Coordinate of the move.
    Coordinate start;
    /// The ending Coordinate of the move.
    Coordinate end;
    /// For pawn promotions, which Piece::Type to promote to.
    Piece::Type promote;
    /// @brief Evaluates as True if Move is valid, False otherwise.
    /// @note A valid move is not necessarily a legal move.
    /// @see Board::IsLegalMove
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

      /// @brief Verifies whether a move is legal on the current board.
      /// @param move A Move to check.
      /// @returns true if the move is legal, false otherwise.
      bool IsLegalMove(Move move) const;
      /// @brief Attempts to make a move on the board.
      /// @param move A Move to attempt to make.
      /// @returns true if move was successful, false otherwise.
      /// @note A false return will not modify the board state.
      bool MakeMove(Move move);

      /// @brief Gets all pieces on the board.
      /// @returns List of pieces on the board.
      const std::vector<Piece> AllPieces() const;
      /// @brief Get the Piece::Type at a given Coordinate.
      /// @param c A Coordinate to check.
      /// @returns A Piece::Type that was found at Coordinate.
      /// @note May be Piece::None if Coordinate was invalid.
      Piece::Type GetPieceTypeAt(Coordinate c) const;
      /// @brief Get the Piece at a given Coordinate.
      /// @param c A Coordinate to check.
      /// @returns A Piece found at Coordinate c.
      /// @note May be an invalid Piece. Check that Piece::type != Piece::None.
      Piece GetPieceAt(Coordinate c) const;

      /// @brief Retrieves a bitboard of the current active player's pieces.
      /// @returns A bitboard populated with active player piece presence.
      bitboard ActivePieces() const;
      /// @brief A query for whether a piece belongs to the active player.
      /// @param p A Piece to check.
      /// @todo Verify whether the piece actually exists within the board.
      /// @returns true if the piece belongs to the active player, false otherwise.
      bool IsActive(Piece p) const;
      /// @brief A query for whether a coordinate is a moveable piece owned by the active player.
      /// @param c A Coordinate to check.
      /// @returns true if coordinate is a moveable piece, false otherwise.
      bool IsActive(Coordinate c) const;

      /// @brief Whether it is white's turn to move.
      /// @returns true if it is white's turn to move, false otherwise.
      bool WhiteToMove() const;
      /// @brief Whether it is black's turn to move.
      /// @returns true if it is black's turn to ovme, false otherwise.
      bool BlackToMove() const;

    private:
      bool white_to_move = true;
      std::unordered_map<Piece::Type, bitboard> white_pieces;
      std::unordered_map<Piece::Type, bitboard> black_pieces;
  };

  std::ostream& operator<<(std::ostream& os, const Piece& p);
  std::ostream& operator<<(std::ostream& os, const Coordinate& c);
  std::ostream& operator<<(std::ostream& os, const Move& m);
}

