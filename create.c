#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/uinput.h>

extern __u16 KEY_CODES[];
extern size_t KEY_CODES_SIZE;

void send_key(int fd, __u16 code, __u16 value) {

  int ret;
  struct input_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.type = EV_KEY;
  ev.code = code;
  ev.value = value;

  ret = write(fd, &ev, sizeof(ev));
  if (ret < 0) {
      perror("evdev create: can\'t send event");
      exit(EXIT_FAILURE);
  }

  ev.type = EV_SYN;
  ev.code = 0;
  ev.value = 0;
  ret = write(fd, &ev, sizeof(ev));
  if (ret < 0) {
      perror("evdev create: can\'t send event");
      exit(EXIT_FAILURE);
  }

}

int main(int argc, char* argv[]) {
  int fd;
  int ret;

  fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  if(fd < 0) {
      perror("evdev create: can\'t open uinput");
      exit(EXIT_FAILURE);
  }

  ret = ioctl(fd, UI_SET_EVBIT, EV_KEY);
  if (ret < 0) {
      perror("evdev create: can\'t set EV_KEY");
      exit(EXIT_FAILURE);
  }

  int i;
  for (i = 0; i < KEY_CODES_SIZE; ++i) {
    ret = ioctl(fd, UI_SET_KEYBIT, KEY_CODES[i]);
    if (ret < 0) {
        perror("evdev create: can\'t set KEY");
        exit(EXIT_FAILURE);
    }
  }

  struct uinput_user_dev uidev;
  memset(&uidev, 0, sizeof(uidev));

  snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor  = 0x1234;
  uidev.id.product = 0xfedc;
  uidev.id.version = 1;

  ret = write(fd, &uidev, sizeof(uidev));
  if (ret < 0) {
      perror("evdev create: can\'t write the device info");
      exit(EXIT_FAILURE);
  }

  ret = ioctl(fd, UI_DEV_CREATE);
  if (ret < 0) {
      perror("evdev create: can\'t create the device");
      exit(EXIT_FAILURE);
  }

  printf("Device was created\n");

  sleep(5);
  printf("Pressing key\n");
  send_key(fd, KEY_LEFTSHIFT, 1);
  send_key(fd, KEY_A, 1);

  sleep(5);
  printf("Releasing key\n");
  send_key(fd, KEY_A, 0);
  send_key(fd, KEY_LEFTSHIFT, 0);

  sleep(5);

  ret = ioctl(fd, UI_DEV_DESTROY);
  if (ret < 0) {
      perror("evdev create: can\'t destroy the device");
      exit(EXIT_FAILURE);
  }

  printf("Device was destroyed\n");

  return 0;
}

