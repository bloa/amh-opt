#pragma once

namespace tsp {
  template<class TIN, class TEVAL, class SOL>
  class nbh_twoopt : public amh::gen<TIN> {
  public:
    nbh_twoopt(int _size, TEVAL& eval_) :
      size(_size), max_(_size*(_size-3)/2), eval(eval_) {} // nico size ?

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
      int x = ((3 + sqrt(9 + 8 * key_))/2);
      int y = x*(x-3)/2;
      int delta = size - x + 1;

      int i1, i2;
      i1 = key_-y;
      i2 = i1 + delta;
      out = eval.do_2opt(out, i1, i2);
      return order(out);
    }

    // nico, fais en sorte que la permutation commencer par 0 et bon ordre
    SOL order(SOL _fs) {
      int pos_1, pos_2;
      for (int i=0; i<size; i++) {
        if (_fs[i] == 1)
          pos_1 = i;
        if (_fs[i] == 2)
          pos_2 = i;
      }
      if (pos_2 < pos_1)
        for (int i=1; i<size/2; i++)
          _fs.swap(i, size-i, false);
      return _fs;
    }

  protected:
    int size;
    int key_ = 0;
    int max_;
    TEVAL& eval;
  };
}
