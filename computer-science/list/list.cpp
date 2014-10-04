#include <stdio.h>
#include <stdlib.h>
#include "list.hpp"

struct Element
{
    int value;
    Element* next;
};

struct List
{
    Element* first;
};

List* createList()
{
    List* list = (List*)malloc(sizeof(list));
    list->first = 0;
    return list;
}

void deleteList(List* list)
{
    Element* currentElement = list->first;
    while(currentElement)
    {
        Element* temp = currentElement->next;
        free(currentElement);
        currentElement = temp;
    }
    free(list);
}

void addElementToHead(List* list, int value)
{
    Element* newElement = (Element*)malloc(sizeof(Element));
    newElement->next = list->first;
    newElement->value = value;
    list->first = newElement;
}

void removeValue(List* list, int value)
{
    Element* prevElement = 0;
    Element* currentElement = list->first;
    while(currentElement)
    {
        if(currentElement->value == value)
        {
            Element* nextElement = currentElement->next;
            if(prevElement == 0)
            {
                list->first = currentElement->next;
                free(currentElement);
            }
            else
            {
                prevElement->next = currentElement->next;
                free(currentElement);
            }
            currentElement = nextElement;
        }
        else
        {
            prevElement = currentElement;
            currentElement = currentElement->next;
        }
    }
}

void printList(List* list)
{
    Element* currentElement = list->first;
    while(currentElement)
    {
        printf("%d ", currentElement->value);
        currentElement = currentElement->next;
    }
    printf("\n");
}

List* createFirstIntegers(int N)
{
    List* list = createList();
    for(int i = 1; i <= N; i++)
        addElementToHead(list, i);
    return list;
}
