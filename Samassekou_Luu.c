#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _noeud {
    int valeur;
    struct _noeud *fg, *fd;
} Noeud, * Arbre;

typedef struct cell {
    Noeud * noeud;
    struct cell * suivant;
} Cellule, * Liste;

typedef struct file {
    Liste debut;
    Liste fin;
    int taille;
} Queue, * File;

Cellule *alloue_cellule(Noeud *n){
    Cellule *nouveau = (Cellule *)malloc(sizeof(Noeud));
    if (!nouveau){
        return NULL;
    }
    nouveau->noeud = n;
    nouveau->suivant = NULL;
    return nouveau;
}

Noeud * alloue_noeud(int val, Arbre fg, Arbre fd){
    Noeud *nouv = (Noeud *)malloc(sizeof(Noeud));
    if(!nouv){
        return NULL;
    }
    nouv->valeur = val;
    nouv->fg = fg;
    nouv->fd = fd;
    return nouv;
}

void insere_en_tete(Liste * l, Cellule * c){
    if (!c){
        return;
    }
    c->suivant = *l;
    *l = c;
}

Cellule * extrait_tete(Liste * l){
    if(!l){
        return NULL;
    }
    Cellule *nouv = *l;
    *l = (*l)->suivant;
    nouv->suivant = NULL;
    return nouv;
}

void affiche_liste_renversee(Liste lst){
    if(!lst){
        return;
    }

    affiche_liste_renversee(lst->suivant);

    printf("%d\n", lst->noeud->valeur);
}

File initialisation(void){
    File nouv = (File)malloc(sizeof(Queue));

    if(!nouv){
        return NULL;
    }

    nouv->debut = NULL;
    nouv->fin = NULL;
    nouv->taille = 0;

    return nouv;
}

int est_vide(File f){
    if(!f || !f->debut){
        return 1;
    }
    return 0;
}

int enfiler(File f, Noeud *n){
    if(!f || !n){
        return 0;
    }

    Cellule *nouv = alloue_cellule(n);
    if(!nouv){
        return 0;
    }

    if(est_vide(f)){
        f->debut = nouv;
        f->fin = nouv;
    } else {
        f->fin->suivant = nouv;
        f->fin = nouv;
    }
    f->taille++;
    return 1;
}

int defiler(File f, Noeud ** sortant){
    if(est_vide(f)){
        return 0;
    }

    Cellule *c_extrait = extrait_tete(&(f->debut));
    if(!c_extrait){
        return 0;
    }

    *sortant = c_extrait->noeud;
    free(c_extrait);

    if(!f->debut){
        f->fin = NULL;
    }
    f->taille--;
    return 1;
}


void free_arbre(Arbre *a){
    if (! (*a)) return;
    free_arbre(&((*a)->fd));
    free_arbre(&((*a)->fg));
    free(*a);
    *a = NULL;
}

int puissance(int x, int n) {
    if ( n == 0) return 1;
    return x * puissance(x, n - 1);
}

int construit_complet_aux(int h, Arbre * a, int valeur) {
    if ( h == -1 || valeur >= puissance(2, h + 1)) {
        *a = NULL;
        return 1;
    }
    *a = alloue_noeud(valeur, NULL, NULL);
    if ( *a == NULL) return 0;
    if ( construit_complet_aux(h, &( (*a)->fg),  valeur * 2) && construit_complet_aux(h, &( (*a)->fd), (valeur + 1) * 2 - 1) ) {
        return 1;
    }
    free_arbre(a);
    return 0;
}
int construit_complet(int h, Arbre * a) {
    return construit_complet_aux(h, a, 1);
}


int construit_filiforme_aleatoire_aux(int h, Arbre * a, int graine, int debut) {
    if ( h == -1 || debut == h + 2) {
        *a = NULL;
        return 1;
    }
    *a = alloue_noeud(debut, NULL, NULL);
    if ( *a == NULL) return 0;
    int r;
    if ( rand()%2 )
        r = construit_filiforme_aleatoire_aux(h, &( (*a)->fg), graine, debut + 1);
    else
        r = construit_filiforme_aleatoire_aux(h, &( (*a)->fd), graine, debut + 1);

    if ( r ) {
        return 1;
    }
    free_arbre(a);
    return 0;
}
int construit_filiforme_aleatoire(int h, Arbre * a, int graine){
    srand(graine);
    return construit_filiforme_aleatoire_aux(h, a, graine, 1);
}

int insere_niveau(Arbre a, int niv, Liste * lst) {
    if ( !a ) return 1;

    if ( niv == 0) {
        Cellule * tmp_cellule = alloue_cellule(a);
        if ( !tmp_cellule) return 0;
        insere_en_tete(lst, tmp_cellule);
        return 1;
    }
    int r_fg = insere_niveau(a->fg, niv - 1, lst);
    int r_fd = insere_niveau(a->fd, niv - 1, lst);
    return r_fg || r_fd;
}


int hauteur(Arbre abr){
    if ( !abr) return -1;
    int max = hauteur(abr->fd);
    int hauteur_gauche = hauteur(abr->fg);
    return max < hauteur_gauche ? hauteur_gauche + 1 : max + 1;
}

int parcours_largeur_naif(Arbre a, Liste * lst) {
    int h = hauteur(a); 
    int r = 1;
    for (int i = 0; i <= h && r; i++) r = insere_niveau(a, i, lst);
    return r;
}

int insere_niveau_V2(Arbre a, int niv, Liste * lst, int * nb_visite) {
    if ( !a ) return 1;
    *nb_visite += 1;
    if ( niv == 0) {
        Cellule * tmp_cellule = alloue_cellule(a);
        if ( !tmp_cellule) return 0;
        insere_en_tete(lst, tmp_cellule);
        return 1;
    }
    else {
        int r_fg = insere_niveau_V2(a->fg, niv - 1, lst, nb_visite);
        int r_fd = insere_niveau_V2(a->fd, niv - 1, lst, nb_visite);
        if ( r_fd) return r_fd;
        return r_fg;
    }}

int parcours_largeur_naif_V2(Arbre a, Liste * lst, int * nb_visite) {
    int h = hauteur(a); 
    int r = 1;
    for (int i = 0; i <= h && r; i++) r = insere_niveau_V2(a, i, lst, nb_visite);
    return r;
}

int parcours_largeur(Arbre a, Liste * lst) {
    File f = initialisation();
    Noeud * sortant = NULL;
    enfiler(f, a);
    while ( !est_vide(f) ) {
        defiler(f, &sortant);
        if ( sortant ) {
            enfiler(f, sortant->fg);
            enfiler(f, sortant->fd);
            //Noeud * tmp_noeud = alloue_noeud((*sortant)->valeur, NULL, NULL);
            Cellule * tmp = alloue_cellule(sortant);
            if ( !tmp) return 0;
            insere_en_tete(lst, tmp);
        }
    }
    return 1;
}

int parcours_largeur_V2(Arbre a, Liste * lst, int * nb_visite)    {
    File f = initialisation();
    Noeud * sortant = NULL;
    enfiler(f, a);
    while ( !est_vide(f) ) {
        defiler(f, &sortant);
        *nb_visite += 1;
        if ( sortant ) {
            enfiler(f, sortant->fg);
            enfiler(f, sortant->fd);
            Cellule * tmp = alloue_cellule(sortant);
            if ( !tmp) return 0;
            insere_en_tete(lst, tmp);
        }
    }
    return 1;
}