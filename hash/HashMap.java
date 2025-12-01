class Entry {
    String key;
    int value;
    Entry next;
    
    public Entry(String key, int value) {
        this.key = key;
        this.value = value;
        this.next = null;
    }
}

public class HashMap {
    private static final int INITIAL_CAPACITY = 16;
    private static final double LOAD_FACTOR_THRESHOLD = 0.75;
    
    private Entry[] buckets;
    private int capacity;
    private int size;
    
    public HashMap() {
        this.capacity = INITIAL_CAPACITY;
        this.size = 0;
        this.buckets = new Entry[capacity];
    }
    
    // Hash function
    private int hash(String key) {
        int hashValue = key.hashCode();
        return Math.abs(hashValue) % capacity;
    }
    
    // Get load factor
    private double getLoadFactor() {
        return (double) size / capacity;
    }
    
    // Resize and rehash the map
    private void resize() {
        int oldCapacity = capacity;
        Entry[] oldBuckets = buckets;
        
        capacity *= 2;
        buckets = new Entry[capacity];
        size = 0;
        
        // Rehash all entries
        for (int i = 0; i < oldCapacity; i++) {
            Entry entry = oldBuckets[i];
            while (entry != null) {
                put(entry.key, entry.value);
                entry = entry.next;
            }
        }
    }
    
    // Put a key-value pair into the map
    public void put(String key, int value) {
        // Check if resize is needed
        if (getLoadFactor() >= LOAD_FACTOR_THRESHOLD) {
            resize();
        }
        
        int index = hash(key);
        Entry entry = buckets[index];
        
        // Check if key already exists
        while (entry != null) {
            if (entry.key.equals(key)) {
                entry.value = value;
                return;
            }
            entry = entry.next;
        }
        
        // Insert new entry at the beginning
        Entry newEntry = new Entry(key, value);
        newEntry.next = buckets[index];
        buckets[index] = newEntry;
        size++;
    }
    
    // Get value for a key
    public Integer get(String key) {
        int index = hash(key);
        Entry entry = buckets[index];
        
        while (entry != null) {
            if (entry.key.equals(key)) {
                return entry.value;
            }
            entry = entry.next;
        }
        
        return null;
    }
    
    // Check if key exists
    public boolean containsKey(String key) {
        int index = hash(key);
        Entry entry = buckets[index];
        
        while (entry != null) {
            if (entry.key.equals(key))
                return true;
            entry = entry.next;
        }
        
        return false;
    }
    
    // Remove a key-value pair
    public boolean remove(String key) {
        int index = hash(key);
        Entry entry = buckets[index];
        Entry prev = null;
        
        while (entry != null) {
            if (entry.key.equals(key)) {
                if (prev == null)
                    buckets[index] = entry.next;
                else
                    prev.next = entry.next;
                
                size--;
                return true;
            }
            prev = entry;
            entry = entry.next;
        }
        
        return false;
    }
    
    // Get all keys
    public String[] getKeys() {
        String[] keys = new String[size];
        int index = 0;
        
        for (int i = 0; i < capacity; i++) {
            Entry entry = buckets[i];
            while (entry != null) {
                keys[index++] = entry.key;
                entry = entry.next;
            }
        }
        
        return keys;
    }
    
    // Get all values
    public int[] getValues() {
        int[] values = new int[size];
        int index = 0;
        
        for (int i = 0; i < capacity; i++) {
            Entry entry = buckets[i];
            while (entry != null) {
                values[index++] = entry.value;
                entry = entry.next;
            }
        }
        
        return values;
    }
    
    // Get all entries
    public Entry[] getEntries() {
        Entry[] entries = new Entry[size];
        int index = 0;
        
        for (int i = 0; i < capacity; i++) {
            Entry entry = buckets[i];
            while (entry != null) {
                entries[index++] = entry;
                entry = entry.next;
            }
        }
        
        return entries;
    }
    
    // Clear all entries
    public void clear() {
        for (int i = 0; i < capacity; i++) {
            buckets[i] = null;
        }
        size = 0;
    }
    
    // Check if map is empty
    public boolean isEmpty() {
        return size == 0;
    }
    
    // Get size of map
    public int size() {
        return size;
    }
    
    // Get capacity
    public int capacity() {
        return capacity;
    }
    
    // Print the hash map
    public void printHashMap() {
        System.out.printf("HashMap (size: %d, capacity: %d, load factor: %.2f):%n", 
                         size, capacity, getLoadFactor());
        
        for (int i = 0; i < capacity; i++) {
            if (buckets[i] != null) {
                System.out.print("Bucket " + i + ": ");
                Entry entry = buckets[i];
                while (entry != null) {
                    System.out.print("[" + entry.key + ": " + entry.value + "] ");
                    entry = entry.next;
                }
                System.out.println();
            }
        }
    }
    
    // Print all key-value pairs
    public void printAll() {
        System.out.println("All entries:");
        for (Entry entry : getEntries()) {
            System.out.println(entry.key + " -> " + entry.value);
        }
    }
    
    // Example usage
    public static void main(String[] args) {
        HashMap map = new HashMap();
        
        // Insert key-value pairs
        System.out.println("Inserting elements...");
        map.put("apple", 100);
        map.put("banana", 200);
        map.put("orange", 300);
        map.put("grape", 400);
        map.put("mango", 500);
        
        map.printHashMap();
        
        // Get values
        System.out.println("\nGetting values:");
        Integer value = map.get("banana");
        System.out.println("banana: " + (value != null ? "found (value: " + value + ")" : "not found"));
        
        value = map.get("cherry");
        System.out.println("cherry: " + (value != null ? "found" : "not found"));
        
        // Check if key exists
        System.out.println("\nContains key 'apple': " + map.containsKey("apple"));
        System.out.println("Contains key 'cherry': " + map.containsKey("cherry"));
        
        // Update value
        System.out.println("\nUpdating 'apple' to 150...");
        map.put("apple", 150);
        value = map.get("apple");
        System.out.println("apple: " + value);
        
        // Get all keys
        System.out.print("\nAll keys: ");
        for (String key : map.getKeys()) {
            System.out.print(key + " ");
        }
        System.out.println();
        
        // Get all values
        System.out.print("All values: ");
        for (int val : map.getValues()) {
            System.out.print(val + " ");
        }
        System.out.println();
        
        // Remove a key
        System.out.println("\nRemoving 'banana'...");
        map.remove("banana");
        System.out.println("Size after removal: " + map.size());
        
        // Test resizing by adding many elements
        System.out.println("\nAdding more elements to trigger resize...");
        for (int i = 0; i < 20; i++) {
            map.put("key" + i, i * 10);
        }
        
        map.printHashMap();
        
        System.out.println("\nAll entries after resize:");
        map.printAll();
        
        // Clear the map
        System.out.println("\nClearing map...");
        map.clear();
        System.out.println("Size after clear: " + map.size());
        System.out.println("Is empty: " + map.isEmpty());
    }
}
