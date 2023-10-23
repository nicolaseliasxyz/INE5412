#include "FIFO.h"

bool FIFO::search(int key) {
    return ram_.isPageInRAM(key);
}

void FIFO::referencePage(int page) {
    // Verifica se a página já está presente na RAM.
    // Se true -> "page hit".
    if (not search(page)) {
        ++pageFaults;
        // Se false -> "page miss".
        if (pagesQueue.size() == ram_.getFrameCapacity()) {
            // Se estiver cheia, remove a página mais antiga da RAM.
            auto oldest = pagesQueue.front(); // 'front()' para obter o primeiro elemento da fila em FIFO.
            pagesQueue.pop();
            ram_.eraseFrame(oldest); // Remove a página mais antiga da RAM.
        }
        // Adiciona a nova página à fila e também à RAM.
        pagesQueue.push(page);
        ram_.referenceFrame(page);
    }
}
