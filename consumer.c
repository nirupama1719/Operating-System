#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXLEN 15
char numstr[MAXLEN];
int main()
{

	int fd;
//	static char numstr[MAXLEN];
	//char *num_read;

	if ( (fd = open("/dev/concurrency", O_RDWR)) < 0) {
		perror(""); printf("error opening the device");
		exit(1);
	}

	while(1) {
		// read a line
		int count=0;
		ssize_t ret = read(fd,numstr,sizeof(numstr));
		if( ret>0) {
			printf("Number read: %s ", numstr);
			printf("Bytes read: %ld\n", ret);
			count++;
		} else {
			fprintf(stderr, "error reading ret=%ld errno=%d perror: ", ret, errno);
			perror("");
			sleep(1);
		}
	}
	close(fd);
	return 0;
}
