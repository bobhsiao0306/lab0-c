#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) /*malloc return NULL*/
    {
        printf("Memory allocation failed\n");
        exit(-1);
    } else /*malloc succeed*/
    {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    list_ele_t *p = q->head;
    while (p) {
        list_ele_t *tmp = p;
        p = p->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (q == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    } else {
        strncpy(newh->value, s, strlen(s) + 1);
        newh->next = q->head;
        q->head = newh;
        q->size++;
        if (q->size == 1)
            q->tail = q->head;
        return true;
    }
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (q == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    } else {
        strncpy(newh->value, s, strlen(s) + 1);
        newh->next = NULL;
        q->tail->next = newh;
        q->tail = newh;
        q->size++;
        if (q->size == 1)
            q->head = q->tail;
        return true;
    }
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL)
        return false;
    if (!strncmp(q->head->value, sp, strlen(sp))) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        strncpy(sp, tmp->value, bufsize - 1);
        free(tmp->value);
        free(tmp);
        q->size--;
        return true;
    } else
        return false;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL)
        return;
    q->tail = q->head;
    list_ele_t *x = q->head;
    list_ele_t *y = NULL;
    list_ele_t *z = NULL;
    while (x) {
        z = y;
        y = x;
        x = x->next;
        y->next = z;
    }
    q->head = y;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->size == 0 || q->size == 1)
        return;

    mergesort(&q->head);

    list_ele_t *buf = q->head;
    while (buf) {
        q->tail = buf;
        buf = buf->next;
    }
}

void mergesort(list_ele_t **head)
{
    // base case — length 0 or 1
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    list_ele_t *a;
    list_ele_t *b;

    // split `head` into `a` and `b` sublists
    frontBackSplit(*head, &a, &b);

    // recursively sort the sublists
    mergesort(&a);
    mergesort(&b);

    // answer = merge the two sorted lists
    *head = sortedMerge(a, b);
}

void frontBackSplit(list_ele_t *source,
                    list_ele_t **frontRef,
                    list_ele_t **backRef)
{
    // if the length is less than 2, handle it separately
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    list_ele_t *slow = source;
    list_ele_t *fast = source->next;

    // advance `fast` two nodes, and advance `slow` one node
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // `slow` is before the midpoint in the list, so split it in two
    // at that point.
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

list_ele_t *sortedMerge(list_ele_t *a, list_ele_t *b)
{
    // base cases
    if (a == NULL)
        return b;

    else if (b == NULL)
        return a;

    list_ele_t *result = NULL;

    // pick either `a` or `b`, and recur
    if (strcmp(a->value, b->value) <= 0) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }

    return result;
}