public abstract class AbstractDictionary implements IDictionary{

    protected Object[] keys;
    protected Object[] values;

    public AbstractDictionary() {
        this.keys = new Object[0];
        this.values = new Object[0];
    }

    public Object get(Object key) throws Exception {
        if (!containsKey(key) || isEmpty()) {
            throw new Exception("Cette clé n'est pas dans le dictionnaire");
        } else {
            return values[this.indexOf(key)];
        }
    }

    public IDictionary put(Object key, Object value) {
        int index = this.indexOf(key);
        if (index != -1) {
            values[index] = value;
        } else {
            int newIndex = this.newIndexOf(key);
            keys[newIndex] = key;
            values[newIndex] = value;
        }
        return this;
    }

    public boolean isEmpty() {
        return this.size() == 0;
    }

    public boolean containsKey(Object key) {
        return this.indexOf(key) != -1;
    }

    public abstract int size();

    public abstract int indexOf(Object key);
    public abstract int newIndexOf(Object key);

}
