void encryptAES(char *inputFile, unsigned char *key);

void decryptAES(char *inputFile, unsigned char *key);

void public_encrypt(unsigned char *text, int len, unsigned char * key, unsigned char *encrypted);

void private_decrypt(unsigned char *encText, int len,unsigned char *key, unsigned char *decrypted);

void newKey(unsigned char *key);
