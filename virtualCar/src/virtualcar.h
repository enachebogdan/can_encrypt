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
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/can.h>

#include "include/nodes/controller.c"

#include "car.c"

/** Create a virtual CAN device that acts as a vehicle CANBus */
void create_car() {
    const char *virtualcar = "virtualcar"; /* Use the const in future */

    system("modprobe vcan");
    system("sudo ip link add dev virtualcar type vcan");
    system("sudo ip link set up virtualcar");
}

int start_virtualcar() {

    int s;                              /* socket */
    struct can_frame frame;             /* frame  */
    int size, i;                        /* data size */
    static struct ifreq ifr;            /* ifr as an instance*/
    static struct sockaddr_ll sl;       /* prefs */
    char *instance = "virtualcar";      /* our virtual device */
    int ifindex;                        /* car socket # */
    char current[8];                    /* current operation received */


    /** Bring up main controller and gateway */
    create_car();

    s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_CAN));
    if (s < 0) {
        // Can't connect to socket?!
        perror("socket");
        //return 1;
    }

    if (strcmp(instance, "any") == 0)
        ifindex = 0; // Debug; listen to index 0
    else {
        strcpy(ifr.ifr_name, instance);
        ioctl(s, SIOCGIFINDEX, &ifr);
        ifindex = ifr.ifr_ifindex;
    }

    sl.sll_family = AF_PACKET;
    sl.sll_ifindex = ifindex;
    sl.sll_protocol = htons(ETH_P_CAN);

    if (bind(s, (struct sockaddr *) &sl, sizeof(sl)) < 0) {
        ("bind"); // please avoid!
        return 1;
    }

    while (1) {

        if ((size = read(s, &frame, sizeof(struct can_frame))) < 0) {
            // invalid frame
            perror("read");
            return 1;
        } else if (size < sizeof(struct can_frame)) {
            // invalid frame
            fprintf(stderr, "CAN frame is wrong! Are you trying to hack me?!\n");
            return 1;
        } else {
            printf("\n_____ going in\n");
            if (frame.can_id & CAN_EFF_FLAG)
                printf("%8X  ", frame.can_id & CAN_EFF_MASK); // 8 bytes
            can_accept_signal(&frame);
            if (frame.can_id & CAN_RTR_FLAG)
                printf("%8X  ", frame.can_id & CAN_EFF_MASK); // 8 bytes

            fflush(stdout);
        } 
    } 
}
