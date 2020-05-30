// Copyright 2020 Kats Eugeny

#include <gtest/gtest.h>
#include <omp.h>

#include <algorithm>
#include <vector>

#include "../../../modules/task_2/kats_binary_convex/convex_binary_omp.h"

TEST(convex_binary_seq, random_pic_validation) {
  // Arrange
  SideClass sc;
  ASSERT_ANY_THROW(sc.getRandomPic(-1, -1));
}

TEST(convex_binary_omp, test_with_one_point) {
  // Arrange
  SideClass sc;
  MainClass mc;
  std::vector<int*> result;
  int** convex_arr = NULL;
  int num = 0, n = 5, m = 20;

  int** arr = new int*[n];
  for (int i = 0; i < n; i++) arr[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) arr[i][j] = 0;
  arr[2][7] = 1;
  // Act
  int num_thr = 2;
  result = mc.getComponent_OMP(arr, n, m, &convex_arr, &num, num_thr);

  int s = result.size();
  int** tmp = new int*[s];
  for (int i = 0; i < s; i++) tmp[i] = new int[2];
  tmp[0][0] = 7;
  tmp[0][1] = 2;
  // Assert
  for (int i = 0; i < s; i++) {
    int* temp = result[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++) delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++) delete[] arr[i];
  delete[] arr;
  for (int i = 0; i < num; i++) delete[] convex_arr[i];
  delete[] convex_arr;
}

TEST(convex_binary_omp, test_with_one_line) {
  // Arrange
  SideClass sc;
  MainClass mc;
  std::vector<int*> result;
  int** convex_arr = NULL;
  int num = 0, n = 1, m = 20;
  // Act
  int** arr = new int*[n];
  for (int i = 0; i < n; i++) arr[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) arr[i][j] = 0;
  arr[0][2] = 1;
  arr[0][11] = 1;
  arr[0][7] = 1;
  arr[0][19] = 1;
  arr[0][5] = 1;
  arr[0][17] = 1;

  int num_thr = 2;
  result = mc.getComponent_OMP(arr, n, m, &convex_arr, &num, num_thr);

  int** tmp = new int*[2];
  for (int i = 0; i < 2; i++) tmp[i] = new int[2];
  tmp[0][0] = 2;
  tmp[0][1] = 0;
  tmp[1][0] = 19;
  tmp[1][1] = 0;
  // Assert
  int s = result.size();
  for (int i = 0; i < s; i++) {
    int* temp = result[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++) delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++) delete[] arr[i];
  delete[] arr;
  for (int i = 0; i < num; i++) delete[] convex_arr[i];
  delete[] convex_arr;
}

TEST(convex_binary_omp, test_sq) {
  // Arrange
  SideClass sc;
  MainClass mc;
  std::vector<int*> result;
  int** convex_arr = NULL;
  int num = 0, n = 10, m = 10;
  // Act
  int** arr = new int*[n];
  for (int i = 0; i < n; i++) arr[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) arr[i][j] = 0;
  arr[0][2] = 1;
  arr[0][5] = 1;
  arr[0][8] = 1;
  arr[2][5] = 1;
  arr[2][8] = 1;
  arr[5][8] = 1;
  arr[5][5] = 1;
  arr[5][4] = 1;
  arr[3][2] = 1;
  arr[1][2] = 1;

  int num_thr = 2;
  result = mc.getComponent_OMP(arr, n, m, &convex_arr, &num, num_thr);

  int s = result.size();
  int** tmp = new int*[s];
  for (int i = 0; i < s; i++) tmp[i] = new int[2];
  tmp[0][0] = 2;
  tmp[0][1] = 0;
  tmp[1][0] = 8;
  tmp[1][1] = 0;
  tmp[2][0] = 8;
  tmp[2][1] = 5;
  tmp[3][0] = 4;
  tmp[3][1] = 5;
  tmp[4][0] = 2;
  tmp[4][1] = 3;
  // Assert
  for (int i = 0; i < s; i++) {
    int* temp = result[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++) delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++) delete[] arr[i];
  delete[] arr;
  for (int i = 0; i < num; i++) delete[] convex_arr[i];
  delete[] convex_arr;
}

TEST(convex_binary_omp, test_inside) {
  // Arrange
  SideClass sc;
  MainClass mc;
  int** convex_arr = NULL;
  int num = 0, n = 6, m = 6;
  // Act
  int** arr = new int*[n];
  for (int i = 0; i < n; i++) arr[i] = new int[m];
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) arr[i][j] = 0;
  arr[0][1] = 1;
  arr[0][2] = 1;
  arr[1][1] = 1;
  arr[1][5] = 1;
  arr[2][4] = 1;
  arr[2][5] = 1;
  arr[3][5] = 1;
  arr[4][2] = 1;

  std::vector<int*> omp_res;
  int num_thr = 2;

  omp_res = mc.getComponent_OMP(arr, n, m, &convex_arr, &num, num_thr);

  std::vector<int*> inl;
  inl = sc.inside(omp_res, convex_arr, num);

  int s = inl.size();
  int** tmp = new int*[s];
  for (int i = 0; i < s; i++) tmp[i] = new int[2];
  tmp[0][0] = 1;
  tmp[0][1] = 0;
  tmp[1][0] = 5;
  tmp[1][1] = 1;
  tmp[2][0] = 2;
  tmp[2][1] = 4;
  // Assert
  for (int i = 0; i < s; i++) {
    int* temp = inl[i];
    EXPECT_EQ(temp[0], tmp[i][0]);
    EXPECT_EQ(temp[1], tmp[i][1]);
  }

  for (int i = 0; i < s; i++) delete[] tmp[i];
  delete[] tmp;
  for (int i = 0; i < n; i++) delete[] arr[i];
  delete[] arr;
  for (int i = 0; i < num; i++) delete[] convex_arr[i];
  delete[] convex_arr;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
