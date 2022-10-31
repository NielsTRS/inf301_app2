#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#ifdef NCURSES
#include <ncurses.h>
#endif

#include "listes.h"
#include "string.h"
#include <ctype.h>

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;

cellule_t *nouvelleCellule(void) {
    cellule_t *c = malloc(sizeof(cellule_t));
    c->suivant = NULL;
    return c;
}

void detruireCellule(cellule_t *cel) {
    free(cel);
}

void detruireListe(sequence_t *l) {
    cellule_t *c = l->tete;
    cellule_t *tmp;
    while (c != NULL) {
        tmp = c;
        c = c->suivant;
        if (tmp->tag == 3) {
            detruireListe(tmp->command.liste);
        }
        detruireCellule(tmp);
    }
    free(l);
}

int depilerEntier(sequence_t *seq) {
    int entier = -1;
    cellule_t *c;
    if (seq->tete != NULL) {
        c = seq->tete;
        entier = c->command.entier;
        if (c->suivant != NULL) {
            seq->tete = c->suivant;
        } else {
            seq->tete = NULL;
        }
        detruireCellule(c);
        return entier;
    }
    return entier;
}

char depilerChar(sequence_t *seq) {
    char caractere = '\0';
    cellule_t *c;
    if (seq->tete != NULL) {
        c = seq->tete;
        caractere = c->command.caractere;
        if (c->suivant != NULL) {
            seq->tete = c->suivant;
        } else {
            seq->tete = NULL;
        }

        detruireCellule(c);
        return caractere;
    }
    return caractere;
}

sequence_t *depilerListe(sequence_t *seq) {
    sequence_t *retour = NULL;
    cellule_t *c;
    if (seq->tete != NULL) {
        c = seq->tete;
        retour = c->command.liste;
        if (c->suivant != NULL) {
            seq->tete = c->suivant;
        } else {
            seq->tete = NULL;
        }

        detruireCellule(c);
        return retour;
    }
    return retour;
}

void empiler(sequence_t *l, char c) {
    cellule_t *cell = nouvelleCellule();
    int tag = getTag(c);
    switch (tag) {
        case 1:
            cell->command.entier = c - '0';
            break;
        case 2:
            cell->command.caractere = c;
            break;
        default:
            break;
    }
    cell->tag = tag;
    if (l->tete != NULL) {
        cell->suivant = l->tete;
    }
    l->tete = cell;
}

void empilerListe(sequence_t *l, sequence_t *aAjouter) {
    cellule_t *cell = nouvelleCellule();
    cell->tag = 3;
    cell->command.liste = aAjouter;
    if (l->tete != NULL) {
        cell->suivant = l->tete;
    }
    l->tete = cell;
}

void conversion(char *texte, sequence_t *seq) {
    seq->tete = NULL;
    int i = 0;
    cellule_t *cell = NULL;
    cell = seq->tete;
    while (texte[i] != '\0') {
        if (texte[i] != ' ' && texte[i] != '\n')
            cell = ajouter_queue_mod(seq, cell, texte[i]);
        i++;
    }
}

// nos fonctions intermédiaires

int getTag(char c) {
    if (isdigit(c)) {
        return 1;
    } else {
        return 2;
    }
}

cellule_t *ajouter_queue_mod(sequence_t *l, cellule_t *queue, char u) {
    cellule_t *c = nouvelleCellule();
    int tag = getTag(u);
    switch (tag) {
        case 1:
            c->command.entier = u - '0';
            break;
        case 2:
            c->command.caractere = u;
            break;
        default:
            printf("ERREUR TAG INCONNU DANS L'AJOUT QUEUE MOD");
            break;
    }
    c->tag = tag;
    c->suivant = NULL;
    if (queue != NULL) {
        queue->suivant = c;
    } else {
        l->tete = c;
    }
    return c;
}

void ajouter_queue(sequence_t *l, char u) {
    cellule_t *new = nouvelleCellule();

    int tag = getTag(u);
    switch (tag) {
        case 1:
            new->command.entier = u - '0';
            break;
        case 2:
            new->command.caractere = u;
            break;
        default:
            printf("ERREUR TAG INCONNU DANS L'AJOUT QUEUE");
            break;
    }
    new->tag = tag;
    cellule_t *c = l->tete;
    if (c != NULL) {
        while (c->suivant != NULL) {
            c = c->suivant;
        }
    }
    new->suivant = NULL;
    if (c != NULL) {
        c->suivant = new;
    } else {
        l->tete = new;
    }
}

void inversion(sequence_t *seq) {
    if (seq->tete != NULL) {
        int n;
        cellule_t *c_suiv = seq->tete;
        for (n = 1; c_suiv->suivant != NULL; n++) {
            c_suiv = c_suiv->suivant;
        }

        int tag[n];
        int commande_entier[n];
        char commande_caractere[n];
        struct sequence *liste[n];
        c_suiv = seq->tete;
        for (int i = 0; i < n; i++) {
            tag[i] = c_suiv->tag;
            switch (tag[i]) {
                case 1:
                    commande_entier[i] = c_suiv->command.entier;
                    break;
                case 2:
                    commande_caractere[i] = c_suiv->command.caractere;
                    break;
                case 3:
                    liste[n] = c_suiv->command.liste;
                    break;
                default:
                    break;
            }
            c_suiv = c_suiv->suivant;
        }

        c_suiv = seq->tete;
        for (int i = 1; i <= n; i++) {
            c_suiv->tag = tag[n - i];
            switch (tag[n - i]) {
                case 1:
                    c_suiv->command.entier = commande_entier[n - i];
                    break;
                case 2:
                    c_suiv->command.caractere = commande_caractere[n - i];
                    break;
                case 3:
                    c_suiv->command.liste = liste[n - i];
                    break;
                default:
                    break;
            }
            c_suiv = c_suiv->suivant;
        }
    }
}

void afficher(sequence_t *seq) {
    assert(seq); /* Le pointeur doit être valide */
    cellule_t *c;
    c = seq->tete;
    while (c != NULL) {
        switch (c->tag) {
            case 1:
                printf("%d", c->command.entier);
                break;
            case 2:
                printf("%c", c->command.caractere);
                break;
            case 3:
                printf("{");
                afficher(c->command.liste);
                printf("}");
                break;
            default:
                printf("ERREUR TAG INCONNU DANS L'AFFICHAGE");
                break;
        }
        c = c->suivant;
    }
    // printf("\n");
}
