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
char* RemplacerMotParPoints(char* motIn, int firstAndLast)
{
	char* motOut;
	int i, lgMot;
	lgMot = strlen(motIn);
	motOut = (char*)malloc((lgMot) * sizeof(char));
	if (motOut == NULL) {free(motOut); return NULL; }
	for (i = 0; i < lgMot; i++)
	{
		motOut[i] = '.';
		if (motIn[i] == '-')
			motOut[i] = '-';
	}
	motOut[lgMot] = '\0';
	if (firstAndLast != 0)
	{
		motOut[0] = motIn[0];
		motOut[lgMot - 1] = motIn[lgMot - 1];
		for ( i = 0; i <lgMot-1; i++)
		{
			if (motIn[0] == motIn[i] || motIn[lgMot-1] == motIn[i]||motIn[i + 1] == '-'||motIn[i-1]=='-')
			{
				motOut[i] = motIn[i];
			}
		}
	}
	return motOut;
}
int RemplacerLeMotParLettre(char motIn[], char motOut[], char lettre,char WrongLetter[])
{
	int i;
	if (EstDans(motIn, lettre))
	{
		for (i = 0; i < strlen(motIn); i++)
		{
			if (motIn[i] == lettre)
				motOut[i] = lettre;
		}
		return 1;
	}
	if (EstDans(WrongLetter, lettre))
		return 1;
	return 0;
}

char** Alloc_DictEtMotDict(int* NbligneDict,char*NomDict)
{
	char** dictDyna;
	int i;
	char ligne[MAXLIGNE + 1];
	i = 0;
	FILE* pfich;
	pfich = fopen(NomDict, "r");
	if (pfich == NULL) return NULL;
	while (fgets(ligne, MAXLIGNE + 1, pfich) != NULL) i++;
	*NbligneDict = i;
	dictDyna = (char**)malloc(*NbligneDict * sizeof(char));
	if (dictDyna == NULL)
	{
		free(dictDyna);
		return NULL;
	}
	rewind(pfich);
	for (i = 0; i < *NbligneDict; i++)
	{
		fgets(ligne, MAXLIGNE + 1, pfich);
		ligne[strlen(ligne) - 1] = '\0';
		dictDyna[i] = (char*)malloc((strlen(ligne) + 1) * sizeof(char));
		if (dictDyna[i] == NULL)
		{
			libererTableauPointeursVersEntiers(i, dictDyna);
			return NULL;
		}
		strcpy(dictDyna[i], ligne);
	}
	fclose(pfich);
	return dictDyna;
}

char* RechercheMotAleatoire(char** dictDyna, int NbligneDict,int* VerifMotDiff)
{
	int nbAleatoire;
	do
	{
		nbAleatoire = rand() % (NbligneDict);
	} while (nbAleatoire == *VerifMotDiff);
		*VerifMotDiff = nbAleatoire;

	return dictDyna[nbAleatoire];
}


void AffichePendu(int nbErreur)
{
	if (nbErreur == 10)
		printf("\n\n\n\n\n\n____\n");
	if (nbErreur == 9)
		printf("\n |\n |\n |\n |\n |\n_|__\n");
	if (nbErreur == 8)
		printf("  _______\n |\n |\n |\n |\n |\n_|__\n");
	if (nbErreur == 7)
		printf("  _______\n | /\n |/\n |\n |\n |\n_|__\n");
	if (nbErreur == 6)
		printf("  _______\n | /     |\n |/\n |\n |\n |\n_|__\n");
	if (nbErreur == 5)
		printf("  _______\n | /     |\n |/      O\n |\n |\n |\n_|__\n");
	if (nbErreur == 4)
		printf("  _______\n | /     |\n |/      O\n |       |\n |\n |\n_|__\n");
	if (nbErreur == 3)
		printf("  _______\n | /     |\n |/      O\n |      /|\n |\n |\n_|__\n");
	if (nbErreur == 2)
		printf("  _______\n | /     |\n |/      O\n |      /|\\  \n |\n |\n_|__\n");
	if (nbErreur == 1)
		printf("  _______\n | /     |\n |/      O\n |      /|\\ \n |      / \n |\n_|__\n");
	if (nbErreur == 0)
		printf("  _______\n | /     |\n |/      O\n |      /|\\ \n |      / \\ \n |\n_|__\n");
}
char MinToMaj(char lettre)
{
	char LettreMaj;
	if ('a' <= lettre && lettre <= 'z')
	{
		LettreMaj = lettre - ('a' - 'A');
		return LettreMaj;
	}
	else return lettre;

}
char* allocWrongLetter(char* WrongLetter, char lettre)
{
	char* WrongLetterDyn;
	WrongLetterDyn = (char*)malloc(((strlen(WrongLetter) + 1) * sizeof(char)));
	if (WrongLetterDyn == NULL)return NULL;
	strcpy(WrongLetterDyn, WrongLetter);
	WrongLetterDyn[strlen(WrongLetter)] = lettre;
	WrongLetterDyn[strlen(WrongLetter)+1] = '\0';
	return WrongLetterDyn;
}
int EstDans(char chaine[], char caractere)
{
	int i;
	for (i = 0; i < strlen(chaine); i++)
	{
		if (caractere == chaine[i])
			return 1;
	}
	return 0;
}
void libererTableauPointeursVersEntiers(int dim, int* tab[])
{
	int i;
	for (i = 0; i < dim; i++)
		free(tab[i]);
	free(tab);	// uniquement dans la version full dynamique
}
int Accent(char* ligne)
{
	int i;
	for (i = 0; i <strlen(ligne); i++)
	{
		if (ligne[i] < 'A' || ligne[i] > 'z')
		{
			return 1;
		}
	}
	return 0;
}
int MotAvecTiret(char* ligne)
{
	int i;
	for (i = 0; i < strlen(ligne); i++)
	{
		if (ligne[i] == '-')
		{
			return 1;
		}
	}
	return 0;
}
int longueur(char*ligne)
{
	if (MotAvecTiret(ligne) == 1)
	{
		if (strlen(ligne) < LgMotAvTiret)

			return 0;
	}
	else
	{
		if (strlen(ligne) < LgMotSansTiret)
			return 0;
	}
	return 1;
}

int CopierFichier(char* fichier1, char* fichier2)
{
	int i,position;
	FILE* pfich1;
	FILE* pfich2;
	char lignes[MAXLIGNE + 1];
	pfich1 = fopen(fichier1, "r");
	pfich2 = fopen(fichier2, "w");
	if (pfich1 == NULL || pfich2 == NULL)
	{
		printf("Impossible d'ouvrir le fichier\n");
		return NULL;
	}
	while (fgets(lignes, MAXLIGNE + 1, pfich1) != NULL)
	{
		if (Accent(lignes) == 1 && longueur(lignes))
		{
			for (i = 0; i < strlen(lignes); i++)
				lignes[i] = toupper(lignes[i]);
			fprintf(pfich2, "%s", lignes);
		}
	}
	fclose(pfich1);
	fclose(pfich2);
}