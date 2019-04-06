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
  class instance_biobj {
  public:
    instance_biobj(const std::string& filename) {
      parse(filename);
    }

    unsigned int n() const {
      return inst1.n();
    }

    instance inst1;
    instance inst2;

  protected:
    std::vector<std::vector<std::vector<double>>> distance; //distance[i] -> matrice de cout de l'objectif i (i appartenant a [|0;nbObjs[|)
    unsigned int nbVille;
    unsigned int nbObjs;
    std::vector<double> distanceMin; // bound min pour chaque matrice de distance
    std::vector<double> distanceMax;
    std::vector<std::vector<double>> abscisse;
    std::vector<std::vector<double>> ordonne;

    // Après l'ouverture du fichier d'entrée, deux possibilités :
    // - première ligne est égale à 1 : donc instances à générer, la deuxième ligne du fichier indique l'adresse d'un autre fichier qui contient
    // les paramètres pour la génération (seed, nombres de villes, nb d'objectifs, etc)
    // - première ligne est égale à 2 : 2 lignes après, chaque ligne indique l'emplacement d'une matrice de distances

    void parse(const std::string& filename) {
      std::ifstream input(filename); // ouverture premier fichier
      std::string line;
      if (!input)
        throw std::runtime_error("*** ERROR : Unable to open the benchmark file");
      std::getline(input, line); // récupère première ligne fichier (1 ou 2)
      int nb_inst = std::atoi(line.data());
      std::getline(input, line); // récupère lien deuxième fichier (instance A)
      if (nb_inst == 1 ) {
        line.resize(line.size()-1);
        std::ifstream input2(line);
        if (!input2)
          throw std::runtime_error("*** ERROR : Unable to open the benchmark file");
        std::getline(input2, line); // seed, nbVille, nbObjs, variance, corr
        std::istringstream iss(line);
        uint32_t seed;
        iss >> seed;
        iss >> nbVille;
        iss >> nbObjs;
        int variance;
        iss >> variance;
        bool corr;
        iss >> corr;
        distance.resize(nbObjs);
        ordonne.resize(nbObjs);
        abscisse.resize(nbObjs);
        double d,a,ord;
        ordonne[0].resize(nbVille);
        abscisse[0].resize(nbVille);

        if (corr) { //generation d'une instance correle
          for (int i=0; i<nbVille; i++) {
            abscisse[0][i] = amh::rng.uniform(3163); // 3163
            ordonne[0][i] = amh::rng.uniform(3163); // 3163
          }

          for (int o=1; o<nbObjs ; o++) {
            ordonne[o].resize(nbVille);
            abscisse[o].resize(nbVille);
            for (int i=0; i<nbVille; i++) {
              a = amh::rng.normal(variance);
              ord = amh::rng.normal(variance);
              abscisse[o][i] = a+abscisse[0][i];
              ordonne[o][i] = ord+ordonne[0][i];
            }
          }
        } else { //generation de coordonnees sans correlation
          for (int o=0; o<nbObjs ; o++) {
            ordonne[o].resize(nbVille);
            abscisse[o].resize(nbVille);
            for (int i=0; i<nbVille; i++) {
              abscisse[o][i]=amh::rng.uniform(3163); // 3163
              ordonne[o][i]=amh::rng.uniform(3163); // 3163
            }
          }
        }

        // calcul des distances entre chaque villes
        // & calcul tour min / max pour chaque obj
        distanceMin.resize(nbObjs);
        distanceMax.resize(nbObjs);

        double distanceMinTemp[2], distanceMaxTemp[2]; // aide à calculer la distance min/max (bound)

        for (int o=0; o<nbObjs ; o++) {
          distanceMin[o]=0;
          distanceMax[o]=0;
          distance[o].resize(nbVille);
          for (int i=0; i<nbVille; i++) {
            distance[o][i].resize(nbVille);
            distance[o][i][i]=0;

            distanceMinTemp[0] = distanceMinTemp[1] = 3163; // valeur max aléatoire
            distanceMaxTemp[0] = distanceMaxTemp[1] = 0;

            for (int j=0; j<nbVille;j++) {
              d = sqrt(pow((abscisse[o][i]-abscisse[o][j]),2)+pow((ordonne[o][i]-ordonne[o][j]),2));
              distance[o][i][j]=d;

              //calcul bound min
              if (d < distanceMinTemp[0]) {
                if (distanceMinTemp[0] < distanceMinTemp[1]) {
                  distanceMinTemp[1] = d;
                } else {
                  distanceMinTemp[0] = d;
                }
              } else {
                if (d < distanceMinTemp[1])
                  distanceMinTemp[1] = d;
              }

              //calcul bound max
              if (d > distanceMaxTemp[0]) {
                if (distanceMaxTemp[0] > distanceMaxTemp[1]) {
                  distanceMaxTemp[1] = d;
                } else {
                  distanceMaxTemp[0] = d;
                }
              } else {
                if (d > distanceMaxTemp[1])
                  distanceMaxTemp[1] = d;
              }
            }
            distanceMin[o] +=   distanceMinTemp[0] + distanceMinTemp[1] ;
            distanceMax[o] +=   distanceMaxTemp[0] + distanceMaxTemp[1] ;
          }
          distanceMin[o] = distanceMin[o]/2;
          distanceMax[o] = distanceMax[o]/2;
        }

        input2.close();

      } else { // veritable instance
        nbObjs = 2;
        std::string file1, file2, coord;
        file1 = line;
        char cwd[1024];
        for (unsigned int o=0; o<nbObjs; o++) {
          if (o>0)
            std::getline(input, file1);
          std::ifstream input2(file1);
          std::cout << file1 << std::endl;
          if (!input2)
            throw std::runtime_error("*** ERROR : Unable to open the benchmark file");
          unsigned int i, j;
          for (i=0; i<4; i++)
            std::getline(input2, line); // skip file info
          std::istringstream iss(line);
          for (j=0; j<2; j++)
            iss >> coord; // récupération du nb de villes
          nbVille = std::stoi(coord);
          for (i=0; i<2; i++)
            std::getline(input2, line); // skip file info

          distance.resize(nbObjs);
          ordonne.resize(nbObjs);
          abscisse.resize(nbObjs);
          ordonne[o].resize(nbVille);
          abscisse[o].resize(nbVille);
          for (i=0; i<nbVille; i++) {
            std::getline(input2, line);
            std::istringstream iss(line);
            iss >> coord;
            iss >> abscisse[o][i];
            iss >> ordonne[o][i];
          }
        }

        double d,a,ord;

        // calcul des distances entre chaque villes
        // & calcul tour min / max pour chaque obj
        distanceMin.resize(nbObjs);
        distanceMax.resize(nbObjs);

        double distanceMinTemp[2], distanceMaxTemp[2]; // aide à calculer la distance min/max (bound)

        for (int o=0; o<nbObjs; o++) {
          distanceMin[o] = 0;
          distanceMax[o] = 0;
          distance[o].resize(nbVille);
          for (int i=0; i<nbVille; i++) {
            distance[o][i].resize(nbVille);
            distance[o][i][i] = 0;

            distanceMinTemp[0] = distanceMinTemp[1] = 3163; // valeur max aléatoire
            distanceMaxTemp[0] = distanceMaxTemp[1] = 0;

            for (int j=0; j<nbVille; j++) {
              d = sqrt(pow((abscisse[o][i]-abscisse[o][j]),2)+pow((ordonne[o][i]-ordonne[o][j]),2));
              distance[o][i][j]=d;
              if (d != 0) {
                //calcul bound min
                if (d < distanceMinTemp[0]) {
                  if (distanceMinTemp[0] < distanceMinTemp[1]) {
                    distanceMinTemp[1] = d;
                  } else {
                    distanceMinTemp[0] = d;
                  }
                } else {
                  if (d < distanceMinTemp[1]) distanceMinTemp[1] = d;
                }

                //calcul bound max
                if (d > distanceMaxTemp[0]) {
                  if (distanceMaxTemp[0] > distanceMaxTemp[1]) {
                    distanceMaxTemp[1] = d;
                  } else {
                    distanceMaxTemp[0] = d;
                  }
                } else {
                  if (d > distanceMaxTemp[1]) distanceMaxTemp[1] = d;
                }
              }
            }
            distanceMin[o] += distanceMinTemp[0] + distanceMinTemp[1] ;
            distanceMax[o] += distanceMaxTemp[0] + distanceMaxTemp[1] ;
          }
          distanceMin[o] = distanceMin[o]/2;
          distanceMax[o] = distanceMax[o]/2;
        }

        input.close();
      }
    }
  };
}
