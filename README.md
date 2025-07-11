# g4-nitro-closed-xxx

This repository contains the program ```g4-nitro-closed-xxx```, which computes the gas-phase
formation enthalpy of a molecule. The theoretical details are described in

[D. Ochoa-Resendiz, J. M. Solano-Altamirano, J. Sandoval-Lira, Julio M. Hernández-Pérez, *Jour. Chem. Phys.*, **163:2** (2025) 024121.](https://doi.org/10.1063/5.0264222)

Since July 10, 2025, this repository has been cloned the following number of times:

<a href='https://github.com/jmsolano/g4-nitro-closed-xxx'><img alt='GitHub Clones' src='https://img.shields.io/badge/dynamic/json?color=success&label=Clone&query=count&url=https://gist.githubusercontent.com/jmsolano/fc6084773c10f50aedf35940137cd353/raw/clone.json&logo=github'></a>


## Contents

This README file contains:

 * The instructions to compile and install the programs.
 * How to run a G09 calculation
 * How to use the program g4-nitro-closed-xxx
 * Output interpretation
 * Contributors list

## Auxiliary programs/dependencies
The main dependence of this script and the auxiliary programs is ```bash```, ```gcc```, and ```git```. In addition, ```g4-nitro-closed-xxx``` has been tested only on Linux and MacOS. Before compilation, install the following dependiencies.

### Linux
* Debian based: 

~~~~~~~
sudo apt install build-essential git
~~~~~~~

### MacOSX
* MacPorts

~~~~~~~
sudo port install gcc12 git
~~~~~~~

* Homebrew

~~~~~~~
brew install gcc@12 git
~~~~~~~

After installing the above dependencies, the source code to compile the program ```g4-nitro-closed-xxx``` is distrubuted along with this repository.

## Installation/compilation

After cloning this repository:

~~~~~~~~
git clone https://github.com/jmsolano/g4-nitro-closed-xxx.git
~~~~~~~~

Compile the programs  (**replace MYPATH by you local path to this repo**):

~~~~~~~~
cd MYPATH/src/
make
sudo make install

cd ../scripts
sudo make install
~~~~~~~~

The above commands will compile and install the programs ```getfe-g4-nitro-closed-xxx``` and ```moleculeinfo``` and
the script ```g4-nitro-closed-xxx```.

```g4-nitro-closed-xxx``` takes the g09 ```log/out``` file and prepare input files (```*-Report.dat```) for the program
```getfe-g4-nitro-closed-xxx```.

```getfe-g4-nitro-closed-xxx``` applies the atomization method together with other corrections to compute the formation enthalpy.

The program ```moleculeinfo``` is an auxiliary program used by the script g4-nitro-closed-xxx to determine whether the molecule
is linear or not.

## Updating the program (git instructions)

To update the programs, go to the repo path:

~~~~~~~
cd MYPATH/src/
~~~~~~~

Execute:

~~~~~~~
git pull
make fullclean
~~~~~~~

Follow the above instructions to compile and install the programs.

A simpler way is to clone the repository into a new path and to follow the compile and install instructions.

## Example g09 input files

Examples of how to run g09 calculations can be found in the directory ```inputExamplesG09```.
All ch4-*.inp can be run with Gaussian 09.

To prepare you own inputs, replace ```ch4.chk``` by your preferred ```chk``` filename (throughout the input file, *i.e.*, **replace all appearances**); replace the adquate charge and multiplicity, and replace the coordinates to match the respective molecule.


## How to use the program

The script has its own help menu; to view it, type:

~~~~~~~
g4-nitro-closed-xxx -h
~~~~~~~

However, to test the program, after installing the programs, use the following commands to test the program:

~~~~~~~
cd MYPATH/scripts
g4-nitro-closed-xxx -m b3lyp ch4-b3lyp.log
~~~~~~~

The output should look like this:

~~~~~~~
lclZPE= 0.04294149119
scaledZPE= 0.04294108184
G4-Nitro-Closed-XXX(0 K)= -40.46562435
G4-Nitro-Closed-XXX Enthalpy= -40.46180227
Delta_H_f(298.15_K) [HO-RR]/NIST:       -75.596 kJ mol-1
Delta_H_f(298.15_K) [HO-RR]/Tajti:      -74.991 kJ mol-1
Delta_H_f(298.15_K) [HO-RR]/ATcT:       -75.392 kJ mol-1
Delta_H_f(298.15_K) [Nicolaides]/NIST:  -75.596 kJ mol-1
Delta_H_f(298.15_K) [Nicolaides]/Tajti: -74.991 kJ mol-1
Delta_H_f(298.15_K) [Nicolaides]/ATcT:  -75.392 kJ mol-1
~~~~~~~

Notice that the method (```option -m```) must coincide with the appropriate method
requested through the ```g09``` input file. For instance, if ```g09``` has been
run on ```ch4-m062x.inp```, and the
output is saved into ```ch4-m062x.out```, then the formation enthalpy is obtained through:

~~~~~~~
g4-nitro-closed-xxx -m m062x ch4-m062x.out
~~~~~~~

The output should look like this:

~~~~~~~
lclZPE= 0.04335547608
scaledZPE= 0.0433555372
G4-Nitro-Closed-XXX(0 K)= -40.46558952
G4-Nitro-Closed-XXX Enthalpy= -40.46176829
Delta_H_f(298.15_K) [HO-RR]/NIST:       -75.506 kJ mol-1
Delta_H_f(298.15_K) [HO-RR]/Tajti:      -74.901 kJ mol-1
Delta_H_f(298.15_K) [HO-RR]/ATcT:       -75.302 kJ mol-1
Delta_H_f(298.15_K) [Nicolaides]/NIST:  -75.506 kJ mol-1
Delta_H_f(298.15_K) [Nicolaides]/Tajti: -74.901 kJ mol-1
Delta_H_f(298.15_K) [Nicolaides]/ATcT:  -75.302 kJ mol-1
~~~~~~~

## Output interpretation

The data printed in the screen is as follows:

* **```lclZPE```** is the Zero Point vibrational energy (in Hartrees), as obtained from the thermodynamics statistical equations.
* **```scaledZPE```** is the Zero Point vibrational energy obtained by scaling the g09 output number times the appropriate scaling factor.
* **```G4-Nitro-Closed-XXX(0K)```** is the G4-Nitro-Closed-XXX energy at 0 K, in Hartrees. If the method is chosen to be ```std```, then G4-Nitro-Closed-XXX is the G4 energy (*i.e.*, the standard output of a G4 calculation).
* **```G4-Nitro-Closed-XXX Enthalpy ```** is the enthalpy of the molecule, at 298.15 K. This includes thermal corrections. It is given in Hartrees.
* **```Delta_H_f(298.15_K)```** is the Gas-phase formation enthalpy of the molecule, obtained with the atomization method. The keywords indicate that the following approximations/corrections have been applied to compute it:
    * **HO-RR** indicates that the **H**armonic **O**scillator and **R**igid **R**otor approximations have been used.
    * **Nicolaides** correction is applied (see [Nicolaides *et al.*, *J. Phys. Chem.*, **100** (1996) 17460)](https://doi.org/10.1021/jp9613753).
    * **NIST** means that all experimental data was taken from [NIST webbook database](https://webbook.nist.gov/chemistry/name-ser/)
    * **Tajti** means that the carbon atomic formation enthalpy is replaced by Tajti's recommended value, see [A. Tajti *et al.*, *J. Chem. Phys.* **121** (2004) 11599–11613](https://pubs.aip.org/aip/jcp/article-abstract/121/23/11599/186868/HEAT-High-accuracy-extrapolated-ab-initio?redirectedFrom=fulltext). All other experimental data is taken from [NIST webbook database](https://webbook.nist.gov/chemistry/name-ser/).
    * **ATcT** means that every possible experimental data was taken from [Active Thermochemical Tables](https://atct.anl.gov). However, experimental data not published in the site was also taken from [NIST webbook database](https://webbook.nist.gov/chemistry/name-ser/).

## Contributors List

The following persons have significantly contributed to the development of the programs distributed through this repository:

* J. M. Solano-Altamirano
* J. M. Hernández-Pérez

JMSA and JMHP acknowledge the contributions of the following persons, their participation includes beta-testing, providing technical information, and performing auxiliary calculations.

* Jacinto Sandoval Lira
* David Ochoa Resendiz



