#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
int i,j,ec,fg,ec2;
char fn[20],e,c;
FILE *fp1,*fp2,*fp;
void Create();
void Edit();
void Delete();
void Display();
char getch();
void writetxt(char *string, int rows, int cols);       //Function to help me center text, vertically or horizontaly.
int main()
{
   do {
         system("clear");
         struct winsize win;                                   //struct winsize to get the window's width and height.
         ioctl(0, TIOCGWINSZ, &win);                           //TIOCGWINSZ, IOCtl to Get the Window SiZe.

         writetxt("TEXT EDITOR", 0, win.ws_col);
         printf("\n\n\tMENU :\n\t------\n ");
         printf("\n\t1.CREATE\n\t2.DISPLAY\n\t3.EDIT\n\t4.DELETE\n\t5.EXIT\n");
         printf("\n\tEnter your choice: ");
         scanf("%d",&ec);

         switch(ec)
         {
           case 1:
                  Create();
                  break;

           case 2:
                  Display();
                  break;

           case 3:
                  Edit();
                  break;

           case 4:
                  Delete();
                  break;

           case 5:system("clear");
                  writetxt("Goodbye.", win.ws_row, win.ws_col);
                  return 0;
         }
       }while(1);
}
void Create()
{
  system("clear");
  struct winsize win;                                   //struct winsize to get the window's width and height.
  ioctl(0, TIOCGWINSZ, &win);

  fp1=fopen("temp.txt","w");
  writetxt("Enter the text and press '~' to save",0,win.ws_col);

  while(1)
  {
     c=getchar();
     fputc(c,fp1);
     if(c == '~')
     {
        fclose(fp1);
        printf("\n\tEnter the filename: ");
        scanf("%s",fn);
        fp1=fopen("temp.txt","r");
        fp2=fopen(fn,"w");

        while(!feof(fp1))
        {
           c=getc(fp1);
           putc(c,fp2);
        }
        fclose(fp2);
        break;
      }
    }
}

void Display()
{
   char a,*disp;
   int i=0;
   struct winsize win;
   ioctl(0, TIOCGWINSZ, &win);
   printf("\n\tEnter the file name: ");
   scanf("%s",fn);
   fp1=fopen(fn,"r");
   disp=(char*)malloc(sizeof(fp1));
   if(fp1==NULL)
   {
      printf("\n\tFile not found!");
      goto end1;
   }

   while(!feof(fp1))
   {
      c=getc(fp1);
      disp[i]=c;
      //printf("%c",c);
      i++;
   }
   system("clear");
   writetxt("File Contents are: \n\n", 0, win.ws_col);
   printf("%lu",strlen(disp));
   writetxt(disp, 0, win.ws_col); //this doesn't work yet? why?
   end1:
   fclose(fp1);
   printf("\n\n\tPress any key to continue...");
   getch();
}

void Delete()
{
  printf("\n\tEnter the file name: ");
  scanf("%s",fn);
  fp1=fopen(fn,"r");
  if(fp1==NULL)
  {
   printf("\n\tFile not found!");
   goto end2;
  }
  fclose(fp1);
  if(remove(fn)==0)
  {
   printf("\n\n\tFile has been deleted successfully!");
   goto end2;
  }
  else
   printf("\n\tError!\n");
end2: printf("\n\n\tPress any key to continue...");
}
void Edit() //This is very badly coded, barely works and Goto function is ruining shit, note to recode.
{
   printf("\n\tEnter the file name: ");
   scanf("%s",fn);
   fp1=fopen(fn,"r");
   if(fp1==NULL)
   {
      printf("\n\tFile not found!");
      goto end3;
   }
   while(!feof(fp1))
   {
      c=getc(fp1);
      printf("%c",c);
   }
   fclose(fp1);
   printf("\n\tType the text and press 'Ctrl+S' to Edit.\n");
   fp1=fopen(fn,"a");
   while(1)
   {
      c=getchar();
      if(c==19)
      goto end3;
      if(c==13)
      {
         c='\n';
         printf("\n\t");
         fputc(c,fp1);
      }
      else
      {
         printf("%c",c);
         fputc(c,fp1);
      }
    }
   end3: fclose(fp1);
}

void writetxt(char *string, int rows, int cols)
{
    int vert = rows/2;
    int hort = 0;
    int stringLength = strlen(string) / 2;

    hort = (cols - strlen(string))/2;

    for (int x = 0; x <= rows; x++)
    {
        printf("\n");
        if (x == vert)
        {
            printf("\n%*s\n", cols / 2 + stringLength, string );
        }
    }
}

char getch()
{
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }
