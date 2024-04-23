#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "../headers/types.h"

List *create_list()
{
    List *new_list = (List *)malloc(sizeof(List));
    if (new_list == NULL)
    {
        perror("malloc");
        return NULL;
    }

    new_list->size = 0;
    new_list->head = NULL;
    return new_list;
}

void add_to_list(List *list, FileState *data)
{
    if (list == NULL)
    {
        syslog(LOG_ERR, "List is NULL");
        return;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        perror("malloc");
        return;
    }

    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

void remove_from_list(List *list, const char *filename)
{
    if (list == NULL || list->size == 0)
    {
        syslog(LOG_ERR, "List is NULL or empty");
        return;
    }

    Node *p = list->head;
    Node *prev = NULL;

    if (strcmp(p->data->filename, filename) == 0)
    {
        list->head = p->next;
        free(p->data);
        free(p);
        list->size--;
        return;
    }

    while (p != NULL && strcmp(p->data->filename, filename) != 0)
    {
        prev = p;
        p = p->next;
    }

    if (p != NULL)
    {
        prev->next = p->next;
        free(p->data);
        free(p);
        list->size--;
    }
}

FileState *find_from_list(List *list, const char *filename)
{
    if (list == NULL)
    {
        syslog(LOG_ERR, "List is NULL");
        return NULL;
    }

    Node *p = list->head;

    while (p != NULL)
    {
        if (strcmp(p->data->filename, filename) == 0)
        {
            return p->data;
        }
        p = p->next;
    }

    return NULL;
}

void free_list(List *list)
{
    if (list == NULL)
    {
        syslog(LOG_ERR, "List is NULL");
        return;
    }

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
    if (list == NULL)
    {
        syslog(LOG_ERR, "List is NULL");
        return;
    }

    Node *current_node = list->head;
    while (current_node != NULL)
    {
        if (current_node->data == NULL)
        {
            syslog(LOG_ERR, "Data in list node is NULL");
            return;
        }

        syslog(LOG_INFO, "%s\n", current_node->data->filename);
        current_node = current_node->next;
    }
}
