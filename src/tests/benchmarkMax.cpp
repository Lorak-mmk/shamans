#include <adventure.h>
#include <types.h>

#include "utils.h"

int main() {
  std::vector<Adventure*> a = {new LonesomeAdventure()};
  for (int i : {1, 2, 4, 8}) {
    a.push_back(new TeamAdventure(i));
  }

  std::vector<Crystal> t2(2000000);
  std::generate(t2.begin(), t2.end(), std::rand);

  for (auto adv : a) {
    for (int i = 0; i < 10; i++) {
      auto startTime = getCurrentTime();
      adv->selectBestCrystal(t2);
      std::cout << getTimeDifference(startTime) << std::endl;
    }
  }
}