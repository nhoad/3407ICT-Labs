GameEntity::GameEntity(string meshFile, string textureFile, Mat4 m)
{
	if (meshFile.size() > 0)
		this->mesh = Loader::readMesh(meshFile);

	if (textureFile.size() > 0)
		this->texture = Loader::loadTexture(textureFile);
	else
		this->texture = 0;

	this->matrix = m;
}

GameEntity::GameEntity(Mesh mesh, Mat4 m)
{
	this->mesh = mesh;
	this->texture = 0;
	this->matrix = m;

}

float GameEntity::centre(int k)
{
	float minK, maxK;

	minK = mesh[0].pos[k];
	maxK = mesh[0].pos[k];

	for (unsigned i=0; i < mesh.size(); i++)
	{
		if (mesh[i].pos[k] < minK)
			minK = mesh[i].pos[k];

		if (mesh[i].pos[k] > maxK)
			maxK = mesh[i].pos[k];
	}

	return (minK + maxK) / 2.0f;
}

float GameEntity::centreY()
{
	return centre(1);
}

float GameEntity::centreZ()
{
	return centre(2);
}

float GameEntity::centreX()
{
	return centre(0);
}

void GameEntity::draw()
{
	glPushMatrix();

	glMultMatrixf(matrix);

	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh[0].tx);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh[0].nx);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_QUADS, 0, mesh.size());

	glPopMatrix();

}

void GameEntity::buffer()
{
	if (mesh.size() == 0)
		return;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.size(), &mesh[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

