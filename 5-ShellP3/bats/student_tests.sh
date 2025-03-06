#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Check exit terminates shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -ne 0 ]
}

@test "Check multiple commands" {
    run ./dsh <<EOF
cd
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Check empty input" {
    run ./dsh <<EOF
EOF
    [ "$status" -eq 0 ]
}

@test "Check cd without arguments" {
    run ./dsh <<EOF
cd
EOF
    [ "$status" -eq 0 ]
}

@test "Check command with arguments" {
    run ./dsh <<EOF
ls -a
EOF
    [ "$status" -eq 0 ]
}

@test "Check handling of multiple spaces between commands" {
    run ./dsh <<EOF
ls     -a
EOF
    [ "$status" -eq 0 ]
}
