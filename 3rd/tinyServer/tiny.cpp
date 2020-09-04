#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <csignal>
#include <cstring>
#include <system_error>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <map>

#define LISTENQ 1024  /* second argument to listen() */
#define MAXLINE 1024  /* max length of a line */
#define RIO_BUFSIZE 1024

typedef struct {
    int rio_fd;             /* descriptor for this buf */
    int rio_cnt;            /* next unread byte in this buf */
    char *rio_bufptr;          /* next unread byte in this buf */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */
} rio_t;

typedef struct sockaddr SA;

typedef struct {
    char filename[512];
    off_t offset;
    size_t end;
} http_request;


std::map<std::string, std::string> mime_map {
    {".css", "text/css"},
    {".gif", "image/gif"},
    {".htm", "text/html"},
    {".html", "text/html"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},
    {".ico", "image/x-icon"},
    {".js", "application/javascript"},
    {".pdf", "application/pdf"},
    {".mp4", "video/mp4"},
    {".png", "image/png"},
    {".svg", "image/svg+xml"},
    {".xml", "text/xml"},
};

std::string const default_mime_type("text/plain");

void rio_readinitb(rio_t *rp, int fd) {
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t writen(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = static_cast<char *>(usrbuf);

    while(nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
    int cnt;
    while(rp->rio_cnt <= 0) {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

        if (rp->rio_cnt < 0) {
            if (errno != EINTR) {
                return -1;
            }
        } else if (rp->rio_cnt == 0) {
            return 0;
        } else {
            rp->rio_bufptr = rp->rio_buf;
        }
    }
    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n;
    if (rp->rio_cnt < n) {
        cnt = rp->rio_cnt;
    }
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}


/*
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    int n, rc;
    char c, *bufp = static_cast<char *>(usrbuf);

    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n')
                break;
        } else if (rc == 0) {
            if (n == 1)
                return 0;
            else
                break;
        } else {
            return -1;
        } 
    }
    *bufp = 0;
    return n;
}

void format_size(char *buf, struct stat *stat) {
    if (S_ISDIR(stat->st_mode)) {
        sprintf(buf, "%s", "[DIR]");
    } else {
                off_t size = stat->st_size;
        if(size < 1024){
            sprintf(buf, "%lu", size);
        } else if (size < 1024 * 1024){
            sprintf(buf, "%.1fK", (double)size / 1024);
        } else if (size < 1024 * 1024 * 1024){
            sprintf(buf, "%.1fM", (double)size / 1024 / 1024);
        } else {
            sprintf(buf, "%.1fG", (double)size / 1024 / 1024 / 1024);
        }
    }
}

void handle_directory_request(int out_fd, int dir_fd, char *filename) {}

static std::string get_mime_type(std::string filename) {
    std::string suffixStr = filename.substr(filename.find_last_of('.') + 1);
    auto search = mime_map.find(suffixStr);
    if (search != mime_map.end()) {
        return search->second;
    } else {
        return default_mime_type;
    }
}

int open_listenfd(int prot) {}

void url_decode(char *src, char *dest, int max) {}

void parse_request(int fd, http_request *req) {}

void log_access(int status, struct sockaddr_in *c_addr, http_request *req) {
    // printf("%s:%d %d - %s\n", inet_ntoa(c_addr->sin_addr),
    //     ntohs(c_addr->sin_port), status, req->filename);
        // The inet_ntoa() function converts the Internet host address in, given in network byte order,
    // to a string in IPv4 dotted-decimal notation. The string is returned in a statically allocated buffer,
    // which subsequent calls will overwrite.
    // The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.
    std::cout << inet_ntoa(c_addr->sin_addr) << ":" << ntohs(c_addr->sin_port) << " " << status << " - " << req->filename << std::endl;
}

void client_error(int fd, int status, char *msg, char *longmsg) {}

void serve_static(int out_fd, int in_fd, http_request *req, size_t total_size) {}

void process(int fd, struct sockadr_in *clientaddr) {}

int main(int argc, char** argv) {
    return 0;
}