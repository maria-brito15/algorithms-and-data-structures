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

public class SimplifiedHashMap {
    private Entry[] buckets;
    private int capacity;
    private int size;
    
    public HashMap(int capacity) {
        this.capacity = capacity;
        this.size = 0;
        this.buckets = new Entry[capacity];
    }
    
    private int hash(String key) {
        return Math.abs(key.hashCode()) % capacity;
    }
    
    public void put(String key, int value) {
        int index = hash(key);
        Entry atual = buckets[index];
        
        // Atualiza se a chave já existe
        while (atual != null) {
            if (atual.key.equals(key)) {
                atual.value = value;
                return;
            }
            atual = atual.next;
        }
        
        // Insere novo no início
        Entry novo = new Entry(key, value);
        novo.next = buckets[index];
        buckets[index] = novo;
        size++;
    }
    
    public Integer get(String key) {
        int index = hash(key);
        Entry atual = buckets[index];
        
        while (atual != null) {
            if (atual.key.equals(key)) {
                return atual.value;
            }
            atual = atual.next;
        }
        return null;
    }
    
    public boolean remove(String key) {
        int index = hash(key);
        Entry atual = buckets[index];
        Entry anterior = null;
        
        while (atual != null) {
            if (atual.key.equals(key)) {
                if (anterior == null) {
                    buckets[index] = atual.next;
                } else {
                    anterior.next = atual.next;
                }
                size--;
                return true;
            }
            anterior = atual;
            atual = atual.next;
        }
        return false;
    }
    
    public boolean containsKey(String key) {
        return get(key) != null;
    }
    
    public String[] getKeys() {
        String[] keys = new String[size];
        int idx = 0;
        
        for (int i = 0; i < capacity; i++) {
            Entry atual = buckets[i];
            while (atual != null) {
                keys[idx++] = atual.key;
                atual = atual.next;
            }
        }
        return keys;
    }
    
    public int[] getValues() {
        int[] values = new int[size];
        int idx = 0;
        
        for (int i = 0; i < capacity; i++) {
            Entry atual = buckets[i];
            while (atual != null) {
                values[idx++] = atual.value;
                atual = atual.next;
            }
        }
        return values;
    }
    
    public Entry[] getEntries() {
        Entry[] entries = new Entry[size];
        int idx = 0;
        
        for (int i = 0; i < capacity; i++) {
            Entry atual = buckets[i];
            while (atual != null) {
                entries[idx++] = atual;
                atual = atual.next;
            }
        }
        return entries;
    }
    
    public void clear() {
        for (int i = 0; i < capacity; i++) {
            buckets[i] = null;
        }
        size = 0;
    }
    
    public boolean isEmpty() {
        return size == 0;
    }
    
    public int size() {
        return size;
    }
    
    public void print() {
        for (int i = 0; i < capacity; i++) {
            if (buckets[i] != null) {
                System.out.print("Bucket " + i + ": ");
                Entry atual = buckets[i];
                while (atual != null) {
                    System.out.print("[" + atual.key + "=" + atual.value + "] ");
                    atual = atual.next;
                }
                System.out.println();
            }
        }
    }
    
    public static void main(String[] args) {
        HashMap map = new HashMap(10);
        
        // Testando put
        map.put("a", 1);
        map.put("b", 2);
        map.put("c", 3);
        map.put("d", 4);
        
        map.print();
        System.out.println("Size: " + map.size());
        
        // Testando get
        System.out.println("\nGet 'b': " + map.get("b"));
        System.out.println("Get 'z': " + map.get("z"));
        
        // Testando containsKey
        System.out.println("\nContains 'a': " + map.containsKey("a"));
        System.out.println("Contains 'z': " + map.containsKey("z"));
        
        // Testando getKeys e getValues
        System.out.print("\nKeys: ");
        for (String k : map.getKeys()) {
            System.out.print(k + " ");
        }
        System.out.print("\nValues: ");
        for (int v : map.getValues()) {
            System.out.print(v + " ");
        }
        
        // Testando remove
        System.out.println("\n\nRemoving 'b': " + map.remove("b"));
        System.out.println("Size: " + map.size());
        map.print();
        
        // Testando isEmpty e clear
        System.out.println("\nIs empty: " + map.isEmpty());
        map.clear();
        System.out.println("After clear, is empty: " + map.isEmpty());
        System.out.println("Size: " + map.size());
    }
}