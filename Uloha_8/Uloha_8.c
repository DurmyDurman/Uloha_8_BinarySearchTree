#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable:4996)
#define DELIM ";"
struct BST
{
	char* key;
	char* payload;
	struct BST* left;
	struct BST* right;
	struct BST* parent;
};
typedef struct BST Strom;

///////////////////////////////////////////
///////////////////////////////////////////
Strom* novy_uzel(char k[], char v[])
{
	Strom* t;
	t = (Strom*)malloc(sizeof(Strom));
	if (!t) {
		return NULL;
	}
	t->key = (char*)malloc((strlen(k) + 1)); //alokace pameti pro retezcovy klic
	if (t->key == NULL)
	{
		printf("Bum\n");
		exit(-1);
	}
	strcpy(t->key, k); //zkopiroani retezce do klice
	//////////
	t->payload = (char*)malloc((strlen(v) + 1)); //alokace pamet pro retezec statu
	if (t->payload == NULL)
	{
		printf("Bum\n");
		exit(-1);
	}
	strcpy(t->payload, v); //zkopiroani retezce do obsahu
	//////////
	t->left = NULL;
	t->right = NULL;
	t->parent = NULL;
	return t;
}
///////////////////////////////////////////
///////////////////////////////////////////
Strom* dat_uzel(Strom* t, char k[], char v[])
{
	///////////////////////////
	if (!t) {
		return novy_uzel(k, v);
	}
	///////////////////////////
	if (t->key > k) {//pridani klice na levou stranu (jelikoz k je vetsi nez novy klic)
		if (t->left) {
			return dat_uzel(t->left, k, v);
		}
		else {
			t->left = novy_uzel(k, v);
			if (!t->left) {
				return NULL;
			}
			t->left->parent = t;
			return t->left;
		}
	}
	///////////////////////////
	if (t->key < k) {//pridani klice na pravou stranu (jelikoz k je mensi nez novy klic)
		if (t->right) {
			return dat_uzel(t->right, k, v); 
		}
		else {
			t->right = novy_uzel(k, v);
			if (!t->right) {
				return NULL;
			}
			t->right->parent = t;
			return t->right;
		}
	}
	return NULL;

}
///////////////////////////////////////////
///////////////////////////////////////////
void smazani_strom(Strom* t)
{
	if (!(t))
	{
		return;
	}
	smazani_strom(t->left);
	smazani_strom(t->right);
	printf("Smazani uzlu s klicem: %s\n", t->key);
	free(t);
}
///////////////////////////////////////////
///////////////////////////////////////////
Strom* search_tree(Strom* t, char k[])
{
	if (!t) {
		return NULL;
	}
	if (t->key > k) {

		return search_tree(t->left, k);
	}

	if (t->key < k) {
		return search_tree(t->right, k);
	}
	printf("%s\n", t->payload);
	return t;
}
///////////////////////////////////////////
///////////////////////////////////////////
void tisk_vsech_klicu(Strom* t) //zaciname vzdy v levem podstromu 
{
	if (!t) {
		return;
	}
	tisk_vsech_klicu(t->left);
	printf("Node with key %s and value %s\n", t->key, t->payload);
	tisk_vsech_klicu(t->right);
}
///////////////////////////////////////////
///////////////////////////////////////////
int pocet_uzlu(Strom* t)
{
	int n = 0;

	if (!t)
	{
		return 0;
	}
	n = 1;
	n += pocet_uzlu(t->left); //v leve casti stromu
	n += pocet_uzlu(t->right); //v prave casti stromu
	return n; //vraci se suma vysledku 
}
///////////////////////////////////////////
///////////////////////////////////////////
int vyska_strom(Strom* t) //vysledek je pocet hladin
{
	int hl, hr;

	if (!t)
	{
		return 0;
	}
	hl = vyska_strom(t->left);
	hr = vyska_strom(t->right);
	//vracim tu delsi cast levy/pravyho stromu
	return ((hl > hr) ? hl : hr) + 1;
}
///////////////////////////////////////////
///////////////////////////////////////////
void tisk_info_strom(Strom* t) //vypis hladiny a mnozstvi uzlu
{
	int vyska, uzly;
	if (!t) {
		printf("NULL strom\n");
		return;
	}
	vyska = vyska_strom(t);
	uzly = pocet_uzlu(t);
	printf("Strom ma %d uzlu\n", uzly);

	printf("Vyska stromu je %d vrstev\n", vyska);
}
///////////////////////////////////////////
///////////////////////////////////////////
char* token_klic(char radek[], int klic)// dle klice vyberu kolikaty token budu odebirat = klic uzlu
{
	int pozice_kodu = 0;
	char* token;
	token = strtok(radek, DELIM);
	while (token != NULL)
	{


		if (pozice_kodu == klic)
		{
			return token;
		}

		token = strtok(NULL, DELIM);
		pozice_kodu++;
	}
	return "Nic";
}
///////////////////////////////////////////
///////////////////////////////////////////
char* token_stat(char radek[], int klic)// dle klice vyberu kolikaty token budu odebirat = payload uzlu
{
	int pozice_kodu = 0;
	char* token;
	char pole[40];
	token = strtok(radek, DELIM);
	while (token != NULL)
	{

		if (pozice_kodu == klic)
		{

			return token;
		}

		token = strtok(NULL, DELIM);
		pozice_kodu++;
	}
	return "Nic";
}

///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
int main(int argc, char* argv[])
{
	FILE* in;
	char pole[1024];
	char radek_klic[1024];
	char radek_stat[1024];
	char* klic;
	char* stat;
	int menu = 0;
	int typ_klice = 0;
	int vypis_statu = 0;
	int preskoc_radek_1 = 0;
	Strom* root, * n, * x;
	printf("Jak se jmenuje soubor?(hint: staty.csv):\n"); //nacteni souboru
	scanf("%s", pole);
	if (!(in = fopen(pole, "r"))) {
		printf("Nemohu otevrit soubor\n");
		exit(-1);
	}

	printf("Zvolte si typ klice: 0 = N; 1 = XX; 2 = XXX \n"); //zvoleni promenne, ktera nam pozdejc bude urcovat typ klice
	scanf("%d", &typ_klice);
	while (!(typ_klice<3 && typ_klice>-1)) {
		printf("Spatny klic, znovu:\n 0 = N; 1= XX; 2 = XXX \n");
		scanf("%d", &typ_klice);
	}

	printf("Zvolte si typ vypisu: 3 = Cesky(dlouze); 4 = Cesky(kratce); 5 = Anglicky \n");//zvoleni promenne, ktera nam pozdejc bude urcovat vypis statu
	scanf("%d", &vypis_statu);
	while (!(vypis_statu <6 && vypis_statu>2)) {
		printf("Spatny vypis, znovu: 3 = Cesky(dlouze); 4 = Cesky(kratce); 5 = Anglicky  \n");
		scanf("%d", &vypis_statu);
	}
	root = NULL;
	while (fgets(pole, sizeof(pole), in)) //nacteni radku
	{

		strcpy(radek_klic, pole); //zde je potreba zkopirovat radek do novych poli
		strcpy(radek_stat, pole); //a duvod? 
								  //zkracene: nevim, takhle mi to funguje....dlouze: pri alokaci pameti dochazi k prepisovani pole, takze jsem vytvoril kopii
		if (preskoc_radek_1 > 0)
		{


			klic = (char*)malloc(sizeof(token_klic(radek_klic, typ_klice))); //alokalizace pole dle velikosti 1 tokenu
			strcpy(klic, token_klic(radek_klic, typ_klice)); 

			stat = (char*)malloc((strlen(token_stat(radek_stat,vypis_statu)) + 1)); // alokalizace pole dle velikosti 1 tokenu, tento typ alokace je jediny ktery zde funguje
			strcpy(stat, token_stat(pole,vypis_statu));	 //zkopirovani tokenu do pole

			if (!(n = dat_uzel(root, klic, stat))) { //pridani uzlu
				printf("CHYBA! Klic %s ve BST uz je\n", klic);
			}

			if (!root) {
				root = n; //urceni korene
			}
			free(klic); //vycisteni pameti
			free(stat);
		}
		preskoc_radek_1++; //preskoceni 1. radku
	}
	tisk_info_strom(root); //vytisk informaci o strome
	while (1) // menu pro manipulaci se stromem
	{
		while (!(menu < 5 && menu>1))
		{
			printf("Zvolte si akci: 2 = vypsat vsechny kody, 3 = smazat strom a ukoncit, 4 = novy klic\n ");
			scanf("%d", &menu);
		}

		if (menu == 2) {
			tisk_vsech_klicu(root);
			menu = 0;
		}
		if (menu == 3) {
			smazani_strom(root);
			menu = 0;
			printf("Strom smazan\n");
			exit (1);
		}
		if (menu == 4)
		{
			printf("\n");
			smazani_strom(root);
			printf("Strom smazan\n");
			main(0, 0); //znovu volani mainu (odpuste) pro znovu urceni klice
			menu = 0;
		}

	}
	return 0;
}
