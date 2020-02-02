#include "adventure.h"

template<size_t SIZE>
std::vector<std::pair<size_t, size_t>> divideArray(size_t threads, size_t count) {
    return std::vector<std::pair<size_t, size_t>>();
}



uint64_t TeamAdventure::packEggs(const std::vector<Egg>& eggs, BottomlessBag& bag) {
    auto **tab = new uint64_t*[eggs.size() + 1];
    tab[0] = new uint64_t[(eggs.size() + 1) * (bag.getCapacity() + 1)];
    for(size_t i = 1; i < (eggs.size() + 1); i++) {
        tab[i] = tab[i - 1] + (bag.getCapacity() + 1);
    }
    for(size_t i = 0; i <= bag.getCapacity(); i++) {
        tab[0][i] = 0;
    }

    for(size_t i = 1; i <= eggs.size(); i++) {
        auto& egg = eggs[i - 1];
        for (size_t w = 0; w <= bag.getCapacity(); w++) {
            if(egg.getSize() > w) {
                tab[i][w] = tab[i - 1][w];
            } else {
                tab[i][w] = std::max(tab[i - 1][w], tab[i - 1][w - egg.getSize()] + egg.getWeight());
            }
        }
    }

    size_t idx = bag.getCapacity();
    for(size_t i = eggs.size(); i > 0; i--) {
        if (tab[i][idx] != tab[i - 1][idx]) {
            auto& egg = eggs[i - 1];
            idx -= egg.getSize();
            bag.addEgg(egg);
        }
    }

    uint64_t result = tab[eggs.size()][bag.getCapacity()];
    delete[] tab[0];
    delete[] tab;

    return result;
}

void TeamAdventure::arrangeSand(std::vector<GrainOfSand>& grains) {
    std::sort(grains.begin(), grains.end());
}

Crystal TeamAdventure::selectBestCrystal(const std::vector<Crystal>& crystals) {
    return *std::max_element(crystals.begin(), crystals.end());
}