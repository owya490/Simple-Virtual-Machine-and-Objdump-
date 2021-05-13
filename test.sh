#!/bin/bash
count=0
for test in tests/*.out; do
	name=$(basename $test .out)
	arguments=tests/$name.x2017
	echo "Running test $name"
	./vm_x2017 $arguments | diff - $test || echo "Test $name: failed!"
	count=$((count+1))
done
echo "Finished running $count tests!"
