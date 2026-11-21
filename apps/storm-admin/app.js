const NODE_SIZE = 16;
const TAG_OFFSET = 0;
const PAYLOAD_OFFSET = 12;
const LEFT_OFFSET = 4;
const RIGHT_OFFSET = 8;

const ATOM_NUMBER = 1;
const ATOM_STRING = 2;

const nodeToDomMap = new Map();
let wasm;
let root;
let arenaBase;
let objectPoolBase;
let dirtyNodesBase;
let page_root;

async function run() {
    const { instance } = await WebAssembly.instantiateStreaming(
        fetch('./arena.wasm')
    );

    wasm = instance.exports;
    wasm.init_arena();

    root = document.getElementById('app');
    arenaBase = wasm.get_arena_base();
    page_root = wasm.get_active_page_root();
    //console.log(page_root);
    objectPoolBase = wasm.get_object_pool_ptr();
    dirtyNodesBase = wasm.get_dirty_nodes_ptr();
    //console.log(arenaBase);
    //const atoms = createTopologiesList(0,3)
    //console.log(atoms);
    //renderTemplate(atoms);
    routeToPage(page_root);
    //bindNodes(root);

    // for (const nodeId of nodeToDomMap.keys()) {
    //     renderNode(nodeId);
    // }
}
function createTopologiesList(start, end){
    let atoms = [];
    for (let i = start;i < end; i++){
	atoms.push(readNode(i));
    }
    return atoms;
}

 function renderTemplate(atoms){

     const tpl = document.getElementById("topologies-page");
     let clone = tpl.content.cloneNode(true);
     const ul = clone.querySelector('[data-role="ul-1"]');
     console.log(ul);
     for (let i = 0;i < 3; i++){
	 const content = readString(atoms[i].payload);
	 
	 const li = document.createElement('li');
	 const a = document.createElement('a');
	 a.setAttribute('href',"http://localhost:9090/");
	 a.setAttribute('data-topology-id',`${i}`);
	 a.textContent = content;
	 a.addEventListener('click', (e) =>{
	     e.preventDefault();
	     console.log("id is " + e.currentTarget.dataset.topologyId);
	     wasm.change_root_page(parseInt(e.currentTarget.dataset.topologyId));
	     let apr = wasm.get_active_page_root();
	     console.log(apr);
	     console.log(readNode(apr));
	     routeToPage(apr);
	     
	 })
	 li.append(a);
	 ul.append(li);
     }

     const parent = document.getElementById("workspace");
     parent.replaceChildren(clone);

 }

//simplified
function routeToPage(root){
    let r_node= readNode(root);
    let template_atom = readNode(r_node.left);
    switch(template_atom.payload){
    case 100:{
	console.log("main page")

	break;
    }
    case 111:{
	console.log("topology page");
	let atom = readNode(r_node.right);
	renderTopologyTemplate(atom);
	break
    }
    default: {
	console.log("main page no root id");
	const atoms = createTopologiesList(0,3);
	renderTemplate(atoms);
    }
    }
    
    
}


 function renderTopologyTemplate(atom){

     const tpl = document.getElementById("topology-page");
     let clone = tpl.content.cloneNode(true);
     const h2 = clone.querySelector('[data-role="h2-topology-name"]');
     const h3 = clone.querySelector('[data-role="h3-topology-id"]');
     h2.textContent = "Topology Name: const test"
     h3.textContent = "Topology Id: const 1"
     const btn1 = clone.querySelector('[data-role="btn-1-main-topology"]');
     btn1.addEventListener('click', ()=> {
	 wasm.change_root_page(-1);
	 let apr = wasm.get_active_page_root();
	 console.log(`now active page is ${apr}`);
	 routeToPage(apr);
     });

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
        payload: view.getUint32(PAYLOAD_OFFSET, true),
	left: view.getUint32(LEFT_OFFSET, true),
	right: view.getUint32(RIGHT_OFFSET, true)
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
