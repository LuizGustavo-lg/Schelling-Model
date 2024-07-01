BIN = ./bin
NAMEPROGAN = SchellingModel

all:
	g++ main.cpp -o ${BIN}/${NAMEPROGAN}

run: 
	${BIN}/${NAMEPROGAN}

clear:
	rm ./bin/*