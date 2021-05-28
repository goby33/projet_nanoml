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
int max_ligne = 50;

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
int returnLongueur(t_table_string ceci) {
    int i = 0;
    do {
        ceci = ceci->le_frere_suivant;
        i++;
    } while  (ceci != NULL);
    return i;
}

// traitement balise
void afficher_trait(int nombre) {
    printf("+");
    for (int i = 0; i <= nombre-2; i++) {
        printf("-");
    }
    printf("+\n");
}
void T_document() {
    afficher_trait(50);
    max_ligne -= 2;
}
void T_section() {
    afficher_trait(max_ligne);
}


// -- METODES STRUCTURE ARBRE
t_entree_arbre* nouvelle_entree_arbre(t_table_string etiquette, a_entree_arbre pere, a_entree_arbre gd_frere, t_balise balise) {
    a_entree_arbre  poub = malloc(sizeof(t_entree_arbre));
    poub->l_etiquette = etiquette;
    poub->balise = balise;
    poub->le_pere = pere;
    poub->le_frere_precedent = gd_frere;
    poub->le_frere_suivant = NULL;
    poub->le_premier_fils = NULL;
    poub->le_dernier_fils = NULL;
    return poub;
}
void afifcherNPoints (int n) {
    for (int i = 0; i < n; i++) {
        printf("   ");
    }
}
void afficher_table_arbre(t_table_arbre ceci) {
    if (ceci == NULL) {
        return;
    } else {
        if (ceci->balise == document) {
            T_document();
        } else if (ceci->balise == section) {
            T_section();
        } else if (ceci->balise == texte) {
            afficher_abre_string(ceci->l_etiquette);
        }
        //printf("|%s|", mes_balises[ceci->balise]);
       // printf("%s", returnStructEnChar(ceci->l_etiquette, returnLongueur(ceci->l_etiquette)));
        printf("\n");
        afficher_table_arbre(ceci->le_premier_fils);
        afficher_table_arbre(ceci->le_frere_suivant);
    }
}
void creer_benjamin_arbre(a_entree_arbre ceci, t_table_string etiquette, t_balise balise) { // ajouter en fin de liste
    if (ceci->le_dernier_fils != NULL) {
        t_entree_arbre* poub  = nouvelle_entree_arbre(etiquette, ceci, ceci->le_dernier_fils, balise);
        (ceci->le_dernier_fils)->le_frere_suivant = poub;
        ceci->le_dernier_fils = poub;
    } else {
        t_entree_arbre* poub  = nouvelle_entree_arbre(etiquette, ceci, NULL, balise);
        ceci->le_premier_fils = poub;
        ceci->le_dernier_fils = poub;
    }
}

// -- DETECTION BALISE
int returnType(t_table_string ceci) {
    char * ma_balise = returnStructEnChar(ceci, returnLongueur(ceci));
    for (int i = 0; i < 12; i++) {
        if (strcmp(ma_balise, mes_balises[i])) {
            return 1;
        }
    }
    return -1;
}
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
    if (comparaisonStruct_Balise(resultatConversion)) {
        return 1;
    }
    return 0;
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
void lectureLettre() {
    do {
        mon_caractere = (char) fgetc(fichier);
        if (mon_caractere == '<') {
            break;
        }
        if (mon_caractere != '\n') {
            ajouter_valeur_string(mon_caractere);
        }

    } while (mon_caractere != EOF);
    fseek(fichier, -1, SEEK_CUR);
}
// -- MAIN
void start(char le_fichier[]) {
    fichier = fopen(le_fichier, "r");
    if (fichier != NULL) {
        arbre_html = nouvelle_entree_arbre(NULL, NULL, NULL, 0);
        do {
            mon_caractere = (char) fgetc(fichier);
            if (mon_caractere == '<') {
                if (estUneBaliseOuverte()) {
                    creer_benjamin_arbre(arbre_html, lectureString, returnType(lectureString));
                    arbre_html = arbre_html->le_dernier_fils;
                    lectureString = NULL;
                }
                else if(estUneBaliseFermante()) {
                    if (arbre_html == NULL) {
                        printf("ne pas commencer par une balise fermante");
                        exit(-1);
                    } else {
                        arbre_html = arbre_html->le_pere;
                    }
                } else {
                    creer_benjamin_arbre(arbre_html, lectureString, texte);
                }
                lectureString = NULL;
            } else {
                lectureLettre();
                creer_benjamin_arbre(arbre_html, lectureString, texte);
                lectureString = NULL;
            }
        } while (mon_caractere != EOF);
        fclose(fichier);
        printf("------\n");
        afficher_table_arbre(arbre_html);
    }
}