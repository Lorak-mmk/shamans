#include <adventure.h>
#include <types.h>

#include "utils.h"

int main() {
  std::vector<Adventure*> a = {new LonesomeAdventure()};
  for (int i : {1, 2, 4, 8}) {
    a.push_back(new TeamAdventure(i));
  }

  for (auto adv : a) {
    for (int i = 0; i < 10; i++) {
      std::vector<GrainOfSand> t2(50000);
      std::generate(t2.begin(), t2.end(), std::rand);

      auto startTime = getCurrentTime();
      adv->arrangeSand(t2);
      std::cout << getTimeDifference(startTime) << std::endl;
    }
  }
}