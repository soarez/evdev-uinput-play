
#include <stdint.h>

#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

const char* device_path = "/dev/input/event2";

int main (int argc, char **argv) {

  int fd;
	int version;
	unsigned short id[4];
	char name[256] = "Unknown";

	if ((fd = open(device_path, O_RDONLY)) < 0) {
		perror("evtest: can\'t open device");
		return 1;
	}

	if (ioctl(fd, EVIOCGVERSION, &version)) {
		perror("evtest: can't get version");
		return 1;
	}

	printf("Input driver version is %d.%d.%d\n",
		version >> 16, (version >> 8) & 0xff, version & 0xff);

	ioctl(fd, EVIOCGID, id);
	printf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

	ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("Input device name: \"%s\"\n", name);


  close(fd);

  return 0;
}

