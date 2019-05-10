#tentativa de make, separei os ficheiros em pastas e está a dar erro, meti o ficheiro .sh nos include

ODIR = obj
IDIR = include
SDIR = src

CC = gcc
EXE = program

DEPS = $(IDIR)/$(wildcard*.h)
SOURCES = $(wildcard $(SDIR)/*.c)

OBJECTS = $(foreach o, $(patsubst %.c,%.o,$(SOURCES)), $(ODIR)/$o)

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< 

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE)

clean:
	rm $(ODIR)/$(SDIR)/*
	rm $(EXE)
