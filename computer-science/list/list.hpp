#ifndef __LIST_HPP__
#define __LIST_HPP__

struct Element;
struct List;

List* createList();
void deleteList(List* list);
List* createFirstIntegers(int N);

void addElementToHead(List* list, int value);
void removeValue(List* list, int value);
void printList(List* list);

#endif
