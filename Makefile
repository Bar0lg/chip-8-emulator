CC := clang
CFLAGS := -Wall -Wextra -Iinclude/
DEBFLAGS := -fsanitize=address,undefined -g
CLIBS := -lm
NAME := exe


CFILES := $(shell find src -type f -name "*.c" | cut -d'/' -f 2-)
OBJFILES := $(patsubst %.c,%.o,$(addprefix build/objs/, $(CFILES)))
DIRS := src include data build build/objs


exe: $(OBJFILES) | $(DIRS)
	@$(CC) -o build/$(NAME) $^ $(CFLAGS) $(DEBFLAGS)
	@echo "$(CFLAGS) $(DEBFLAGS)" | sed 's/ /\n/g' > compile_flags.txt
	@cp -r data build/


run:exe
	@./build/$(NAME)


.SECONDEXPANSION:
$(OBJFILES): $$(patsubst %.o,%.c, $$(addprefix src/,$$(shell echo "$$@" | cut -d'/' -f 3-)))
	@mkdir -p $(dir $@)
	@$(CC) -c -o $@ $^ $(CFLAGS) $(DEBFLAGS)

clean:
	rm -rf build/


$(DIRS):
	@mkdir -p $@
