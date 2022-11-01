#include <stdio.h>
#include <string.h>

#define ROW 2000
#define COL 2000

int readInput ( int map[][COL], int width, int height ) {
 int i, j;
 for ( i = 0; i < height; i++ ) {
  for ( j = 0; j < width; j++ ) {
   if ( scanf("%d", &map[i][j]) != 1 ) {
    printf("Nespravny vstup.\n");
    return -1;
   }
  }
 }
 return 0;
}

int processEstate ( int map[][COL], int width, int height, int price, char out ) {
 int total = 0, rowS = 0, colS = 0, rowE = 0, colE = 0, row = 0, col = 0, i = 0, j = 0;

 for ( rowS = 0; rowS < height; rowS++) {
  for ( colS = 0; colS < width; colS++) {
   for ( rowE = 0; rowE < height; rowE++) {
    for ( colE = 0; colE < width; colE++) {
     total = 0;
     i = 0;
     for ( row = rowS; row <= rowE; row++) {
      for ( col = colS; col <= colE; col++) {
       total += map[row][col];
       ++i;
      }
     }
     if ( i > 0 && total == price) {
      ++j;
      if ( out )
       printf("%d @ (%d,%d) - (%d,%d)\n", price, colS, rowS, colE, rowE);
     }
    }
   }
  }
 }
 printf("Celkem: %d\n", j);
 return 0;
}

int queryRequest ( int map[][COL], int width, int height ) {
 char query[6];
 int price, res;
 while ( (res = scanf("%5s %d", query, &price)) == 2 ) {
  if ( strcmp(query, "list") == 0 ) {
   processEstate(map, width, height, price, 1);
  }
  else if ( strcmp(query, "count") == 0 ) {
   processEstate(map, width, height, price, 0);
  }
  else {
   return -1;
  }
 }
 if ( res == -1 )
  return 0;
 return -1;
}

int main ( void ) {
 static int map[ROW][COL];
 int width = 0, height = 0;
 printf("Velikost mapy:\n");
 if ( scanf("%d %d", &width, &height) != 2 || width < 1 || height < 1 || width > 2000 || height > 2000 ) {
  printf("Nespravny vstup.\n");
  return 1;
 }
 printf("Cenova mapa:\n");
 if ( readInput(map, width, height) == -1 ) {
  return 1;
 }
 printf("Dotazy:\n");
 if ( queryRequest(map, width, height) == -1 ) {
  printf("Nespravny vstup.\n");
  return 1;
 }
 return 0;
}
