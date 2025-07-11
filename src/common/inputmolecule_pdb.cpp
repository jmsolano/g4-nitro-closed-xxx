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
#include <sstream>
#include <iomanip>
#include "inputmolecule_pdb.h"
#include "screenutils.h"
#include "stringtools.h"

InputMoleculePDB::InputMoleculePDB() : Molecule() {
   buffer.clear();
   atmBuffIdx.clear();
   buffRecType.clear();
   verboseLevel=0;
   showHWarnings=true;
}
InputMoleculePDB::InputMoleculePDB(string fname) : InputMoleculePDB() {
   ReadFromFile(fname);
}
InputMoleculePDB::InputMoleculePDB(string fname,short int setVbsLvl) : 
   InputMoleculePDB() {
   verboseLevel=setVbsLvl;
   ReadFromFile(fname);
}
void InputMoleculePDB::ReadFromFile(string fname) {
   ifstream ifil(fname.c_str());
   if ( !ifil.good() ) {
      ScreenUtils::DisplayErrorMessage(string("Could not open the file \"")+fname+string("\"!"));
      ifil.close();
      imsetup=false;
      return;
   }
   ReadModel(ifil);
   ifil.close();
}
void InputMoleculePDB::DisplayProperties() {
   DisplayAtomProperties();
   DisplayBondProperties();
}
void InputMoleculePDB::Save(const string &onam) const {
   ofstream ofil(onam);
   WriteModel(ofil);
   ofil.close();
}
void InputMoleculePDB::WriteModel(ofstream &ofil) const {
   for ( size_t i=0 ; i<buffer.size() ; ++i ) {
      ofil << buffer[i] << '\n';
   }
}
std::ostream &operator<<(std::ostream &out,const InputMoleculePDB (&mol)) {
   size_t n=mol.buffer.size();
   for ( size_t i=0 ; i<n ; ++i ) {
      out << mol.buffer[i] << '\n';
   }
   return out;
}
void InputMoleculePDB::ExtractAtoms() {
   string symb;
   vector<double> x(3);
   for ( size_t i=0 ; i<buffer.size() ; ++i ) {
      //see: http://www.wwpdb.org/documentation/file-format-content/format33/sect9.html#ATOM
      if ( StringTools::StartsWith(buffer[i],"ATOM") ){
         symb=buffer[i].substr(12,2);
         StringTools::RemoveSpacesLeftAndRight(symb);
         if ( isdigit(symb[0]) ) { symb.erase(0,1); }
         if ( symb.size()==2 ) {
            symb[1]=std::tolower(symb[1]);
            if ( symb[0]=='H' ) {
               if ( (symb[1]=='e'||symb[1]=='o'||symb[1]=='g'||symb[1]=='s') ) {
                  if ( showHWarnings ) {
                     ScreenUtils::DisplayWarningMessage(string("Assuming ")+\
                           symb+string(" as Hydrogen!"));
                     if ( verboseLevel==0 ) {
                        ScreenUtils::DisplayWarningMessage("No further messages will be given about this");
                        showHWarnings=false;
                     }
                  }
               }
               symb.pop_back();
            }
         }
         //cout << '\'' << symb << '\'' << '\n';
         CopyAtomCoordinates(buffer[i],x);
         AddAtom(x,symb);
         atmBuffIdx.push_back(i);
      } else if ( StringTools::StartsWith(buffer[i],"HETATM") ) {
         symb=buffer[i].substr(12,2);
         StringTools::RemoveSpacesLeftAndRight(symb);
         if ( symb.size()==2 ) {
            symb[1]=std::tolower(symb[1]);
         }
         CopyAtomCoordinates(buffer[i],x);
         AddAtom(x,symb);
         atmBuffIdx.push_back(i);
      }
   }
}
void InputMoleculePDB::ReloadAtoms() {
   vector<double> x(3);
   size_t bPos;
   for ( size_t i=0 ; i<atom.size() ; ++i ) {
      bPos=atmBuffIdx[i];
      CopyAtomCoordinates(buffer[bPos],x);
      for ( size_t j=0 ; j<3 ; ++j ) { atom[i].x[j]=x[j]; }
   }
}
void InputMoleculePDB::CopyAtomCoordinates(const string &line,vector<double> &x) {
   x[0]=std::stod(line.substr(31,8));
   x[1]=std::stod(line.substr(39,8));
   x[2]=std::stod(line.substr(47,8));
}
void InputMoleculePDB::CopyAllAtomCoordinates() {
   for ( size_t i=0 ; i<atom.size() ; ++i ) {
      ReplaceCoordinatesInRecord(i,atom[i].x);
   }
}
void InputMoleculePDB::ReplaceCoordinatesInRecord(const size_t idx,const vector<double> &x) {
   std::stringstream ss;
   size_t bIdx=atmBuffIdx[idx];
   ss << std::fixed << std::setprecision(3);
   ss << buffer[bIdx].substr(0,30)
      << std::setw(8) << x[0] << std::setw(8) << x[1] << std::setw(8) << x[2]
      << buffer[bIdx].substr(54,string::npos) << '\n';
   getline(ss,buffer[bIdx]);
}
size_t InputMoleculePDB::ReadModel(ifstream &ifil,const size_t buffPos) {
   string line;
   if ( buffPos==string::npos ) {
      ScreenUtils::DisplayWarningMessage("No more frames to read!");
      cout << __FILE__ << ", line: " << __LINE__ << '\n';
      return string::npos;
   }
   size_t tmppos=buffPos;
   ifil.seekg(tmppos);
   if ( !imsetup ) {
      while ( !ifil.eof() ) {
         getline(ifil,line);
         if ( line.size() > 0 ) {
            buffer.push_back(line);
            buffRecType.push_back(GetRecordType(line));
         }
         if ( StringTools::StartsWith(line,"ENDMDL") ) { break; }
      }
      ExtractAtoms();
   } else {
      size_t bPos=0,nBuf=buffer.size();
      PDBRecordType rectype;
      bool consistentModel=true;
      while ( !ifil.eof() ) {
         getline(ifil,line);
         tmppos=line.size();
         rectype=GetRecordType(line);
         if ( tmppos > 0 ) {
            if ( bPos<nBuf ) {
               if ( rectype!=buffRecType[bPos] ) {
                  string msg="Record types differ! Different molecule/model? ";
                  msg+=string("Offending lines:\nold: \"");
                  msg+=buffer[bPos];
                  msg+=string("\"\nnew: \"");
                  msg+=line;
                  msg+=string("\"\n");
                  ScreenUtils::DisplayWarningMessage(msg);
                  buffRecType[bPos]=rectype;
                  consistentModel=false;
               }
               rectype=GetRecordType(line);
               if ( tmppos!=buffer[bPos].size() ) {
                  buffer[bPos].resize(tmppos);
               }
               for ( size_t j=0 ; j<tmppos ; ++j ) { buffer[bPos][j]=line[j]; }
            } else {
               ScreenUtils::DisplayWarningMessage("PDB frames (models) have different size!");
               cout << __FILE__ << ", line: " << __LINE__ << '\n';
               buffer.push_back(line);
               consistentModel=false;
            }
         }
         ++bPos;
         if ( StringTools::StartsWith(line,"ENDMDL") ) { break; }
      }
      if ( bPos!=nBuf ) { consistentModel=false; }
      if ( consistentModel ) {
         ReloadAtoms();
      } else {
         ExtractAtoms();
      }
   }
   tmppos=ifil.tellg();
   imsetup=true;
   return tmppos;
}
PDBRecordType InputMoleculePDB::GetRecordType(const string &line) {
   string tmp=line.substr(0,6);
   StringTools::RemoveSpacesRight(tmp);
   if ( tmp=="ATOM" ) { return PDBRecordType::ATOM; }
   if ( tmp=="HETATM" ) { return PDBRecordType::HETATM; }
   if ( tmp=="ANISOU" ) {return PDBRecordType::ANISOU; }
   if ( tmp=="CRYST1" ) {return PDBRecordType::CRYST1; }
   if ( tmp=="COMPND" ) {return PDBRecordType::COMPND; }
   if ( tmp=="MODEL" ) {return PDBRecordType::MODEL; }
   if ( tmp=="ENDMDL" ) {return PDBRecordType::ENDMDL; }
   if ( tmp=="TER" ) {return PDBRecordType::TER; }
   if ( tmp=="HEADER" ) {return PDBRecordType::HEADER; }
   if ( tmp=="TITLE" ) {return PDBRecordType::TITLE; }
   if ( tmp=="REMARK" ) {return PDBRecordType::REMARK; }
   if ( tmp=="CONECT" ) {return PDBRecordType::CONECT; }
   return PDBRecordType::UNDEF;
}


