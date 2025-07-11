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
#include "stringtools.h"
#include "myparser.h"


bool MyParser::Read(string fname,string key,double &var){
   ifstream ifil(fname.c_str());
   size_t pos=GetPosAfterKeyword(ifil,key,false);
   if ( pos==string::npos ) { return false; } //The file did not contain the key!
   ifil >> var;
   ifil.close();
   return true;
}
bool MyParser::Read(string fname,string key,int &var){
   ifstream ifil(fname.c_str());
   size_t pos=GetPosAfterKeyword(ifil,key,false);
   if ( pos==string::npos ) { return false; } //The file did not contain the key!
   ifil >> var;
   ifil.close();
   return true;
}
bool MyParser::Read(string fname,string key,vector<double> &vec){
   ifstream ifil(fname.c_str());
   size_t pos=GetPosAfterKeyword(ifil,key,false);
   if ( pos==string::npos ) { return false; } //The file did not contain the key!
   int n;
   ifil >> n;
   vec.resize(n);
   for ( int i=0 ; i<n ; ++i ) { ifil >> vec[i]; }
   ifil.close();
   return true;
}
size_t MyParser::GetPosAfterKeyword(const string fname,const string key) {
   ifstream ifil(fname.c_str());
   size_t pos=GetPosAfterKeyword(ifil,key,false);
   ifil.close();
   return pos;
}
size_t MyParser::GetPosAfterKeyword(ifstream &ifil,const string key,bool rewind) {
   if ( rewind ) { ifil.seekg(0); }
   size_t pos=string::npos;
   string line;
   while ( !ifil.eof() ) {
      pos=ifil.tellg();
      std::getline(ifil,line);
      StringTools::RemoveSpacesLeftAndRight(line);
      if ( key==line ) { break; }
   }
   if ( ifil.eof() ) { pos=string::npos; }
   return pos;
}

