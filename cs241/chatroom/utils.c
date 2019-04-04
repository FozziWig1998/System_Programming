/**
 * Charming Chatroom
 * CS 241 - Spring 2019
 *
 * riyabd2
 * miteshp2
 * parthgp2
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"
static const size_t MESSAGE_SIZE_DIGITS = 4;

char *create_message(char *name, char *message) {
    int name_len = strlen(name);
    int msg_len = strlen(message);
    char *msg = calloc(1, msg_len + name_len + 4);
    sprintf(msg, "%s: %s", name, message);

    return msg;
}

ssize_t get_message_size(int socket) {
    int32_t size;
    ssize_t read_bytes =
        read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
    if (read_bytes == 0 || read_bytes == -1)
        return read_bytes;

    return (ssize_t)ntohl(size);
}

// You may assume size won't be larger than a 4 byte integer
/**
 * Writes the bytes of size to the socket
 *
 * Returns the number of bytes successfully written,
 * 0 if socket is disconnected, or -1 on failure
 */
ssize_t write_message_size(size_t size, int socket) {
    // Your code here
    ssize_t mesg_size = htonl(size);
    ssize_t retval = write_all_to_socket(socket, (char *)&mesg_size, MESSAGE_SIZE_DIGITS);
    
    if (retval != 0 && retval != -1) {
        return mesg_size;
    }

    return retval;
}

/**
 * Attempts to read all count bytes from socket into buffer.
 * Assumes buffer is large enough.
 *
 * Returns the number of bytes read, 0 if socket is disconnected,
 * or -1 on failure
 */
ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
    // Your Code Here
    ssize_t bytes_read_so_far = 0;
    ssize_t bytes_left = count;

    while (bytes_left > 0) {
        ssize_t success = read(socket, (void*) (buffer + bytes_read_so_far), bytes_left);

        if (success == -1 && errno == EINTR) {
            continue;
        }

        if (!success) {
            perror(NULL);
            return 0;
        }

        bytes_left -= success;
        bytes_read_so_far += success;
    }

    return bytes_read_so_far;
}

/**
 * Attempts to write all count bytes from buffer to socket.
 * Assumes buffer contains at least count bytes.
 *
 * Returns the number of bytes written, 0 if socket is disconnected,
 * or -1 on failure.
 */
ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
    // Your Code Here
    ssize_t bytes_read_so_far = 0;
    ssize_t bytes_left = count;

    while (bytes_left > 0) {
        ssize_t success = write(socket, (void*) (buffer + bytes_read_so_far), bytes_left);

        if (success == -1 && errno == EINTR) {
            continue;
        }

        if (!success) {
            perror(NULL);
            return 0;
        }

        bytes_left -= success;
        bytes_read_so_far += success;
    }

    return bytes_read_so_far;
}
