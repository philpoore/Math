#!/bin/bash

cd $(dirname $0)

RED="\e[31m"
GREEN="\e[32m"
RESET="\e[0m"

TEST_FILE=${1:-"tests.txt"}

cat $TEST_FILE | sed -e "/^$/d" -e "/^#/d" | while read test
do
    input=$(echo $test | sed "s/=.*//g")
    expected=$(echo $test | sed "s/.*=//g")
    actual=$(../math $input)

    
    extra=""

    if [[ $actual == $expected ]]
    then
        pass=true
        res=$GREEN"pass"$RESET
    else
        pass=false
        res=$RED"fail"$RESET
        extra="$RED$actual$RESET"
    fi

    echo -e "--> [$res] $input = $expected $extra"
done
