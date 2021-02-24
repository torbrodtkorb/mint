// Circular linked list implementation

#ifndef LIST_H
#define LIST_H

#include <mint/types.h>

typedef struct {
    List* next;
    List* prev;
} List;

static inline void list_init(List* list) {
    list->next = list;
    list->prev = list;
}

static inline void list_insert_between(List* node, List* before,
    List* after) {

    node->next = after;
    node->prev = before;

    before->next = node;
    after->prev = node;
}

static inline void list_push_front(List* node, List* list) {
    list_insert_between(node, list, list->next);
}

static inline void list_push_back(List* node, List* list) {
    list_insert_between(node, list->prev, list);
}

static inline List* list_pop_front(List* list) {
    List* tmp = list->next;

    list->next = tmp->next;
    tmp->next->prev = list;    

    return tmp;
}

static inline List* list_pop_back(List* list) {
    List* tmp = list->prev;

    tmp->prev->next = list;
    list->prev = tmp->prev;

    return tmp;
}

#define list_iterate(node, list) \
    for ((node) = (list)->next; (node) != (list); (node) = (node)->next)

#define list_get_struct(node, struct_type, struct_element) \
    (struct_type *)((u8 *)(node) - offsetof(struct_type, struct_element))

#endif
