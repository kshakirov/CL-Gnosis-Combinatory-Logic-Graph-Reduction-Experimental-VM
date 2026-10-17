CLANG ?= clang
NODE ?= node
PYTHON ?= python3

WASM := arena.wasm
SOURCE := arena.c

CFLAGS := --target=wasm32 -std=c23 -O2 -Wall -Wextra -Wpedantic -Werror -Wno-unused-variable -nostdlib
WASM_LDFLAGS := \
	-Wl,--no-entry \
	-Wl,--export=init_arena \
	-Wl,--export=get_active_page_root \
	-Wl,--export=change_root_page \
	-Wl,--export=get_node_tag \
	-Wl,--export=get_node_left \
	-Wl,--export=get_node_right \
	-Wl,--export=get_node_payload \
	-Wl,--export-memory

.PHONY: all check serve clean

all: $(WASM)

$(WASM): $(SOURCE)
	$(CLANG) $(CFLAGS) $(WASM_LDFLAGS) $< -o $@

check: $(WASM)
	$(NODE) -e 'const fs = require("fs"); const bytes = fs.readFileSync("$(WASM)"); const module = new WebAssembly.Module(bytes); const { exports: wasm } = new WebAssembly.Instance(module, {}); wasm.init_arena(); const node = id => [wasm.get_node_tag(id), wasm.get_node_left(id), wasm.get_node_right(id), wasm.get_node_payload(id)]; const expected = new Map([[0,[0,0,0,0]],[1,[0,0,0,1]],[2,[0,0,0,2]],[3,[0,0,0,3]],[4,[0,0,0,4]],[5,[0,0,0,5]],[6,[0,0,0,6]],[7,[0,0,0,7]],[8,[1,0,1,0]],[9,[1,8,2,0]],[10,[1,9,3,0]],[11,[1,4,5,0]],[12,[1,11,6,0]],[13,[1,12,7,0]]]); for (const [id, value] of expected) { const actual = node(id); if (actual.some((item, index) => item !== value[index])) throw new Error(`invalid node $${id}: $${actual}`); } if (wasm.get_active_page_root() !== 10) throw new Error("invalid active page root"); console.log("WASM pages check passed: roots=10,13; active=10");'

serve: $(WASM)
	$(PYTHON) -m http.server 8000 --directory ..

clean:
	rm -f $(WASM)
