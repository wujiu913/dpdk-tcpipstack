#include "socket_interface.h"
#include <pthread.h>
#include "logger.h"

int new_socket = 0;
void *DoWork(void *test)
{
   char buffer[1024];
   int len = 0;
   logger(SOCKET, NORMAL, "coming off accept\n");
   socket_send(new_socket, "Hello World", 12);
   printf("waiting on socket read\n");
#if 0
   socket_read(new_socket, buffer, 10);
   printf("received from socket %s\n", buffer);
#endif
   len = 0;
   while(len < 22) {
      len += socket_read_nonblock(new_socket, buffer);
      if(len < 0) {
         printf("socket closed\n");
         break;
      }
      if(len==0) {
         printf("nothing to show\n");
      }
      else {
         printf("received from socket %s length %d\n", buffer, len);
      }
   }
   //printf("received from socket %s\n", buffer);
   printf("closing socket\n");
   socket_close(new_socket);
   return NULL;
}

void init_socket_example(int port, uint8_t *ip)
{
   int i = 0;
   int socket = socket_open(TCP_STREAM);
   struct sock_addr addr;
   addr.port = port;
   addr.ip = 0;
   for(i=0; i<4; i++) {
      addr.ip |= ip[i] << i*8;
   }
//   printf("ip is %x\n", addr.ip);
   socket_bind(socket, &addr);
   socket_listen(socket, 5);
   struct sock_addr client;
//   printf("Waiting for accept\n");
   logger(SOCKET, NORMAL, "waiting on accept\n");
   pthread_attr_t attr;
   int thread_id = 0;
   while(1) {
      new_socket = socket_accept(socket, &client);
      pthread_create(&thread_id, NULL, DoWork, NULL); 
   }
//   printf("accepted the connection\n");
}


