#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sqlite3.h>

#define PORT 2728
#define MSG_LENGTH 1024

extern int errno;		

int logic();
int logic1(char*);
int append_terte(int);

char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  strcpy (str, inet_ntoa (address.sin_addr));	
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

void toLowerCase(char* st) {
    while (*st) 
    {
      *st = tolower((unsigned char)*st);
      st++;
    }
}

int callback(void *data, int argc, char **argv, char **colNames) {
    FILE *file = (FILE *)data;

    for (int i = 0; i < argc; i++) {
        fprintf(file, "%s = %s", colNames[i], argv[i] ? argv[i] : "NULL");
        if (i < argc - 1) {
            fprintf(file, ", ");
        }
    }

    fprintf(file, "\n");

    return 0;
}

int SQLITE()
{
    sqlite3 *db;
      int rc = sqlite3_open("test.db", &db);

      if (rc != SQLITE_OK) {
          fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
          sqlite3_close(db);
          return 1;
      }

      const char *sql = "SELECT * FROM informatii_trafic;";
      FILE *outputFile = fopen("strazioriginal.txt", "w");

      if (!outputFile) {
          fprintf(stderr, "Cannot open output file for writing\n");
          sqlite3_close(db);
          return 1;
      }

      rc = sqlite3_exec(db, sql, callback, outputFile, NULL);

      if (rc != SQLITE_OK) {
          fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
      }

      fclose(outputFile);
      sqlite3_close(db);


      sqlite3_close(db);
}

char appear(int client,int option,int use)
{
    FILE* file,*decoy;
    int character;
    char cuvant[MSG_LENGTH];
    char arg1[MSG_LENGTH],arg2,arg3,arg4;
    int i = 0,k = 0,arg = 1;
    char value;

    bzero(cuvant,MSG_LENGTH);
    bzero(arg1,MSG_LENGTH);

    file = fopen("strazi/terte","r");
    decoy = fopen("strazi/decoy","w");

    if(file == NULL)
    {
      perror("Error at opening file");
    }
    if(decoy == NULL)
    {
      perror("Error at opening file");
    }

    while((character = fgetc(file)) != EOF)
    {

      if(character == '\n')
      {

        cuvant[strlen(cuvant)] = '\0';

        for(int j = 0 ; j < strlen(cuvant) ; j++)
        {
            if(cuvant[j] != ' ' && cuvant[j] != '\0')
            {
              switch(arg)
              {
                case 1:{
                  arg1[k] = cuvant[j];
                  k++;
                  break;
                }
                case 2:{
                  arg2 = cuvant[j];
                  break;
                }
                case 3:{
                  arg3 = cuvant[j];
                  break;
                }
                case 4:{
                  arg4 = cuvant[j];
                  break;
                }
              }
            }
            else
            {
              k = 0;
              arg++;
            }
        }

        arg = 1;
        k = 0;

        if(client != atoi(arg1))
        {
          fprintf(decoy,"%s %c %c %c\n",arg1,arg2,arg3,arg4);
        }
        else
        {
          fprintf(decoy,"%s ",arg1);
          if(option == 1)
          {
            if(use == 1)
            {
              if(arg2 == '0')
                { 
                  fprintf(decoy,"1 ");
                  value = '1';
                }
              else
                {
                  fprintf(decoy,"0 ");
                  value = '0';
                }
            }
            else
            {
               if(arg2 == '0')
                { 
                  fprintf(decoy,"0 ");
                  value = '0';
                }
              else
                {
                  fprintf(decoy,"1 ");
                  value = '1';
                }
            }

          }
          else
          {
            fprintf(decoy,"%c ",arg2);
          }
          if(option == 2)
          {
            if(use == 1)
            {
              if(arg3 == '0')
                { 
                  fprintf(decoy,"1 ");
                  value = '1';
                }
              else
                {
                  fprintf(decoy,"0 ");
                  value = '0';
                }
            }
            else
            {
               if(arg3 == '0')
                { 
                  fprintf(decoy,"0 ");
                  value = '0';
                }
              else
                {
                  fprintf(decoy,"1 ");
                  value = '1';
                }
            }
          }
          else
          {
            fprintf(decoy,"%c ",arg3);
          }
          if(option == 3)
          {
            if(use == 1)
            {
              if(arg4 == '0')
                { 
                  fprintf(decoy,"1\n");
                  value = '1';
                }
              else
                {
                  fprintf(decoy,"0\n");
                  value = '0';
                }
            }
            else
            {
               if(arg4 == '0')
                { 
                  fprintf(decoy,"0\n");
                  value = '0';
                }
              else
                {
                  fprintf(decoy,"1\n");
                  value = '1';
                }
            }
          }
          else
          {
            fprintf(decoy,"%c\n",arg4);
          }
        }

        bzero(cuvant,MSG_LENGTH);
        bzero(arg1,MSG_LENGTH);
        i = 0;
      }
      else
      {
          cuvant[i] = character;
          i++;
      }
    }
    
    fclose(file);
    fclose(decoy);

    file = fopen("strazi/terte","w");
    decoy = fopen("strazi/decoy","r");

    if(file == NULL)
    {
      perror("Error at opening file");
    }
    if(decoy == NULL)
    {
      perror("Error at opening file");
    }

    while((character = fgetc(decoy)) != EOF)
    { 
        fprintf(file,"%c",character);
    }

    fclose(file);
    fclose(decoy);
    return value;

}

void remove_terte(int fd)
{
    FILE *file,*decoy;
    int character;

    file = fopen("strazi/terte","w");
    decoy = fopen("strazi/decoy","r");

    if(file == NULL)
    {
      perror("Error at opening file");
    }
    if(decoy == NULL)
    {
      perror("Error at opening file");
    }

    char cuvant[MSG_LENGTH];

    bzero(cuvant,MSG_LENGTH);
    int arg = 1,k = 0,i = 0;
    char arg1[MSG_LENGTH],arg2,arg3,arg4;

    bzero(arg1,MSG_LENGTH);

    while((character = fgetc(decoy)) != EOF)
    {

      if(character == '\n')
      {

        cuvant[strlen(cuvant)] = '\0';

        for(int j = 0 ; j < strlen(cuvant) ; j++)
        {
            if(cuvant[j] != ' ' && cuvant[j] != '\0')
            {
              switch(arg)
              {
                case 1:{
                  arg1[k] = cuvant[j];
                  k++;
                  break;
                }
                case 2:{
                  arg2 = cuvant[j];
                  break;
                }
                case 3:{
                  arg3 = cuvant[j];
                  break;
                }
                case 4:{
                  arg4 = cuvant[j];
                  break;
                }
              }
            }
            else
            {
              k = 0;
              arg++;
            }
        }

        arg = 1;
        k = 0;

        if(fd != atoi(arg1))
        {
          fprintf(file,"%s %c %c %c\n",arg1,arg2,arg3,arg4);
        }

        bzero(cuvant,MSG_LENGTH);
        bzero(arg1,MSG_LENGTH);
        i = 0;
      }
      else
      {
          cuvant[i] = character;
          i++;
      }
    }
    
    fclose(file);
    fclose(decoy);

    FILE *srcfile, *destfile;
    char buffer[MSG_LENGTH];

    srcfile = fopen("strazi/terte", "rb");
    if (srcfile == NULL) {
        perror("Error opening source file");
    }

    destfile = fopen("strazi/decoy", "wb");
    if (destfile == NULL) {
        perror("Error opening destination file");
        fclose(srcfile);
    }
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcfile)) > 0) {
        fwrite(buffer, 1, bytesRead, destfile);
    }

    fclose(srcfile);
    fclose(destfile);

}

void initializare_km()
{

  FILE *srcfile, *destfile;
  char buffer[MSG_LENGTH];

  srcfile = fopen("strazi/strada_km_original", "rb");
  if (srcfile == NULL) {
      perror("Error opening source file");
  }

  destfile = fopen("strazi/strada_km", "wb");
  if (destfile == NULL) {
      perror("Error opening destination file");
      fclose(srcfile);
  }
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcfile)) > 0) {
      fwrite(buffer, 1, bytesRead, destfile);
  }

  fclose(srcfile);
  fclose(destfile);

}

struct sockaddr_in server;
  struct sockaddr_in from;
  fd_set readfds;		
  fd_set actfds;		
  struct timeval tv;		/* structura de timp pentru select() */
  int sd, client;		
  int optval=1; 			/* optiune folosita pentru setsockopt()*/ 
  int fd;			
				  
  int nfds;			
  int len;

int main ()
{			

  srand((unsigned)time(NULL));
  initializare_km();

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
      perror ("[server] Eroare la socket().\n");
      return errno;
  }

  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  bzero (&server, sizeof (server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  // server.sin_addr.s_addr = inet_addr("SERVER_IP_ADDRESS"); 
  server.sin_port = htons (PORT);

  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  {
      perror ("[server] Eroare la bind().\n");
      return errno;
  }

  if (listen (sd, 10) == -1)
  {
      perror ("[server] Eroare la listen().\n");
      return errno;
  }
  
  /* completam multimea de descriptori de citire */
  FD_ZERO (&actfds);		/* initial, multimea este vida */
  FD_SET (sd, &actfds);		/* includem in multime socketul creat */

  tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = sd;

  printf ("[server] Asteptam la portul %d...\n", PORT);
  fflush (stdout);
        
  while (1)
  {
    /* ajustam multimea descriptorilor activi (efectiv utilizati) */
    bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

    if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
    {
      perror ("[server] Eroare la select().\n");
      return errno;
    }
    /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
    if (FD_ISSET (sd, &readfds))
    {
      /* pregatirea structurii client */
      len = sizeof (from);
      bzero (&from, sizeof (from));

      /* a venit un client, acceptam conexiunea */
      client = accept (sd, (struct sockaddr *) &from, &len);

      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
        {
          perror ("[server] Eroare la accept().\n");
          continue;
        }

            if (nfds < client) /* ajusteaza valoarea maximului */
              nfds = client;
              
      /* includem in lista de descriptori activi si acest socket */
      FD_SET (client, &actfds);
      append_terte(client);

      printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));
      fflush (stdout);
    }
    /* vedem daca e pregatit vreun socket client pentru a trimite raspunsul */
    for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
    {
      /* este un socket de citire pregatit? */
      if (fd != sd && FD_ISSET (fd, &readfds))
      {	
          logic(fd);
      }
    }			/* for */

    // Stergere informatii accident
    FILE *file;
    int size;

    file = fopen("strazi/accidente","r");

    if(NULL != file)
    {
      fseek(file,0,SEEK_END);
      size = ftell(file);
      fseek(file, 0, SEEK_SET);

      if(size != 0)
      {
          char mesaj[MSG_LENGTH],ch;
          int i = 0;

          bzero(mesaj,MSG_LENGTH);

          while ((ch = fgetc(file)) != EOF) {
              if(ch != '\n')
              {
                mesaj[i] = ch;
                i++;
              }
          }
          mesaj[i - 1] = '\0';

          printf("[server]Trimitem mesajul...%s\n",mesaj);

          // Send a message to all connected clients (periodic task)
          for (fd = 0; fd <= nfds; fd++) {
              if (fd != sd && FD_ISSET(fd, &actfds)) {
                  logic1(mesaj);
              }
          }

          fclose(file);

          file = fopen("strazi/accidente","w");
          fclose(file);
      }
      else
        fclose(file);
    }
  }				/* while */
}				/* main */

int logic(int fd)
{
  char buffer[MSG_LENGTH];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */
  char msg[MSG_LENGTH];		//mesajul primit de la client 
  char msgrasp[MSG_LENGTH]=" ";        //mesaj de raspuns pentru client

  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
  {
      perror ("Eroare la read() de la client.\n");
      return 0;
  }

  if(bytes == 0)    //inchidem descriptorul unui client daca acesta este inchis abrupt
  {
      close (fd);		
      FD_CLR (fd, &actfds);

      remove_terte(fd);

      printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
      fflush (stdout);
      return 0;
  }
  else
  {

    time_t currentTime;
    time(&currentTime);

    struct tm *timeInfo = localtime(&currentTime);
    char bufferr[9];
    strftime(bufferr, sizeof(bufferr), "%H:%M:%S", timeInfo);

    printf ("[server][%s]Mesajul a fost receptionat...%s\n",bufferr, msg);

    if (strcmp(msg,"exit\n") == 0)  //in cazul in care comanda este exit inchidem descriptorul clientului
    {
        bzero(msgrasp,MSG_LENGTH);

        strcat(msgrasp,"exit");
            
        printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
            
        if (bytes && write (fd, msgrasp, MSG_LENGTH) < 0)
        {
            perror ("[server] Eroare la write() catre client.\n");
            return 0;
        }

        //inchidere de descriptori

        close (fd);		
        FD_CLR (fd, &actfds);

        remove_terte(fd);

        printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
        fflush (stdout);
        return 0;
    }
    else if(strstr(msg,"km/per/ora") != NULL)
    {

      FILE *file;
      char character;
      char strada[MSG_LENGTH],viteza[MSG_LENGTH],cuvant[MSG_LENGTH],strada_client[MSG_LENGTH],viteza_client[MSG_LENGTH];
      int i = 0,str = 1;

      bzero(strada,MSG_LENGTH);
      bzero(viteza,MSG_LENGTH);
      bzero(strada_client,MSG_LENGTH);
      bzero(viteza_client,MSG_LENGTH);
      bzero(cuvant,MSG_LENGTH);

      for(int j = 0 ; j < strlen(msg) ; j++)
      {
        if(msg[j] != '|' && msg[j] != ':')
          {
            if(str)
            {
              strada_client[i] = msg[j];
              i++;
            }
            else 
            {
              viteza_client[i] = msg[j];
              i++;
            }
          }
        else
          if(msg[j] == '|')
          {
            str = 0;
            strada_client[i] = ' ';
            strada_client[i + 1] = '\0';
            i = 0;
          }
          else if(msg[j] == ':')
          {
            viteza_client[i] = '\0';
            break;
          }
      }

      i = 0;

      file = fopen("strazi/strada_km","r");

      if(file == NULL)
      {
        perror("Error at opening file");
      }

      while((character = fgetc(file)) != EOF)
      {

        if(character == '\n')
        {

          cuvant[strlen(cuvant)] = '\0';
          strcat(viteza,cuvant);

          if(strcmp(strada_client,strada) == 0)
          {
              bzero(msgrasp,MSG_LENGTH);

              if((atoi(viteza) - atoi(viteza_client)) >= 15)
              {
                strcat(msgrasp,"1");
              }
              else if((atoi(viteza) - atoi(viteza_client)) < 15 && (atoi(viteza) - atoi(viteza_client)) > 5)
              {
                strcat(msgrasp,"2");
              }
              else if((atoi(viteza) - atoi(viteza_client)) <= 5 && (atoi(viteza) - atoi(viteza_client)) >= 0)
              {
                strcat(msgrasp,"3");
              }
              else if((atoi(viteza) - atoi(viteza_client)) < 0)
              {
                strcat(msgrasp,"4");
              }

              strcat(msgrasp,viteza);
              strcat(msgrasp," km/h");

              break;
              

          }

          bzero(strada,MSG_LENGTH);
          bzero(viteza,MSG_LENGTH);
          bzero(cuvant,MSG_LENGTH);
          i = 0;
        }
        else
        {
          if(character != ' ')
          {
            cuvant[i] = character;
            i++;
          }
          else
          {

            cuvant[strlen(cuvant)] = '\0';

            if(strcmp(cuvant,"kilometri/per/ora") != 0)
            {
                strcat(strada,cuvant);
                strcat(strada," ");
            }
            bzero(cuvant,MSG_LENGTH);
            i = 0;
          }
        }
      }
      
      fclose(file);

      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
          
      if (bytes && write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }
      
      return bytes;
    }
    else if(strstr(msg,"|acc/ident"))
    {

      char strada_client[MSG_LENGTH],viteza_client[MSG_LENGTH];
      int i = 0;

      bzero(strada_client,MSG_LENGTH);
      bzero(viteza_client,MSG_LENGTH);

      for(int j = 0 ; j < strlen(msg) ; j++)
      {
        if(msg[j] != '|')
          {
  
            strada_client[i] = msg[j];
            i++;
          }
        else
          {
            strada_client[i] = ' ';
            strada_client[i + 1] = '\0';
            break;
          }
      }

      FILE *file;
      file = fopen("strazi/accidente", "a");

      if (file == NULL) {
          perror("Error opening file");
          return 1;
      }
      fprintf(file, "%s\n",strada_client);

      fclose(file);

      ////////////////////

      FILE *filee;
      filee = fopen("strazi/decoy_accidente", "w");

      fclose(filee);

      filee = fopen("strazi/decoy_accidente", "a");


      if (filee == NULL) {
          perror("Error opening file");
          return 1;
      }

      i = 0;
      char character;
      char strada[MSG_LENGTH],viteza[MSG_LENGTH],cuvant[MSG_LENGTH];

      bzero(strada,MSG_LENGTH);
      bzero(viteza,MSG_LENGTH);
      bzero(cuvant,MSG_LENGTH);

      file = fopen("strazi/strada_km","r");

      if(file == NULL)
      {
        perror("Error at opening file");
      }

      while((character = fgetc(file)) != EOF)
      {

        if(character == '\n')
        {

          cuvant[strlen(cuvant)] = '\0';
          strcat(viteza,cuvant);

          if(strcmp(strada_client,strada) == 0)
          {

              /////

              FILE *work;
              int ch,m = 0;
              char cuv[MSG_LENGTH],str[MSG_LENGTH],vit[MSG_LENGTH];

              bzero(cuv,MSG_LENGTH);
              bzero(str,MSG_LENGTH);
              bzero(vit,MSG_LENGTH);


              work = fopen("strazi/strada_km_original","r");

              if(work == NULL)
              {
                perror("Error at opening file");
              }

              while((ch = fgetc(work)) != EOF)
              {

                if(ch == '\n')
                {

                  cuv[strlen(cuv)] = '\0';
                  strcat(vit,cuv);

                  if(strcmp(strada_client,str) == 0)
                  {
                      break;
                  }

                  bzero(str,MSG_LENGTH);
                  bzero(vit,MSG_LENGTH);
                  bzero(cuv,MSG_LENGTH);
                  m = 0;
                }
                else
                {
                  if(ch != ' ')
                  {
                    cuv[m] = ch;
                    m++;
                  }
                  else
                  {

                    cuv[strlen(cuv)] = '\0';

                    if(strcmp(cuv,"kilometri/per/ora") != 0)
                    {
                        strcat(str,cuv);
                        strcat(str," ");
                    }
                    bzero(cuv,MSG_LENGTH);
                    m = 0;
                  }
                }
              }
              
              fclose(work);

              /////

            int vit_original_int = atoi(vit);
            int vit_int = atoi(viteza);

            if(vit_int == vit_original_int)
              vit_int -= vit_int * 0.2;

            sprintf(viteza,"%d",vit_int);

          }

          fprintf(filee, "%skilometri/per/ora %s\n",strada,viteza);  


          bzero(strada,MSG_LENGTH);
          bzero(viteza,MSG_LENGTH);
          bzero(cuvant,MSG_LENGTH);
          i = 0;
        }
        else
        {
          if(character != ' ')
          {
            cuvant[i] = character;
            i++;
          }
          else
          {

            cuvant[strlen(cuvant)] = '\0';

            if(strcmp(cuvant,"kilometri/per/ora") != 0)
            {
                strcat(strada,cuvant);
                strcat(strada," ");
            }
            bzero(cuvant,MSG_LENGTH);
            i = 0;
          }
        }
      }

      fclose(filee);
      fclose(file);

      FILE *srcfile, *destfile;
      char buffer[MSG_LENGTH];

      srcfile = fopen("strazi/decoy_accidente", "rb");
      if (srcfile == NULL) {
          perror("Error opening source file");
      }

      destfile = fopen("strazi/strada_km", "wb");
      if (destfile == NULL) {
          perror("Error opening destination file");
          fclose(srcfile);
      }
      size_t bytesRead;
      while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcfile)) > 0) {
          fwrite(buffer, 1, bytesRead, destfile);
      }

      fclose(srcfile);
      fclose(destfile);

      ///////////////////

      srcfile = fopen("strazi/decoy_accidente", "w");
      if (srcfile == NULL) {
          perror("Error opening source file");
      }
      
      fclose(srcfile);

      return bytes;
    }
    else if(strstr(msg,"|switch_street"))
    {

      FILE *file;
      char character;
      char strada[MSG_LENGTH],viteza[MSG_LENGTH],cuvant[MSG_LENGTH],strada_client[MSG_LENGTH],viteza_client[MSG_LENGTH];
      int i = 0;

      bzero(strada,MSG_LENGTH);
      bzero(viteza,MSG_LENGTH);
      bzero(strada_client,MSG_LENGTH);
      bzero(viteza_client,MSG_LENGTH);
      bzero(cuvant,MSG_LENGTH);

      for(int j = 0 ; j < strlen(msg) ; j++)
      {
        if(msg[j] != '|')
          {
  
            strada_client[i] = msg[j];
            i++;
          }
        else
          {
            strada_client[i] = ' ';
            strada_client[i + 1] = '\0';
            break;
          }
      }
      i = 0;

      ///////////////////////////////

      file = fopen("strazi/strada_km","r");

      if(file == NULL)
      {
        perror("Error at opening file");
      }

      while((character = fgetc(file)) != EOF)
      {

        if(character == '\n')
        {

          cuvant[strlen(cuvant)] = '\0';
          strcat(viteza,cuvant);

          if(strcmp(strada_client,strada) == 0)
          {
              bzero(msgrasp,MSG_LENGTH);

              int random_vreme = rand() % 3;

              if(random_vreme == 0)
              {
                strcat(msgrasp,"0");
                strcat(msgrasp," ");
                strcat(msgrasp,viteza);
                strcat(msgrasp," km/h");
              }
              else if(random_vreme == 1)
              {
                strcat(msgrasp,"1");
                strcat(msgrasp," ");

                int vi;
                vi = atoi(viteza);
                vi -= 5;

                sprintf(viteza,"%d",vi);
                strcat(msgrasp,viteza);
                strcat(msgrasp," km/h");
              }
              else if(random_vreme == 2)
              {
                strcat(msgrasp,"2");
                strcat(msgrasp," ");

                int vi;
                vi = atoi(viteza);
                vi -= 10;

                strcat(msgrasp,viteza);
                strcat(msgrasp," km/h");
              }
              
              break;
              

          }

          bzero(strada,MSG_LENGTH);
          bzero(viteza,MSG_LENGTH);
          bzero(cuvant,MSG_LENGTH);
          i = 0;
        }
        else
        {
          if(character != ' ')
          {
            cuvant[i] = character;
            i++;
          }
          else
          {

            cuvant[strlen(cuvant)] = '\0';

            if(strcmp(cuvant,"kilometri/per/ora") != 0)
            {
                strcat(strada,cuvant);
                strcat(strada," ");
            }
            bzero(cuvant,MSG_LENGTH);
            i = 0;
          }
        }
      }
      
      fclose(file);

      ///////////////////////////////
      
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
          
      if (bytes && write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }
      
      return bytes;
    }
    else if(strstr(msg,"|vreme"))
    {
      bzero(msgrasp,MSG_LENGTH);

      int number_of_lines = 1,i = 0,index_line = 1;

      /////

      FILE *file;
      char character;

      file = fopen("info/vreme", "r");

      if (file == NULL) {
          perror("Error opening file");
          return 1; 
      }

      while ((character = getc(file)) != EOF) {
          if(character == '\n')
            number_of_lines++;
      }

      number_of_lines--;
      fclose(file);

      /////
    
      if(appear(fd,1,1) == '1')
        {
          int random = rand() % number_of_lines + 1;

          file = fopen("info/vreme", "r");

          if (file == NULL) {
          perror("Error opening file");
          return 1; 
          }

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                index_line++;
              if(index_line == random && character != '\n')
              {
                msgrasp[i] = character;
                i++;
              }
          }
        }
      else
        strcat(msgrasp,"Arata Vreme");
      
      strcat(msgrasp,"\0");
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
          
      if (bytes && write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }

      return bytes;
    }
    else if(strstr(msg,"|sport"))
    {
      bzero(msgrasp,MSG_LENGTH);

      int number_of_lines = 1,i = 0,index_line = 1;

      /////

      FILE *file;
      char character;

      file = fopen("info/sport", "r");

      if (file == NULL) {
          perror("Error opening file");
          return 1; 
      }

      while ((character = getc(file)) != EOF) {
          if(character == '\n')
            number_of_lines++;
      }

      number_of_lines--;
      fclose(file);

      /////
    
      if(appear(fd,2,1) == '1')
        {
          int random = rand() % number_of_lines + 1;
          int number_index = 0;

          char cuvinte[MSG_LENGTH];

          bzero(cuvinte,MSG_LENGTH);

          file = fopen("info/sport", "r");

          if (file == NULL) {
          perror("Error opening file");
          return 1; 
          }

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                index_line++;
              if(index_line == random && character != '\n')
              {
                cuvinte[i] = character;
                i++;
              }
              if(character == ' ' || character == '\n')
              {
                if(strlen(cuvinte) + number_index <= 22)
                {
                  strcat(msgrasp,cuvinte);
                  number_index += strlen(cuvinte);
                  bzero(cuvinte,MSG_LENGTH);
                  i = 0;
                }
                else
                {
                  strcat(msgrasp,"\n");
                  strcat(msgrasp,cuvinte);
                  number_index = strlen(cuvinte);

                  bzero(cuvinte,MSG_LENGTH);
                  i = 0;
                }
              }
          }
        }
      else
        strcat(msgrasp,"Arata Stiri Sportive");

      strcat(msgrasp,"\0");
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
          
      if (bytes && write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }

      return bytes;
    }
    else if(strstr(msg,"|preturi"))
    {
      bzero(msgrasp,MSG_LENGTH);

      int number_of_lines = 1,i = 0,index_line = 1;

      /////

      FILE *file;
      char character;

      file = fopen("info/preturi", "r");

      if (file == NULL) {
          perror("Error opening file");
          return 1; 
      }

      while ((character = getc(file)) != EOF) {
          if(character == '\n')
            number_of_lines++;
      }

      number_of_lines--;
      fclose(file);

      /////
    
      if(appear(fd,3,1) == '1')
        {

          char companie[MSG_LENGTH];
          char combustibil[MSG_LENGTH];
          char spaces[MSG_LENGTH];

          bzero(companie,MSG_LENGTH);
          bzero(combustibil,MSG_LENGTH);
          bzero(spaces,MSG_LENGTH);

          int random = rand() % number_of_lines + 1;

          file = fopen("info/preturi", "r");

          if (file == NULL) {
          perror("Error opening file");
          return 1; 
          }

          int there_was_a_space = 0;

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                index_line++;
              if(index_line == random && character != '\n')
              {
                if(character == ' ' && there_was_a_space == 0)
                {
                  there_was_a_space = 1;
                  i = 0;
                }
                else
                {
                  if(there_was_a_space == 0)
                  {
                    companie[i] = character;
                    i++;
                  }
                  else
                  {
                    combustibil[i] = character;
                    i++;
                  }
                }
              }
          }
          
          for(int l = 0 ; l <= 26 + 9 - strlen(companie); l++)
          {
            strcat(msgrasp," ");
          }

          strcat(msgrasp,companie);
          strcat(msgrasp,"\n");
          strcat(msgrasp,combustibil);
        }
      else
        strcat(msgrasp,"Arata Pret Combustibili");

      strcat(msgrasp,"\0");
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
          
      if (bytes && write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }

      return bytes;
    }
    else if(strstr(msg,"|send/news"))
    {
      bzero(msgrasp,MSG_LENGTH);

      int number_of_lines = 1,i = 0,index_line = 1,Ok = 0;
      FILE *file;
      char character;
    
      if(appear(fd,1,2) == '1')
        {
          file = fopen("info/vreme", "r");

          if (file == NULL) {
              perror("Error opening file");
              return 1; 
          }

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                number_of_lines++;
          }

          number_of_lines--;
          fclose(file);
          //////////////////////
          Ok = 1;
          int random = rand() % number_of_lines + 1;

          file = fopen("info/vreme", "r");

          if (file == NULL) {
          perror("Error opening file");
          return 1; 
          }

          msgrasp[i] = '1';
          i++;

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                index_line++;
              if(index_line == random && character != '\n')
              {
                msgrasp[i] = character;
                i++;
              }
          }
          strcat(msgrasp,"\n");
          i++;
          fclose(file);
        }
        number_of_lines = 1;
        index_line = 1;
        if(appear(fd,2,2) == '1')
        {
          file = fopen("info/sport", "r");

          if (file == NULL) {
              perror("Error opening file");
              return 1; 
          }

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                number_of_lines++;
          }

          number_of_lines--;
          fclose(file);
          /////////
          Ok = 1;
          int random = rand() % number_of_lines + 1;

          file = fopen("info/sport", "r");

          if (file == NULL) {
          perror("Error opening file");
          return 1; 
          }

          msgrasp[i] = '2';
          i++;

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                index_line++;
              if(index_line == random && character != '\n')
              {
                msgrasp[i] = character;
                i++;
              }
          }
          strcat(msgrasp,"\n");
          i++;
          fclose(file);
        }
        number_of_lines = 1;
        index_line = 1;
        if(appear(fd,3,2) == '1')
        {
          file = fopen("info/preturi", "r");

          if (file == NULL) {
              perror("Error opening file");
              return 1; 
          }

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                number_of_lines++;
          }

          number_of_lines--;
          fclose(file);
          /////////
          Ok = 1;
          int random = rand() % number_of_lines + 1;

          file = fopen("info/preturi", "r");

          if (file == NULL) {
          perror("Error opening file");
          return 1; 
          }

          msgrasp[i] = '3';
          i++;

          while ((character = getc(file)) != EOF) {
              if(character == '\n')
                index_line++;
              if(index_line == random && character != '\n')
              {
                msgrasp[i] = character;
                i++;
              }
          }
          strcat(msgrasp,"\n");
          i++;
          fclose(file);
        }

    if(Ok)
    {
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);

      if (write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }
    }
    else
    {
      strcat(msgrasp,"NO NEWS TO SEND\n");
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);

      if (write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }
    }


    return 1;
    }
    else
    {
      bzero(msgrasp,MSG_LENGTH);
      strcat(msgrasp,msg);
      
      printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
          
      if (bytes && write (fd, msgrasp, strlen(msgrasp)) < 0)
      {
          perror ("[server] Eroare la write() catre client.\n");
          return 0;
      }

      return bytes;                                                                       //
    }
  }
}

int logic1(char *message)
{
    char msgrasp[MSG_LENGTH]=" "; 

    bzero(msgrasp,MSG_LENGTH);
    strcat(msgrasp,":A fost raportat un incident\npe strada ");
    strcat(msgrasp,message);
    strcat(msgrasp,".\nReduceti viteza si conduceti\ncu mare atentie!");
        
    printf("%s\n",msgrasp);

    if (write (fd, msgrasp, strlen(msgrasp)) < 0)
    {
        perror ("[server] Eroare la write() catre client.\n");
        return 0;
    }
    
    return 1;
}

int append_terte(int client)
{
  FILE *file;
  FILE *decoy;
  file = fopen("strazi/terte", "a");
  decoy = fopen("strazi/decoy", "a");

  if (file == NULL) {
      perror("Error opening file");
      return 1;
  }
  if (decoy == NULL) {
      perror("Error opening file");
      return 1;
  }

  fprintf(file,"%d %d %d %d\n",client,0,0,0);
  fprintf(decoy,"%d %d %d %d\n",client,0,0,0);


  fclose(file);
  fclose(decoy);
  return 0;
}
