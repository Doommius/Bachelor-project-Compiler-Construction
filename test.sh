#!bin/bash
echo "+-------------+"
echo "|    Make     |"
echo "+-------------+"
make
echo "+-------------+"
echo "|    Tests    |"
echo "+-------------+"
for file in ./tests/*/*.src
do
	echo "+--------------------"
	echo "| TEST: "$file
	echo "+--------------------"
	./compiler -p 1 $file
	echo " "
done

# echo "+-------------+"
# echo "|  Examples   |"
# echo "+-------------+"
# for file in ./examples/*.src
# do
# 	echo "+--------------------"
# 	echo "| EXAMPLE: "$file
# 	echo "+--------------------"
# 	./compiler $file
# done
echo "done"