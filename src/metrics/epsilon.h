/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

// TODO: maximization/mixed

class epsilon {
public:
  static const int additive = 0;
  static const int multiplicative = 1;

  epsilon(int m) : method(m) {}
  epsilon(int m, std::vector<std::vector<double>> _ref) : method(m), ref(_ref) {}

  void set_ref(std::vector<std::vector<double>> _ref) {
    ref = _ref;
  }

  double operator()(std::vector<std::vector<double>> front) {
    if (ref.size() > 0)
      return operator()(front, ref);
    if (method == 0)
      return operator()(front, {{0, 0}});
    else
      return operator()(front, {{1e-10, 1e-10}});
  }

  double operator()(std::vector<std::vector<double>> front,
                    std::vector<std::vector<double>> ref) {
    double eps, eps_j, eps_k, eps_temp;
    int dim = front[0].size();

    for (int i=0; i < ref.size(); i++) {
      for (int j=0; j < front.size(); j++) {
        for (int k=0; k < dim; k++) {
          if (method == 0)
            eps_temp = front[j][k] - ref[i][k];
          else
            eps_temp = front[j][k] / ref[i][k];
          if (k == 0 || eps_k < eps_temp)
            eps_k = eps_temp;
        }
        if (j == 0 || eps_j > eps_k)
          eps_j = eps_k;
      }
      if (i == 0 || eps < eps_j)
        eps = eps_j;
    }

    return eps;
  }

private:
  int method = 0;
  std::vector<std::vector<double>> ref;
};
