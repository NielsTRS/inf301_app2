#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "curiosity.h"

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

void stop(void)
{
    char enter = '\0';
    printf("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
}

int interprete(sequence_t *seq, bool debug)
{
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;
    cellule_t *c;
    //sequence_t *pile; // on initialise nos piles (pour stocker le résultat par exemple)

    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf("Programme:");
    afficher(seq);
    printf("\n");
    if (debug)
        stop();
    c = seq->tete;
    int ret; // utilisée pour les valeurs de retour
    sequence_t pile;
    int n; //pour stocker un argument (le dernier élément de pile par ex)
    char tempC; //char temporaire pour empiler un entier
    while (c->suivant != NULL)
    {
        if(c->tag == 1){ //c'est un entier donc on l'empile
            empiler(&pile, c->command.entier);
        }else if (c->tag == 2) //c'est un caractère donc une commande
        {
            commande = c->command.character;
                printf("%c", c->command.character);
            switch (commande)
            {
            case 'A':
                ret = avance();
                if (ret == VICTOIRE)
                    return VICTOIRE; /* on a atteint la cible */
                if (ret == RATE)
                    return RATE; /* tombé dans l'eau ou sur un rocher */
                break;           /* à ne jamais oublier !!! */
            case 'G':
                gauche();
                break;
            case 'D':
                droite();
                break;
            case 'P':
                n = depilerEntier(&pile);
                if(n == -1){
                    printf("Erreur: la pile est vide dans interprération");
                }else if(n == 0){
                    retirerMarque();
                }else{
                    poserMarque();
                }
                break;
            case 'M':
                n = depilerEntier(&pile);
                if(n == -1){
                    printf("Erreur: la pile est vide dans interprération");
                }else{
                    n = mesure(n);
                    tempC = n + '0';
                    empiler(&pile, tempC);
                }
                break;
            default:
                eprintf("Caractère inconnu: '%c'\n", commande);
            }
        }

        c = c->suivant;

        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf("Programme:");
        afficher(seq);
        printf("\n");
        if (debug)
            stop();
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    return CIBLERATEE;
}
