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
#ifndef _INPUTMOLECULE_PDB_H_
#define _INPUTMOLECULE_PDB_H_
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <vector>
using std::vector;
#include "molecule.h"

enum class PDBRecordType {UNDEF,ATOM,HETATM,ANISOU,CRYST1,\
   COMPND,MODEL,ENDMDL,TER,HEADER,TITLE,REMARK,CONECT};
/* ************************************************************************** */
class InputMoleculePDB : public Molecule {
/* ************************************************************************** */
public:
   InputMoleculePDB();
   InputMoleculePDB(string fname);
   InputMoleculePDB(string fname,short int setVbsLvl);
   void ReadFromFile(string fname);
   /** Reads a single model from the pdb file. The reading stars at
    * the position buffPos. The function returns the buffer position
    * at which the line 'ENDMDL' ends. */
   size_t ReadModel(ifstream &ifil,const size_t buffPos=0);
   void Save(const string &onam) const;
   void WriteModel(ofstream &ofil) const;
   void DisplayProperties();
   void ParseToScreen();
   void ExtractAtoms();
   void ReloadAtoms();
   static bool RecordTypeIs(const string &line,const string &testrectp);
   /** Copies the atom coordinates contained in line  */
   static void CopyAtomCoordinates(const string &line,vector<double> &x);
   /** Copies the current atoms coordinates to the respective buffer items */
   void CopyAllAtomCoordinates();
   vector<string> buffer;
   vector<size_t> atmBuffIdx; /*!< atmBuffIdx[i] is the buffer index associated with atom[i]  */
   vector<PDBRecordType> buffRecType;
   /** Replaces/overwrites the coordinates of atom[idx] in the respective
    * buffered record (buffer[atmBuffIdx[idx]]).  */
   void ReplaceCoordinatesInRecord(const size_t idx,const vector<double> &x);
   void SetVerboseLevel(short int sv) { verboseLevel=sv; }
   PDBRecordType GetRecordType(const string &line);
/* ************************************************************************** */
protected:
   short int verboseLevel;
   bool showHWarnings;
/* ************************************************************************** */
};
/* ************************************************************************** */
std::ostream &operator<<(std::ostream &out,const InputMoleculePDB (&mol));

#endif  /* _INPUTMOLECULE_PDB_H_ */

