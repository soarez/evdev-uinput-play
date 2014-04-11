
all: evtest inject create

evtest: evtest.c
	$(CC) -o $@ $<

uinput-sample: uinput-sample.c
	$(CC) -o $@ $<

inject: inject.c
	$(CC) -o $@ $<

create: create.c event_list.c
	$(CC) -o $@ create.c event_list.c

.PHONY: test

