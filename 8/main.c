#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct TResult {
 TResult * m_Next;
 int m_ID;
 char * m_Name;
} TRESULT;
#endif /* __PROGTEST__ */

/* struktura dat s rodicemi pro databazi */
typedef struct item { 
 struct item * m_Next;
 int m_ID;
 char * m_Name;
 int parent1;
 int parent2;
} ITEM;

/* globalni ukazatele na spojove struktury */
static ITEM * first; /* ukazatel na prvni prvek spojove struktury databaze */
static TRESULT * firstRes; /* ukazatel na prvni prvek spojove struktury pro navrat predku */
static TRESULT * firstCom; /* ukazatel na prvni prvek spojove struktury pro navrat spolecnych predku */

/* pridani prvku do serazene spojove struktury pro predky, vraci 1 pri uspechu a 0 pri neuspechu */
int RegisterR( int ID, const char * name ) {
 TRESULT * tmp, * current;
 size_t strLen;

 current = firstRes;
/* pomoci promenne current projede celou spojovou strukturu az po sentinel ( nema cenu hledat dal protoze je to serazene ) */
 while ( current && current->m_ID <= ID ) {
/* jestli uz tam prvek je, zkoncime, protoze nechceme duplicity */
  if ( current->m_ID == ID )
   return 0;
/* posun na dalsi prvek ve spojaku */
  current = current->m_Next;
 }

/* vse je ok, muzeme naalokovat a inicializovat strukturu kterou chceme zaregistrovat */
 tmp = (TRESULT*)malloc(sizeof(*tmp));
 tmp->m_ID = ID;
 strLen = strlen(name);
/* alokace pameti pro jmeno a nulovy znak */
 tmp->m_Name = (char*)malloc(strLen + 1);
/* prekopirovani jmena a pridani nuloveho znaku */
 strncpy(tmp->m_Name, name, strLen);
 tmp->m_Name[strLen] = '\0';
 tmp->m_Next = NULL;

/* jestli je seznam prazdny, pridame prvni prvek primo na zacatek */
 if ( !firstRes ) {
  firstRes = tmp;
  return 1;
 }

/* jestli prvek patri uplne na zacatek, pridame ho tam rovnou protoze nize ve while loopu pracujeme s posunutim o prvek a pridani na zacatek by to zbytecne komplikovalo */
 if ( firstRes->m_ID > ID ) {
  current = firstRes;
  firstRes = tmp;
  firstRes->m_Next = current;
  return 1;
 }

/* loop projede spojak a po zkonceni loopu bude v promenne current pozice, na kterou chceme prvek navazat aby se zachovalo serazeni */
 current = firstRes;
 while ( current->m_Next && current->m_Next->m_ID < ID ) {
  current = current->m_Next;
 }

/* navazeme strukturu na spravne misto spojaku */
 tmp->m_Next = current->m_Next;
 current->m_Next = tmp; 
 return 1;
}

int RegisterC( int ID, const char * name ) {
 TRESULT * tmp, * current;

 tmp = (TRESULT*)malloc(sizeof(*tmp));
 tmp->m_ID = ID;
 size_t strLen = strlen(name);
 tmp->m_Name = (char*)malloc(strLen + 1);
 strncpy(tmp->m_Name, name, strLen);
 tmp->m_Name[strLen] = '\0';
 tmp->m_Next = NULL;

 if ( !firstCom ) {
  firstCom = tmp;
  return 1;
 }

 if ( firstCom->m_ID > ID ) {
  current = firstCom;
  firstCom = tmp;
  firstCom->m_Next = current;
  return 1;
 }
 
 current = firstCom;
 while ( current->m_Next && current->m_Next->m_ID < ID ) {
  current = current->m_Next;
 }

 tmp->m_Next = current->m_Next;
 current->m_Next = tmp; 
 return 1;
}

void Init( void ) {
 first = NULL;
}

void Done( void ) {
 ITEM * tmp;
 while ( first ) {
  free(first->m_Name);
  tmp = first->m_Next;
  free(first);
  first = tmp;
 }
}

int Register( int ID, const char * name, int ID1, int ID2 ) {
 ITEM * tmp, * current;
 char p1Valid = 0, p2Valid = 0;

 if ( ID == 0 || ( ID1 == ID2 && ID1 != 0 && ID2 != 0 ) )
  return 0;

 current = first;
 while ( current && current->m_ID <= ID ) {
  if ( current->m_ID == ID )
   return 0;
  current = current->m_Next;
 }

 if ( !ID1 ) {
  p1Valid = 1;
 } else {
  current = first;
  while ( current && current->m_ID <= ID1 ) {
   if ( current->m_ID == ID1 )
    p1Valid = 1;
   current = current->m_Next;
  }
 }

 if ( !ID2 ) {
  p2Valid = 1;
 } else {
  current = first;
  while ( current && current->m_ID <= ID2 ) {
   if ( current->m_ID == ID2 )
    p2Valid = 1;
   current = current->m_Next;
  }
 }

 if ( !p1Valid || !p2Valid )
  return 0;

 tmp = (ITEM*)malloc(sizeof(*tmp));
 tmp->m_ID = ID;
 tmp->parent1 = ID1;
 tmp->parent2 = ID2;
 size_t strLen = strlen(name);
 tmp->m_Name = (char*)malloc(strLen + 1);
 strncpy(tmp->m_Name, name, strLen);
 tmp->m_Name[strLen] = '\0';
 tmp->m_Next = NULL;

 if ( !first ) {
  first = tmp;
  return 1;
 }

 if ( first->m_ID > ID ) {
  current = first;
  first = tmp;
  first->m_Next = current;
  return 1;
 }
 
 current = first;
 while ( current->m_Next && current->m_Next->m_ID < ID ) {
  current = current->m_Next;
 }

 tmp->m_Next = current->m_Next;
 current->m_Next = tmp; 
 return 1;
}

int FindAncestors( int parentID ) {
 if ( !parentID )
  return 0;
 ITEM * current;
 current = first;
 while ( current && current->m_ID <= parentID ) {
  if ( current->m_ID == parentID ) {
   if ( !RegisterR( parentID, current->m_Name ) )
    return 0;
   FindAncestors(current->parent1);
   FindAncestors(current->parent2);
  }
  current = current->m_Next;
 }
 return 0;
}

TRESULT * Ancestors( int ID ) {
 ITEM * current;
 TRESULT * tmp = NULL;

 firstRes = NULL;
 current = first;
 while ( current && current->m_ID <= ID ) {
  if ( current->m_ID == ID ) {
   FindAncestors(current->parent1);
   FindAncestors(current->parent2);
  }
  current = current->m_Next;
 }

 tmp = firstRes;
 firstRes = NULL;
 return tmp;
}

void FreeResult( TRESULT * res ) {
 TRESULT * tmp;
 while ( res ) {
  free(res->m_Name);
  tmp = res->m_Next;
  free(res);
  res = tmp;
 }
}

TRESULT * CommonAncestors( int ID1, int ID2 ) {
 TRESULT * parent1 = NULL;
 TRESULT * parent2 = NULL;
 TRESULT * res = NULL;
 TRESULT * tmp1 = NULL;
 TRESULT * tmp2 = NULL;

 parent1 = Ancestors(ID1);
 parent2 = Ancestors(ID2);

 firstCom = NULL;

 tmp1 = parent1;
 tmp2 = parent2;
 while ( tmp1 && tmp2 ) {
  if ( tmp1->m_ID == tmp2->m_ID ) {
   RegisterC(tmp1->m_ID, tmp1->m_Name);
   tmp1 = tmp1->m_Next;
   tmp2 = tmp2->m_Next;
  }
  else if ( tmp1->m_ID < tmp2->m_ID ) {
   tmp1 = tmp1->m_Next;
  }
  else {
   tmp2 = tmp2->m_Next;
  }
 }
 
 FreeResult(parent1);
 FreeResult(parent2);

 res = firstCom;
 firstCom = NULL;
 return res;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
 {
   char name[100];
   TRESULT * l;

   Init ();
   assert ( Register ( 1, "John", 0, 0 ) == 1 );
   strncpy ( name, "Jane", sizeof ( name ) );
   assert ( Register ( 2, name, 0, 0 ) == 1 );
   assert ( Register ( 3, "Caroline", 0, 0 ) == 1 );
   assert ( Register ( 4, "Peter", 0, 0 ) == 1 );
   assert ( Register ( 5, "George", 1, 2 ) == 1 );
   assert ( Register ( 6, "Martin", 1, 2 ) == 1 );
   assert ( Register ( 7, "John", 3, 4 ) == 1 );
   assert ( Register ( 8, "Sandra", 3, 4 ) == 1 );
   assert ( Register ( 9, "Eve", 1, 2 ) == 1 );
   assert ( Register ( 10, "Douglas", 1, 4 ) == 1 );
   strncpy ( name, "Phillipe", sizeof ( name ) );
   assert ( Register ( 11, name, 6, 8 ) == 1 );
   strncpy ( name, "Maria", sizeof ( name ) );
   assert ( Register ( 12, name, 5, 8 ) == 1 );
   l = Ancestors ( 11 );

   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    * ID=3, name="Caroline"
    * ID=4, name="Peter"
    * ID=6, name="Martin"
    * ID=8, name="Sandra"
    */
   FreeResult ( l );
   assert ( Ancestors ( 3 ) == NULL );
   assert ( Ancestors ( 13 ) == NULL );
   l = CommonAncestors ( 11, 12 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    * ID=3, name="Caroline"
    * ID=4, name="Peter"
    * ID=8, name="Sandra"
    */
   FreeResult ( l );
   l = CommonAncestors ( 10, 9 );
   /*
    * ID=1, name="John"
    */
   FreeResult ( l );
   assert ( CommonAncestors ( 7, 6 ) == NULL );
   l = CommonAncestors ( 7, 10 );
   /*
    * ID=4, name="Peter"
    */
   FreeResult ( l );
   assert ( Register ( 13, "Quido", 12, 11 ) == 1 );
   l = Ancestors ( 13 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    * ID=3, name="Caroline"
    * ID=4, name="Peter"
    * ID=5, name="George"
    * ID=6, name="Martin"
    * ID=8, name="Sandra"
    * ID=11, name="Phillipe"
    * ID=12, name="Maria"
    */
   FreeResult ( l );
   l = CommonAncestors ( 9, 12 );
   /*
    * ID=1, name="John"
    * ID=2, name="Jane"
    */
   FreeResult ( l );
   assert ( Register ( 1, "Francois", 0, 0 ) == 0 );
   Done ();

   Init ();
   assert ( Register ( 10000, "John", 0, 0 ) == 1 );
   assert ( Register ( 10000, "Peter", 0, 0 ) == 0 );
   assert ( Register ( 20000, "Jane", 10000, 0 ) == 1 );
   assert ( Register ( 30000, "Maria", 10000, 10000 ) == 0 );
   assert ( Register ( 40000, "Joe", 10000, 30000 ) == 0 );
   assert ( Register ( 50000, "Carol", 50000, 20000 ) == 0 );
   Done ();

   return 0;
 }
#endif /* __PROGTEST__ */
