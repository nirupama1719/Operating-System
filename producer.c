#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAXLEN 100

int main()
{
	int fd;
	char numstr[MAXLEN];	
	int count=0;
	if ( (fd = open("/dev/concurrency", O_RDWR)) < 0) {
		perror(""); printf("error opening the device");
		exit(1);
	}  
	while(1) {
		bzero(numstr, MAXLEN);
		sprintf(numstr, "%d%d", getpid(), count++);
		printf("Writing: %s\t", numstr);
		ssize_t ret = write(fd, &numstr, strlen(numstr));
		if ( ret < 0) {
			printf("Buffer Full!");
			return errno;
		} else{
			printf("Bytes written: %ld\n", ret);
		}
		sleep(1);
	}
	close(fd);

	return 0;
}

