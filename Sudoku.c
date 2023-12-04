#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/**
 * @brief Codes de couleur pour l'affichage dans le terminal.
 */
#define RED "\033[0;31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

/**
 * @brief Codes d'erreur possibles.
 */
#define VALEUR_EXISTE_LIGNE 76105103
#define VALEUR_EXISTE_COLONNE 67111108
#define VALEUR_EXISTE_BLOC 6610899
#define SAISIR_ENTIER 69110116
#define SAISIR_DANS_BORNES 66114110
#define CASE_DE_BASE 67115101
#define ENTREE_TROP_LONGUE 76110103

#define TAILLE 3
#define BLOC TAILLE * TAILLE
typedef int tGrille[BLOC][BLOC];

const int TAILLE_MIN = 1;

/**
 * @brief Charge la grille à partir d'un fichier.
 * 
 * @param grille La grille à remplir avec les données du fichier.
 */
void chargerGrille(tGrille grille);

/**
 * @brief Copie la grille de base dans une grille modifiable.
 * 
 * @param grilleDeBase La grille de base à copier.
 * @param grille La grille modifiable.
 */
void copieGrille(tGrille grilleDeBase, tGrille grille);

/**
 * @brief Affiche la grille dans le terminal.
 * 
 * @param grille La grille actuelle.
 * @param grilleDeBase La grille de base.
 */
void afficherGrille(tGrille grille, tGrille grilleDeBase);

/**
 * @brief Saisit une valeur depuis l'entrée standard.
 * 
 * @param valeur Pointeur vers la variable où stocker la valeur saisie.
 */
void saisir(int *valeur);

/**
 * @brief Vérifie si une valeur peut être placée dans une position donnée.
 * 
 * @param grille La grille actuelle.
 * @param numLigne Numéro de ligne.
 * @param numColonne Numéro de colonne.
 * @param valeur La valeur à vérifier.
 * @param grilleDefault La grille de base.
 * @return true si la valeur peut être placée, false sinon.
 */
bool possible(tGrille grille, int numLigne, int numColonne, int valeur, tGrille grilleDefault);

/**
 * @brief Vérifie si la grille est entièrement remplie.
 * 
 * @param grille La grille à vérifier.
 * @return true si la grille est pleine, false sinon.
 */
bool grilleEstPleine(tGrille grille);

/**
 * @brief Gère les messages d'erreur.
 * 
 * @param id Identifiant de l'erreur.
 */
void erreurs(int id);

/**
 * @brief Affiche une animation de chargement.
 */
void chargement();

/**
 * @brief Fonction principale du jeu Sudoku.
 * 
 * @return EXIT_SUCCESS si le programme se termine correctement.
 */
int main()
{
    char rejouer;

    do
    {
        // Initialisation
        tGrille grille, grilleDeBase;
        int numLigne, numColonne, valeur;

        chargerGrille(grilleDeBase);
        copieGrille(grilleDeBase, grille);
        printf("Initialisation");
        chargement();

        // Tant que la grille n'est pas finie
        while (!grilleEstPleine(grille))
        {
            afficherGrille(grille, grilleDeBase);

            printf("Indice de " CYAN "ligne" RESET ": ");
            saisir(&numLigne); // Il y a une erreur ici car le programme renvoie une erreur pour aucune raison et je ne sais pas comment fixer 
            numLigne--; // Passe de [1-9] à [0-8], borne de la grille

            printf("Indice de " CYAN "colonne" RESET ": ");
            saisir(&numColonne);
            numColonne--; // Passe de [1-9] à [0-8], borne de la grille

            printf(CYAN "Valeur" RESET " à mettre en " CYAN "(%d, %d): " RESET, numLigne + 1, numColonne + 1);
            saisir(&valeur);

            printf("\033[2J\033[1;1H"); // Vide le terminal

            if (possible(grille, numLigne, numColonne, valeur, grilleDeBase))
            {
                // Si la valeur n'est pas out of range ou déjà présente
                grille[numLigne][numColonne] = valeur; // alors elle est ajoutée à la grille
            }
        }
        printf("Grille pleine, fin de partie\n");

        // Demander à l'utilisateur s'il veut rejouer
        printf("Voulez-vous rejouer ? (O/N): ");
        scanf(" %c", &rejouer);
        if (rejouer == 'O' || rejouer == 'o')
        {
            printf("Reinitialisation");
            chargement();
        }

    } while (rejouer == 'O' || rejouer == 'o');

    return EXIT_SUCCESS;
}

/**
 * @brief Charge la grille à partir d'un fichier.
 * 
 * @param grille La grille à remplir avec les données du fichier.
 */
void chargerGrille(tGrille grille)
{
    char nomFichier[30];
    FILE *file;

    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);

    file = fopen(nomFichier, "rb");
    if (file == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(grille, sizeof(int), BLOC * BLOC, file);
    }
    fclose(file);
    while (getchar() != '\n'); // Vide le buffer d'entrée

    /*
    Liste des grilles:
    Grille1.sud
    ...
    Grille10.sud
    */
}

/**
 * @brief Copie la grille de base dans une grille modifiable.
 * 
 * @param grilleDeBase La grille de base à copier.
 * @param grille La grille modifiable.
 */
void copieGrille(tGrille grilleDeBase, tGrille grille)
{
    for (int ligne = 0; ligne < BLOC; ligne++)
    {
        for (int colonne = 0; colonne < BLOC; colonne++)
        {
            grille[ligne][colonne] = grilleDeBase[ligne][colonne];
        }
    }
}

/**
 * @brief Affiche la grille dans le terminal.
 * 
 * @param grille La grille actuelle.
 * @param grilleDeBase La grille de base.
 */
void afficherGrille(tGrille grille, tGrille grilleDeBase)
{
    printf(GREEN "    1 2 3   4 5 6   7 8 9  \n" RESET); // Affichage des chiffres en haut (pour choisir la colonne)
    printf("  +-------+-------+-------+\n");

    for (int ligne = 0; ligne < BLOC; ligne++)
    {
        if (ligne % TAILLE == 0 && ligne != 0)
        {
            printf("  +-------+-------+-------+\n"); // Affichage d'une ligne de séparation pour les blocs de 3x3
        }

        printf(GREEN "%d" RESET " | ", ligne + 1); // Affichage des chiffres à droite (pour choisir la ligne)

        for (int colonne = 0; colonne < BLOC; colonne++)
        {
            if (colonne % TAILLE == 0 && colonne != 0)
            {
                printf("| "); // Affichage d'une colonne de séparation pour les blocs de 3x3
            }

            if (grille[ligne][colonne] == 0)
            {
                printf(". "); // Affichage d'un point si valeur vide
            }
            else if (grille[ligne][colonne] == grilleDeBase[ligne][colonne])
            {
                printf(RED "%d " RESET, grille[ligne][colonne]); // La valeur sinon, si case de base en rouge
            }
            else
            {
                printf(BLUE "%d " RESET, grille[ligne][colonne]); // Bleu sinon
            }
        }
        printf("|\n");
    }
    printf("  +-------+-------+-------+\n");
}

/**
 * @brief Vérifie si une valeur peut être placée dans une position donnée.
 * 
 * @param grille La grille actuelle.
 * @param numLigne Numéro de ligne.
 * @param numColonne Numéro de colonne.
 * @param valeur La valeur à vérifier.
 * @param grilleDefault La grille de base.
 * @return true si la valeur peut être placée, false sinon.
 */
bool possible(tGrille grille, int numLigne, int numColonne, int valeur, tGrille grilleDefault)
{
    // Vérifie la ligne
    for (int colonne = 0; colonne < BLOC; colonne++)
    {
        if (grille[numLigne][colonne] == valeur)
        {
            erreurs(VALEUR_EXISTE_LIGNE);
            return false;
        }
    }

    // Vérifie la colonne
    for (int ligne = 0; ligne < BLOC; ligne++)
    {
        if (grille[ligne][numColonne] == valeur)
        {
            erreurs(VALEUR_EXISTE_COLONNE);
            return false;
        }
    }

    // Vérifie le bloc 3x3
    int debutBlocLigne = (numLigne / TAILLE) * TAILLE; // val = [0,1,2], x3 = [0, 3, 6]
    int debutBlocColonne = (numColonne / TAILLE) * TAILLE; // Donc prends l'input est quelque soit la valeur la transforme en [0,3,6], permettant de savoir son bloc

    for (int ligne = debutBlocLigne; ligne < debutBlocLigne + TAILLE; ++ligne) // Le +3 est pour avoir la fin du bloc
    {
        for (int colonne = debutBlocColonne; colonne < debutBlocColonne + TAILLE; ++colonne)
        {
            if (grille[ligne][colonne] == valeur) // Puis vérifie si la valeur existe
            {
                erreurs(VALEUR_EXISTE_BLOC);
                return false;
            }
        }
    }

    // Vérifie si il s'agit d'une case de base
    if (grilleDefault[numLigne][numColonne] != 0)
    {
        erreurs(CASE_DE_BASE);
        return false;
    }

    return true;
}

/**
 * @brief Vérifie si la grille est entièrement remplie.
 * 
 * @param grille La grille à vérifier.
 * @return true si la grille est pleine, false sinon.
 */
bool grilleEstPleine(tGrille grille)
{
    for (int ligne = 0; ligne < BLOC; ++ligne)
    {
        for (int colonne = 0; colonne < BLOC; colonne++)
        {
            if (grille[ligne][colonne] == 0) // Si il reste au moins une case vide
            {
                return false;
            }
        }
    }
    return true; // Toutes les cases sont remplies
}

/**
 * @brief Saisit une valeur depuis l'entrée standard.
 * 
 * @param valeur Pointeur vers la variable où stocker la valeur saisie.
 */
void saisir(int *valeur)
{
    bool valide = false; // Condition

    do
    {
        char chaine[30] = "";

        fgets(chaine, sizeof(chaine), stdin); // Lit l'entrée
        fflush(stdin); // Vide le buffer d'entrée

        // Enlève le caractère `retour à la ligne` de l'entrée si présent
        int longueur = strlen(chaine);
        if (longueur > 0 && chaine[longueur - 1] == '\n')
        {
            chaine[longueur - 1] = '\0';
        }
        else // si l'entrée est trop longue
        {
            erreurs(ENTREE_TROP_LONGUE);
            while (getchar() != '\n'); // Vide le buffer d'entrée
            continue; // Pour que le message d'erreur ne se répète pas
        }

        if (strlen(chaine) <= 30) // Si l'entrée n'est pas trop longue
        {
            if (sscanf(chaine, "%d", valeur) != 0)
            {
                // Conversion réussie
                if (*valeur >= TAILLE_MIN && *valeur <= BLOC)
                {
                    valide = true; // La valeur est correcte, on sort de la boucle
                }
                else
                {
                    erreurs(SAISIR_DANS_BORNES);
                }
            }
            else
            {
                erreurs(SAISIR_ENTIER);
            }
        }
        else // Car mieux vaut être sûr :)
        {
            erreurs(ENTREE_TROP_LONGUE);
        }
    } while (!valide);
}

/**
 * @brief Gère les messages d'erreur.
 * 
 * @param id Identifiant de l'erreur.
 */
void erreurs(int id)
{
    switch (id)
    {
    case VALEUR_EXISTE_LIGNE:
        printf(RED "Erreur" RESET "!\nLa valeur est déjà " RED "présente" RESET " dans la " RED "ligne" RESET ". Veuillez réessayer.\n");
        break;

    case VALEUR_EXISTE_COLONNE:
        printf(RED "Erreur" RESET "!\nLa valeur est déjà " RED "présente" RESET " dans la " RED "colonne" RESET ". Veuillez réessayer.\n");
        break;

    case VALEUR_EXISTE_BLOC:
        printf(RED "Erreur" RESET "!\nLa valeur est déjà " RED "présente" RESET " dans le " RED "bloc" RESET ". Veuillez réessayer.\n");
        break;

    case SAISIR_ENTIER:
        printf(RED "Erreur" RESET "!\nVeuillez " RED "saisir" RESET " un " RED "entier" RESET ". Veuillez réessayer.\n");
        break;

    case SAISIR_DANS_BORNES:
        printf(RED "Erreur" RESET "!\nVeuillez " RED "saisir" RESET " un " RED "entier entre 1 & %d" RESET ". Veuillez réessayer.\n", BLOC);
        break;

    case CASE_DE_BASE:
        printf(RED "Erreur" RESET "!\nLa case que vous cherchez à " RED "modifier" RESET " est une case de la " RED "grille de base" RESET ". Veuillez réessayer.\n");
        break;

    case ENTREE_TROP_LONGUE:
        printf(RED "Erreur" RESET "!\n" RED "L'entrée est trop longue" RESET ". Veuillez réessayer.\n");
        break;

    default:
        break;
    }
}

/**
 * @brief Affiche une animation de chargement.
 */
void chargement()
{
    for (int _ = 0; _ < 3; _++)
    {
        fflush(stdout);
        sleep(1);
        printf(".");
    }
    printf("\n");
}