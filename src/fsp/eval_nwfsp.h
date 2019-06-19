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
  class eval_nwfsp : public opt::eval_perm<SOL> {
  public:
    eval_nwfsp(const instance& inst) :
      n(inst.n()), m(inst.m()), p(inst.p()), psumv(inst.psumv()) {
      c.resize(n);
      compute_delay();
    }

    SOL operator()(SOL& _fs) {
      auto& fit = _fs.fitness();
      if (fit.isValid())
        return _fs;
      auto objv = fit.objectives();
      completionTime(_fs);
      objv.resize(0); // todo better?
      objv.push_back(maxCompTime(_fs));
      fit.objectives(objv);
      return _fs;
    }

    // SOL do_swap(SOL& _fs, int i, int j) {
    //   // todo
    // }

    SOL do_insert(SOL& _fs, int i, int j) {
      // _fs.insert(i, j);
      // return operator()(_fs);
      auto& fit = _fs.fitness();
      if (!fit.isValid()) {
        _fs.insert(i, j, true);
        return operator()(_fs);
      }
      auto objv = fit.objectives();
      // incremental makespan
      objv[0] = objv[0] - makespan_remove(_fs, i) + makespan_insert(_fs, _fs[i], j);
      _fs.insert(i, j, false);
      fit.objectives(objv);
      return _fs;
    }

    // SOL do_move(SOL& _fs, int i, int j) {
    //   // todo
    // }

  private:
    const unsigned int n;
    const unsigned int m;
    const std::vector<std::vector<int>> p;
    const std::vector<int> psumv;
    std::vector<double> c;
    std::vector<std::vector<double>> d;

    int totalCompTime(const SOL & _fs) {
      int k = _fs.size();
      int sum = 0;
      for (unsigned int i=0; i<k ; i++)
        sum += c[_fs[i]];
      return sum;
    }

    int maxCompTime(const SOL & _fs) {
      int k = _fs.size();
      return c[_fs[k-1]];
    }

    void completionTime(const SOL & _fs) {
      int k = _fs.size();
      unsigned int delay = 0;
      // c[i] = sum delay + psum[i]
      c[_fs[0]] = psumv[_fs[0]];
      for (unsigned int i=1; i<k; i++) {
        delay += d[_fs[i-1]][_fs[i]];
        c[_fs[i]] = delay + psumv[_fs[i]];
      }
    }

    void compute_delay() {
      // ref: A heuristic for no-wait flow shop scheduling (2013)
      // by Sagar U. Sapkal & Dipak Laha
      d.resize(n);
      for (unsigned int i=0; i<n; i++)
        d[i].resize(n);
      for (unsigned int i=0; i<n; i++) {
        for (unsigned int j=0; j<n; j++) {
          if (i == j) {
            d[i][j] = 0;
          } else {
            int max = 0;
            for (unsigned int r=1; r<=m; r++) {
              int s = 0;
              for (unsigned int h=0; h<r-1; h++)
                s += p[h+1][i] - p[h][j];
              if (s > 0 && s > max)
                max = s;
            }
            d[i][j] = p[0][i] + max;
          }
        }
      }
    }

    double makespan_remove(const SOL& _fs, unsigned int i) {
      if (i == 0)
        return d[_fs[0]][_fs[1]];
      else if (i == _fs.size()-1)
        return d[_fs[i-1]][_fs[i]] + psumv[_fs[i]] - psumv[_fs[i-1]];
      else
        return -d[_fs[i-1]][_fs[i+1]] + d[_fs[i-1]][_fs[i]] + d[_fs[i]][_fs[i+1]];
    }

    double makespan_insert(const SOL& _fs, unsigned int i, unsigned int j) {
      if (j == 0)
        return d[i][_fs[0]];
      else if (j == _fs.size())
        return d[_fs[j-1]][i] + psumv[i] - psumv[_fs[j-1]];
      else
        return d[_fs[j-1]][i] + d[i][_fs[j]] - d[_fs[j-1]][_fs[j]];
    }
  };
}
