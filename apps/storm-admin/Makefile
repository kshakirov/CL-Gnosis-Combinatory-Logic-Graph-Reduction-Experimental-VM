CLANG ?= clang
NODE ?= node
PYTHON ?= python3

WASM := arena.wasm
SOURCE := arena.c

CFLAGS := --target=wasm32 -std=c23 -O2 -Wall -Wextra -Wpedantic -Werror -nostdlib
WASM_LDFLAGS := \
	-Wl,--no-entry \
	-Wl,--export=init_arena \
	-Wl,--export=get_arena_base \
	-Wl,--export=get_object_pool_ptr \
	-Wl,--export=get_dirty_nodes_ptr \
	-Wl,--export=get_dirty_count \
	-Wl,--export=clear_dirty_nodes \
	-Wl,--export-memory

.PHONY: all check serve clean

all: $(WASM)

$(WASM): $(SOURCE)
	$(CLANG) $(CFLAGS) $(WASM_LDFLAGS) $< -o $@

check: $(WASM)
	$(NODE) -e 'const fs = require("fs"); const bytes = fs.readFileSync("$(WASM)"); const module = new WebAssembly.Module(bytes); const { exports: wasm } = new WebAssembly.Instance(module, {}); wasm.init_arena(); if (wasm.get_dirty_count() !== 0) throw new Error("invalid initial dirty state"); console.log("Storm Admin WASM check passed");'

serve: $(WASM)
	$(PYTHON) -m http.server 8000

clean:
	rm -f $(WASM)
