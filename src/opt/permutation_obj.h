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
  template<class T, class FIT>
  class permutation_obj : public base_sol<FIT> {
  public:
    typedef FIT Fitness;

    permutation_obj() {}
    permutation_obj(std::vector<T*>& v) : vec_(v) {}
    permutation_obj(const permutation_obj& o) {
      vec_.reserve(o.size());
      for (int i=0; i<o.size(); i++)
        vec_.push_back(o[i]->clone());
      this->fitness(o.fitness());
      this->flag(o.flag());
    }

    ~permutation_obj() {
      for (long unsigned int i=0; i<vec_.size(); i++)
        delete vec_[i];
      vec_.clear();
    }

    permutation_obj& operator=(const permutation_obj<T,FIT>& o) {
      if (&o == this) return *this;
      for (long unsigned int i=0; i<vec_.size(); i++)
        delete vec_[i];
      vec_.clear();
      vec_.reserve(o.size());
      for (int i=0; i<o.size(); i++)
        vec_.push_back(o[i]->clone());
      this->fitness(o.fitness());
      this->flag(o.flag());
      return *this;
    }

    T* operator[](unsigned int k) {
      return vec_[k];
    }

    const T* operator[](unsigned int k) const {
      return vec_[k];
    }

    int size() const {
      return vec_.size();
    }

    void push_back(T* p) {
      vec_.push_back(p);
      this->fitness_.invalidate();
    }

    void swap(const int i, const int j, const bool invalidate=true) {
      std::swap(vec_[i], vec_[j]);
      if (invalidate)
        this->fitness_.invalidate();
    }

    void insert(const int i, const int j, const bool invalidate=true) {
      if (i < j) {
        auto tmp = vec_[i];
        std::move(vec_.begin()+i+1, vec_.begin()+j, vec_.begin()+i);
        vec_[j-1] = tmp;
      } else {
        auto tmp = vec_[i];
        std::move_backward(vec_.begin()+j, vec_.begin()+i, vec_.begin()+i+1);
        vec_[j] = tmp;
      }
      if (invalidate)
        this->fitness_.invalidate();
    }

    void swap_ranges(const int i, const int j, const int k, const bool invalidate=true) {
      if (j == i+1) {
        if (k == j+1)
          return swap(i, j, invalidate);
        else {
          return insert(i, k, invalidate);
        }
      } else if (k == j+1) {
        return insert(j, i, invalidate);
      } else {
        std::swap_ranges(vec_.begin()+i, vec_.begin()+j, vec_.begin()+k+i-j);
        std::swap_ranges(vec_.begin()+j, vec_.begin()+k+i-j, vec_.begin()+i);
        if (invalidate)
          this->fitness_.invalidate();
      }
    }

    void erase(const int i) {
      delete vec_[i];
      vec_.erase(vec_.begin()+i);
    }

    void shuffle() {
      amh::rng.shuffle(vec_.begin(), vec_.end());
      this->fitness_.invalidate();
    }

    void replace(std::vector<T*>& v) {
      vec_ = v;
      this->fitness_.invalidate();
    }

    std::vector<T*>& vec() {
      return vec_;
    }

    const std::vector<T*>& vec() const {
      return vec_;
    }

    virtual void printOn(std::ostream& os) const {
      this->fitness_.printOn(os);
      os << ' ' << vec_.size() << ' ';
      for (unsigned int i=0; i<vec_.size(); i++) {
        os << *vec_[i] << ' ';
      }
    }

    virtual void readFrom(std::istream& is) {
      // this->fitness_.readFrom(is);
      // int size, tmp;
      // is >> size;
      // vec_.resize(size);
      // for (int i=0; i<size; i++) {
      //   is >> tmp;
      //   vec_[i] = tmp;
      // }
    }

  protected:
    std::vector<T*> vec_;
  };

  template<class T, class FIT>
  std::ostream& operator<<(std::ostream& os, const permutation_obj<T,FIT>& sol) {
    sol.printOn(os);
    return os;
  }
}
