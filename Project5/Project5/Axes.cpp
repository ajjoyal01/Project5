#include "Axes.h"

void Axes::init(vec4 inDirection[NUM_AXES][NUM_VERTICES])
{
	// sets whether affected by transformations
	isTransformed = 0;
	setIsTextured(false);

	// initialize positions and colors (i = axis,j = vertex,k = value)
	for (int i = 0; i < NUM_AXES; i++)
	{
		for (int j = 0; j < NUM_VERTICES; j++)
		{
			_position[i][j] = inDirection[i][j];
		}
	}

	// create Vertex Array
	glGenVertexArrays(NUM_AXES_VAOS, VAOs);
	glBindVertexArray(VAOs[AXES_VERTS]);

	// create Buffer
	glGenBuffers(NUM_AXES_BUFFERS, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[AXES_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_position), _position, GL_DYNAMIC_DRAW);

	// set attrib pointer
	glVertexAttribPointer(vDirection, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
}


/*
	Sets the color of the axes.
	Pre: None
	Post: the axes color is set
*/
void Axes::setColor(Color inColor)
{
	// populate colors
	color.red = inColor.red;
	color.green = inColor.green;
	color.blue = inColor.blue;
	color.alpha = inColor.alpha;
}

void Axes::draw(Shader shader)
{
	glBindVertexArray(VAOs[AXES_VERTS]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[AXES_BUFFER]);

	// enable vertex array attributes
	glEnableVertexAttribArray(vDirection);
	
	glVertexAttribI1i(vIsTextured, isTextured);
	glVertexAttribI1i(vIsTransformed, isTransformed);
	glVertexAttrib4fv(vModelMatrix0, &transform[0][0]);
	glVertexAttrib4fv(vModelMatrix1, &transform[1][0]);
	glVertexAttrib4fv(vModelMatrix2, &transform[2][0]);
	glVertexAttrib4fv(vModelMatrix3, &transform[3][0]);
	glVertexAttrib4fv(vColor, &color.red);

	glDrawArrays(GL_LINES, 0, NUM_AXES * NUM_VERTICES);

}

void Axes::setIsTextured(bool isTextured)
{
	if (isTextured)
		isTextured = 1;
	else
		isTextured = 0;

}