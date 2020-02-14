prefix=/usr
    
all:
	cc -g -lm src/*.c -o passgen
	
install: passgen
	install -m 0755 passgen $(prefix)/bin
	
uninstall: all
	rm $(prefix)/bin/passgen
