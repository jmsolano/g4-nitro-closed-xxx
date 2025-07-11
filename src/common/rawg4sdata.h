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
#ifndef _RAWG4SDATA_H_
#define _RAWG4SDATA_H_
#include <vector>
using std::vector;
#include <string>
using std::string;

/* ************************************************************************** */
class RawG4sData {
/* ************************************************************************** */
public:
/* ************************************************************************** */
   RawG4sData();
   RawG4sData(const string &repname);
/* ************************************************************************** */
   bool Read(const string &repname);
   bool ImSetup() {return imsetup;}
/* ************************************************************************** */
   string method;
   bool islinear;
   double zpe; //From step 2 (name: *G42*.log)
   double mp2gtbas1,mp4gtbas1,ccsdtg3bas1; //From step 3 (name: *G43*.log)
   double mp2gtbas2,mp4gtbas2; //From step 4 (name: *G44*.log)
   double mp2gtbas3,mp4gtbas3; //From step 5 (name: *G45*.log)
   double hfgtlargexp,mp2gtlargexp; //From step 6 (name: *G46*.log)
   double hfgfhfb1; //From step 7 (name: *G47*.log)
   double hfgfhfb2; //From step 8 (name: *G48*.log)
   int nElAlpha;
   int nElBeta;
   string atomsInMolecule;
   vector<double> frequencies;
/* ************************************************************************** */
   void DisplayResults();
/* ************************************************************************** */
protected:
/* ************************************************************************** */
   bool imsetup;
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _RAWG4SDATA_H_ */

