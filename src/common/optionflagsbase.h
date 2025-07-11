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
#ifndef _OPTIONFLAGSBASE_H
#define _OPTIONFLAGSBASE_H
#include <string>
using std::string;

class OptionFlagsBase {
   /* ************************************************************************** */
public: 
   enum class ExitCode {
      OFEC_CONTINUE, OFEC_EXITNOERR, OFEC_EXITERR
   };
   OptionFlagsBase();
   virtual bool BaseProcessOptions(int &argc,char** &argv);
   virtual bool ProcessOptions(int &argc,char** &argv) {return true;}
   virtual void PrintHelpMenu(int &argc,char** &argv);
   void BaseProcessDoubleDashOption(int &argc,char** &argv,int &pos);
   virtual void ProcessDoubleDashOption(int &argc,char** &argv,int &pos) {}
   static string GetRawProgramName(char** &argv);
   static string GetSmileyProgramName(char** &argv);
   void SetMinNumberOfArguments(int nn) {minArgs=nn;}
   void PrintFigletName(void);
   OptionFlagsBase::ExitCode GetExitCode() {return exitcode;}
   /* ************************************************************************** */
   unsigned short int inFileName,outFileName;
   unsigned short int verboseLevel;
   /* ************************************************************************** */
protected:
   void BasePrintErrorMessage(char** &argv,char lab);
   bool haveFigletName;
   string figletName;
   int minArgs;
   OptionFlagsBase::ExitCode exitcode;
   string rawprogramname,smileyprogramname;
};
   /* ************************************************************************** */

#endif //_OPTIONFLAGSBASE_H

