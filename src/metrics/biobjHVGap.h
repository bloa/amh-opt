/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

// todo: auto sort?
// todo: automatic minimisation/maximisation detection

class biobjHVGap {
public:
  biobjHVGap() {}
  biobjHVGap(double ideal_x, double ideal_y, double nadir_x, double nadir_y) :
    ideal({ideal_x, ideal_y}), nadir({nadir_x, nadir_y}) {}
  biobjHVGap(std::vector<double> _ideal, std::vector<double> _nadir) :
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

    // setup full front (with extremities)
    std::vector<std::vector<double>> full_front;
    full_front.push_back(ex_left);
    for (auto pt : front)
      full_front.push_back(pt);
    full_front.push_back(ex_right);

    // compute gap
    double gap = 0;
    std::vector<double> last = full_front[0];
    std::vector<double> tmp = full_front[full_front.size()-1];
    double ratio = (last[1]-tmp[1])*(last[0]-tmp[0]);
    for (auto pt : full_front) {
      gap += (last[1]-pt[1])*(last[0]-pt[0]);
      last = pt;
    }
    return gap/ratio;
  }

private:
  std::vector<double> ideal;
  std::vector<double> nadir;
};
