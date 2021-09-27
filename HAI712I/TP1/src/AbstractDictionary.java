public abstract class AbstractDictionary implements IDictionary{

    Object[] keys;
    Object[] values;

    public AbstractDictionary(Object[] keys, Object[] values) {
        this.keys = keys;
        this.values = values;
    }

    public Object get(Object key) {
        return !containsKey(key) || isEmpty() ? null : values[indexOf(key)];
    }

    public IDictionary put(Object key, Object value) {
        if (containsKey(key)) {
            values[indexOf(key)] = value;
        } else {
            int newIndex = newIndexOf(key);
            keys[newIndex] = key;
            values[newIndex] = value;
        }
        return this;
    }

    public boolean isEmpty() {
        return size() == 0;
    }

    public boolean containsKey(Object key) {
        return indexOf(key) == -1;
    }

    public abstract int size();

    abstract int indexOf(Object key);
    abstract int newIndexOf(Object key);

}
