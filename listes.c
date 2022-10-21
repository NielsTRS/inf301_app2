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
    /* À compléter (utiliser malloc) */
    cellule_t *c = malloc(sizeof(cellule_t));
    return c;
}

void detruireCellule(cellule_t *cel) {
    free(cel);
}

int depilerEntier(sequence_t *seq) {
    cellule_t *c;
    if (seq->tete != NULL) {
        c = seq->tete;
        if (c->suivant != NULL) {
            seq->tete = c->suivant;
        }
        detruireCellule(c);
        return c->command.entier;
    }
    return -1;
}

char depilerChar(sequence_t *seq) {
    cellule_t *c;
    if (seq->tete != NULL) {
        c = seq->tete;
        seq->tete = c->suivant;
        detruireCellule(c);
        return c->command.character;
    }
    return '\0';
}

void empiler(sequence_t *l, char c) {
    cellule_t *cell = nouvelleCellule();
    int tag = getTag(c);
    switch (tag) {
        case 1:
            cell->command.entier = c - '0';
            break;
        case 2:
            cell->command.character = c;
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

void conversion(char *texte, sequence_t *seq) {
    int l = strlen(texte);
    if (l >= 0) {
        empiler(seq, texte[0]);
        cellule_t *cell;
        cell = seq->tete;
        int i = 1;
        while (i < l) {
            if (texte[i] != ' ') {
                cell = ajouter_queue_mod(seq, cell, texte[i]);
            }
            i = i + 1;
        }
    } else {
        printf("erreur tentative création liste vide");
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
            c->command.character = u;
            break;
        default:
            printf("ERREUR TAG INCONNU DANS L'AJOUT QUEUE MOD");
            break;
    }
    c->tag = tag;
    c->suivant = NULL;
    queue->suivant = c;
    return c;
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
                printf("%c", c->command.character);
                break;
            default:
                printf("ERREUR TAG INCONNU DANS L'AFFICHAGE");
                break;
        }
        c = c->suivant;
    }
    printf("\n");
}
