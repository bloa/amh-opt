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
  class eval_biobj : public opt::eval_base<SOL> {
  public:
    eval_biobj(const instance inst1, const instance inst2) : eval1(inst1), eval2(inst2) {}

    SOL do_2opt(SOL& _ts, int i, int j) {
      auto& fit = _ts.fitness();
      auto objv = fit.objectives();
      auto tmp = objv;
      objv.resize(0);
      objv.push_back(eval1.fake_2opt(_ts, tmp[0], i, j));
      objv.push_back(eval2.fake_2opt(_ts, tmp[1], i, j));
      fit.objectives(objv);
      int stop = (j-i+1)/2;
      for (int k=1; k<=stop; k++)
        _ts.swap(i+k, j-k+1, false);
      if (counterT) counter++;
      return _ts;
    }

    SOL operator()(SOL& _ts) {
      auto& fit = _ts.fitness();
      auto objv = fit.objectives();
      objv.resize(0); // todo better?
      objv.push_back(eval1.totalDistance(_ts));
      objv.push_back(eval2.totalDistance(_ts));
      fit.objectives(objv);
      if (counterT) counter++;
      return _ts;
    }

    const std::vector<double> getMinMax() {
      std::vector<double> bounds, tmp;
      tmp = eval1.getMinMax();
      bounds.push_back(tmp[0]);
      bounds.push_back(tmp[1]);
      tmp = eval2.getMinMax();
      bounds.push_back(tmp[0]);
      bounds.push_back(tmp[1]);
      return bounds;
    }

  public:
    int counter = 0;

  protected:
    eval_tsp<SOL> eval1;
    eval_tsp<SOL> eval2;
  };
}
