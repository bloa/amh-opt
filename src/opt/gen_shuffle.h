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
  class gen_shuffle : public amh::gen<SOL> {
  public:
    gen_shuffle(int n, amh::algo<SOL>& _eval) : size(n), eval(_eval) {}

    SOL operator()() {
      SOL out(size);
      out.shuffle();
      eval(out);
      return out;
    }

  private:
    int size;
    amh::algo<SOL>& eval;
  };
}
