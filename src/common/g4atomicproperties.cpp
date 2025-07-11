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
#include <cstdlib>
#include <iostream>
using std::cout;
#include "screenutils.h"
#include "g4atomicproperties.h"

double G4AtomicProperties::GetG4StdEnthalpy0K(const int n) {
   if ( n<1 || (n>=int(deltaHf0KkCal.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   return (deltaHf0KkCal[n-1]*kCalPMole2Hartree);
}
double G4AtomicProperties::GetG4StdH298mH0(const int n) {
   if ( n<1 || (n>=int(h298KmH0KkCal.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   return (h298KmH0KkCal[n-1]*kCalPMole2Hartree);
}
double G4AtomicProperties::GetDeltaHf0KJANAF(const int n) {
   if ( n<1 || (n>=int(dHf0KexpJANAF.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range dHf0KexpJANAF!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   return (dHf0KexpJANAF[n-1]*JPMole2Hartree);
}
double G4AtomicProperties::GetDeltaHf0KATcT(const int n) {
   if ( n<1 || (n>=int(dHf0KexpATcT.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range dHf0KexpATcT!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   switch ( n ) {
      case 1 :
      case 6 :
      case 7 :
      case 8 :
      case 9 :
      case 16:
      case 17:
         return (dHf0KexpATcT[n-1]*JPMole2Hartree);
         break;
      default :
         ScreenUtils::DisplayErrorMessage("The data is not available for this atom (dHf0KexpATcT!)");
         std::cout << n << '\n';
         std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
         return 0.0e0;
         break;
   }
   return (dHf0KexpATcT[n-1]*JPMole2Hartree);
}
double G4AtomicProperties::GetDeltaHf0KTajti(const int n) {
   if ( n<1 || (n>=int(dHf0KexpJANAF.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range dHf0KexpJANAF!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   if ( n==6 ) { return 711790.0e0*JPMole2Hartree; }
   return (dHf0KexpJANAF[n-1]*JPMole2Hartree);
}
double G4AtomicProperties::GetDeltaHf298KNISTJANAF(const int n) {
   if ( n<1 || (n>=int(dHf298KExpNISTJANAF.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range dHf298KExpNISTJANAF!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   return (dHf298KExpNISTJANAF[n-1]*JPMole2Hartree);
}
double G4AtomicProperties::GetG4StdEnergy0K(const int n) {
   if ( n<1 || (n>=int(g4std0k.size())) ) {
      ScreenUtils::DisplayErrorMessage("Out of range!");
      std::cout << n << '\n';
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   return g4std0k[n-1];
}
double G4AtomicProperties::GetG4Energy(const int n,const string &mthd) {
   if ( mthd==string("std") ) {
      return GetG4StdEnergy0K(n);
   } else if ( mthd==string("mp2") ) {
      ScreenUtils::DisplayWarningMessage("GetG4MP2Energy0K needs to be implemented, returning zero.");
      std::cout << __FILE__ << ", line: " << __LINE__ << std::endl;
      return 0.0e0;
   }
   return 0.0e0;
}
vector<double> G4AtomicProperties::g4std0k = {
   -0.50142e0,    // H
   -2.90491e0,    // He
   -7.46636e0,    // Li
   -14.65765e0,   // Be
   -24.64665e0,   // B
   -37.83417e0,   // C
   -54.57367e0,   // N
   -75.04550e0,   // O
   -99.70498e0,   // F
   -128.90099e0,  // Ne
   -162.11789e0,  // Na
   -199.91204e0,  // Mg
   -242.22107e0,  // Al
   -289.23704e0,  // Si
   -341.13463e0,  // P
   -397.98018e0,  // S
   -460.01505e0,  // Cl
   -527.40045e0,  //Ar
   -599.71219e0,  // K
   -677.36261e0,  // Ca
   0.00000000e0,  // Sc
   0.00000000e0,  // Ti
   0.00000000e0,  // V
   0.00000000e0,  // Cr
   0.00000000e0,  // Mn
   0.00000000e0,  // Fe
   0.00000000e0,  // Co
   0.00000000e0,  // Ni
   0.00000000e0,  // Cu
   0.00000000e0,  // Zn
   -1924.35057e0, // Ga
   -2076.44070e0, // Ge
   -2235.31207e0, // As
   -2400.97797e0, // Se
   -2573.58537e0, // Br
   -2753.22582e0  // Kr
};
vector<double> G4AtomicProperties::deltaHf0KkCal={
   51.63e0,  // H
   0.0e0,    // He
   37.69e0,  // Li
   76.48e0,  // Be
   136.20e0, // B
   169.98e0, // C
   112.53e0, // N
   58.99e0,  // O
   18.47e0,  // F
   0.000e0,  // Ne
   25.69e0,  // Na
   34.87e0,  // Mg
   78.23e0,  // Al
   106.60e0, // Si
   75.42e0,  // P
   65.66e0,  // S
   28.59e0,  // Cl
   0.000e0   // Ar
};
vector<double> G4AtomicProperties::h298KmH0KkCal={
   1.01e0, // H
   0.00e0, // He
   1.10e0, // L
   0.46e0, // Be
   0.29e0, // B
   0.25e0, // C
   1.04e0, // N
   1.04e0, // O
   1.05e0, // F
   0.00e0, // Ne
   1.54e0, // Na
   1.19e0, // Mg
   1.08e0, // Al
   0.76e0, // Si
   1.28e0, // P
   1.05e0, // S
   1.10e0, // Cl
   0.00e0  // Ar
};
vector<double> G4AtomicProperties::dHf0KexpJANAF={
   216035.0e0, // H
   0.000000e0, // He
   157800.0e0, // Li
   317100.0e0, // Be
   569900.0e0, // B
   711185.0e0, // C
   470820.0e0, // N orig: 470820.0e0 ATcT: 470590.0e0
   246790.0e0, // O
   77400.0e0,  // F
   0.00000e0,  // Ne
   107600.0e0, // Na
   146500.0e0, // Mg
   327300.0e0, // Al
   452300.0e0, // Si
   315600.0e0, // P
   274735.0e0, // S
   119600.0e0, // Cl
   0.000000e0  // Ar
};
/* Last dHf0KexpATcT updated: 2025-04-25 */
vector<double> G4AtomicProperties::dHf0KexpATcT={
   216034.0e0, // H
   0.000000e0, // He
   0.000000e0, // Li
   0.000000e0, // Be
   0.000000e0, // B
   711393.0e0, // C
   470582.0e0, // N
   246844.0e0, // O
   77254.0e0,  // F
   0.00000e0,  // Ne
   0.000000e0, // Na
   0.000000e0, // Mg
   0.000000e0, // Al
   0.000000e0, // Si
   0.000000e0, // P
   276840.0e0, // S
   119621.0e0, // Cl
   0.000000e0  // Ar
};
// Below, the factor 0.5 stems from the fact that H is actually H2.
// Thus the enthalpy of formation for a single atom contribution
// is half the value found in tables.
vector<double> G4AtomicProperties::dHf298KExpNISTJANAF={
   0.5e0*8467.0e0, // H
   0.0000e0, // He
   4628.0e0, // Li
   1950.0e0, // Be
   1213.0e0, // B
   1051.0e0, // C
   0.5e0*8670.0e0, // N
   0.5e0*8683.0e0, // O
   0.5e0*8820.0e0, // F
   0.0000e0, // Ne
   6440.0e0, // Na
   5000.0e0, // Mg
   4577.0e0, // Al
   3217.0e0, // Si
   5360.0e0, // P
   4412.0e0, // S
   0.5e0*9176.0e0,  // Cl
   0.0000e0  // Ar
};


