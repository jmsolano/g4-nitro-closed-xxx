/*
                      This source code is part of
  
                             G 4 - X X X
  
                           VERSION: 1.0.0
  
               Contributors: Juan Manuel Solano-Altamirano
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
#ifndef _MOLECULEINERTIATENSOR_CPP_
#define _MOLECULEINERTIATENSOR_CPP_

#include <cstdlib>
#include <iostream>
#if DEBUG
#include <cassert>
#endif
using std::cout;
using std::endl;
using std::cerr;
#include <iomanip>
using std::scientific;
using std::setprecision;
#include "moleculeinertiatensor.h"
#include "eigendecompositionjama.h"
#include "nrjacobi.h"
#include "matrixvectoroperations3d.h"
#include "screenutils.h"

MoleculeInertiaTensor::MoleculeInertiaTensor(shared_ptr<Molecule> &umol)
   : MoleculeInertiaTensor() {
   molecule=umol;
   Setup();
}
MoleculeInertiaTensor::MoleculeInertiaTensor() {
   Init();
}
void MoleculeInertiaTensor::Init() {
   data.resize(3);
   evec.resize(3);
   for ( int i=0 ; i<3 ; ++i ) {
      data[i].resize(3);
      evec[i].resize(3);
   }
   for ( int i=0 ; i<3 ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) { data[i][j]=0.0e0; }
   }
   eval.resize(3);
   initCentOfMass.resize(3);
   for ( int i=0 ; i<3 ; ++i ) { initCentOfMass[i]=0.0e0; }
   totalMass=0.0e0;
}
void MoleculeInertiaTensor::Setup() {
   ComputeTotalMass();
   CenterMolecule();
   ComputeInertiaTensor();
   Diagonalize();
   normFact=ComputeNormFactor();
   ComputeNormalData();
}
void MoleculeInertiaTensor::ComputeTotalMass() {
   totalMass=0.0e0;
   int nn=molecule->Size();
   for ( int i=0 ; i<nn ; ++i ) { totalMass+=molecule->atom[i].weight; }
}
void MoleculeInertiaTensor::CenterMolecule() {
   for ( int i=0 ; i<3 ; ++i ) { initCentOfMass[i]=0.0e0; }
   int nn=molecule->Size();
   for ( int i=0 ; i<nn ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) {
         initCentOfMass[j]+=((molecule->atom[i].weight)*(molecule->atom[i].x[j]));
      }
   }
   for ( int i=0 ; i<3 ; ++i ) { initCentOfMass[i]/=totalMass; }
   for ( int i=0 ; i<nn ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) {
         (molecule->atom[i].x[j])-=(initCentOfMass[j]);
      }
   }
}
void MoleculeInertiaTensor::ComputeInertiaTensor() {
   for ( int i=0 ; i<3 ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) { data[i][j]=0.0e0; }
   }
   int nn=molecule->Size();
   double m,x,y,z;
   for ( int i=0 ; i<nn ; ++i ) {
      m=molecule->atom[i].weight;
      x=molecule->atom[i].x[0];
      y=molecule->atom[i].x[1];
      z=molecule->atom[i].x[2];
      data[0][0]+=(m*(y*y+z*z));
      data[1][1]+=(m*(x*x+z*z));
      data[2][2]+=(m*(x*x+y*y));
      data[0][1]-=(m*x*y);
      data[0][2]-=(m*x*z);
      data[1][2]-=(m*y*z);
   }
   data[1][0]=data[0][1];
   data[2][0]=data[0][2];
   data[2][1]=data[1][2];
}
void MoleculeInertiaTensor::Diagonalize() {
#if 1
   EigenDecompositionJAMA::EigenDecomposition3(data,evec,eval);
#else
   NRJacobi::Jacobi(data,eval,evec);
   vector<double> tve(3);
   double tmp;
   if ( eval[1]<eval[0] ) {
      tmp=eval[0]; eval[0]=eval[1]; eval[1]=tmp;
      tve=evec[0]; evec[0]=evec[1]; evec[1]=tve;
   }
   if ( eval[2]<eval[1] ) {
      tmp=eval[1]; eval[1]=eval[2]; eval[2]=tmp;
      tve=evec[1]; evec[1]=evec[2]; evec[2]=tve;
   }
   if ( eval[1]<eval[0] ) {
      tmp=eval[0]; eval[0]=eval[1]; eval[1]=tmp;
      tve=evec[0]; evec[0]=evec[1]; evec[1]=tve;
   }
#endif
   /*
   for ( int i=0 ; i<3 ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) {
         if ( fabs(evec[i][j])<=EPSVECCOMPONENT ) { evec[i][j]=0.0e0; }
      }
   }
   // */
   /*
   double det=MatrixVectorOperations3D::Determinant(evec);
   if ( det<0.0e0 ) {
      ScreenUtils::DisplayWarningMessage("Mirror! Correcting now");
      cout << __FILE__ << ", line: " << __LINE__ << '\n';
      for ( size_t i=0 ; i<3 ; ++i ) {
         evec[2][i]=-evec[2][i];
      }
   }
   // */
   double minval=1.0e+50;
   for ( int i=0 ; i<3 ; ++i ) {
      if ( eval[i]<minval ) { minval=eval[i]; }
   }
   if ( minval<0.0e0 ) { ScreenUtils::DisplayWarningMessage("Found negative moment of inertia!"); }
}
double MoleculeInertiaTensor::ComputeNormFactor() {
   double maxval=-1.0e+50;
   for ( int i=0 ; i<3 ; ++i ) {
         if ( fabs(eval[i])>maxval ) { maxval=fabs(eval[i]); }
   }
   return 1.0e0/maxval;
}
double MoleculeInertiaTensor::TotalMass() {
   if ( totalMass==0.0e0 ) { ComputeTotalMass(); }
   return totalMass;
}
void MoleculeInertiaTensor::ComputeNormalData() {
   normaldata=data;
   for ( int i=0 ; i<3 ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) { normaldata[i][j]*=normFact; }
   }
}
double& MoleculeInertiaTensor::operator()(int row, int col) {
#if DEBUG
    assert( col >=0 && col<3 );
    assert( row >=0 && row<3 );
#endif
    return data[row][col];
}

const double& MoleculeInertiaTensor::operator()(int row, int col) const {
#if DEBUG
    assert( col>=0 && col<3 );
    assert( row>=0 && row<3 );
#endif
    return data[row][col];
}
void MoleculeInertiaTensor::DisplayEigenvalues() {
   cout << "Eigenvalues (normalized): " << endl;
   cout << scientific << setprecision(10);
   for ( int i=0 ; i<3 ; ++i ) { cout << eval[i] << " (" << (eval[i]*normFact) << ")" << endl; }
}
void MoleculeInertiaTensor::DisplayEigenvectors() {
   ScreenUtils::PrintScrStarLine();
   cout << "Eigenvectors:" << endl;
   ScreenUtils::PrintScrStarLine();
   for ( int i=0 ; i<3 ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) { cout << evec[i][j] << " "; }
      cout << endl;
      if ( i<3 ) { ScreenUtils::PrintScrCharLine('-'); }
   }
   ScreenUtils::PrintScrStarLine();
}
void MoleculeInertiaTensor::DisplayNormalI() {
   ScreenUtils::PrintScrStarLine();
   cout << "Normalized components:" << endl;
   ScreenUtils::PrintScrStarLine();
   for ( int i=0 ; i<3 ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) { cout << normaldata[i][j] << " "; }
      cout << endl;
      if ( i<3 ) { ScreenUtils::PrintScrCharLine('-'); }
   }
   ScreenUtils::PrintScrStarLine();
}

#endif  /* _MOLECULEINERTIATENSOR_CPP_ */

