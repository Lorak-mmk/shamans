#ifndef SRC_ADVENTURE_H_
#define SRC_ADVENTURE_H_

#include <algorithm>
#include <vector>

#include "../third_party/threadpool/threadpool.h"
#include "./types.h"

class Adventure {
 public:
  virtual ~Adventure() = default;

  virtual uint64_t packEggs(const std::vector<Egg>& eggs,
                            BottomlessBag& bag) = 0;

  virtual void arrangeSand(std::vector<GrainOfSand>& grains) = 0;

  virtual Crystal selectBestCrystal(const std::vector<Crystal>& crystals) = 0;
};

class LonesomeAdventure : public Adventure {
 public:
  LonesomeAdventure() = default;

  uint64_t packEggs(const std::vector<Egg>& eggs, BottomlessBag& bag) override;

  void arrangeSand(std::vector<GrainOfSand>& grains) override;

  Crystal selectBestCrystal(const std::vector<Crystal>& crystals) override;
};

class TeamAdventure : public Adventure {
 public:
  explicit TeamAdventure(uint64_t numberOfShamansArg)
      : numberOfShamans(numberOfShamansArg),
        councilOfShamans(numberOfShamansArg) {}

  uint64_t packEggs(const std::vector<Egg>& eggs, BottomlessBag& bag) override;

  void arrangeSand(std::vector<GrainOfSand>& grains) override;

  Crystal selectBestCrystal(const std::vector<Crystal>& crystals) override;

 private:
  uint64_t numberOfShamans;
  ThreadPool councilOfShamans;
};

#endif  // SRC_ADVENTURE_H_
