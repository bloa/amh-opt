/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

#include <cmath>

// todo: auto sort?

class biobjSpread {
public:
  biobjSpread() {}
  biobjSpread(double ideal_x, double ideal_y, double nadir_x, double nadir_y) :
    ideal({ideal_x, ideal_y}), nadir({nadir_x, nadir_y}) {}
  biobjSpread(std::vector<double> _ideal, std::vector<double> _nadir) :
    ideal(_ideal), nadir(_nadir) {}

  double operator()(std::vector<std::vector<double>> front) {
    if (front.size() < 2)
      return 1;

    // setup extremities
    std::vector<double> ideal_(ideal), nadir_(nadir), ex_left, ex_right;
    if (ideal.size() == 0) {
      ideal_.push_back(front[0][0]);
      ideal_.push_back(front[front.size()-1][1]);
    }
    if (nadir.size() == 0) {
      nadir_.push_back(front[front.size()-1][0]);
      nadir_.push_back(front[0][1]);
    }
    ex_left.push_back(ideal_[0]);
    ex_left.push_back(nadir_[1]);
    ex_right.push_back(nadir_[0]);
    ex_right.push_back(ideal_[1]);

    if (ex_left[0] == ex_right[0] && ex_left[1] == ex_right[1])
      return 1;

    // distances between reference points and extremities of the front
    double df = dist(front[0], ex_left);
    double dl = dist(front[front.size()-1], ex_right);

    // distances between neighbours of the front
    std::vector<double> distances;
    for (int i=0; i<front.size()-1; i++)
      distances.push_back(dist(front[i], front[i+1]));

    // average distance
    double avgDist = 0;
    for (int i=0; i<distances.size(); i++)
      avgDist += distances[i];
    avgDist /= distances.size();

    // final spread
    double sumDiff = 0;
    for (int i=0; i<distances.size(); i++)
      sumDiff += std::abs(distances[i]-avgDist);
    return (df+dl+sumDiff)/(df+dl+distances.size()*avgDist);
  }

private:
  std::vector<double> ideal;
  std::vector<double> nadir;

  // euclidian distance
  double dist(const std::vector<double> u, const std::vector<double> v) {
    return std::sqrt(std::pow(u[0]-v[0], 2.0) + std::pow(u[1]-v[1], 2.0));
  }
};
