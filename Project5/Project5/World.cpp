#include "World.h"

World::World()
{
	srand(time(NULL));

	axes = new Axes();
	drawAxes = false;

	// Lighting parameters
	directionalColor = { 0.9, 0.9, 0.9 };
	ambientColor = { 0.7, 0.7, 0.7 };

}

World::~World()
{
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		delete textures[i];
	}
}

void World::init()
{
	initValues();

	// initialize camera
	cam.init(0, 2, 5);

	// set camera movement characteristics
	cam.setEyeMove(.05);
	cam.setLookAtMove(.5);

	// initialize shaders
	shader.init("Shaders/PointLight.vert", "Shaders/PointLight.frag");


	// Antialiasing
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	// line width
	glLineWidth(3);

	// set for 3d culling
	glEnable(GL_DEPTH_TEST);

	// setup textures
	setupTextures();

	glClearColor(.7, .8, 1, 1);
}

void World::display()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw world
	draw();

	// swap the buffers at the end of the display sequence
	glutSwapBuffers();
}

void World::keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		//_light.toggle();
		//cout << "Is state: " << _light.isOn() << endl;
		break;
	case 'w':
		cam.camIn();
		break;
	case 'a':
		cam.camLeft();
		break;
	case 's':
		cam.camOut();
		break;
	case 'd':
		cam.camRight();
		break;
	case 'q':
		drawAxes = !drawAxes;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void World::arrowInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		cam.camLookUp();
		break;
	case GLUT_KEY_DOWN:
		cam.camLookDown();
		break;
	case GLUT_KEY_LEFT:
		cam.camLookLeft();
		break;
	case GLUT_KEY_RIGHT:
		cam.camLookRight();
		break;
	}

	glutPostRedisplay();
}

void World::draw()
{
	shader.use();

	//light.setEyeDirection(cam.getEye());

	// setup lighting uniforms
	//light.render(shader);

	// setup camera uniforms
	cam.render(shader);

	if (drawAxes)
		axes->draw(shader);

	terrain.draw(shader);
}

void World::initValues()
{
	
	// init light values
	light.setColor(directionalColor);
	light.setAmbient(ambientColor);
	light.setPosition(vmath::vec3(0.0, 0.5, 0.0));
	light.setShininess(5);
	light.setStrength(20);
	light.setConstantAttenuation(.25);
	light.setLinearAttenuation(.25);
	light.setQuadraticAttenuation(2);
	//light.setEyeDirection(cam.getEye());
	light.toggle();
	

	//----------------------------------------------------------
	// Data for Axes
	//----------------------------------------------------------
	vec4 axesPosition[NUM_AXES][2] = {
		{ vec4(-5.0, 0.0, 0.0, 1.0), vec4(5.0, 0.0, 0.0, 1.0) },
		{ vec4(0.0, -5.0, 0.0, 1.0), vec4(0.0, 5.0, 0.0, 1.0) },
		{ vec4(0.0, 0.0, -5.0, 1.0), vec4(0.0, 0.0, 5.0, 1.0) }
	};

	Color axesColor = { .8, .8, .8, 1 };
	Color terrainColor = { .2, .9, .2, 1 };

	axes->init(axesPosition);
	axes->setColor(axesColor);

	terrain.init("Models/TerrainTest.obj");
	terrain.setColor(terrainColor);
}

void World::setupTextures()
{
	/*
	// Texture Files
	textureFilenames[0] = "";

	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		textures[i] = new Texture();
		textures[i]->loadFromFile(textureFilenames[i]);
	}

	textures[0]->load();
	textures[1]->load();
	textures[2]->load();
	*/
}

void World::idleFunc()
{

}