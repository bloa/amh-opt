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
  template<class TIN, class TX>
  class pareto_dmls : amh::algo<TIN,TIN> {
  public:
    pareto_dmls(int size) : max_size(size) {}

    TIN operator()(TIN& _in) {
      TIN out, tmp(_in);
      fast_update(last_pop, tmp, out);
      update(tmp, out);
      last_pop.resize(0);
      last_pop.insert(last_pop.end(), out.begin(), out.end());
      return out;
    }

    void fast_update(TIN& last, TIN& tmp, TIN& out) {
      unsigned int i = 0;
      unsigned int m = std::min(last.size(), tmp.size());
      for (; i<m; i++)
        if (last[i] != tmp[i])
          break;
        else
          tmp[i].flag(tmp[i].flag() | last[i].flag());
      out.insert(out.end(), last.begin(), last.begin()+i);
      tmp.erase(tmp.begin(), tmp.begin()+i);
    }

    void update(TIN& tmp, TIN& out) {
      for (TX& x : tmp)
        update(out, x);
    }

    void update(TIN& out, TX& x) {
      TIN tmp(out);
      out.resize(0);
      // keep if not unwanted
      for (TX& y : tmp)
        if (!(x.fitness() < y.fitness()))
          out.push_back(y);
      // bounding
      if (tmp.size() >= max_size)
        return;
      // add iif not unwanted
      for (TX& y : out)
        if (y == x) {
          y.flag(y.flag() | x.flag());
          return;
        } else if (y.fitness() < x.fitness()) {
          return;
        }
      out.push_back(x);
    }

  private:
    TIN last_pop;
    int max_size;
  };
}
