/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace tsp {
  template<class SOL, bool counterT=false>
  class eval_tsp : public opt::eval_base<SOL> {
  public:
    eval_tsp(const instance inst) : d(inst.d()), n(inst.n())  {}

    double fake_2opt(SOL& _ts, double dist, int i, int j) {
      return dist - d[_ts[i]][_ts[(i+1)%n]] - d[_ts[j]][_ts[(j+1)%n]] +
        d[_ts[i]][_ts[j]] + d[_ts[(i+1)%n]][_ts[(j+1)%n]];
    }

    SOL do_2opt(SOL& _ts, int i, int j) {
      auto& fit = _ts.fitness();
      auto objv = fit.objectives();
      objv[0](fake_2opt(_ts, objv[0], i, j));
      fit.objectives(objv);
      unsigned int stop = (j-i+1)/2;
      for (int k=1; k<=stop; k++)
        _ts.swap(i+k, j-k+1, false);
      if (counterT) counter++;
      return _ts;
    }

    // without incremental eval
    // SOL do_2opt(SOL& _ts, int i, int j) {
    //   unsigned int stop = (j-i+1)/2;
    //   for (int k=1; k<=stop; k++)
    //     _ts.swap(i+k, j-k+1);
    //   return operator()(_ts);
    // }

    SOL operator()(SOL& _ts) {
      auto& fit = _ts.fitness();
      auto objv = fit.objectives();
      objv.resize(1); // todo better?
      objv[0] = totalDistance(_ts);
      fit.objectives(objv);
      if (counterT) counter++;
      return _ts;
    }

    const std::vector<double> getMinMax() {
      std::vector<double> bounds;
      bounds.push_back(lowerDistance());
      bounds.push_back(upperDistance());
      return bounds;
    }

    double totalDistance(const SOL & _ts) {
      double dist = 0;
      for (int i=0; i<n-1; i++)
        dist += d[_ts[i]][_ts[i+1]];
      dist += d[_ts[n-1]][_ts[0]];
      return dist;
    }

  public:
    int counter = 0;

  protected:
    const unsigned int n;
    const std::vector<std::vector<double>> d;

    int lowerDistance() {
      int res = 0;
      int min = d[0][1];
      for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
          if (i != j && d[i][j] < min)
            min = d[i][j];
      return min*n;
    }

    int upperDistance() {
      int res = 0;
      int max = d[0][1];
      for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
          if (i != j && d[i][j] > max)
            max = d[i][j];
      return max*n;
    }
  };
}
