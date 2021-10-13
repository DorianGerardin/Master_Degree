class UtilisationDictionnaire {
	public static void main(String[] args) throws Exception {

		//--------Ordered Dic--------
		/*IDictionary dic = new OrderedDictionary();
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
		System.out.println("contains key qui existe pas : " + dic.containsKey(6));*/


		//--------Fast Dic--------
		IDictionary dic = new FastDictionary();
		System.out.println("put 1 : " + dic.put("velo", "objet pour rouler"));
		System.out.println("put 2 : " + dic.put("fourchette", "Ustentile pour manger"));
		System.out.println("put 3 : " + dic.put("Ordinateur", "Machine binaire"));
		System.out.println("put 4 : " + dic.put("Racisme", "C'est pas bien"));
		System.out.println("put 5 : " + dic.put("Mort", "Etat de non-vie"));
		System.out.println("put 6 same key : " + dic.put("velo", "Etat de non-mort"));

		System.out.println("taille : " + dic.size());

		System.out.println("empty ? : " + dic.isEmpty());

		System.out.println("contains vélo : " + dic.containsKey("vélo"));
		System.out.println("contains Ordinateur : " + dic.containsKey("Ordinateur"));
		
		System.out.println("get fourchette : " + dic.get("fourchette"));
		
		System.out.println("get Ordi pas dedans : " + dic.get("Ordi"));
	}
}