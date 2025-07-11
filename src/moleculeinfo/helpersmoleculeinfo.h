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
#ifndef _HELPERSMOLECULEINFO_H_
#define _HELPERSMOLECULEINFO_H_
#include <memory>
using std::shared_ptr;
#include "molecule.h"

/* ************************************************************************** */
class HelpersMoleculeInfo {
/* ************************************************************************** */
public:
/* ************************************************************************** */
   HelpersMoleculeInfo();
   HelpersMoleculeInfo(int vl) : HelpersMoleculeInfo() {verboseLevel=vl;}
   void SetVerboseLevel(int vl) {verboseLevel=vl;}
/* ************************************************************************** */
   /** Displays the indices of the neighbour atoms of the atom idx.
    * @param[in] idx : The index of an atom (from 1-N --N=number of atoms
    * in the molecule--).
    * Notice: all indices in this function are counted from 1 to N,
    * as opposed to 0 - (N-1). This is for the convenience of the user. */
   void DisplayIndicesOfNeighbourAtoms(shared_ptr<Molecule> mol,size_t idx);
   size_t NumberOfNeighbours(shared_ptr<Molecule> mol,size_t idx);
   /** Checks if the molecule is linear. If it is, the function returns true.  */
   bool CheckIfMoleculeIsLinear(shared_ptr<Molecule> mol);
/* ************************************************************************** */
protected:
/* ************************************************************************** */
   int verboseLevel;
/* ************************************************************************** */
};
/* ************************************************************************** */


#endif  /* _HELPERSMOLECULEINFO_H_ */

