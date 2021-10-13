class SortedDictionary extends AbstractDictionary{

    public SortedDictionary() {
        keys = new Object[10];
        values = new Object[10];
    }

    public int indexOf(Object key) {
        for (int i = 0;i < this.size(); i++) {
            if(this.keys[i] == key) {
                return i;
            }
        } return -1;
    }

    public int newIndexOf(Object key) {
        int taille = this.size();
        Object[] newKeys = new Object[taille+1];
        Object[] newValues = new Object[taille+1];
        for (int i = 0; i < taille; i++) {
            newValues[i] = this.values[i];
            newKeys[i] = this.keys[i];
        }
        keys = newKeys;
        values = newValues;
        return taille;
    }

    public int size() {
        return this.keys.length;
    }

}