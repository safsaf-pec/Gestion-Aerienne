#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vol {
    char numero[10];
    char aeroport_depart[50];
    char aeroport_arrivee[50];
    char heure_depart[20];
    char heure_arrivee[20];
    int sieges_eco;
    int sieges_aff;
    int sieges_prem;
    float cout;

    struct Vol* suivant;
} Vol;

Vol* tete = NULL;

void nettoyer_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lire_chaine(char* buffer, int taille) {
    fgets(buffer, taille, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

Vol* creer_vol() {
    Vol* nv = (Vol*)malloc(sizeof(Vol));
    if (!nv) {
        printf("Erreur d'allocation memoire.\n");
        return NULL;
    }

    printf("Numero du vol: ");
    lire_chaine(nv->numero, sizeof(nv->numero));

    printf("Aeroport de depart: ");
    lire_chaine(nv->aeroport_depart, sizeof(nv->aeroport_depart));

    printf("Aeroport de destination: ");
    lire_chaine(nv->aeroport_arrivee, sizeof(nv->aeroport_arrivee));

    printf("Heure de depart (YYYY-MM-DD HH:MM): ");
    lire_chaine(nv->heure_depart, sizeof(nv->heure_depart));

    printf("Heure d'arrivee (YYYY-MM-DD HH:MM): ");
    lire_chaine(nv->heure_arrivee, sizeof(nv->heure_arrivee));

    printf("Nombre de sieges disponibles :\n");
    printf("  Classe Economique : ");
    scanf("%d", &nv->sieges_eco);
    printf("  Classe Affaires : ");
    scanf("%d", &nv->sieges_aff);
    printf("  Premiere Classe : ");
    scanf("%d", &nv->sieges_prem);

    printf("Entrer le cout du billet : ");
    scanf("%f", &nv->cout);

    nettoyer_buffer();
    nv->suivant = NULL;
    return nv;
}

void ajouter_vol() {
    int choix;
    printf("1. Ajouter au debut\n2. Ajouter apres un vol\n3. Ajouter a la fin\nVotre choix: ");
    scanf("%d", &choix);
    nettoyer_buffer();

    Vol* nv = creer_vol();
    if (!nv) return;

    if (choix == 1) {
        nv->suivant = tete;
        tete = nv;
    } else if (choix == 2) {
        char num[10];
        printf("Entrer le numero du vol apres lequel ajouter: ");
        lire_chaine(num, sizeof(num));

        Vol* temp = tete;
        while (temp && strcmp(temp->numero, num) != 0)
            temp = temp->suivant;
        if (!temp) {
            printf("Vol non trouve.\n");
            free(nv);
            return;
        }
        nv->suivant = temp->suivant;
        temp->suivant = nv;
    } else if (choix == 3) {
        if (!tete) {
            tete = nv;
        } else {
            Vol* temp = tete;
            while (temp->suivant)
                temp = temp->suivant;
            temp->suivant = nv;
        }
    }
    printf("Vol ajoute avec succes.\n");
}

void modifier_vol() {
    char num[10];
    printf("Entrez le numero du vol a modifier: ");
    lire_chaine(num, sizeof(num));

    Vol* temp = tete;
    while (temp) {
        if (strcmp(temp->numero, num) == 0) {
            printf("Modifier les informations du vol:\n");
            printf("Aeroport de depart: ");
            lire_chaine(temp->aeroport_depart, sizeof(temp->aeroport_depart));
            printf("Aeroport de destination: ");
            lire_chaine(temp->aeroport_arrivee, sizeof(temp->aeroport_arrivee));
            printf("Heure de depart: ");
            lire_chaine(temp->heure_depart, sizeof(temp->heure_depart));
            printf("Heure d'arrivee: ");
            lire_chaine(temp->heure_arrivee, sizeof(temp->heure_arrivee));
            printf("Nombre de sieges (Eco / Affaires / Premiere): ");
            scanf("%d %d %d", &temp->sieges_eco, &temp->sieges_aff, &temp->sieges_prem);
            printf("Cout du billet : ");
            scanf("%f", &temp->cout);
            nettoyer_buffer();
            printf("Vol modifie avec succes.\n");
            return;
        }
        temp = temp->suivant;
    }
    printf("Vol non trouve.\n");
}

void supprimer_vol() {
    int choix;
    printf("1. Supprimer au debut\n2. Supprimer apres un vol\n3. Supprimer a la fin\nVotre choix: ");
    scanf("%d", &choix);
    nettoyer_buffer();

    if (!tete) {
        printf("Aucun vol a supprimer.\n");
        return;
    }

    if (choix == 1) {
        Vol* temp = tete;
        tete = tete->suivant;
        free(temp);
    } else if (choix == 2) {
        char num[10];
        printf("Entrer le numero du vol AVANT celui a supprimer: ");
        lire_chaine(num, sizeof(num));

        Vol* temp = tete;
        while (temp->suivant && strcmp(temp->numero, num) != 0)
            temp = temp->suivant;
        if (!temp || !temp->suivant) {
            printf("Vol non trouve ou aucun vol apres celui-ci.\n");
            return;
        }
        Vol* supp = temp->suivant;
        temp->suivant = supp->suivant;
        free(supp);
    } else if (choix == 3) {
        Vol* temp = tete;
        Vol* prev = NULL;
        while (temp->suivant) {
            prev = temp;
            temp = temp->suivant;
        }
        if (!prev) {
            free(tete);
            tete = NULL;
        } else {
            prev->suivant = NULL;
            free(temp);
        }
    }
    printf("Vol supprime avec succes.\n");
}

void afficher_vols() {
    Vol* temp = tete;
    while (temp) {
        printf("Vol: %s | Depart: %s | Destination: %s\n", temp->numero, temp->aeroport_depart, temp->aeroport_arrivee);
        printf("Heure depart: %s | Heure arrivee: %s\n", temp->heure_depart, temp->heure_arrivee);
        printf("Sieges - Eco: %d | Affaires: %d | Premiere: %d\n", temp->sieges_eco, temp->sieges_aff, temp->sieges_prem);
        printf("Cout du billet : %.2f\n", temp->cout);
        printf("--------------------------------------------------\n");
        temp = temp->suivant;
    }
}

void rechercher_vol() {
    char num[10];
    printf("Entrez le numero du vol a rechercher: ");
    lire_chaine(num, sizeof(num));

    Vol* temp = tete;
    while (temp) {
        if (strcmp(temp->numero, num) == 0) {
            printf("Vol trouve:\n");
            printf("Depart: %s, Destination: %s\n", temp->aeroport_depart, temp->aeroport_arrivee);
            printf("Heure depart: %s, Heure arrivee: %s\n", temp->heure_depart, temp->heure_arrivee);
            return;
        }
        temp = temp->suivant;
    }
    printf("Vol non trouve.\n");
}

void verifier_disponibilite() {
    char num[10];
    printf("-- Verifier la disponibilite des sieges --\n");
    printf("Entrez le numero du vol : ");
    lire_chaine(num, sizeof(num));

    Vol* temp = tete;
    while (temp) {
        if (strcmp(temp->numero, num) == 0) {
            printf("\nDetails du vol :\n");
            printf("- Vol : %s\n- Depart : %s\n- Destination : %s\n", temp->numero, temp->aeroport_depart, temp->aeroport_arrivee);
            printf("- Heure de depart : %s\n- Heure d'arrivee : %s\n", temp->heure_depart, temp->heure_arrivee);
            printf("\nNombre de sieges disponibles :\n");
            printf("- Classe Economique : %d\n", temp->sieges_eco);
            printf("- Classe Affaires : %d\n", temp->sieges_aff);
            printf("- Premiere Classe : %d\n", temp->sieges_prem);
            return;
        }
        temp = temp->suivant;
    }
    printf("Vol non trouve.\n");
}

int menu() {
    int ch1;
    printf("\nMenu principal:\n");
    printf("1->Gestion vols aeriens\n");
    printf("2->Quitter\n");
    printf("Entrer votre choix : ");
    scanf("%d", &ch1);
    nettoyer_buffer();
    return ch1;
}

int main() {
    int choix1, choix;
    while ((choix1 = menu()) != 2) {
        do {
            printf("\nGestion vols aeriens:\n");
            printf("1. Ajouter un vol\n2. Supprimer un vol\n3. Modifier un vol\n4. Afficher les vols\n5. Rechercher un vol\n6. Verifier la disponibilite des sieges\n0. Retour menu principal\n");
            printf("Votre choix: ");
            scanf("%d", &choix);
            nettoyer_buffer();
            switch (choix) {
                case 1: ajouter_vol(); break;
                case 2: supprimer_vol(); break;
                case 3: modifier_vol(); break;
                case 4: afficher_vols(); break;
                case 5: rechercher_vol(); break;
                case 6: verifier_disponibilite(); break;
                case 0: break;
                default: printf("Choix invalide.\n");
            }
        } while (choix != 0);
    }
    printf("Au revoir !\n");
    return 0;
}

