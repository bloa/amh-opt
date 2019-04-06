/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace fsp {
  template<class SOL>
  class NEH : public amh::gen<SOL> {
  public:
    typedef typename SOL::Fitness FIT;

    NEH(amh::algo<SOL>& _eval, int _size) : eval(_eval), size(_size) {}

    SOL operator()() {
      SOL best, tmp;
      std::vector<int> indexes;
      std::vector<FIT> fitness;
      bool minimizing = true;

      // init
      for (int i=0; i<size; i++) {
        SOL sol;
        sol.push_back(i);
        sol = eval(sol);
        fitness.push_back(sol.fitness());
        indexes.push_back(i);
      }
      std::sort(indexes.begin(), indexes.end(), [&](int i, int j) {
          return fitness[i] < fitness[j];
        });

      // first member
      best.push_back(indexes[0]);

      // for each other members
      for (int i=1; i<size; i++)
        // insert at the best position
        best_insert(best, indexes[i], eval);

      best = eval(best);
      return best;
    }

    static void best_insert(SOL& _sol, int k, amh::algo<SOL>& _eval) {
      // TODO: avoid superfluous eval
      SOL tmp;
      std::vector<int> tmpv;
      int best_j = 0;
      FIT best_f;
      // find best insertion
      for (int j=0; j<_sol.size()+1; j++) {
        tmpv = _sol.vec();
        tmpv.insert(tmpv.begin()+j, k);
        tmp.replace(tmpv);
        tmp = _eval(tmp);
        if (j == 0 || tmp.fitness() < best_f) {
          best_j = j;
          best_f = tmp.fitness();
        }
      }
      tmpv = _sol.vec();
      tmpv.insert(tmpv.begin()+best_j, k);
      _sol.replace(tmpv);
      _sol = _eval(_sol);
      // _sol.fitness().scalar(best_f);
    }

  private:
    amh::algo<SOL>& eval;
    int size;
  };
}
