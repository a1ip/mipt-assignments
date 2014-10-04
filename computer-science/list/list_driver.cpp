#include <stdio.h>
#include "list.hpp"

int main() 
{
    List* list = 0;
    int command;
    int N;

    printf("> 0   - Create a new list (the old one will be removed).\n");
    printf("> 1   - Delete the current list.\n");
    printf("> 2   - Print out the current list.\n");
    printf("> 3 N - Insert a number in the front of the list.\n");
    printf("> 4 N - Delete all numbers with the same value.\n");
    printf("> 5 N - Create a new \"First-integers\" list.\n");
    printf("> 6   - Exit.\n");

    while(true) 
    {
        printf("> Choose a command: ");
        scanf("%d", &command);
        switch(command) 
        {
            case 0:
                if(list) deleteList(list);
                list = createList();
                printf("> The list has been created.\n");
                break;
            case 1:
                if(list)
                {
                    deleteList(list);
                    printf("> The list has been deleted.\n");
                }
                list = 0;
                break;
            case 2:
                if(!list) 
                {
                    printf("> A list hasn't been created yet!\n");
                    break;
                }
                printf("> ");
                printList(list);
                break;
            case 3:
                if(!list) 
                {
                    printf("> A list hasn't been created yet!\n");
                    break;
                }
                scanf("%d", &N);
                addElementToHead(list, N);
                break;
            case 4:
                if(!list) 
                {
                    printf("> A list hasn't been created yet!\n");
                    break;
                }
                scanf("%d", &N);
                removeValue(list, N);
                break;
            case 5:
                scanf("%d", &N);
                if(list) deleteList(list);
                list = createFirstIntegers(N);
                break;
            case 6:
                if(list) deleteList(list);
                return 0;
        }
    }
    if(list) deleteList(list);
    return 0;
}
