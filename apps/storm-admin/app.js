const NODE_SIZE = 16;
const TAG_OFFSET = 0;
const PAYLOAD_OFFSET = 12;

const ATOM_NUMBER = 1;
const ATOM_STRING = 2;

const nodeToDomMap = new Map();
let wasm;
let root;
let arenaBase;
let objectPoolBase;
let dirtyNodesBase;

async function run() {
    const { instance } = await WebAssembly.instantiateStreaming(
        fetch('./arena.wasm')
    );

    wasm = instance.exports;
    wasm.init_arena();

    root = document.getElementById('app');
    arenaBase = wasm.get_arena_base();
    objectPoolBase = wasm.get_object_pool_ptr();
    dirtyNodesBase = wasm.get_dirty_nodes_ptr();
    console.log(arenaBase);
    renderTemplate();
    
    //bindNodes(root);

    // for (const nodeId of nodeToDomMap.keys()) {
    //     renderNode(nodeId);
    // }
}

 function renderTemplate(){

     const tpl = document.getElementById("topologies-page");
     let clone = tpl.content.cloneNode(true);
     const ul = clone.querySelector('[data-role="ul-1"]');
     console.log(ul);
     for (let i = 0;i < 3; i++){
	 const li = document.createElement('li');
	 li.textContent = 'new ' + i;
	 ul.append(li);
     }

     const parent = document.getElementById("workspace");
     parent.replaceChildren(clone);

 }


function bindNodes(scope) {
    nodeToDomMap.clear();

    for (const element of scope.querySelectorAll('[data-node-id]')) {
        const nodeId = Number(element.dataset.nodeId);

        if (!Number.isInteger(nodeId)) {
            throw new Error(`Invalid data-node-id: ${element.dataset.nodeId}`);
        }

        nodeToDomMap.set(nodeId, element);
    }
}

function readNode(nodeId) {
    const offset = arenaBase + nodeId * NODE_SIZE;
    const view = new DataView(wasm.memory.buffer, offset, NODE_SIZE);

    return {
        tag: view.getUint32(TAG_OFFSET, true),
        payload: view.getUint32(PAYLOAD_OFFSET, true)
    };
}

function readString(poolOffset) {
    const objectOffset = objectPoolBase + poolOffset;
    const length = new Uint8Array(wasm.memory.buffer, objectOffset, 1)[0];
    const bytes = new Uint8Array(wasm.memory.buffer, objectOffset + 1, length);

    return new TextDecoder().decode(bytes);
}

function renderNode(nodeId) {
    const element = nodeToDomMap.get(nodeId);

    if (!element) {
        return;
    }

    const node = readNode(nodeId);

    if (node.tag === ATOM_NUMBER) {
        element.textContent = String(node.payload);
    } else if (node.tag === ATOM_STRING) {
        element.textContent = readString(node.payload);
    }
}

function flushDirtyNodes() {
    const count = wasm.get_dirty_count();
    const dirtyNodes = new Uint32Array(
        wasm.memory.buffer,
        dirtyNodesBase,
        count
    );

    for (const nodeId of dirtyNodes) {
        renderNode(nodeId);
    }

    wasm.clear_dirty_nodes();
}

run().catch(console.error);
