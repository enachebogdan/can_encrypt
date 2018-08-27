#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1
#define CTR 1
#define ECB 1

#include "can_encrypt.h"
#include "aes.h"
#include "virtualcar.h"
//#include "messages.h"

static void encrypt_ecb();
static void decrypt_ecb();

//static const uint8_t car_open[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
//static const uint8_t car_open[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,};// 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };


#define ENCRYPT 0

pthread_t vcar;


void send_unencrypted_data(char* data)
{
    char cangen_command[100] = "cangen virtualcar -v -L 8 -n 1 -I 00B -D ";
    
    if(16 == strlen(data))
    {
        strcat(cangen_command, data);
        printf("!!!!!!! WE ARE RUNNING THIS COMMAND: %s \n", cangen_command);
        system(cangen_command);
    }

    printf("_________ IMPOSIBLE");
}

int main()
{
	printf(" Creating the virtual thread car in starting it !!! \n");
        /** Check thread */
    int tid = pthread_create(&(vcar), NULL, &start_virtualcar, NULL);

    if (tid != 0)
        printf("\ncan't create thread :[%s]", strerror(tid));
    else
    {
        printf("\nThread created successfully. Sleeping for one second to get virtual car set up.\n");
        sleep(1);
    }

    char string_data[50];
    hex_to_str(car_open, string_data);

    printf("\n\n ZZZZZZZZZZZZZ %s \n\n", string_data);

    printf(" Sending some stuff!!! \n");   

    send_unencrypted_data(string_data);
    pthread_join(vcar, NULL);
    
//	encrypt_ecb();
//	return 0;
}





static void encrypt_ecb()
{
	uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    uint8_t out[] = { 0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97 };
    uint8_t in[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
    struct AES_ctx ctx;

    AES_init_ctx(&ctx, key);
    AES_ECB_encrypt(&ctx, in);

    printf("ECB encrypt: ");

    if (0 == memcmp((char*) out, (char*) in, 16))
    {
        printf("SUCCESS!\n");
    }
    else
    {
        printf("FAILURE!\n");
    }
}
