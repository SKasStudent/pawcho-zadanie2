#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

#define autor "Szymon Kasperczuk 101587"
#define PORT 8080

void respond(int cs){
  char input[1000];
  read(cs, input, sizeof(input));
  char city[20] = "Warszawa";
  char *index = strstr(input, "miasto=");
  if(index){
    index+=7;
    sscanf(index, "%20[^ \r\n]", city);
  }
  char command[100];
  char pogoda[100];
  sprintf(command, "wget -qO- 'wttr.in/%s?format=3'", city);
  FILE *f = popen(command, "r");
  if(f){
    fgets(pogoda, sizeof(pogoda),f);
    pclose(f);
  }else{
    strcpy(pogoda,"blad wczytania danych");
  }

  char body[1000];
  sprintf(body,
    "<html><body>"
    "<p>Wybierz miasto:</p>"
    "<form>"
    "<input type='radio' name='miasto' value='Warszawa' checked>Warszawa<br>"
    "<input type='radio' name='miasto' value='Lublin'>Lublin<br>"
    "<input type='radio' name='miasto' value='Krakow'>Krakow<br>"
    "<input type='submit' value='check'/>"
    "</form>"
    "<p>Miasto: %s</p>"
    "<p>Pogoda: %s</p>"
    "</body></html>",city,pogoda
  );

  char header[500];
  sprintf(header,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8\r\n"
    "Content-Length: %zu\r\n"
    "\r\n",strlen(body)
  );

  write(cs,header,strlen(header));
  write(cs,body,strlen(body));
  close(cs);
}

int main(int argc, char* argv[]) {
  time_t raw;
  struct tm * start_time;
  time(&raw);
  start_time = localtime(&raw);

  printf("Start: %sAutor: %s\nPort: %d\n", asctime (start_time), autor, PORT);
  fflush(stdout);

  struct sockaddr_in server_address, client_address;
  
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);

  int listen_socket = socket(AF_INET, SOCK_STREAM,0);
  int connect_socket;

  int opt = 1;
  setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(listen_socket));
  bind(listen_socket,(struct sockaddr*) &server_address, sizeof(server_address));
  listen(listen_socket, 7);
  while(1){
    socklen_t client_length = sizeof(client_address);
    connect_socket = accept(listen_socket, (struct sockaddr*) &client_address, &client_length);
    respond(connect_socket);
  }
}
