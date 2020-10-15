BINARY=bin
CODEDIRS=.
INCDIRS=.

CC=gcc
OPT=-O0
# generate files that encode make rules for the .h dependencies
DEPFLAGS=-MP -MD
LIBS=-lpthread
# automatically add the -I onto each include directory
CFLAGS=-Wall -Wextra -Wno-unused-but-set-variable -Wno-unused-label -Wno-discarded-qualifiers -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS)

# for-style iteration (foreach) and regular expression completions (wildcard)
CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.c))
# regular expression replacement
OBJECTS=$(patsubst %.c,%.o,$(CFILES))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

all: $(BINARY)

test: $(BINARY)
	sh test.sh

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

# only want the .c file dependency here, thus $< instead of $^.
#
%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

# shell commands are a set of keystrokes away
distribute: clean
	tar zcvf dist.tgz *

# @ silences the printing of the command
# $(info ...) prints output
diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

# include the dependencies
-include $(DEPFILES)
