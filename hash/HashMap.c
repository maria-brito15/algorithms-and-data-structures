#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct HashMap {
    Entry **buckets;
    int capacity;
    int size;
} HashMap;

// Create a new entry
Entry* createEntry(const char *key, int value) {
    Entry *entry = (Entry*)malloc(sizeof(Entry));
    entry->key = (char*)malloc(strlen(key) + 1);
    strcpy(entry->key, key);
    entry->value = value;
    entry->next = NULL;
    return entry;
}

// Hash function using djb2 algorithm
unsigned int hash(const char *key, int capacity) {
    unsigned long hashValue = 5381;
    int c;
    
    while ((c = *key++))
        hashValue = ((hashValue << 5) + hashValue) + c;
    
    return hashValue % capacity;
}

// Initialize hash map
HashMap* createHashMap() {
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    map->capacity = INITIAL_CAPACITY;
    map->size = 0;
    map->buckets = (Entry**)calloc(map->capacity, sizeof(Entry*));
    return map;
}

// Get load factor
double getLoadFactor(HashMap *map) {
    return (double)map->size / map->capacity;
}

// Resize and rehash the map
void resize(HashMap *map) {
    int oldCapacity = map->capacity;
    Entry **oldBuckets = map->buckets;
    
    map->capacity *= 2;
    map->buckets = (Entry**)calloc(map->capacity, sizeof(Entry*));
    map->size = 0;
    
    // Rehash all entries
    for (int i = 0; i < oldCapacity; i++) {
        Entry *entry = oldBuckets[i];
        while (entry != NULL) {
            Entry *next = entry->next;
            
            // Reinsert into new buckets
            unsigned int index = hash(entry->key, map->capacity);
            entry->next = map->buckets[index];
            map->buckets[index] = entry;
            map->size++;
            
            entry = next;
        }
    }
    
    free(oldBuckets);
}

// Put a key-value pair into the map
void put(HashMap *map, const char *key, int value) {
    // Check if resize is needed
    if (getLoadFactor(map) >= LOAD_FACTOR_THRESHOLD) {
        resize(map);
    }
    
    unsigned int index = hash(key, map->capacity);
    Entry *entry = map->buckets[index];
    
    // Check if key already exists
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    
    // Insert new entry at the beginning
    Entry *newEntry = createEntry(key, value);
    newEntry->next = map->buckets[index];
    map->buckets[index] = newEntry;
    map->size++;
}

// Get value for a key
int get(HashMap *map, const char *key, bool *found) {
    unsigned int index = hash(key, map->capacity);
    Entry *entry = map->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            *found = true;
            return entry->value;
        }
        entry = entry->next;
    }
    
    *found = false;
    return -1;
}

// Check if key exists
bool containsKey(HashMap *map, const char *key) {
    unsigned int index = hash(key, map->capacity);
    Entry *entry = map->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0)
            return true;
        entry = entry->next;
    }
    
    return false;
}

// Remove a key-value pair
bool removeKey(HashMap *map, const char *key) {
    unsigned int index = hash(key, map->capacity);
    Entry *entry = map->buckets[index];
    Entry *prev = NULL;
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (prev == NULL)
                map->buckets[index] = entry->next;
            else
                prev->next = entry->next;
            
            free(entry->key);
            free(entry);
            map->size--;
            return true;
        }
        prev = entry;
        entry = entry->next;
    }
    
    return false;
}

// Get all keys
char** getKeys(HashMap *map, int *numKeys) {
    char **keys = (char**)malloc(map->size * sizeof(char*));
    int index = 0;
    
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            keys[index] = (char*)malloc(strlen(entry->key) + 1);
            strcpy(keys[index], entry->key);
            index++;
            entry = entry->next;
        }
    }
    
    *numKeys = index;
    return keys;
}

// Get all values
int* getValues(HashMap *map, int *numValues) {
    int *values = (int*)malloc(map->size * sizeof(int));
    int index = 0;
    
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            values[index++] = entry->value;
            entry = entry->next;
        }
    }
    
    *numValues = index;
    return values;
}

// Clear all entries
void clear(HashMap *map) {
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            Entry *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
        map->buckets[i] = NULL;
    }
    map->size = 0;
}

// Check if map is empty
bool isEmpty(HashMap *map) {
    return map->size == 0;
}

// Get size of map
int getSize(HashMap *map) {
    return map->size;
}

// Print the hash map
void printHashMap(HashMap *map) {
    printf("HashMap (size: %d, capacity: %d, load factor: %.2f):\n", 
           map->size, map->capacity, getLoadFactor(map));
    
    for (int i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL) {
            printf("Bucket %d: ", i);
            Entry *entry = map->buckets[i];
            while (entry != NULL) {
                printf("[%s: %d] ", entry->key, entry->value);
                entry = entry->next;
            }
            printf("\n");
        }
    }
}

// Free the hash map
void freeHashMap(HashMap *map) {
    clear(map);
    free(map->buckets);
    free(map);
}

// Example usage
int main() {
    HashMap *map = createHashMap();
    
    // Insert key-value pairs
    printf("Inserting elements...\n");
    put(map, "apple", 100);
    put(map, "banana", 200);
    put(map, "orange", 300);
    put(map, "grape", 400);
    put(map, "mango", 500);
    
    printHashMap(map);
    
    // Get values
    printf("\nGetting values:\n");
    bool found;
    int value = get(map, "banana", &found);
    printf("banana: %s (value: %d)\n", found ? "found" : "not found", value);
    
    value = get(map, "cherry", &found);
    printf("cherry: %s\n", found ? "found" : "not found");
    
    // Check if key exists
    printf("\nContains key 'apple': %s\n", containsKey(map, "apple") ? "yes" : "no");
    printf("Contains key 'cherry': %s\n", containsKey(map, "cherry") ? "yes" : "no");
    
    // Update value
    printf("\nUpdating 'apple' to 150...\n");
    put(map, "apple", 150);
    value = get(map, "apple", &found);
    printf("apple: %d\n", value);
    
    // Get all keys
    printf("\nAll keys: ");
    int numKeys;
    char **keys = getKeys(map, &numKeys);
    for (int i = 0; i < numKeys; i++) {
        printf("%s ", keys[i]);
        free(keys[i]);
    }
    free(keys);
    printf("\n");
    
    // Get all values
    printf("All values: ");
    int numValues;
    int *values = getValues(map, &numValues);
    for (int i = 0; i < numValues; i++) {
        printf("%d ", values[i]);
    }
    free(values);
    printf("\n");
    
    // Remove a key
    printf("\nRemoving 'banana'...\n");
    removeKey(map, "banana");
    printf("Size after removal: %d\n", getSize(map));
    
    // Test resizing by adding many elements
    printf("\nAdding more elements to trigger resize...\n");
    for (int i = 0; i < 20; i++) {
        char key[20];
        sprintf(key, "key%d", i);
        put(map, key, i * 10);
    }
    
    printHashMap(map);
    
    // Clear the map
    printf("\nClearing map...\n");
    clear(map);
    printf("Size after clear: %d\n", getSize(map));
    printf("Is empty: %s\n", isEmpty(map) ? "yes" : "no");
    
    // Free memory
    freeHashMap(map);
    
    return 0;
}
