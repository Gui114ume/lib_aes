#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


void Cipher(intput_t* Input,
            ouput_t*  Output,
            key_t*    Key) // a verifier, c'est peut etre pas la cle qu'il faut
{
    unsigned int Nr = Nr10; // a detecter en fonction de la taille de la key

    state_t* state = malloc(sizeof(state_t));

    memcpy(state,Input,sizeof(intput_t));

    AddRoundKey(state, Key);
    int i = 0;
    for(i = 0 ; i < Nr - 1 ; i++)
    {
        SubBytes(state, sbox);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, Key);
    }

    SubBytes(state, sbox);
    ShiftRows(state);
    AddRoundKey(state, Key(i));

    memcpy(Output, state, sizeof(state_t));
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
        I = state[i] >> 4;
        J = ( state[i] << 4 ) >> 4;
        state[i] = sbox->content[ I * ( n - 1) + J ]; // on fait une traduction en gros
    }
    return (void)0;
}

void ShiftRows(state_t* state)
{
    unsigned int tmp1 = 0;
    unsigned int tmp2 = 0;
    unsigned int tmp3 = 0;
    unsigned int tmp4 = 0;
    for(int i, j = 0 ; j < 4 ; j++) // 16 octets à traiter
    {
        tmp1 = state->value[i];
        tmp2 = state->value[i + 1];
        tmp3 = state->value[i + 2]
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
        tmp_state->value[i + 0]   = ( 2 * state->value[i + 0] ) ^ (3 * state->value[i + 4]) ^ (1 * state->value[i + 8]) ^ (1 * state->value[i + 12]);
        tmp_state->value[i + 4]   = ( 1 * state->value[i + 0] ) ^ (2 * state->value[i + 4]) ^ (3 * state->value[i + 8]) ^ (1 * state->value[i + 12]);
        tmp_state->value[i + 8]   = ( 1 * state->value[i + 0] ) ^ (1 * state->value[i + 4]) ^ (2 * state->value[i + 8]) ^ (3 * state->value[i + 12]);
        tmp_state->value[i + 12]  = ( 3 * state->value[i + 0] ) ^ (1 * state->value[i + 4]) ^ (1 * state->value[i + 8]) ^ (2 * state->value[i + 12]);
    }
    memcpy(state, tmp_state, sizeof(state_t));
    free(tmp_state);
    return (void)0;
}


void AddRoundKey(state_t* state,
                 key_t*   RoundKey)
{

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
    BYTE* tmp_word = malloc(4 * sizeof(BYTE));

    tmp_word[0] = sbox->content[   (word[0] >> 4)  * (n - 1) +  ( (word[0] << 4) >> 4) ];
    tmp_word[1] = sbox->content[   (word[1] >> 4)  * (n - 1) +  ( (word[1] << 4) >> 4) ];
    tmp_word[2] = sbox->content[   (word[2] >> 4)  * (n - 1) +  ( (word[2] << 4) >> 4) ];
    tmp_word[3] = sbox->content[   (word[3] >> 4)  * (n - 1) +  ( (word[3] << 4) >> 4) ];

    memcpy(word,tmp_word,4 * sizeof(BYTE));
    free(tmp_word);
    return (void)0;
}