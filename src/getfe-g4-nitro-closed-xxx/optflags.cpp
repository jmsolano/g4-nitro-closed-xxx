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
   minArgs=2; //Change if needed
   setscalefreqfact=0;
   setconsta=0;
   setalpha=0;
   setconstm=0;
   sett1diag=0;
   setconstg=0;
   setlhgap=0;
}
OptionFlags::OptionFlags(int &argc,char** &argv) : OptionFlags() {
   /* Remember to initialize local short ints before calling Init()!  */
   Init(argc,argv);
}
OptionFlags::~OptionFlags() {
   
}
void OptionFlags::Init(int &argc, char** &argv) {
   if (!BaseProcessOptions(argc,argv)) {return;}
   GetProgramNames(argv[0]);
   if (argc>1 && string(argv[1])==string("-h")) {
      PrintHelpMenu(argc,argv);
      exitcode=OptionFlagsBase::ExitCode::OFEC_EXITNOERR;
      return;
   }
   if (argc<minArgs) {
      ScreenUtils::SetScrRedBoldFont();
      cout << "\nError: Not enough arguments." << endl;
      ScreenUtils::SetScrNormalFont();
      cout << "\nTry: \n\t" << argv[0] << " -h\n" << endl << "to view the help menu.\n\n";
      exitcode=OptionFlagsBase::ExitCode::OFEC_EXITERR;
      return;
   }
   //outFileName and inFileName are processed in ProcessBaseOptions
   //cases h an V are also processed there.
   //See OptionFlagsBase class ProcessBaseOptions
   for (int i=1; i<argc; i++){
      if (argv[i][0] == '-'){
         switch (argv[i][1]){
            case 'h' :
               PrintHelpMenu(argc,argv);
               break;
            case 'v' :
               verboseLevel=(++i);
               if (i>=argc) {PrintErrorMessage(argv,'v');}
               break;
            case '-':
               //BaseProcessDoubleDashOption(argc,argv,i);
               ProcessDoubleDashOption(argc,argv,i);
               break;
            default:
               cout << "\nCommand line error. Unknown switch: " << argv[i] << endl;
               cout << "\nTry: \n\t" << argv[0] << " -h\n" << endl << "to view the help menu.\n\n";
               exitcode=OptionFlagsBase::ExitCode::OFEC_EXITERR;
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
   ScreenUtils::CenterString("This program computes the G4 energy of a molecule.");
   ScreenUtils::CenterString("Th program takes a report file as input. This file");
   ScreenUtils::CenterString("can be created with the script extractLabFQOTG4Info,");
   ScreenUtils::CenterString("which is distributed together with the source code");
   ScreenUtils::CenterString("of this program.See the help menu of");
   ScreenUtils::CenterString("extractLabFQOTG4Info for more details.");
   ScreenUtils::CenterString("The report is also generated automatically when");
   ScreenUtils::CenterString("using the script labfqotComputeG4, which requests");
   ScreenUtils::CenterString("and computes all the G4 individual calculations.");
   cout << endl;
   ScreenUtils::CenterString((string("Compilation date: ")+string(__DATE__)));
   cout << endl;
   ScreenUtils::CenterString(string("Version: ")+string(CURRENTVERSION));
   cout << endl;
   ScreenUtils::CenterString((string(":-) Created by: ")+string(PROGRAMCONTRIBUTORS)+string(" (-:")));
   cout << endl;
   ScreenUtils::PrintScrStarLine();
   ScreenUtils::SetScrBoldFont();
   cout << "\nUsage:\n\n\t" << rawprogramname << " report.dat [option [value(s)]] ... [option [value(s)]]\n\n";
   ScreenUtils::SetScrNormalFont();
   cout << "Here options can be:\n\n";
   cout << "  -o outfname\tSets the output file name to be outfname." << endl;
   cout << "  -v VerbLev \tSets the verbose level to be VerbLev. Default: 0.\n"
        << "             \t  The quantity of information printed to std::cout\n"
        << "             \t  increases as VerbLev increases, and VerbLev is an\n"
        << "             \t  integer." << '\n';
   cout << "  -V         \tDisplays the version of this program." << endl;
   cout << "  -h         \tDisplay the help menu.\n\n";
   //-------------------------------------------------------------------------------------
   cout << "  --set-freq-fact f\tForce the freqency factor to be f." << '\n';
   cout << "  --set-const-A a  \tForce the constant A to be a. (This constant\n"
        << "                   \t  is applied to the number of valence beta electrons.)" << '\n';
   cout << "  --set-alpha a    \tForce alpha to be a. (This is the factor used\n"
        << "                   \t  in the exponential term.)" << '\n';
   cout << "  --set-const-M m  \tForce M to be m. This constant is used to apply T1Diag\n"
        << "                   \t  correction, and it must be used together with\n"
        << "                   \t  option --set-t1diag (below)." << '\n';
   cout << "  --set-t1diag t1d \tSet T1Diag to be t1d. This variable must be used\n"
        << "                   \t  together with option --set-const-M (above)." << '\n';
   cout << "  --set-const-G g  \tForce G to be g. This constant is used to apply LHGap \n"
        << "                   \t  correction, and it must be used together with\n"
        << "                   \t  option --set-LHGap (below)." << '\n';
   cout << "  --set-LHGap lhg  \tSet LHGap to be lhg. This variable must be used\n"
        << "                   \t  together with option --set-const-G (above)." << '\n';
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
      case 'v' :
         cout << "should be followed by an integer." << '\n';
         break;
      case 'o':
         cout << "should be followed by a name." << endl;
         break;
      case 'S' :
         cout << "should be followed by a string." << endl;
         break;
      default:
         cout << "is triggering an unknown error." << endl;
         break;
   }
   ScreenUtils::SetScrNormalFont();
   cout << "\nTry:\n\t" << argv[0] << " -h " << endl;
   cout << "\nto view the help menu.\n\n";
   exitcode=OptionFlagsBase::ExitCode::OFEC_EXITERR;
   return;
}
void OptionFlags::ProcessDoubleDashOption(int &argc,char** &argv,int &pos) {
   string str=argv[pos];
   str.erase(0,2);
   if (str==string("version")) {
      cout << rawprogramname << " " << CURRENTVERSION << endl;
      exitcode=OptionFlagsBase::ExitCode::OFEC_EXITNOERR;
   } else if (str==string("help")) {
      PrintHelpMenu(argc,argv);
      exitcode=OptionFlagsBase::ExitCode::OFEC_EXITNOERR;
   } else if (str==string("set-freq-fact")) {
      setscalefreqfact=(++pos);
   } else if (str==string("set-const-A")) {
     setconsta=(++pos);
   } else if (str==string("set-alpha")) {
     setalpha=(++pos);
   } else if ( str==string("set-const-M") ) {
     setconstm=(++pos);
   } else if ( str==string("set-t1diag") ) {
     sett1diag=(++pos);
   } else if ( str==string("set-const-G") ) {
     setconstg=(++pos);
   } else if ( str==string("set-LHGap") ) {
     setlhgap=(++pos);
   } else {
      ScreenUtils::SetScrRedBoldFont();
      cout << "Error: Unrecognized option '" << argv[pos] << "'" << endl;
      ScreenUtils::SetScrNormalFont();
      exitcode=OptionFlagsBase::ExitCode::OFEC_EXITERR;
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


