#!/bin/bash

OUTDIR=testoutput
CHECKDIR=tests

if [ ! -d "$OUTDIR" ]
then
    mkdir $OUTDIR
fi

if [ "$FIZZBUZZEXE" == "" ]
then
    export FIZZBUZZEXE="build/fizzbuzz"
fi

export VERBY
if [ "$VERBOSE" != "" ]
then
    VERBY=1
fi

runtest()
{
    TESTNAME=$1
    shift 1 # the remaining arguments re passed to FIZZBUZZEXE
    
    if [ $VERBY ] ; then echo "$FIZZBUZZEXE $@ > $OUTDIR/$TESTNAME" ; fi
    $FIZZBUZZEXE $@ > $OUTDIR/$TESTNAME
    if [ "$?" -ne "0" ] ;
    then
        echo "test: $TESTNAME failed"
        echo 
        exit 1
    fi

    if [ $VERBY ] ; then echo "diff -Z -q $CHECKDIR/$TESTNAME $OUTDIR/$TESTNAME" ; fi
    diff -Z -q $CHECKDIR/$TESTNAME $OUTDIR/$TESTNAME
    if [ "$?" -ne "0" ] ;
    then
        echo "test: $TESTNAME failed"
        echo 
        exit 1
    fi

    echo $TESTNAME passed
    if [ $VERBY ] ; 
    then
        echo
    fi
}


if [ "$FIZZBUZZEXE" != "" ]
then
    runtest default-200 200
    runtest args-lf-200 -lf 200
    runtest args-z-lf-200 -z -lf 200
    runtest args-v-z-lf-200 -v -z -lf 200
    runtest args-v-a-z-lf-3000 -v -a -z -lf 3000

    echo all tests were run and passed
fi

