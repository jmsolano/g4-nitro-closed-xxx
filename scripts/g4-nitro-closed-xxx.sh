#!/bin/bash

prog_name=$(echo "$0" | sed -e 's;\(.*\)/\(.*\);\2;')

help_and_syntax() {
   the_prog_name=$1
echo -e "\n    Script syntax: \033[1m$the_prog_name \033[0m [\033[0;4moption(s)\033[0m \
[\033[4margument(s)\033[0m]] \033[4mbaseName.xxx\033[0m"
#Here you may put a description of the script. The text between 'END_USAGE_TEXT'
#will be displayed as it is.
cat << END_USAGE_TEXT

This script extracts the relevant information from a Gaussian09 outputfile,
wherein the eight steps of a G4-XXX calculations were requested.

The script will take the g09 log/out filename, and it will create a new
file. For instance, if the g09 log/out file has the basename 'baseName.log',
(baseName.out is also an acceptable name)
then the extracted information will be saved into a file named
baseName-ReportG09.dat. The last file can be used as input of
getfe-g4-xxx, however the script executes this program when it is run.

getfe-g4-xxx and $prog_name are developed by the
Laboratory of Mathematical Modelling and Scientific Software Development
   and the
Laboratory of Organic Physical Chemistry
of the Faculty of Chemical Sciences
of Meritorious Autonomous University of Puebla.

The available options are:
   -m method        Set xxx to be method. Valid options:
                       std, b3lyp, wb97xd, m062x, mp2.
   -c               Force to look for Cartesian coordinates
   -z               Force to look for Z-matrix
   -h               Display the help menu.

END_USAGE_TEXT
}

if [ $# -lt 1 ]; then
   echo "Error: input filename missing (Try -h for more information.)."
   exit 2
fi

if [ $# -eq 1 ]; then
   if [ "$1" == "-h" ];then
      help_and_syntax $prog_name
      exit 0
   fi
fi

g09LogName="$BASH_ARGV"
if [ ! -f "$g09LogName" ];then
   echo -e "File $g09LogName does not exist!\nExiting..."
   exit 1
fi
baseName="${g09LogName%.log}"
baseName="${baseName%.out}"

method="std"
paramF="0.9854"
paramA="0.006947"
debugVersion="F"
haveZmatrix="T"
if [ "$(grep -e Variables: $g09LogName | wc -l | tr -d ' ')" == "0" ];then
   haveZmatrix="F"
fi

while getopts "::hcDm:z" opt; do
  case $opt in
    c)
      haveZmatrix="F"
      ;;
    D)
      debugVersion="T"
      ;;
    m)
       method="$(echo $OPTARG | tr [:upper:] [:lower:])"
      ;;
    z)
      haveZmatrix="T"
      ;;
    h)
      help_and_syntax $prog_name
      exit 0
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      echo -e "(Try\n   $prog_name -h\nfor more information.)"
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      echo -e "(Try\n   $prog_name -h\nfor more information.)"
      exit 1
      ;;
  esac
done

#The corpus of the script
shift $(($OPTIND - 1))

#Some help to the developer:
#the number of items in an array: nTerms=$(( ${#theArray[@]} - 1 ))

check_multiplicity() {
   the_log_file="$1"
   the_mult="$(grep -e Multiplicity $the_log_file | head -n 1 | awk '{print $NF}')"
   if [ "$the_mult" != "1" ];then
      echo "Error: The current version of $prog_name cannot be used to study"
      echo "open-shell systems nor excited states!"
      exit 1
   fi
}
get_list_blocks_start() {
   the_log_file="$1"
   grep -ne "1\\\\1" $the_log_file | awk '{print $1}' | tr -d ':' | tr '\n' ' '
}
get_list_blocks_end() {
   the_log_file="$1"
   grep -ne "\\@" $the_log_file | awk '{print $1}' | tr -d ':' | tr '\n' ' '
}
get_logblock() {
   the_init_line=$1
   the_final_line=$2
   the_filename=$3
   if (( $the_init_line > $the_final_line ));then
      echo "Error: logblock information could not be properly read!"
      echo "init line > final line!"
      exit 2
   fi
   sed -n "$the_init_line","$the_final_line"p $the_filename | tr '' '\n' |\
      sed -e 's/^ //' | tr -d '@' | tr -d '\n' | tr '\\' '\n'
}
get_first_z_matrix() {
   the_log_file="$1"
   cat $the_log_file | sed -n '/Symbolic Z-matrix/,/Variables:/p' |\
   awk '{print $1}' | grep -v "^$"  | grep -v "Charge\|Variables\|Symbolic"
}
get_first_cart_coords() {
   the_log_file="$1"
   cat $the_log_file | sed -n '/Symbolic Z-matrix/,/GradGrad/p' |\
   awk '{print $1}' | grep -v "^$"  | grep -v "Charge\|Grad\|Symbolic"
}
get_last_cart_coords() {
   the_log_file="$1"
   the_start_line_num="$(grep -ne "Standard orientation:" $the_log_file |\
      tail -n 1 | awk '{print $1}' | tr -d ':')"
   the_start_line_num=$(( $the_start_line_num + 5 ))
   sed -n ${the_start_line_num},'/------/'p $the_log_file | grep -v "^ ----" |\
      awk '{print $2" "$4" "$5" "$6}'
}
count_atoms() {
   the_log_file="$1"
   get_last_cart_coords $the_log_file | wc -l | tr -d ' '
}
print_frequencies() {
   the_log_file="$1"
   the_frequencies="$(grep -e Frequencies $the_log_file | tr -s ' ' |\
      sort -gk 3 | uniq |\
      sed -e 's/^ Frequencies -- //' | sort -g )"
   echo ${the_frequencies} | awk '{print NF}'
   echo $the_frequencies | tr ' ' '\n'
}

check_multiplicity $g09LogName

arrayBlockStart=($(get_list_blocks_start $g09LogName))
arrayBlockEnd=($(get_list_blocks_end $g09LogName))

nBlocksStart=$(( ${#arrayBlockStart[@]} - 1 ))
nBlocksEnd=$(( ${#arrayBlockEnd[@]} - 1 ))

if [ "$nBlocksStart" != "$nBlocksEnd" ];then
   echo "Error: Incomplete blocks!"
   echo "nBlocksStart: $nBlocksStart"
   echo "nBlocksEnd: $nBlocksEnd"
   exit 1
else
   if [ "$debugVersion" == "T" ];then
      echo "Blocks OK..."
   fi
fi

reportName="${baseName}-ReportG09.dat"

if [ "$debugVersion" == "T" ];then
   echo "baseName: $baseName" 
   echo "repName:  $reportName"
fi
echo "#This file contain sthe information extracted from the Gaussian09" > $reportName
echo "# G4 calculation." >> $reportName

tmpName="$g09LogName"
echo -e "#\n#$tmpName:\n#" >> $reportName
echo -e "ATOMS_IN_MOLECULE" >> $reportName
if [ "$haveZmatrix" == "T" ];then
   get_first_z_matrix $tmpName | sort | tr '\n' ' '>> $reportName
else
   get_first_cart_coords $tmpName | sort | tr '\n' ' '>> $reportName
fi
echo -e "\nALPHA_ELECTRONS" >> $reportName
grep -e "alpha electrons" $tmpName | tail -n 1 | awk '{print $1}'  >> $reportName
echo -e "BETA_ELECTRONS" >> $reportName
grep -e "alpha electrons" $tmpName | tail -n 1  | tr -s ' ' | sed -e 's/^ //' | awk '{print $4}'  >> $reportName

echo -e "METHOD\ng4-$method" >> $reportName

tmpXYZ="${RANDOM}${RANDOM}.xyz"
#count_atoms $g09LogName > $tmpXYZ
numOfAtoms="$(count_atoms $g09LogName)"
if [ "$numOfAtoms" == "1" ];then
   echo "Error: The current version of $prog_name cannot compute properties of"
   echo "single atoms!"
   exit 2
fi
echo $numOfAtoms > $tmpXYZ
echo "Coordinates extracted from $g09LogName" >> $tmpXYZ
get_last_cart_coords $g09LogName >> $tmpXYZ
echo -e "IS_LINEAR\n$(moleculeinfo $tmpXYZ -l)" >> $reportName
rm $tmpXYZ

zpe="$(grep -e "Zero-point correction=" $tmpName | awk '{print $3}')"
echo ZeroPoint >> $reportName
echo $zpe >> $reportName

echo -e "FREQUENCIES" >> $reportName
print_frequencies $tmpName >> $reportName

echo -e "#\n#Step3:\n#" >> $reportName
get_logblock ${arrayBlockStart[2]} ${arrayBlockEnd[2]} $g09LogName \
  | grep -v "#" | grep -e "MP2=\|MP4SDTQ=\|CCSD(T)=" | tr '=' '\n' >> $reportName

echo -e "#\n#Step4:\n#" >> $reportName
get_logblock ${arrayBlockStart[3]} ${arrayBlockEnd[3]} $g09LogName \
  | grep -v "#" | grep -e "MP2=\|MP4SDTQ=" | tr '=' '\n' >> $reportName

echo -e "#\n#Step5:\n#" >> $reportName
get_logblock ${arrayBlockStart[4]} ${arrayBlockEnd[4]} $g09LogName \
  | grep -v "#" | grep -e "MP2=\|MP4SDTQ=" | tr '=' '\n' >> $reportName

echo -e "#\n#Step6:\n#" >> $reportName
get_logblock ${arrayBlockStart[5]} ${arrayBlockEnd[5]} $g09LogName \
  | grep -v "#" |  grep -e "HF=\|MP2=" | tr '=' '\n' >> $reportName

echo -e "#\n#Step7:\n#" >> $reportName
get_logblock ${arrayBlockStart[6]} ${arrayBlockEnd[6]} $g09LogName \
  | grep -v "#" |  grep -e "HF=" | tr '=' '\n' >> $reportName

echo -e "#\n#Step8:\n#" >> $reportName
get_logblock ${arrayBlockStart[7]} ${arrayBlockEnd[7]} $g09LogName \
  | grep -v "#" |  grep -e "HF=" | tr '=' '\n' >> $reportName

getfe-g4-nitro-closed-xxx $reportName

