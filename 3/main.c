#include <stdio.h>
#include <math.h>

int main ( void ) {
 long long int startA = 0, startB = 0, msgA = 0, msgB = 0, i = 0;
 int loadStA = 1, loadStB = 0, loadMsgA = 0, loadMsgB = 0;
 char tmp;
 printf("Zpravy:\n");
 while (scanf("%c", &tmp) == 1) {
  if ((tmp < 97 && tmp != 10) || tmp > 124 || tmp == 123) {
   printf("Nespravny vstup.\n");
   return 1;
  }
  if (loadStA) {
   if (tmp == 124) {
    msgA = startA;
    loadStA = 0;
    loadMsgA = 1;
    continue;
   }
   startA += pow(2, (tmp-97));
  } else if (loadMsgA) {
   if (tmp == 10) {
    loadMsgA = 0;
    loadStB = 1;
    continue;
   }
   if (tmp == 124) {
    printf("Nespravny vstup.\n");
    return 1;
   }
   msgA += pow(2, (tmp-97));
  } else if (loadStB) {
   if (tmp == 124) {
    msgB = startB;
    loadStB = 0;
    loadMsgB = 1;
    continue;
   }
   startB += pow(2, (tmp-97));
  } else if (loadMsgB) {
   if (tmp == 10) {
    loadMsgB = 0;
    continue;
   }
   if (tmp == 124) {
    printf("Nespravny vstup.\n");
    return 1;
   }
   msgB += pow(2, (tmp-97));
  }
 }
 if (msgA == 0 || msgB == 0) {
   printf("Nespravny vstup.\n");
   return 1;
 }
 if ((startA == msgA && startB == msgB) || (startA == msgA && startB == 0) || (startB == msgB && startA == 0) ) {
   printf("Synchronizace za: 0\n");
   return 0;
 }
 if (startA == startB) {
   printf("Synchronizace za: %lld\n", startA);
   return 0;
 }
 if (msgA == msgB || (startA % 2 == 0 && msgA % 2 == 0 && startB % 2 == 1 && msgB % 2 == 0) || (startB % 2 == 0 && msgB % 2 == 0 && startA % 2 == 1 && msgA % 2 == 0)) {
   printf("Nelze dosahnout.\n");
   return 0;
 }
 while (startA != startB) {
  ++i;
  if (startA < startB) {
   startA += msgA;
  } else {
   startB += msgB;
  }
  if (i>100000000) {
   printf("Nelze dosahnout.\n");
   return 0;
  }
 }
 printf("Synchronizace za: %lld\n", startA);
 return 0;
}
