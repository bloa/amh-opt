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
  class nbh_shiftmswap : public amh::gen<TIN> {
  public:
    nbh_shiftmswap(int _size, TEVAL& eval_) :
      size(_size), max_((_size-1)*(_size-2)), eval(eval_) {}

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
      key_ = (key_+1)%max_;
    }

    int max() {
      return max_;
    }

    TIN operator()(TIN& _in) {
      TIN out(_in);
      int i1, i2;
      if (size == 3) {
        i1 = key_*2;
        i2 = (1-key_)*3;
      } else {
        if (key_ <= size - 3) {
          i1 = 0;
          i2 = key_ + 3;
        } else {
          i1 = (key_ - 1) / (size - 3);
          if (i1 == size) {
            i1 = size-1;
            i2 = size-3;
          } else {
            int tmp = (key_-1)%(size-3);
            if ( tmp >= i1 - 1)
              i2 = tmp + 4;
            else
              i2 = tmp;
          }
        }
      }
      if (i1 < i2)
        out = eval.do_move(out, i1, i1+1, i2);
      else
        out = eval.do_move(out, i2, i1-1, i1);
      return out;
    }

  protected:
    int size;
    int key_ = 0;
    int max_;
    TEVAL& eval;
  };
}
