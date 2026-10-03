const nodeMap = new Map();

const NODE_ATOM_NUMBER = 0;
const NODE_COMBINATION = 1;
const NODE_ATOM_STRING = 2;

async function run() {
    // 1. Загружаем Wasm-модуль
    const { instance } = await WebAssembly.instantiateStreaming(
	fetch('./arena.wasm')
    );
    const wasm = instance.exports;

    // 2. Инициализируем Си-Арену
    wasm.init_arena();
    const base = wasm.get_memory_base();
    // 3. Привязываем наблюдаемые узлы к DOM-элементам
    const btn = document.getElementById('btn-42');
    nodeMap.set(42, btn);

    const div1 = document.getElementById('div-1');
    nodeMap.set(1, div1);

    // Прямой точечный патч DOM
    function render() {
	const count = wasm.get_dirty_count();
	const ptr = wasm.get_dirty_nodes_ptr();
	const dirty = new Uint32Array(wasm.memory.buffer, ptr, count);
	const poolPtr = base + 1600000; //hardcoded for the time being
	const poolSize = 64 * 1024;//hardcoded too
	const pool = new Uint8Array(wasm.memory.buffer, poolPtr, poolSize);


	for (let i = 0; i < count; i++) {
	    const id = dirty[i];
	    const tag = wasm.get_node_tag(id);
	    const domNode = nodeMap.get(id);

	    if (!domNode) {
		continue;
	    }

	    switch (tag) {
            case NODE_ATOM_NUMBER: {
		const value = wasm.get_node_payload_number(id);
		domNode.textContent = `[Атом Number  #${id}] Значение: ${value}`;
		break;
            }

            case NODE_ATOM_STRING:
		// Строковый round-trip будет добавлен после завершения Wasm API.
		const length = wasm.get_node_payload_string_length(id);
		const offset = wasm.get_node_payload_string_pool(id);
		const bytes = new Uint8Array(
		    wasm.memory.buffer,
		    poolPtr + offset,
		    length
		);
		const text = new TextDecoder().decode(bytes);
		domNode.textContent = `[Атом String #${id}] Значение: ${length} text is ${text}`;
		break;

            case NODE_COMBINATION:
		break;
	    }
	}
    }

    // Стартовый рендер
    wasm.increment_node_42();
    render();

    // Клик мутирует Си-память -> Vanilla JS локально обновляет DOM
    btn.addEventListener('click', () => {
	wasm.increment_node_42();
	render();
    });
}

run().catch(console.error);
