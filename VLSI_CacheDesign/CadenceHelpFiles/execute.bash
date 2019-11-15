#!/bin/bash

###############################################################################################
# File Name: 		 execute.bash
# Author: 			 Dang-Quang Tran (dtran6@umbc.edu)
# Description: 	 A script for CMPE 315/640: Principles of VLSI Design lab VHDL programs.
# Last Revision: 11/15/2017
###############################################################################################

###############################################################################################
# Setup Instructions:
#
#   1. Place this file and the write_testbench/write_testbench.cpp files in a directory. 
#      (Recommendation: place in the directory that holds your Cadence files, such as hdl.var, 
#      cds.lib, run_ncvhdl.bash, etc., or in a subdirectory that is in the directory that 
#      holds your Cadence files.)
#
#   2. Add an alias called vlsi with the command to run this script to the .cshrc file 
#      in your home directory. Specify the path to this script. E.g.:
#
#         alias vlsi "bash ~/CMPE315/execute.bash"
#
#      You would replace "~/CMPE315/" with your own path to your directory in which this script
#      is located. Save the .cshrc file.
#
#   3. Restart the terminal, or run the following command:
#
#         source .cshrc
#
#   4. Replace the value of the PATH_TO_CADENCE_FILES variable in this script with your own
#      path to your directory in which your Cadence files are located. Then replace the value
#      of the PATH_TO_TESTBENCH_WRITER variable in this script with your own path to your
#      directory in which the write_testbench executable is located. Save this file.
#
#   5. Run the following command to view the usage of the script:
#
#         vlsi
#
###############################################################################################

# Notes: 
#		
#		- You must adjust PATH_TO_CADENCE_FILES to be the path to your Cadence files.
#		- You must adjust PATH_TO_TESTBENCH_WRITER to be the path to your directory that holds the
#     write_testbench executable.
#		- This script works under the assumption that every file is named after the entity 
#			it defines.

PATH_TO_CADENCE_FILES=~/CMPE315 # Modify to your own path.
PATH_TO_TESTBENCH_WRITER=~/CMPE315 # Modify to your own path.

BUILD_USAGE="vlsi build [-v93] FILE_NAME(S)"
BUILDUP_USAGE="vlsi buildup FILE_NAME"
ELAB_USAGE="vlsi elab ITEM_NAME(S)"
SIM_USAGE="vlsi sim SIM_FILE_NAME ITEM_NAME"
WRITETEST_USAGE="vlsi writetest ITEM_NAME"
RUNTEST_USAGE="vlsi runtest SIM_FILE_NAME ITEM_NAME"

ENTITY_NOT_FOUND_ERROR="--- ERROR: Could not locate entity "
FILE_NOT_FOUND_ERROR="--- ERROR: Could not locate file "

function build() {
	entity=$1
	version=$2
	echo
	echo --- Compiling $entity:
	echo
	echo $PATH_TO_CADENCE_FILES/run_ncvhdl.bash $version -messages -linedebug -cdslib $PATH_TO_CADENCE_FILES/cds.lib -hdlvar $PATH_TO_CADENCE_FILES/hdl.var -smartorder $entity
	echo
	$PATH_TO_CADENCE_FILES/run_ncvhdl.bash $version -messages -linedebug -cdslib $PATH_TO_CADENCE_FILES/cds.lib -hdlvar $PATH_TO_CADENCE_FILES/hdl.var -smartorder $entity
}

function build_recursively() {
	entity=$1
	built=()
	build_recursively_helper ${entity%.*} $built
	build $entity
}

function build_recursively_helper() {
	local entity=$1
	built=$2
	local component
	local components
	if [ -e $entity.vhdl ]
	then
		components="$(grep -oP '(?<=component ).+?(?=$)' $entity.vhdl)"
	elif [ -e $entity.vhd ]
	then
		components="$(grep -oP '(?<=component ).+?(?=$)' $entity.vhd)"
	fi
	for component in $components
	do
		build_recursively_helper $component $built
		if [[ " ${built[*]} " != *" $component "* ]]
		then
			if [ -e $component.vhdl ]
			then
				build $component.vhdl
				built+=("$component")
			elif [ -e $component.vhd ]
			then
				build $component.vhd
				built+=("$component")
			else
				echo
				echo $ENTITY_NOT_FOUND_ERROR $component.
			fi
		fi
	done
}

function elaborate() {
	out=$1
	echo
	echo --- Elaborating $out:
	echo
	echo $PATH_TO_CADENCE_FILES/run_ncelab.bash -messages -access rwc -cdslib $PATH_TO_CADENCE_FILES/cds.lib -hdlvar $PATH_TO_CADENCE_FILES/hdl.var $out
	echo
	$PATH_TO_CADENCE_FILES/run_ncelab.bash -messages -access rwc -cdslib $PATH_TO_CADENCE_FILES/cds.lib -hdlvar $PATH_TO_CADENCE_FILES/hdl.var $out
}

function simulate() {
	runfile=$1
	item=$2
	echo
	echo --- Running simulation $item using $runfile:
	echo
	echo $PATH_TO_CADENCE_FILES/run_ncsim.bash -input $runfile -messages -cdslib $PATH_TO_CADENCE_FILES/cds.lib -hdlvar $PATH_TO_CADENCE_FILES/hdl.var $item
	echo
	$PATH_TO_CADENCE_FILES/run_ncsim.bash -input $runfile -messages -cdslib $PATH_TO_CADENCE_FILES/cds.lib -hdlvar $PATH_TO_CADENCE_FILES/hdl.var $item
}

function write_test() {
  entityfile=$1
  echo
  echo --- Writing testbench for $1:
  echo
  echo $PATH_TO_TESTBENCH_WRITER/write_testbench $1
  echo
  $PATH_TO_TESTBENCH_WRITER/write_testbench $1
}

function display_usage() {
	echo "usage:" $BUILD_USAGE
	echo "      " $BUILDUP_USAGE
	echo "      " $ELAB_USAGE
	echo "      " $SIM_USAGE
	echo "      " $WRITETEST_USAGE
	echo "      " $RUNTEST_USAGE
}

if [ $# -eq 0 ]
then
	display_usage
else
	vlsi_command=$1
	if [ $vlsi_command == 'build' ]
	then
		if [ $# -lt 2 ]
		then
			echo usage: $BUILD_USAGE
			echo 
			echo "Compiles the specified file(s)."
		else
			version=""
			if [ $2 == '-v93' ]
			then
				shift
				version="-v93"
			fi
			shift
			while test $# -gt 0
			do
				entity=$1
				if [ -e $entity ]
				then
					build $entity $version
				else
					echo
					echo $FILE_NOT_FOUND_ERROR $entity.
				fi
				shift
			done
		fi
	elif [ $vlsi_command == 'buildup' ]
	then
		if [ $# -lt 2 ]
		then
			echo usage: $BUILDUP_USAGE
			echo
			echo "Compiles the specified file and all its dependencies from the bottom up."
		else
			build_recursively $2
		fi
	elif [ $vlsi_command == 'elab' ]
	then
		if [ $# -lt 2 ]
		then
			echo usage: $ELAB_USAGE
			echo
			echo "Elaborates the specified item(s)."
		else
			shift
			while test $# -gt 0
			do
				entity=$1
				elaborate $entity
				shift
			done
		fi
	elif [ $vlsi_command == 'sim' ]
	then
		if [ $# -lt 3 ]
		then
			echo usage: $SIM_USAGE
			echo
			echo "Simulates the test using the simulation file."
		else
      simulate $2 $3
		fi
  elif [ $vlsi_command == 'writetest' ]
  then
    if [ $# -lt 2 ]
    then
      echo usage: $WRITETEST_USAGE
      echo
      echo "Creates testbench(es) for the entity/entities in the specified file(s)."
    else
			shift
			while test $# -gt 0
			do
				entityfile=$1
				write_test $entityfile
				shift
			done
    fi
	elif [ $vlsi_command == 'runtest' ]
	then
		if [ $# -lt 3 ]
		then
			echo usage: $RUNTEST_USAGE
			echo
			echo "Elaborates and runs the test using the .run file."
		else
      elaborate $3
      simulate $2 $3
		fi
  elif [ $vlsi_command == 'buildtestwriter' ]
  then
    echo
    echo --- Compiling testbench writer program write_testbench.cpp:
    echo
    echo g++ -o $PATH_TO_TESTBENCH_WRITER/write_testbench -Wall $PATH_TO_TESTBENCH_WRITER/write_testbench.cpp
    echo
    g++ -o $PATH_TO_TESTBENCH_WRITER/write_testbench -Wall $PATH_TO_TESTBENCH_WRITER/write_testbench.cpp
	else
		echo $vlsi_command: Command not found.
		echo
		display_usage
	fi
fi
