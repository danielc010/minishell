#!/bin/bash

#!/bin/bash

PASS=0
FAIL=0

run_test()
{
    input="$1"
    expected_out=$(echo "$input" | bash 2>/tmp/bash_err)
    expected_exit=$?
    actual_out=$(echo "$input" | ./minishell 2>/tmp/mini_err | grep -v "^minishell>" | sed 's/minishell> *$//g' | sed 's/minishell>//g')
    actual_exit=$?
    if [ "$expected_out" = "$actual_out" ] && [ "$expected_exit" = "$actual_exit" ]; then
        echo "✅ PASS: $input"
        PASS=$((PASS + 1))
    else
        echo "❌ FAIL: $input"
        echo "   expected out: [$expected_out]"
        echo "   actual out:   [$actual_out]"
        echo "   expected exit: $expected_exit"
        echo "   actual exit:   $actual_exit"
        FAIL=$((FAIL + 1))
    fi
}

run_test 'echo bonjour'
run_test 'echo bonjour | cat'
run_test 'echo $HOME'
run_test 'echo "$HOME"'
run_test "echo '\$HOME'"
run_test 'echo $?'
run_test 'echo'
run_test 'echo simple'
run_test 'echo -n simple'
run_test "echo ''"
run_test 'echo ""'
run_test 'echo "\n \n \n"'
run_test 'echo -n -n -nnnn'
run_test 'echo hi";" hihi'
run_test 'echo hi " ; " hihi'
run_test 'echo $'
run_test 'echo "$"'
run_test "echo '\$'"
run_test 'echo $NOEXISTE'
run_test '/bin/echo bonjour'
run_test '/bin/ls'
run_test 'cd /'
run_test 'cd .'
run_test 'cd /tmp'
run_test 'cd no_file'
run_test 'pwd'
run_test 'pwd a'
run_test 'pwd a b c d'
run_test 'cat /etc/hostname'
run_test 'cat diufosgid'
run_test 'ls | cat'
run_test 'ls | grep src'
run_test 'cat /etc/hostname | cat'
run_test 'echo bonjour | cat'
run_test 'echo 2 >> /tmp/out1 > /tmp/out2'
run_test 'echo 2 > /tmp/out1 >> /tmp/out2'
run_test 'echo > /tmp/testfile Hello'
run_test '> /tmp/testfile echo Hello'
run_test 'exit 0'
run_test 'exit -10'
run_test 'exit +10'
run_test 'export HELLO=123'
run_test 'export HELLO=123 A'
run_test 'unset HELLO'
run_test 'export A-'
run_test 'export 123'
run_test 'export ='
run_test 'unset'
run_test 'not_cmd bonjour'
run_test 'echo bonjour > /tmp/test1'
run_test 'cat < /tmp/test1'

echo ""
echo "PASS: $PASS / FAIL: $FAIL"