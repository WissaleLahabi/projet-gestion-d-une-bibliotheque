#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************************************************
                        Structures et Définitions de Données
********************************************************************************************/

// Définir une structure pour le livre
typedef struct Livre {
    int id;
    char titre[100];
    char auteur[100];
    struct Livre* suivant;
} Livre;

// Définir une structure pour la pile (livres empruntés)
typedef struct NoeudPile {
    Livre* livre;
    struct NoeudPile* suivant;
} NoeudPile;

// Définir une structure pour la file d'attente (demandes de livres)
typedef struct NoeudFile {
    Livre* livre;
    struct NoeudFile* suivant;
} NoeudFile;

// Définir une structure pour l'arbre (livres triés)
typedef struct NoeudArbre {
    Livre* livre;
    struct NoeudArbre* gauche;
    struct NoeudArbre* droite;
} NoeudArbre;

Livre* listeLivres = NULL;
NoeudPile* livresEmpruntes = NULL;
NoeudFile* demandesLivres = NULL;
NoeudArbre* arbreLivres = NULL;

/********************************************************************************************
                        Fonctions pour Gérer la Liste des Livres
********************************************************************************************/

void ajouterLivre(int id, char* titre, char* auteur) {
    Livre* nouveauLivre = (Livre*)malloc(sizeof(Livre));
    nouveauLivre->id = id;
    strcpy(nouveauLivre->titre, titre);
    strcpy(nouveauLivre->auteur, auteur);
    nouveauLivre->suivant = listeLivres;
    listeLivres = nouveauLivre;
}

void supprimerLivre(int id) {
    Livre* actuel = listeLivres;
    Livre* precedent = NULL;

    while (actuel != NULL && actuel->id != id) {
        precedent = actuel;
        actuel = actuel->suivant;
    }

    if (actuel == NULL) {
        printf("Livre non trouvé.\n");
        return;
    }

    if (precedent == NULL) {
        listeLivres = actuel->suivant;
    } else {
        precedent->suivant = actuel->suivant;
    }

    free(actuel);
    printf("Livre supprimé avec succès.\n");
}

void modifierLivre(int id, char* nouveauTitre, char* nouveauAuteur) {
    Livre* livre = listeLivres;
    while (livre != NULL && livre->id != id) {
        livre = livre->suivant;
    }

    if (livre == NULL) {
        printf("Livre non trouvé.\n");
        return;
    }

    strcpy(livre->titre, nouveauTitre);
    strcpy(livre->auteur, nouveauAuteur);
    printf("Livre modifié avec succès.\n");
}

void afficherLivres() {
    Livre* actuel = listeLivres;
    while (actuel != NULL) {
        printf("ID: %d, Titre: %s, Auteur: %s\n", actuel->id, actuel->titre, actuel->auteur);
        actuel = actuel->suivant;
    }
}

/********************************************************************************************
                        Fonctions pour Gérer la Pile de Livres Empruntés
********************************************************************************************/

void emprunterLivre(int id) {
    Livre* livre = listeLivres;
    while (livre != NULL && livre->id != id) {
        livre = livre->suivant;
    }

    if (livre == NULL) {
        printf("Livre non trouvé.\n");
        return;
    }

    NoeudPile* nouveauNoeud = (NoeudPile*)malloc(sizeof(NoeudPile));
    nouveauNoeud->livre = livre;
    nouveauNoeud->suivant = livresEmpruntes;
    livresEmpruntes = nouveauNoeud;
    printf("Livre emprunté avec succès.\n");
}

void retournerLivre() {
    if (livresEmpruntes == NULL) {
        printf("Aucun livre à retourner.\n");
        return;
    }

    NoeudPile* noeud = livresEmpruntes;
    livresEmpruntes = livresEmpruntes->suivant;
    printf("Livre retourné : ID: %d, Titre: %s, Auteur: %s\n", noeud->livre->id, noeud->livre->titre, noeud->livre->auteur);
    free(noeud);
}

/********************************************************************************************
                        Fonctions pour Gérer la File d'Attente des Demandes de Livres
********************************************************************************************/

void demanderLivre(int id) {
    Livre* livre = listeLivres;
    while (livre != NULL && livre->id != id) {
        livre = livre->suivant;
    }

    if (livre == NULL) {
        printf("Livre non trouvé.\n");
        return;
    }

    NoeudFile* nouveauNoeud = (NoeudFile*)malloc(sizeof(NoeudFile));
    nouveauNoeud->livre = livre;
    nouveauNoeud->suivant = NULL;

    if (demandesLivres == NULL) {
        demandesLivres = nouveauNoeud;
    } else {
        NoeudFile* actuel = demandesLivres;
        while (actuel->suivant != NULL) {
            actuel = actuel->suivant;
        }
        actuel->suivant = nouveauNoeud;
    }
    printf("Demande de livre ajoutée avec succès.\n");
}

void traiterDemande() {
    if (demandesLivres == NULL) {
        printf("Aucune demande à traiter.\n");
        return;
    }

    NoeudFile* noeud = demandesLivres;
    demandesLivres = demandesLivres->suivant;
    printf("Demande traitée : ID: %d, Titre: %s, Auteur: %s\n", noeud->livre->id, noeud->livre->titre, noeud->livre->auteur);
    free(noeud);
}

/********************************************************************************************
                        Fonctions pour Gérer l'Arbre de Livres
********************************************************************************************/

NoeudArbre* ajouterNoeudArbre(NoeudArbre* racine, Livre* livre) {
    if (racine == NULL) {
        NoeudArbre* nouveauNoeud = (NoeudArbre*)malloc(sizeof(NoeudArbre));
        nouveauNoeud->livre = livre;
        nouveauNoeud->gauche = NULL;
        nouveauNoeud->droite = NULL;
        return nouveauNoeud;
    }

    if (livre->id < racine->livre->id) {
        racine->gauche = ajouterNoeudArbre(racine->gauche, livre);
    } else {
        racine->droite = ajouterNoeudArbre(racine->droite, livre);
    }

    return racine;
}

void ajouterLivreArbre(int id, char* titre, char* auteur) {
    Livre* nouveauLivre = (Livre*)malloc(sizeof(Livre));
    nouveauLivre->id = id;
    strcpy(nouveauLivre->titre, titre);
    strcpy(nouveauLivre->auteur, auteur);
    nouveauLivre->suivant = NULL;

    arbreLivres = ajouterNoeudArbre(arbreLivres, nouveauLivre);
    printf("Livre ajouté à l'arbre avec succès.\n");
}

void afficherArbre(NoeudArbre* racine) {
    if (racine != NULL) {
        afficherArbre(racine->gauche);
        printf("ID: %d, Titre: %s, Auteur: %s\n", racine->livre->id, racine->livre->titre, racine->livre->auteur);
        afficherArbre(racine->droite);
    }
}

/********************************************************************************************
                        Fonctions de Sauvegarde et de Chargement
********************************************************************************************/

void sauvegarderLivres(const char* cheminFichier) {
    FILE* fichier = fopen(cheminFichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }

    Livre* actuel = listeLivres;
    while (actuel != NULL) {
        fprintf(fichier, "%d\n%s\n%s\n", actuel->id, actuel->titre, actuel->auteur);
        actuel = actuel->suivant;
    }

    fclose(fichier);
    printf("Données sauvegardées avec succès.\n");
}

void chargerLivres(const char* cheminFichier) {
    FILE* fichier = fopen(cheminFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour le chargement.\n");
        return;
    }

    while (!feof(fichier)) {
        int id;
        char titre[100];
        char auteur[100];

        if (fscanf(fichier, "%d\n", &id) != 1) break;
        fgets(titre, sizeof(titre), fichier);
        fgets(auteur, sizeof(auteur), fichier);

        // Enlever le saut de ligne à la fin des chaînes lues
        titre[strcspn(titre, "\n")] = 0;
        auteur[strcspn(auteur, "\n")] = 0;

        ajouterLivre(id, titre, auteur);
    }

    fclose(fichier);
    printf("Données chargées avec succès.\n");
}

/********************************************************************************************
                                Fonction Principale
********************************************************************************************/

void afficherMenu() {
    printf("\nSystème de Gestion de Bibliothèque\n");
    printf("1. Ajouter un livre\n");
    printf("2. Supprimer un livre\n");
    printf("3. Modifier un livre\n");
    printf("4. Afficher tous les livres\n");
    printf("5. Emprunter un livre\n");
    printf("6. Retourner un livre\n");
    printf("7. Demander un livre\n");
    printf("8. Traiter une demande\n");
    printf("9. Ajouter un livre à l'arbre\n");
    printf("10. Afficher les livres dans l'arbre\n");
    printf("11. Sauvegarder les données\n");
    printf("12. Charger les données\n");
    printf("0. Quitter\n");
}

int main() {
    const char* cheminFichier = "C:\WissalProject\Storage.txt";
    int choix, id;
    char titre[100], auteur[100];

    // Charger les données au démarrage
    chargerLivres(cheminFichier);

    while (1) {
        afficherMenu();
        printf("Entrez votre choix: ");
        scanf("%d", &choix);

        switch (choix) {
        	printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
            case 1:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Entrez l'ID du livre: ");
                scanf("%d", &id);
                printf("Entrez le titre du livre: ");
                scanf(" %[^\n]%*c", titre);
                printf("Entrez l'auteur du livre: ");
                scanf(" %[^\n]%*c", auteur);
                ajouterLivre(id, titre, auteur);
                break;
            case 2:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Entrez l'ID du livre à supprimer: ");
                scanf("%d", &id);
                supprimerLivre(id);
                break;
            case 3:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Entrez l'ID du livre à modifier: ");
                scanf("%d", &id);
                printf("Entrez le nouveau titre du livre: ");
                scanf(" %[^\n]%*c", titre);
                printf("Entrez le nouvel auteur du livre: ");
                scanf(" %[^\n]%*c", auteur);
                modifierLivre(id, titre, auteur);
                break;
            case 4:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                afficherLivres();
                break;
            case 5:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Entrez l'ID du livre à emprunter: ");
                scanf("%d", &id);
                emprunterLivre(id);
                break;
            case 6:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                retournerLivre();
                break;
            case 7:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Entrez l'ID du livre à demander: ");
                scanf("%d", &id);
                demanderLivre(id);
                break;
            case 8:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                traiterDemande();
                break;
            case 9:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Entrez l'ID du livre: ");
                scanf("%d", &id);
                printf("Entrez le titre du livre: ");
                scanf(" %[^\n]%*c", titre);
                printf("Entrez l'auteur du livre: ");
                scanf(" %[^\n]%*c", auteur);
                ajouterLivreArbre(id, titre, auteur);
                break;
            case 10:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                afficherArbre(arbreLivres);
                break;
            case 11:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                sauvegarderLivres(cheminFichier);
                break;
            case 12:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                chargerLivres(cheminFichier);
                break;
            case 0:
			printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                // Sauvegarder les données avant de quitter
                sauvegarderLivres(cheminFichier);
                exit(0);
            default:
            	printf("\n");for(int i=0; i<50;i++)printf("_");printf("\n");
                printf("Choix invalide. Veuillez réessayer.\n");
        }
        
    }

    return 0;
}

