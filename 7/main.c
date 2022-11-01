#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_UID 1
#define ADD_NAME 2

typedef struct zaznam {
 char * jmeno;
 char * t9;
 char uid[21];
 int uidLen;
 int arrayLen;
} ZAZNAM;

void freeStruct ( ZAZNAM * data, int structAmount ) { /* uvoleni celeho pole struktur */
 int i;
 for ( i = 0; i < structAmount; i++ ) {
  free(data[i].jmeno);
  free(data[i].t9);
 }
 free(data);
}

char letterToT9 ( char letter ) {
 if ( letter == ' ' )
  return '1';
 else if ( ( letter >= 'A' && letter <= 'C' ) || ( letter >= 'a' && letter <= 'c' )  )
  return '2';
 else if ( ( letter >= 'D' && letter <= 'F' ) || ( letter >= 'd' && letter <= 'f' )  )
  return '3';
 else if ( ( letter >= 'G' && letter <= 'I' ) || ( letter >= 'g' && letter <= 'i' )  )
  return '4';
 else if ( ( letter >= 'J' && letter <= 'L' ) || ( letter >= 'j' && letter <= 'l' )  )
  return '5';
 else if ( ( letter >= 'M' && letter <= 'O' ) || ( letter >= 'm' && letter <= 'o' )  )
  return '6';
 else if ( ( letter >= 'P' && letter <= 'S' ) || ( letter >= 'p' && letter <= 's' )  )
  return '7';
 else if ( ( letter >= 'T' && letter <= 'V' ) || ( letter >= 't' && letter <= 'v' )  )
  return '8';
 else if ( ( letter >= 'W' && letter <= 'Z' ) || ( letter >= 'w' && letter <= 'z' )  )
  return '9';
 else if ( letter == '\n' )
  return '\n';
 else
  return '\0';
}

void invInput ( void ) { /* Nespravny vstup - vynulovat radku + hlaska */
 while ( fgetc(stdin) != '\n' ) {}
 printf("Nespravny vstup.\n");
}

int main ( void ) {
 ZAZNAM * data = NULL;
 char * vyhledavac = NULL;
 int structAlloc = 0, structAmount = 0, lineAlloc = 0, vyhledavacAlloc = 0, delkaHledac = 0, i, celkem = 0;
 char tmpChar, spaceCntr = 0, addSwitch = ADD_UID, tmpName;
 
 while ( scanf("%c", &tmpChar) == 1 ) { /* nacita prvni znak a rozhoduje jestli je +, ? nebo chyba */
  if ( tmpChar == '\n' ) {
   printf("Nespravny vstup.\n");
  }
  if ( tmpChar == '+' ) { /* mod pridavani nove struktury */
   if ( structAmount >= structAlloc ) { /* alokace zaznamu do zasoby */
    structAlloc += structAlloc < 60 ? 10 : structAlloc / 2;
    data = (ZAZNAM*)realloc(data, structAlloc * sizeof(*data));
    if ( !data ) {
     freeStruct(data, structAmount);
     free(vyhledavac);
     return 1;
    }
    for ( i = structAmount; i < structAlloc; i++ ) {
     data[i].jmeno = NULL;
     data[i].t9 = NULL;
     memset(data[i].uid, 0, sizeof(data[i].uid) * sizeof(*data[i].uid));
     data[i].uidLen = 0;
     data[i].arrayLen = 0;
    }
   }
   while ( scanf("%c", &tmpChar) == 1 ) { /* nacitame zbytek zpravy */
    if ( addSwitch == ADD_UID ) { /* pridavame UID - prvni cast zpravy */
     if ( tmpChar == '\n' ) {
      spaceCntr = 0;
      data[structAmount].uidLen = 0;
      printf("Nespravny vstup.\n");
      break;
     }
     if ( tmpChar == ' ' ) { 
 /* pocitadlo mezer - muze byt pouze 1 mezera a to pred UID, jakmile je tam druha tak prepiname do rezimu nacitani jmena */
     ++spaceCntr;
      if ( spaceCntr > 1 ) {
       spaceCntr = 0;
       if ( data[structAmount].uidLen == 0 ) {
        invInput();
       }
       addSwitch = ADD_NAME;
      }
      continue;
     }

 /* jestli je to cislo a celkem je mene nez 20 cisel pridame ho tam */
     if ( tmpChar >= '0' && tmpChar <= '9' && spaceCntr == 1 && data[structAmount].uidLen < 20 ) {
      data[structAmount].uid[data[structAmount].uidLen] = tmpChar;
      ++data[structAmount].uidLen;
     }
     else { /* spatny vstup, vynulujeme vse protoze pole nebylo pridano */
      spaceCntr = 0;
      data[structAmount].uidLen = 0;
      invInput();
      break;
     }
    }
    else { /* addSwitch == ADD_NAME */
     if ( (tmpName = letterToT9(tmpChar)) && !(tmpChar == ' ' && data[structAmount].arrayLen == 0) ) {
      if ( tmpName != '\n' ) {
       if ( data[structAmount].arrayLen >= lineAlloc ) { /* alokace pro chary jmena */
        lineAlloc += lineAlloc < 60 ? 10 : lineAlloc / 2;
        data[structAmount].jmeno = (char*)realloc(data[structAmount].jmeno, lineAlloc * sizeof(*data[structAmount].jmeno));
        data[structAmount].t9 = (char*)realloc(data[structAmount].t9, lineAlloc * sizeof(*data[structAmount].t9));
        if ( !data[structAmount].jmeno || !data[structAmount].t9 ) {
         freeStruct(data, structAmount);
         free(vyhledavac);
         return 1;
        }
       }
       data[structAmount].jmeno[data[structAmount].arrayLen] = tmpChar;
       data[structAmount].t9[data[structAmount].arrayLen] = tmpName;
       ++data[structAmount].arrayLen;
      } else { /* uspesne pridana radka */
       addSwitch = ADD_UID;
       spaceCntr = 0;
       if ( data[structAmount].arrayLen == 0 || data[structAmount].jmeno[data[structAmount].arrayLen-1] == ' ' ) { /* chybi udaje */
        data[structAmount].uidLen = 0;
        data[structAmount].arrayLen = 0;
        printf("Nespravny vstup.\n");
        break;
       }

       if ( data[structAmount].arrayLen >= lineAlloc ) { /* alokace pro chary jmena */
        lineAlloc += 1;
        data[structAmount].jmeno = (char*)realloc(data[structAmount].jmeno, lineAlloc * sizeof(*data[structAmount].jmeno));
        data[structAmount].t9 = (char*)realloc(data[structAmount].t9, lineAlloc * sizeof(*data[structAmount].t9));
        if ( !data[structAmount].jmeno || !data[structAmount].t9 ) {
         freeStruct(data, structAmount);
         free(vyhledavac);
         return 1;
        }
       }

       data[structAmount].jmeno[data[structAmount].arrayLen] = '\0';
       data[structAmount].uid[data[structAmount].uidLen] = '\0';
       for ( i = 0; i < structAmount; i++ ) {
        if ( data[i].arrayLen == data[structAmount].arrayLen && data[i].uidLen == data[structAmount].uidLen
          && memcmp(data[i].jmeno, data[structAmount].jmeno, data[structAmount].arrayLen * sizeof(*data[i].jmeno)) == 0
          && memcmp(data[i].uid, data[structAmount].uid, data[structAmount].uidLen * sizeof(*data[i].uid)) == 0 ) {
         data[structAmount].uidLen = 0;
         data[structAmount].arrayLen = 0;
         printf("Kontakt jiz existuje.\n");
         spaceCntr = -1;
         break;
        }
       }
       if ( spaceCntr == -1 ) {
        spaceCntr = 0;
        break;
       }
       ++structAmount;
       lineAlloc = 0;
       printf("OK\n");
       break;
      }
     }
     else {
      addSwitch = ADD_UID;
      spaceCntr = 0;
      data[structAmount].uidLen = 0;
      data[structAmount].arrayLen = 0;
      invInput();
      break;
     }
    }
   }
  }
  else if ( tmpChar == '?' ) {

   while ( scanf("%c", &tmpChar) == 1 ) {
    if ( delkaHledac >= vyhledavacAlloc ) {
     vyhledavacAlloc += vyhledavacAlloc < 60 ? 10 : vyhledavacAlloc / 2;
     vyhledavac = (char*)realloc(vyhledavac, vyhledavacAlloc * sizeof(*vyhledavac));
     if ( !vyhledavac ) {
      free(vyhledavac);
      return 1;
     }
    }
    if ( tmpChar == '\n' ) {

     if ( delkaHledac == 0 ) {
      spaceCntr = 0;
      celkem = 0;
      printf("Nespravny vstup.\n");
      break;
     }

     for ( i = 0; i < structAmount; i++) {
      if ( delkaHledac <= data[i].arrayLen ) {
       if ( memcmp(vyhledavac, data[i].t9, delkaHledac * sizeof(*vyhledavac)) == 0 ) {
        ++celkem;
        continue;
       }
      }
      if ( delkaHledac <= data[i].uidLen ) {
       if ( memcmp(vyhledavac, data[i].uid, delkaHledac * sizeof(*vyhledavac)) == 0 ) {
        ++celkem;
       }
      }
     }
     if ( celkem <= 10 ) {
      for ( i = 0; i < structAmount; i++) {
       if ( delkaHledac <= data[i].arrayLen ) {
        if ( memcmp(vyhledavac, data[i].t9, delkaHledac * sizeof(*vyhledavac)) == 0 ) {
         printf("%s %s\n", data[i].uid, data[i].jmeno);
         continue;
        }
       }
       if ( delkaHledac <= data[i].uidLen ) {
        if ( memcmp(vyhledavac, data[i].uid, delkaHledac * sizeof(*vyhledavac)) == 0 ) {
         printf("%s %s\n", data[i].uid, data[i].jmeno);
        }
       }
      }
     }


     printf("Celkem: %d\n", celkem);
     spaceCntr = 0;
     delkaHledac = 0;
     celkem = 0;
     break;
    }
    if ( tmpChar == ' ' ) {
     ++spaceCntr;
     if ( spaceCntr > 1 ) {
      spaceCntr = 0;
      delkaHledac = 0;
      celkem = 0;
      invInput();
      break;
     }
     continue;
    }
    if ( tmpChar >= '0' && tmpChar <= '9' && spaceCntr == 1 ) {
     vyhledavac[delkaHledac] = tmpChar;
     ++delkaHledac;
    }
    else {
     spaceCntr = 0;
     delkaHledac = 0;
     celkem = 0;
     invInput();
     break;
    }
   }
  }
  else {
   invInput();
  }
 }
 freeStruct(data, structAmount);
 free(vyhledavac);
 return 0;
}
