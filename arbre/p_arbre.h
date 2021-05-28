#ifndef P_ARBRE
#define P_ARBRE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// -- LISTE BALISE CONNUES -- TYPE
typedef enum e_balise{texte_enrichi,document,annexes,contenu,section,
    titre,liste,item,liste_texte,texte_liste,
    texte,mot_enrichi,mot_important,NB_OPERATIONS} t_balise;


// -- STRUCTURE STRING
typedef struct s_entree_string{
    char l_etiquette;
    struct s_entree_string* le_frere_suivant;
} t_entree_string;
typedef t_entree_string* a_entree_string;
typedef a_entree_string t_table_string;

// -- STRUCTURE ARBRE HTML
typedef struct s_entree_arbre{
    char* l_etiquette;
    struct s_entree_arbre* le_premier_fils;
    struct s_entree_arbre* le_dernier_fils;
    struct s_entree_arbre* le_frere_precedent;
    struct s_entree_arbre* le_frere_suivant;
    struct s_entree_arbre* le_pere;
}t_entree_arbre;
typedef t_entree_arbre* a_entree_arbre;
typedef a_entree_arbre t_table_arbre;


// ---- METODES

void start(char le_fichier[]);


#endif