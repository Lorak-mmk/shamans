#include "adventure.h"

uint64_t TeamAdventure::packEggs(const std::vector<Egg>& eggs, BottomlessBag& bag) {
    return 0;
}

void TeamAdventure::arrangeSand(std::vector<GrainOfSand>& grains) {
    std::sort(grains.begin(), grains.end());
}

Crystal TeamAdventure::selectBestCrystal(const std::vector<Crystal>& crystals) {
    return *std::max_element(crystals.begin(), crystals.end());
}