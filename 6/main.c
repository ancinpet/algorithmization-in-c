#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_ALLOC 16
#define COL_ALLOC 16

void freeMem ( int ** data, size_t * colManager, int rowManager ) {
 int e;
 for ( e = 0; e < rowManager; e++ )
  free(data[e]);

 free(data);
 free(colManager);
}

int processData ( void ) {
 /* data je 2D pole, pocet radku = pocet frekvenci, kazdy sloupec ma na 0. indexu frekvenci
  * letadla a od 1. indexu sekvenci dat letadla.
  * columnManager uchovava pocet sloupcu na kazdem radku.
  * rowManager uchovava pocet radek (frekvenci) pole. */
 int **data = NULL;
 size_t *columnManager = NULL, rowManager = 0;
 /* Pomocne promenne */
 int scanNum, res, l, j, k, n;
 /* max pocet radek, max pocet sloupcu radky, index aktualni radky */
 size_t maxRowMem = 0, maxColMem = 0, curRow = 0, i, oldRowMem = 0, e, f;
 char scanChar, addedRow = 0;

 /* Nacita cislo a non-whitespace znak, pri EOF je res = -1.
  * Cislo a znak nenastanou pouze pri 'cislo : [' - osetreno podminkou uvnitr. */
 while ( ( res = scanf( "%d %c", &scanNum, &scanChar ) ) == 2 ) {
 /* Znak je dvojtecka - cislo pred ni je frekvence letadla, musime se podivat jestli
  * frekvence je na seznamu, jestli neni, musime naalokovat pamet pro novou radku */
  if ( scanChar == ':' ) {
   maxColMem = 0;
   addedRow = 0;
 /* Zkontrolujeme jestli nasleduje '[' a sezereme ji pres scanf aby se dalo pokracovat
  * pres sekvenci 'cislo znak' */
   if ( scanf(" %c ", &scanChar) != 1 || scanChar != '[' || scanNum < 0 ) {
    freeMem(data, columnManager, rowManager);
    return 1;   
   }
 /* Projed radkama a zkontroluj jestli uz nemame zpravu z teto frekvence, jestli ne, pridej
  * novou radku, curRow bude index zapisovaci radky a rowManager celkovy pocet radek */
   curRow = rowManager;
   for (i = 0; i < curRow; i++)
    if ( data[i][0] == scanNum )
     curRow = i;

   if ( curRow == rowManager ) {
    ++rowManager;
    addedRow = 1;
   }
 /* Dynamicka alokace radek po ROW_ALLOC clenech do zasoby */
   if ( curRow >= maxRowMem ) {
    oldRowMem = maxRowMem;
    maxRowMem += ROW_ALLOC;
    data = (int**)realloc( data, maxRowMem * sizeof(*data) );
    if ( !data ) {
     freeMem(data, columnManager, rowManager);
     return 1;
    }
    columnManager = (size_t*)realloc( columnManager, maxRowMem * sizeof(*columnManager) );
    if ( !columnManager ) {
     freeMem(data, columnManager, rowManager);
     return 1;
    }
    memset(columnManager + oldRowMem, 0, (maxRowMem - oldRowMem) * sizeof(*columnManager));
   }
 /* Prvni dynamicka alokace sloupce po COL_ALLOC clenech do zasoby a zapis frekvence */
   if ( addedRow ) {
    maxColMem += COL_ALLOC;
    data[curRow] = (int*)malloc( maxColMem * sizeof(*data[curRow]) );
    if ( !data[curRow] ) {
     freeMem(data, columnManager, rowManager);
     return 1;
    }
    data[curRow][columnManager[curRow]] = scanNum;
    ++columnManager[curRow];
   }
  }
  else if ( scanChar == ',' || scanChar == ']' ) {
 /* Dynamicka alokace sloupce po COL_ALLOC clenech do zasoby */
   if ( columnManager[curRow] >= maxColMem ) {
    maxColMem = columnManager[curRow] + COL_ALLOC;
    data[curRow] = (int*)realloc( data[curRow], maxColMem * sizeof(*data[curRow]) );
    if ( !data[curRow] ) {
     freeMem(data, columnManager, rowManager);
     return 1;
    }
   }
   data[curRow][columnManager[curRow]] = scanNum;
   ++columnManager[curRow];
  }
  else {
   freeMem(data, columnManager, rowManager);
   return 1;
  }
 }

 if ( res != -1 ) {
  freeMem(data, columnManager, rowManager);
  return 1;
 }

 for ( e = 0; e < rowManager; e++ )
  if ( columnManager[e] < 2 ) {
   freeMem(data, columnManager, rowManager);
   return 1;
  }
 
 printf( "Unikatni zpravy:\n" );


 for ( e = 0; e < rowManager; e++ ) {
  if ( data[e][0] != -5 )
   printf("%d", data[e][0]);
  for ( f = e + 1; f < rowManager; f++ ) {
   if ( data[f][0] != -5 && columnManager[e] == columnManager[f] ) {
    n = columnManager[e] - 1, l = -1, j = -1;
    while( l < n-1 && j < n-1 )
    { 
     k = 1;
     while(k<=n && data[e][1+(l+k)%n]==data[f][1+(j+k)%n])
      k++;

     if (k>n) {
      printf(", %d", data[f][0]);
      data[f][0] = -5;
     }
     if (data[e][1+(l+k)%n] > data[f][1+(j+k)%n]) {
      l += k;
     }
     else {
      j += k;
     } 
    }
   }   
  }
  if ( data[e][0] != -5 )
  printf("\n");
 }

 freeMem(data, columnManager, rowManager);

 return 0;
}

int main ( void ) {

 printf( "Zpravy:\n" );
 /* processData se pokusi nacist data v zadanem formatu, vraci 0 pri uspechu a 1 pri neuspechu */
 if ( processData() ) {
  printf( "Nespravny vstup.\n" );
  return 1;
 }
 


 return 0;
}
