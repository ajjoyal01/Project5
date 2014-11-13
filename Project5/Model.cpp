#include "Model.h"


Model::Model()
{
	color = { 1, 1, 1, 1 };
}

Model::~Model()
{
}

// Draw
void Model::draw(Shader shader)
{
	glBindVertexArray(VAOs[0]);

	activateTextures(shader);

	glEnableVertexAttribArray(vPosition);

	if (texels.size() > 0)
	{
		glEnableVertexAttribArray(vTexel);
	}
	if (normals.size() > 0)
	{
		glEnableVertexAttribArray(vNormal);
	}
	if (textureIDs.size() > 0)
	{
		glEnableVertexAttribArray(vTexture);
	}

	glVertexAttribI1i(vIsTextured, isTextured);
	glVertexAttribI1i(vIsTransformed, isTransformed);
	glVertexAttrib4fv(vModelMatrix0, &transform[0][0]);
	glVertexAttrib4fv(vModelMatrix1, &transform[1][0]);
	glVertexAttrib4fv(vModelMatrix2, &transform[2][0]);
	glVertexAttrib4fv(vModelMatrix3, &transform[3][0]);
	glVertexAttrib4fv(vColor, &color.red);
	glVertexAttrib4fv(vNormalMatrix0, &nTransform[0][0]);
	glVertexAttrib4fv(vNormalMatrix1, &nTransform[1][0]);
	glVertexAttrib4fv(vNormalMatrix2, &nTransform[2][0]);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	deactivateTextures();
}


// Transformation Stuff
void Model::scale(float scaleFactor)
{
	// Translate to center
	vmath::mat4 translate1 = vmath::translate(0 - center.x, 0 - center.y, 0 - center.z);
	vmath::mat4 scale = vmath::scale(scaleFactor);
	vmath::mat4 translate2 = vmath::translate(center.x, center.y, center.z);

	transform = (translate2 * scale * translate1) * transform;
	updateCenter();
	updateNormalMat();
}

void Model::translate(float x, float y, float z)
{
	vmath::mat4 translate = vmath::translate(x, y, z);
	transform = translate * transform;

	updateCenter();
	updateNormalMat();
}

void Model::rotate(float angle, vmath::vec3 inAxis)
{
	// Translate to center
	vmath::mat4 translate1 = vmath::translate(0 - center.x, 0 - center.y, 0 - center.z);
	vmath::mat4 rotate = vmath::rotate(angle, inAxis);
	vmath::mat4 translate2 = vmath::translate(center.x, center.y, center.z);

	transform = (translate2 * rotate * translate1) * transform;
	updateCenter();
	updateNormalMat();
}

void Model::updateCenter()
{
	center.x = transform[3][0];
	center.y = transform[3][1];
	center.z = transform[3][2];
}

// Loading Stuff
void Model::init(string filename)
{
	loadObject(filename);

	// create Vertex Array
	glGenVertexArrays(1, VAOs);
	glBindVertexArray(VAOs[0]);

	// create Buffers
	glGenBuffers(NUM_BUFFERS, Buffers);

	glEnableVertexAttribArray(vPosition);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[POS_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vmath::vec4), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	if (texels.size() > 0)
	{
		glEnableVertexAttribArray(vTexel);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXEL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, texels.size() * sizeof(vmath::vec2), &texels[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vTexel, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (normals.size() > 0)
	{
		glEnableVertexAttribArray(vNormal);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vmath::vec3), &normals[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (textureIDs.size() > 0)
	{
		glEnableVertexAttribArray(vTexture);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, textureIDs.size() * sizeof(int), &textureIDs[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vTexture, 1, GL_INT, GL_FALSE, 0, 0);
	}

	transform = vmath::mat4::identity();
	updateNormalMat();
	center = vec4(0.0, 0.0, 0.0, 1.0);

	isTransformed = 1;
	calculateDimentions();
}

// Gets vertex, texel, normal data
void Model::loadObject(string filename)
{
	name = filename;

	vector<vmath::vec4> in_vertices;
	vector<vmath::vec3> in_normals;
	vector<vmath::vec2> in_texels;
	vector<string> in_materials;

	bool isTexel = false;
	bool isNormal = false;
	int curTextID = -1;

	string matFileName;

	ifstream in(filename, ios::in);

	if (!in)
	{
		cerr << "Cannot open " << filename << endl;
	}

	string line;

	while (getline(in, line))
	{
		//-----------------------------------
		// Get Material File Name
		//-----------------------------------
		if (line.substr(0, 7) == "mtllib ")
		{
			istringstream s(line.substr(7));
			s >> matFileName;
		}
		//-----------------------------------
		else
		{
			//-----------------------------------
			// Read Vertex Positions
			//-----------------------------------
			if (line.substr(0, 2) == "v ")
			{
				istringstream s(line.substr(2));
				vmath::vec4 v;
				float x, y, z, w;
				s >> x;
				s >> y;
				s >> z;
				w = 1.0f;
				v = vmath::vec4(x, y, z, w);
				in_vertices.push_back(v);
			}
			//-----------------------------------
			else
			{
				//-----------------------------------
				// Read Texels
				//-----------------------------------
				if (line.substr(0, 3) == "vt ")
				{
					isTexel = true;
					istringstream s(line.substr(3));
					vmath::vec2 v;
					float x, y;
					s >> x;
					s >> y;
					y = 1 - y;
					v = vmath::vec2(x, y);
					in_texels.push_back(v);
					//-----------------------------------
				}
				else
				{
					//-----------------------------------
					// Read Normals
					//-----------------------------------
					if (line.substr(0, 3) == "vn ")
					{
						isNormal = true;
						istringstream s(line.substr(3));
						vmath::vec3 v;
						float x, y, z;
						s >> x;
						s >> y;
						s >> z;
						v = vmath::vec3(x, y, z);
						in_normals.push_back(v);
						//-----------------------------------
					}
					else
					{
						if (line.substr(0, 7) == "usemtl ")
						{
							istringstream s(line.substr(7));
							string temp;
							s >> temp;

							if (temp.find("NONE") == -1)
							{
								curTextID = 1;
							}
							else
							{
								curTextID = 0;
							}

							/*
							//cout << temp << endl;
							bool newMaterial = true;

							// if material name is already in the vector
							for (int i = 0; i < in_materials.size(); i++)
							{
							if (temp == in_materials.at(i))
							{
							newMaterial = false;

							// set current ID to the location of the existing material
							curTextID = i;
							}
							}

							// if this is a new material
							if (newMaterial)
							{
							in_materials.push_back(temp);

							// current ID is the location of the last element in the array
							curTextID = in_materials.size();
							}	*/
						}
						else
						{
							//-----------------------------------
							// Read Face Info
							//-----------------------------------
							if (line.substr(0, 2) == "f ")
							{
								istringstream s(line.substr(2));

								if (!isNormal && !isTexel)
								{
									GLushort av, bv, cv;
									s >> av;
									s >> bv;
									s >> cv;
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
								}
								else if (!isTexel)
								{
									GLushort av, an, bv, bn, cv, cn;
									s >> av;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> an;
									s >> bv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bn;
									s >> cv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> cn;
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
									normals.push_back(in_normals[--an]);
									normals.push_back(in_normals[--bn]);
									normals.push_back(in_normals[--cn]);
								}
								else if (!isNormal)
								{
									GLushort av, at, bv, bt, cv, ct;
									s >> av;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> at;
									s >> bv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bt;
									s >> cv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> ct;
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
									texels.push_back(in_texels[--at]);
									texels.push_back(in_texels[--bt]);
									texels.push_back(in_texels[--ct]);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
								}
								else if (isNormal && isTexel)
								{
									GLushort av, at, an, bv, bt, bn, cv, ct, cn;
									s >> av;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> at;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> an;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bt;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> bn;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> cv;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> ct;
									while (s.peek() == '/')
									{
										s.get();
									}
									s >> cn;
									while (s.peek() == '/')
									{
										s.get();
									}
									vertices.push_back(in_vertices[--av]);
									vertices.push_back(in_vertices[--bv]);
									vertices.push_back(in_vertices[--cv]);
									texels.push_back(in_texels[--at]);
									texels.push_back(in_texels[--bt]);
									texels.push_back(in_texels[--ct]);
									normals.push_back(in_normals[--an]);
									normals.push_back(in_normals[--bn]);
									normals.push_back(in_normals[--cn]);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
									textureIDs.push_back(curTextID);
								}

							}// end "f " if else

						}// end "usemtl " else

					}// end "vn " if else

				}// end "vt " if else

			}// end "v " if else

		}// end getting material file name

	}// end parse while loop for .obj files

	/*
	if (filename == "Models/2_clubs.obj")
	{
	for (int i = 0; i < textureIDs.size(); i++)
	{
	//for (int j = 0; j < 2; j++)
	//{
	cout << textureIDs[i] << "\t";
	//}
	cout << endl;

	}

	cout << endl << endl << endl;
	}*/

	in.close();

	/*
	// if there is a material file
	if (matFileName != "")
	{
	// switch to parsing Material File
	ifstream in("Models/" + matFileName, ios::in);

	if (!in)
	{
	cerr << "Cannot open " << matFileName << endl;
	exit(1);
	}

	// while the line == map_Kd, get the filename off the back of the line
	while (getline(in, line))
	{
	if (line.substr(0, 7) == "map_Kd ")
	{
	istringstream s(line.substr(7));
	string texName;
	getline(s, texName);

	//erase up to file name
	texName.erase(texName.begin(), texName.begin() + texName.find_last_of("\\") + 1);

	// make new texture
	Texture* newTexture = new Texture();

	// load texture from file
	newTexture->loadFromFile( "../Textures/Playing Cards/" + texName);

	// push texture into object's texture vector
	textures.push_back(newTexture);
	}
	}
	}

	in.close();
	*/
}

void Model::calculateDimentions()
{
	max_x = 0;
	max_y = 0;
	max_z = 0;
	min_x = INFINITY;
	min_y = INFINITY;
	min_z = INFINITY;

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		if (vertices.at(i)[0] > max_x)
			max_x = vertices.at(i)[0];
		if (vertices.at(i)[1] > max_y)
			max_y = vertices.at(i)[1];
		if (vertices.at(i)[2] > max_z)
			max_z = vertices.at(i)[2];

		if (vertices.at(i)[0] < min_x)
			min_x = vertices.at(i)[0];
		if (vertices.at(i)[1] < min_y)
			min_y = vertices.at(i)[1];
		if (vertices.at(i)[2] < min_z)
			min_z = vertices.at(i)[2];
	}

	//cout << "Width: " << max_y - min_y << endl;
	//cout << "Height: " << max_z - min_z << endl;
	//cout << "Depth: " << max_x - min_x << endl;

	height = max_x - min_x;
	depth = max_y - min_y;
	width = max_z - min_z;


}

float Model::getWidth()
{
	return width;
}

float Model::getHeight()
{
	return height;
}

float Model::getDepth()
{
	return depth;
}


// Texture Stuff
void Model::setTexture(Texture* inTexture)
{
	texture = inTexture;

	if (texture != nullptr)
	{
		isTextured = 1;
	}
	else
	{
		isTextured = 0;
	}
}

void Model::activateTextures(Shader shader)
{
	if (texture != nullptr)
		texture->activate(shader.getUniformLocation("tex"));
}

void Model::deactivateTextures()
{
	if (texture != nullptr)
		texture->deactivate();
}

void Model::updateTransform(vmath::mat4 inTransform)
{
	transform = inTransform * transform;
	updateCenter();
	updateNormalMat();
}

void Model::setColor(Color inColor)
{
	color = inColor;
}

void Model::updateNormalMat()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			nTransform[j][i] = transform[j][i];
		}
	}

	nTransform = glm::transpose(glm::inverse(nTransform));
}

float Model::getMaxY()
{
	return max_y;
}