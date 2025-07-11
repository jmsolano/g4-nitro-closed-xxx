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
/** This class and derived functions are a refactor of the code
   * distributed by GROMACS under the name nrjac.{h,c}.
   * Up to 2022-06-04, the refactor consists of translating the
   * original code into a C++ version, which uses std::vectors,
   * as opposed to c-style arrays.
   * The initial translation was performed by JMSA.
*/
#ifndef NRJACOBI_H
#define NRJACOBI_H
#include <vector>
using std::vector;
/* ************************************************************************** */
class NRJacobi {
/* ************************************************************************** */
public:
   /** vector<vector<double> > omega = input matrix a[0..n-1][0..n-1] must be symmetric
    * int     n    = number of rows and columns
    * vector<double> d:  d[0]..d[n-1] are the eigenvalues of a[][]
    * vector<vector<double> > evec = evec[0..n-1][0..n-1] the eigenvectors:
    *                                    evec[i][j] is component i of vector j
    * int      nrot = number of Jacobi rotations
    */
   static void Jacobi(vector<vector<double> > &a,vector<double> &eval,\
         vector<vector<double> > &evec,int &nrot);
   static void Jacobi(vector<vector<double> > &a,vector<double> &eval,\
         vector<vector<double> > &evec);
/* ************************************************************************** */
/* ************************************************************************** */
protected:
/* ************************************************************************** */
/* ************************************************************************** */
};
/* ************************************************************************** */


int m_inv_gen(double *m, int n, double *minv);
/* Produces minv, a generalized inverse of m, both stored as linear arrays.
 * Inversion is done via diagonalization,
 * eigenvalues smaller than 1e-6 times the average diagonal element
 * are assumed to be zero.
 * For zero eigenvalues 1/eigenvalue is set to zero for the inverse matrix.
 * Returns the number of zero eigenvalues.
 */

#endif
