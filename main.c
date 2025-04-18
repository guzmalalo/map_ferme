#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#define TAILLE_TUILE 80 // 80 pixels

// PELOUSE =0, CHEMIN = 1 ...... NB_TUILES= 5
enum{PELOUSE, CHEMIN, EAU, MAIS, TOMATE, NB_TUILES};

// on charge uniquement une fois les images
// Attention -> pour chaque al_load_... il faut un al_destroy_...
void chargeImages(ALLEGRO_BITMAP * tuiles[NB_TUILES])
{
    tuiles[PELOUSE]=al_load_bitmap("../images/pelouse.png");
    tuiles[CHEMIN]=al_load_bitmap("../images/chemin.png");
    tuiles[EAU]=al_load_bitmap("../images/eau.png");
    tuiles[MAIS]=al_load_bitmap("../images/mais.png");
    tuiles[TOMATE]=al_load_bitmap("../images/tomate.png");
}

// On ouvre le fichier texte.
// On vient lire chaque int ligne par ligne
// et colonne par colonne.
// On initialise la map avec cette information.
void initMap(int map[10][10], char * cheminFichier)
{
    FILE * fichier = NULL;
    // On charge le fichier en mode lecture "Read (r)"
    fichier = fopen(cheminFichier, "r");

    if (fichier != NULL)
    {
        for (int i = 0; i < 10; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                // on vient lire un int à partir du "fichier"
                // et on le sauvegarde dans une position i,j du tableau.
             fscanf(fichier, "%d", &map[i][j]);
            }
        }

        // pour chaque fopen .. il faut un fclose
        fclose(fichier);
        fichier = NULL;
    }
}

void afficheMap(int map[10][10], ALLEGRO_BITMAP* tuiles[NB_TUILES])
{
    int id_tuile;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            id_tuile = map[i][j];
            al_draw_bitmap(tuiles[id_tuile],j*TAILLE_TUILE, i*TAILLE_TUILE,0);
        }
    }

}


int main(void)
{
    // declaration
    ALLEGRO_DISPLAY* fenetre = NULL;
    ALLEGRO_TIMER* timer = NULL;
    ALLEGRO_EVENT_QUEUE *file =NULL;
    ALLEGRO_EVENT event = {0};
    bool fini = false;
    char * cheminFichier = "../map.txt";

    int map[10][10] = {0};
    ALLEGRO_BITMAP * tuiles[NB_TUILES] = {0};

    // initialisation
    assert(al_init());
    assert(al_init_image_addon());

    // chargement des images et initialisation de la map
    chargeImages(tuiles);
    initMap(map, cheminFichier);

    // creation
    // - fenetre
    fenetre = al_create_display(800,800);
    al_set_window_title(fenetre, "ECE MAP");

    // - timer
    timer = al_create_timer(1.0/24.0);
    al_start_timer(timer);

    // - file
    file  = al_create_event_queue();

    // sources d'événements
    al_register_event_source(file, al_get_display_event_source(fenetre));
    al_register_event_source(file, al_get_timer_event_source(timer));

    // boucle principale
    while (!fini)
    {
        al_wait_for_event(file,&event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            fini = true;
            break;
        case ALLEGRO_EVENT_TIMER:
            afficheMap(map, tuiles);
            al_flip_display();
            break;
        }
    }

    // liberation ressources
    al_destroy_display(fenetre);
    al_destroy_timer(timer);
    for (int i = 0; i < NB_TUILES; ++i)
    {
        al_destroy_bitmap(tuiles[i]);
    }
    return 0;
}