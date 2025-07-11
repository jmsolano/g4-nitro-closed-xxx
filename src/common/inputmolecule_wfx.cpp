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
using std::endl;
using std::cerr;
#include <vector>
using std::vector;
#include <fstream>
using std::ofstream;
#include <iomanip>
#include "inputmolecule_wfx.h"
#include "screenutils.h"
#include "stringtools.h"
#include "fileutils.h"
#include "unitconversion.h"

InputMoleculeWFX::InputMoleculeWFX() : Molecule() {
}
InputMoleculeWFX::InputMoleculeWFX(string fname) : InputMoleculeWFX() {
   ReadFromFile(fname);
}
void InputMoleculeWFX::ReadFromFile(string fname) {
   if ( !FileUtils::ExtensionMatches(fname,"wfx") ) {
      ScreenUtils::DisplayErrorFileNotOpen(fname);
      imsetup=false;
      return;
   }
   ifstream ifil(fname.c_str());
   if ( !ifil.good() ) {
      ScreenUtils::DisplayErrorMessage(string("Could not open the file \"")+fname+string("\"!"));
      ifil.close();
      imsetup=false;
      return;
   }
   ReadFromFile(ifil);
   ifil.close();
   imsetup=true;
}
void InputMoleculeWFX::ReadFromFile(ifstream &ifil) {
   ReadTitle(ifil);
   int nn=ReadInteger(ifil,string("Number of Nuclei"));
   vector<int> nAt(nn);
   if ( !SetPosAfterKeyword(ifil,"<atomic numbers>") ) { return; }
   for ( int i=0 ; i<nn ; ++i ) {
      ifil >> nAt[i];
   }
   if ( !SetPosAfterKeyword(ifil,"<nuclear cartesian coordinates>") ) { return; }
   vector<double> xt(3);
   for ( int i=0 ; i<nn ; ++i ) {
      for ( int j=0 ; j<3 ; ++j ) { ifil >> xt[j]; xt[j]*=unitconv::bohr2angstrom; }
      AddAtom(xt,nAt[i]);
   }
}
int InputMoleculeWFX::ReadInteger(ifstream &ifil,const string &kwd) {
   string target=string("<")+kwd+string(">");
   StringTools::ToLower(target);
   bool fndtag=SetPosAfterKeyword(ifil,target);
   if ( !fndtag ) { return 0; }
   int tt;
   ifil >> tt;
   return tt;
}
void InputMoleculeWFX::DisplayProperties() {
   if ( !imsetup ) { return; }
   cout << title << endl;
   DisplayAtomProperties();
}
bool InputMoleculeWFX::SetPosAfterKeyword(ifstream &ifil,const string &tgttag) {
   ifil.seekg(0);
   string tag;
   std::getline(ifil,tag);
   StringTools::ToLower(tag);
   while ( tag!=tgttag && (!ifil.eof()) ) {
      std::getline(ifil,tag);
      StringTools::ToLower(tag);
   }
   if ( ifil.eof() ) {
      ScreenUtils::DisplayErrorMessage("End of file!");
      cout << __FILE__ << ", fnc: " << __FUNCTION__ << ", line: " << __LINE__ << '\n';
      return false;
   }
   return true;
}
void InputMoleculeWFX::ReadTitle(ifstream &ifil) {
   if ( !SetPosAfterKeyword(ifil,"<title>") ) { return; }
   string str,tag;
   title="";
   do {
      std::getline(ifil,str);
      tag=str;
      StringTools::ToLower(tag);
      if ( tag!=string("</title>") ) {
         if ( title.size()>0 ) { title+='\n'; }
         title+=str;
      }
   } while ( tag!=string("</title>") );
}
std::ostream &operator<<(std::ostream &out,const InputMoleculeWFX (&mol)) {
   const Molecule* pmol=&mol;
   if ( !mol.ImSetup() ) { return out; }
   out << mol.title << endl;
   out << pmol;
   return out;
}

