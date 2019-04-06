/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace qap {
  class instance {
  public:
    instance(int _n) {
      generate(_n, -1);
    }
    instance(int _n, int _seed) {
      generate(_n, _seed);
    }
    instance(const std::string& filename) {
      parse(filename);
    }

    unsigned int n() const {
      return n_;
    }

    std::vector<std::vector<int>> flow() const {
      return flow_;
    }

    std::vector<std::vector<int>> distance() const {
      return distance_;
    }

    void parse(const std::string& filename) {
      std::ifstream input(filename);
      std::string::size_type start, end;
      std::string line;
      if (!input)
        throw std::runtime_error("Error: unable to open benchmark file");

      std::getline(input, line); // number of facilities
      n_ = std::atoi(line.data());
      std::getline(input, line); // blank line

      // flow
      flow_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_));
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        getline(input, line);
        start = line.find_first_not_of(" ");
        flow_[i].resize(n_);
        for (unsigned int j=0; j<n_; j++) {
          end = line.find_first_of(" ", start);
          flow_[i][j] = atoi(line.substr(start, end - start).data());
          start = line.find_first_not_of(" ", end);
        }
      }
      getline(input, line); // blank line

      // distance
      distance_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_));
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        getline(input, line);
        start = line.find_first_not_of(" ");
        distance_[i].resize(n_);
        for (unsigned int j=0; j<n_; j++) {
          end = line.find_first_of(" ", start);
          distance_[i][j] = atoi(line.substr(start, end - start).data());
          start = line.find_first_not_of(" ", end);
        }
      }
      input.close();
    }

    void write(const std::string& filename) {
      std::ofstream output(filename);
      if (!output)
        throw std::runtime_error("Error: unable to open file");

      output << n_ << "\n"; // number of facilities
      output << "\n"; // blank line

      // flow
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        for (unsigned int j=0; j<n_; j++) {
          if (j > 0)
            output << " ";
          output << flow_[i][j];
        }
        output << "\n";
      }
      output << "\n"; // blank line

      // distance
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        for (unsigned int j=0; j<n_; j++) {
          if (j > 0)
            output << " ";
          output << distance_[i][j];
        }
        output << "\n";
      }
      output.close();
    }

    void generate(int _n, int _seed, int lb=1, int ub=99) {
      std::vector<int> xaxis_;
      std::vector<int> yaxis_;
      n_ = _n;

      amh::rng.reseed(_seed);

      yaxis_.resize(n_);
      xaxis_.resize(n_);
      for (int i=0; i<n_; i++) {
        xaxis_[i] = amh::rng.uniform(lb, ub);
        yaxis_[i] = amh::rng.uniform(lb, ub);
      }

      // flow
      flow_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_));
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        flow_[i].resize(n_);
        for (unsigned int j=0; j<i; j++) {
          flow_[i][j] = amh::rng.uniform(lb, ub);
          flow_[j][i] = flow_[i][j];
        }
        flow_[i][i] = 0;
      }

      // distance
      distance_ = std::vector<std::vector<int>>(n_, std::vector<int>(n_));
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        flow_[i].resize(n_);
        for (unsigned int j=0; j<i; j++) {
          distance_[i][j] = std::abs(xaxis_[i]-xaxis_[j]) + std::abs(yaxis_[i]-yaxis_[j]);
          distance_[j][i] = distance_[i][j];
        }
        distance_[i][i] = 0;
      }
    }

    void cover(instance inst, double corr) {
      std::vector<std::vector<int>> ref_flow = inst.flow();
      std::vector<std::vector<int>> ref_distance = inst.distance();

      // flow
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        for (unsigned int j=0; j<i; j++) {
          if (amh::rng.uniform() < corr) {
            flow_[i][j] = ref_flow[i][j];
            flow_[j][i] = flow_[i][j];
          }
        }
      }

      // distance
      // for each facility...
      for (unsigned int i=0; i<n_; i++) {
        for (unsigned int j=0; j<i; j++) {
          distance_[i][j] = ref_distance[i][j];
          distance_[j][i] = distance_[i][j];
        }
      }
    }

  protected:
    unsigned int n_;
    std::vector<std::vector<int>> flow_;
    std::vector<std::vector<int>> distance_;

  };
}
