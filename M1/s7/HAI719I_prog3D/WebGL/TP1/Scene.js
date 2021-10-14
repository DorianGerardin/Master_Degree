// Définition de la classe Scene

// superclasses et classes nécessaires
Requires("Spoke");


class Scene
{
    /** constructeur */
    constructor()
    {
        // créer les objets à dessiner
        this.m_spoke = new Spoke();

        // couleur du fond : gris foncé
        gl.clearColor(0.4, 0.4, 0.4, 0.0);
    }


    /**
     * appelée quand la taille de la vue OpenGL change
     * @param width : largeur en nombre de pixels de la fenêtre
     * @param height : hauteur en nombre de pixels de la fenêtre
     */
    onSurfaceChanged(width, height)
    {
        // met en place le viewport
        gl.viewport(0, 0, width, height);
    }


    /**
     * Dessine l'image courante
     */
    onDrawFrame()
    {
        // effacer l'écran
        gl.clear(gl.COLOR_BUFFER_BIT);

        // dessiner le triangle
        this.m_spoke.onDraw();
    }


/** supprime tous les objets de cette scène */
    destroy()
    {
        this.m_spoke.destroy();
    }
}
