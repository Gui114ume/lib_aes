#ifndef LIB_AES_H
#define LIB_AES_H

// Key Length in words of 32 bits
#define Nk4 4
#define Nk6 6
#define Nk8 8

// Number of rounds
#define Nr10 10
#define Nr12 12
#define Nr14 14

// Block Size in words of 32 bits
#define Nb 4

typedef unsigned char BYTE;
typedef unsigned int  WORD_t;

typedef struct state_s
{
    WORD_t value[4];
} state_t;

typedef struct key_s
{
    BYTE* arr_key;
    unsigned int key_length;
} key_t;

typedef struct rcon_s
{
    WORD_t* content;
}rcon_t;

typedef struct SBox_s
{
    BYTE content[16][16];
}SBox_t;

typedef state_t intput_t;
typedef state_t ouput_t;

void Cipher(intput_t* Input,
            ouput_t*  Output);

void AddRoundKey(state_t* state);

void SubBytes(state_t* state,
              SBox_t* sbox);

void ShiftRows(state_t* state);

void MixColumns(state_t* state);

void AddRoundKey(state_t* state,
                 ROUND_KEY); //je ne sais pas encore ce que c'est


WORD_t RotWord(WORD_t* word);

WORD_t SubWord(WORD_t* word);


void InvMixColumns();
void InvShiftRows();
void InvSubBytes();



#endif
