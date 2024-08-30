#include "minishell.h"

Node* createNode(char* data)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, char* data)
{
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void freeList(Node* head)
{
    Node* current = head;
    while (current != NULL) 
    {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}