CC=g++
CFLAGS=-Wall -g -O1 --pedantic
#-std=c++11

src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

#LDFLAGS = -lGL -lglut -lpng -lz -lm

popSim: $(obj)
	$(CC) $(CFLAGS) -o $@ $^ #$(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) mygame

.PHONY: cleandep
cleandep:
	rm -f $(dep)
