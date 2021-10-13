class FastDictionary extends AbstractDictionary{

	public FastDictionary() {
        keys = new Object[4];
        values = new Object[4];
    }

    public int indexOf(Object key) {
        int hash = Math.abs(key.hashCode());
        int index = hash%this.keys.length;
        if(this.keys[index] != null && this.keys[index].equals(key)) return index;
        else {
        	for (int i = index;i < this.keys.length; i++) {
	            if(this.keys[i] == key) {
	                return i;
	            }
        	} return -1;
        }
    }

    public int newIndexOf(Object key) {
        int hash = key.hashCode();
        int index = Math.abs(hash%this.keys.length);
    	while(this.keys[index] != null) {
            index = (index+1)%this.keys.length;
    	}
    	return index;
   		
    }

    public boolean mustGrow() {
    	return this.keys.length*0.75 <= this.size();
    }

    public void grow() {
        System.out.println("Growing ...");
    	int size = this.keys.length;
        Object[] newKeys = new Object[size*2];
        Object[] newValues = new Object[size*2];
    	for (int i = 0; i < size; i++) {
    		if(this.keys[i] != null) {
	    		int hash = Math.abs(this.keys[i].hashCode());
	        	int index = hash%newKeys.length;
	        	while(newKeys[index] != null) {
		    		index = (index+1)%newKeys.length;
		    	} 
		    	newKeys[index] = this.keys[i];
		    	newValues[index] = this.values[i];
    		} 
    	}
    	this.keys = newKeys;
        this.values = newValues;
    }

    public IDictionary put(Object key, Object value) {
        if (this.mustGrow()) this.grow();
        return super.put(key, value);
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