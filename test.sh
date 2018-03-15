#!bin/bash
echo "+-------------+"
echo "|    Make     |"
echo "+-------------+"
make
echo "+-------------+"
echo "|    Tests    |"
echo "+-------------+"
for i in {0..6}
do
	echo "test$i.src"
	./compiler tests/test$i.src
done

echo "+-------------+"
echo "|  Examples   |"
echo "+-------------+"
for file in ./examples/*.src
do
	echo "+--------------------"
	echo "| "$file
	echo "+--------------------"
	./compiler $file
done
echo "done"