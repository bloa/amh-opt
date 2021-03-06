/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

template <class T=double>
class biobjIGD {
public:
  biobjIGD(T ideal_x, T ideal_y) : ref({{ideal_x, ideal_y}}) {}
  biobjIGD(std::vector<T> _ideal) : ref({_ideal}) {}
  biobjIGD(bool c, T ideal_x, T ideal_y) : correction(c), ref({{ideal_x, ideal_y}}) {}
  biobjIGD(bool c, std::vector<T> _ideal) : correction(c), ref({_ideal}) {}

  T operator()(std::vector<std::vector<T>> front) {
    if (ref.size() == 0 || front.size() == 0)
      return 0;

    T dist = 0;
    for (auto pt : ref)
      dist += min_dist(pt, front);
    return dist/ref.size();;
  }

private:
  std::vector<std::vector<T>> ref;
  bool correction = true;

  double min_dist(const std::vector<T> u, const std::vector<std::vector<T>> front) {
    T m = dist(u, front[0]);
    for (int i=1; i<front.size(); i++) {
      T d = correction ? dist(front[i], u) : distp(front[i], u);
      m = std::min(m, d);
    }
    return m;
  }

  // euclidian distance
  double dist(const std::vector<T> u, const std::vector<T> v) {
    return std::sqrt(std::pow(u[0]-v[0], 2.0) + std::pow(u[1]-v[1], 2.0));
  }

  // modified distance (minimisation)
  double distp(const std::vector<T> u, const std::vector<T> v) {
    return std::sqrt(std::pow(std::max(u[0]-v[0], 0.0), 2.0) + std::pow(std::max(u[1]-v[1], 0.0), 2.0));
  }
};
