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

void can_send_signal(struct can_frame *frame)
{
	
}

// prints string as hex
static void hex_to_str(uint8_t* str, char* output)
{
//	output = "";
    unsigned char i;
    for (i = 0; i < strlen(str); ++i)
    {
        //printf("[%.2x] ", str[i]);
        sprintf(output + 2 * i,"%.2x", str[i]);
    }    
    printf("\n");
}

void can_accept_signal(struct can_frame *frame)
{

	uint8_t i;
	printf("___________________ New frame recieved ...");

	printf("\ncan_id = %x\n", frame->can_id);

	printf("\n  We are going to compare the values now \n");
	{
		char received[50];
		char string_unencrypted_message[50];
		hex_to_str(frame->data, received);
		printf("\n_____ THIS IS WHAT WE RECEIVED: %s \n", &received);
		uint8_t unencr_messages_count = sizeof(unencrypted_messages)/sizeof(unencrypted_messages[0]);
		printf("\n ___________ WE HAVE [%d] known unencrypted_messages\n", unencr_messages_count);

		for(i = 0; i < unencr_messages_count; ++i)
		{
			//printf("\n Now I am going to convert one of the unencr to string [%d] \n", i);
			hex_to_str(unencrypted_messages[i], string_unencrypted_message);
			//printf("\n OBTAINED UNENCR MESSAGE IS: %s\n" , string_unencrypted_message);
			if(strcmp(received, string_unencrypted_message) == 0)  // we have a match
			{
				printf("!!!! BINGOOOO!!!!!!!!");
			}
		}
//		
	}

	printf("\n");
}