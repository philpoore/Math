#!/bin/bash

cd $(dirname $0)

RED="\e[31m"
GREEN="\e[32m"
RESET="\e[0m"

TEST_FILE=$@

input=$(cat $TEST_FILE | sed -e "/^$/d" -e "/^#/d")



echo "$input" | {
    
    PASS=0
    FAIL=0
    TOTAL=0

    while read test
    do
        input=$(echo $test | sed "s/=.*//g")
        expected=$(echo $test | sed "s/.*=//g")
        actual=$(../math $input)

        extra=""

        if [[ $actual == $expected ]]
        then
            res=$GREEN"pass"$RESET

            PASS=$((PASS + 1))
            TOTAL=$((TOTAL + 1))
        else
            res=$RED"fail"$RESET
            extra="$RED$actual$RESET"

            FAIL=$((FAIL + 1))
            TOTAL=$((TOTAL + 1))
        fi

        echo -e "--> [$res] $input = $expected $extra"
    done

    echo "Passed = $PASS" > /dev/stderr
    echo "Failed = $FAIL" > /dev/stderr
    echo "Total  = $TOTAL" > /dev/stderr

    if [ $FAIL -gt 0 ]
    then
        exit 1;
    fi
}
