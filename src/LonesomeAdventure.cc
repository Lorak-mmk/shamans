#include "adventure.h"

template<class iterator>
static void merge(iterator it, const size_t l, const size_t r, decltype(&*it) tmp) {
    iterator li = it, ri = it + l;
    decltype(&*it) dp = tmp;

    while(li < it + l && ri < it + l + r) {
        *(dp++) = *li < *ri ? *(li++) : *(ri++);
    }
    while(li < it + l) {
        *(dp++) = *(li++);
    }
    while(ri < it + l + r) {
        *(dp++) = *(ri++);
    }

    std::copy(tmp, tmp + l + r, it);
}

template<class iterator>
static void mergeSortSingle(iterator begin, iterator end, decltype(&*begin) tmp) {
    size_t n = std::distance(begin, end);
    size_t s = n / 2;

    if(n == 1) return;
    if(n == 2) {
        if (*(begin + 1) < *begin) {
            std::swap(*begin, *(begin + 1));
        }
        return;
    }

    mergeSortSingle(begin, begin + s, tmp);
    mergeSortSingle(begin + s, end, tmp + s);
    merge(begin, s, n - s, tmp);
}


uint64_t LonesomeAdventure::packEggs(const std::vector<Egg>& eggs, BottomlessBag& bag) {
    auto **tab = new uint64_t*[eggs.size() + 1];
    tab[0] = new uint64_t[(eggs.size() + 1) * (bag.getCapacity() + 1)];
    for(size_t i = 1; i < (eggs.size() + 1); i++) {
        tab[i] = tab[i - 1] + (bag.getCapacity() + 1);
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

    return tab[eggs.size()][bag.getCapacity()];
}

void LonesomeAdventure::arrangeSand(std::vector<GrainOfSand>& grains) {
    auto *tmp = new GrainOfSand[grains.size()];
    mergeSortSingle(grains.begin(), grains.end(), tmp);
    delete[] tmp;
}


Crystal LonesomeAdventure::selectBestCrystal(const std::vector<Crystal>& crystals) {
    Crystal result = Crystal();
    for(auto elem : crystals) {
        if(result < elem){
            result = elem;
        }
    }
    return result;
}