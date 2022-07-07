#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// Ordered by letter frequency according to https://en.wikipedia.org/wiki/Letter_frequency
#define ALPHABET "etaoinshrdlcumwfgypbvkjxqz "
// #define ALPHABET "abcdefghijklmnopqrstuvwxyz "

// Define the alphabet size
#define CHAR_SIZE 27
struct Server
{
    int                 domain;
    int                 service;
    int                 protocol;
    u_long              interface;
    int                 port;
    int                 backlog;

    struct sockaddr_in  address;

    int                 socket;

    void (*launch)(struct Server *server);
};

// Data structure to store a Trie node
struct Trie
{
    int isLeaf; // 1 when the node is a leaf node
    int children;
    char    *name;
    struct Trie *character[CHAR_SIZE];
};


struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void (*launch)(struct Server *server));

void    createTrie(void);
void    searchAndPrintRec(struct Trie *head, char *currPrefix);
void    searchAndPrintRec2(struct Trie *head);

#endif