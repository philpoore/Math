#!/bin/bash
# Generate random expressions for fuzz testing
# Usage: ./gen_fuzz_test.sh $NUM $LEN

cd $(dirname $0)

RED="\e[31m"
GREEN="\e[32m"
RESET="\e[0m"


NUM=${1:-100}
LEN=${2:-10}

function gen_expr () {
    max_len=${1:-20}
    d=$(( $RANDOM % 100 ))

    expr="${d}"
    while [ true ]
    do
        N=4
        n=$(( $RANDOM % $N ))

        if [ $n -eq 0 ]
        then
            d=$(( $RANDOM % 10 ))
            expr="${expr}+"
        elif [ $n -eq 1 ]
        then
            d=$(( $RANDOM % 10 ))
            expr="${expr}-"
        elif [ $n -eq 2 ]
        then
            d=$(( $RANDOM % 10 ))
            expr="${expr}*"
        elif [ $n -eq 3 ]
        then
            d=$(( $RANDOM % 10 ))
            expr="${expr}/"
        fi

        d=$(( $RANDOM % 100 ))
        expr="${expr}${d}"

        len=${#expr}

        if [ $len -gt $max_len ]
        then
            echo $expr
            break
        fi
    done
}

for i in `seq 1 $NUM`
do
    input=$(gen_expr $LEN)
    result=$(node -p "${input}")

    echo "${input}=${result}"
done