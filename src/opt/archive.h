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
  class archive {
  public:
    typedef typename std::vector<SOL>::iterator IT;
    archive() {}
    archive(SOL& _v) : pfs_vector({_v}) {}
    archive(SOL&& _v) : pfs_vector({std::move(_v)}) {}
    archive(const archive& _a) : pfs_vector(_a.vector()) {}
    archive(archive&& _a) {
      if (this != &_a)
        pfs_vector = std::move(_a.vector());
    }
    archive(std::vector<SOL>& _a) : pfs_vector(_a) {}

    template<class FOO>
    bool operator==(archive<FOO>& s) {
      return pfs_vector == s.vector();
    }

    template<class FOO>
    bool operator!=(archive<FOO>& s) {
      return pfs_vector != s.vector();
    }

    SOL& operator[](unsigned int k) {
      return pfs_vector[k];
    }

    const SOL& operator[](unsigned int k) const {
      return pfs_vector[k];
    }

    archive& operator=(archive& a) {
      pfs_vector = a.vector();
      return *this;
    }

    archive& operator=(archive&& a) {
      if (this != &a) {
        pfs_vector = a.vector();
      }
      return *this;
    }

    void push_back(SOL fs) {
      pfs_vector.push_back(fs);
    }

    void push_back(archive a) {
      for (auto& fs : a)
        pfs_vector.push_back(fs);
    }

    IT begin() {
      return pfs_vector.begin();
    }

    IT end() {
      return pfs_vector.end();
    }

    void resize(unsigned int n) {
      pfs_vector.resize(n);
    }

    void replace(archive& _a) {
      // pfs_vector.assign(_a.begin(), _a.end());
      pfs_vector = _a.vector();
    }

    void replace(archive&& _a) {
      // pfs_vector.assign(_a.begin(), _a.end());
      pfs_vector = _a.vector();
    }

    int size() {
      return pfs_vector.size();
    }

    void sort() {
      std::sort(pfs_vector.begin(), pfs_vector.end(), [&](SOL i, SOL j) {
          return i.fitness().lexicoComp(j.fitness());
        });
    }

    archive sorted() {
      archive tmp(*this);
      tmp.sort();
      return tmp;
    }

    void scalarSort() {
      sort();
      std::sort(pfs_vector.begin(), pfs_vector.end(), [&](SOL i, SOL j) {
          return i.fitness().scalarComp(j.fitness()) == -1;
        });
    }

    void shuffle() {
      amh::rng.shuffle(pfs_vector.begin(), pfs_vector.end());
    }

    void insert(IT pos, const SOL& sol) {
      pfs_vector.insert(pos, sol);
    }

    void insert(IT pos, IT first, IT last) {
      pfs_vector.insert(pos, first, last);
    }

    void erase(IT first, IT last) {
      pfs_vector.erase(first, last);
    }

    void erase(IT pos) {
      pfs_vector.erase(pos);
    }

    std::vector<SOL>& vector() {
      return pfs_vector;
    }

    const std::vector<SOL>& vector() const {
      return pfs_vector;
    }

    void printOn(std::ostream & _os) const {
      std::copy(pfs_vector.begin(), pfs_vector.end(), std::ostream_iterator<SOL>(_os, "\n"));
    }

  private:
    std::vector<SOL> pfs_vector;
  };

  template<class SOL>
  std::ostream& operator<<( std::ostream& _os, const archive<SOL>& _a ) {
    _a.printOn(_os);
    return _os;
  }
}
