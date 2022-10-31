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
    char commande;
    cellule_t *c;
    int i = 0;
    debug = true; /* À enlever par la suite et utiliser "-d" sur la ligne de commandes */

    if (!silent_mode) {
        printf("Programme:");
        afficher(seq);
        printf("\n");
    }
    if (debug)
        stop();
    c = seq->tete;
    int ret = -1; // utilisée pour les valeurs de retour
    sequence_t *pile = malloc(sizeof(sequence_t));
    pile->tete = NULL;
    int n;                    // pour stocker un argument (le dernier élément de pile par ex)
    sequence_t *pileV = NULL; // pour stocker un bloc de commandes
    sequence_t *pileF = NULL; // pour stocker un bloc de commandes
    sequence_t *bloc;         // pour stocker le bloc de commande en cours de construction
    int parenthesesOuvertes = 0;

    while (c != NULL) {
        if (parenthesesOuvertes > 0) {
            printf("Tag : %d %c\n", c->tag, c->command.caractere);
            switch (c->tag) {
                case 1:
                    ajouter_queue(bloc, c->command.entier + '0');
                    printf("tag entier");
                    break;
                case 2:
                    if (c->command.caractere == '}') {
                        parenthesesOuvertes--;
                        if (parenthesesOuvertes == 0) {
                            empilerListe(pile, bloc);
                            printf("Liste à ajouter :");
                            afficher(bloc);
                            bloc = NULL;
                        } else {

                            ajouter_queue(bloc, c->command.caractere);
                        }
                    } else {
                        printf("Liste à ajouter :");
                        afficher(bloc);
                        ajouter_queue(bloc, c->command.caractere);
                        if (c->command.caractere == '{') {
                            parenthesesOuvertes++;
                        }
                    }
                    break;
                default:
                    printf("tag inconnu interprete parenthese ouverte");
                    break;
            }
        } else {

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
                    case 'G':            // gauche
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
                    case '{':
                        bloc = malloc(sizeof(sequence_t));
                        bloc->tete = NULL;
                        parenthesesOuvertes++;
                        break;
                    case '!':
                        pileV = depilerListe(pile);
                        assert(pileV);
                        ret = interprete(pileV, debug);
                        detruireListe(pileV);
                        if (ret == VICTOIRE) {
                            detruireListe(pile);
                            return VICTOIRE; /* on a atteint la cible */
                        }
                        if (ret == RATE) {
                            detruireListe(pile);
                            return RATE;
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
                    case 'Z':
                        inversion(pile);
                        break;
                    case '?': // condition

                        pileF = depilerListe(pile);
                        pileV = depilerListe(pile);
                        n = depilerEntier(pile);
                        if (n == -1) {
                            printf("Erreur: la pile est vide dans interprération");
                        } else if (n == 0) { // execute F
                            ret = interprete(pileF, debug);
                            detruireListe(pileF);
                            if (ret == VICTOIRE) {
                                detruireListe(pile);
                                return VICTOIRE;
                            }
                            if (ret == RATE) {
                                detruireListe(pile);
                                return RATE;
                            }
                        } else { // execute V
                            ret = interprete(pileV, debug);
                            detruireListe(pileV);
                            if (ret == VICTOIRE) {
                                detruireListe(pile);
                                return VICTOIRE; /* on a atteint la cible */
                            }
                            if (ret == RATE) {
                                detruireListe(pile);
                                return RATE;
                            }
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
