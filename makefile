# library source files and object destinations
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
HEADER = $(notdir $(SRC:.c=.h))
OBJDIR = obj
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

# test application files and object destinations
TESTDIR = test
TESTSRC = $(wildcard $(TESTDIR)/$(SRCDIR)/*.c)
TESTOBJ = $(addprefix $(TESTDIR)/$(OBJDIR)/, $(notdir $(TESTSRC:.c=.o)))
TEXEC = $(basename $(notdir $(TESTSRC)))

# library static vs dynamic
LIBOUT_STATIC = $(addprefix lib, $(notdir $(SRC:.c=.a)))
LIBOUT_DYNA = $(addprefix lib, $(notdir $(SRC:.c=.so)))

# generate documents
DOXYGEN_GEN = doxygen
DOXYGEN_CFG = dox.cfg

#$(addprefix -l,$(basename $(LIBS)))

CC = gcc
CFLAGS = -Wall -Ofast --std=c89
LFLAGS = -I. -fPIC -lpthread
TLFLAGS = -L. -I. -l$(notdir $(basename $(SRC))) -lpthread
DYNAFLAGS = -shared -fPIC
ARCHIVE = ar
AFLAGS = rcs

.PHONY: clean dox_gen

all: lib exe dox_gen

lib: $(LIBOUT_STATIC) $(LIBOUT_DYNA)

exe: $(TEXEC)

$(TEXEC): % : $(TESTDIR)/$(OBJDIR)/%.o $(LIBOUT_STATIC)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $^ -o $(TESTDIR)/$@ $(TLFLAGS)

$(TESTDIR)/$(OBJDIR)/%.o : $(TESTDIR)/$(SRCDIR)/%.c
	mkdir -p $(TESTDIR)/$(OBJDIR)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(TLFLAGS) -c $< -o $@

$(LIBOUT_DYNA)  : $(OBJECTS)
	$(CROSS_COMPILE)$(CC) $(DYNAFLAGS) $^ -o $@
	
$(LIBOUT_STATIC): $(OBJECTS)
	$(CROSS_COMPILE)$(ARCHIVE) $(AFLAGS) $@ $^
	
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@

dox_gen: 
	$(DOXYGEN_GEN) $(DOXYGEN_CFG) $(HEADER)
	
clean:
	rm -f $(TESTDIR)/$(TEXEC) $(OBJECTS) $(LIBOUT_STATIC) $(LIBOUT_DYNA)
	rm -rf $(OBJDIR) $(DOXYGEN_GEN) $(TESTDIR)/$(OBJDIR)
