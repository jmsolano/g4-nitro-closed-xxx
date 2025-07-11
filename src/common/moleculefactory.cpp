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
#include "screenutils.h"
#include "fileutils.h"
#include "moleculefactory.h"
#include "inputmolecule_cub.h"
#include "inputmolecule_pdb.h"
#include "inputmolecule_wfx.h"
#include "inputmolecule_xyz.h"

shared_ptr<Molecule> MoleculeFactory::OpenMolecule(const string fname) {
   if ( FileUtils::ExtensionMatches(fname,"cub") ||\
         FileUtils::ExtensionMatches(fname,"cube") ) {
      shared_ptr<InputMoleculeCub> inMol=std::make_shared<InputMoleculeCub>(fname);
      return shared_ptr<Molecule>(inMol);
   }
   if ( FileUtils::ExtensionMatches(fname,"pdb") ) {
      shared_ptr<InputMoleculePDB> inMol=std::make_shared<InputMoleculePDB>(fname);
      return shared_ptr<Molecule>(inMol);
   }
   if ( FileUtils::ExtensionMatches(fname,"wfx") ) {
      shared_ptr<InputMoleculeWFX> inMol=std::make_shared<InputMoleculeWFX>(fname);
      return shared_ptr<Molecule>(inMol);
   }
   if ( FileUtils::ExtensionMatches(fname,"xyz") ) {
      shared_ptr<InputMoleculeXYZ> inMol=std::make_shared<InputMoleculeXYZ>(fname);
      return shared_ptr<Molecule>(inMol);
   }
   ScreenUtils::DisplayErrorMessage("Something went wrong while openning molecule.");
   ScreenUtils::DisplayErrorFileNotOpen(fname);
   ScreenUtils::DisplayErrorMessage("shared_ptr set to nullptr.");
   cout << __FILE__ << ", fnc: " << __FUNCTION__ << ", line: " << __LINE__ << '\n';
   return shared_ptr<Molecule>(nullptr);
}

