const nodeToDomMap = {};


async function run() {
    const { instance } = await WebAssembly.instantiateStreaming(
        fetch('./arena.wasm')
    );
    
    const wasm = instance.exports;
    wasm.init_arena();
    const arena_base=wasm.get_arena_base(),
	  object_pool_ptr = wasm.get_object_pool_ptr(),
	  dirty_nodes_ptr = wasm.get_dirty_nodes_ptr();
    
    const root = document.getElementById("app");
    nodeToDomMap[1] = root.querySelector(`[data-node-id="1"]`);
    nodeToDomMap[2] = root.querySelector(`[data-node-id="2"]`);
    const initital_button_label = view_arena_node(wasm.memory.buffer, arena_base + 16,  16);
    nodeToDomMap[1].textContent= "Hello, bro " + initital_button_label + " times" ;
}

run().catch(console.error);


function view_arena_node(buffer, arena_base, nodeLength){
    const dv = new DataView(buffer, arena_base, nodeLength);
    return dv.getUint32(12, true);   // payload

}
