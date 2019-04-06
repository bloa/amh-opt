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
  class eval_perm : public amh::algo<SOL> {
  public:
    virtual SOL operator()(SOL& _fs) = 0;

    virtual SOL do_swap(SOL& _fs, int i, int j) {
      _fs.swap(i, j);
      return operator()(_fs);
    }

    virtual SOL do_insert(SOL& _fs, int i, int j) {
      _fs.insert(i, j);
      return operator()(_fs);
    }

    virtual SOL do_move(SOL& _fs, int i, int j, int k) {
      _fs.swap_ranges(i, j, k);
      return operator()(_fs);
    }
  };
}
