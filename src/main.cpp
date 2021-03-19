#include <graphics.h>
#include <chess.h>
#include <thread>
#include <chrono>
#include <map>

Color WHITE{ 216, 191, 161 };
Color BLACK{ 158, 116, 66 };

using SpriteMap = std::map<chess::Piece::Type, Sprite>;
SpriteMap white_sprites;
SpriteMap black_sprites;

void initialize_sprites(Texture& texture, SpriteMap& sprites, int offset) {
  sprites[chess::Piece::King] = Sprite{ &texture };
  sprites[chess::Piece::King].setRegion(texture.getRegion());
  sprites[chess::Piece::King].setHFrames(6);
  sprites[chess::Piece::King].setVFrames(2);
  sprites[chess::Piece::King].setFrame(0 + offset);

  sprites[chess::Piece::Queen] = Sprite{ &texture };
  sprites[chess::Piece::Queen].setRegion(texture.getRegion());
  sprites[chess::Piece::Queen].setHFrames(6);
  sprites[chess::Piece::Queen].setVFrames(2);
  sprites[chess::Piece::Queen].setFrame(1 + offset);

  sprites[chess::Piece::Bishop] = Sprite{ &texture };
  sprites[chess::Piece::Bishop].setRegion(texture.getRegion());
  sprites[chess::Piece::Bishop].setHFrames(6);
  sprites[chess::Piece::Bishop].setVFrames(2);
  sprites[chess::Piece::Bishop].setFrame(2 + offset);

  sprites[chess::Piece::Knight] = Sprite{ &texture };
  sprites[chess::Piece::Knight].setRegion(texture.getRegion());
  sprites[chess::Piece::Knight].setHFrames(6);
  sprites[chess::Piece::Knight].setVFrames(2);
  sprites[chess::Piece::Knight].setFrame(3 + offset);

  sprites[chess::Piece::Rook] = Sprite{ &texture };
  sprites[chess::Piece::Rook].setRegion(texture.getRegion());
  sprites[chess::Piece::Rook].setHFrames(6);
  sprites[chess::Piece::Rook].setVFrames(2);
  sprites[chess::Piece::Rook].setFrame(4 + offset);

  sprites[chess::Piece::Pawn] = Sprite{ &texture };
  sprites[chess::Piece::Pawn].setRegion(texture.getRegion());
  sprites[chess::Piece::Pawn].setHFrames(6);
  sprites[chess::Piece::Pawn].setVFrames(2);
  sprites[chess::Piece::Pawn].setFrame(5 + offset);
}

void drawBoard(Graphics& gfx, chess::Board board) {
  for (int file = 0; file < 8; ++file) {
    for (int rank = 0; rank < 8; ++rank) {
      bool even = (file + rank) % 2 == 0;
      Color color = even ? WHITE : BLACK;
      int x,y,w,h;
      w = h = 256;
      x = file * 256;
      y = rank * 256;
      gfx.fillRect(x,y,w,h,color);
    }
  }
  for (auto& p : board.pieces) {
    SpriteMap& sprites = p.team == chess::Piece::White ? white_sprites : black_sprites;
    int x = (p.coord % 8) * 256;
    int y = (p.coord / 8) * 256;
    gfx.drawSprite(sprites[p.type], x, y, 256, 256);
  }
}

inline int floor_div(int a, int b) {
  int d = a / b;
  int r = a % b;
  return r ? (d - ((a < 0) ^ (b < 0))) : d;
}

chess::Coordinate ScreenToCoordinate(int x, int y) {
  return chess::Coordinate{
    floor_div(x,256),
    floor_div(y,256)
  };
}

int main() {
  Graphics gfx("Window Title");
  gfx.setLogical(8*256,8*256);
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

