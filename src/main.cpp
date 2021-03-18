#include <graphics.h>
#include <chess.h>
#include <thread>
#include <chrono>

Color WHITE{ 216, 191, 161 };
Color BLACK{ 158, 116, 66 };

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
    // TODO draw pieces
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

  while (gfx.loop()) {
    gfx.clear();
    drawBoard(gfx, game);
    gfx.swap();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
