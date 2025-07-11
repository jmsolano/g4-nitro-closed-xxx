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
#ifndef _MOLECULEGEOMETRICOPERATIONS_H_
#define _MOLECULEGEOMETRICOPERATIONS_H_
#include <memory>
using std::shared_ptr;
#include "molecule.h"
#include "moleculeinertiatensor.h"
#include "matrixvectoroperations3d.h"

/* ************************************************************************** */
/** This class (MoleculeGeometricOperations) is basically a container of
 * functions that act upon Molecule objects. The functions should be
 * restricted to geometric operations. 
 * Recall that MoleculeInertiaTensor has an internal pointer to the
 * molecule to which it belongs. */
class MoleculeGeometricOperations {
/* ************************************************************************** */
public:
   /** As the name suggest, this function aligns the first moment
    * of inertia (the greater) along the z-axis, and the
    * second moment of inertia along the y-axis.
    * The atom coordinates of the aligned molecule will be
    * sorted if srt==true */
   static void AlignFirstAndSecondMoments(shared_ptr<Molecule> &mol,\
         shared_ptr<MoleculeInertiaTensor> &I,bool srt=true);
   /** Aligns the molecule using the main moment of inertia. Recall
    * that the jama diagonalization function orders the eigenvalues
    * in an ascending manner. The principal moments of inertia should
    * be positive, thus we are asumming that the principal
    * moment of inertia is the third of the eigenvalues.
    * (There is a message that should warn agains the odd
    * case wherein a negative eigenvalue had been found --see
    * MoleculeInertiaTensor::Diagonalize().)  */
   static void AlignFirstMomentOfInertiaToZ(shared_ptr<Molecule> &mol,\
         shared_ptr<MoleculeInertiaTensor> &I);
   /** This will rotate the molecule in such a manner that  the second
    * moment of inertia is aligned to the x-axis. Notice: this requires
    * that the molecule had already been rotated so as to the 
    * main moment of inertia is aligned to the z-axis.  */
   static void AlignSecondMomemtOfInertaiToY(shared_ptr<Molecule> &mol,\
         shared_ptr<MoleculeInertiaTensor> &I);
   /** Returns the RMSD of the coordinates of two molecules, m1 and m2.
    * Notice: This function assumes that both molecules have been
    * centered and rotated in such a manner that the two first principal
    * moments of inertia are aligned to zx.
    * The atom coordinates of the aligned molecule are assumed to
    * be sorted!*/
   static double RMSD(shared_ptr<Molecule> &m1,shared_ptr<Molecule> &m2);
   /** As the name suggests, this function rotates all atom coordinates
    * by 90 degrees around the z-axis. At the end of the rotation,
    * it also calls the SortCoordinates() method.  */
   static void Rotate90DegAroundZAndSort(shared_ptr<Molecule> &mol);
   static void Rotate90DegAroundYAndSort(shared_ptr<Molecule> &mol);
   static void Rotate180DegAroundYAndSort(shared_ptr<Molecule> &mol);
   static void Rotate90DegAroundXAndSort(shared_ptr<Molecule> &mol);
   static void RotateWithRotationMatrix(shared_ptr<Molecule> &mol,\
         vector<vector<double> > &RR);
   static void InvertCoordinates(shared_ptr<Molecule> &mol);
   static void RotateUsingEulerAngles(shared_ptr<Molecule> &mol,\
         const double alpha,const double beta,const double gamma);
   static void RotateUsingEulerAngles(shared_ptr<Molecule> &mol);
   static void TranslateCoordinates(shared_ptr<Molecule> &mol,const vector<double> &a);
   static void TranslateCoordinates(shared_ptr<Molecule> &mol);
/* ************************************************************************** */
protected:
/* ************************************************************************** */
   static MatrixVectorOperations3D mop;
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _MOLECULEGEOMETRICOPERATIONS_H_ */

