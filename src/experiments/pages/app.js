const PAGE_A = 0;
const PAGE_B = 1;

async function run() {
    const { instance } = await WebAssembly.instantiateStreaming(
        fetch('./arena.wasm')
    );
    const wasm = instance.exports;
    const root = document.getElementById('app');

    function render() {
        const page = wasm.get_current_page();
        const templateId = page === PAGE_A ? 'page-a' : 'page-b';
        const template = document.getElementById(templateId);

        root.replaceChildren(template.content.cloneNode(true));
        root.querySelector('[data-role="info"]').textContent =
            `Текущая страница хранится в WASM: ${page === PAGE_A ? 'A' : 'B'}`;
        root.querySelector('[data-role="show"]').addEventListener('click', () => {
            window.alert(page === PAGE_A ? 'A' : 'B');
        });
        root.querySelector('[data-role="navigate"]').addEventListener('click', () => {
            wasm.navigate();
            render();
        });
    }

    wasm.init_arena();
    render();
}

run().catch(console.error);
