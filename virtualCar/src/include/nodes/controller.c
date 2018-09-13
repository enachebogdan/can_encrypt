/**
 *
 *  This file is part of virtualcar.
 *
 *  virtualcar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3 of the License.
 *
 *  virtualcar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with virtualcar.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  virtualcar is a vehicle computer-powered system engine written in C to 
 *  help you research and learn more about vehicle devices and CAN protocol.
 *  This project represents a central unit or the CAN Bus, along with several
 *  different nodes that transmit and analyze signals.
 *
 *  The purpose of this code and the project is to replace paid alternatives
 *  and allow others to learn more about vehicle embedded systems.
 *
 *  The project is developed in C and it's based on a SocketCAN which is 
 *  available in any modern Linux kernel.
 *
 *  (c) Halis Duraki <duraki.halis@nsoft.ba>
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/can.h>

#include "../logger.h"
#include "../../messages.h"

#include "nodes.h"

/** nodes header */
#include "door.h"
#include "pump.h"
 
struct can_frame *current;
struct car_state 
{
	uint8_t ENGINE;
	uint8_t DOORS;
	uint8_t PLAYER;
	uint8_t LIGHTS;

} CAR;

void init_car_state(struct car_state *car)
{
	car->ENGINE = 1;
	car->DOORS  = 0;
	car->PLAYER = 0;
	car->LIGHTS = 0;
}

void print_car_state(struct car_state *car)
{
	if(car->ENGINE)
		printf("CAR ENGINE IS ON\n");
	else
		printf("CAR ENGINE IS OFF\n");
	if(car->DOORS)
		printf("CAR DOORS ARE OPENED\n");
	else
		printf("CAR DOORS ARE CLOSED\n");
	if(car->PLAYER)
		printf("CD PLAYER IS ON\n");
	else
		printf("CD PLAYER IS OFF\n");
	if(car->LIGHTS)
		printf("CAR LIGHTS ARE ON\n");
	else
		printf("CAR LIGHTS ARE OFF\n");

	car->LIGHTS = 0;
}

static void decrypt_ecb(uint8_t* in)
{
    struct AES_ctx ctx;
    
    AES_init_ctx(&ctx, key);
    AES_ECB_decrypt(&ctx, in);
}

// prints string as hex
void hex_to_str(const uint8_t* str, char* output)
{
    uint8_t i;
    for (i = 0; i < 16; ++i)
    {
        //printf("[%.2x] ", str[i]);
        sprintf(output + 2 * i,"%.2x", str[i]);
    }
    //printf("\n");
}

void can_accept_signal(struct can_frame *frame)
{

	uint8_t i;
	char received[50];
	char string_unencrypted_message[50];
	hex_to_str(frame->data, received);
	printf("CAN socket received the following message: %s\n", received);
	uint8_t unencr_messages_count = sizeof(unencrypted_messages)/sizeof(unencrypted_messages[0]);
	uint8_t found = 0;

	uint8_t decrypted[50];
	memcpy(decrypted, frame->data, 16);
	decrypt_ecb(decrypted);
	char decrypted_str[50];
	hex_to_str(decrypted, decrypted_str);
	printf("The string after we decrypted it: %s\n", decrypted_str);

	for(i = 0; i < unencr_messages_count; ++i)
	{
		hex_to_str(unencrypted_messages[i], string_unencrypted_message);
		if(strcmp(received, string_unencrypted_message) == 0 ||  // we have a match in an unencrypted message
			strcmp(decrypted_str, string_unencrypted_message) == 0 )  // we have a match in an encrypted message	
		{
			printf("We have received the following CAN command: ");
			switch(i)
			{
			case 0:
				printf("ENGINE ON\n");
				CAR.ENGINE = 1;
				break;
			case 1:
				printf("ENGINE OFF\n");
				CAR.ENGINE = 0;
				break;
			case 2:
				printf("DOOR OPEN\n");
				CAR.DOORS = 1;
				break;
			case 3:
				CAR.DOORS = 0;
				printf("DOOR CLOSE\n");
				break;
			case 4:
				CAR.PLAYER = 1;
				printf("TURN ON CD PLAYER\n");
				break;
			case 5:
				CAR.PLAYER = 0;
				printf("TURN OFF CD PLAYER\n");
				break;
			case 6:
				CAR.LIGHTS = 1;
				printf("TURN ON LIGHTS\n");
				break;
			case 7:
				CAR.LIGHTS = 1;
				printf("TURN OFF LIGHTS\n");
				break;
			}
			found = 1;
			break;
		}
	}
	if(!found)
	{
		printf("The received message does not mean anything, it will be ignored.\n");
	}

	printf("\n");
}