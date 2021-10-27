public class main {

	public static void main(String[] args){
		//Ex 1
		System.out.println("-------Exercice 1 ------ \n");

		Produit lgv = new Produit("La grande vadrouille", 10.0);
		Client cl = new Client("Dupont");
		Compte cmt = new Compte(cl);
		System.out.println(cmt.prixLocation(lgv));

		//Ex 2
		System.out.println("\n-------Exercice 2 ------ \n");

		Compte cmt2 = new CompteAvecReduction(cl);
		System.out.println("CompteReduction : " + cmt2.prixLocation(lgv));
		Compte cmt3 = new CompteAvecSeuil(cl); // le seuil est `a 2 par d ́efaut
		System.out.println("CompteSeuil : " + cmt3.prixLocation(lgv));
		System.out.println("CompteSeuil : " + cmt3.prixLocation(lgv));
		System.out.println("CompteSeuil : " + cmt3.prixLocation(lgv)); // doit afficher 0
		Produit r4 = new ProduitSolde("RockyIV", 10.0);
		System.out.println("CompteNormal+ProduitSolde : " + cmt.prixLocation(r4));
		System.out.println("CompteReduction+ProduitSolde : " + cmt2.prixLocation(r4));
	}

}

//javac Compte.java; javac Produit.java; javac Client.java; javac ProduitSolde.java; javac CompteAvecSeuil.java; javac CompteAvecReduction.java;