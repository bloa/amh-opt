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
  class eval_biobj : public opt::eval_perm<SOL> {
  public:
    eval_biobj(const instance inst1, const instance inst2) : eval1(inst1), eval2(inst2) {}

    SOL operator()(SOL& _qa) {
      auto& fit = _qa.fitness();
      auto objv = fit.objectives();
      objv.resize(0); // todo better?
      objv.push_back(eval1.totalCost(_qa));
      objv.push_back(eval2.totalCost(_qa));
      fit.objectives(objv);
      if (counterT) counter++;
      return _qa;
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
    eval_qap<SOL> eval1;
    eval_qap<SOL> eval2;
  };
}
