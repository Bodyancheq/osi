#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
 
 
const int BUFFER_SIZE = 200;
 
void get_current_time(char* into_string)
{
    time_t now_time;
    time(&now_time);
    struct tm* now_tm = localtime(&now_time);
    struct timespec now_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now_timespec);
    long milliseconds = now_timespec.tv_nsec / 1000000;
 
    snprintf(into_string, BUFFER_SIZE, "%d:%d:%d:%ld", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, milliseconds);
}
 
void get_pid_info(pid_t pid)
{
    char time_string[BUFFER_SIZE];
    if (pid == 0)
    {
        get_current_time(time_string);
        printf("Дочерний процесс, pid=%d время: %s\n", getpid(), time_string);
        get_current_time(time_string);
        printf("pid Родительского процесса=%d время: %s\n", getppid(), time_string);
    }
    else if (pid > 0)
    {
        get_current_time(time_string);
        printf("Родительский процесс pid=%d время: %s\n", getpid(), time_string);
    }
    else
    {
        printf("Ошибка вызова fork, потомок не создан\n");
    }
}
 
int main()
{
    pid_t pid1 = fork();
    get_pid_info(pid1);
 
    if (pid1 > 0)
    {
        pid_t pid2 = fork();
        get_pid_info(pid2);
        if (pid2 > 0)
        {
            system("ps -x");
        }
    }
 
    return 0;
}
