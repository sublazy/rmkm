#!/usr/bin/env bats

# Helper functions
# -----------------------------------------------------------------------------
setup() {
    sudo insmod ../rmkm.ko
    sudo chmod o+rw /dev/median
    echo -e "\n--- $BATS_TEST_DESCRIPTION" >> report
}

teardown() {
    sudo rmmod rmkm.ko
}

# Supply data to the module; also store it in the report file.
feed() {
    echo -e $@ > /dev/median
    echo "input: $@" >> report
}

# Supply data from a file to the module; store in report file.
file() {
    local data_file=$1
    cat $data_file > /dev/median
    echo -n "input: " >> report
    cat $data_file >> report
}

# Get the median from the module; store in report file.
median() {
    cat /dev/median | tee --append report
}

# Syntactic sugar for typing expected result
result() {
    [ $(median) = $1 ]
}

# Tests
# -----------------------------------------------------------------------------
@test "Example #1 from the assignment" {
    echo 1 2 3 > /dev/median
    ans=`cat /dev/median`
    [ "$ans" = "2.0" ]
}

@test "Example #2 from the assignment" {
    echo 1 2 > /dev/median
    ans1=`cat /dev/median`
    echo 3 > /dev/median
    ans2=`cat /dev/median`
    [ "$ans1" = "1.5" ] && [ "$ans2" = "2.0" ]
}

@test "Whitespace bonanza" {
    echo -e "   \t  1\t  \n\t\v   \t2  \n2\n   \v   \v\t\t\n  3  \t " > /dev/median
    ans=`cat /dev/median`
    [ "$ans" = "2.0" ]
}

@test "Test helper functions and syntactic sugars" {
    feed 1 2 3
    result 2.0
}

@test "Feed whitespace" {
    feed "1     2     \t\t 3"
    result 2.0
}

@test "Two-stage input" {
    feed 12 8
    result 10.0
    feed 3
    result 8.0
}

@test "Input from file" {
    file data/basic
    result 3.0
}

@test "Return NaN when no input supplied" {
    result NaN
}

@test "Multiple queries keep reporting NaN" {
    result NaN
    result NaN
    result NaN
    result NaN
    result NaN
    result NaN
    feed 1
    result 1.0
}

@test "Single zero" {
    feed 0
    result 0.0
}

@test "Bunch of zeros" {
    feed 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    result 0.0
}

@test "Non-adjacent average" {
    feed 1 3 6 7
    result 4.5
}

@test "Two numbers on the median spot" {
    feed 1 3 3 7
    result 3.0
}

@test "Three numbers on the median spot" {
    feed 0 4 4 4 7
    result 4.0
}

@test "Negative numbers" {
    feed -1
    result -1.0
    feed -2
    result -1.5
    feed -3
    result -2.0
    feed -4
    result -2.5
}

@test "Crossing zero downwards" {
    feed 1
    result 1.0
    feed 0
    result 0.5
    feed -1
    result 0.0
    feed -2
    result -0.5
    feed -3
    result -1.0
}

@test "Crossing zero upwards" {
    feed -1
    result -1.0
    feed 0
    result -0.5
    feed 1
    result 0.0
    feed 2
    result 0.5
    feed 3
    result 1.0
}

