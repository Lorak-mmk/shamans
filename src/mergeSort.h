#ifndef SRC_MERGESORT_H_
#define SRC_MERGESORT_H_

#include <algorithm>

template <class iterator>
static void merge(iterator it, const size_t l, const size_t r,
                  decltype(&*it) tmp) {
  iterator li = it, ri = it + l;
  decltype(&*it) dp = tmp;

  while (li < it + l && ri < it + l + r) {
    *(dp++) = *li < *ri ? *(li++) : *(ri++);
  }
  while (li < it + l) {
    *(dp++) = *(li++);
  }
  while (ri < it + l + r) {
    *(dp++) = *(ri++);
  }

  std::copy(tmp, tmp + l + r, it);
}

template <class iterator>
static void mergeSortSingle(iterator begin, iterator end,
                            decltype(&*begin) tmp) {
  size_t n = std::distance(begin, end);
  size_t s = n / 2;

  if (n == 1) return;
  if (n == 2) {
    if (*(begin + 1) < *begin) {
      std::swap(*begin, *(begin + 1));
    }
    return;
  }

  mergeSortSingle(begin, begin + s, tmp);
  mergeSortSingle(begin + s, end, tmp + s);
  merge(begin, s, n - s, tmp);
}

#endif  // SRC_MERGESORT_H_
