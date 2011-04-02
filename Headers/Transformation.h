class Mat4
{
	float data[16];

	public:
		Mat4();
		Mat4(float * values);
		float& operator()(int x, int y);
		float operator()(int x, int y) const;
		Mat4& operator=(const Mat4& m);
};
