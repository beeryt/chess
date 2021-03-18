#include <graphics.h>
#include <thread>
#include <chrono>

int main() {
  Graphics gfx("Window Title");
  gfx.setLogical(8*256,8*256);
  auto mcb = [](int x, int y) {
    printf("Motion at (%d,%d)\n", x, y);
  };
  gfx.setMotionCallback(mcb);

  auto ccb = [](int x, int y) {
    printf("Click at (%d,%d)\n", x, y);
  };
  gfx.setClickCallback(ccb);

  while (gfx.loop()) {
    gfx.clear();
    // TODO draw here
    gfx.swap();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
