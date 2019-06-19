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
  template<class SOL, bool counterT=false>
  class eval_pfsp : public opt::eval_perm<SOL> {
  public:
    eval_pfsp(const instance& inst) : n(inst.n()), m(inst.m()), p(inst.p()) {
      c.resize(m);
      for (unsigned int i=0; i<m; i++)
        c[i].resize(n);
    }

    SOL operator()(SOL& _fs) {
      auto& fit = _fs.fitness();
      if (fit.isValid())
        return _fs;
      auto objv = fit.objectives();
      completionTime(_fs);
      objv.resize(0); // todo better?
      objv.push_back(maxCompTime(_fs));
      objv.push_back(totalCompTime(_fs));
      fit.objectives(objv);
      if (counterT) counter++;
      return _fs;
    }

    const std::vector<double> getMinMax() {
      std::vector<double> bounds;
      bounds.push_back(lowerMakespan());
      bounds.push_back(upperMakespan());
      bounds.push_back(lowerFlowtime());
      bounds.push_back(upperFlowtime());
      return bounds;
    }

    void setup(SOL& _fs) {
      completionTime(_fs);
    }

    int totalCompTime(const SOL & _fs) {
      int k = _fs.size();
      int sum = 0;
      for (unsigned int i=0 ; i<k ; i++)
        sum += c[m-1][_fs[i]];
      return sum;
    }

    int maxCompTime(const SOL & _fs) {
      int k = _fs.size();
      return c[m-1][_fs[k-1]];
    }

  public:
    int counter = 0;

  protected:
    const unsigned int n;
    const unsigned int m;
    const std::vector<std::vector<int>> p;
    std::vector< std::vector<double>> c;

    void completionTime(const SOL & _fs) {
      int k = _fs.size();
      if (first_fs) {
        first_fs = false;
        full_completionTime(_fs);
        last_fs.resize(n);
        for (int i=0; i<k; i++)
          last_fs[i] = _fs[i];
        return;
      }
      // compare
      int prefix = 0;
      while (prefix < k && last_fs[prefix] == _fs[prefix])
        prefix++;
      // compute
      if (prefix == 0) {
        full_completionTime(_fs);
      } else {
        c[0][_fs[0]] = p[0][_fs[0]];
        for (unsigned int i=prefix; i<k; i++)
          c[0][_fs[i]] = c[0][_fs[i-1]] + p[0][_fs[i]];
        for (unsigned int j=1; j<m; j++)
          for (unsigned int i=prefix; i<k; i++)
            c[j][_fs[i]] = std::max(c[j][_fs[i-1]], c[j-1][_fs[i]]) + p[j][_fs[i]];
      }
      // store
      if (last_fs.size() != k)
        last_fs.resize(k);
      for (int i=prefix; i<k; i++)
        last_fs[i] = _fs[i];
    }

    bool first_fs = true;
    std::vector<int> last_fs;

    void full_completionTime(const SOL & _fs) {
      int k = _fs.size();
      c[0][_fs[0]] = p[0][_fs[0]];
      for (unsigned int i=1; i<k; i++)
        c[0][_fs[i]] = c[0][_fs[i-1]] + p[0][_fs[i]];
      for (unsigned int j=1; j<m; j++)
        c[j][_fs[0]] = c[j-1][_fs[0]] + p[j][_fs[0]];
      for (unsigned int j=1; j<m; j++)
        for (unsigned int i=1; i<k; i++)
          c[j][_fs[i]] = std::max(c[j][_fs[i-1]], c[j-1][_fs[i]]) + p[j][_fs[i]];
    }

    int upperMakespan() {
      int sumAll = 0;
      int max_pij = 0;
      for (unsigned int i=0; i<n; i++){
        for (unsigned int j=0; j<m; j++){
          sumAll += p[j][i];
          if (p[j][i]>max_pij)
            max_pij = p[j][i];
        }
      }
      int otherMax = (n+m+1) * max_pij;
      return (sumAll < otherMax) ? sumAll : otherMax;
    }

    int lowerMakespan() {
      int sumLast = 0;
      int min_pij = p[0][0];
      int min_jobtime = 0;
      int min_jobnb = 0;
      for (unsigned int i=0; i<n; i++) {
        sumLast += p[m-1][i];
        int jobtime = 0;
        for (unsigned int j=0; j<m; j++)
          jobtime += p[j][i];
        if (jobtime < min_jobtime || min_jobtime == 0) {
          min_jobtime = jobtime;
          min_jobnb = i;
        }
        for (unsigned int j=0; j<m; j++)
          if (p[j][i]<min_pij)
            min_pij = p[j][i];
      }
      sumLast -= p[m-1][min_jobnb];
      int firstMin = (n+m-1) * min_pij;
      int secondMin = sumLast + min_jobnb;
      return (firstMin > secondMin) ? firstMin : secondMin;
    }

    int upperFlowtime(){
      int max_pij = 0;
      for (unsigned int i=0; i<n; i++)
        for (unsigned int j=0; j<m; j++)
          if (p[j][i]>max_pij)
            max_pij = p[j][i];
      return (n*m + (n*(n-1))/2)*max_pij;
    }

    int lowerFlowtime(){
      int sumAll = 0;
      std::vector<int> sortedp0(n);
      std::copy(p[0].begin(), p[0].end(), sortedp0.begin());
      std::sort(sortedp0.begin(), sortedp0.end());
      int sumSmall = 0;
      for(unsigned int i=0; i<n; i++)
        sumSmall += (n-i-1)*sortedp0[i];
      for (unsigned int i=0; i<n; i++)
        for (unsigned int j=0; j<m; j++)
          sumAll += p[j][i];
      return sumSmall + sumAll;
    }
  };

  template<class SOL>
  class eval_pfsp_makespan : public eval_pfsp<SOL> {
  public:
    eval_pfsp_makespan(const instance inst) : eval_pfsp<SOL>(inst) {}

    SOL operator()(SOL& _fs) {
      auto& fit = _fs.fitness();
      if (fit.isValid())
        return _fs;
      auto objv = fit.objectives();
      this->completionTime(_fs);
      objv.resize(0); // todo better?
      objv.push_back(this->maxCompTime(_fs));
      fit.objectives(objv);
      return _fs;
    }
  };
}
