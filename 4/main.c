#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int isLeapYear ( int y ) {
 if ( y % 4000 == 0 )
  return 0;
 if ( y % 400 == 0 )
  return 1;
 if ( y % 100 == 0 )
  return 0;
 if ( y % 4 == 0 )
  return 1;
 return 0;
}

int isValid ( int y, int m, int d ) {
 if ( y < 2000 || m < 1 || m > 12 || d > 31 || d < 1 )
  return 0;
 if ( m == 2 ) {
  if ( ( isLeapYear( y ) && d > 0 && d < 30 ) || ( !isLeapYear( y ) && d > 0 && d < 29 ) ) {
   return 1;
  } else {
   return 0;
  }
 }
 if ( m < 8 ) {
  if ( !( m % 2 ) && d == 31 )
   return 0;
 } else {
  if ( m % 2 && d == 31 )
   return 0;
 }
 return 1;
}

int isBankHoliday ( int m, int d ) {
  if (( d == 1 && m == 1 ) || ( d == 1 && m == 5 ) || ( d == 8 && m == 5 )
     || ( d == 5 && m == 7 ) || ( d == 6 && m == 7 ) || ( d == 28 && m == 9 )
     || ( d == 28 && m == 10 ) || ( d == 17 && m == 11 ) || ( d == 24 && m == 12 )
     || ( d == 25 && m == 12 ) || ( d == 26 && m == 12 )) {
   return 1;
  }
  return 0;
}

int IsWorkDay ( int y, int m, int d )
{
  int result;
  if ( isBankHoliday( m, d ) || !isValid( y, m, d ))
   return 0;
  /*Keith algorythm*/
  if ( m < 3 ) {
   d += y;
   --y;
  } else {
   d += y - 2;
  }
  result = 23*m/9 + d + 4 + y/4 - y/100 + y/400;
  result %= 7;
  if ( result == 0 || result == 6 ) {
   return 0;
  }
  return 1;
}

int isValidInterval ( int y1, int m1, int d1, int y2, int m2, int d2 ) {
  if ( y2 < y1 )
   return 0;
  if ( m2 < m1 && y2 == y1 )
   return 0;
  if ( d2 < d1 && m2 == m1 && y2 == y1 )
   return 0;
  return 1;
}

int CountWorkDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2,
                    int * cnt )
{
  if ( !isValid( y1, m1, d1 ) || !isValid( y2, m2, d2 ) || !isValidInterval( y1, m1, d1, y2, m2, d2 ) )
   return 0;
  
  *cnt = 0;

  for ( ; y1 <= y2 ; y1++) {
   for ( ; m1 <= 12 ; m1++) {
    if ( y1 == y2 && m1 > m2 )
      continue;
    for ( ; d1 <= 31 ; d1++) {
     if ( m1 == m2 && y1 == y2 && d1 > d2 )
      continue;
     if ( IsWorkDay( y1, m1, d1 ) )
      (*cnt)++;
    }
    d1 = 1;
   }
   m1 = 1;
  }
  return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  int cnt;
  assert (   IsWorkDay ( 2016,  11,  11 ) );
  assert (   IsWorkDay ( 2008,   1,   2 ) );
  assert (   IsWorkDay ( 2016,   2,  29 ) );
  assert ( ! IsWorkDay ( 2016,  11,  12 ) );
  assert ( ! IsWorkDay ( 2016,  11,  17 ) );
  assert ( ! IsWorkDay ( 2016,  11,  31 ) );
  assert ( ! IsWorkDay ( 2004,   2,  29 ) );
  assert ( ! IsWorkDay ( 2001,   2,  29 ) );
  assert ( ! IsWorkDay ( 1996,   1,   1 ) );
  assert ( ! IsWorkDay ( 2018, 123, 124 ) );
  assert ( CountWorkDays ( 2000, -10, -10,    2200, 10,  1, &cnt ) == 0 );
  assert ( CountWorkDays ( 1999,  12,  31,    2000, 12, 31, &cnt ) == 0 );
  assert ( CountWorkDays ( 2001,   1,   1,    2000,  1,  1, &cnt ) == 0 );
  assert ( CountWorkDays ( 2001,   1,   1,    2015,  2, 29, &cnt ) == 0 );
  assert ( CountWorkDays ( 2000,  12,   2,    2000, 12,  3, &cnt ) == 1 && cnt ==          0 );
  assert ( CountWorkDays ( 2000,  12,   3,    2000, 12,  3, &cnt ) == 1 && cnt ==          0 );
  assert ( CountWorkDays ( 2016,  11,  17,    2016, 11, 17, &cnt ) == 1 && cnt ==          0 );
  assert ( CountWorkDays ( 2000,  12,   1,    2000, 12,  3, &cnt ) == 1 && cnt ==          1 );
  assert ( CountWorkDays ( 2016,  11,   1,    2016, 11,  1, &cnt ) == 1 && cnt ==          1 );
  assert ( CountWorkDays ( 2000,  11,  30,    2000, 12,  3, &cnt ) == 1 && cnt ==          2 );
  assert ( CountWorkDays ( 2000,  11,  29,    2000, 12,  3, &cnt ) == 1 && cnt ==          3 );
  assert ( CountWorkDays ( 2000,  11,  28,    2000, 12,  3, &cnt ) == 1 && cnt ==          4 );
  assert ( CountWorkDays ( 2000,  11,  27,    2000, 12,  3, &cnt ) == 1 && cnt ==          5 );
  assert ( CountWorkDays ( 2004,  12,  26,    2004, 12, 31, &cnt ) == 1 && cnt ==          5 );
  assert ( CountWorkDays ( 2004,  12,  25,    2004, 12, 31, &cnt ) == 1 && cnt ==          5 );
  assert ( CountWorkDays ( 2016,  11,   1,    2016, 11, 17, &cnt ) == 1 && cnt ==         12 );
  assert ( CountWorkDays ( 2016,  11,   1,    2016, 11, 30, &cnt ) == 1 && cnt ==         21 );
  assert ( CountWorkDays ( 2008,   9,  30,    2008, 11, 11, &cnt ) == 1 && cnt ==         30 );
  assert ( CountWorkDays ( 2000,   5,   8,    2000, 12, 31, &cnt ) == 1 && cnt ==        163 );
  assert ( CountWorkDays ( 2015,   1,   1,    2015, 12, 31, &cnt ) == 1 && cnt ==        252 );
  assert ( CountWorkDays ( 2016,   1,   1,    2016, 12, 31, &cnt ) == 1 && cnt ==        254 );
  assert ( CountWorkDays ( 2014,   3,  27,    2016, 11, 12, &cnt ) == 1 && cnt ==        666 );
  assert ( CountWorkDays ( 2001,   2,   3,    2016,  7, 18, &cnt ) == 1 && cnt ==       3911 );
  assert ( CountWorkDays ( 2000,   1,   1,    2016, 12, 31, &cnt ) == 1 && cnt ==       4302 );
  assert ( CountWorkDays ( 2416,   4,   3,    2531,  1,  4, &cnt ) == 1 && cnt ==      29038 );
  assert ( CountWorkDays ( 2279, 10, 14, 3256, 10, 5, &cnt ) == 1 && cnt == 247202); 
  assert ( ! IsWorkDay ( 2100, 12, -5 ) ); assert ( CountWorkDays ( 2009, 8, 22, 2009, 8, 22, &cnt ) == 1 && cnt == 0 );
  assert ( IsWorkDay ( 2016, 11, 11 ) );
  assert ( ! IsWorkDay ( 2016, 11, 12 ) );
  assert ( ! IsWorkDay ( 2016, 11, 17 ) );
  assert ( ! IsWorkDay ( 2016, 11, 31 ) );
  assert ( IsWorkDay ( 2016,  2, 29 ) );
  assert ( ! IsWorkDay ( 2004,  2, 29 ) );
  assert ( ! IsWorkDay ( 2001,  2, 29 ) );
  assert ( ! IsWorkDay ( 1996,  1,  1 ) );
  assert ( CountWorkDays ( 2016, 11,  1,
                           2016, 11, 30, &cnt ) == 1
           && cnt == 21 );
  assert ( CountWorkDays ( 2016, 11,  1,
                           2016, 11, 17, &cnt ) == 1
           && cnt == 12 );
  assert ( CountWorkDays ( 2016, 11,  1,
                           2016, 11,  1, &cnt ) == 1
           && cnt == 1 );
  assert ( CountWorkDays ( 2016, 11, 17,
                           2016, 11, 17, &cnt ) == 1
           && cnt == 0 );
  assert ( CountWorkDays ( 2016,  1,  1,
                           2016, 12, 31, &cnt ) == 1
           && cnt == 254 );
  assert ( CountWorkDays ( 2015,  1,  1,
                           2015, 12, 31, &cnt ) == 1
           && cnt == 252 );
  assert ( CountWorkDays ( 2000,  1,  1,
                           2016, 12, 31, &cnt ) == 1
           && cnt == 4302 );
  assert ( CountWorkDays ( 2001,  2,  3,
                           2016,  7, 18, &cnt ) == 1
           && cnt == 3911 );
  assert ( CountWorkDays ( 2014,  3, 27,
                           2016, 11, 12, &cnt ) == 1
           && cnt == 666 );
  assert ( CountWorkDays ( 2001,  1,  1,
                           2000,  1,  1, &cnt ) == 0 );
  assert ( CountWorkDays ( 2001,  1,  1,
                           2015,  2, 29, &cnt ) == 0 );
  return 0;
}
#endif /* __PROGTEST__ */
