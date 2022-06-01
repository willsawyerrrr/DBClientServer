#ifndef _STRINGSTORE_H
#define _STRINGSTORE_H

// Opaque type for StringStore - you'll need to define 'struct StringStore' 
// in your stringstore.c file
typedef struct StringStore StringStore;

// Create a new StringStore instance, and return a pointer to it
StringStore *stringstore_init(void);

// Delete all memory associated with the given StringStore, and return NULL
StringStore *stringstore_free(StringStore *store);

// Add the given 'key'/'value' pair to the StringStore 'store'.  
// The 'key' and 'value' strings are copied with strdup() before being 
// added to the database. Returns 1 on success, 0 on failure (e.g. if
// strdup() fails).
int stringstore_add(StringStore *store, const char *key, const char *value);

// Attempt to retrieve the value associated with a particular 'key' in the 
// StringStore 'store'.  
// If the key exists in the database, return a const pointer to corresponding 
// value string.
// If the key does not exist, return NULL
const char *stringstore_retrieve(StringStore *store, const char *key);

// Attempt to delete the key/value pair associated with a particular 'key' in 
// the StringStore 'store'.
// If the key exists and deletion succeeds, return 1.
// Otherwise, return 0
int stringstore_delete(StringStore *store, const char *key);
#endif
