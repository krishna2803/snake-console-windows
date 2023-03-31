# SHELL	:= pwsh
CC 		:= gcc
CFLAGS	:= -Ofast
IFLAGS	:= 
LFLAGS	:= 
LDFLAGS	:= 

BINDIR  := bin
SRCDIR  := src

MAIN	:= $(SRCDIR)/main.c
SRCS	:= $(filter-out $(MAIN), $(wildcard $(SRCDIR)/*.c))
OBJS 	:= $(addprefix $(BINDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS)))))

run: $(BINDIR)/main.exe
	.\bin\main.exe

$(BINDIR)/main.exe: $(MAIN)
	$(CC) $(MAIN) $(OBJS) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(LDFLAGS) -o $(BINDIR)/main.exe 

$(MAIN): $(OBJS)

$(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) $(IFLAGS) $(LFLAGS) $(LDFLAGS) -o $@ $(SRCDIR)/$(basename $(notdir $@)).c

.PHONY: clean
clean:
	del $(BINDIR) /f /q

