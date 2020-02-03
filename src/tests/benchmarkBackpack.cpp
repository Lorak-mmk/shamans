#include <adventure.h>
#include <types.h>

#include "utils.h"

int main() {
  std::vector<Adventure*> a = {new LonesomeAdventure()};
  for (int i : {1, 2, 4, 8}) {
    a.push_back(new TeamAdventure(i));
  }

  std::vector<Egg> eggs;
  for (int i = 0; i < 100; ++i) {
    eggs.emplace_back(Egg(i % 10, i * 3 + 2));
  }

  for (auto adv : a) {
    for (int i = 0; i < 10; i++) {
      BottomlessBag bag(10000);

      auto startTime = getCurrentTime();
      adv->packEggs(eggs, bag);
      std::cout << getTimeDifference(startTime) << std::endl;
    }
  }
}
