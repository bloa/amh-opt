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
  class nbh_shift : public amh::gen<TIN> {
  public:
    nbh_shift(int _size, TEVAL& eval_) :
      size(_size), max_((_size-1)*(_size-1)), eval(eval_) {}

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
      int i1, i2; // put _in[i1] before _in[i2]
      if (key_ <= size-2) {
        i1 = 0;
        i2 = key_ + 2;
      } else {
        i1 = (key_-1)/(size-2);
        int tmp = (key_-1)%(size-2);
        if (tmp >= i1 - 1)
          i2 = tmp + 3;
        else
          i2 = tmp;
      }
      out = eval.do_insert(out, i1, i2);
      return out;
    }

  protected:
    int size;
    int key_ = 0;
    int max_;
    TEVAL& eval;
  };

  template<class TIN, class TEVAL>
  class nbh_shift_move : public nbh_move<TIN> {
  public:
    nbh_shift_move(int _i, int _j, TEVAL& _eval): i(_i), j(_j), eval(_eval) {}

    TIN operator()(TIN& _in) {
      // put _in[i] before _in[j]
      TIN out(_in);
      out = eval.do_insert(out, i, j);
      return out;
    }

  protected:
    int i, j;
    TEVAL& eval;
  };

  template<class TIN, class TEVAL>
  class nbh_shift_wmove : public amh::gen<TIN,nbh_shift_move<TIN,TEVAL>> {
  public:
    nbh_shift_wmove(int _size, TEVAL& eval_) :
      size(_size), max_((_size-1)*(_size-1)), eval(eval_) {}

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
      int i1, i2; // put _in[i1] before _in[i2]
      if (key_ <= size-2) {
        i1 = 0;
        i2 = key_ + 2;
      } else {
        i1 = (key_-1)/(size-2);
        int tmp = (key_-1)%(size-2);
        if (tmp >= i1 - 1)
          i2 = tmp + 3;
        else
          i2 = tmp;
      }
      return nbh_shift_move<TIN,TEVAL>(i1, i2, eval);
    }

  protected:
    int size;
    int key_ = 0;
    int max_;
    TEVAL& eval;
  };
}
