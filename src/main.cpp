#include <graphics.h>
#include <chess.h>
#include <thread>
#include <chrono>
#include <map>
#include <iostream>
using std::cout;
using std::endl;

#define SCALE 256

Color WHITE{ 216, 191, 161 };
Color BLACK{ 158, 116, 66 };

using SpriteMap = std::map<chess::Piece::Type, Sprite>;
SpriteMap white_sprites;
SpriteMap black_sprites;

void initialize_sprites(Texture& texture, SpriteMap& sprites, unsigned offset) {
  Sprite defaultSprite{ &texture };
  defaultSprite.setRegion(texture.getRegion());
  defaultSprite.setHFrames(6);
  defaultSprite.setVFrames(2);

  sprites[chess::Piece::King]   = Sprite{ defaultSprite, 0 + offset };
  sprites[chess::Piece::Queen]  = Sprite{ defaultSprite, 1 + offset };
  sprites[chess::Piece::Bishop] = Sprite{ defaultSprite, 2 + offset };
  sprites[chess::Piece::Knight] = Sprite{ defaultSprite, 3 + offset };
  sprites[chess::Piece::Rook]   = Sprite{ defaultSprite, 4 + offset };
  sprites[chess::Piece::Pawn]   = Sprite{ defaultSprite, 5 + offset };
}

inline int floor_div(int a, int b) {
  int d = a / b;
  int r = a % b;
  return r ? (d - ((a < 0) ^ (b < 0))) : d;
}

chess::Coordinate ScreenToCoordinate(int x, int y) {
  return chess::Coordinate{
    floor_div(x,SCALE),
    floor_div(y,SCALE)
  };
}

void CoordinateToScreen(chess::Coordinate c, int& x, int& y) {
  x = (c.idx % 8) * SCALE;
  y = (c.idx / 8) * SCALE;
}

void drawBoard(Graphics& gfx) {
  for (int file = 0; file < 8; ++file) {
    for (int rank = 0; rank < 8; ++rank) {
      bool even = (file + rank) % 2 == 0;
      Color color = even ? WHITE : BLACK;
      int x,y,w,h;
      w = h = SCALE;
      x = file * SCALE;
      y = rank * SCALE;
      gfx.fillRect(x,y,w,h,color);
    }
  }
}

void drawPieces(Graphics& gfx, chess::Board board) {
  for (const auto& p : board.AllPieces()) {
    auto& sprites = p.white ? white_sprites : black_sprites;
    int x,y;
    CoordinateToScreen(p.coord, x, y);
    gfx.drawSprite(sprites[p.type], x, y, SCALE, SCALE);
  }
}

int main() {
  Graphics gfx("Window Title", SCALE, SCALE);
  gfx.setLogical(8*SCALE,8*SCALE);

  // Clear the screen to a neutral color
  Color eigengrau{ 0x16, 0x16, 0x1D };
  gfx.clear(eigengrau);
  gfx.swap();

  printf("sizeof(chess::Piece): %ld\n", sizeof(chess::Piece));
  printf("sizeof(chess::Board): %ld\n", sizeof(chess::Board));
  printf("sizeof(Texture): %ld\n", sizeof(Texture));
  chess::Board game{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" };

  using chess::Piece;
  using chess::Coordinate;
  Coordinate selected;
  Coordinate highlight;
  Piece grabbed;
  int grabX;
  int grabY;

  auto onMotion = [&](int x, int y) {
    auto coord = ScreenToCoordinate(x,y);
    // Track motion of grabbed piece.
    if (grabbed.type != Piece::None) {
      grabX = x - SCALE / 2;
      grabY = y - SCALE / 2;
    }
    // Highlight valid moves onHover.
    if (game.IsLegalMove({ selected, coord })) highlight = coord;
    else highlight = Coordinate{};
  };

  auto onTouch = [&](int x, int y) {
    auto coord = ScreenToCoordinate(x,y);
    // If there is a selection, attempt to make move.
    if (selected && selected != coord) {
      /// @todo This code is duplicated in onRelease.
      game.MakeMove({ selected, coord });
      selected = Coordinate{};
      highlight = Coordinate{};
    }

    // Make selection and grab piece if valid.
    if (game.IsActive(coord)) {
      selected = coord;
      grabbed = game.GetPieceAt(coord);
      grabX = x - SCALE / 2;
      grabY = y - SCALE / 2;
    }
  };

  auto onRelease = [&](int x, int y) {
    /// @bug After this call highlight is active until next move.
    auto coord = ScreenToCoordinate(x,y);

    // Clear grabbed piece.
    grabbed = chess::Piece::None;

    // Nothing is selected, return.
    if (!selected) return;

    // Attempt to move and clear selection/highlight.
    if (selected != coord) {
      game.MakeMove({ selected, coord });
      selected = Coordinate{};
      highlight = Coordinate{};
    }
  };

  gfx.setMotionCallback(onMotion);
  gfx.setTouchCallback(onTouch);
  gfx.setReleaseCallback(onRelease);

  auto& texture = gfx.createTexture("pieces.png");
  initialize_sprites(texture, white_sprites, 0);
  initialize_sprites(texture, black_sprites, 6);

  Sprite circle{ &gfx.createTexture("circle.png") };

  while (gfx.loop()) {
    gfx.clear();
    drawBoard(gfx);
    int x,y;
    CoordinateToScreen(selected, x, y);
    gfx.drawSprite(circle, x, y, SCALE, SCALE);
    CoordinateToScreen(highlight, x, y);
    gfx.fillRect(x,y,SCALE,SCALE,{0xFF,0,0,0x1F});
    drawPieces(gfx, game);
    if (grabbed.type != Piece::None) {
      auto& sprites = grabbed.white ? white_sprites : black_sprites;
      gfx.drawSprite(sprites[grabbed.type], grabX, grabY, SCALE, SCALE);
    }
    gfx.swap();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

