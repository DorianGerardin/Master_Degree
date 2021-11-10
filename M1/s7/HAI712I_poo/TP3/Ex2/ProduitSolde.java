public class ProduitSolde extends Produit {

	private final double solde = 0.5;

	public ProduitSolde(String label, double prixLocation) {
		super(label, prixLocation);
	}

	public double getPrixLocation() {
		return super.getPrixLocation() * this.solde;
	}

}