GCC=gcc

#GCCFLAGS= -std=c23 -Wall -Wextra -Wpedantic -Werror -O2 
# on purpose duplicated to rewrite and not to forget about the first one
GCCFLAGS= -std=c23 -Wall  -O2 -lxxhash -DDSS_DEBUG

all:
	$(GCC) main.c   -o main $(GCCFLAGS)

