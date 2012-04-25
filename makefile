PAWPRINT=lappp9
# set up linker options
LDLIBS= -lncurses

game: 

clean:
	rm -f *.o

test: game

nuke:
	rm -rf *.o game *.gz dist

submit.sh:
	curl -L --output submit.sh http://www.ryannedolan.info/teaching/cs4520/projects/submit.sh
	chmod +x submit.sh

game.tar.gz: game.c makefile submit.sh 
	mkdir -p dist
	cp $^ dist
	tar -cvzf $@ dist

submit: submit.sh game.tar.gz
	PAWPRINT=$(PAWPRINT) sh submit.sh game.tar.gz
