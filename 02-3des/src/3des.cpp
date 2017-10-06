#include "3des.h"
#include <vector>
#include <array>
#include <bitset>
#include <iostream>
#ifndef NDEBUG
#define DEBUG_OUTPUT(s) cout << (s) << endl; cout.flush();
#else
#define DEBUG_OUTPUT(s) ;
#endif

const array<size_t, 64> IP = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
};

const array<size_t, 64> KEY_PERM = {
    57, 49, 41, 33, 25, 17, 9, 8,
    1, 58, 50, 42, 34, 26, 18, 16,
    10, 2, 59, 51, 43, 35, 27, 24,
    19, 11, 3, 60, 52, 44, 36, 32,
    63, 55, 47, 39, 31, 23, 15, 40,
    7, 62, 54, 46, 38, 30, 22, 48,
    14, 6, 61, 53, 45, 37, 29, 56,
    21, 13, 5, 28, 20, 12, 4, 64,
};

const array<size_t, 48> KEY_BITS = {
    14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32,
};

const array<size_t, 48> HALFBLOCK_BITS = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1,
};

const array<array<array<uint32_t, 16>, 4>, 8> S_BLOCKS = {{
    {{
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    }},
    {{
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    }},
    {{
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    }},
    {{
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    }},
    {{
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    }},
    {{
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    }},
    {{
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    }},
    {{
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
    }},
}};

TripleDES::TripleDES(array<uint64_t, 3> keys) : keys(keys)
{
    DEBUG_OUTPUT("Created TripleDES");
}

array<size_t, 64> shift_perm(array<size_t, 64> perm)
{
    array<size_t, 64> result;
    result[0] = perm[30];
    for (int i = 1; i <= 31; i++)
    {
        if (i % 8 == 7)
        {
            result[i] = perm[i];
        }
        else
        {
            result[i] = perm[(i % 8 == 0) ? i - 2 : i - 1];
        }
    }
    result[32] = perm[62];
    for (int i = 33; i <= 63; i++)
    {
        if (i % 8 == 7)
        {
            result[i] = perm[i];
        }
        else
        {
            result[i] = perm[(i % 8 == 0) ? i - 2 : i - 1];
        }
    }
    return result;
}

uint64_t TripleDES::encrypt_block(uint64_t block)
{
    // EDE3
    DEBUG_OUTPUT("Encrypting block");
    return des_encrypt_stage(des_decrypt_stage(des_encrypt_stage(block, keys[0]), keys[1]), keys[2]);
}

uint64_t TripleDES::decrypt_block(uint64_t block)
{
    // EDE3
    DEBUG_OUTPUT("Decrypting block");
    return des_decrypt_stage(des_encrypt_stage(des_decrypt_stage(block, keys[2]), keys[1]), keys[0]);
}

string TripleDES::encrypt_message(string message)
{
    DEBUG_OUTPUT("Encrypting message");
    // number of blocks needed
    size_t block_count = (message.size() + 7) / 8;
    // converting string to blocks
    vector<uint64_t> blocks(block_count);
    for (int i = 0; i < message.size(); i++)
    {
        uint8_t c = message[i];
        blocks[i / 8] |= (uint64_t(c) << (i % 8 * 8));
    }
    // encrypting blocks
    vector<uint64_t> encrypted_blocks(block_count);
    for (int i = 0; i < block_count; i++)
    {
        encrypted_blocks[i] = encrypt_block(blocks[i]);
    }
    // converting blocks back to string
    string result;
    result.resize(block_count * 8, '\0');
    for (int i = 0; i < block_count; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            result[i * 8 + j] = uint8_t((encrypted_blocks[i] >> (j * 8)) & 255);
        }
    }
    return result;
}

string TripleDES::decrypt_message(string message)
{
    DEBUG_OUTPUT("Decrypting message");
    // number of blocks needed
    size_t block_count = (message.size() + 7) / 8;
    // converting string to blocks
    vector<uint64_t> blocks(block_count);
    for (int i = 0; i < message.size(); i++)
    {
        uint8_t c = message[i];
        blocks[i / 8] |= (uint64_t(c) << (i % 8 * 8));
    }
    // decrypting blocks
    vector<uint64_t> decrypted_blocks(block_count);
    for (int i = 0; i < block_count; i++)
    {
        decrypted_blocks[i] = decrypt_block(blocks[i]);
    }
    // converting blocks back to string
    string result;
    result.resize(block_count * 8, 0);
    for (int i = 0; i < block_count; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            result[i * 8 + j] = uint8_t((decrypted_blocks[i] >> (j * 8)) & 255);
        }
    }
    return result;
}

uint64_t TripleDES::des_encrypt_stage(uint64_t block, uint64_t key)
{
    DEBUG_OUTPUT("DES encrypt stage");
    bitset<64> block_bits(block);
    bitset<56> key_bits(key);
    bitset<64> extended_key;
    bitset<64> ip;
    for (int i = 0; i < 64; i++)
    {
        ip[i] = block_bits[IP[i] - 1];
    }
    for (int i = 0; i < 56; i++)
    {
        int count;
        extended_key[i + (i / 7)] = key_bits[i];
        count += key_bits[i];
        if (i % 7 == 6)
        {
            extended_key[i + (i / 7) + 1] = (count % 2 == 1);
            count = 0;
        }
    }
    array<size_t, 64> cd_perm = KEY_PERM;
    array<bitset<48>, 16> round_keys;
    for (int i = 0; i < 16; i++)
    {
        bitset<64> perm_key;
        for (int i = 0; i < 64; i++)
        {
            perm_key[i] = extended_key[cd_perm[i] - 1];
        }
        bitset<48> round_key;
        for (int i = 0; i < 48; i++)
        {
            round_key[i] = perm_key[(KEY_BITS[i] - 1) + (KEY_BITS[i] - 1) / 7];
        }
        round_keys[i] = round_key;
        cd_perm = (i == 0 || i == 1 || i == 8 || i == 15) ? shift_perm(cd_perm) : shift_perm(shift_perm(cd_perm));
    }
    uint32_t left = ip.to_ullong() >> 32, right = ip.to_ullong();
    for (int i = 0; i < 16; i++)
    {
        uint32_t old_left = left;
        left = right;
        right = old_left ^ feistel_function(right, round_keys[i]);
    }
    uint64_t q = (uint64_t(left) << 32) | right;
    bitset<64> ip_result(q), result;
    for (int i = 0; i < 64; i++)
    {
        result[IP[i] - 1] = ip_result[i];
    }
    return result.to_ullong();
}

uint64_t TripleDES::des_decrypt_stage(uint64_t block, uint64_t key)
{
    DEBUG_OUTPUT("DES decrypt stage");
    bitset<64> block_bits(block);
    bitset<56> key_bits(key);
    bitset<64> extended_key;
    bitset<64> ip;
    for (int i = 0; i < 64; i++)
    {
        ip[i] = block_bits[IP[i] - 1];
    }
    for (int i = 0; i < 56; i++)
    {
        int count;
        extended_key[i + (i / 7)] = key_bits[i];
        count += key_bits[i];
        if (i % 7 == 6)
        {
            extended_key[i + (i / 7) + 1] = (count % 2 == 1);
            count = 0;
        }
    }
    array<size_t, 64> cd_perm = KEY_PERM;
    array<bitset<48>, 16> round_keys;
    for (int i = 0; i < 16; i++)
    {
        bitset<64> perm_key;
        for (int i = 0; i < 64; i++)
        {
            perm_key[i] = extended_key[cd_perm[i] - 1];
        }
        bitset<48> round_key;
        for (int i = 0; i < 48; i++)
        {
            round_key[i] = perm_key[(KEY_BITS[i] - 1) + (KEY_BITS[i] - 1) / 7];
        }
        round_keys[i] = round_key;
        cd_perm = (i == 0 || i == 1 || i == 8 || i == 15) ? shift_perm(cd_perm) : shift_perm(shift_perm(cd_perm));
    }
    uint32_t left = ip.to_ullong() >> 32, right = ip.to_ullong();
    for (int i = 0; i < 16; i++)
    {
        uint32_t old_right = right;
        right = left;
        left = old_right ^ feistel_function(left, round_keys[15 - i]);
    }
    uint64_t q = (uint64_t(left) << 32) | right;
    bitset<64> ip_result(q), result;
    for (int i = 0; i < 64; i++)
    {
        result[IP[i] - 1] = ip_result[i];
    }
    return result.to_ullong();
}

uint32_t TripleDES::feistel_function(uint32_t half_block, bitset<48> round_key)
{
    bitset<32> hb_bits(half_block);
    bitset<48> extended_hb;
    for (int i = 0; i < 48; i++)
    {
        extended_hb[i] = hb_bits[HALFBLOCK_BITS[i] - 1];
    }
    uint32_t result;
    for (int i = 0; i < 8; i++)
    {
        int a = extended_hb[6 * i + 5] * 2 + extended_hb[6 * i + 0];
        int b = extended_hb[6 * i + 4] * 8 + extended_hb[6 * i + 3] * 4 + extended_hb[6 * i + 2] * 2 + extended_hb[6 * i + 1];
        int n = S_BLOCKS[i][a][b];
        result |= (n << (4 * i));
    }
    return result;
}