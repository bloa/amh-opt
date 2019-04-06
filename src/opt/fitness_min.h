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
  class fitness_min : public fitness_base<double,double> {
  public:

    bool operator==(const fitness_min& _f) const {
      if (valid_scalar && _f.isValidScalar())
        return scalarComp(_f.scalar()) == 0;
      int s = objectives_.size();
      for (int i=0; i<s; i++)
        if (objectives_[i] != _f[i])
          return false;
      return true;
    }

    bool operator!=(const fitness_min& _f) const {
      return !operator==(_f);
    }

    bool operator<=(const fitness_min& _f) const {
      if (valid_scalar && _f.isValidScalar())
        return scalarComp(_f.scalar()) < 1;
      int s = objectives_.size();
      for (int i=0; i<s; i++)
        if (objectives_[i] > _f[i])
          return false;
      return true;
    }

    bool operator<(const fitness_min& _f) const {
      return operator<=(_f) && !operator==(_f);
    }

    bool operator>=(const fitness_min& _f) const {
      if (valid_scalar && _f.isValidScalar())
        return scalarComp(_f.scalar()) > -1;
      int s = objectives_.size();
      for (int i=0; i<s; i++)
        if (objectives_[i] < _f[i])
          return false;
      return true;
    }

    bool operator>(const fitness_min& _f) const {
      return operator>=(_f) && !operator==(_f);
    }

    int scalarComp(fitness_min& _f) const {
      return scalarComp(_f.scalar());
    }

    int scalarComp(double s) const {
      if (scalar_ == s)
        return 0;
      if (scalar_ < s)
        return -1;
      else
        return 1;
    }

    bool lexicoComp(const fitness_min& _f) const {
      int s = objectives_.size();
      for (int k=0; k<s; k++)
        if (objectives_[k] < _f[k])
          return true;
        else if (objectives_[k] > _f[k])
          return false;
      return false;
    }

  };
}
