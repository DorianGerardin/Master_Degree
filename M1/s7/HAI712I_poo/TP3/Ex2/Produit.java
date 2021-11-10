public class Produit {

	private double prixLocation;
	private String label;

	public Produit(String label, double prixLocation) {
		this.label = label;
		this.prixLocation = prixLocation;
	}

	public double getPrixLocation() {
		return this.prixLocation;
	}

}