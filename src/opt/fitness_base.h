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
  template<class NUM=double, class NUM_SCAL=NUM>
  class fitness_base {
  public:
    fitness_base() {
      valid = false;
      valid_scalar = false;
    }

    fitness_base(std::vector<NUM> objv, NUM_SCAL scal=0) {
      objectives_ = objv;
      scalar_ = scal;
      valid = true;
    }

    std::vector<NUM> objectives() {
      return objectives_;
    }

    NUM_SCAL& scalar() {
      return scalar_;
    }

    const NUM_SCAL& scalar() const {
      return scalar_;
    }

    void objectives(std::vector<NUM> objv) {
      objectives_ = objv;
      valid = true;
    }

    NUM get(int k) {
      return objectives_[k];
    }

    void scalar(NUM_SCAL v) {
      scalar_ = v;
      valid_scalar = true;
    }

    bool isValid() const {
      return valid;
    }

    bool isValidScalar() const {
      return valid_scalar;
    }

    void invalidate() {
      valid = false;
      valid_scalar = false;
    }

    void invalidate_scalar() {
      valid_scalar = false;
    }

    NUM& operator[](unsigned int k) {
      return objectives_[k];
    }

    const NUM& operator[](unsigned int k) const {
      return objectives_[k];
    }

    virtual void printOn(std::ostream & _os) const {
      if (valid)
        std::copy(objectives_.begin(), objectives_.end(), std::ostream_iterator<NUM>(_os, " "));
      else
        _os << "INVALID ";
      if (valid_scalar)
        _os << "(" << scalar_ << ") ";
    }

    virtual void readFrom(std::istream& is) {
      char c;
      NUM tmp;
      std::string s;
      objectives_.resize(0);
      valid_scalar = false;
      while (true) {
        c = is.peek();
        if (c == ' ') {
          is.get(c);
          c = is.peek();
        }
        if (c == 'I') {
          valid = false;
          is >> s;
          // todo: check if really "INVALID" ?
          break;
        } if (c == '(') {
          is >> s;
          s = s.substr(1, s.size()-2);
          std::istringstream ss(s);
          ss >> scalar_;
          valid_scalar = true;
          break;
        } else if (c == ' ') {
          break;
        }
        is >> tmp;
        objectives_.push_back(tmp);
        valid = true;
      }
    }

    static constexpr NUM_SCAL tolerance = 1e-10;

  protected:
    std::vector<NUM> objectives_;
    NUM_SCAL scalar_ = 0;
    bool valid = false;
    bool valid_scalar = false;
  };

  template<class NUM>
  std::ostream& operator<<(std::ostream& _os, const fitness_base<NUM>& _o) {
    _o.printOn(_os);
    return _os;
  }
}
