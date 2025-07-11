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
#ifndef _G4ATOMICPROPERTIES_H_
#define _G4ATOMICPROPERTIES_H_
#include <vector>
using std::vector;
#include <string>
using std::string;

/* ************************************************************************** */
class G4AtomicProperties {
/* ************************************************************************** */
public:
/* ************************************************************************** */
   static double GetG4Energy(const int n,const string &mthd); // in hartrees
   static double GetG4StdEnergy0K(const int n); // in hartrees
   static double GetG4StdEnthalpy0K(const int n); // in hartrees
   static double GetG4StdH298mH0(const int n); // in hartrees
   static double GetDeltaHf0KJANAF(const int n); // in hartrees
   static double GetDeltaHf0KTajti(const int n); // in hartrees
   static double GetDeltaHf298KNISTJANAF(const int n); // in hartrees
   static double GetDeltaHf0KATcT(const int n); // in hartrees
   static constexpr double kCalPMole2Hartree=1.0e0/627.5e0;
   static constexpr double JPMole2Hartree=1.0e0/2.6255e+06;
   static constexpr double hartree2kJPerMole=2.6255e+03;
   static constexpr double hartree2JPerMole=2.6255e+06;
   static constexpr double hartree2kCalPerMole=6.27503e+02;
/* ************************************************************************** */
protected:
/* ************************************************************************** */
   /** This are G4 energies at 0 K, for the ground state.
    * Extracted from Curtiss, Redfern, and Raghavachari, J. Chem. Phys,
    *   126 (2007) 084108, Table 1.  */
   static vector<double> g4std0k;
   /** Atomic enthalpies of formation at 0 K for atoms.
    * Extracted from Curtiss, Raghavachari, Redfern, and Pople,
    * J. Chem. Phys., 103 (1997) 1063, Table 1.  */
   static vector<double> deltaHf0KkCal;
   static vector<double> h298KmH0KkCal;
   /** Experimental \f$\Delta H_f^{\circ}(0K)\f$, extracted from
    * https://atct.anl.gov/Thermochemical%20Data/version%201.122p/index.php */
   static vector<double> dHf0KexpATcT; // in J/mol
   /** Experimental \f$\Delta H_f^{\circ}(0K)\f$, extracted from
    * M. W. Chase, Jr., C. A. Davies, J. R. Downey, Jr., D. J. Frurip, R. A.
    * McDonald, and A. N. Syverud, J. Phys. Chern. Ref. Data 14, Suppl. I
    * (1985).*/
   static vector<double> dHf0KexpJANAF; // in J/mol
   /* dHf298KExpNISTJANAF has the
   //experimental Delta H  of formation at 298 K (J mol-1), as
   //taken from [A] J. Phys. Chem. 100 17460 (1996)
   //or [B] NIST-JANAF Thermochemical Tables J. Phys. Chem.
   //Data Monograph 9, 1998, 1-1951 */
   static vector<double> dHf298KExpNISTJANAF; //in J/mol
   /* dHf298KExpNISTJANAF has the
   //experimental Delta H  of formation at 298 K (J mol-1),
   //relative to the Delta H of formation at 0 K, as
   //taken from 
   //https://atct.anl.gov/Thermochemical%20Data/version%201.122p/index.php  */
   static vector<double> dHf298KExpATcT; //in J/mol
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _G4ATOMICPROPERTIES_H_ */

