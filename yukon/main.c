#include <stdlib.h>

#include "src/cardList.h"
#include "src/gameLogic.h"
#include "src/utils.h"

int main(void ){
    int numberOfColumns = 7;
    int numberOfFoundations = 4;
    Card *columns[numberOfColumns];
    Card *foundations[numberOfFoundations];

    for(int i = 0; i < numberOfColumns; i++){
        columns[i] = NULL;
    }
    for(int i = 0; i < numberOfFoundations; i++){
        foundations[i] = NULL;
    }

    readDeck("unshuffled.txt", columns);

    printColumns(columns, foundations);

    int srcColumn = 7;
    int dstColumn = 6;
    //moveCard(createCard(2,'H'),&columns[dstColumn-1]);
    //printColumns(columns);
    moveCard(findCardByProp(&columns[srcColumn-1],'8','D'), &columns[dstColumn-1]);
    printColumns(columns, foundations);
}
