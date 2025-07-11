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
#ifndef _MYPARSER_H_
#define _MYPARSER_H_
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <vector>
using std::vector;

/* ************************************************************************** */
/** This is a helper class that reads from a file. The names
 * usually suggest the kind of data that is read. The basic structure
 * of a parsed file can be found in '../test/file2parse.in'.
 * The file has keywords followed by numbers/values/strings.
 *
 * Each function of this class returns a boolean, which tells
 * whether or not the KEYWORD was found. Also, in each function,
 * the first variable must be a string that contains the 
 * filename wherein the data will be extracted.
 * The second parameter is a string that contains the keyword
 * to be searched. The third parameter is a reference to the variable type
 * that will be extracted from the input file.
 * Usually, the documentatio of each function will tell details about
 * the third parameter.
 */
class MyParser {
/* ************************************************************************** */
public:
/* ************************************************************************** */
   static bool Read(const string fname,string key,double &var);
   static bool Read(const string fname,const char* key,double &var) {return Read(fname,string(key),var);}
   static bool Read(const string fname,string key,int &var);
   static bool Read(const string fname,const char* key,int &var) {return Read(fname,string(key),var);}
   static bool Read(const string fname,const string key,vector<double> &vec);
   static bool Read(const string fname,const char* key,vector<double> &vec) {return Read(fname,string(key),vec);}
   static bool Read(const string fname,string key,string &str);
   static bool Read(const string fname,const char* key,string &str) {return Read(fname,string(key),str);}
/* ************************************************************************** */
   static size_t GetPosAfterKeyword(ifstream &ifil,const string key,bool rewind=false);
   static size_t GetPosAfterKeyword(const string fname,const string key);
protected:
/* ************************************************************************** */
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _MYPARSER_H_ */

