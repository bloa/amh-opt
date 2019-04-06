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
  class nbh_move : public amh::algo<TIN,TIN> {
  public:
    nbh_move() {}

    TIN operator()(TIN& _in) {
      return _in;
    }

  private:
  };
}
