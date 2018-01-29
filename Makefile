P = greedy-superstring
SOURCES = overlap_matrix.c

default: clean $(P)

$(P):
	gcc -o $@ $(SOURCES)
	@echo "Usage: greedy-superstring <input_file>"

clean:
	rm -f *.o *.out $(P)

.PHONY: default clean