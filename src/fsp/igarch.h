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
  template<class SOL, class ARCH>
  class IGArchBase : public amh::algo<SOL,ARCH> {
  public:
    virtual ARCH operator()(SOL& sol) = 0;

    virtual SOL best() = 0;

    virtual SOL current() = 0;

    virtual void reset() = 0;

  };

  template<class SOL, class ARCH>
  class IGArch : public IGArchBase<SOL, ARCH> {
  public:

    /*
      _eval: evaluation function
      _ck: termination criteria function
      _neighb: neighbourdhood generator
      _d: number of removed jobs
      _nls: number of neighbourdhood scans (-1: infinite)
     */
    IGArch(amh::algo<SOL>& _eval, amh::algo<SOL>& _ck, amh::gen<SOL> _neighb, int _d, int _nls):
      eval(_eval), ck(_ck), d(_d), nls(_nls), neighb(_neighb) {}

    ARCH operator()(SOL& sol) {
      SOL current = eval(sol);
      SOL best = current;
      ARCH arch = best;
      arch[0].fitness().invalidate_scalar();
      if (has_best) {
        last_best.fitness().invalidate_scalar();
        best = eval(last_best);
        arch = last_arch;
      }
      has_best = true;
      while (ck.check(current)) {
        SOL newsol(current);
        newsol = de_reconstruct(newsol);
        newsol = monols(newsol);
        newsol = eval(newsol);
        if (newsol < current) {
          current = newsol;
          arch.push_back(current);
          arch[arch.size()-1].fitness().invalidate_scalar();
          arch = pareto(arch);
          if (current < best)
            best = current;
        } else {
          if (accept(newsol, current))
            current = newsol;
        }
      }
      last_arch = arch;
      last_best = best;
      last_current = current;
      return arch;
    }

    SOL best() {
      return last_best;
    }

    SOL current() {
      return last_current;
    }

    void reset() {
      has_best = false;
    }

    SOL de_reconstruct(SOL& sol) {
      int index;
      std::vector<int> indexes;

      // deconstruction
      for (int k=0; k<d; k++) {
        index = amh::rng.random(sol.size());
        indexes.push_back(sol[index]);
        sol.schedule().erase(sol.schedule().begin()+index);
        sol.fitness().invalidate();
      }

      // reconstruction
      amh::rng.shuffle(indexes.begin(), indexes.end());
      for (auto index : indexes)
        NEH<SOL>::best_insert(sol, index, eval);

      return sol;
    }

    SOL monols(SOL& sol) {
      sol = eval(sol);
      int neighscans = 0;

      while (neighscans < nls || nls == -1) {
        SOL old = sol;
        neighb.init();
        for (int i=0; i<neighb.max(); i++) {
          SOL tmp = neighb(sol);
          if (tmp < sol) {
            sol = tmp;
            break;
          }
          neighb.next();
        }
        if (old == sol)
          break;
        neighscans++;
      }
      return sol;
    }

    virtual bool accept(SOL newsol, SOL current) = 0;

  private:
    amh::algo<SOL>& eval;
    amh::algo<SOL>& ck;
    int d;
    int nls;
    amh::gen<SOL> neighb;
    opt::pareto<ARCH,SOL> pareto;
    ARCH last_arch;
    SOL last_best, last_current;
    bool has_best = false;
  };

  // IGArch for minimizing makespan

  template<class SOL, class ARCH>
  class IGArch_Cmax : public IGArch<SOL, ARCH> {
  public:

    IGArch_Cmax(amh::algo<SOL>& _eval, amh::algo<SOL>& _ck, amh::gen<SOL> _neighb, fsp::instance inst):
      IGArch<SOL,ARCH>(_eval, _ck, _neighb, 4, -1) {
      constT = 0.4 * inst.psum() / (inst.n() * inst.m() * 10); // Tc = 0.4
    }

    bool accept(SOL newsol, SOL current) {
      double proba = exp((current.fitness().scalar() - newsol.fitness().scalar())/constT);
      return amh::rng.uniform() < proba;
    }

  private:
    double constT;
  };

  // IGArch for minimizing flowtime

  template<class SOL, class ARCH>
  class IGArch_Sft : public IGArch<SOL, ARCH> {
  public:

    IGArch_Sft(amh::algo<SOL>& _eval, amh::algo<SOL>& _ck, amh::gen<SOL> _neighb, fsp::instance inst):
      IGArch<SOL,ARCH>(_eval, _ck, _neighb, 5, 3) {
      constT = 0.5 * inst.psum() / (inst.m() * 10); // Tc = 0.5
    }

    bool accept(SOL newsol, SOL current) {
      double proba = exp((current.fitness().scalar() - newsol.fitness().scalar())/constT);
      return amh::rng.uniform() < proba;
    }

  private:
    double constT;
  };

  // IGArch for minimizing scalarized problems

  template<class SOL, class ARCH>
  class IGArch_Scalar : public IGArch<SOL, ARCH> {
  public:

    IGArch_Scalar(amh::algo<SOL>& _eval, amh::algo<SOL>& _ck, amh::gen<SOL> _neighb):
      IGArch<SOL,ARCH>(_eval, _ck, _neighb, 5, 1) {}

    bool accept(SOL newsol, SOL current) {
      double proba = exp(100*((current.fitness().scalar() - newsol.fitness().scalar())/current.fitness().scalar())/6);
      return amh::rng.uniform() < proba;
    }
  };

  // to allow IG<SOL> being handled as algo<SOL,ARCH>

  template<class SOL, class ARCH>
  class IGArch_Mock : public IGArchBase<SOL, ARCH> {
  public:

    IGArch_Mock(IG<SOL>& _igbase):
      igbase(_igbase) {}

    ARCH operator()(SOL& sol) {
      return igbase(sol);
    }

    SOL best() {
      return igbase.best();
    }

    SOL current() {
      return igbase.current();
    }

    void reset() {
      igbase.reset();
    }

    bool accept(SOL newsol, SOL current) {
      return false;
    }

  private:
    IG<SOL>& igbase;
  };
}
