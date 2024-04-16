//
// Created by lemse on 11/04/2024.
//

#include <stdlib.h>

#include "assembler.h"

int main(void){
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
    printf("\n Start of if");
    if(moveToFound(findCardByProp(&columns[6],'A','D'),&foundations[0])){
        printf("\n Moved");
    } else {
        printf("\n Not Moved");
    }
    printf("\n End of if");
    printColumns(columns, foundations);
}
