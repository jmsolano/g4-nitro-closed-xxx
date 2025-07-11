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
#ifndef _CALCULATEG4_H_
#define _CALCULATEG4_H_
#include "rawg4sdata.h"
#include <memory>
using std::shared_ptr;
#include "g4atomicproperties.h"

/* ************************************************************************** */
/** This class computes the G4 energy of a molecule. The std method
 * follows exactly the recipe described in 
 * L. A. Curtiss, P. C. Redfern, and K. Krishnan, J. Chem. Phys., 126 (2007) 084108
 * (hereafter [curtiss2007]). 
 * The mp2 method is a variant proposed in the Laboratory of Organic
 * Physical Chemistry.  */
class CalculateG4 {
/* ************************************************************************** */
public:
/* ************************************************************************** */
   CalculateG4(RawG4sData &data,int vlvl=1);
   /**  Function to manually compute the thermodynamics; it is called by the constructor,
    * so it might be useful for computind more than one variant (selecting Tajti, Nicolaides,
    * etc. */
   void Compute();
   /** Returns the G4 energy, in hartrees (this should coincide with the
    * g09 output ---G4(0K)---, when using std method.  */
   double G4Energy() const { return g4Energy;}
   /** Returns the G4 enthalpy, in hartres. This should coincide
    * with the g09 output ---G4 Enthalpy---, when using std method.   */
   double G4Enthalpy0K() const { return g4Enthalpy0K;}
   /** Returns the Delta H f, using atomization reactions, at 0 K. in \f$kJ mol^{-1}\f$  */
   double DeltaHf0KAtomization() const { return deltaHf0KAtomization*G4AtomicProperties::hartree2kJPerMole; }
   /** Returns the Delta H f, using atomization reactions, at 298.15 K. in \f$kJ mol^{-1}\f$ */
   double DeltaHf298KAtomization() const { return deltaHf298KAtomization*G4AtomicProperties::hartree2kJPerMole; }
   void UseNicolaidesCorrection(bool unc) { useNicolaidesCorr=unc; }
   void UseTajtiCorrection(bool utc) { useTajtiCorr=utc; }
   void UseArgonneData(bool uad) {  useArgonneData=uad; }
   double LclZPE() { return lclZPE; }
   double ScaledZPE() { return (rd->zpe)*scalingFreqFact; }
   void SetScalingFreqencyFactor(const double ff);
   double GetScalingFrequencyFactor() {return scalingFreqFact;}
   void SetConstantA(const double aa);
   double GetConstantA() {return A;}
   void SetAlpha(const double aa);
   double ScalingFreqencyFactor(void) { return scalingFreqFact; }
   void SetT1Diag(const double t1);
   void SetConstantM(const double mm);
   void SetLHGap(const double gap);
   void SetConstantG(const double gg);
/* ************************************************************************** */
protected:
/* ************************************************************************** */
   RawG4sData* rd;
   CalculateG4(); //Forbidden to construct without data.
   /** Future compatibility function. Currently, it returns zero. It should compute the spin-orbit correction
    * for atoms and a few non-closed-shell molecules.  */
   double GetSpinOrbitCorr();
   /** Computes the High-level corrections; see curtiss2007, Eq. (7) and text below it.  */
   double GetHLCTerm();
   double GetT1DiagTerm();
   double GetLHGapTerm();
   int GetValenceElectrons();
   int GetTotalNumberOfElectrons();
   void ComputeG4Energy();
   void ComputeG4Enthalpies0Kand298K();
   void ComputeDeltaHf298KAtomization();
   void PrintWarnings();
   void SetupVars();
   void ChooseFrequencyFactor();
   void ChooseParameterA();
   void SafetyChecks();
   /** Returns the experimental atomic enthalpy of formation at 0 K.
    * This function selects the appropriate value depending on
    * the recommendations, i.e. JANAF values are used, unless
    * otherwise specified (e.g. using Tajti carbon data, or
    * Argonne tables. Default: JANAF. In this version, only JANAF
    * values are returned. Reimplement this to include other
    * corrections or variants.  */
   double GetDeltaHfAtom0KExp(int nAt);
   double GetDeltaHf298KmDeltaHf0KExp(int nAt);
   /** This function computes the vibrational energy WITHOUT
    * the ZPE.  */
   void ComputeEvib();
   void ComputeEnergiesFromStatTherm();
   double g4Energy; /*!< Where the G4 energy at 0 K is internally saved. In a.u.  */
   double g4Enthalpy0K; /*!< Where the G4 enthalpy at 0 K is saved.  In a.u. */
   double g4Enthalpy298K; /*!< Where the G4 enthalpy at 298 K is saved. In a.u.  */
   double deltaHf0KAtomization; /*!< Where the Delta H_f(0 K), using atomization reactions, is saved; in a.u.  */
   double deltaHf298KAtomization; /*!< Where the Delta H_f(298.15 K), using atomization reactions, is saved; in a.u.  */
   double scalingFreqFact; /*!< Contains the frequency scaling factor. Taken from: https://cccbdb.nist.gov/vibscalejust.asp  */
   double alpha; /*!< See Eq. (1) of [curtiss2007]  */
   double A; /*!< See Eq. (7), and the paragraph that follows it, of [curtiss2007].  */
   double B; /*!< See Eq. (7), and the paragraph that follows it, of [curtiss2007].  */
   double C; /*!< See Eq. (7), and the paragraph that follows it, of [curtiss2007].  */
   double D; /*!< See Eq. (7), and the paragraph that follows it, of [curtiss2007].  */
   double M; /*!< Multireference correction constant. M is intended to be used together with T1Diag.  */
   double T1d; /*!< T1diag value.  */
   double G; /*!< Multireference (LHgap) correction constant. G is intended to be used together with LHgap.  */
   double LHGap;
   bool applySOCorr; /*!< set to true for applying spin orbit corrections (see ComputeSpinOrbitCorr).  */
   double Evib; /*!< Vibrational energy, in a.u.  */
   double EvibJ; /*!< Vibrational energy, in Joules  */
   double Erot; /*!< Rotational energy, in a.u.  */
   double Etrans; /*!< Translational energy, in a.u.  */
   double PV; /*!< PV term, in a.u.  */
   double lclZPE; /*!< Experimental/test/trial zero point energy; currently not working.  */
   bool useNicolaidesCorr;
   bool useTajtiCorr;
   bool useArgonneData;
   bool usrScaleFact;
   bool usrConstantA;
   bool usrAlpha;
   bool useT1Diag;
   bool usrConstantM;
   bool useLHGap;
   bool usrConstantG;
   int verboseLevel;
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _CALCULATEG4_H_ */

