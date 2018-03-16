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
	./compiler $file
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