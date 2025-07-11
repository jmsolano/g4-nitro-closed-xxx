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
#ifndef _COMMONHELPERS_H_
#define _COMMONHELPERS_H_

double f1(const double x,const double y,const double z) {
   double res=1.0e0+x+y*y+x*x*y+(x*x*x*x)+(y*y*y*y*y)+x*x*y*y*z*z;
   return res;
}
double f2(const double x,const double y,const double z) {
   double res=(0.75e0*exp(-0.25e0*(\
               (9.0e0*x-2.0e0)*(9.0e0*x-2.0e0)+\
               (9.0e0*y-2.0e0)*(9.0e0*y-2.0e0)+\
               (9.0e0*z-2.0e0)*(9.0e0*z-2.0e0))\
            ));
   res+=(0.75e0*exp(\
            -(9.0e0*x+1.0e0)*(9.0e0*x+1.0e0)/49.0e0\
            -(9.0e0*y+1.0e0)                /10.0e0\
            -(9.0e0*z+1.0e0)                /10.0e0\
            ));
   res+=(0.50e0*exp(-0.25e0*(\
               (9.0e0*x-7.0e0)*(9.0e0*x-7.0e0)+\
               (9.0e0*y-3.0e0)*(9.0e0*y-3.0e0)+\
               (9.0e0*z-5.0e0)*(9.0e0*z-5.0e0))\
            ));
   res-=(0.20e0*exp(\
            -(9.0e0*x-4.0e0)*(9.0e0*x-4.0e0)\
            -(9.0e0*y-7.0e0)*(9.0e0*y-7.0e0)\
            -(9.0e0*z-5.0e0)*(9.0e0*z-5.0e0)\
            ));
   return res;
}

#endif  /* _COMMONHELPERS_H_ */

