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

void stop(void) {
    char enter = '\0';
    printf("Appuyer sur entrée pour continuer...\n");
    while (enter != '\r' && enter != '\n') {
        enter = getchar();
    }
}

int interprete(sequence_t *seq, bool debug) {
    // Version temporaire a remplacer par une lecture des commandes dans la
    // liste chainee et leur interpretation.

    char commande;
    cellule_t *c;
    int i = 0;
    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    printf("Programme:");
    afficher(seq);
    printf("\n");
    if (debug)
        stop();
    c = seq->tete;
    int ret; // utilisée pour les valeurs de retour
    sequence_t *pile = malloc(sizeof(sequence_t));
    pile->tete = NULL;
    int n;                                          // pour stocker un argument (le dernier élément de pile par ex)
    sequence_t *pileV = malloc(sizeof(sequence_t)); // pour stocker un bloc de commandes
    sequence_t *pileF = malloc(sizeof(sequence_t)); // pour stocker un bloc de commandes
    while (c != NULL) {

        if (c->tag == 1) { // c'est un entier donc on l'empile
            empiler(pile, c->command.entier + '0');
        } else if (c->tag == 2) // c'est un caractère donc une commande
        {
            commande = c->command.caractere;
            printf("instruction %d : %c", i, c->command.caractere);
            switch (commande) {
                case 'A': // avancer
                    ret = avance();
                    if (ret == VICTOIRE)
                        return VICTOIRE; /* on a atteint la cible */
                    if (ret == RATE)
                        return RATE; /* tombé dans l'eau ou sur un rocher */
                    break;           /* à ne jamais oublier !!! */
                case 'G': // gauche
                    gauche();
                    break;
                case 'D': // droite
                    droite();
                    break;
                case 'P': // pose
                    n = depilerEntier(pile);
                    if (n == -1) {
                        printf("Erreur: la pile est vide dans interprération");
                    } else if (n == 0) {
                        pose(n);
                        retirerMarque();
                    } else {
                        pose(n);
                        poserMarque();
                    }
                    break;
                case 'M': // mesure
                    n = depilerEntier(pile);
                    if (n == -1) {
                        printf("Erreur: la pile est vide dans interprération");
                    } else {
                        n = mesure(n);
                        empiler(pile, n + '0');
                    }
                    break;
                case '?': // condition
                    *pileF = *depilerListe(pile);
                    *pileV = *depilerListe(pile);
                    n = depilerEntier(pile);
                    if (n == -1) {
                        printf("Erreur: la pile est vide dans interprération");
                    } else if (n == 0) { // execute F
                        ret = interprete(pileF, debug);
                        if (ret == VICTOIRE)
                            return VICTOIRE;
                        if (ret == RATE)
                            return RATE;
                    } else { // execute V
                        ret = interprete(pileV, debug);
                        if (ret == VICTOIRE)
                            return VICTOIRE; /* on a atteint la cible */
                        if (ret == RATE)
                            return RATE;
                    }
                    break;
                default:
                    eprintf("Caractère inconnu: '%c'\n", commande);
            }
        } else if (c->tag == 3) { // c'est une liste
            empilerListe(pile, c->command.liste);
        } else {
            printf("Erreur: tag inconnu");
        }
        afficher(pile);

        c = c->suivant;

        /* Affichage pour faciliter le debug */
        afficherCarte();
        printf("Programme:");
        afficher(seq);
        printf("\n");
        i = i + 1;
        if (debug)
            stop();
    }

    /* Si on sort de la boucle sans arriver sur la cible,
     * c'est raté :-( */

    return CIBLERATEE;
}
