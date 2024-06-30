BIN = ./bin
NAMEPROGAN = main

all:
	g++ main.cpp -o ${BIN}/${NAMEPROGAN}

run: 
	${BIN}/${NAMEPROGAN}

clear:
	rm ./bin/*