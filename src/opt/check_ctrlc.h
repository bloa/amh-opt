/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

#include <csignal>

namespace opt {
  bool sigint_handled = false;
  bool sigint_catched = false;

  template<class TIN, bool B=true>
  class check_ctrlc: public amh::algo<TIN,TIN> {
  public:
    check_ctrlc(bool reset=false) : auto_reset(reset) {
      if (!sigint_handled)
        attach();
    }
    ~check_ctrlc() {
      if (sigint_attached)
        detach();
    }

    void init() {
      if (sigint_catched)
        sigint_catched = false;
    }

    void init(TIN&) {
      init();
    }

    static void signal_handler(int) {
      sigint_catched = true;
    }

    bool check() {
      return last_check;
    }

    bool check(TIN&) {
#ifdef SIGINT
      last_check = B ^ sigint_catched;
      if (auto_reset)
        init();
#endif
      return last_check;
    }

    void detach() {
      if (!sigint_attached)
        std::cerr << "warning: sigint not attached" << std::endl;
#ifdef SIGINT
      signal(SIGINT, SIG_IGN);
      sigint_attached = false;
      sigint_handled = false;
#endif
    }

    void attach() {
      if (sigint_attached)
        std::cerr << "warning: sigint already attached" << std::endl;
      else if (sigint_handled)
        throw std::runtime_error("another check_ctrlc is currently attached");
#ifdef SIGINT
      signal(SIGINT, signal_handler);
      sigint_attached = true;
      sigint_handled = true;
#endif
    }

  private:
    bool last_check = B;
    bool auto_reset = false;
    bool sigint_attached = false;
  };
}
