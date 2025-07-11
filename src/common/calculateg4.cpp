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
#include <cmath>
#include "atom.h"
#include "calculateg4.h"
#include "screenutils.h"
#include "stringtools.h"
#include "physicalconstants.h"

#define NICOLAIDESLOWERBOUND 260.0e0

CalculateG4::CalculateG4() {
   applySOCorr=false;
   useNicolaidesCorr=false;
   useTajtiCorr=false;
   useArgonneData=false;
   usrScaleFact=false;
   usrConstantA=false;
   usrAlpha=false;
   verboseLevel=1;
   useT1Diag=false;
   usrConstantM=false;
   useLHGap=false;
   usrConstantG=false;
}
CalculateG4::CalculateG4(RawG4sData &data,int vlvl) : CalculateG4() {
   verboseLevel=vlvl;
   rd=&data;
   if ( verboseLevel>0 ) { PrintWarnings(); }
   Compute();
}
void CalculateG4::Compute() {
   SetupVars();
   SafetyChecks();
   ComputeG4Energy();
   ComputeEnergiesFromStatTherm();
   ComputeG4Enthalpies0Kand298K();
   ComputeDeltaHf298KAtomization();
}
void CalculateG4::ComputeG4Energy() {
   double B1=rd->mp2gtbas1;
   double B2=rd->mp4gtbas1;
   double B3=rd->ccsdtg3bas1;
   double B4=rd->mp2gtbas2;
   double B5=rd->mp4gtbas2;
   double B6=rd->mp2gtbas3;
   double B7=rd->mp4gtbas3;
   double B8=rd->hfgtlargexp;
   double B9=rd->mp2gtlargexp;
   double B10=rd->hfgfhfb1;
   double B11=rd->hfgfhfb2;
   double B13=(rd->zpe)*scalingFreqFact;
   double E_HFLim=(B11-B10*exp(-alpha))/(1.0e0-exp(-alpha));
   double dEplus=B5-B2;
   double dE2dfp=B7-B2;
   double dECC=B3-B2;
   double dEG3LargeXP=B9-B6-B4+B1;
   double dEHF=E_HFLim-B8;
   double dESO=GetSpinOrbitCorr(); //Here would go the spin-orbit corrections (not needed for CHON).
   double EComb=B2+dEplus+dE2dfp+dECC+dEG3LargeXP+dEHF+dESO;
   double EHLC=GetHLCTerm();
   double EeG4=EComb+EHLC;
   if ( useT1Diag ) {
      EeG4+=GetT1DiagTerm();
   }
   if ( useLHGap ) {
      EeG4+=GetLHGapTerm();
   }
   g4Energy=EeG4+B13;
}
double CalculateG4::GetSpinOrbitCorr() {
   return 0.0e0;
}
int CalculateG4::GetValenceElectrons() {
   string s=rd->atomsInMolecule;
   string symb;
   int res=0;
   while ( s.size()>0 ) {
      symb=StringTools::GetFirstChunkAndDeleteFromLine(s);
      if ( symb==string("X") ) { continue; }
      res+=(Atom::GetValenceElectrons(symb));
   }
   res/=2;
   return res;
}
int CalculateG4::GetTotalNumberOfElectrons() {
   string s=rd->atomsInMolecule;
   string symb;
   int res=0;
   while ( s.size()>0 ) {
      symb=StringTools::GetFirstChunkAndDeleteFromLine(s);
      if ( symb==string("X") ) { continue; }
      res+=(Atom::GetAtomicNumberFromSymbol(symb));
   }
   return res;
}
double CalculateG4::GetHLCTerm() {
   if ( (rd->nElAlpha) != (rd->nElBeta) ) {
      ScreenUtils::DisplayErrorMessage("Only closed shell molecules are accepted!");
      ScreenUtils::DisplayErrorMessage("No HLCTerm computed.");
      return 0.0e0;
   }
   int nValEl=GetValenceElectrons();
   return (-nValEl*A);
}
double CalculateG4::GetT1DiagTerm() {
   //int nValEl=GetValenceElectrons();
   int nTotEl=GetTotalNumberOfElectrons();
   double t1=T1d*sqrt(double(nTotEl));
   return -t1*double(nTotEl)*M;
}
double CalculateG4::GetLHGapTerm() {
   //int nValEl=GetValenceElectddrons();
   //double nTotEl=double(GetTotalNumberOfElectrons());
   return G/(LHGap);//*sqrt(nTotEl);
}
void CalculateG4::ComputeG4Enthalpies0Kand298K() {
   string s=rd->atomsInMolecule;
   string symb;
   double sumHatom0=0.0e0;
   double sumH298mH0=0.0e0;
   int nAt=0;
   while ( s.size()>0 ) {
      symb=StringTools::GetFirstChunkAndDeleteFromLine(s);
      if ( symb==string("X") ) { continue; }
      nAt=Atom::GetAtomicNumberFromSymbol(symb);
      //cout << nAt << ' ';
      sumHatom0+=(G4AtomicProperties::GetG4StdEnthalpy0K(nAt));
      sumH298mH0+=(G4AtomicProperties::GetG4StdH298mH0(nAt));
   }
   g4Enthalpy0K=g4Energy+Evib+Erot+Etrans+PV;
}
void CalculateG4::ComputeDeltaHf298KAtomization() {
   string s=rd->atomsInMolecule;
   string symb;
   deltaHf0KAtomization=g4Energy;
   double sumDeltaHfAtom298Exp=0.0e0;
   int nAt=0;
   while ( s.size()>0 ) {
      symb=StringTools::GetFirstChunkAndDeleteFromLine(s);
      if ( symb==string("X") ) { continue; }
      nAt=Atom::GetAtomicNumberFromSymbol(symb);
      deltaHf0KAtomization-=(G4AtomicProperties::GetG4StdEnergy0K(nAt));
      deltaHf0KAtomization+=(GetDeltaHfAtom0KExp(nAt));
      sumDeltaHfAtom298Exp+=(GetDeltaHf298KmDeltaHf0KExp(nAt));
   }
   deltaHf298KAtomization=deltaHf0KAtomization;
   deltaHf298KAtomization+=(Evib+Erot+Etrans+PV);
   deltaHf298KAtomization-=sumDeltaHfAtom298Exp;
}
void CalculateG4::ChooseFrequencyFactor() {
   string method=rd->method;
   if ( method==string("g4-std") ) {
      scalingFreqFact=0.9854e0; // g09/G4 uses this factor 
      //scalingFreqFact=0.9650e0; // As it appears in https://cccbdb.nist.gov/vibscalejust.asp
   } else if ( method==string("g4-b3lyp") ) {
      scalingFreqFact=0.95932e0;
   } else if ( method==string("g4-m062x") ) {
      scalingFreqFact=0.96196e0;
   } else if ( method==string("g4-mp2") ) {
      scalingFreqFact=0.93312e0;
   } else if ( method==string("g4-wb97xd") ) {
      scalingFreqFact=0.95606e0;
   //} else if ( method==string("g4-pbe1pbe") ) {
   //   scalingFreqFact=0.957e0;
   //} else if ( method==string("g4-pbepbe" ) ) {
   //   scalingFreqFact=0.990e0;
   } else {
      string msg="Scaling factor for the method '";
      msg+=method;
      msg+="' is not implemented in this version. Setting scalingFreqFact=0.";
      ScreenUtils::DisplayErrorMessage(msg);
      cout << __FILE__ << ", line: " << __LINE__ << endl;
      scalingFreqFact=0.0e0;
   }
}
void CalculateG4::ChooseParameterA() {
   string method=rd->method;
   if ( method==string("g4-std") ) {
      A=6.947e-3; // g09/G4 uses this factor 
   } else if ( method==string("g4-b3lyp") ) {
      A=6.7354e-3;
   } else if ( method==string("g4-m062x") ) {
      A=6.8188e-3;
   } else if ( method==string("g4-mp2") ) {
      A=6.7168e-3;
   } else if ( method==string("g4-wb97xd") ) {
      A=6.7867e-3;
   //} else if ( method==string("g4-pbe1pbe") ) {
   //   A=6.947e-3;
   //} else if ( method==string("g4-pbepbe" ) ) {
   //   A=6.947e-3;
   } else {
      string msg="Constant A for the method '";
      msg+=method;
      msg+="' is not implemented in this version. Setting A=0.";
      ScreenUtils::DisplayErrorMessage(msg);
      cout << __FILE__ << ", line: " << __LINE__ << endl;
      A=0.0e0;
   }
}
void CalculateG4::SetScalingFreqencyFactor(const double ff) {
   usrScaleFact=true;
   scalingFreqFact=ff;
}
void CalculateG4::SetConstantA(const double aa) {
   usrConstantA=true;
   A=aa;
}
void CalculateG4::SetAlpha(const double aa) {
   usrAlpha=true;
   alpha=aa;
}
void CalculateG4::SetT1Diag(const double t1) {
   useT1Diag=true;
   T1d=t1;
}
void CalculateG4::SetConstantM(const double mm) {
   usrConstantM=true;
   M=mm;
}
void CalculateG4::SetLHGap(const double gap) {
   useLHGap=true;
   LHGap=gap;
}
void CalculateG4::SetConstantG(const double gg) {
   usrConstantG=true;
   G=gg;
}
void CalculateG4::SetupVars() {
   if ( !usrScaleFact ) {
      ChooseFrequencyFactor();
   } else {
      if ( verboseLevel>0 ) {
         cout << "Using frequency factor: " << scalingFreqFact << '\n';
      }
   }
   if ( !usrAlpha ) {
      alpha=1.63e0;
   } else {
      if ( verboseLevel>0 ) {
         cout << "Using alpha: " << alpha << '\n';
      }
   }
   if ( !usrConstantA ) {
      //A=0.006947e0;
      ChooseParameterA();
   } else {
      if ( verboseLevel > 0 ) {
         cout << "Using constant A: " << A << '\n';
      }
   }
   B=0.002441e0;
   C=0.007116e0;
   D=0.001414e0;
   if ( !usrConstantM ) {
      M=0.0e0;
   } else {
      if ( verboseLevel>0 ) {
         cout << "Using M: " << M << '\n';
      }
   }
   if ( !useT1Diag ) {
      T1d=0.0e0;
   } else {
      if ( verboseLevel>0 ) {
         cout << "Using T1Diag: " << T1d << '\n';
      }
   }
   if ( !usrConstantG ) {
      G=0.0e0;
   } else {
      if ( verboseLevel>0 ) {
         cout << "Using G: " << M << '\n';
      }
   }
   if ( !useLHGap ) {
      LHGap=0.0e0;
   } else {
      if ( verboseLevel>0 ) {
         cout << "Using LumoHomoGap: " << LHGap << '\n';
      }
   }
}
void CalculateG4::PrintWarnings() {
   string msg="In this version, the following restrictions apply:";
   msg+="\n   a) The G4 can be obtained ONLY for closed shell molecules.";
   ScreenUtils::DisplayWarningMessage(msg);
}
double CalculateG4::GetDeltaHfAtom0KExp(int nAt) {
   if ( useTajtiCorr ) {
      return G4AtomicProperties::GetDeltaHf0KTajti(nAt);
   }
   if ( useArgonneData ) {
      return G4AtomicProperties::GetDeltaHf0KATcT(nAt);
   }
   return G4AtomicProperties::GetDeltaHf0KJANAF(nAt);
}
double CalculateG4::GetDeltaHf298KmDeltaHf0KExp(int nAt) {
   return G4AtomicProperties::GetDeltaHf298KNISTJANAF(nAt);
}
void CalculateG4::ComputeEvib() {
   double fc100=100.0e0*scalingFreqFact*PhysicalConstants::c;
   double ookT=1.0e0/(2.9815e+02*PhysicalConstants::kB);
   double NA=PhysicalConstants::NA;
   double h=PhysicalConstants::h;
   double hnui,hnuiokT;
   double sum=0.0e0;
   lclZPE=0.0e0;
   double RT=PhysicalConstants::R*2.9815e+02; // In Joules/mol
   for ( size_t i=0 ; i<(rd->frequencies.size()) ; ++i ) {
      hnui=fc100*(rd->frequencies[i])*h;
      hnuiokT=hnui*ookT;
      if ( useNicolaidesCorr && (scalingFreqFact*(rd->frequencies[i])) < NICOLAIDESLOWERBOUND ) {
         sum+=(0.5e0*RT);
      } else {
         sum+=(NA*hnui/(exp(hnuiokT)-1.0e0));
      }
      lclZPE+=0.5e0*hnui;
   }
   EvibJ=sum; // in Joules/mol
   Evib=EvibJ*(G4AtomicProperties::JPMole2Hartree); //in hartrees
   lclZPE*=NA*G4AtomicProperties::JPMole2Hartree; // in hartrees; scaled
}
void CalculateG4::ComputeEnergiesFromStatTherm() {
   double RT=PhysicalConstants::R*2.9815e+02*(G4AtomicProperties::JPMole2Hartree); // In hartrees
   PV=RT; // in hartrees
   Erot=((rd->islinear) ? 1.0e0 : 1.5e0)*RT; // in hartrees
   Etrans=1.5e0*RT; //in hartrees
   ComputeEvib();
}
void CalculateG4::SafetyChecks() {
   if ( useTajtiCorr&&useArgonneData ) {
      ScreenUtils::DisplayErrorMessage("Tajti and Agonne is not recommended!");
   }
}

