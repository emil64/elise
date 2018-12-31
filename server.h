struct key_id{
    int id;
    unsigned char key[5000];
};

typedef struct key_id key_id;

void getKeyId(key_id *ki);
