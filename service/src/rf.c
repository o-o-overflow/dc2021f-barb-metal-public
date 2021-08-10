#include <sys/types.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	for (int i = 1; i < argc; i++) {
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0) return 1;
		sendfile(1, fd, NULL, (unsigned int)-1);
		struct stat st;
		stat(argv[i], &st);
		if(st.st_size == 48) { // flag give it a newline
			write(1, "\n", 1);
		}
	}
	while (1) {
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		struct timeval tv;
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		int status;
		status = select(1, &rfds, NULL, NULL, &tv);
		if (status == -1) {
			perror("select");
			return 1;
		}

		if (!FD_ISSET(0, &rfds)) continue;

		char buf;
		int n;
	    n = read(0, &buf, 1);
	    if (n < 0) {
	    	if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // fprintf(stderr, "EAGAIN: Continuing\n");
                continue;
            } else {
            	perror("read");
            	return 1;
            }
	    }
	    if (n == 0) {
	    	return 0;
	    }
	    n = write(1, &buf, 1);
	    if (n < 0) return 1;
	}
	return 0;
}
