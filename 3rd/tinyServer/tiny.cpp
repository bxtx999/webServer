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
#include <system_error>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

#define LISTENQ 1024  /* second argument to listen() */
#define MAXLINE 1024  /* max length of a line */
#define RIO_BUFSIZE 1024

typedef struct {
    size_t rio_fd;             /* descriptor for this buf */
    size_t rio_cnt;            /* next unread byte in this buf */
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

void rio_readinitb(rio_t *rp, int fd) {}

ssize_t writen(int fd, void *usrbuf, size_t n) {}

/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {}


/*
 * rio_readlineb - robustly read a text line (buffered)
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {}

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
    
}

int open_listenfd(int prot) {}

void url_decode(char *src, char *dest, int max) {}

void parse_request(int fd, http_request *req) {}

void log_access(int status, struct sockaddr_in *c_addr, http_request *req) {
    printf("%s:%d %d - %s\n", inet_ntoa(c_addr->sin_addr),
        ntohs(c_addr->sin_port), status, req->filename);
}

void client_error(int fd, int status, char *msg, char *longmsg) {}

void serve_static(int out_fd, int in_fd, http_request *req, size_t total_size) {}

void process(int fd, struct sockadr_in *clientaddr) {}

int main(int argc, char** argv) {
    return 0;
}