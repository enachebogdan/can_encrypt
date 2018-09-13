//This are the unencrypted messages. 
const uint8_t unencrypted_messages[][16] =
{
	{ 0x6b, 0x2a, 0xbe, 0xe2, 0x2e, 0xc1, 0x9f, 0x11, 0xe9, 0x3d, 0x7e, 0x96, 0x73, 0x93, 0x17, 0x6b }, // ENGINE ON
	{ 0x96, 0xc1, 0xe9, 0xe2, 0xc1, 0x40, 0xe9, 0xc1, 0x9f, 0x96, 0xe9, 0x3d, 0x6b, 0xc1, 0xbe, 0xe2 }, // ENGINE OFF
	{ 0xe2, 0xc1, 0xbe, 0x2a, 0x2e, 0x2a, 0x9f, 0x96, 0xe9, 0x3d, 0x40, 0x11, 0x73, 0x93, 0x2e, 0x2a },	// DOOR OPEN
	{ 0xe9, 0x9f, 0xbe, 0x2a, 0x9f, 0x96, 0x7e, 0x7e, 0xe9, 0x96, 0x7e, 0x96, 0x40, 0x40, 0x2e, 0x2a },	// DOOR CLOSE
	{ 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0xe9, 0x93, 0x17, 0x17 }, // TURN ON CD PLAYER
	{ 0x7e, 0xbe, 0xbe, 0xc1, 0xe9, 0x11, 0x9f, 0x96, 0x2a, 0x3d, 0x96, 0x40, 0x2e, 0x7e, 0x93, 0x2a }, // TURN OFF CD PLAYER
	{ 0xe2, 0xc1, 0xbe, 0x2a, 0x2e, 0x2a, 0x9f, 0x96, 0xe9, 0x11, 0x40, 0x3d, 0x73, 0x93, 0x2e, 0x2a },	// LIGHTS ON
	{ 0xe2, 0xc1, 0xbe, 0x2a, 0x93, 0x2a, 0x9f, 0x96, 0xe9, 0x3d, 0x40, 0x93, 0x93, 0x40, 0x2e, 0x73 }	// LIGHTS OFF
};


const uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };