/*
I write the code for 5 processes and 5 resources so I don't use any variable
in order to define number of processes or resources.
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int main()
{
    int resources[5];
    FILE *fptr = fopen("resources.txt", "r");
    if (fptr == NULL)
    {
        printf("error opening file");
        return 0;
    }
    fscanf(fptr, "%d %d %d %d %d", &resources[0], &resources[1], &resources[2], &resources[3], &resources[4]);

    fclose(fptr);

    int requests[5][5];
    FILE *fpt = fopen("requests.txt", "r");
    if (fptr == NULL)
    {
        printf("error opening file");
        return 0;
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            fscanf(fpt, " %d ", &requests[i][j]);
        }
    }

    fclose(fpt);

    int allocations[5][5];
    FILE *fptr_alloc = fopen("allocations.txt", "r");
    if (fptr_alloc == NULL)
    {
        printf("error opening file");
        return 0;
    }
    for (int i = 0; i < 5; i++)
    {

        for (int j = 0; j < 5; j++)
        {
            fscanf(fptr_alloc, " %d ", &allocations[i][j]);
        }
    }
    fclose(fptr_alloc);

    int available[5];
    for (int i = 0; i < 5; i++)
    {
        int allocated = 0;
        for (int j = 0; j < 5; j++)
        {
            allocated = allocations[j][i] + allocated;
        }
        available[i] = resources[i] - allocated;
    }

    int finished[5];
    int process[5];
    int index = 0;
    for (int i = 0; i < 5; i++) // initialize finished and process order array as 0
    {
        finished[i] = 0; // 0 means it has not worked
        process[i] = 0;  // 0 means it has not worked and no process
    }
    for (int c = 0; c < 5; c++) // 5 times will be enough for the worst time scenario bec there are 5 processes
    {

        for (int n = 0; n < 5; n++) // n for the process type
        {
            int flag = 0;
            for (int i = 0; i < 5; i++) // i for the resource type
            {
                int x = available[i] - requests[n][i];
                if (x >= 0 && finished[n] == 0) // when process hasn't worked yet and i type of source is available.
                {
                    flag++;
                }
                else
                {
                    break; // It means there are not enough available source or process has already finished.
                }
            }

            if (flag == 5) // when all type of sources which are requested by the process is available.
            {
                finished[n] = 1;          // marking the resource as worked
                process[index++] = n + 1; // for the order
                for (int j = 0; j < 5; j++)
                {
                    available[j] = allocations[n][j] + available[j];
                }
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        printf("Information for process: P%d\n", i + 1);
        printf("Allocated Resources R1:%d R2:%d R3:%d R4:%d R5:%d\n", allocations[i][0], allocations[i][1], allocations[i][2],
               allocations[i][3], allocations[i][4]);
        printf("Resource Request R1:%d R2:%d R3:%d R4:%d R5:%d\n", requests[i][0], requests[i][1], requests[i][2], requests[i][3],
               requests[i][4]);
    }

    printf("Running order for process:");
    for (int i = 0; i < 5; i++)
    {
        if (process[i] == 0) // when there is deadlock
        {
            printf("\nThere is a deadlock. ");
            if (i == 4) // for single process
            {
                printf("Following process is the cause of deadlock: ");
            }
            else // multiple processes
            {
                printf("Following processes are the cause of deadlock: ");
            }

            break;
        }
        else
        {
            printf("P%d ", process[i]);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (finished[i] == 0) // processes that cause deadlock
        {
            printf("P%d ", i + 1);
        }
    }
    return 0;
}
