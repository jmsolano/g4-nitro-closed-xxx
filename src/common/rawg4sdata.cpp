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
#include <iomanip>
using std::scientific;
using std::setprecision;
#include <fstream>
using std::ifstream;
#include <cmath>
#include "screenutils.h"
#include "rawg4sdata.h"
#include "myparser.h"
#include "physicalconstants.h"

RawG4sData::RawG4sData() {
   imsetup=false;
   islinear=true;
}
RawG4sData::RawG4sData(const string &repname) : RawG4sData() {
   imsetup=Read(repname);
   if ( !imsetup ) {
      ScreenUtils::DisplayErrorMessage(string("Data could not be loaded!"
               " while loading '")+repname+string("' file."));
      cout << __FILE__ << ", line: " << __LINE__ << endl;
   }
}
bool RawG4sData::Read(const string &repname) {
   /* The order of reading strongly depends on the order of the 
    * report. The report's format is set by the script extractLabFQOTG4Info
    * (usually: ../../scripts/extractLabFQOTG4Info.sh). */
   ifstream ifil(repname.c_str());
   if ( !ifil.good() ) {
      ScreenUtils::DisplayErrorFileNotOpen(repname);
      ifil.close();
      return false;
   }
   bool ok=true;
   //From *G41*.log
   size_t pos=MyParser::GetPosAfterKeyword(ifil,string("ATOMS_IN_MOLECULE"),false);
   if ( pos!=string::npos ) { std::getline(ifil,atomsInMolecule); } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("ALPHA_ELECTRONS"),false);
   if ( pos!=string::npos ) { ifil >> nElAlpha; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("BETA_ELECTRONS"),false);
   if ( pos!=string::npos ) { ifil >> nElBeta; } else { ok=false; }
   //
   pos=MyParser::GetPosAfterKeyword(ifil,string("METHOD"),false);
   if ( pos!=string::npos ) { ifil >> method; } else { ok=false; }
   //*
   size_t bkppos=pos;
   pos=MyParser::GetPosAfterKeyword(ifil,string("IS_LINEAR"),false);
   if ( pos!=string::npos ) {
      char ttt;
      ifil >> ttt;
      islinear=(ttt=='y' || ttt=='Y');
   } else {
      ifil.clear(); ifil.seekg(bkppos);
      ScreenUtils::DisplayWarningMessage("You are using an old report format!\n"
            "Surmising the molecule is nonlinear...");
   }
   // */
   //From *G42*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("ZeroPoint"),false);
   if ( pos!=string::npos ) { ifil >> zpe; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("FREQUENCIES"),false);
   if ( pos!=string::npos ) {
      int nf;
      ifil >> nf;
      frequencies.resize(nf);
      for ( int i=0 ; i<nf ; ++i ) {
         ifil >> frequencies[i];
         if ( frequencies[i]<0.0e0 ) {
            ok=false;
            ScreenUtils::DisplayErrorMessage("Imaginary frequency found!");
            cout << __FILE__ << ", fnc: " << __FUNCTION__ << ", line: " << __LINE__ << '\n';
         }
      }
   } else { ok=false; }
   //From *G43*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP2"),false);
   if ( pos!=string::npos ) { ifil >> mp2gtbas1; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP4SDTQ"),false);
   if ( pos!=string::npos ) { ifil >> mp4gtbas1; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("CCSD(T)"),false);
   if ( pos!=string::npos ) { ifil >> ccsdtg3bas1; } else { ok=false; }
   //From *G44*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP2"),false);
   if ( pos!=string::npos ) { ifil >> mp2gtbas2; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP4SDTQ"),false);
   if ( pos!=string::npos ) { ifil >> mp4gtbas2; } else { ok=false; }
   //From *G45*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP2"),false);
   if ( pos!=string::npos ) { ifil >> mp2gtbas3; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP4SDTQ"),false);
   if ( pos!=string::npos ) { ifil >> mp4gtbas3; } else { ok=false; }
   //From *G46*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("HF"),false);
   if ( pos!=string::npos ) { ifil >> hfgtlargexp; } else { ok=false; }
   pos=MyParser::GetPosAfterKeyword(ifil,string("MP2"),false);
   if ( pos!=string::npos ) { ifil >> mp2gtlargexp; } else { ok=false; }
   //From *G47*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("HF"),false);
   if ( pos!=string::npos ) { ifil >> hfgfhfb1; } else { ok=false; }
   //From *G48*.log
   pos=MyParser::GetPosAfterKeyword(ifil,string("HF"),false);
   if ( pos!=string::npos ) { ifil >> hfgfhfb2; } else { ok=false; }
   //
   ifil.close();
   /* This needs to be read from report.  */
   if ( nElAlpha!=nElBeta ) {
      ScreenUtils::DisplayWarningMessage("Only closed shell molecules can be correctly analized!");
      ok=false;
      cout << __FILE__ << ", line: " << __LINE__ << endl;
   }
   return ok;
}
void RawG4sData::DisplayResults() {
   cout << scientific << setprecision(10);
   ScreenUtils::PrintScrStarLine();
   cout << "Atoms in molecule: '" << atomsInMolecule << "'\n";
   ScreenUtils::PrintScrCharLine('-');
   cout << "Alpha electrons: " << nElAlpha << '\n';
   cout << "Beta electrons:  " << nElBeta << '\n';
   ScreenUtils::PrintScrCharLine('-');
   cout << "Method:         " << method << '\n';
   ScreenUtils::PrintScrStarLine();
   ScreenUtils::PrintScrCharLine('-');
   cout << "zpe:          " << (zpe>0.0e0? "  " : " ") << zpe << '\n';
   cout << "Frequencies: " << '\n';
   for ( size_t i=0 ; i<frequencies.size() ; ++i ) {
      cout << frequencies[i] << '\n';
   }
   ScreenUtils::PrintScrCharLine('-');
   cout << "mp2gtbas1:    " << (mp2gtbas1>0.0e0? "  " : " ") << mp2gtbas1 << '\n';
   cout << "mp4gtbas1:    " << (mp4gtbas1>0.0e0? "  " : " ") << mp4gtbas1 << '\n';
   cout << "ccsdtg3bas1:  " << (ccsdtg3bas1>0.0e0? "  " : " ") << ccsdtg3bas1 << '\n';
   ScreenUtils::PrintScrCharLine('-');
   cout << "mp2gtbas2:    " << (mp2gtbas2>0.0e0? "  " : " ") << mp2gtbas2 << '\n';
   cout << "mp4gtbas2:    " << (mp4gtbas2>0.0e0? "  " : " ") << mp4gtbas2 << '\n';
   ScreenUtils::PrintScrCharLine('-');
   cout << "mp2gtbas3:    " << (mp2gtbas3>0.0e0? "  " : " ") << mp2gtbas3 << '\n';
   cout << "mp4gtbas3:    " << (mp4gtbas3>0.0e0? "  " : " ") << mp4gtbas3 << '\n';
   ScreenUtils::PrintScrCharLine('-');
   cout << "hfgtlargexp:  " << (hfgtlargexp>0.0e0? "  " : " ") << hfgtlargexp << '\n';
   cout << "mp2gtlargexp: " << (mp2gtlargexp>0.0e0? "  " : " ") << mp2gtlargexp << '\n';
   ScreenUtils::PrintScrCharLine('-');
   cout << "hfgfhfb1:     " << (hfgfhfb1>0.0e0? "  " : " ") << hfgfhfb1 << '\n';
   ScreenUtils::PrintScrCharLine('-');
   cout << "hfgfhfb2:     " << (hfgfhfb2>0.0e0? "  " : " ") << hfgfhfb2 << '\n';
   //ScreenUtils::PrintScrCharLine('-');
   //cout << " Energies from Statistical Thermodynamics (hartrees): " << '\n';
   //cout << "Evib (a.u.)   :" << EvibH << '\n';
   //cout << "Evib (J/mol)  :" << EvibJ << '\n';
   //cout << "Erot:          " << Erot << '\n';
   //cout << "Etrans:        " << Etrans << '\n';
   //cout << "PV:            " << PV << '\n';
   //cout << "lclZPE:        " << lclZPE << '\n';
   ScreenUtils::PrintScrStarLine();
}

