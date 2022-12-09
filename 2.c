#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
 
double fix_x(double x)
{
    while (x > 2*M_PI)
        x = x - 2*M_PI;
    while (x < 0)
        x = x + 2*M_PI;
    return x;
}
 
double fact(double n) 
{
    return !n ? 1 : n * fact(n - 1);
}
 
void summ(int N, char* strName)
{    
        char z,ch[20],cc[20];
    	char* pEnd;
        double mass[N];
        int i=0;
 
        FILE *file;
        char *fname = "./temp.txt";
        if ((file = fopen(fname,"r")) == NULL)
	{
		fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
		return; 
	}

        while (!feof(file))
	{      
		fscanf(file,"%c",&z);
		if (z == ' ')
		{
			i = 0;
			while (1)
			{
				fscanf(file, "%c", &z);
				if ((z==' ') )
					break;
				else
				{
					ch[i]=z;
					i++;
				}

    			}
			ch[i] = '\0';
			i = 0;
			while (1)
			{
				fscanf(file, "%c", &z);
				if (z=='\n')
				 break;
				else
				{
					cc[i] = z;
					i++;
				}
			}
			cc[i] = '\0';
			mass[atoi(ch)] += strtod(cc,&pEnd);
		}

	}

	if (fclose(file) == -1)
	{
		fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
		return; 
	}
 
 
        fname = "./result.txt";
        if ((file = fopen(fname,"w")) == NULL)
	{
		fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
		return; 
	}
 
        for (i=0; i<N; i++)
        {
              fprintf(file,"y[%d] = %0*.*f\n",i,8,7,mass[i]);
        }
        if (fclose(file) == -1)
	{
	fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
	return; 
	}
}
 
void element(int N, int n, int i, int j, char* strName)
{
    double x,el=0;
    int f = 0;
    FILE *file;
    char *fname = "./temp.txt";
    
    if ((file = fopen(fname,"a")) == NULL)
    {
        fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
        return; 
    }
 
    x = fix_x(2*M_PI*i/N);
    if (x > M_PI)
    {
        f = 1;
        x = x - M_PI; 
    }
 
    if (x > M_PI/2 )
    { 
        x = M_PI - x;
    }
 
    el = (pow((-1),j) * pow(x,(2*j+1)))/(fact(2*j+1) ) ;
 
    if (f == 1)
    {
        el = (-1)*el;
    }
    if ((fprintf(file,"%d: %d %0*.*f\n",getpid(),i,8,7,el)) == -1)
    {
        fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
        return;
    }
 
    printf("%d: %d %0*.*f\n",getpid(),i,8,7,el);
    
    if (fclose(file) == -1)
    {
        fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
        return; 
    }
}
 
 
int main(int argc, char *argv[])
{
    char strName[strlen(argv[0])];
  
        int k=0;
        int p=0;
 
    for (p=2; p<strlen(argv[0]); p++)
    {
        strName[k] = argv[0][p];
        k++;
    }
    strName[k] = '\0';
    
        if (argc !=3 )
        {
        fprintf(stderr,"%s: %s\n", strName, strerror(errno)); 
        return 1;
    }
 
    FILE *file;
        char *fname = "./temp.txt";
 
        if ((file = fopen(fname,"w")) == NULL)
	{
		fprintf(stderr,"%s: %s: %s\n", strName, strerror(errno), fname); 
		return 1;
	}
 
        //fclose (file);
 
    int N,n,i,j,count,tmp;
        pid_t pid;
 
    int status;
 
    N = atoi(argv[1]);
    n = atoi(argv[2]);
    count = n;
    tmp = 0;
 
    for (i=0; i<N; i++)
    {
        for (j=0; j<n; j++)
        {
           pid = fork();
           if (pid > 0)
           {
           	tmp++;
           	if (tmp > count )
           	{
                //wait();
                	tmp--;
            	}
           }
 
           if (pid == -1)
           {
                    fprintf(stderr,"%s: %s\n", strName, strerror(errno)); 
                        return 0;
           }
           else if (pid == 0)
           { 
           	element(N,n,i,j, strName);
           	exit(1);
           }  
         }
         
    }
 
    while (wait(&status) > 0);      
 
        summ(N, strName);
        
    return 0;
}
