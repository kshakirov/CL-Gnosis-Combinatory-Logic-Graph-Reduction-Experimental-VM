CLANG ?= clang
NODE ?= node
PYTHON ?= python3

WASM := arena.wasm
SOURCE := arena.c

CFLAGS := --target=wasm32 -std=c23 -O2 -Wall -Wextra -Wpedantic -Werror -nostdlib
WASM_LDFLAGS := \
	-Wl,--no-entry \
	-Wl,--export=init_arena \
	-Wl,--export=increment_node_42 \
	-Wl,--export=get_dirty_count \
	-Wl,--export=get_dirty_nodes_ptr \
	-Wl,--export=get_node_tag \
	-Wl,--export=get_node_payload \
	-Wl,--export-memory

.PHONY: all check serve clean

all: $(WASM)

$(WASM): $(SOURCE)
	$(CLANG) $(CFLAGS) $(WASM_LDFLAGS) $< -o $@

check: $(WASM)
	$(NODE) -e 'const fs = require("fs"); const bytes = fs.readFileSync("$(WASM)"); const module = new WebAssembly.Module(bytes); const { exports: wasm } = new WebAssembly.Instance(module, {}); wasm.init_arena(); if (wasm.get_node_payload(42) !== 100 || wasm.get_dirty_count() !== 0) throw new Error("invalid initial state"); wasm.increment_node_42(); const dirty = new Uint32Array(wasm.memory.buffer, wasm.get_dirty_nodes_ptr(), wasm.get_dirty_count()); if (wasm.get_node_payload(42) !== 101 || dirty.length !== 1 || dirty[0] !== 42) throw new Error("invalid increment state"); console.log("WASM check passed: payload=101, dirty_nodes=[42]");'

serve: $(WASM)
	$(PYTHON) -m http.server 8000

clean:
	rm -f $(WASM)
