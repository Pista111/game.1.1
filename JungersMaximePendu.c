#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "..\Module\Lecture\lecture.h"
#define ErreurMax 10
#define ErreurMin 2
#define MAXLIGNE 25
#define LgMotSansTiret 6
#define LgMotAvTiret 12
int RemplacerLeMotParLettre(char motIn[], char motOut[], char lettre, char WrongLetter[]);
char* RemplacerMotParPoints(char* motIn, int firstAndLast);
char** Alloc_DictEtMotDict(int* NbligneDict, char* NomDict);
char* RechercheMotAleatoire(char** dictDyna, int NbligneDict, int* VerifMotDiff);
void AffichePendu(int nbErreur);
char MinToMaj(char lettre);
char* allocWrongLetter(char* WrongLetter, char lettre);
int EstDans(char chaine[], char caractere);
void libererTableauPointeursVersEntiers(int dim, int* tab[]);
int CopierFichier(char* fichier1, char* fichier2);
int Accent(char* ligne);
int longueur(char* ligne);
int MotAvecTiret(char* ligne);
main()
{
	char** dictDynamique,*nomDict,*nomDict2, * WrongLetter, * motAleatoire, * motOut, lettre, LettreMaj;
	int *nbAleatoire,nbrErreurAvPendaison, i, FirstAndLast, win, game, restart, NbligneDict, nbrErreurAvPendaisoninit, u;
	nbAleatoire = -1; win = 0; game = 0;
	nomDict = Lire_Chaine_Dynamique("Entrez le nom du dictionnaire : ");
	if (Encore("Voulez-vous jouer sans les caracteres speciaux ? :", 'o', 'n'))
	{
		nomDict2 = Lire_Chaine_Dynamique("Nom du fichier modifie ? : ");
		CopierFichier(nomDict, nomDict2);
		nomDict = nomDict2;
	}
	dictDynamique = Alloc_DictEtMotDict(&NbligneDict,nomDict);
	if (dictDynamique == NULL) { printf("Probleme d'allocation !\n"); return; }
	while (Lire_Entier_Entre("Combien d'erreurs avant la pendaison : ", &nbrErreurAvPendaisoninit, ErreurMin, ErreurMax) == 0);
	FirstAndLast = Encore("Afficher premiere et derniere lettre :", 'o', 'n');
	srand((unsigned)time(NULL));
	do
	{
		nbrErreurAvPendaison = nbrErreurAvPendaisoninit;
		motAleatoire = RechercheMotAleatoire(dictDynamique, NbligneDict, &nbAleatoire);
		motOut = RemplacerMotParPoints(motAleatoire, FirstAndLast);
		if (motOut == NULL) { printf("Probleme d'allocation"); return; }
		WrongLetter = (char*)malloc(sizeof(char));
		if (WrongLetter == NULL) { printf("Probleme d'allocation"); free(WrongLetter); return; }
		WrongLetter[0] = '\0';
		AffichePendu(nbrErreurAvPendaison);
		printf("Vous avez encore %d vies ! %s \t\t\t\tLettre proposees : (null)\n", nbrErreurAvPendaison, motOut);
		u = 0;
		for (i = nbrErreurAvPendaison; 0 < i && strcmp(motAleatoire, motOut) != 0; i)
		{
			printf("Votre proposition : ");
			lettre = _getch();
			LettreMaj = MinToMaj(lettre);
				printf("%c\n", LettreMaj);
				if (RemplacerLeMotParLettre(motAleatoire, motOut, LettreMaj,WrongLetter))
				{
					AffichePendu(nbrErreurAvPendaison);
					printf("Vous avez encore %d vies  ! ", i);
				}
				else
				{
					nbrErreurAvPendaison--; u++;
					AffichePendu(nbrErreurAvPendaison);
					if(i!=1)
					printf("Vous avez encore %d vies  ! ", i - 1);
					WrongLetter = allocWrongLetter(WrongLetter, LettreMaj);
					i--;
				}
				if (i != 0)
				{
					printf("%s                           ", motOut);
					if (u == 0)
						printf("Lettre proposees : (null)\n");
					else
						printf("Lettre proposees : %s\n", WrongLetter);
				}
		}
		if (strcmp(motAleatoire, motOut) == 0)
		{
			printf("Bravo,vous avez gagne, le mot cherche etait bien %s\n", motOut);
			win++;
		}
		else
			printf("Vous avez perdu, le mot cherche etait %s\n", motAleatoire);
		game++;

		printf("Vous avez gagner %d parties sur les %d parties que vous venez de jouer !\n", win, game);
		restart = Encore("Une autre partie", 'o', 'n');
	} while (restart);
}
