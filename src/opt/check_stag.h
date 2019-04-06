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
  template<class TIN>
  class check_stag: public amh::algo<TIN,TIN> {
  public:
    check_stag(int _max): max(_max) {}

    void init() {
      current = 0;
      is_init = true;
    }

    void init(TIN& _in) {
      init();
      last_in = _in;
    }

    bool check() {
      return last_check;
    }

    bool check(TIN& _in) {
      if (is_init && last_in == _in)
        current++;
      else
        init(_in);
      last_check = current < max;
      return last_check;
    }

    void set_max(int _max) {
      max = _max;
    }

    int get_max() {
      return max;
    }

    int get_current() {
      return current;
    }

  private:
    int max;
    int current = 0;
    TIN last_in;
    bool is_init = false;
    bool last_check = true;
  };
}
