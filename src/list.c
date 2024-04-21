#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "../headers/types.h"

List *create_list()
{
    List *new_list = (List *)malloc(sizeof(List));
    new_list->size = 0;
    new_list->head = NULL;
    return new_list;
}

void add_to_list(List *list, FileState *data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

List *remove_from_list(List *list, const char *filename)
{
    if (list->size == 0)
    {
        return NULL;
    }
    Node *p = list->head;
    Node *q = list->head->next;
    while (strcmp(q->data->filename, filename) != 0 && q->next != NULL)
    {
        p = p->next;
        q = q->next;
    }

    if (strcmp(q->data->filename, filename) == 0)
    {
        p->next = q->next;
        free(q);
    }
    return list;
}

Node *find_from_list(List *list, const char *filename)
{
    if (list->size == 0)
    {
        return NULL;
    }
    Node *p = list->head;
    Node *q = list->head->next;
    while (strcmp(q->data->filename, filename) != 0 && q->next != NULL)
    {
        p = p->next;
        q = q->next;
    }

    if (strcmp(q->data->filename, filename) == 0)
    {
        return q;
    }
    return NULL;
}

void free_list(List *list)
{
    Node *current_node = list->head;
    while (current_node != NULL)
    {
        Node *next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(list);
}

void print_list(List *list)
{
    Node *current_node = list->head;
    while (current_node != NULL)
    {
        syslog(LOG_INFO, "%s\n", current_node->data->filename);
        syslog(LOG_INFO, "%s\n", current_node->data->hash);
        current_node = current_node->next;
    }
}