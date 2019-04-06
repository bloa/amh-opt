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
  template<class TIN, class TEVAL>
  class nbh_adjswap : public amh::gen<TIN> {
  public:
    nbh_adjswap(int _size, TEVAL& eval_) : size(_size), eval(eval_) {}

    void init() {
      key_ = 0;
    }

    void key(int k) {
      key_ = k;
    }

    int key() {
      return key_;
    }

    void next() {
      key_ = (key_+1)%max();
    }

    int max() {
      return (size-1);
    }

    TIN operator()(TIN& _in) {
      TIN out(_in);
      int i1 = key_;
      int i2 = key_ + 1;
      out = eval.do_swap(out, i1, i2);
      return out;
    }

  private:
    int size;
    int key_ = 0;
    TEVAL& eval;
  };
}
