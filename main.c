#include <stdio.h>


#define MAX 126

// function prototypes
void storeTask(char *buffer);
void viewTasks(char *buffer);
void removeTask(int taskNum, char *buffer);
int countLines(int taskNum);
void clearBuffer(void);

// declare it here, so it can be accessed in any function
FILE *file;
char filename[] = "tasks.txt";


int main(void)
{
    char task[MAX];
    int choice = 0;

    while (1)
    {
        printf("\n1. Add Task\n2. View Tasks\n3. Remove Task\n4. End Program\nEnter Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                clearBuffer(); // clear the '\n' from the choice input
                storeTask(task);
                break;
            case 2:
                viewTasks(task);
                break;
            case 3:
                clearBuffer();
                int line;
                printf("Enter the line number of task you want to delete: ");
                scanf("%d", &line);

                removeTask(line, task);
                break;
            case 4:
                printf("\nEnding program.\n");
                return 0; // end program
            default:
                printf("\nNot a valid option!\n");
                break;
        }
    }

    // just in case there is some exception that bypasses the other close/free statements
    fclose(file);
    
    return 0; 
}


void storeTask(char *buffer)
{
    file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("\nError storing task.\n");
    }

    printf("Enter your task: ");
    fgets(buffer, MAX, stdin);

    // check if user actually typed an input
    if (buffer[0] == '\n')
    {
        printf("\nDidn't store a task. Nothing was entered.\n");
        return;
    }

    fprintf(file, "%s", buffer);

    fclose(file);
}


void viewTasks(char *buffer)
{
    // to read and print the files, open the file in read mode
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("\nError opening file.\n");
    }

    printf("\n"); // some spacing for formatting the tasks 
    int line = 0;

    // while a line from file can be stored into buffer...
    while (fgets(buffer, MAX, file) != NULL)
    {
        line++;
        printf("%d: %s", line, buffer);
    }

    fclose(file);
}


void removeTask(int taskNum, char *buffer) 
{   
    int numOfLines = countLines(taskNum);

    if (numOfLines == -1)
    {
        return;
    }

    // create a temp file, copy all tasks except the one that needs to be deleted
    file = fopen(filename, "r");
    char tempFileName[] = "temp.txt";
    FILE *tempFile = fopen(tempFileName, "w");
    if (tempFile == NULL)
    {
        printf("\nError deleting task.\n");
    }

    int lineNum = 1; 

    // while a line can be read
    while(fgets(buffer, MAX, file) != NULL)
    {
        // if the line isn't the requested task
        if (lineNum != taskNum)
        {
            fprintf(tempFile, "%s", buffer);
            lineNum++;
        }
        else 
        {
            // read the next line
            lineNum++;
        }
    }

    fclose(file);
    fclose(tempFile);

    // rename the temp file, and delete the tasks file
    int status = remove(filename);
    
    if (status != 0)
    {
        printf("\nError deleting the file.\n");
    }

    status = rename(tempFileName, filename);

    if (status != 0)
    {
        printf("\nError renaming the file.\n");
    }
}


int countLines(int taskNum)
{
    int nextChar = 0;
    int numOfLines = 0;
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("\nError loading file.\n");
    }

    // while the character isn't the last, if the char is a newline, it must mean, it is the end of 1 line
    for (nextChar = getc(file); nextChar != EOF; nextChar = getc(file))
    {
        if (nextChar == '\n')
        {
            numOfLines++;
        }
    }

    fclose(file);

    // check if taskNum is valid
    if ((taskNum - 1) > numOfLines)
    {
        printf("\nThe entered task number is not valid (%d:%d).\n", taskNum, numOfLines);
        return -1;
    }
    else
    {
        return numOfLines; 
    }
}


void clearBuffer(void)
{
    int nextChar;

    // while the next char isn't an enter key, and the input buffer isn't empty
    while (((nextChar = getchar()) != '\n') && (nextChar != EOF))
    {
        // do nothing, we just want to store the the char in the var so we can get rid of it
    }
    // when this line is reached, all the chars in the input buffer have been cleared
}