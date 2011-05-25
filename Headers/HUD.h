#ifndef HUD_h
#define HUD_h

class HUD
{
	public:
		HUD (int height, int width);
		virtual ~HUD ();

		/**
		 * draw method. Should be called every frame, after your 3D drawing.
		 *
		 * This should be over ridden, but stil called to set up the orthogonal matrix
		 */
		virtual void draw();

		/**
		 * Return to normal draw mode. Should be called once you're done drawing 2D stuff.
		 */
		virtual void reset();

	private:
		int height, width;
};
#endif
