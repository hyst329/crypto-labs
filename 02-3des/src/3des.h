#include <cstdint>
#include <array>
#include <bitset>

using namespace std;

class TripleDES
{
  public:
    TripleDES(array<uint64_t, 3> keys);
    uint64_t encrypt_block(uint64_t block);
    uint64_t decrypt_block(uint64_t block);
    string encrypt_message(string message);
    string decrypt_message(string message);

  private:
    array<uint64_t, 3> keys;
    uint64_t des_encrypt_stage(uint64_t block, uint64_t key);
    uint64_t des_decrypt_stage(uint64_t block, uint64_t key);
    uint32_t feistel_function(uint32_t half_block, bitset<48> round_key);
};