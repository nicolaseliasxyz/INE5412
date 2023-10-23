#include "OPT.h"
#include <algorithm>
#include <unordered_set>

bool OPT::search(int key) {
    return ram_.isPageInRAM(key);
}

int OPT::predict(int index) {
    int farthest = -1, pageToReplace;

    for (int page : pageSet) {
        auto it = std::find(futureReferences_.begin() + index, futureReferences_.end(), page);

        // Se a pagina não for encontrada nas futureReferences_, ela pode ser substituida
        if (it == futureReferences_.end()) {
            return page;
        } else {
            int distance = std::distance(futureReferences_.begin(), it);
            if (distance > farthest) {
                farthest = distance;
                pageToReplace = page;
            }
        }
    }

    return pageToReplace;
}

void OPT::referencePage(int page) {

    // Checa se a pagina ja esta na ram
    if (search(page)) {
        // -> Page Hit.
        // Mantive o if para caso quisse colocar um counter ou uma ação ao ter um page hit
    } else {
        // Se ram esta cheia pega a pagina atraves do algoritmo de predict
        if (pageSet.size() == ram_.getFrameCapacity()) {
            int pageToReplace = predict(currentIndex);
            ram_.eraseFrame(pageToReplace); // Remove the chosen page from RAM.
            pageSet.erase(pageToReplace); // Remove the chosen page from the set of pages.
        }

        ram_.referenceFrame(page);
        pageSet.insert(page);

        ++pageFaults;
    }

    // Move para a proxima referencia o index usado no predict.
    ++currentIndex;
}
