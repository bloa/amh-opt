/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

#include <ctime>

namespace opt {
  template<class T>
  class check_time: public amh::algo<T,T> {
  public:
    check_time() : max(0) {}
    check_time(int _max): max(_max) {}

    void init() {
      start = time(NULL);
    }

    void init(T&) {
      start = time(NULL);
    }

    bool check() {
      return last_check;
    }

    bool check(T&) {
      bool res;
      elapsed = difftime(time(NULL), start);
      last_check = elapsed <= max;
      return last_check;
    }

    void set_max(int _max) {
      max = _max;
    }

    int get_max() {
      return max;
    }

    int get_time() {
      return elapsed;
    }

  private:
    time_t start;
    int max;
    int elapsed;
    bool last_check = true;
  };
}
