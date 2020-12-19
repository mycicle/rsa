CPP=g++
GMP=-lgmp
DEBUG=-g

all: init decrypt encrypt

init:
	$(CPP) $(DEBUG) src/init.cc $(GMP) -o init

decrypt:
	$(CPP) $(DEBUG) src/decrypt.cc $(GMP) -o decrypt

encrypt:
	$(CPP) $(DEBUG) src/encrypt.cc $(GMP) -o encrypt

clean:
	rm *.txt init decrypt encrypt