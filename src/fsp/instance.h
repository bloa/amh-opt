/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace fsp {
  class instance {
  public:
    instance(int _n, int _m) {
      generate(_n, _m, -1);
    }
    instance(int _n, int _m, int _seed) {
      generate(_n, _m, _seed);
    }
    instance(const std::string& filename) {
      parse(filename);
    }

    unsigned int m() const {
      return m_;
    }

    unsigned int n() const {
      return n_;
    }

    std::vector<std::vector<int>> p() const {
      return p_;
    }

    std::vector<int> psumv() const {
      return psumv_;
    }

    int psum() {
      return psum_;
    }

    std::vector<int> d() const {
      return d_;
    }

    void parse(const std::string& filename) {
      std::ifstream input(filename);
      std::string line;
      if (!input)
        throw std::runtime_error("Error: unable to open benchmark file");

      std::getline(input, line); // number of jobs
      n_ = std::atoi(line.data());
      std::getline(input, line); // number of machines
      m_ = std::atoi(line.data());
      std::getline(input, line); // seed
      seed_ = std::atoi(line.data());
      p_.resize(m_);
      for (unsigned int j=0; j<m_; j++)
        p_[j].resize(n_);
      d_.resize(n_);
      psumv_.resize(n_);

      // for each job...
      for (unsigned int j=0; j<n_; j++) {
        std::getline(input, line); // index of the job (=> j)
        std::getline(input, line); // due-date of the job j
        d_[j] = std::atoi(line.data());
        std::getline(input, line); // processing times of the job j on each machine
        std::istringstream iss(line);
        int tmp = 0;
        for (unsigned int i=0; i<m_; i++) {
          iss >> p_[i][j];
          tmp += p_[i][j];
        }
        psumv_[j] = tmp;
        psum_ += tmp;
      }
      input.close();
    }

    void write(const std::string& filename) {
      std::ofstream output(filename);
      if (!output)
        throw std::runtime_error("Error: unable to open file");

      output << n_ << "\n"; // number of jobs
      output << m_ << "\n"; // number of machines
      output << seed_ << "\n"; // seed

      // for each job...
      for (unsigned int j=0; j<n_; j++) {
        output << j << "\n"; // index of the job (=> j)
        output << d_[j] << "\n"; // due-date of the job j
        // processing times of the job j on each machine
        for (unsigned int i=0; i<m_; i++) {
          if (i > 0)
            output << " ";
          output << p_[i][j];
        }
        output << "\n";
      }
      output.close();
    }

    void generate(int _n, int _m, int _seed, int lb=1, int ub=99) {
      n_ = _n;
      m_ = _m;
      seed_ = _seed;

      if (_seed != -1)
        amh::rng.reseed(_seed);

      p_.resize(m_);
      for (unsigned int j=0; j<m_; j++)
        p_[j].resize(n_);
      d_.resize(n_);
      psumv_.resize(n_);

      // for each job...
      for (unsigned int j=0; j<n_; j++) {
        d_[j] = 0; // due-date of the job j
        // processing times of the job j on each machine
        int tmp = 0;
        for (unsigned int i=0; i<m_; i++) {
          p_[i][j] = amh::rng.uniform(lb, ub);
          tmp += p_[i][j];
        }
        psumv_[j] = tmp;
        psum_ += tmp;
      }

      // due dates
      double mean = psum_/(n_*m_);
      for (unsigned int j=0; j<n_; j++)
        d_[j] = amh::rng.uniform(mean*m_, mean*(m_+n_+1));
      //d_[j] = psumv_[j]*(1+3*amh::rng::uniform);
    }

    void cover(const instance inst, double corr) {
      std::vector<std::vector<int>> ref_p = inst.p();
      seed_ = -1;
      psum_ = 0;

      for (unsigned int j=0 ; j<n_ ; j++) {
        int tmp = 0;
        for (unsigned int i=0 ; i<m_ ; i++) {
          if (amh::rng.uniform() < corr)
            p_[i][j] = ref_p[i][j];
          tmp += p_[i][j];
        }
        psumv_[j] = tmp;
        psum_ += tmp;
      }
    }

  protected:
    unsigned int m_;
    unsigned int n_;
    unsigned int seed_;
    std::vector<std::vector<int>> p_;
    std::vector<int> d_;
    std::vector<int> psumv_;
    int psum_=0;

  };
}
