#include "ifi/v5_api.h"
#include <stdint.h>

int32_t _write(int32_t fd, char* buf, int32_t nbytes)
{
	// This allows printf and cout to work with second USB CDC channel
	if (nbytes > 256)
		return (0);

	vexSerialWriteBuffer(1, (uint8_t*)buf, nbytes);

	return nbytes;
}
