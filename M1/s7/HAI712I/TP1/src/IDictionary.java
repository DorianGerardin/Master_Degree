public interface IDictionary {

    //Return la valeur associée à key dans le receveur
    Object get(Object key);

    //Entre un nouveau couple clé-valeur dans le receveur et le return
    IDictionary put(Object key, Object value);

    //Return true si le receveur est vide, false sinon
    boolean isEmpty();

    //Return true si la clé est connue dans le receveur, false sinon
    boolean containsKey(Object key);

    //Return le nombre d'éléments contenus dans le receveur
    int size();

}
