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
#include "optflags.h"
#include "screenutils.h"
#include <iostream>
using std::cout;
using std::endl;
using std::ios;
#include <cstdlib>
using namespace std;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;

OptionFlags::OptionFlags() : OptionFlagsBase() {
   exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_CONTINUE;
}
OptionFlags::OptionFlags(int argc,char** &argv) : OptionFlags() {
   /* Remember to initialize local short ints before calling Init()!  */
   verbose=false;
   checklinearity=false;
   neighboursofatom=nofneighbours=0;
   Init(argc,argv);
}
OptionFlags::~OptionFlags() {
   
}
void OptionFlags::Init(int argc, char** &argv) {
   GetProgramNames(argv[0]);
   if (argc>1 ) {
      if ( string(argv[1])==string("-h") ) {
         PrintHelpMenu(argc,argv);
         exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR;
         return;
      }
      if ( string(argv[1])==string("-V") ) {
         cout << rawprogramname << " version: " << CURRENTVERSION << endl;
         exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR;
         return;
      }
   }
   if (argc<2) {
      ScreenUtils::SetScrRedBoldFont();
      cout << "\nError: Not enough arguments." << endl;
      ScreenUtils::SetScrNormalFont();
      cout << "\nTry: \n\t" << argv[0] << " -h\n" << endl << "to view the help menu.\n\n";
      exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITERR;
      return;
   }
   for (int i=2; i<argc; i++){
      if (argv[i][0] == '-'){
         switch (argv[i][1]){
            case 'l' :
               checklinearity=true;
               break;
            case 'n' :
               nofneighbours=(++i);
               if (i>=argc) {PrintErrorMessage(argv,'n');}
               break;
            case 'N' :
               neighboursofatom=(++i);
               if (i>=argc) {PrintErrorMessage(argv,'N');}
               break;
            case 'o':
               outfname=(++i);
               if (i>=argc) {PrintErrorMessage(argv,'o');}
               break;
            case 'h':
               PrintHelpMenu(argc,argv);
               exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR;
               return;
               break;
            case 'v' :
               verbose=true;
               break;
            case 'V':
               cout << rawprogramname << " " << CURRENTVERSION << endl;
               exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR;
               return;
               break;
            case '-':
               ProcessDoubleDashOptions(argc,argv,i);
               break;
            default:
               cout << "\nCommand line error. Unknown switch: " << argv[i] << endl;
               cout << "\nTry: \n\t" << argv[0] << " -h\n" << endl << "to view the help menu.\n\n";
               exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITERR;
         }
      }
   }
   return;
}
void OptionFlags::PrintHelpMenu(int argc, char** &argv) {
   ScreenUtils::PrintScrStarLine();
   cout << endl;
   ScreenUtils::CenterString(smileyprogramname);
   cout << endl;
   ScreenUtils::CenterString("This program displays information of a molecule.");
   ScreenUtils::CenterString("The molecule is read from an input file; see below");
   ScreenUtils::CenterString("for the complete list of supported formats.");
   ScreenUtils::CenterString("The information to be displayed is requested by the");
   ScreenUtils::CenterString("user throught options.");
   //ScreenUtils::CenterString("");
   cout << endl;
   ScreenUtils::CenterString((string("Compilation date: ")+string(__DATE__)));
   cout << endl;
   ScreenUtils::CenterString(string("Version: ")+string(CURRENTVERSION));
   cout << endl;
   ScreenUtils::CenterString((string(":-) Created by: ")+string(PROGRAMCONTRIBUTORS)+string(" (-:")));
   cout << endl;
   ScreenUtils::PrintScrStarLine();
   ScreenUtils::SetScrBoldFont();
   cout << "\nUsage:\n\n\t" << rawprogramname
        << " inputmolecule.xxx [option [value(s)]] ... [option [value(s)]]\n\n";
   ScreenUtils::SetScrNormalFont();
   cout << "The molecule information can be read from the following file-formats:\n"
        << "             \txyz, cub, pdb, and wfx." << '\n';
   cout << "Here options can be (assuming that the molecule has N atoms):\n\n";
   cout << "  -l         \tCheck if the molecule is linear." << '\n';
   cout << "  -n k       \tDisplay the number of neighbours of atom k.\n"
        << "             \t  k is the index of the atom as given in the input\n"
        << "             \t  file (from 1 to N)" << '\n';
   cout << "  -N k       \tDisplay the list of neighbours of atom k. The displayed numbers\n"
        << "             \t  are the indices of the atoms as given in the input file\n"
        << "             \t  (from 1 to N)." << '\n';
   cout << "  -o outfname\tSet the output file name to be outfname." << endl;
   cout << "  -v         \tPrint information other than the pure result." << endl;
   cout << "  -V         \tDisplay the version of this program." << endl;
   cout << "  -h         \tDisplay the help menu.\n\n";
   //-------------------------------------------------------------------------------------
   cout << "  --help    \t\tSame as -h" << endl;
   cout << "  --version \t\tSame as -V" << endl;
   cout << endl;
   ScreenUtils::PrintScrStarLine();
   //-------------------------------------------------------------------------------------
}
void OptionFlags::PrintErrorMessage(char** &argv,char lab) {
   ScreenUtils::SetScrRedBoldFont();
   cout << "\nError: the option \"-" << lab << "\" ";
   switch (lab) {
      case 'n' :
      case 'N' :
         cout << "should be followed by an integer." << '\n';
         break;
      case 'o':
         cout << "should be followed by a name." << endl;
         break;
      default:
         cout << "is triggering an unknown error." << endl;
         break;
   }
   ScreenUtils::SetScrNormalFont();
   cout << "\nTry:\n\t" << argv[0] << " -h " << endl;
   cout << "\nto view the help menu.\n\n";
   exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITERR;
   return;
}
void OptionFlags::ProcessDoubleDashOptions(int argc,char** &argv,int &pos) {
   string str=argv[pos];
   str.erase(0,2);
   if (str==string("version")) {
      cout << rawprogramname << " " << CURRENTVERSION << endl;
      exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR;
   } else if (str==string("help")) {
      PrintHelpMenu(argc,argv);
      exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITNOERR;
   } else {
      ScreenUtils::SetScrRedBoldFont();
      cout << "Error: Unrecognized option '" << argv[pos] << "'" << endl;
      ScreenUtils::SetScrNormalFont();
      exitcode=OptionFlagsBase::OptFlgsExitCodes::OFEC_EXITERR;
   }
   return;
}
void OptionFlags::GetProgramNames(char* argv0) {
   string progname=argv0;
   size_t pos=progname.find("./");
   if (pos!=string::npos) {progname.erase(pos,2);}
   rawprogramname=progname;
   smileyprogramname=":-)  ";
   smileyprogramname+=rawprogramname;
   smileyprogramname+="  (-:";

}


