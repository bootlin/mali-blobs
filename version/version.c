/*
 * Copyright (c) 2011-2013 Luc Verhaegen <libv@skynet.be>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

struct mali_api_version {
	unsigned int empty;
	unsigned int version;
	int compatible;
};

#define MALI_GET_API_VERSION	  _IOWR(0x82, 5, struct mali_get_api_version *)
#define MALI_GET_API_VERSION_R3P1 _IOWR(0x82, 3, struct mali_get_api_version *)

static struct {
	int version;
	const char *compat;
} version_compat[] = {
	{6, "UNSUPPORTED"}, /* r2p0 and r2p1-rel0 */
	{7, "UNSUPPORTED"}, /* r2p1-rel1 */
	{8, "r2p2"},
	{9, "r2p3"},
	{10, "r2p4"},
	{14, "r3p0"},
	{17, "r3p1"},
	{19, "r3p2"},
	{20, "r3p2-01rel1"}, /* and r3p2-01rel2 */
	{23, "r3p2-01rel3"},
	{900, "r6p2-01rel0"},

	{0, NULL},
};

int
main(int argc, char *argv[])
{
	int fd;
	struct mali_api_version api_version;
	int i, ret, version;

	fd = open("/dev/mali", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Error: Failed to open /dev/mali: %s\n",
		       strerror(errno));
		return errno;
	}

	ret = ioctl(fd, MALI_GET_API_VERSION, &api_version);
	if (ret == -EPERM)
		ret = ioctl(fd, MALI_GET_API_VERSION_R3P1, &api_version);

	if (ret) {
		fprintf(stderr, "Error: ioctl(GET_API_VERSION) failed: %s\n",
			strerror(ret));
		return ret;
	}

	version = api_version.version & 0xFFFF;
	for (i = 0; version_compat[i].compat; i++)
		if (version_compat[i].version == version)
			break;

	if (version_compat[i].compat)
		printf("%s", version_compat[i].compat);
	else
		printf("UNKNOWN");

	return 0;
}
