#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#*                                                                           *
#*            This file is part of the test engine for MIPLIB2010            *
#*                                                                           *
#* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
# $Id: run_mipcl.sh,v 1.0 2013/09/11 14:11:24 bzfheinz Exp $

SOLVER=$1
BINNAME=$2
NAME=$3
TIMELIMIT=$4
SOLFILE=$5
THREADS=$6
MIPGAP=$7


echo $SOLFILE

# set threads to given value 
# set mipgap to given value
# set timing to wall-clock time and pass time limit
# read, optimize, display statistics, write solution, and exit
echo $BINNAME  -solfile $SOLFILE -threads $THREADS -time $TIMELIMIT $NAME

$BINNAME -solfile $SOLFILE -threads $THREADS -time $TIMELIMIT $NAME

# remove MIPCL log file
rm *_mipcl.log

