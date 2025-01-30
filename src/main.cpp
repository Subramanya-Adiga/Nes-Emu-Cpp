#define OLC_PGE_APPLICATION
#include "frontends/olc/application.hpp"

int main() {
  olc_app::OlcApplication demo;
  demo.Construct(780, 480, 2, 2);
  demo.Start();
  return 0;
}
