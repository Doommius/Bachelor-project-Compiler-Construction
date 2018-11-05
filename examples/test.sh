#!bin/bash
echo "+-------------+"
echo "|    Make     |"
echo "+-------------+"
make
echo "+-------------+"
echo "|    Tests    |"
echo "+-------------+"
# for file in ./tests/*/*.src
# do
# 	echo "+--------------------"
# 	echo "| TEST: "$file
# 	echo "+--------------------"
# 	./compiler -p 1 $file
# 	echo " "
# done

echo "+-------------+"
echo "|  Examples   |"
echo "+-------------+"
let "passed = 0"
let "files = 0"
for file in examples/*.src
do
	let "files = files + 1"
	echo "+--------------------"
	echo "| EXAMPLE: "$file
	echo "+--------------------"
	./compiler -o out/ -a -f $file
	y=${file%.*}
	if [ -f $y.out ]; then
		diff <(timeout 10s ./out/${y##*/}.out) $y.out
		let "eq = $(echo $?)"
		#echo "eq: $(echo $?)" 
		if [ $eq = "0" ]; then
			echo "+-----"
			echo "| " $file " has passed!"
			echo "+-----"
			let "passed = passed + 1"
		else 
			echo "+-----"
			echo "| " $file " has NOT passed!"
			echo "+-----"
		fi
	else 
		let "passed = passed + 1"
	fi
	

done
echo $passed "/" $files " has passed."
echo "done"