#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct StringStore {
    char* key;
    char* value;
    // makes struct a linked list
    struct StringStore* next;
} StringStore;

StringStore* stringstore_init(void) {
    // create new element with empty key and value and no next element
    StringStore* ss = malloc(sizeof(StringStore));
    strncpy(ss->key, "\0", 1);
    strncpy(ss->value, "\0", 1);
    ss->next = NULL;
    return ss;
}

StringStore* stringstore_free(StringStore* store) {
    StringStore* old = NULL;
    while (store) {
        free(store->key);
        free(store->value);
        old = store;
        store = store->next;
        free(old);
    }
    return NULL;
}

int stringstore_add(StringStore* store, const char* key, const char* value) {
    char* newKey = strdup(key);
    char* newValue = strdup(value);

    if (!newKey || !newValue) { // if either strdup call failed
        return 0;
    }
    
    // create new element
    StringStore* new = stringstore_init();
    new->key = newKey;
    new->value = newValue;

    // skip existing to last element
    while (store->next) {
        store = store->next;
    }

    // make new element the "next" member of last currently existing element
    store->next = new;

    return 1;
}

const char* stringstore_retrieve(StringStore* store, const char* key) {
    // search for key
    while (store && strcmp(store->key, key)) {
        store = store->next;
    }

    // if key exists, return the corresponding value
    // if we checked every element and key doesn't exist, return NULL
    return store ? store->value : NULL;
}

int stringstore_delete(StringStore* store, const char* key) {
    // search for key
    StringStore* prev = NULL;
    while (store && strcmp(store->key, key)) {
        prev = store;
        store = store->next;
    }
    
    // if we checked every element and key doesn't exist, return NULL
    if (!store) {
        return 0;
    }

    // skip over element to delete in list
    prev->next = store->next;
    // don't let element to delete feed back into list
    store->next = NULL;
    // now the element to delete (store) forms its own chain of
    //      element->next->next->next
    // which only includes itself
    return (stringstore_free(store) == NULL);
}

