#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#ifdef NCURSES
#include <ncurses.h>
#endif
#include "listes.h"
#include "string.h"

/*
 *  Auteur(s) :
 *  Date :
 *  Suivi des Modifications :
 *
 */

bool silent_mode = false;

cellule_t *nouvelleCellule(void)
{
    /* À compléter (utiliser malloc) */
    printf("\n>>>>>>>>>>> A Faire : liste.c/nouvelleCellule() <<<<<<<<<<<<<<<<\n");
    return NULL;
}

void detruireCellule(cellule_t *cel)
{
    /* À compléter (utiliser free) */
    printf("\n>>>>>>>>>>> A Faire : liste.c/detruireCellule() <<<<<<<<<<<<<<<<\n");
}

void conversion(char *texte, sequence_t *seq)
{
    int l = strlen(texte);
    if (l >= 0)
    {
        ajouter_tete(seq, texte[0]);
        cellule_t *c;
        c = seq->tete;
        for (int i = 1; i < l; i++)
        {
            c = ajouter_queue_mod(seq, c, texte[i]);
        }
    }
    else
    {
        printf("erreur tentative création liste vide");
    }
}

// nos fonctions intermédiaires
cellule_t *ajouter_queue_mod(sequence_t *l, cellule_t *queue, char u)
{
    cellule_t *c = malloc(sizeof(cellule_t));
    c->command = u;
    c->suivant = NULL;
    queue->suivant = c;
    return c;
}

void ajouter_tete(sequence_t *l, int u)
{
    cellule_t *c = malloc(sizeof(cellule_t));
    c->command = u;
    c->suivant = l->tete;
    l->tete = c;
}

void afficher(sequence_t *seq)
{
    assert(seq); /* Le pointeur doit être valide */
    /* À compléter */
    printf("\n>>>>>>>>>>> A Faire : liste.c/afficher() <<<<<<<<<<<<<<<<\n");
    cellule_t *c;
    c = seq->tete;
    while (c != NULL)
    {
        printf(" %c", c->command);
        c = c->suivant;
    }
    printf("\n");
}
