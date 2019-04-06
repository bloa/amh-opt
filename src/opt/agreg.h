/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace opt {
  template<class SOL>
  class agreg : public amh::algo<SOL> {
  public:
    agreg(amh::algo<SOL>& _eval) : eval(_eval) {}

    void setup(std::vector<double> w) {
      weights = w;
      sum = 0;
      for (auto v : w)
        sum += v;
    }

    SOL operator()(SOL& sol) {
      eval(sol);
      auto& fit = sol.fitness();
      auto scal = sol.fitness().scalar();
      scal = 0;
      for (int i=0; i<weights.size(); i++)
        scal += fit[i]*weights[i];
      scal /= sum; // TODO: remove?
      fit.scalar(scal);
      return sol;
    }

  private:
    amh::algo<SOL>& eval;
    std::vector<double> weights;
    double sum;
  };
}
