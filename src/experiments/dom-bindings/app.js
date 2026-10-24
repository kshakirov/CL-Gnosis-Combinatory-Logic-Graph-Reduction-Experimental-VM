const nodeToDomMap = {};
async function run() {
    const { instance } = await WebAssembly.instantiateStreaming(
        fetch('./arena.wasm')
    );
    
    const wasm = instance.exports;
    wasm.init_arena();
    nodeToDomMap[1] = document.querySelector(`[data-node-id="1"]`);
    nodeToDomMap[2] = document.querySelector(`[data-node-id="2"]`);
    console.log(nodeToDomMap);
}

run().catch(console.error);
