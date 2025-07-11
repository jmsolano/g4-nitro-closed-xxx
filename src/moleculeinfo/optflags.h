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
/* optflags.h (OptionFlags)
   This class is the implementation class for parsing and handling command line
   options. It is based on previous implementations using optflags.h
   of previous projects.

   ------------------------

   Author: Juan Manuel Solano Altamirano
   Affiliation at the moment the template for the class OptionFlags was started:
   Centro de Investigaciones y Estudios Avanzados del 
   Instituto Politécnico Nacional, 
   Unidad Monterrey, Mexico.
   2011
   e-mail: jmsolanoalt@gmail.com
   
   Affiliation at the moment of implementation improvements:
   University of Guelph,
   Guelph, Ontario, Canada.
   May 2013

   Affiliation at the moment the last version (using optflagsbase.h)
   was designed/implemented:
   Faculty of Chemical Sciences,
   Meritorious Autonomous University of Puebla,
   Puebla, Pue. Mexico
   November 2017
   ------------------------

	 This code is free code; you can redistribute it and/or
	 modify it under the terms of the GNU General Public License
	 as published by the Free Software Foundation; either version 2
	 of the License, or (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
	 along with this program; if not, write to the Free Software 
	 Foundation, Inc., 59 Temple Place - Suite 330, 
	 Boston, MA  02111-1307, USA.

   WWW:  http://www.gnu.org/copyleft/gpl.html
	
	----------------------
*/

#ifndef _OPTSFLAGS_H
#define _OPTSFLAGS_H

#include <string>
using std::string;
#include "optflagsbase.h"

class OptionFlags : public OptionFlagsBase {
/* ************************************************************************** */
public: 
/* ************************************************************************** */
   OptionFlags();
/* ************************************************************************** */
   OptionFlags(int argc,char ** &argv);
/* ************************************************************************** */
   ~OptionFlags();
/* ************************************************************************** */
   void PrintErrorMessage(char** &argv,char lab);
/* ************************************************************************** */
   void PrintHelpMenu(int argc, char** &argv);//self-described
/* ************************************************************************** */
   void Init(int argc, char** &argv);//this function will assign the values to
/* ************************************************************************** */
   void ProcessDoubleDashOptions(int argc,char** &argv,int &pos);
/* ************************************************************************** */
   void GetProgramNames(char* argv0);
/* ************************************************************************** */
   /* insert here your personal flags.  */
   bool verbose;
   bool checklinearity;
   unsigned short int nofneighbours,neighboursofatom;
/* ************************************************************************** */
protected:
/* ************************************************************************** */
};
#endif //_OPTSFLAGS_H

