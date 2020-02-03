#include <iostream>

#include "adventure.h"
#include "mergeSort.h"

constexpr size_t CACHE_LINE = 64;

template <typename T>
std::vector<std::pair<size_t, size_t>> toEqualParts(T *array, size_t N,
												   size_t threads) {
	std::vector<std::pair<size_t, size_t>> result;
	size_t part_size = N / threads;
	size_t remainder = N - part_size * threads;
	for (size_t start = 0, end = part_size; start < N;
		 start = end, end = start + part_size) {
		if (remainder) {
			end++;
			remainder--;
		}
		result.emplace_back(start, end);
	}

	return result;
}

template <typename T>
std::vector<std::pair<size_t, size_t>> divideArray(T *array, size_t N,
                                                   size_t threads) {
  if(CACHE_LINE % sizeof(T)) {
  	return toEqualParts(array, N, threads);
  }
  auto result = toEqualParts(array, N, threads);
  return result;
}

template <typename T, size_t ALIGN = CACHE_LINE>
T **alloc2DArray(size_t x, size_t y) {
  size_t padding = ((sizeof(T) * y) % ALIGN) / sizeof(T);
  y = y + padding;
  T **tab = new (std::align_val_t{ALIGN}) T *[x];
  tab[0] = new (std::align_val_t{ALIGN}) T[x * y];
  for (size_t i = 1; i < x; i++) {
    tab[i] = tab[i - 1] + y;
  }
  return tab;
}

template <typename T, size_t ALIGN = CACHE_LINE>
void delete2DArray(T **tab) {
  ::operator delete[](tab[0], std::align_val_t{ALIGN});
  ::operator delete[](tab, std::align_val_t{ALIGN});
}

uint64_t TeamAdventure::packEggs(const std::vector<Egg> &eggs,
                                 BottomlessBag &bag) {
  uint64_t **tab =
      alloc2DArray<uint64_t>(eggs.size() + 1, bag.getCapacity() + 1);
  std::fill(tab[0], tab[0] + bag.getCapacity() + 1, 0);

  auto f = [](auto *tab_i, auto *tab_ip, size_t i, size_t a, size_t b,
              Egg egg) {
    for (size_t w = a; w < b; w++) {
      if (egg.getSize() > w) {
        tab_i[w] = tab_ip[w];
      } else {
        tab_i[w] =
            std::max(tab_ip[w], tab_ip[w - egg.getSize()] + egg.getWeight());
      }
    }
  };

  std::vector<std::future<void>> futures;
  auto idxs = divideArray(tab[0], bag.getCapacity() + 1, numberOfShamans);

  for (size_t i = 1; i <= eggs.size(); i++) {
    auto &egg = eggs[i - 1];
    for (auto p : idxs) {
      auto future = councilOfShamans.enqueue(f, tab[i], tab[i - 1], i, p.first,
                                             p.second, egg);
      futures.emplace_back(std::move(future));
    }
    for (auto &future : futures) {
      future.wait();
    }
    futures.clear();
  }

  size_t idx = bag.getCapacity();
  for (size_t i = eggs.size(); i > 0; i--) {
    if (tab[i][idx] != tab[i - 1][idx]) {
      auto &egg = eggs[i - 1];
      idx -= egg.getSize();
      bag.addEgg(egg);
    }
  }

  uint64_t result = tab[eggs.size()][bag.getCapacity()];
  delete2DArray(tab);

  return result;
}

static uint64_t nextPowerOf2(uint64_t n) {
  uint64_t count = 0;
  if (n && !(n & (n - 1))) {
    return n;
  }

  while (n != 0) {
    n >>= 1u;
    count += 1;
  }

  return 1u << count;
}

class mergeInfo {
 public:
  static mergeInfo *createTree(std::vector<GrainOfSand> &arr,
                               std::vector<std::pair<size_t, size_t>> &idxs,
                               GrainOfSand *tmp) {
    size_t N = nextPowerOf2(idxs.size());
    auto *tab = new mergeInfo[2 * N];
    for (size_t i = 0; i < idxs.size(); i++) {
      tab[i + N].begin = arr.begin() + idxs[i].first;
      tab[i + N].end = arr.begin() + idxs[i].second;

      tab[i + N].parent = &tab[(i + N) / 2];

      tab[i + N].buffer = tmp + idxs[i].first;
    }
    for (size_t i = idxs.size(); i < N; i++) {
      tab[i + N].begin = arr.end();
      tab[i + N].end = arr.end();

      tab[i + N].parent = &tab[(i + N) / 2];
      tab[i + N].buffer = nullptr;
    }
    for (size_t i = N - 1; i > 0; i--) {
      tab[i].begin = tab[2 * i].begin;
      tab[i].end = tab[2 * i + 1].end;

      tab[i].parent = &tab[i / 2];
      tab[i].left = &tab[2 * i];
      tab[i].right = &tab[2 * i + 1];
      tab[i].buffer = tab[i].left->buffer;
    }
    for (size_t i = idxs.size(); i < N; i++) {
      tab[i + N].mark_unused();
    }
    tab[1].parent = nullptr;

    return tab;
  }

  static void deleteTree(mergeInfo *tab) { delete[] tab; }

  std::atomic<uint8_t> childrenReady{0};
  bool unused = false, ready = false;
  mergeInfo *parent = nullptr, *left = nullptr, *right = nullptr;
  GrainOfSand *buffer = nullptr;
  std::vector<GrainOfSand>::iterator begin, end;

 private:
  void mark_unused() {
    childrenReady++;
    if (left == nullptr || childrenReady == 2) {
      unused = true;
      if (parent) {
        parent->mark_unused();
      }
    }
  }
};

void TeamAdventure::arrangeSand(std::vector<GrainOfSand> &grains) {
  auto idxs = divideArray(grains.data(), grains.size(), numberOfShamans);
  auto *tmp = new GrainOfSand[grains.size()];
  auto tree = mergeInfo::createTree(grains, idxs, tmp);
  size_t N = nextPowerOf2(idxs.size());

  std::mutex mut;
  std::condition_variable cv;

  std::function<void(mergeInfo *)> f2 = [this, &f2, &cv](mergeInfo *node) {
    if (!node->right->unused) {
      merge(node->left->begin,
            std::distance(node->left->begin, node->left->end),
            std::distance(node->right->begin, node->right->end), node->buffer);
    }
    node->ready = true;
    if (node->parent == nullptr) {
      cv.notify_one();
    } else if (++node->parent->childrenReady == 2) {
      this->councilOfShamans.enqueue(f2, node->parent);
    }
  };
  auto f1 = [this, &f2, &cv](auto begin, auto end, mergeInfo *node) {
    mergeSortSingle<std::vector<GrainOfSand>::iterator>(begin, end,
                                                        node->buffer);
    node->ready = true;
    if (node->parent == nullptr) {
      cv.notify_one();
    } else if (++node->parent->childrenReady == 2) {
      this->councilOfShamans.enqueue(f2, node->parent);
    }
  };
  std::unique_lock<std::mutex> lk(mut);
  for (size_t i = 0; i < idxs.size(); i++) {
    auto &p = idxs[i];
    auto future = councilOfShamans.enqueue(
        f1, grains.begin() + p.first, grains.begin() + p.second, &tree[N + i]);
  }
  cv.wait(lk, [tree] { return tree[1].ready; });
  lk.unlock();

  mergeInfo::deleteTree(tree);
  delete[] tmp;
}

Crystal TeamAdventure::selectBestCrystal(const std::vector<Crystal> &crystals) {
  auto idxs = divideArray(crystals.data(), crystals.size(), numberOfShamans);
  auto f = [](auto it, auto end) { return *std::max_element(it, end); };

  std::vector<std::future<Crystal>> partial_results;
  for (auto el : idxs) {
    auto future = councilOfShamans.enqueue(f, crystals.begin() + el.first,
                                           crystals.begin() + el.second);
    partial_results.emplace_back(std::move(future));
  }

  auto result = Crystal(0);
  for (auto &partial : partial_results) {
    result = std::max(result, partial.get());
  }
  return result;
}
