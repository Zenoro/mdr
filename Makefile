PROG=mdr
$(PROG): text.o  header.o simple_text.o code.o lists.o wrapper.o main.o 
	g++ $^ -g -o $@
text.o: text.cpp text.h
	g++ -c -g $< -o $@
header.o: header.cpp header.h
	g++ -c -g $< -o $@
simple_text.o: simple_text.cpp simple_text.h
	g++ -c -g $< -o $@
lists.o: lists.cpp lists.h
	g++ -c -g $< -o $@
code.o: code.cpp code.h
	g++ -c -g $< -o $@
wrapper.o: wrapper.cpp wrapper.h
	g++ -c -g $< -o $@
main.o: main.cpp
	g++ -c -g $< -o $@
clean:
	rm -f *.o $(PROG)
