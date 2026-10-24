const PAGE_A = 0;
const PAGE_B = 4;

const PAGE_MAP = ['page-a',,,,'page-b'];


async function run() {
    const { instance } = await WebAssembly.instantiateStreaming(
        fetch('./arena.wasm')
    );
    const root = document.getElementById('app');
    const wasm = instance.exports;
    wasm.init_arena();
    window.navigatePage = function(){
	console.log("changing the root");
	wasm.change_root_page();
	let pageRoot = wasm.get_active_page_root();
	let atoms = collectAtoms(wasm, pageRoot);
	renderTemplate(root, atoms);

    };

    window.testDirtyNodes = function(){
	console.log("Dirty nodes testing ");
	const infoElement = document.querySelector('[data-role="info"]');
	infoElement.textContent = "Help me";

    };


    const pageRoot = wasm.get_active_page_root();

    const ATOM = 0;
    const COMBINATION = 1;


    function collectAtoms(wasm, nodeId, result = []) {
	const tag = wasm.get_node_tag(nodeId);

	if (tag === ATOM) {
//	    result.push(wasm.get_node_payload(nodeId));
	    result.push(nodeId);             // индекс узла
	    return result;
	}

	if (tag === COMBINATION) {
	    collectAtoms(wasm, wasm.get_node_left(nodeId), result);
	    collectAtoms(wasm, wasm.get_node_right(nodeId), result);
	    return result;
	}

	throw new Error(`Unknown node tag: ${tag}`);
    }

    function render() {

    }


    function renderTemplate(parent,atoms){

	let tpl = document.getElementById(PAGE_MAP[atoms[0]]);
	console.log("render template " + atoms[0 ] + " " + PAGE_MAP[4]);

	let clone = tpl.content.cloneNode(true);

	const nodeMap = new Map();
	const infoElement = clone.querySelector('[data-role="info"]');
	const showButton = clone.querySelector('[data-role="show"]');
	const navigateButton = clone.querySelector('[data-role="navigate"]');
	nodeMap.set(atoms[1], infoElement);
	nodeMap.set(atoms[2], showButton);
	nodeMap.set(atoms[3], navigateButton);

	parent.replaceChildren(clone);

    }


    let pageAtoms = collectAtoms(wasm,pageRoot);
    console.log(pageAtoms);
    renderTemplate(root, pageAtoms);
}

run(). catch(console.error);
