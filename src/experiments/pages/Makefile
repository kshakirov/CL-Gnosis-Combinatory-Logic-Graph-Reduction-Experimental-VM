CLANG ?= clang
NODE ?= node
PYTHON ?= python3

WASM := arena.wasm
SOURCE := arena.c

CFLAGS := --target=wasm32 -std=c23 -O2 -Wall -Wextra -Wpedantic -Werror -nostdlib
WASM_LDFLAGS := \
	-Wl,--no-entry \
	-Wl,--export=init_arena \
	-Wl,--export=navigate \
	-Wl,--export=get_current_page \
	-Wl,--export-memory

.PHONY: all check serve clean

all: $(WASM)

$(WASM): $(SOURCE)
	$(CLANG) $(CFLAGS) $(WASM_LDFLAGS) $< -o $@

check: $(WASM)
	$(NODE) -e 'const fs = require("fs"); const bytes = fs.readFileSync("$(WASM)"); const module = new WebAssembly.Module(bytes); const { exports: wasm } = new WebAssembly.Instance(module, {}); wasm.init_arena(); if (wasm.get_current_page() !== 0) throw new Error("invalid initial page"); wasm.navigate(); if (wasm.get_current_page() !== 1) throw new Error("invalid page after first navigation"); wasm.navigate(); if (wasm.get_current_page() !== 0) throw new Error("invalid page after second navigation"); console.log("WASM pages check passed: A -> B -> A");'

serve: $(WASM)
	$(PYTHON) -m http.server 8000 --directory ..

clean:
	rm -f $(WASM)
