// Copyright 2020 Kats Eugeny
#ifndef MODULES_TASK_2_KATS_BINARY_CONVEX_CONVEX_BINARY_OMP_H_
#define MODULES_TASK_2_KATS_BINARY_CONVEX_CONVEX_BINARY_OMP_H_

#include <omp.h>
#include <algorithm>
#include <vector>

class SideClass {
 public:
  double length(int* start, int* end);
  double cosinus(int* start, int* end, int* startsecond);
  int** getRandomPic(const int a, const int b);
};
class MainClass {
 public:
  std::vector<int*> jarvis_OMP(int** convex_arr, int a);
  std::vector<int*> getComponent_OMP(int** arr, int a, int b, int*** convex_arr,
                                 int* num);
  std::vector<int*> inside(std::vector<int*> vec, int** convex_arr, int a);
};

#endif  // MODULES_TASK_2_KATS_BINARY_CONVEX_CONVEX_BINARY_OMP_H_
