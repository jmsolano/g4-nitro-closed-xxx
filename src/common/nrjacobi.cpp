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
#include "nrjacobi.h"
#include <cmath>
static inline void NRJacobi_Rotate(vector<vector<double> > &a, int i, int j, int k, int l, double tau, double s) {
   double g,h;
   g       = a[i][j];
   h       = a[k][l];
   a[i][j] = g - s * (h + g * tau);
   a[k][l] = h + s * (g - h * tau);
}

void NRJacobi::Jacobi(vector<vector<double > > &a,vector<double> &eval,\
      vector<vector<double> > &evec,int &nrot) {
   int    j, i;
   int    iq, ip;
   double tresh, theta, tau, t, sm, s, h, g, c;

   int n=int(a.size());
   if ( n!=int(a[0].size()) ) {
      cout << "Error: imput matrix is not a square matrix!" << '\n';
      cout << __FILE__ << ", line: " << __LINE__ << '\n';
   }
   if ( int(eval.size())!=n ) { eval.resize(n); }

   vector<double> b(n);
   vector<double> z(n);
   for (ip = 0; ip < n; ip++) {
      for (iq = 0; iq < n; iq++) {
         evec[ip][iq] = 0.0;
      }
      evec[ip][ip] = 1.0;
   }
   for (ip = 0; ip < n; ip++) {
      b[ip] = eval[ip] = a[ip][ip];
      z[ip] = 0.0;
   }
   nrot = 0;
   for (i = 1; i <= 50; i++) {
      sm = 0.0;
      for (ip = 0; ip < n-1; ip++) {
         for (iq = ip+1; iq < n; iq++) {
            sm += std::abs(a[ip][iq]);
         }
      }
      if (sm == 0.0) {
         z.clear();
         b.clear();
         return;
      }
      if (i < 4) {
         tresh = 0.2*sm/(n*n);
      }
      else {
         tresh = 0.0;
      }
      for (ip = 0; ip < n-1; ip++) {
         for (iq = ip+1; iq < n; iq++) {
            g = 100.0*std::abs(a[ip][iq]);
            if (i > 4 && std::abs(eval[ip])+g == std::abs(eval[ip])
                  && std::abs(eval[iq])+g == std::abs(eval[iq])) {
               a[ip][iq] = 0.0;
            }
            else if (std::abs(a[ip][iq]) > tresh) {
               h = eval[iq]-eval[ip];
               if (std::abs(h)+g == std::abs(h)) {
                  t = (a[ip][iq])/h;
               }
               else {
                  theta = 0.5*h/(a[ip][iq]);
                  t     = 1.0/(std::abs(theta)+std::sqrt(1.0+theta*theta));
                  if (theta < 0.0) {
                     t = -t;
                  }
               }
               c         = 1.0/std::sqrt(1+t*t);
               s         = t*c;
               tau       = s/(1.0+c);
               h         = t*a[ip][iq];
               z[ip]    -= h;
               z[iq]    += h;
               eval[ip]    -= h;
               eval[iq]    += h;
               a[ip][iq] = 0.0;
               for (j = 0; j < ip; j++) {
                  NRJacobi_Rotate(a, j, ip, j, iq, tau, s);
               }
               for (j = ip+1; j < iq; j++) {
                  NRJacobi_Rotate(a, ip, j, j, iq, tau, s);
               }
               for (j = iq+1; j < n; j++) {
                  NRJacobi_Rotate(a, ip, j, iq, j, tau, s);
               }
               for (j = 0; j < n; j++) {
                  NRJacobi_Rotate(evec, j, ip, j, iq, tau, s);
               }
               ++nrot;
            }
         }
      }
      for (ip = 0; ip < n; ip++) {
         b[ip] +=  z[ip];
         eval[ip]  =  b[ip];
         z[ip]  =  0.0;
      }
   }
   cout << "Error: Too many iterations in routine JACOBI\n" << '\n';
   cout << __FILE__ << ", line: " << __LINE__ << '\n';
}
void NRJacobi::Jacobi(vector<vector<double > > &a,vector<double> &eval,\
      vector<vector<double> > &evec) {
   int nr;
   Jacobi(a,eval,evec,nr);
   double tmp;
   int n=int(a.size());
   for ( int i=0 ; i<n ; ++i ) {
      for ( int j=0 ; j<n ; ++j ) {
         tmp=a[i][j];
         a[i][j]=a[j][i];
         a[j][i]=tmp;
      }
   }
}
