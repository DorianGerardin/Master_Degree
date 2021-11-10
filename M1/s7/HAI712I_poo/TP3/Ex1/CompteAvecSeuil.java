public class CompteAvecSeuil extends Compte {

	private int seuil;
	private int nbLocation;

	public CompteAvecSeuil(Client c) {
		super(c);
		this.seuil = 2; 
	}

	public double prixLocation(Produit p) {
		if (this.nbLocation == this.seuil) {
			this.nbLocation = 0;
			return 0;
		} else {
			this.nbLocation++;
			return p.getPrixLocation();
		}
	}

}