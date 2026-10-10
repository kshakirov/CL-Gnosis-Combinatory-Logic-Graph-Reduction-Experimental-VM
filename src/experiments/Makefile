PYTHON ?= python3
EXPERIMENTS := memory pages

.PHONY: all check serve clean $(EXPERIMENTS)

all: $(EXPERIMENTS)

$(EXPERIMENTS):
	$(MAKE) -C $@

check:
	@for experiment in $(EXPERIMENTS); do \
		$(MAKE) -C $$experiment check || exit $$?; \
	done

serve: all
	$(PYTHON) -m http.server 8000

clean:
	rm -f arena.wasm
	@for experiment in $(EXPERIMENTS); do \
		$(MAKE) -C $$experiment clean || exit $$?; \
	done
