class UtilisationDictionnaire {
	public static void main(String[] args) throws Exception {
		IDictionary dic = new OrderedDictionary();
		System.out.println("put 1 : " + dic.put(1, "Coucou"));
		System.out.println("put 2 : " + dic.put(2, "Je"));
		System.out.println("put 3 : " + dic.put(3, "Vais"));
		System.out.println("put 4 : " + dic.put(4, "Te"));
		System.out.println("put 5 : " + dic.put(5, "Tuer"));
		System.out.println("put 6 same key : " + dic.put(5, "Mort"));

		System.out.println("taille : " + dic.size());

		System.out.println("get 1 : " + dic.get(4));

		System.out.println("empty ? : " + dic.isEmpty());

		System.out.println("contains 1 : " + dic.containsKey(1));
		System.out.println("contains 12 : " + dic.containsKey(12));
		System.out.println("get 2 pas dedans : " + dic.get(6));
		System.out.println("contains key qui existe pas : " + dic.containsKey(6));

	}
}