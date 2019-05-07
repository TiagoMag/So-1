#! /bin/bash

for x in {1..6};
do
	./cv <cmd.txt>saida"$x".txt &
done
wait