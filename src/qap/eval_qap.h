/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace qap {
  template<class SOL, bool counterT=false>
  class eval_qap : public opt::eval_base<SOL> {
  public:
    eval_qap(const instance inst) : n(inst.n()), flow(inst.flow()), dist(inst.distance())  {}

    SOL operator()(SOL& _qa) {
      auto& fit = _qa.fitness();
      auto objv = fit.objectives();
      objv.resize(1); // todo better?
      objv[0] = totalCost(_qa);
      fit.objectives(objv);
      if (counterT) counter++;
      return _qa;
    }

    double totalCost(const SOL & _qa) {
      double cost = 0;
      for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
          cost += flow[i][j] * dist[_qa[i]][_qa[j]];
      return cost;
    }

    const std::vector<double> getMinMax() {
      std::vector<double> bounds;
      bounds.push_back(lowerCost());
      bounds.push_back(upperCost());
      return bounds;
    }

  public:
    int counter = 0;

  protected:
    const unsigned int n;
    const std::vector<std::vector<int>> flow;
    const std::vector<std::vector<int>> dist;

    int lowerCost() {
      int res = 0;
      int min_dist = dist[0][1];
      int min_flow = flow[0][1];
      for (int i=0 ; i<n ; i++) {
        for (int j=0 ; j<n ; j++) {
          if (i != j) {
            if (flow[i][j] < min_flow)
              min_flow = flow[i][j];
            if (dist[i][j] < min_dist)
              min_dist = dist[i][j];
          }
        }
      }
      return min_dist*min_flow*n*n;
    }

    int upperCost() {
      int res = 0;
      int max_dist = dist[0][1];
      int max_flow = flow[0][1];
      for (int i=0 ; i<n ; i++) {
        for (int j=0 ; j<n ; j++) {
          if (i != j) {
            if (flow[i][j] > max_flow)
              max_flow = flow[i][j];
            if (dist[i][j] > max_dist)
              max_dist = dist[i][j];
          }
        }
      }
      return max_dist*max_flow*n*n;
    }
  };
}
