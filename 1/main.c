#include <stdio.h>

int main ( void ) {
	int a = 0;
	if (scanf("%d", &a) == 1) {
		switch(a) {
			case 1:
				printf("ml' nob:\nQapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
				break;
			case 2:
				printf("ml' nob:\nQapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
				break;
			case 3:
				printf("ml' nob:\nQapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
				break;
			case 4:
				printf("ml' nob:\nQapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
				break;
			case 5:
				printf("ml' nob:\nQapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
				break;
			default:
				printf("ml' nob:\nluj\n");
				break;
		}
	}
	else {
		printf("ml' nob:\nluj\n");
	}

	return 0;
}

