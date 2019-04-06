/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

#include <chrono>

namespace opt {
  template<class CT, class TIN>
  class check_chrono: public amh::algo<TIN,TIN> {
  public:
    check_chrono() : max(0) {}
    check_chrono(int _max) : max(_max) {}

    void init() {
      start = std::chrono::system_clock::now();
      bak = 0;
    }

    void init(TIN&) {
      start = std::chrono::system_clock::now();
      bak = 0;
    }

    bool check() {
      return last_check;
    }

    bool check(TIN&) {
      elapsed = std::chrono::duration_cast<CT>(std::chrono::system_clock::now() - start);
      last_check = bak + elapsed.count() <= max;
      return last_check;
    }

    void pause() {
      if (!paused) {
        elapsed = std::chrono::duration_cast<CT>(std::chrono::system_clock::now() - start);
        bak += elapsed.count();
        elapsed = (CT) 0;;
        paused = true;
      }
    }

    void resume() {
      if (paused) {
        start = std::chrono::system_clock::now();
        paused = false;
      }
    }

    void set_max(int _max) {
      max = _max;
    }

    int get_max() {
      return max;
    }

    int get_time() {
      return bak + elapsed.count();
    }

  private:
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    int max, bak=0;
    CT elapsed = (CT) 0;
    bool last_check = true;
    bool paused = false;
  };
}
