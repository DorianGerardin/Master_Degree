class FastDictionary implements IDictionary{

	public FastDictionary() {
        keys = new Obejct[20];
        values = new Obejct[20];
    }

    public int indexOf(Object key) {
        int hash = key.hashCode();
        int index = hash%this.size();
        if(this.keys[index].equals(key)) return index;
        else {
        	for (int i = index;i < this.size(); i++) {
	            if(this.keys[i] == key) {
	                return i;
	            }
        	} return -1;
        }
    }

    public int newIndexOf(Object key) {
        int hash = key.hashCode();
        int index = hash%this.size();
    	while(!this.keys[index].equals(null)) {
    		(index++)%this.key.length;
    	}
    	return index;
   		
    }

    public boolean mustGrow() {
    	return this.keys.length*0.75 < this.size();
    }

    public void grow() {
    	int size = this.key.length;
        Object[] newKeys = new Object[size*2];
        Object[] newValues = new Object[size*2];
    	for (int i = 0; i < size; i++) {
    		if(!this.keys[i].equals(null)) {
	    		int hash = key[i].hashCode();
	        	int index = hash%this.size();
	        	while(!this.keys[index].equals(null) & index < this.keys.length) {
		    		index++;
		    	} 
		    	newKeys[index] = this.keys[i];
		    	newValues[index] = this.values[i];
    		} 
    	}
    	this.keys = newKeys;
        this.values = newValues;
    }

    public int size() {
    	int size = 0;
        for (int i = 0; i < this.keys.length; ++i) {
        	if (this.keys[i] != null) {
        		size++;
        	}
        } return size;
    }

}