/*
                      This source code is part of
  
                 G 4 - N I T R O - C L O S E D - X X X
  
                           VERSION: 1.0.0
  
               Contributors: Juan Manuel Solano-Altamirano
                             Julio Manuel Hernández-Pérez
          Copyright (c) 2024-2025, Juan Manuel Solano-Altamirano
                                   <jmsolanoalt@gmail.com>
  
   -------------------------------------------------------------------
  
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
   ---------------------------------------------------------------------
  
   If you want to redistribute modifications of the suite, please
   consider to include your modifications in our official release.
   We will be pleased to consider the inclusion of your code
   within the official distribution. Please keep in mind that
   scientific software is very special, and version control is 
   crucial for tracing bugs. If in despite of this you distribute
   your modified version, please do not call it DensToolKit.
  
   If you find DensToolKit useful, we humbly ask that you cite
   the paper(s) on the package --- you can find them on the top
   README file.
*/
#ifndef _MOLECULEINERTIATENSOR_H_
#define _MOLECULEINERTIATENSOR_H_
#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;
#include "molecule.h"
#include <cassert>

/* ************************************************************************** */
class MoleculeInertiaTensor {
/* ************************************************************************** */
public:
   MoleculeInertiaTensor(shared_ptr<Molecule> &umol);
   void Setup();
   void DisplayEigenvalues();
   void DisplayEigenvectors();
   void DisplayNormalI();
   void Diagonalize();
   double ComputeNormFactor();
   double TotalMass();
   void ComputeInertiaTensor();
   void CenterMolecule();
   void ComputeTotalMass();
   void ComputeNormalData();
   /** Returns the element data[row][col]  */
   double& operator()(int row, int col);
   /** Returns the element data[row][col]  */
   const double& operator()(int row, int col) const; // for const objects
   vector<double> Eve(int idx) const {vector<double> r(evec[idx]); return r;}
   vector<vector<double> > Eve() const {return evec;}
   vector<double> Eva(void) const {return eval;}
   vector<double> CenterOfMass() const {return initCentOfMass;}
   double Eva(int idx) const {assert(idx<4 && idx>=0); return eval[idx];}
   double NormFactEva() const {return normFact;}
/* ************************************************************************** */
protected:
/* ************************************************************************** */
   void Init();
   MoleculeInertiaTensor();
   vector<vector<double> > data; /*!< Holds the inertia tensor's data  */
   vector<vector<double> > normaldata; /*!< Normalized data.  */
   vector<vector<double> > evec;
   shared_ptr<Molecule> molecule;
   vector<double> initCentOfMass,eval;
   double totalMass,normFact;
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _MOLECULEINERTIATENSOR_H_ */

