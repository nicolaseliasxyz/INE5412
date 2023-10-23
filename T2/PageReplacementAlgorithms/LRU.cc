#include "LRU.h"
#include <algorithm>

bool LRU::search(int key) {
    return ram_.isPageInRAM(key);
}

void LRU::referencePage(int page) {
    if (search(page)) {
        // -> "Page hit".
        // Se a página já está na RAM (page hit), movemos ela para a frente da lista.
        auto it = std::find(pageList.begin(), pageList.end(), page);
        // So movemos caso ela ja nao seja o comeco a lista
        if(it != pageList.begin()) {
            pageList.erase(it);
            pageList.push_front(page);
        }
    } else {
        // -> "Page miss".
        ++pageFaults;
        if (pageList.size() == ram_.getFrameCapacity()) {
            // Se a RAM estiver cheia, removemos a página mais antiga.
            int oldest = pageList.back();
            ram_.eraseFrame(oldest);
            pageList.pop_back();
        }

        // Adicionamos a nova página à RAM e à frente da lista.
        pageList.push_front(page);
        ram_.referenceFrame(page);
    }
}
