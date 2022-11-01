#include <stdio.h>
#include <math.h>

/*int pocetLatky ( double velPlachtyX, double velPlachtyY, double velLatkyX, double velLatkyY, double prekryv ) {

 int pocetLatkyX;
 int pocetLatkyY;
 int celkem;

 pocetLatkyX = ceil(velPlachtyX / velLatkyX);
 pocetLatkyY = ceil(velPlachtyY / velLatkyY);

 velPlachtyX += (pocetLatkyX - 1) * prekryv;
 velPlachtyY += (pocetLatkyY - 1) * prekryv;

 celkem = ceil(velPlachtyX / velLatkyX) * ceil(velPlachtyY / velLatkyY);

 return celkem;
}*/

int pocetLatky ( double velPlachtyX, double velPlachtyY, double velLatkyX, double velLatkyY, double prekryv ) {

 int pocetLatkyX;
 int pocetLatkyY;
 double odchylkaX;
 double odchylkaY;
 double mezX;
 double mezY;
 
 odchylkaX = velPlachtyX - velLatkyX;
 odchylkaY = velPlachtyY - velLatkyY;

 if (odchylkaX < 0) odchylkaX = 0;
 if (odchylkaY < 0) odchylkaY = 0;
 
 mezX = odchylkaX / (velLatkyX - prekryv);
 mezY = odchylkaY / (velLatkyY - prekryv);

 if (fabs(mezX - floor(mezX)) < 1e-14 * (mezX + floor(mezX) / 2)) {
  pocetLatkyX = floor((odchylkaX / (velLatkyX - prekryv)));
 } else {
  pocetLatkyX = ceil((odchylkaX / (velLatkyX - prekryv)));
 }
 if (fabs(mezY - floor(mezY)) < 1e-14 * (mezY + floor(mezY) / 2)) {
  pocetLatkyY = floor((odchylkaY / (velLatkyY - prekryv)));
 } else {
  pocetLatkyY = ceil((odchylkaY / (velLatkyY - prekryv)));
 }

 pocetLatkyX++;
 pocetLatkyY++;

 return pocetLatkyX * pocetLatkyY;
}

int main ( void ) {
 double velLatkyX;
 double velLatkyY;
 double velPlachtyX;
 double velPlachtyY;
 double prekryv = 0;
 int pocetLatkyA = -5;
 int pocetLatkyB = -5;
 printf("Velikost latky:\n");
 if (scanf("%lf %lf", &velLatkyX, &velLatkyY) != 2) {
  printf("Nespravny vstup.\n");
  return 1;
 }
 if (velLatkyX <= 0 || velLatkyY <= 0) {
  printf("Nespravny vstup.\n");
  return 1;
 }
 printf("Velikost plachty:\n");
 if (scanf("%lf %lf", &velPlachtyX, &velPlachtyY) != 2) {
  printf("Nespravny vstup.\n");
  return 1;
 }
 if (velPlachtyX <= 0 || velPlachtyY <= 0) {
  printf("Nespravny vstup.\n");
  return 1;
 }
 if (velPlachtyX < velPlachtyY) {
  double tmp = velPlachtyX;
  velPlachtyX = velPlachtyY;
  velPlachtyY = tmp;
 }
 if (velLatkyX < velLatkyY) {
  double tmp = velLatkyX;
  velLatkyX = velLatkyY;
  velLatkyY = tmp;
 }
 if ((velPlachtyX > velLatkyX || velPlachtyY > velLatkyY)/* && (fabs(velPlachtyX - velLatkyX) > 1e-14 * ((velPlachtyX + velLatkyX) / 2))*/) {
  printf("Prekryv:\n");
  if (scanf("%lf", &prekryv) != 1) {
   printf("Nespravny vstup.\n");
   return 1;
  }
 }
 if (prekryv < 0) {
  printf("Nespravny vstup.\n");
  return 1;
 }


 if (velLatkyX <= prekryv && velLatkyY <= prekryv) {
  printf("Nelze vyrobit.\n");
  return 1;
 }

 if (velLatkyX > prekryv) {
  pocetLatkyA = pocetLatky ( velPlachtyX, velPlachtyY, velLatkyX, velLatkyY, prekryv );
 }

 if (velLatkyY > prekryv) {
  pocetLatkyB = pocetLatky ( velPlachtyX, velPlachtyY, velLatkyY, velLatkyX, prekryv );
 }

 if ( pocetLatkyA > 0 && ( pocetLatkyA < pocetLatkyB || pocetLatkyB < 0 ) ) 
  printf("Pocet kusu latky: %d\n", pocetLatkyA);

 if ( pocetLatkyB > 0 && ( pocetLatkyB <= pocetLatkyA || pocetLatkyA < 0 ) ) 
  printf("Pocet kusu latky: %d\n", pocetLatkyB);

 return 0;
}
