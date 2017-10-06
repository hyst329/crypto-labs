#include "3des.h"
#include <iostream>

int main()
{
    array<uint64_t, 3> keys =
        {
            18194459440113652026ULL,
            1573933694751181523ULL,
            6292804709873077373ULL,
        };
    TripleDES the3des = TripleDES(keys);
    string message = "In Greek mythology, the Trojan War was waged against the city"
                     " of Troy by the Achaeans (Greeks) after Paris of Troy took Helen"
                     " from her husband Menelaus, king of Sparta. The war is one of the"
                     " most important events in Greek mythology and has been narrated"
                     " through many works of Greek literature, most notably through Homer's"
                     " Iliad. The Iliad relates four days in the tenth year of the decade-long"
                     " siege of Troy; the Odyssey describes the journey home of Odysseus, one"
                     " of the war's heroes. Other parts of the war are described in a cycle"
                     " of epic poems, which have survived through fragments. Episodes from"
                     " the war provided material for Greek tragedy and other works of Greek"
                     " literature, and for Roman poets including Virgil and Ovid.";
    uint64_t x = the3des.encrypt_block(0);
    cout << x << endl;
    cout << the3des.decrypt_block(x) << endl;
    string encrypted = the3des.encrypt_message(message);
    cout << encrypted << endl;
    string decrypted = the3des.decrypt_message(encrypted);
    cout << decrypted << endl;
}