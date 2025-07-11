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
#include <memory>
using std::shared_ptr;
#include <iomanip>
using std::scientific;
using std::setprecision;
#include <string>
using std::string;
#include <cmath>
#include "optflags.h"
#include "screenutils.h"
#include "mytimer.h"
#include "fileutils.h"
#include "molecule.h"
#include "moleculefactory.h"
#include "moleculeinertiatensor.h"
#include "moleculegeometricoperations.h"
#include "commonhelpers.h"
#include "helpersmoleculeinfo.h"

int main (int argc, char *argv[]) {
   /* ************************************************************************** */
   MyTimer timer;
   timer.Start();
   /* ************************************************************************** */
   /* Configures the program (options, variables, etc.)  */
   shared_ptr<OptionFlags> options = shared_ptr<OptionFlags>(new OptionFlags(argc,argv));
   switch ( options->exitCode() ) {
      case OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR :
         std::exit(EXIT_SUCCESS);
         break;
      case OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITERR :
         std::exit(EXIT_FAILURE);
         break;
      default :
         break;
   }
   if ( options->verbose ) {
      ScreenUtils::PrintHappyStart(argv,CURRENTVERSION,PROGRAMCONTRIBUTORS);
   }

   /* Main corpus  */
   cout << scientific << setprecision(10);
   string fname=string(argv[1]);
   /* Setups the pointers and objects (molecules and inertiaTensors)  */
   shared_ptr<Molecule> mol=MoleculeFactory::OpenMolecule(fname);

   /* Checks that molecules are correctly loaded.  */
   if ( (mol.use_count()==0) || (!mol->ImSetup()) ) {
      return EXIT_FAILURE;
   }
   int verbLev=0;
   if ( options->verbose ) { verbLev=1; }

   HelpersMoleculeInfo hlp(verbLev);

   if ( options->verbose ) { cout << *mol << endl; }
   if ( options->neighboursofatom ) { 
      size_t idx=size_t(std::stoi(string(argv[options->neighboursofatom])));
      hlp.DisplayIndicesOfNeighbourAtoms(mol,idx);
   }
   if ( options->nofneighbours ) {
      size_t idx=size_t(std::stoi(string(argv[options->nofneighbours])));
      cout << hlp.NumberOfNeighbours(mol,idx) << '\n';
   }
   if ( options->checklinearity ) {
      cout << (hlp.CheckIfMoleculeIsLinear(mol) ? 'y' : 'n') << '\n';
   }

   /* All OK  */
   if ( options->verbose ) { ScreenUtils::PrintHappyEnding(); }
   timer.End();
   if ( options->verbose ) { timer.PrintElapsedTimeSec(string("global timer"));}
   /* ************************************************************************** */
   return EXIT_SUCCESS;
}

