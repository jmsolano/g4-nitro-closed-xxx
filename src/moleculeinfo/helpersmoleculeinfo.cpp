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
#include <vector>
using std::vector;
#include <cmath>
#include "screenutils.h"
#include "helpersmoleculeinfo.h"
#include "moleculeinertiatensor.h"
HelpersMoleculeInfo::HelpersMoleculeInfo() {
   verboseLevel=0;
}
void HelpersMoleculeInfo::DisplayIndicesOfNeighbourAtoms(shared_ptr<Molecule> mol,size_t idx) {
   if ( idx<1 || idx>(mol->Size()) ) {
      ScreenUtils::DisplayErrorMessage("Not a valid atom!");
      cout << __FILE__ << ", fnc: " << __FUNCTION__ << ", line: " << __LINE__ << '\n';
      return;
   }
   vector<size_t> n=mol->ListOfNeighbours(idx-1);
   for ( size_t i=1 ; i<n.size() ; ++i ) {
      cout << (n[i]+1) << (i<(n.size()-1)? ' ' : '\n');
   }
}
size_t HelpersMoleculeInfo::NumberOfNeighbours(shared_ptr<Molecule> mol,size_t idx) {
   if ( idx<1 || idx>(mol->Size()) ) {
      ScreenUtils::DisplayErrorMessage("Not a valid atom!");
      cout << __FILE__ << ", fnc: " << __FUNCTION__ << ", line: " << __LINE__ << '\n';
      return 0;
   }
   vector<size_t> n=mol->ListOfNeighbours(idx-1);
   if ( n.size()<1 ) {
      ScreenUtils::DisplayErrorMessage("Unknown error!");
      cout << __FILE__ << ", fnc: " << __FUNCTION__ << ", line: " << __LINE__ << '\n';
      return 0;
   }
   return (n.size()-1);
}
bool HelpersMoleculeInfo::CheckIfMoleculeIsLinear(shared_ptr<Molecule> mol) {
   shared_ptr<MoleculeInertiaTensor> I=shared_ptr<MoleculeInertiaTensor>(new MoleculeInertiaTensor(mol));
   bool res=false;
   if ( (fabs((I->Eva(0))*(I->NormFactEva())) <=1.0e-06) &&
            (fabs((I->NormFactEva())*((I->Eva(1))-(I->Eva(2)))) < 1.0e-04) ) {
      res=true;
   }
   if ( verboseLevel>0 ) {
      cout << fabs((I->Eva(0))*(I->NormFactEva())) << '\n';
      cout << fabs((I->NormFactEva())*((I->Eva(1))-(I->Eva(2)))) << '\n';
      I->DisplayEigenvalues();
   }
   return res;
}

