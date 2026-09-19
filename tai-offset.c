/* Set tai offset
 *              by: John Stultz <john.stultz@linaro.org>
 *              (C) Copyright Linaro 2013
 *              Licensed under the GPLv2
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

int set_tai(int offset)
{
	struct timex tx;

	memset(&tx, 0, sizeof(tx));

	tx.modes = ADJ_TAI;
	tx.constant = offset;

	return adjtimex(&tx);
}

int get_tai(void)
{
	struct timex tx;

	memset(&tx, 0, sizeof(tx));

	adjtimex(&tx);
	return tx.tai;
}

void help() {
    printf("Usage: tai-offset [num-seconds]\n");
    printf("  \n");
    printf("  Prints the offset between CLOCK_TAI and unixtime,\n");
    printf("  and sets it once if the optional argument is given.\n");
    exit(1);
}

int main(int argc, char **argv)
{
	int n_args = argc - 1;
	if (n_args > 1)
	    help();
	int offset;
	bool do_set_offset;
	if (n_args == 0) {
	    do_set_offset = false;
	} else {
	    char* const input = argv[1];
	    char* remainder = input;
	    offset = strtol(input, &remainder, 10);
	    int len = strlen(input);
	    if ((remainder - input) != len)
		help();
	    do_set_offset = true;
	}

	int ret = get_tai();
	printf("current CLOCK_TAI offset: %i\n", ret);
	fflush(stdout);

	if (do_set_offset) {
	    ret = set_tai(offset);
	    ret = get_tai();
	    if (ret != offset) {
		printf("[FAILED] expected: %i got %i\n", offset, ret);
		return 1;
	    }
	    printf("offset set to %i\n", offset);
	}
	return 0;
}
