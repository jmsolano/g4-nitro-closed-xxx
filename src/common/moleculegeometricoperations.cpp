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
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <cassert>
#include "moleculegeometricoperations.h"
#include "screenutils.h"

MatrixVectorOperations3D MoleculeGeometricOperations::mop;
void MoleculeGeometricOperations::AlignFirstMomentOfInertiaToZ(shared_ptr<Molecule> &mol,\
      shared_ptr<MoleculeInertiaTensor> &I) {
   vector<double> v2=I->Eve(2),tmp;
   vector<vector<double> > m=MatrixVectorOperations3D::GetMatrixToAlignVToZ(v2);
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      tmp=mol->atom[i].x;
      mol->atom[i].x=MatrixVectorOperations3D::MatrixVectorProduct(m,tmp);
   }
}
void MoleculeGeometricOperations::AlignSecondMomemtOfInertaiToY(shared_ptr<Molecule> &mol,\
      shared_ptr<MoleculeInertiaTensor> &I) {
   vector<double> y=MatrixVectorOperations3D::Y();
   double angle=MatrixVectorOperations3D::InnerProduct(y,I->Eve(1));
   //angle/=MatrixVectorOperations3D::Norm(I->Eve(1));
   angle=-acos(angle);
   vector<double> z=MatrixVectorOperations3D::Z();
   vector<double> cross=MatrixVectorOperations3D::CrossProduct(y,I->Eve(1));
   if ( MatrixVectorOperations3D::InnerProduct(cross,z) < 0.0e0 ) { angle=-angle; }
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      MatrixVectorOperations3D::RotateAroundZAxis(mol->atom[i].x,angle);
   }
   /*
   //There is no garantee for the eigenvector to change its quirality if 
   //  the molecule is rotated pi rads around z!
   angle=(I->Eve(1))[1];
   if ( angle<0.0e0 ) {
      cout << "Rotating again... (" << angle << ")" << endl;
      for ( size_t i=0 ; i<mol->Size() ; ++i ) {
         MatrixVectorOperations3D::RotateAroundZAxis(mol->atom[i].x,M_PI);
      }
      I->Setup();
   }
   // */
}
void MoleculeGeometricOperations::AlignFirstAndSecondMoments(shared_ptr<Molecule> &mol,\
      shared_ptr<MoleculeInertiaTensor> &I,bool srt) {
   AlignFirstMomentOfInertiaToZ(mol,I);
   I->Setup();
   AlignSecondMomemtOfInertaiToY(mol,I);
   I->Setup();
   if ( srt ) { mol->SortCoordinates(); }
}
double MoleculeGeometricOperations::RMSD(shared_ptr<Molecule> &m1,shared_ptr<Molecule> &m2) {
   if ( m1->Size() != m2->Size() ) {
      ScreenUtils::DisplayErrorMessage("The molecules have different number of atoms!");
   }
   double rmsd=0.0e0,tmp;
   assert(m1->Size() == m2->Size());
   for ( size_t i=0 ; i<m1->Size() ; ++i ) {
      for ( size_t j=0 ; j<3 ; ++j ) {
         tmp=(m1->atom[i].x[j])-(m2->atom[i].x[j]);
         rmsd+=(tmp*tmp);
      }
   }
   rmsd/=double(m1->Size());
   return sqrt(rmsd);
}
void MoleculeGeometricOperations::Rotate90DegAroundZAndSort(shared_ptr<Molecule> &mol) {
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      MatrixVectorOperations3D::RotateAroundZAxis(mol->atom[i].x,M_PI_2);
   }
   mol->SortCoordinates();
   //cout << *mol << endl;
}
void MoleculeGeometricOperations::Rotate180DegAroundYAndSort(shared_ptr<Molecule> &mol) {
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      MatrixVectorOperations3D::RotateAroundYAxis(mol->atom[i].x,M_PI);
   }
   mol->SortCoordinates();
   //cout << *mol << endl;
}
void MoleculeGeometricOperations::Rotate90DegAroundYAndSort(shared_ptr<Molecule> &mol) {
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      MatrixVectorOperations3D::RotateAroundYAxis(mol->atom[i].x,M_PI_2);
   }
   mol->SortCoordinates();
   //cout << *mol << endl;
}
void MoleculeGeometricOperations::Rotate90DegAroundXAndSort(shared_ptr<Molecule> &mol) {
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      MatrixVectorOperations3D::RotateAroundXAxis(mol->atom[i].x,M_PI_2);
   }
   mol->SortCoordinates();
   //cout << *mol << endl;
}
void MoleculeGeometricOperations::InvertCoordinates(shared_ptr<Molecule> &mol) {
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      for ( size_t j=0 ; j<3 ; ++j ) { mol->atom[i].x[j]=-mol->atom[i].x[j]; }
      //mol->atom[i].x[0]=-mol->atom[i].x[0];
   }
   mol->SortCoordinates();
   //cout << *mol << endl;
}
void MoleculeGeometricOperations::RotateWithRotationMatrix(shared_ptr<Molecule> &mol,\
      vector<vector<double> > &RR) {
   vector<double> tmp(3);
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      tmp=mol->atom[i].x;
      mol->atom[i].x=MatrixVectorOperations3D::MatrixVectorProduct(RR,tmp);
   }
}
void MoleculeGeometricOperations::RotateUsingEulerAngles(shared_ptr<Molecule> &mol,\
      const double alpha,const double beta,const double gamma) {
   vector<vector<double> > R=mop.GetEulerRotationMatrix(alpha,beta,gamma);
   cout << "det: " << MatrixVectorOperations3D::Determinant(R) << endl;
   RotateWithRotationMatrix(mol,R);
}
void MoleculeGeometricOperations::RotateUsingEulerAngles(shared_ptr<Molecule> &mol) {
   vector<vector<double> > R=mop.GetEulerRotationMatrix();
   RotateWithRotationMatrix(mol,R);
}
void MoleculeGeometricOperations::TranslateCoordinates(shared_ptr<Molecule> &mol,const vector<double> &a) {
   if ( a.size()!=3 ) {
      ScreenUtils::DisplayErrorMessage("Receving a non 3D-vector!\nNothing done.");
      cout << __FILE__ << ", line: " << __LINE__ << endl;
      return;
   }
   for ( size_t i=0 ; i<mol->Size() ; ++i ) {
      for ( size_t j=0 ; j<3 ; ++j ) { mol->atom[i].x[j]+=a[j]; }
   }
}
void MoleculeGeometricOperations::TranslateCoordinates(shared_ptr<Molecule> &mol) {
   vector<double> a=mop.RandomVector();
   TranslateCoordinates(mol,a);
}

