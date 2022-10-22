#ifndef LISTES_H
#define LISTES_H

#include <stdbool.h>

/*
 * Pour réaliser des tests de performance, désactiver tous les
 * affichages.
 * Pour cela, le plus simple est de redefinir les fonctions principales
 * en decommentant les 3 lignes suivantes et en commentant l'ancienne
 * definition de 'eprintf' juste dessous.
 */

#ifdef SILENT

#define printf(fmt, ...) (0)
#define eprintf(fmt, ...) (0)
#define putchar(c) (0)

#else

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#endif

extern bool silent_mode;

union CommandeValeur{
    int entier;
    char caractere;
};

struct cellule {
    int tag;
    /*
    tag = 1 : entier
    tag = 2 : character
    */
    union CommandeValeur command;

    /* vous pouvez rajouter d'autres champs ici */
    struct cellule *suivant;
};
typedef struct cellule cellule_t;

struct sequence {
    cellule_t *tete;
};
typedef struct sequence sequence_t;

cellule_t *nouvelleCellule(void);


void detruireCellule(cellule_t *);

int depilerEntier(sequence_t *seq);

char depilerChar(sequence_t *seq);

void empiler(sequence_t *l, char c);

int getTag(char c);

void conversion(char *texte, sequence_t *seq);

cellule_t *ajouter_queue_mod(sequence_t *l, cellule_t *queue, char u);

void afficher(sequence_t *seq);

#endif
