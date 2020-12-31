// NAME: Joseph Picchi
// EMAIL: jpicchi22@g.ucla.edu

#include "SortedList.h"
#include <string.h>
#include <pthread.h>
#include <sched.h>


// NOTE: empty list = head points to itself

/**
 * SortedList_insert ... insert an element into a sorted list
 *
 *	The specified element will be inserted in to
 *	the specified list, which will be kept sorted
 *	in ascending order based on associated keys
 *
 * param SortedList_t *list ... header for the list
 * param SortedListElement_t *element ... element to be added to the list
 */
void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    // if either is NULL, we must return
    if (!list || !element) {
        return;
    }

    SortedListElement_t *last = list;
    SortedListElement_t *current = list->next;

    // increment until you get to the end of the list
    while (current != list) {
        // if next element is larger, break out
        if (strcmp(element->key, current->key) < 0) {
            break;
        }
        
        // otherwise, increment
        last = current;
        current = current->next;
    }

    // critical section --> right before you insert into list
    if (opt_yield & INSERT_YIELD) {
        sched_yield();
    }

    // now add in the element btw last and current
    element->next = current;
    element->prev = last;
    current->prev = element;
    last->next = element;
}

/**
 * SortedList_delete ... remove an element from a sorted list
 *
 *	The specified element will be removed from whatever
 *	list it is currently in.
 *
 *	Before doing the deletion, we check to make sure that
 *	next->prev and prev->next both point to this node
 *
 * param SortedListElement_t *element ... element to be removed
 *
 * return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 */
int SortedList_delete( SortedListElement_t *element) {
    // handle a NULL element
    if (!element || !element->key) {
        return 1;
    }
    
    // check to make sure that next->prev and prev->next both point
    // to this node
    if (element->next->prev != element || element->prev->next != element) {
        return 1;
    }

    // yield after we checked that we're in the right spot
    if (opt_yield & DELETE_YIELD) {
        sched_yield();
    }

    // delete the element
    element->prev->next = element->next;
    element->next->prev = element->prev;

    return 0;
}

/**
 * SortedList_lookup ... search sorted list for a key
 *
 *	The specified list will be searched for an
 *	element with the specified key.
 *
 * SortedList_t *list ... header for the list
 * const char * key ... the desired key
 *
 * return pointer to matching element, or NULL if none is found
 */
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
    // handle NULL list
    if (!list || list->key) {
        return NULL;
    }
    
    SortedListElement_t *current = list->next;
    while(current != list) {
        if (!strcmp(current->key, key)) {
            return current;
        }
        // yield after we checked the key
        if (opt_yield & LOOKUP_YIELD) {
            sched_yield();
        }
        current = current->next;
    }

    return NULL;
}

/**
 * SortedList_length ... count elements in a sorted list
 *	While enumeratign list, it checks all prev/next pointers
 *
 * param SortedList_t *list ... header for the list
 *
 * return int number of elements in list (excluding head)
 *	   -1 if the list is corrupted
 */
int SortedList_length(SortedList_t *list) {
    // handle NULL list
    if (!list || list->key) {
        return -1;
    }

    int element_count = 0;
    SortedList_t *current = list->next;
    SortedList_t *last = list;
    while(current != list) {
        if (current->prev != last || last->next != current) {
            return -1;
        }
        element_count++;
        if (opt_yield & LOOKUP_YIELD) {
            sched_yield();
        }
        last = current;
        current = current->next;
    }

    // one last check
    if (current->prev != last || last->next != current) {
        return -1;
    }

    return element_count;
}