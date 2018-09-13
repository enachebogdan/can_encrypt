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

#define ENCRYPT 0

pthread_t vcar;


void send_unencrypted_data(char* data)
{
    char cangen_command[100] = "cangen virtualcar -vvvv -L 16 -n 1 -f -I 00B -D ";
    strcat(cangen_command, data);
    system(cangen_command);
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

    char string_data[16];
    uint8_t unecr_1[] = { 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    hex_to_str(unecr_1, string_data);

    printf("\n.................................................................................\n");
    printf(" Test nr 1: Sending unencrypted message:%s \n", string_data);   
    send_unencrypted_data(string_data);
    getchar();

    printf("\n.................................................................................\n");
    hex_to_str(unencrypted_messages[0], string_data);
    printf(" Test nr 2: Sending unencrypted message:%s \n", string_data);
    send_unencrypted_data(string_data);
    print_car_state(&CAR);
    getchar();


    printf("\n.................................................................................\n");
    printf(" Test nr 3: Sending encrypted message\n");
    uint8_t in[16];
    memcpy(in, unencrypted_messages[2], 16);
    encrypt_ecb(in);
    hex_to_str(in, string_data);
    printf("The string after encrypting it. We send it to CAN: %s\n", string_data);
    send_unencrypted_data(string_data);
    print_car_state(&CAR);
    getchar();


    printf("\n.................................................................................\n");
    printf(" Test nr 4: Sending encrypted message\n");
    //uint8_t in[16];
    memcpy(in, unencrypted_messages[4], 16);
    encrypt_ecb(in);
    hex_to_str(in, string_data);
    printf("The string after encrypting it. We send it to CAN: %s\n", string_data);
    send_unencrypted_data(string_data);
    print_car_state(&CAR);
    getchar();

    printf("\n.................................................................................\n");
    printf(" Test nr 5: Sending encrypted message\n");
    memcpy(in, unencrypted_messages[5], 16);
    encrypt_ecb(in);
    hex_to_str(in, string_data);
    printf("The string after encrypting it. We send it to CAN: %s\n", string_data);
    send_unencrypted_data(string_data);
    print_car_state(&CAR);
    getchar();

    printf("\n.................................................................................\n");
    printf(" Test nr 6: Sending encrypted message\n");
    memcpy(in, unencrypted_messages[1], 16);
    encrypt_ecb(in);
    hex_to_str(in, string_data);
    printf("The string after encrypting it. We send it to CAN: %s\n", string_data);
    send_unencrypted_data(string_data);
    print_car_state(&CAR);


    pthread_join(vcar, NULL);

}

static void encrypt_ecb(uint8_t* in)
{
    struct AES_ctx ctx;

    AES_init_ctx(&ctx, key);
    AES_ECB_encrypt(&ctx, in);
}
