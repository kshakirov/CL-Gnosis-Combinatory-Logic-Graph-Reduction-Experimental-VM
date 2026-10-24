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
    
    const h1_object_offset = view_arena_node(wasm.memory.buffer, arena_base + 16,  16);
  
    const initial_h1_value = view_string(wasm.memory.buffer, object_pool_ptr + h1_object_offset);
    nodeToDomMap[1].textContent= "bro " + initial_h1_value  ;
    const initital_button_label = view_arena_node(wasm.memory.buffer, arena_base + 32,  16);
    nodeToDomMap[2].textContent= "Clicked:  " + initital_button_label + " times"  ;


    window.clickIncrement = function(){
	wasm.increment_node(2);
	console.log("Hi");
	render_dirty_nodes(wasm.memory.buffer, arena_base, dirty_nodes_ptr,wasm.get_dirty_count())
	wasm.clean_dirty_nodes();
    }
  
}

run().catch(console.error);


function render_dirty_nodes(buffer, arena_base,  dirty_nodes_ptr, dirty_count){
    const dn = new Uint32Array(buffer,dirty_nodes_ptr,dirty_count);
    for(let i = 0; i  < dirty_count; i++){
	console.log(dn[i]);
	const value = view_arena_node(buffer, arena_base + 16*dn[i],  16);
	nodeToDomMap[dn[i]].textContent= "Clicked:  " + value + " times"  ;
    }
    

    
}

function view_arena_node(buffer, arena_base, byteLength){
    const dv = new DataView(buffer, arena_base, byteLength);
    return dv.getUint32(12, true);   // payload

}

function view_string(buffer, object_pool_ptr){
    const length = new Uint8Array(buffer,object_pool_ptr,1)
    const payload = new Uint8Array(buffer,object_pool_ptr + 1,length[0]);
    const decoder = new TextDecoder();
    return decoder.decode(payload);
    
}
