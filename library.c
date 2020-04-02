#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>


void Cipher(intput_t* Input,
            ouput_t*  Output,
            key_sched_t* Key_tab,
            SBox_t* sbox,
            unsigned int Nb,
            unsigned int Nr)
{
    state_t* state = malloc(sizeof(state_t));

    memcpy(state,Input,sizeof(intput_t));

    AddRoundKey(state, &(Key_tab->tab[0]));
    int i = 0;
    for(i = 1 ; i < Nr ; i++) //segfault dans la boucle for()
    {
        SubBytes(state, sbox);
        ShiftRows(state); // segfault ici !
        MixColumns(state);
        AddRoundKey(state, &(Key_tab->tab[i]));
    }

    SubBytes(state, sbox);
    ShiftRows(state);
    AddRoundKey(state, &(Key_tab->tab[ (Nr + 1) * Nb - 1]));

    memcpy(Output, state, sizeof(state_t));
    free(state);
    return (void)0;
}



void SubBytes(state_t* state,
              SBox_t* sbox)
{
    unsigned int n = 16;

    BYTE I = 0;
    BYTE J = 0;
    for(int i = 0 ; i < 32 ; i++)
    {
        // tab[i][j] = arr[ i * ( n - 1 ) + j ]
        I = state->value[i] >> 4;
        J = ( state->value[i] << 4 ) >> 4;
        state->value[i] = sbox[ I * ( n - 1) + J ]; // on fait une traduction en gros
    }
    return (void)0;
}

void ShiftRows(state_t* state)
{
    unsigned int tmp1 = 0;
    unsigned int tmp2 = 0;
    unsigned int tmp3 = 0;
    unsigned int tmp4 = 0;
    int i = 0;
    int j = 0;
    for(i,j ; j < 4 ; j++) // 16 octets à traiter
    {
        tmp1 = state->value[i];
        tmp2 = state->value[i + 1];
        tmp3 = state->value[i + 2];
        tmp4 = state->value[i + 3];

        switch (j)
        {
            case 0:
                state->value[i]     += 0;
                state->value[i + 1] += 0;
                state->value[i + 2] += 0;
                state->value[i + 3] += 0;
                i += 4;
                break;
            case 1:
                state->value[i]     = tmp2;
                state->value[i + 1] = tmp3;
                state->value[i + 2] = tmp4;
                state->value[i + 3] = tmp1;
                i += 4;
                break;
            case 2:
                state->value[i]     = tmp3;
                state->value[i + 1] = tmp4;
                state->value[i + 2] = tmp1;
                state->value[i + 3] = tmp2;
                i += 4;
                break;
            case 3:
                state->value[i]     = tmp4;
                state->value[i + 1] = tmp1;
                state->value[i + 2] = tmp2;
                state->value[i + 3] = tmp3;
                i += 4;
                break;
            default:
                perror("ShiftRows error : switch case");
                abort();
                break;
        }
    }

    return (void)0;
}

void MixColumns(state_t* state)
{
    state_t* tmp_state = malloc(sizeof(state_t));
    for(int i = 0 ; i < 4 ; i++)
    {
        //printf("%d\n",i);
        tmp_state->value[i + 0]   = ( 2 * state->value[i + 0] ) ^ (3 * state->value[i + 4]) ^ (1 * state->value[i + 8]) ^ (1 * state->value[i + 12]);
        tmp_state->value[i + 4]   = ( 1 * state->value[i + 0] ) ^ (2 * state->value[i + 4]) ^ (3 * state->value[i + 8]) ^ (1 * state->value[i + 12]);
        tmp_state->value[i + 8]   = ( 1 * state->value[i + 0] ) ^ (1 * state->value[i + 4]) ^ (2 * state->value[i + 8]) ^ (3 * state->value[i + 12]);
        tmp_state->value[i + 12]  = ( 3 * state->value[i + 0] ) ^ (1 * state->value[i + 4]) ^ (1 * state->value[i + 8]) ^ (2 * state->value[i + 12]);
    }
    memcpy(state, tmp_state, sizeof(state_t));
    //free(tmp_state); //comprends pas pourquoi ca segfault

    return (void)0;
}


void AddRoundKey(state_t* state,
                 my_key_t*   RoundKey)
{
    for(int i = 0 ; i < 4 ; i++)
    {
        state->value[0  + i] = state->value[0  + i] ^ RoundKey->arr_key[0];
        state->value[4  + i] = state->value[4  + i] ^ RoundKey->arr_key[1];
        state->value[8  + i] = state->value[8  + i] ^ RoundKey->arr_key[2];
        state->value[12 + i] = state->value[12 + i] ^ RoundKey->arr_key[3];
    }
    return (void)0;
}

void RotWord(BYTE* word)
{
    BYTE tmp = 0;
    tmp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tmp;

    return (void)0;
}

void SubWord(BYTE* word,
             SBox_t* sbox)
{
    //sbox initialisé et remplie ailleurs, dans le main
    BYTE* tmp_word = malloc(4 * sizeof(BYTE));
    unsigned int n = 16; // nb d'element par ligne
    tmp_word[0] = sbox[   (word[0] >> 4)  * (n - 1) +  ( (word[0] << 4) >> 4) ];
    tmp_word[1] = sbox[   (word[1] >> 4)  * (n - 1) +  ( (word[1] << 4) >> 4) ];
    tmp_word[2] = sbox[   (word[2] >> 4)  * (n - 1) +  ( (word[2] << 4) >> 4) ];
    tmp_word[3] = sbox[   (word[3] >> 4)  * (n - 1) +  ( (word[3] << 4) >> 4) ];

    memcpy(word,tmp_word,4 * sizeof(BYTE));
    free(tmp_word);
    return (void)0;
}

void AllocKeySched(key_sched_t* key_sched,
                   unsigned int Nb,
                   unsigned int Nr)
{
    //create key plante parceque allockey fait pas bien son boulot FAUX tout va bien ici
    key_sched->tab = malloc(Nb * (Nr + 1) * sizeof(my_key_t));

    for(int i = 0 ; i < Nb * (Nr + 1) ; i++)
    {
        //printf("%d\n",i);
        key_sched->tab[i].arr_key = malloc(4 * sizeof(BYTE));
    }
    return (void)0;
}

// Key Expansion
void CreateKeySched(key_sched_t* key_sched,
                    my_key_t* key,
                    SBox_t* sbox,
                    rcon_t* Rcon,
                    unsigned int Nb,
                    unsigned int Nr,
                    unsigned int Nk)
{
    unsigned int i = 0;
    BYTE* temp = malloc(sizeof(BYTE) * 4);

    while(i < Nk)
    {
        key_sched->tab[i].arr_key[0] = key->arr_key[4 * i];
        key_sched->tab[i].arr_key[1] = key->arr_key[4 * i + 1];
        key_sched->tab[i].arr_key[2] = key->arr_key[4 * i + 2];
        key_sched->tab[i].arr_key[3] = key->arr_key[4 * i + 3];
        i = i + 1;
    }

    i = Nk;
    //printf("%d\n",Nb * (Nr +  1));
    while( i < Nb * (Nr + 1) ) // au bout d'un moment ça plante segfault dans le while()
    {
        //printf("%u\n",i);
        memcpy(temp,key_sched->tab[i - 1].arr_key,sizeof(BYTE)*4);
        if( (i % Nk) == 0)
        {
            RotWord(temp);
            SubWord(temp, sbox);
            for(int j = 0 ; j < 4 ; j++)
                temp[j] = (j == 0) ? temp[j] ^ Rcon[i/Nk] : temp[j] ^ 0b00000000; // tableau 2D en 1D, il faut que j apparaisse dedans
        // attention rcon contient juste les valeurs interessant, càd le premier octet,
        // les autres sont a zero { x^(i-1), 0, 0, 0  }
        }
        else if (  (Nk > 6) && (  (i % Nk) == 4)  )
        {
            SubWord(temp,sbox);
        }
        for(int j = 0 ; j < 4 ; j++)
            key_sched->tab[i].arr_key[j] = key_sched->tab[i - Nk].arr_key[j] ^ temp[j];// trouver comment ecrire ça ?

        i = i + 1;
    }

    free(temp);
    return (void)0;
}



