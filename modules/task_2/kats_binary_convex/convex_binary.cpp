// Copyright 2020 Kats Eugeny

#include <omp.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

#include "../../../modules/task_2/kats_binary_convex/convex_binary_omp.h"

int** SideClass::getRandomPic(const int a, const int b) {
  if ((a <= 0) || (b <= 0)) throw "\tError_In_Rows_&_Columns\n";
  int** arr = new int*[a];
  std::mt19937 seed;
  std::uniform_real_distribution<> urd(0, 100);
  for (int i = 0; i < a; i++) arr[i] = new int[b];
  for (int i = 0; i < a; i++) {
    for (int j = 0; j < b; j++) {
      arr[i][j] = static_cast<int>(urd(seed)) % 2;
    }
  }
  return arr;
}

double SideClass::length(int* p1, int* p2) {
  double result;
  result = (sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) +
                 (p2[1] - p1[1]) * (p2[1] - p1[1])));
  return result;
}

double SideClass::cosinus(int* p1, int* p2, int* p3) {
  double result;
  double ax = p2[0] - p1[0];
  double ay = p2[1] - p1[1];
  double bx = p3[0] - p1[0];
  double by = p3[1] - p1[1];
  result = ((ax * bx + ay * by) /
            (sqrt(ax * ax + ay * ay) * sqrt(bx * bx + by * by)));
  return result;
}

std::vector<int*> MainClass::jarvis(int** convex_arr, int n) {
  SideClass sc;
  std::vector<int*> res;
  if (n == 1) {
    res.push_back(convex_arr[0]);
  } else if (n == 2) {
    res.push_back(convex_arr[0]);
    res.push_back(convex_arr[1]);
  } else {
    double* buf = new double[2];
    buf[0] = convex_arr[0][0];
    buf[1] = convex_arr[0][1];
    int count = 0;

    for (int i = 1; i < n; i++) {
      if (convex_arr[i][1] < buf[1]) {
        count = i;
      } else {
        if ((convex_arr[i][1] == buf[1]) && (convex_arr[i][0] < buf[0])) {
          count = i;
        }
      }
    }

    res.push_back(convex_arr[count]);

    int* last = new int[2];
    int* beforelast = new int[2];
    last = convex_arr[count];
    beforelast[0] = convex_arr[count][0] - 2;
    beforelast[1] = convex_arr[count][1];

    double mincos, cos;
    int minind = 0;
    double maxlen = 0;
    while (1) {
      mincos = 2;
      for (int i = 0; i < n; i++) {
        cos = round(sc.cosinus(last, beforelast, convex_arr[i]) * 10000000) /
              10000000;
        if (cos < mincos) {
          minind = i;
          mincos = cos;
          maxlen = sc.length(last, convex_arr[i]);
        } else if (cos == mincos) {
          double len = sc.length(last, convex_arr[i]);
          if (len > maxlen) {
            minind = i;
            maxlen = len;
          }
        }
      }

      beforelast = last;
      last = convex_arr[minind];
      if (last == convex_arr[count]) break;
      res.push_back(convex_arr[minind]);
    }
  }
  return res;
}

std::vector<int*> MainClass::getComponent(int** arr, int n, int m,
                                          int*** convex_arr, int* num) {
  MainClass mc;
  int x = 0, y = 0, bf = 1;
  int A, B, C;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      x = j - 1;
      if (x + 1 <= 0) {
        x = 1;
        B = 0;
      } else {
        B = arr[i][x];
      }
      y = i - 1;
      if (y + 1 <= 0) {
        y = 1;
        C = 0;
      } else {
        C = arr[y][j];
      }
      A = arr[i][j];

      if (A == 0) {
      } else {
        *num = *num + 1;
        if (B == 0 && C == 0) {
          bf++;
          arr[i][j] = bf;
        }
        if (B != 0 && C == 0) {
          arr[i][j] = B;
        }
        if (B == 0 && C != 0) {
          arr[i][j] = C;
        }
        if (B != 0 && C != 0) {
          if (B == C) {
            arr[i][j] = B;
          } else {
            arr[i][j] = B;
            for (int i_s = 0; i_s < i; i_s++)
              for (int i_t = 0; i_t < m; i_t++)
                if (arr[i_s][i_t] == C) arr[i_s][i_t] = B;
          }
        }
      }
    }

  (*convex_arr) = new int*[*num];
  for (int i = 0; i < *num; i++) (*convex_arr)[i] = new int[3];

  int tmp = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (arr[i][j] != 0) {
        (*convex_arr)[tmp][0] = j;
        (*convex_arr)[tmp][1] = i;
        (*convex_arr)[tmp][2] = arr[i][j];
        tmp++;
      }
    }

  std::vector<int*> res;
  res = mc.jarvis(*convex_arr, *num);
  return res;
}

std::vector<int*> MainClass::jarvis_OMP(int** convex_arr, int n, int num_thr) {
  SideClass sc;
  std::vector<int*> res;
  std::vector<std::vector<int*>> local_res(num_thr);
  if (n == 1) {
    res.push_back(convex_arr[0]);
  } else if (n == 2) {
    res.push_back(convex_arr[0]);
    if (convex_arr[0] != convex_arr[1]) res.push_back(convex_arr[1]);
  } else {
    int m = 0;

    res.push_back(convex_arr[m]);
    for (int i = 0; i < num_thr; i++) local_res[i].push_back(convex_arr[m]);

    int* last;
    int* beforelast;

    double mincos, cos;
    int minind = 0;
    double maxlen = 0;
    int id, delta, ost;
    double len;

#pragma omp parallel private(id, delta, ost, mincos, cos, minind, maxlen, \
                             last, beforelast, len) num_threads(num_thr)
    {
      delta = n / num_thr;
      id = omp_get_thread_num();

      if (id == num_thr - 1) {
        ost = n % num_thr;
      } else {
        ost = 0;
      }

      maxlen = 0;
      minind = 0;
      last = new int[2];
      beforelast = new int[2];
      last = convex_arr[m];
      beforelast[0] = convex_arr[m][0] - 2;
      beforelast[1] = convex_arr[m][1];

      while (1) {
        mincos = 2;
        for (int i = id * delta; i < id * delta + delta + ost; i++) {
          cos = round(sc.cosinus(last, beforelast, convex_arr[i]) * 10000000) /
                10000000;
          if (cos < mincos) {
            minind = i;
            mincos = cos;
            maxlen = sc.length(last, convex_arr[i]);
          } else if (cos == mincos) {
            len = sc.length(last, convex_arr[i]);
            if (len > maxlen) {
              minind = i;
              maxlen = len;
            }
          }
        }

        if (id != 0) {
          cos = round(sc.cosinus(last, beforelast, convex_arr[0]) * 10000000) /
                10000000;
          if (cos < mincos) {
            minind = 0;
            mincos = cos;
            maxlen = sc.length(last, convex_arr[0]);
          } else if (cos == mincos) {
            len = sc.length(last, convex_arr[0]);
            if (len > maxlen) {
              minind = 0;
              maxlen = len;
            }
          }
        }

        beforelast = last;
        last = convex_arr[minind];
        if (last == convex_arr[m]) break;
        local_res[id].push_back(convex_arr[minind]);
      }
    }

    std::vector<int*> finale_local;
    for (int i = 0; i < num_thr; i++) {
      int size = local_res[i].size();
      for (int j = 0; j < size; j++) finale_local.push_back(local_res[i][j]);
    }

    int s = finale_local.size();

    last = new int[2];
    beforelast = new int[2];
    last = convex_arr[m];
    beforelast[0] = convex_arr[m][0] - 2;
    beforelast[1] = convex_arr[m][1];
    while (1) {
      mincos = 2;
      for (int i = 0; i < s; i++) {
        cos = round(sc.cosinus(last, beforelast, finale_local[i]) * 10000000) /
              10000000;
        if (cos < mincos) {
          minind = i;
          mincos = cos;
          maxlen = sc.length(last, finale_local[i]);
        } else if (cos == mincos) {
          double len = sc.length(last, finale_local[i]);
          if (len > maxlen) {
            minind = i;
            maxlen = len;
          }
        }
      }

      beforelast = last;
      last = finale_local[minind];
      if (last == finale_local[m]) break;
      res.push_back(finale_local[minind]);
    }
  }
  return res;
}

std::vector<int*> MainClass::getComponent_OMP(int** arr, int n, int m,
                                              int*** convex_arr, int* num,
                                              int num_thr) {
  MainClass mc;
  *num = 0;
  int kn = 0, km = 0, cur = 1;
  int A, B, C;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      kn = j - 1;
      if (kn + 1 <= 0) {
        kn = 1;
        B = 0;
      } else {
        B = arr[i][kn];
      }
      km = i - 1;
      if (km + 1 <= 0) {
        km = 1;
        C = 0;
      } else {
        C = arr[km][j];
      }
      A = arr[i][j];

      if (A == 0) {
      } else {
        *num = *num + 1;
        if (B == 0 && C == 0) {
          cur++;
          arr[i][j] = cur;
        }
        if (B != 0 && C == 0) {
          arr[i][j] = B;
        }
        if (B == 0 && C != 0) {
          arr[i][j] = C;
        }
        if (B != 0 && C != 0) {
          if (B == C) {
            arr[i][j] = B;
          } else {
            arr[i][j] = B;
            int k;
#pragma omp parallel private(k) num_threads(num_thr)
            {
#pragma omp for
              for (k = 0; k < i; k++)
                for (int kk = 0; kk < m; kk++)
                  if (arr[k][kk] == C) arr[k][kk] = B;
            }
          }
        }
      }
    }

  (*convex_arr) = new int*[*num];
  for (int i = 0; i < *num; i++) (*convex_arr)[i] = new int[3];

  int tmp = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (arr[i][j] != 0) {
        (*convex_arr)[tmp][0] = j;
        (*convex_arr)[tmp][1] = i;
        (*convex_arr)[tmp][2] = arr[i][j];
        tmp++;
      }
    }

  std::vector<int*> res;
  res = mc.jarvis_OMP(*convex_arr, *num, num_thr);
  return res;
}

std::vector<int*> SideClass::inside(std::vector<int*> root, int** convex_arr,
                                    int n) {
  std::vector<int*> v_first;
  std::vector<int> v_sec;
  std::vector<int> v_t;

  int s = root.size();
  for (int i = 0; i < s; i++) {
    int* tmp = root[i];
    v_t.push_back(tmp[2]);
  }

  for (int i = 0; i < n; i++) {
    auto result = std::find(v_t.begin(), v_t.end(), convex_arr[i][2]);
    auto result_sec = std::find(v_sec.begin(), v_sec.end(), convex_arr[i][2]);
    if (result_sec != v_sec.end()) {
      continue;
    } else {
      if (result != v_t.end()) {
        v_first.push_back(convex_arr[i]);
        int* tmp = convex_arr[i];
        v_sec.push_back(tmp[2]);
      }
    }
  }
  return v_first;
}
