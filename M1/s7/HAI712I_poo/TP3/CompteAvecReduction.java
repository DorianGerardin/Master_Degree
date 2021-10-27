public class CompteAvecReduction extends Compte {

	public double reduction;

	public CompteAvecReduction(Client c) {
		super(c);
		this.reduction = 0.1;
	}

	public double prixLocation(Produit p) {
		return super.prixLocation(p) * (1-this.reduction);
	}



}