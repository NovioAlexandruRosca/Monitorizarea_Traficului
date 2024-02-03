#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>

extern int errno;
#define MSG_LENGTH 1024

int send_speed = 0;
int switch_street = 0;
int send_news = 0;
int OK = 1,last_street,has_changed = 1;

struct Info_Strada
{
    char strada[100];
    int index;

}Data[100];

struct Neighbour
{
    int nb_of_nei;
    int nei[5];
}neigh[21];

void setNonBlockingMode() {
    struct termios state;
    tcgetattr(STDIN_FILENO, &state);
    state.c_lflag &= ~(ICANON | ECHO);
    state.c_cc[VMIN] = 0;
    state.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &state);
}

void setBlockingMode() {
    struct termios state;

    tcgetattr(STDIN_FILENO, &state);
    state.c_lflag |= (ICANON | ECHO);
    state.c_cc[VMIN] = 1;
    state.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &state);
}

int port,nb_of_str,str_index = 1,speed_rn = 60;

int initialize()
{

    for(int kl = 0 ; kl <= 20 ; kl++)
    {
        switch (kl)
        {
            case 0:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 2;neigh[kl].nei[1] = 4;neigh[kl].nei[2] = 5;neigh[kl].nei[3] = 21;break;
            case 1:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 1;neigh[kl].nei[1] = 3;neigh[kl].nei[2] = 19;break;
            case 2:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 2;neigh[kl].nei[1] = 4;neigh[kl].nei[2] = 19;neigh[kl].nei[3] = 20;break;
            case 3:neigh[kl].nb_of_nei = 5;neigh[kl].nei[0] = 1;neigh[kl].nei[1] = 3;neigh[kl].nei[2] = 19;neigh[kl].nei[3] = 5;neigh[kl].nei[4] = 21;break;
            case 4:neigh[kl].nb_of_nei = 5;neigh[kl].nei[0] = 1;neigh[kl].nei[1] = 21;neigh[kl].nei[2] = 4;neigh[kl].nei[3] = 6;neigh[kl].nei[4] = 10;break;
            case 5:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 5;neigh[kl].nei[1] = 7;neigh[kl].nei[2] = 10;break;
            case 6:neigh[kl].nb_of_nei = 2;neigh[kl].nei[0] = 6;neigh[kl].nei[1] = 8;break;
            case 7:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 7;neigh[kl].nei[1] = 9;neigh[kl].nei[2] = 11;break;
            case 8:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 10;neigh[kl].nei[1] = 11;neigh[kl].nei[2] = 8;break;
            case 9:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 6;neigh[kl].nei[1] = 9;neigh[kl].nei[2] = 5;break;
            case 10:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 9;neigh[kl].nei[1] = 12;neigh[kl].nei[2] = 8;neigh[kl].nei[3] = 15;break;
            case 11:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 11;neigh[kl].nei[1] = 13;neigh[kl].nei[2] = 15;break;
            case 12:neigh[kl].nb_of_nei = 2;neigh[kl].nei[0] = 12;neigh[kl].nei[1] = 14;break;
            case 13:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 13;neigh[kl].nei[1] = 16;neigh[kl].nei[2] = 17;break;
            case 14:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 11;neigh[kl].nei[1] = 12;neigh[kl].nei[2] = 16;neigh[kl].nei[3] = 21;break;
            case 15:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 14;neigh[kl].nei[1] = 15;neigh[kl].nei[2] = 17;neigh[kl].nei[3] = 21;break;
            case 16:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 14;neigh[kl].nei[1] = 16;neigh[kl].nei[2] = 18;neigh[kl].nei[3] = 19;break;
            case 17:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 17;neigh[kl].nei[1] = 19;neigh[kl].nei[2] = 20;break;
            case 18:neigh[kl].nb_of_nei = 4;neigh[kl].nei[0] = 17;neigh[kl].nei[1] = 18;neigh[kl].nei[2] = 2;neigh[kl].nei[3] = 3;break;
            case 19:neigh[kl].nb_of_nei = 3;neigh[kl].nei[0] = 18;neigh[kl].nei[1] = 3;neigh[kl].nei[2] = 4;break;
            case 20:neigh[kl].nb_of_nei = 5;neigh[kl].nei[0] = 1;neigh[kl].nei[1] = 4;neigh[kl].nei[2] = 5;neigh[kl].nei[3] = 15;neigh[kl].nei[4] = 16;break;
        }
    }

    FILE *file;
    
    file = fopen("strazi/titlu_strazi", "r");

    if (file == NULL) {
        perror("Err opening file");
        return -1;
    }

    int i = 0,ch,j = 0;
    char strada[100];

    while ((ch = getc(file)) != EOF) {
        if(ch != '\n')
        {
            
            strada[i] = ch;
            i++;
            
        }
        else
        {

            j++;
            Data[j].index = j;
            strcpy(Data[j].strada,strada);
            memset(strada, 0, strlen(strada));
            i = 0;
              
        }
    }
    nb_of_str = j;

    fclose(file);
    return 0;
       
}

void modify_pret(char* stiri1)
{
    strcpy(stiri1,stiri1 + 1);

    char companie[MSG_LENGTH];
    char combustibil[MSG_LENGTH];

    int there_was_a_space = 0;
    int j = 0;

    for(int i = 0 ; i < strlen(stiri1) ; i++)
    {
        if(stiri1[i] == ' ' && there_was_a_space == 0)
        {
            there_was_a_space = 1;
            companie[j] = '\0';
            j = 0;
        }
        else 
        {
            if(there_was_a_space == 0)
            {
                companie[j] = stiri1[i];
                j++;
            }
            else
            {
                combustibil[j] = stiri1[i];
                j++;
            }
        }
    }

    combustibil[j] = '\0';
    
    strcpy(stiri1,"");
    
    for(int l = 0 ; l <= 26 + 9 - strlen(companie); l++)
    {
    strcat(stiri1," ");
    }

    strcat(stiri1,companie);
    strcat(stiri1,"\n");
    strcat(stiri1,combustibil);
}

void modify_sport(char* stiri1)
{
    char cuvinte[MSG_LENGTH];
    int j = 0;
    int  number_index = 0;
    char rasp[MSG_LENGTH];

    bzero(rasp,MSG_LENGTH);
    bzero(cuvinte,MSG_LENGTH);

    strcpy(stiri1,stiri1 + 1);

    for(int i = 0 ; i < strlen(stiri1) ; i++)
    {
              
        if(stiri1[i] != '\n')
        {
            cuvinte[j] = stiri1[i];
            j++;
        }
        if(stiri1[i] == ' ' || stiri1[i] == '\n')
        {
        if(strlen(cuvinte) + number_index <= 22)
        {
            strcat(rasp,cuvinte);
            number_index += strlen(cuvinte);
            bzero(cuvinte,MSG_LENGTH);
            j = 0;
        }
        else
        {
            strcat(rasp,"\n");
            strcat(rasp,cuvinte);
            number_index = strlen(cuvinte);

            bzero(cuvinte,MSG_LENGTH);
            j = 0;
        }
        }
    }

    strcpy(stiri1,rasp);

}

int sd;
GtkWidget *speed,*street,*rightLabel1,*bottomLabel2,*rightLabel2,*bottomLabel1,*IMAGE;
int able_to_send = 1;
int seconds = 0;
int acc_seconds = 0;

//////////////////////////////////////////GTK

void button_clicked_temp(GtkWidget *widget, gpointer data) {
    
    char send_speed[MSG_LENGTH],speed_msg[MSG_LENGTH];
    int bytes;

    bzero(send_speed,MSG_LENGTH);
    bzero(speed_msg,MSG_LENGTH);

    strcat(send_speed,"|vreme");

    while(able_to_send == 0){}

    if (write (sd, send_speed, MSG_LENGTH) <= 0)
    {
        perror ("Eroare la write() spre server.\n");
    }

    able_to_send = 0;

    if ((bytes = read (sd, speed_msg, MSG_LENGTH)) < 0)
    {
        perror ("Eroare la read() de la server.\n");
    }

    able_to_send = 1;

    speed_msg[strlen(speed_msg)] = '\0';

    g_print("[client]Update vreme:%s\n",speed_msg);

    if(bytes == 0)
    {
        printf ("Serverul a fost inchis\n");
        gtk_main_quit();
    }

    const gchar *current_label = gtk_button_get_label(GTK_BUTTON(widget));

    gchar *new_label = g_strdup_printf("New Label: %s", current_label);
    gtk_button_set_label(GTK_BUTTON(widget), speed_msg);
    g_free(new_label);

}

void button_clicked_pret(GtkWidget *widget, gpointer data) {
    
    char send_pret[MSG_LENGTH],pret_msg[MSG_LENGTH];
    int bytes;

    bzero(send_pret,MSG_LENGTH);
    bzero(pret_msg,MSG_LENGTH);

    strcat(send_pret,"|preturi");

    while(able_to_send == 0){}

    if (write (sd, send_pret, MSG_LENGTH) <= 0)
    {
        perror ("Eroare la write() spre server.\n");
    }

    able_to_send = 0;

    if ((bytes = read (sd, pret_msg, MSG_LENGTH)) < 0)
    {
        perror ("Eroare la read() de la server.\n");
    }

    able_to_send = 1;

    pret_msg[strlen(pret_msg)] = '\0';

    g_print("[client]Carburanti:%s\n",pret_msg);

    if(bytes == 0)
    {
        printf ("Serverul a fost inchis\n");
        gtk_main_quit();
    }

    const gchar *current_label = gtk_button_get_label(GTK_BUTTON(widget));

    gchar *new_label = g_strdup_printf("New Label: %s", current_label);
    gtk_button_set_label(GTK_BUTTON(widget), pret_msg);
    g_free(new_label);

}

void button_clicked_sport(GtkWidget *widget, gpointer data) {
    
    char send_sport[MSG_LENGTH],sport_msg[MSG_LENGTH];
    int bytes;

    bzero(send_sport,MSG_LENGTH);
    bzero(sport_msg,MSG_LENGTH);

    strcat(send_sport,"|sport");

    while(able_to_send == 0){}

    if (write (sd, send_sport, MSG_LENGTH) <= 0)
    {
        perror ("Eroare la write() spre server.\n");
    }

    able_to_send = 0;

    if ((bytes = read (sd, sport_msg, MSG_LENGTH)) < 0)
    {
        perror ("Eroare la read() de la server.\n");
    }

    able_to_send = 1;

    sport_msg[strlen(sport_msg)] = '\0';

    g_print("[client]Sport Update:%s\n",sport_msg);

    if(bytes == 0)
    {
        printf ("Serverul a fost inchis\n");
        gtk_main_quit();
    }

    const gchar *current_label = gtk_button_get_label(GTK_BUTTON(widget));

    gchar *new_label = g_strdup_printf("New Label: %s", current_label);
    gtk_button_set_label(GTK_BUTTON(widget), sport_msg);
    g_free(new_label);

}

void button_clicked_up(GtkWidget *widget, gpointer data) {
    
    if(speed_rn <= 195)
        speed_rn += 5;

    char speedy[MSG_LENGTH];

    bzero(speedy,MSG_LENGTH);

    sprintf(speedy,"%d km/h",speed_rn);

    gtk_label_set_text(GTK_LABEL(speed), speedy);

}

void button_clicked_down(GtkWidget *widget, gpointer data) {
    
    if(speed_rn >= 5)
        speed_rn -= 5;

    char speedy[MSG_LENGTH];

    bzero(speedy,MSG_LENGTH);

    sprintf(speedy,"%d km/h",speed_rn);

    gtk_label_set_text(GTK_LABEL(speed), speedy);

}

void button_clicked_accident(GtkWidget *widget, gpointer data)
{
    char send_accident[MSG_LENGTH];
    int bytes;

    bzero(send_accident,MSG_LENGTH);

    strcat(send_accident,Data[str_index].strada);
    strcat(send_accident,"|acc/ident");

    if(acc_seconds <= 0)
    {
        while(able_to_send == 0){}

        if (write (sd, send_accident, MSG_LENGTH) <= 0)
        {
            perror ("Eroare la write() spre server.\n");
        }
    }
    acc_seconds = 2;


}

gboolean update_max_speed(GtkWidget *widget)
{

    char send_speed[MSG_LENGTH],speed_msg[MSG_LENGTH];
    char viteza[MSG_LENGTH];
    int bytes;

    bzero(send_speed,MSG_LENGTH);
    bzero(speed_msg,MSG_LENGTH);
    bzero(viteza,MSG_LENGTH);

    sprintf(viteza,"%d",speed_rn);

    strcat(send_speed,Data[str_index].strada);
    strcat(send_speed,"|");
    strcat(send_speed,viteza);
    strcat(send_speed,":km/per/ora");

    while(able_to_send == 0){}

    if (write (sd, send_speed, MSG_LENGTH) <= 0)
    {
        perror ("Eroare la write() spre server.\n");
    }

    able_to_send = 0;

    if ((bytes = read (sd, speed_msg, MSG_LENGTH)) < 0)
    {
        perror ("Eroare la read() de la server.\n");
    }

    able_to_send = 1;

    speed_msg[strlen(speed_msg)] = '\0';

    g_print("[client]Viteza maxima pe strada %s este %s\n",Data[str_index].strada,speed_msg);

    if(bytes == 0)
    {
        printf ("Serverul a fost inchis\n");
        gtk_main_quit();
    }


    char type = speed_msg[0];
    strcpy(speed_msg,speed_msg + 1);


    GtkStyleContext *context6 = gtk_widget_get_style_context(speed);

    gtk_style_context_add_class(context6, "speed-button");

    if(type == '1')
    {
        const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #4CAF50; font-family: 'DejaVu Sans';}";

        GtkCssProvider *provider6 = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
        gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
    else if(type == '2')
    {
        const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #FFC107; font-family: 'DejaVu Sans';}";

        GtkCssProvider *provider6 = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
        gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
    else if(type == '3')
    {
        const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #FF9800; font-family: 'DejaVu Sans';}";

        GtkCssProvider *provider6 = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
        gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
    else if(type == '4')
    {
        const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #F44336; font-family: 'DejaVu Sans';}";

        GtkCssProvider *provider6 = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
        gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    gtk_label_set_text(GTK_LABEL(widget), speed_msg);
  
    return TRUE;
}

gboolean update_street(GtkWidget *widget)
{

    char street_msg[MSG_LENGTH];
    char switch_strada[MSG_LENGTH];
    int bytes;

    bzero(street_msg,MSG_LENGTH);
    bzero(switch_strada,MSG_LENGTH);

    str_index++;
    if(str_index > nb_of_str)
        str_index = 1;

    sprintf(switch_strada,"%s|switch_street\n",Data[str_index].strada);

    while(able_to_send == 0){}

    if (write (sd, switch_strada, MSG_LENGTH) <= 0)
    {
        perror ("Eroare la write() spre server.\n");
    }

    able_to_send = 0;

    if ((bytes = read (sd, street_msg, MSG_LENGTH)) < 0)
    {
        perror ("Eroare la read() de la server.\n");
    }

    able_to_send = 1;

    street_msg[strlen(street_msg)] = '\0';

    g_print("[client]S-a schimbat strada in %s unde viteza maxima este %s\n",Data[str_index].strada,street_msg);

    if(bytes == 0)
    {
        printf ("Serverul a fost inchis\n");
        gtk_main_quit();
    }

    gtk_label_set_text(GTK_LABEL(widget), street_msg);
    gtk_label_set_text(GTK_LABEL(street), Data[str_index].strada);

    update_max_speed(widget);
  
    return TRUE;
}

gboolean accident(GtkWidget *widget)
{

    int bytesread;
    char buffer[MSG_LENGTH];

    bzero(buffer,MSG_LENGTH);
    
    if(able_to_send == 1)
    {
        bytesread = recv(sd,buffer,sizeof(buffer),MSG_DONTWAIT);

        buffer[strlen(buffer)] = '\0';

        if(bytesread > 0)
        {
            g_print("[client]Accident:%s\n",buffer);
            gtk_label_set_text(GTK_LABEL(rightLabel1), buffer);
        }
    }
    return TRUE;
}

gboolean periodic(GtkWidget *widget)
{
    seconds++;
    acc_seconds--;

    if(seconds % 15 == 0)
    {
        if(OK == 1)
        {
            OK = 2;
            send_news = 1;
            switch_street = 1;
        }
        else if(OK == 2)
        {
            OK = 3;
            send_news = 1;
        }
        else if(OK == 3)
        {
            OK = 4;
            send_news = 1;
            switch_street = 1;
        }
        else if(OK == 4)
        {
            OK = 1;
            send_news = 1;
            send_speed = 1;
        }
    }

    if(send_speed)
    {
        send_speed = 0;

        char send_speed[MSG_LENGTH],speed_msg[MSG_LENGTH];
        char viteza[MSG_LENGTH];
        int bytes;

        bzero(send_speed,MSG_LENGTH);
        bzero(speed_msg,MSG_LENGTH);
        bzero(viteza,MSG_LENGTH);

        sprintf(viteza,"%d",speed_rn);

        strcat(send_speed,Data[str_index].strada);
        strcat(send_speed,"|");
        strcat(send_speed,viteza);
        strcat(send_speed,":km/per/ora");

        while(able_to_send == 0){}

        if (write (sd, send_speed, MSG_LENGTH) <= 0)
        {
            perror ("Eroare la write() spre server.\n");
        }

        able_to_send = 0;

        if ((bytes = read (sd, speed_msg, MSG_LENGTH)) < 0)
        {
            perror ("Eroare la read() de la server.\n");
        }

        able_to_send = 1;

        speed_msg[strlen(speed_msg)] = '\0';

        g_print("[client]Viteza maxima pe strada %s este %s\n",Data[str_index].strada,speed_msg);

        if(bytes == 0)
        {
            printf ("Serverul a fost inchis\n");
            gtk_main_quit();
        }


        char type = speed_msg[0];
        strcpy(speed_msg,speed_msg + 1);


        GtkStyleContext *context6 = gtk_widget_get_style_context(speed);

        gtk_style_context_add_class(context6, "speed-button");

        if(type == '1')
        {
            const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #4CAF50; font-family: 'DejaVu Sans';}";

            GtkCssProvider *provider6 = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
            gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
        }
        else if(type == '2')
        {
            const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #FFC107; font-family: 'DejaVu Sans';}";

            GtkCssProvider *provider6 = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
            gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
        }
        else if(type == '3')
        {
            const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #FF9800; font-family: 'DejaVu Sans';}";

            GtkCssProvider *provider6 = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
            gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
        }
        else if(type == '4')
        {
            const char *css6 = "label.speed-button {color:white;font-weight :bold;font-size: 15px; background-color: #F44336; font-family: 'DejaVu Sans';}";

            GtkCssProvider *provider6 = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider6, css6, -1, NULL);
            gtk_style_context_add_provider(context6, GTK_STYLE_PROVIDER(provider6), GTK_STYLE_PROVIDER_PRIORITY_USER);
        }

        gtk_label_set_text(GTK_LABEL(widget), speed_msg);
    }

    if(switch_street)
    {
        switch_street = 0;

        char street_msg[MSG_LENGTH];
        char switch_strada[MSG_LENGTH];
        int bytes;

        bzero(street_msg,MSG_LENGTH);
        bzero(switch_strada,MSG_LENGTH);

        int temp;

        do{
            temp = neigh[str_index - 1].nei[rand() % neigh[str_index - 1].nb_of_nei];
        }while(temp == last_street);

        last_street = str_index;
        str_index = temp;


        char path_to_picture[100];
        sprintf(path_to_picture,"poze/%d.png",str_index);

        gtk_image_set_from_file(GTK_IMAGE(IMAGE), path_to_picture);

        sprintf(switch_strada,"%s|switch_street\n",Data[str_index].strada);

        while(able_to_send == 0){}

        if (write (sd, switch_strada, MSG_LENGTH) <= 0)
        {
            perror ("Eroare la write() spre server.\n");
        }

        able_to_send = 0;

        if ((bytes = read (sd, street_msg, MSG_LENGTH)) < 0)
        {
            perror ("Eroare la read() de la server.\n");
        }

        able_to_send = 1;

        street_msg[strlen(street_msg)] = '\0';

        g_print("[client]S-a schimbat strada in %s unde viteza maxima este %s\n",Data[str_index].strada,street_msg);

        if(bytes == 0)
        {
            printf ("Serverul a fost inchis\n");
            gtk_main_quit();
        }

        char symbol = street_msg[0];

        gtk_label_set_text(GTK_LABEL(widget), street_msg + 2);

        char stradaa[MSG_LENGTH];

        bzero(stradaa,MSG_LENGTH);

        if(symbol == '0')
            strcat(stradaa,"☀️");
        else if(symbol == '1')
            strcat(stradaa,"🌧️");
        else if(symbol == '2')
            strcat(stradaa,"❄️");

        strcat(stradaa,Data[str_index].strada);

        gtk_label_set_text(GTK_LABEL(street), stradaa);

        update_max_speed(widget);

    }

    if(send_news)
    {
        send_news = 0;

        char news_msg[MSG_LENGTH];
        char send_news[MSG_LENGTH];
        int bytes;

        bzero(news_msg,MSG_LENGTH);
        bzero(send_news,MSG_LENGTH);

        strcat(send_news,"|send/news");

        while(able_to_send == 0){}

        if (write (sd, send_news, MSG_LENGTH) <= 0)
        {
            perror ("Eroare la write() spre server.\n");
        }

        able_to_send = 0;

        if ((bytes = read (sd, news_msg, MSG_LENGTH)) < 0)
        {
            perror ("Eroare la read() de la server.\n");
        }

        able_to_send = 1;

        news_msg[strlen(news_msg)] = '\0';

        if(strcmp(news_msg,"NO NEWS TO SEND\n") != 0)
        {
            char stiri1[MSG_LENGTH];
            char stiri2[MSG_LENGTH];
            char stiri3[MSG_LENGTH];

            int j = 0;
            int number_of_endl = 0;

            bzero(stiri1,MSG_LENGTH);
            bzero(stiri2,MSG_LENGTH);
            bzero(stiri3,MSG_LENGTH);

            for(int i = 0 ; i < strlen(news_msg) ; i++)
            {
                if(news_msg[i] == '\n')
                {
                    number_of_endl++;
                    if(number_of_endl == 1)
                    {
                        stiri1[j] = '\0';
                    }
                    else if(number_of_endl == 2)
                    {
                        stiri2[j] = '\0';
                    }
                    else if(number_of_endl == 3)
                    {
                        stiri3[j] = '\0';
                    }
                    j = 0;
                }
                else if(news_msg[i] != '\n')
                {
                    if(number_of_endl == 0)
                        stiri1[j] = news_msg[i];
                    else if(number_of_endl == 1)
                        stiri2[j] = news_msg[i];
                    else if(number_of_endl == 2)
                        stiri3[j] = news_msg[i];
                    
                    j++;
                }
            }

            const gchar *current_label;
            gchar *new_label;

            switch(stiri1[0])
            {
                case  '1':
                    current_label = gtk_button_get_label(GTK_BUTTON(bottomLabel1));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(bottomLabel1), stiri1 + 1);
                    g_free(new_label);
                    g_print("[client]Se afiseaza vremea\n");
                break;
                case  '2':

                    modify_sport(stiri1);

                    current_label = gtk_button_get_label(GTK_BUTTON(rightLabel2));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(rightLabel2), stiri1);
                    g_free(new_label);
                    g_print("[client]Se afiseaza sport\n");
                break;
                case  '3':

                    modify_pret(stiri1);

                    current_label = gtk_button_get_label(GTK_BUTTON(bottomLabel2));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(bottomLabel2), stiri1);
                    g_free(new_label);
                    g_print("[client]Se afiseaza preturi\n");
                break;
            }

            switch(stiri2[0])
            {
                case  '1':
                    current_label = gtk_button_get_label(GTK_BUTTON(bottomLabel1));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(bottomLabel1), stiri2 + 1);
                    g_free(new_label);
                    g_print("[client]Se afiseaza vremea\n");
                break;
                case  '2':

                    modify_sport(stiri2);

                    current_label = gtk_button_get_label(GTK_BUTTON(rightLabel2));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(rightLabel2), stiri2);
                    g_free(new_label);
                    g_print("[client]Se afiseaza sport\n");
                break;
                case  '3':

                    modify_pret(stiri2);

                    current_label = gtk_button_get_label(GTK_BUTTON(bottomLabel2));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(bottomLabel2), stiri2);
                    g_free(new_label);
                    g_print("[client]Se afiseaza preturi\n");
                break;
            }

            switch(stiri3[0])
            {
                case  '1':
                    current_label = gtk_button_get_label(GTK_BUTTON(bottomLabel1));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(bottomLabel1), stiri3 + 1);
                    g_free(new_label);
                    g_print("[client]Se afiseaza vremea\n");
                break;
                case  '2':

                    modify_sport(stiri3);

                    current_label = gtk_button_get_label(GTK_BUTTON(rightLabel2));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(rightLabel2), stiri3);
                    g_free(new_label);
                    g_print("[client]Se afiseaza sport\n");
                break;
                case  '3':

                    modify_pret(stiri3);

                    current_label = gtk_button_get_label(GTK_BUTTON(bottomLabel2));

                    new_label = g_strdup_printf("New Label: %s", current_label);
                    gtk_button_set_label(GTK_BUTTON(bottomLabel2), stiri3);
                    g_free(new_label);
                    g_print("[client]Se afiseaza preturi\n");
                break;
            }

        }

        if(bytes == 0)
        {
            printf ("Serverul a fost inchis\n");
            gtk_main_quit();
        }

    }

    int bytesread;
    char buffer[MSG_LENGTH];

    bzero(buffer,MSG_LENGTH);
    
    if(able_to_send == 1)
    {
        bytesread = recv(sd,buffer,sizeof(buffer),MSG_DONTWAIT);

        buffer[strlen(buffer)] = '\0';

        if(buffer[0] == ':')
            if(bytesread > 0)
            {
                g_print("[client]Accident:%s\n",buffer);
                gtk_label_set_text(GTK_LABEL(rightLabel1), buffer + 1);
            }
    }

    return TRUE;

}

int main(int argc, char *argv[]) {

    srand((unsigned)time(NULL));
    
    struct sockaddr_in server;	
    char msg[MSG_LENGTH],speed_msg[MSG_LENGTH],part_msg[MSG_LENGTH];

    if (argc != 3)
    {
        printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    port = atoi (argv[2]);

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[client] Eroare la socket().\n");
        return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons (port);
    
    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
        perror ("[client]Eroare la connect().\n");
        return errno;
    }

    int bytes;
    bzero (msg, MSG_LENGTH);

    initialize();
    str_index = rand() % nb_of_str + 1;
    last_street = str_index;

    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Monitorizare Trafic");
    gtk_window_set_default_size(GTK_WINDOW(window), 770, 420);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid container
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create labels and button for the bottom part
    bottomLabel1 = gtk_button_new_with_label("Arata Vreme");              //VREME
    g_signal_connect(bottomLabel1, "clicked", G_CALLBACK(button_clicked_temp), NULL);


    GtkWidget *bottomButton = gtk_button_new_with_label("      Accident!     ");     //ACCIDENT
    g_signal_connect(bottomButton, "clicked", G_CALLBACK(button_clicked_accident), NULL);

    bottomLabel2 = gtk_button_new_with_label("Arata Pret Combustibili");                  //PRET
    g_signal_connect(bottomLabel2, "clicked", G_CALLBACK(button_clicked_pret), NULL);

    rightLabel2 = gtk_button_new_with_label("Arata Stiri Sportive");            //SPORT
    g_signal_connect(rightLabel2, "clicked", G_CALLBACK(button_clicked_sport), NULL);

    GtkWidget *up = gtk_button_new_with_label("⏫");
    g_signal_connect(up, "clicked", G_CALLBACK(button_clicked_up), NULL);

    GtkWidget *down = gtk_button_new_with_label("⏬");
    g_signal_connect(down, "clicked", G_CALLBACK(button_clicked_down), NULL);

    
    rightLabel1 = gtk_label_new("Accidente");

    char path_to_picture[100];
    sprintf(path_to_picture,"poze/%d.png",str_index);

    IMAGE  = gtk_image_new_from_file(path_to_picture);
    // GtkWidget *IMAGE = gtk_label_new("Image");
    GtkWidget *maxspeed = gtk_label_new("");
    speed = gtk_label_new("60 km/h");

    char decoy[MSG_LENGTH];
    bzero(decoy,MSG_LENGTH);

    strcat(decoy,"☀️");
    strcat(decoy,Data[str_index].strada);

    street = gtk_label_new(decoy);         //Strada    

    // Wrap labels and button inside frames
    GtkWidget *frame1 = gtk_frame_new(NULL);
    GtkWidget *frame2 = gtk_frame_new(NULL);
    GtkWidget *frame3 = gtk_frame_new(NULL);
    GtkWidget *frame4 = gtk_frame_new(NULL);
    GtkWidget *frame5 = gtk_frame_new(NULL);
    GtkWidget *frame6 = gtk_frame_new(NULL);

    GtkWidget *frame7 = gtk_frame_new(NULL);
    GtkWidget *frame8 = gtk_frame_new(NULL);
    GtkWidget *frame9 = gtk_frame_new(NULL);

    GtkWidget *frame10 = gtk_frame_new(NULL);
    GtkWidget *frame11 = gtk_frame_new(NULL);


    gtk_container_add(GTK_CONTAINER(frame1), bottomLabel1);
    gtk_container_add(GTK_CONTAINER(frame2), bottomButton);
    gtk_container_add(GTK_CONTAINER(frame3), bottomLabel2);
    gtk_container_add(GTK_CONTAINER(frame4), IMAGE);
    gtk_container_add(GTK_CONTAINER(frame5), rightLabel1);
    gtk_container_add(GTK_CONTAINER(frame6), rightLabel2);
    gtk_container_add(GTK_CONTAINER(frame7), speed);
    gtk_container_add(GTK_CONTAINER(frame8), maxspeed);
    gtk_container_add(GTK_CONTAINER(frame9), street);

    gtk_container_add(GTK_CONTAINER(frame10), up);
    gtk_container_add(GTK_CONTAINER(frame11), down);

    // Pack frames into the grid
    gtk_grid_attach(GTK_GRID(grid), frame1, 0, 12, 4, 1);    //Vreme                 
    gtk_grid_attach(GTK_GRID(grid), frame2, 4, 12, 3, 1);    //Accident!               
    gtk_grid_attach(GTK_GRID(grid), frame3, 7, 12, 4, 1);    //Preturi                  
    gtk_grid_attach(GTK_GRID(grid), frame4, 0, 0, 8, 12);   //Image       
    gtk_grid_attach(GTK_GRID(grid), frame5, 8, 0, 3, 6);    //Accidente   
    gtk_grid_attach(GTK_GRID(grid), frame6, 8, 6, 3, 6);    //Sport              
    gtk_grid_attach(GTK_GRID(grid), frame7, 6, 0, 1, 1);    //speed       
    gtk_grid_attach(GTK_GRID(grid), frame8, 7, 0, 1, 1);    //maxspeed                          
    gtk_grid_attach(GTK_GRID(grid), frame9, 0, 0, 2, 1);    //street      
    gtk_grid_attach(GTK_GRID(grid), frame10, 6, 1, 1, 1);   //up          
    gtk_grid_attach(GTK_GRID(grid), frame11, 7, 1, 1, 1);   //down          

    //Setting the sizes
    
    gtk_widget_set_size_request(IMAGE, 560, 360);        //  IMAGE
    gtk_widget_set_hexpand(IMAGE, FALSE);
    gtk_widget_set_vexpand(IMAGE, FALSE);

    gtk_widget_set_size_request(rightLabel1, 210, 180);        //  Accidente
    gtk_widget_set_hexpand(rightLabel1, FALSE);
    gtk_widget_set_vexpand(rightLabel1, FALSE);

    gtk_widget_set_size_request(rightLabel2, 210, 180);        //  Sport
    gtk_widget_set_hexpand(rightLabel2, FALSE);
    gtk_widget_set_vexpand(rightLabel2, FALSE);

    gtk_widget_set_size_request(bottomLabel2, 280, 60);        //  PRETURI
    gtk_widget_set_hexpand(bottomLabel2, FALSE);
    gtk_widget_set_vexpand(bottomLabel2, FALSE);

    gtk_widget_set_size_request(bottomLabel1, 280, 60);        //   VREME
    gtk_widget_set_hexpand(bottomLabel2, FALSE);
    gtk_widget_set_vexpand(bottomLabel2, FALSE);

    gtk_widget_set_size_request(bottomButton, 210, 60);        //   ACCIDENT
    gtk_widget_set_hexpand(bottomLabel2, FALSE);
    gtk_widget_set_vexpand(bottomLabel2, FALSE);

    //////////////

    gtk_widget_set_size_request(street, 70, 15);        //  Strada
    gtk_widget_set_hexpand(street, FALSE);
    gtk_widget_set_vexpand(street, FALSE);

    gtk_widget_set_size_request(maxspeed, 70, 15);        //  MaxSpeed
    gtk_widget_set_hexpand(maxspeed, FALSE);
    gtk_widget_set_vexpand(maxspeed, FALSE);

    gtk_widget_set_size_request(down, 70, 15);        //  Down
    gtk_widget_set_hexpand(down, FALSE);
    gtk_widget_set_vexpand(down, FALSE);

    gtk_widget_set_size_request(up, 35, 15);        //   Up
    gtk_widget_set_hexpand(up, FALSE);
    gtk_widget_set_vexpand(up, FALSE);

    gtk_widget_set_size_request(speed, 70, 15);        //    Speed
    gtk_widget_set_hexpand(speed, FALSE);
    gtk_widget_set_vexpand(speed, FALSE);

    //////////

    GtkStyleContext *context = gtk_widget_get_style_context(bottomButton);
    GtkStyleContext *context1 = gtk_widget_get_style_context(bottomLabel1);
    GtkStyleContext *context2 = gtk_widget_get_style_context(bottomLabel2);
    GtkStyleContext *context3 = gtk_widget_get_style_context(rightLabel2);

    GtkStyleContext *context4 = gtk_widget_get_style_context(up);
    GtkStyleContext *context5 = gtk_widget_get_style_context(down);

    GtkStyleContext *context7 = gtk_widget_get_style_context(maxspeed);
    GtkStyleContext *context8 = gtk_widget_get_style_context(street);
    GtkStyleContext *context9 = gtk_widget_get_style_context(rightLabel1);

    gtk_style_context_add_class(context, "red-button");
    gtk_style_context_add_class(context1, "vreme-button");
    gtk_style_context_add_class(context2, "preturi-button");
    gtk_style_context_add_class(context3, "sport-button");
    gtk_style_context_add_class(context4, "up-button");
    gtk_style_context_add_class(context5, "down-button");
    gtk_style_context_add_class(context7, "maxspeed-button");
    gtk_style_context_add_class(context8, "street-button");
    gtk_style_context_add_class(context9, "accident-button");

    const char *css = "button.red-button:hover { font-weight: bold;background-color: red; font-size: 25px; } button.red-button { background-color: orange; font-size: 20px; } button.red-button:active { background-color: black; color:white; font-size: 22px; }";
    const char *css1 = "button.vreme-button { background-color: #A8A8A8; font-family: 'DejaVu Sans'; font-size: 20px; } button.vreme-button:hover { background-color: #808080; font-size: 22px; } button.vreme-button:active {border: 2px solid black; }";
    const char *css2 = "button.preturi-button { background-color: #A8A8A8; font-family: 'DejaVu Sans';} button.preturi-button:hover { background-color: #808080;} button.preturi-button:active {border: 2px solid black; }";
    const char *css3 = "button.sport-button { background-color: #A8A8A8; font-family: 'DejaVu Sans';} button.sport-button:hover { background-color: #808080;} button.sport-button:active {border: 2px solid black; }";
    const char *css4 = "button.up-button {font-size: 30px; background-color: orange; font-family: 'DejaVu Sans';} button.up-button:hover { background-color: #FF8C00;} button.up-button:active {font-size: 27px; }";
    const char *css5 = "button.down-button {font-size: 30px; background-color: orange; font-family: 'DejaVu Sans';} button.down-button:hover { background-color: #FF8C00;} button.down-button:active {font-size: 27px;  }";
    const char *css7 = "label.maxspeed-button {font-weight :bold;font-size: 15px; background-color: black;color:red; font-family: 'DejaVu Sans';}";
    const char *css8 = "label.street-button {font-size: 20px; background-color: #A8A8A8; font-family: 'DejaVu Sans';}";
    const char *css9 = "label.accident-button { background-color: #A8A8A8; font-family: 'DejaVu Sans';}";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider1 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider1, css1, -1, NULL);
    gtk_style_context_add_provider(context1, GTK_STYLE_PROVIDER(provider1), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    GtkCssProvider *provider2 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider2, css2, -1, NULL);
    gtk_style_context_add_provider(context2, GTK_STYLE_PROVIDER(provider2), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider3 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider3, css3, -1, NULL);
    gtk_style_context_add_provider(context3, GTK_STYLE_PROVIDER(provider3), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider4 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider4, css4, -1, NULL);
    gtk_style_context_add_provider(context4, GTK_STYLE_PROVIDER(provider4), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider5 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider5, css5, -1, NULL);
    gtk_style_context_add_provider(context5, GTK_STYLE_PROVIDER(provider5), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider7 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider7, css7, -1, NULL);
    gtk_style_context_add_provider(context7, GTK_STYLE_PROVIDER(provider7), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider8 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider8, css8, -1, NULL);
    gtk_style_context_add_provider(context8, GTK_STYLE_PROVIDER(provider8), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkCssProvider *provider9 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider9, css9, -1, NULL);
    gtk_style_context_add_provider(context9, GTK_STYLE_PROVIDER(provider9), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Show all widgets
    gtk_widget_show_all(window);

    update_max_speed(maxspeed);
    // g_timeout_add_seconds(10,(GSourceFunc)update_max_speed,maxspeed);
    // g_timeout_add_seconds(5,(GSourceFunc)update_street,maxspeed);
    // g_timeout_add_seconds(1,(GSourceFunc)accident,rightLabel1);

    g_timeout_add_seconds(1,(GSourceFunc)periodic,maxspeed);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
