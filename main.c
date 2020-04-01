#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "library.h"

int main(int argc, char* argv[], char* varenv[])
{

    // Etapes du programme

    // 1. Récupérer la clé AES
    // 2. Ouvrir le fichier à chiffrer, le fichier de sortie
    // 3. Creation des cles de chiffrements
    // 4. Chiffrer des blocs de 128 bits jusqu'à la fin du fichier (on suppose pas de padding)
    // 5. On ecrit le fichier chiffré au fur et à mesure du chiffrement

    if(argc < 2)
    {
        printf("usage : %s [fichier_key] [fichier_clair] [fichier_chiffre]\n",argv[0]);
        return -1;
    }
// Recuperation de la cle AES //////////////////////////////
    FILE* fptr_key = fopen(argv[1],"rb");
    unsigned int key_length = 0;

    my_key_t* key = malloc(sizeof(key_t));
    key->arr_key = malloc(sizeof(BYTE) * 256);

    key_length = fread( key->arr_key , sizeof(BYTE), 256, fptr_key);
    if(key_length < 0)
    {
        perror("ouverture de la key");
        exit(-1);
    }
    key->arr_key = realloc(key->arr_key, key_length);
    key->key_length = key_length / 32; // taille en nb de mots de 32 bits
    fclose(fptr_key);
    unsigned int Nb = 4;// AES 128 dans la cas test

    unsigned int Nk = key->key_length; //normalement ça il faut le recuperer qqpart
    unsigned int Nr = Nb + Nk + 2;
/////////////////////////////////////////////////////////////
// Ouverture du fichier à chiffrer

    FILE* fptr_msg = fopen(argv[2],"rb");
    state_t* input = malloc(sizeof(state_t));
    //input->value = malloc(128 * sizeof(BYTE));
    unsigned int read_bytes = 0;
/////////////////////////////////////////////////////////////
// Ouverture du fichier en sortie ( pour ecriture )

    FILE* fptr_out = fopen(argv[3], "wb");
    ouput_t* ouput = malloc(sizeof(ouput_t));
    //ouput->value = malloc(128 * sizeof(BYTE));
    unsigned int nb_bytes_to_write = 128;
// Creation des differentes cles de chiffrement
/////////////////////////////////////////////////////////////

    key_sched_t* Key_tab = malloc(sizeof(key_sched_t));
    AllocKeySched(Key_tab, Nb, Nr);
    CreateKeySched(Key_tab, key, glob_sbox, glob_rcon, Nb, Nr, Nk);

/////////////////////////////////////////////////////////////
// Boucle de lecture, chiffrement, ecriture

    while(read_bytes = fread(input->value, sizeof(BYTE),128, fptr_msg) )
    {
        Cipher(input, ouput, Key_tab, glob_sbox, Nb, Nr);
        fwrite(ouput->value, sizeof(BYTE), nb_bytes_to_write, fptr_out);
    }
    fclose(fptr_msg);
    fclose(fptr_out);
    return 1;
}