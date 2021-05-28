#include "p_arbre.h"
// -- VARIABLE GLOBALES
char* mes_balises[] = {
        "texte_enrichi","document","annexes","contenu","section",
        "titre","liste","item","liste_texte","texte_liste",
        "texte","mot_enrichi","mot_important", "12" };

char mon_caractere;
FILE* fichier = NULL;
t_table_string lectureString = NULL;
t_table_arbre arbre_html = NULL;

// -- METODES STRUCTURE STRING
t_entree_string* nouvelle_entree_string(char etiquette) {
    a_entree_string poub = malloc(sizeof(t_entree_string));
    poub->l_etiquette = etiquette;
    poub->le_frere_suivant = NULL;
    return poub;

}
void creer_petit_frere(a_entree_string ceci, char etiquette) {
    if (ceci != NULL) {
        if (ceci->le_frere_suivant == NULL) {
            ceci->le_frere_suivant = nouvelle_entree_string(etiquette);
        } else {
            creer_petit_frere(ceci->le_frere_suivant, etiquette);
        }
    }
}
void afficher_abre_string(a_entree_string ceci) {
    if (ceci != NULL) {
        printf("%c" ,ceci->l_etiquette);
        afficher_abre_string(ceci->le_frere_suivant);
    }
}
char * returnStructEnChar(a_entree_string ceci, int longueur) {
    char* conteneurLettres = malloc(sizeof(char)*longueur);
    a_entree_string poub = ceci;
    int i = 0;
    do {
        conteneurLettres[i] = poub->l_etiquette;
        poub = poub->le_frere_suivant;
        i++;
    } while (poub != NULL);
    return conteneurLettres;
}
void ajouter_valeur_string(char etiquette) {
    if (lectureString == NULL) {
        lectureString = nouvelle_entree_string(etiquette);
    } else {
        creer_petit_frere(lectureString, etiquette);
    }
}
int comparaisonStruct_Balise(char* ceci) {
    for (int i = 0; i <= 12; i++) {
        if (strcmp(mes_balises[i], ceci)== 0) {
            return 1;
        }
    }
    return 0;
}


// -- METODES STRUCTURE ARBRE
t_entree_arbre* nouvelle_entree_arbre(char* etiquette, a_entree_arbre pere, a_entree_arbre gd_frere) {
    a_entree_arbre  poub = malloc(sizeof(t_entree_arbre));
    poub->l_etiquette = etiquette;
    poub->le_pere = pere;
    poub->le_frere_precedent = gd_frere;
    poub->le_frere_suivant = NULL;
    poub->le_premier_fils = NULL;
    poub->le_dernier_fils = NULL;
    return poub;
}
void afficher_entree_arbre(a_entree_arbre ceci) {
    printf("Voici le conteneu de ceci: %s \n", ceci->l_etiquette);
    printf("le pere : %p \n", ceci->le_pere);
    printf("son grand frere :  %p \n", ceci->le_frere_precedent);
    printf("son petit frere :  %p \n", ceci->le_frere_suivant);
    printf("son premier fils :  %p  \n", ceci->le_premier_fils);
    printf("son dernier fils : %p \n", ceci->le_dernier_fils);
}
void afifcherNPoints (int n) {
    for (int i = 0; i < n; i++) {
        printf("   ");
    }
}
void afficher_table_arbre(t_table_arbre ceci, int decalage) {
    if (ceci == NULL) {
        return;
    } else {
        afifcherNPoints(decalage);
        printf("%s \n", ceci->l_etiquette);
        afficher_table_arbre(ceci->le_premier_fils, decalage+1);
        afficher_table_arbre(ceci->le_frere_suivant, decalage);
    }
}
void creer_aine_arbre(t_table_arbre ceci, char* etiquette) { // ajouter en tete de liste
    t_entree_arbre* poub  = nouvelle_entree_arbre(etiquette, ceci, NULL);
    if (ceci->le_premier_fils != NULL) {
        poub->le_frere_suivant = ceci->le_premier_fils;
        (ceci->le_premier_fils)->le_frere_precedent = poub;
        ceci->le_premier_fils = poub;
    } else {
        ceci->le_premier_fils = poub;
        ceci->le_dernier_fils = poub;
    }
}
void creer_benjamin_arbre(a_entree_arbre ceci, char* etiquette) { // ajouter en fin de liste
    if (ceci->le_dernier_fils != NULL) {
        t_entree_arbre* poub  = nouvelle_entree_arbre(etiquette, ceci, ceci->le_dernier_fils);
        (ceci->le_dernier_fils)->le_frere_suivant = poub;
        ceci->le_dernier_fils = poub;
    } else {
        t_entree_arbre* poub  = nouvelle_entree_arbre(etiquette, ceci, NULL);
        ceci->le_premier_fils = poub;
        ceci->le_dernier_fils = poub;
    }
}
void creer_petit_frere_arbre(a_entree_arbre ceci, char* etiquette) {
    t_entree_arbre* poub  = nouvelle_entree_arbre(etiquette, ceci->le_pere, ceci);
    poub->le_frere_suivant = ceci->le_frere_suivant;
    if (poub->le_frere_suivant != NULL) {
        (poub->le_frere_suivant)->le_frere_precedent = poub;
    }
    if (ceci->le_frere_suivant == NULL && ceci->le_pere != NULL) {
        (ceci->le_pere)->le_dernier_fils = poub;
    }
    ceci->le_frere_suivant = poub;
}
void inserer_grand_frere_arbre(t_table_arbre * ceci, char* etiquette) {
    if ((*ceci)->le_frere_precedent != NULL) {
        creer_petit_frere_arbre((*ceci)->le_frere_precedent, etiquette);
    } else {
        a_entree_arbre poub = nouvelle_entree_arbre(etiquette, (*ceci)->le_pere, NULL);
        poub->le_frere_suivant = *ceci;
        (*ceci)->le_frere_precedent = poub;
        *ceci = poub;
    }
}

// -- DETECTION BALISE
int analyseMotBalise() {
    int i = 0;
    do {
        mon_caractere = (char) fgetc(fichier);
        if (mon_caractere ==  '>') {
            mon_caractere = ' ';
            break;
        }
        ajouter_valeur_string(mon_caractere);
        i++;
    } while (i < 15);
    char * resultatConversion = returnStructEnChar(lectureString, i);
    return comparaisonStruct_Balise(resultatConversion);
}
int estUneBaliseOuverte() {
        ajouter_valeur_string((char) fgetc(fichier));
        if (lectureString->l_etiquette == '/') { // on verifie que ce n'est pas une balise fermente
            fseek(fichier, -1, SEEK_CUR);
            lectureString = NULL;
            return 0;
        } else {
            return analyseMotBalise();
        }
}
int estUneBaliseFermante() {
        if ((char) fgetc(fichier) == '/') {
            return analyseMotBalise();
        }
        return 0;
}
// -- MAIN
void start(char le_fichier[]) {
    fichier = fopen(le_fichier, "r");
    if (fichier != NULL) {
        arbre_html = nouvelle_entree_arbre("body", NULL, NULL);
        do {
            mon_caractere = (char) fgetc(fichier);
            if (mon_caractere == '<') {
                if (estUneBaliseOuverte()) {
                    creer_benjamin_arbre(arbre_html, "aa");
                    arbre_html = arbre_html->le_dernier_fils;
                    printf("|");
                    afficher_abre_string(lectureString);
                    printf("|");
                } else {
                    afficher_abre_string(lectureString);
                }
                lectureString = NULL;
                if (estUneBaliseFermante()) {
                    if (arbre_html == NULL) {
                        printf("ne pas commencer par une balise fermante");
                        exit(-1);
                    } else {
                        arbre_html = arbre_html->le_pere;
                    }
                    printf("|");
                    afficher_abre_string(lectureString);
                    printf("|");
                } else {
                    afficher_abre_string(lectureString);
                }
                lectureString = NULL;
            }
            printf("%c", mon_caractere);
        } while (mon_caractere != EOF);
        fclose(fichier);
        printf("------\n");
        afficher_table_arbre(arbre_html, 0);
    }
}