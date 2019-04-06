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
  template<class FIT>
  class base_sol {
  public:
    typedef FIT Fitness;

    base_sol() {}
    base_sol(const base_sol& o) {
      fitness(o.fitness());
      flag(o.flag());
    }

    base_sol& operator=(const base_sol<FIT>& o) {
      if (&o == this) return *this;
      fitness(o.fitness());
      flag(o.flag());
      return *this;
    }

    FIT& fitness() {
      return fitness_;
    }

    const FIT& fitness() const {
      return fitness_;
    }

    void fitness(FIT f) {
      fitness_ = f;
    }

    int flag() const {
      return inner_flag;
    }

    void flag(const int v) {
      inner_flag = v;
    }

    bool operator==(const base_sol& sol) const {
      return fitness_ == sol.fitness();
    }
    bool operator!=(const base_sol& sol) const {
      return fitness_ != sol.fitness();
    }
    bool operator<=(const base_sol& sol) const {
      return fitness_ <= sol.fitness();
    }
    bool operator<(const base_sol& sol) const {
      return fitness_ < sol.fitness();
    }
    bool operator>=(const base_sol& sol) const {
      return fitness_ >= sol.fitness();
    }
    bool operator>(const base_sol& sol) const {
      return fitness_ > sol.fitness();
    }

  protected:
    FIT fitness_;
    int inner_flag = 0;
  };
}
