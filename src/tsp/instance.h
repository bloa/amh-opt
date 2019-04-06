/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

#pragma once

namespace tsp {
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

    std::vector<int> xaxis() const {
      return xaxis_;
    }

    std::vector<int> yaxis() const {
      return yaxis_;
    }

    std::vector<std::vector<double>> d() const {
      return d_;
    }

    void recompute_distances() {
      d_.resize(n_);
      for (int i=0; i<n_; i++) {
        d_[i].resize(n_);
        d_[i][i] = 0;
        for (int j=0; j<i; j++) {
          if (i != j) {
            double x = sqrt(pow((xaxis_[i]-xaxis_[j]),2)+pow((yaxis_[i]-yaxis_[j]),2));
            d_[i][j] = x;
            d_[j][i] = x;
          }
        }
      }
    }

    void parse(const std::string& filename) {
      std::ifstream input(filename);
      if (!input)
        throw std::runtime_error("Error: unable to open benchmark file");
      std::string line, tmp;

      // skip first 3 lines (file info)
      for (int i=0; i<4; i++)
        std::getline(input, line); // skip file info
      // end of line 4 is number of cities
      std::istringstream iss(line);
      for (int j=0; j<2; j++)
        iss >> tmp;
      if (tmp.compare(":") == 0)
        iss >> tmp;
      n_ = std::stoi(tmp);
      // skip 2 lines (file info)
      for (int i=0; i<2; i++)
        std::getline(input, line); // skip file info

      yaxis_.resize(n_);
      xaxis_.resize(n_);
      for (int i=0; i<n_; i++) {
        std::getline(input, line);
        std::istringstream iss(line);
        iss >> tmp; // skip city id
        iss >> xaxis_[i];
        iss >> yaxis_[i];
      }

      input.close();

      recompute_distances();
    }

    void write(const std::string& filename) {
      std::ofstream output(filename);
      if (!output)
        throw std::runtime_error("Error: unable to open file");
      std::string line, tmp;

      // file info
      output << "NAME : ???\n";
      output << "COMMENT : AMH N=" << n_ << ", seed=" << seed_ << "\n";
      output << "TYPE : TSP\n";
      output << "DIMENSION : " << n_ << "\n";
      output << "EDGE_WEIGHT_TYPE : ???\n";
      output << "NODE_COORD_SECTION\n";

      // cities
      for (int i=0; i<n_; i++)
        output << i+1 << " " << xaxis_[i] << " " << yaxis_[i] << "\n";

      output.close();
    }

    void generate(int _n, int _seed, int ub=3163) {
      n_ = _n;
      seed_ = _seed;

      if (_seed != -1)
        amh::rng.reseed(_seed);

      yaxis_.resize(n_);
      xaxis_.resize(n_);
      for (int i=0; i<n_; i++) {
        xaxis_[i] = amh::rng.uniform(ub);
        yaxis_[i] = amh::rng.uniform(ub);
      }

      recompute_distances();
    }

    void cover(instance inst, double corr) {
      std::vector<int> ref_x = inst.xaxis();
      std::vector<int> ref_y = inst.yaxis();
      seed_ = -1;

      for (int i=0; i<n_; i++) {
        xaxis_[i] = (int) (ref_x[i] + amh::rng.normal(corr));
        yaxis_[i] = (int) (ref_y[i] + amh::rng.normal(corr));
      }

      recompute_distances();
    }

  protected:
    std::vector<std::vector<double>> d_; // distances matrix
    unsigned int n_;
    unsigned int seed_;
    std::vector<int> xaxis_;
    std::vector<int> yaxis_;

  };
}
