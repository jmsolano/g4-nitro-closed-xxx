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
#include <memory>
using std::shared_ptr;
#include <iomanip>
using std::scientific;
using std::setprecision;
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include "optflags.h"
#include "screenutils.h"
#include "stringtools.h"
#include "mytimer.h"
#include "rawg4sdata.h"
#include "calculateg4.h"

int main (int argc, char *argv[]) {
   /* ************************************************************************** */
   MyTimer timer;
   timer.Start();
   /* ************************************************************************** */
   /* Configures the program (options, variables, etc.)  */
   shared_ptr<OptionFlags> options = shared_ptr<OptionFlags>(new OptionFlags(argc,argv));
   switch ( options->GetExitCode() ) {
      case OptionFlagsBase::ExitCode::OFEC_EXITNOERR :
         std::exit(EXIT_SUCCESS);
         break;
      case OptionFlagsBase::ExitCode::OFEC_EXITERR :
         std::exit(EXIT_FAILURE);
         break;
      default :
         break;
   }
   int verboseLevel=0;
   if ( options->verboseLevel ) {
      verboseLevel=std::stoi(string(argv[options->verboseLevel]));
   }
   if ( verboseLevel!=0 ) {
      ScreenUtils::PrintHappyStart(argv,CURRENTVERSION,PROGRAMCONTRIBUTORS);
   }
   /* Main corpus  */
   string repname=argv[1];
   if ( verboseLevel>0 ) {
      cout << "Working file: " << repname << '\n';
   }
   RawG4sData data(repname);
   if ( !(data.ImSetup()) ) { return EXIT_FAILURE; }
   cout << setprecision(10);
   if ( verboseLevel>0 ) { ScreenUtils::PrintScrStarLine(); }
   CalculateG4 cg(data,verboseLevel);
   if ( options->setscalefreqfact ) {
      cg.SetScalingFreqencyFactor(std::stod(string(argv[options->setscalefreqfact])));
   }
   if ( options->setconsta ) {
      cg.SetConstantA(std::stod(string(argv[options->setconsta])));
   }
   if ( options->setalpha ) {
      cg.SetAlpha(std::stod(string(argv[options->setalpha])));
   }
   if ( options->setconstm ) {
      cg.SetConstantM(std::stod(string(argv[options->setconstm])));
   }
   if ( options->sett1diag ) {
      cg.SetT1Diag(std::stod(string(argv[options->sett1diag])));
   }
   if ( options->setconstg ) {
      cg.SetConstantG(std::stod(string(argv[options->setconstg])));
   }
   if ( options->setlhgap ) {
      cg.SetLHGap(std::stod(string(argv[options->setlhgap])));
   }
   if ( options->setscalefreqfact || options->setconsta || options->setalpha ||\
        options->setconstm || options->sett1diag || options->setconstg ||\
        options->setlhgap ) {
      cg.Compute();
   }
   
   if ( verboseLevel>0 ) { ScreenUtils::PrintScrStarLine(); }
   cout << "lclZPE= " << cg.LclZPE() << '\n';
   cout << "scaledZPE= " << cg.ScaledZPE() << '\n';
   cout << "G4-Nitro-Closed-XXX(0 K)= " << cg.G4Energy() << '\n';
   cout << "G4-Nitro-Closed-XXX Enthalpy= " << cg.G4Enthalpy0K() << '\n';
   if ( verboseLevel>0 ) { ScreenUtils::PrintScrStarLine(); }
   cout << setprecision(5);
   cout << "Delta_H_f(298.15_K) [HO-RR]/NIST:       " << cg.DeltaHf298KAtomization() << " kJ mol-1\n";

   cg.UseNicolaidesCorrection(false);
   cg.UseTajtiCorrection(true);
   cg.UseArgonneData(false);
   cg.Compute();
   cout << "Delta_H_f(298.15_K) [HO-RR]/Tajti:      " << cg.DeltaHf298KAtomization() << " kJ mol-1\n";

   cg.UseNicolaidesCorrection(false);
   cg.UseTajtiCorrection(false);
   cg.UseArgonneData(true);
   cg.Compute();
   cout << "Delta_H_f(298.15_K) [HO-RR]/ATcT:       " << cg.DeltaHf298KAtomization() << " kJ mol-1\n";

   cg.UseNicolaidesCorrection(true);
   cg.UseTajtiCorrection(false);
   cg.UseArgonneData(false);
   cg.Compute();
   cout << "Delta_H_f(298.15_K) [Nicolaides]/NIST:  " << cg.DeltaHf298KAtomization() << " kJ mol-1\n";

   cg.UseNicolaidesCorrection(true);
   cg.UseTajtiCorrection(true);
   cg.UseArgonneData(false);
   cg.Compute();
   cout << "Delta_H_f(298.15_K) [Nicolaides]/Tajti: " << cg.DeltaHf298KAtomization() << " kJ mol-1\n";

   cg.UseNicolaidesCorrection(true);
   cg.UseTajtiCorrection(false);
   cg.UseArgonneData(true);
   cg.Compute();
   cout << "Delta_H_f(298.15_K) [Nicolaides]/ATcT:  " << cg.DeltaHf298KAtomization() << " kJ mol-1\n";

   //data.DisplayResults();
   //string entName=repname;
   //entName.erase(entName.size()-10,entName.size()-1);
   //entName+=".ent";
   //cout << entName << '\n';
   //Helpers::GenerateInputEnthalpyCalculation(entName,data,cg);
   /* All OK  */
   if ( verboseLevel!=0 ) {
      ScreenUtils::PrintHappyEnding();
      timer.End();
      timer.PrintElapsedTimeSec(string("global timer"));
   }
   /* ************************************************************************** */
   return EXIT_SUCCESS;
}

