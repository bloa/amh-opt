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
  template<class FIT>
  class permutation {
  public:
    typedef FIT Fitness;

    permutation(int n=0) {
      vec_.reserve(n);
      for (int i=0; i<n; i++)
        vec_.push_back(i);
    }
    permutation(std::vector<int>& v) : vec_(v) {}
    // permutation(const permutation& _p) :
    //   vec_(_p.vec()), fitness_(_p.fitness()) {}
    // permutation(permutation&& _p) :
    //   vec_(_p.vec()), fitness_(_p.fitness()) {}

    template<class FOO>
    bool operator==(permutation<FOO>& s) {
      return vec_ == s.vec();
    }

    template<class FOO>
    bool operator!=(permutation<FOO>& s) {
      return vec_ != s.vec();
    }

    int operator[](unsigned int k) {
      return vec_[k];
    }

    const int operator[](unsigned int k) const {
      return vec_[k];
    }

    int size() const {
      return vec_.size();
    }

    void push_back(const int i) {
      vec_.push_back(i);
      fitness_.invalidate();
    }

    void swap(const int i, const int j, const bool invalidate=true) {
      std::swap(vec_[i], vec_[j]);
      if (invalidate)
        fitness_.invalidate();
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
        fitness_.invalidate();
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
          fitness_.invalidate();
      }
    }

    void shuffle() {
      amh::rng.shuffle(vec_.begin(), vec_.end());
      fitness_.invalidate();
    }

    void replace(std::vector<int>& v) {
      vec_ = v;
      fitness_.invalidate();
    }

    std::vector<int>& vec() {
      return vec_;
    }

    const std::vector<int>& vec() const {
      return vec_;
    }

    FIT& fitness() {
      return fitness_;
    }

    const FIT& fitness() const {
      return fitness_;
    }

    void fitness(FIT f) {
      fitness_ = f;
    }

    int flag() {
      return inner_flag;
    }

    void flag(int v) {
      inner_flag = v;
    }

    bool operator==(const permutation& sol) const {
      return fitness_ == sol.fitness();
    }
    bool operator!=(const permutation& sol) const {
      return fitness_ != sol.fitness();
    }
    bool operator<=(const permutation& sol) const {
      return fitness_ <= sol.fitness();
    }
    bool operator<(const permutation& sol) const {
      return fitness_ < sol.fitness();
    }
    bool operator>=(const permutation& sol) const {
      return fitness_ >= sol.fitness();
    }
    bool operator>(const permutation& sol) const {
      return fitness_ > sol.fitness();
    }

    virtual void printOn(std::ostream& os) const {
      fitness_.printOn(os);
      os << ' ' << vec_.size() << ' ';
      std::copy(vec_.begin(), vec_.end(), std::ostream_iterator<int>(os, " "));
    }

    virtual void readFrom(std::istream& is) {
      fitness_.readFrom(is);
      int size, tmp;
      is >> size;
      vec_.resize(size);
      for (unsigned int i=0; i<size; i++) {
        is >> tmp;
        vec_[i] = tmp;
      }
    }

  private:
    std::vector<int> vec_;
    FIT fitness_;
    int inner_flag = 0;
  };

  template<class FIT>
  std::ostream& operator<<(std::ostream& os, const permutation<FIT>& sol) {
    sol.printOn(os);
    return os;
  }
}

