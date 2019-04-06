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
// todo: automatic 5% borders when no boundaries?

class biobjHV {
public:
  biobjHV() : ideal({0,0}) {}
  biobjHV(double nadir_x, double nadir_y) :
    ideal({0, 0}), nadir({nadir_x, nadir_y}) {}
  biobjHV(double ideal_x, double ideal_y, double nadir_x, double nadir_y) :
    ideal({ideal_x, ideal_y}), nadir({nadir_x, nadir_y}) {}
  biobjHV(std::vector<double> _nadir) :
    ideal({0, 0}), nadir(_nadir) {}
  biobjHV(std::vector<double> _ideal, std::vector<double> _nadir) :
    ideal(_ideal), nadir(_nadir) {}

  double operator()(std::vector<std::vector<double>> front) {
    double hv = 0;
    if (front.size() == 0)
      return hv;

    // setup extremities
    std::vector<double> nadir_(nadir);
    if (nadir.size() == 0) {
      nadir_.push_back(front[front.size()-1][0]);
      nadir_.push_back(front[0][1]);
    }

    // hypervolume
    double last_y = nadir_[1];
    double ratio = (nadir_[1]-ideal[1])*(nadir_[0]-ideal[0]);
    for (auto pt : front) {
      hv += (last_y-pt[1])*(nadir_[0]-pt[0]);
      last_y = pt[1];
    }
    return hv/ratio;
  }

private:
  std::vector<double> ideal;
  std::vector<double> nadir;
};
