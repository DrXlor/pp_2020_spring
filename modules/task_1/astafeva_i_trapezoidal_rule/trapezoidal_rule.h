// Copyright 2020 Astafeva Irina
#ifndef MODULES_TASK_1_ASTAFEVA_I_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE
#define MODULES_TASK_1_ASTAFEVA_I_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE

#include <vector>

double calculateIntegral(double (*function)(std::vector<double>), std::vector <std::pair<double, double>> boundaries,
                        std::vector<int> partition);

#endif // MODULES_TASK_1_ASTAFEVA_I_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE
