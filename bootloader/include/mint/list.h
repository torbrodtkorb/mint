// Circular linked list implementation

#ifndef LIST_H
#define LIST_H

#include <mint/types.h>

struct list{
    struct list* next;
    struct list* prev;
};

static inline void list_init(struct list* list) {
    list->next = list;
    list->prev = list;
}

static inline void list_insert_between(struct list* node, struct list* before,
    struct list* after) {

    node->next = after;
    node->prev = before;

    before->next = node;
    after->prev = node;
}

static inline void list_push_front(struct list* node, struct list* list) {
    list_insert_between(node, list, list->next);
}

static inline void list_push_back(struct list* node, struct list* list) {
    list_insert_between(node, list->prev, list);
}

static inline struct list* list_pop_front(struct list* list) {
    struct list* tmp = list->next;

    list->next = tmp->next;
    tmp->next->prev = list;    

    return tmp;
}

static inline struct list* list_pop_back(struct list* list) {
    struct list* tmp = list->prev;

    tmp->prev->next = list;
    list->prev = tmp->prev;

    return tmp;
}

#define list_iterate(node, list) \
    for ((node) = (list)->next; (node) != (list); (node) = (node)->next)

#define list_get_struct(node, struct_type, struct_element) \
    (struct_type *)((u8 *)(node) - offsetof(struct_type, struct_element))

#endif
