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
  class nbh_swap : public amh::gen<TIN,TIN> {
  public:
    nbh_swap(int _size, TEVAL& eval_) : size(_size), eval(eval_) {}

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
      return size*(size-1)/2;
    }

    TIN operator()(TIN& _in) {
      TIN out(_in);
      // TODO: from paradiseo
      unsigned int n = (unsigned int) ((1 + sqrt(1 + 8*key_))/2);
      int i1 = key_ - (n-1)*n/2;
      int i2 = size - n + i1;
      out = eval.do_swap(out, i1, i2);
      return out;
    }

  protected:
    int size;
    int key_ = 0;
    TEVAL& eval;
  };

  template<class TIN, class TEVAL>
  class nbh_swap_move : public nbh_move<TIN> {
  public:
    //nbh_swap_move() {}
    nbh_swap_move(int _i, int _j, TEVAL& _eval): i(_i), j(_j), eval(_eval) {}

    TIN operator()(TIN& _in) {
      TIN out(_in);
      out = eval.do_swap(out, i, j);
      return out;
    }

  private:
    int i, j;
    TEVAL& eval;
  };

  template<class TIN, class TEVAL>
  class nbh_swap_wmove : public amh::gen<TIN,nbh_move<TIN>*> {
  public:
    nbh_swap_wmove(int _size, TEVAL& eval_) : size(_size), eval(eval_) {}

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
      return size*(size-1)/2;
    }

    nbh_move<TIN>* operator()(TIN& _in) {
      TIN out(_in);
      // TODO: from paradiseo
      unsigned int n = (unsigned int) ( (1 + sqrt(1 + 8 * key_)) / 2);
      int i1 = key_ - (n - 1) * n / 2;
      int i2 = size - 1  - (n - 1 - i1);
      auto move = new nbh_swap_move<TIN,TEVAL>(i1, i2, eval);
      return move;
    }

  protected:
    int size;
    int key_ = 0;
    TEVAL& eval;
  };
}
