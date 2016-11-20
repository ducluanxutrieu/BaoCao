#include <cnet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Hamming.h"
#include "Hamming.c"



typedef struct {
    size_t	 len;
    char data[MAX_MESSAGE_SIZE];
} FRAME;

#define FRAME_HEADER_SIZE  (sizeof(FRAME) - MAX_MESSAGE_SIZE)
#define FRAME_SIZE(f)      (FRAME_HEADER_SIZE + f.len)

static void transmit_frame(char* msg, size_t length)
{
  FRAME       f;
  int		link = 1;

  //memcpy(&f.data, msg, (int)length);

  split(msg, f.data, (int) length);
  encode(f.data, length*2, DATA_SIZE, BLOCK_SIZE);
  //make it incorrect
  f.len = length*2;
  randomToggleArray(f.data, f.len, BLOCK_SIZE);

  length = FRAME_SIZE(f);
  CHECK(CNET_write_physical(link, &f, &length));
}


static EVENT_HANDLER(physical_ready)
{
    char buffer[256];
    FRAME        f;
    size_t	 len;
    int          link;

    len         = sizeof(FRAME);
    CHECK(CNET_read_physical(&link, &f, &len));
    decode(f.data,f.len, DATA_SIZE, BLOCK_SIZE);
    join(f.data, buffer, f.len);
    printf("Received data: %s", buffer);
}

EVENT_HANDLER(on_keyboard) {
  char buffer[256];
  size_t bufferLength = 256;
	CHECK( CNET_read_keyboard( buffer, &bufferLength ) );
  printf("Input: %s\n", buffer);
  printf("Start transmitting data\n");
  transmit_frame(buffer, bufferLength);
}

EVENT_HANDLER(reboot_node) {
  srand(time(NULL));
  printf("Hello, my name is %s\n", nodeinfo.nodename);

  //Set keyboard event
  CHECK(CNET_set_handler( EV_KEYBOARDREADY, on_keyboard, 0));

  CHECK(CNET_set_handler( EV_PHYSICALREADY,    physical_ready, 0));


}
