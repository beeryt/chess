#include <graphics.h>
#include <chess.h>
#include <thread>
#include <chrono>
#include <map>

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

void drawBoard(Graphics& gfx, chess::Board board) {
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
  for (auto& p : board.pieces) {
    SpriteMap& sprites = p.team == chess::Piece::White ? white_sprites : black_sprites;
    int x = (p.coord % 8) * SCALE;
    int y = (p.coord / 8) * SCALE;
    gfx.drawSprite(sprites[p.type], x, y, SCALE, SCALE);
  }
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

int main() {
  Graphics gfx("Window Title", SCALE, SCALE);
  gfx.setLogical(8*SCALE,8*SCALE);
  printf("sizeof(chess::Piece): %ld\n", sizeof(chess::Piece));
  printf("sizeof(chess::Board): %ld\n", sizeof(chess::Board));
  printf("sizeof(Texture): %ld\n", sizeof(Texture));
  chess::Board game{ "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" };


  auto mcb = [](int x, int y) {
    static chess::Coordinate last;
    // convert to Board::Coordinate
    auto coord = ScreenToCoordinate(x,y);
    if (coord && (coord != last)) {
      printf("Hovering: %s\n", chess::Board::coordToStr(coord).c_str());
      last = coord;
    }
  };
  gfx.setMotionCallback(mcb);

  auto ccb = [](int x, int y) {
    auto coord = ScreenToCoordinate(x,y);
    if (coord) {
      printf("Clicked: %s\n", chess::Board::coordToStr(coord).c_str());
    }
  };
  gfx.setClickCallback(ccb);

  auto& texture = gfx.createTexture("pieces.png");
  initialize_sprites(texture, white_sprites, 0);
  initialize_sprites(texture, black_sprites, 6);

  while (gfx.loop()) {
    gfx.clear();
    drawBoard(gfx, game);
    gfx.swap();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

