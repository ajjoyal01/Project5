#include "World.h"

World::World()
{
	srand(time(NULL));

	axes = new Axes();
	drawAxes = false;

	animate_amount = 0;
	animate_factor = 0.01;

	lighting_on = true;
	shadow_maps_on = true;
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
	cam.init(0, 5, 5);

	// set camera movement characteristics
	cam.setEyeMove(.2);
	cam.setLookAtMove(.7);

	// initialize shaders
	shader.init("Shaders/vertices.vert", "Shaders/fragments.frag");
	shadowMapShader.init("Shaders/shadowMap.vert", "Shaders/shadowMap.frag");

	shadowMap.init();

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

	glClearColor(.7, .8, .8, 1);
}

void World::display()
{
	// clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (lights.at(DIRECTIONAL)->getIsEnabled())
	{
		// set the light view matrix
		shadowMap.updateView(lights.at(DIRECTIONAL)->getPosition());

		// use the shadow map shader
		shadowMapShader.use();

		// setup uniforms for light VPMatrix
		shadowMap.render(shadowMapShader);

		// bind framebuffer, set viewport
		shadowMap.draw();

		// draw world
		if (drawAxes)
			axes->draw(shadowMapShader);

		for (int i = 0; i < cubes.size(); i++)
		{
			cubes.at(i)->draw(shadowMapShader);
		}
		terrain.draw(shadowMapShader);

		// unbind framebuffer
		shadowMap.endDraw();

		// unuse shadow map shader
		shadowMapShader.unuse();

		// set uniforms for Shadow Matrix and depth_texture
		shadowMap.use(shader, 0);
	}
	
	

	//renderShadowMaps();

	// use main shader
	shader.use();

	draw();

	shader.unuse();
	
	// swap the buffers at the end of the display sequence
	glutSwapBuffers();
}

void World::renderShadowMaps()
{
	shadowMapShader.use();

	for (int i = 0; i < 1; i++)
	{
		lights.at(i)->mapShadows(shadowMapShader);
		drawScene(shadowMapShader);
		lights.at(i)->endMapShadows();
	}

	shadowMapShader.unuse();
}

void World::keyPress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		cam.setView(glm::lookAt(vec3(10,10,0), vec3(0, 0, 0), vec3(0, 1, 0)));
		break;
	case 'l':
		lighting_on = !lighting_on;
		break;
	case 'm':
		shadow_maps_on = !shadow_maps_on;
		break;
	case 't':
		lights.at(DIRECTIONAL)->setIsEnabled(!lights.at(DIRECTIONAL)->getIsEnabled());
		break;
	case 'f':
		lights.at(FLASHLIGHT)->setIsEnabled(!lights.at(FLASHLIGHT)->getIsEnabled());
		break;
	case 'w':
		//if (cam.getEyeZ() > -70)
			cam.camIn();
		break;
	case 'a':
		//if (cam.getEyeX() > -70)
			cam.camLeft();
		break;
	case 's':
		//if (cam.getEyeZ() < 70)
			cam.camOut();
		break;
	case 'd':
		//if (cam.getEyeX() < 70)
			cam.camRight();
		break;
	case 'q':
		drawAxes = !drawAxes;
		break;
	case 'p':
		createBlock();
		break;
	default:
		break;
	}

	//cout << "Eye: (" << cam.getEyePosition().x << "," << cam.getEyePosition().z << ")" << endl;

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
	glViewport(0, 0, 900, 900);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform3f(shader.getUniformLocation("eyeDirection"), 0,0,0);
	glUniform3f(shader.getUniformLocation("Ambient"), ambientLight.x, ambientLight.y, ambientLight.z);
	glUniform1f(shader.getUniformLocation("Strength"), .5);
	glUniform1f(shader.getUniformLocation("Shininess"), 10);
	glUniform1i(shader.getUniformLocation("LightingOn"), lighting_on);
	glUniform1i(shader.getUniformLocation("ShadowsOn"), shadow_maps_on);

	// setup lighting uniforms
	for (int i = 0; i < lights.size(); i++)
	{
		lights.at(i)->render(shader, i);
	}

	// setup camera uniforms
	cam.render(shader);

	drawScene(shader);
}

void World::drawScene(Shader in_shader)
{
	if (drawAxes)
		axes->draw(in_shader);

	terrain.draw(in_shader);
	for (int i = 0; i < cubes.size(); i++)
	{
		cubes.at(i)->draw(in_shader);
	}
}

void World::initValues()
{
	Color lightColor = { .8, .8, .8, 1 };
	Color ambientColor = { .3, .3, .3, 1 };

	Light* directionalLight = new Light();
	lights.push_back(directionalLight);
	// init light values
	lights.at(DIRECTIONAL)->setIsEnabled(true);
	lights.at(DIRECTIONAL)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(DIRECTIONAL)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(DIRECTIONAL)->setPosition(vec3(10, 10, 10));

	lightColor = { 1, 1, 1, 1 };
	ambientColor = { 0, 0, 0, 1 };

	Light* flashlight = new Light();
	lights.push_back(flashlight);
	// init light values
	lights.at(FLASHLIGHT)->setIsEnabled(true);
	lights.at(FLASHLIGHT)->setIsLocal(true);
	lights.at(FLASHLIGHT)->setIsSpot(true);
	lights.at(FLASHLIGHT)->setAmbient(vec3(ambientColor.red, ambientColor.green, ambientColor.blue));
	lights.at(FLASHLIGHT)->setColor(vec3(lightColor.red, lightColor.green, lightColor.blue));
	lights.at(FLASHLIGHT)->setPosition(vec3(0, 0, 0));
	lights.at(FLASHLIGHT)->setHalfVector(vec3(0, 0, 0));
	lights.at(FLASHLIGHT)->setConeDirection(vec3(0, 0, -1));
	lights.at(FLASHLIGHT)->setSpotCosCutoff(.9);
	lights.at(FLASHLIGHT)->setSpotExponent(100);
	lights.at(FLASHLIGHT)->setConstantAttenuation(.05);
	lights.at(FLASHLIGHT)->setLinearAttenuation(.005);
	lights.at(FLASHLIGHT)->setQuadraticAttenuation(.005);
	
	ambientLight = vec3(.3, .3, .3);

	//----------------------------------------------------------
	// Data for Axes
	//----------------------------------------------------------
	vec4 axesPosition[NUM_AXES][2] = {
			{ vec4(-5.0, 0.0, 0.0, 1.0), vec4(5.0, 0.0, 0.0, 1.0) },
			{ vec4(0.0, -5.0, 0.0, 1.0), vec4(0.0, 5.0, 0.0, 1.0) },
			{ vec4(0.0, 0.0, -5.0, 1.0), vec4(0.0, 0.0, 5.0, 1.0) }
	};

	Color axesColor = { 0, 0, 0, 1 };
	Color terrainColor = { .2, .9, .2, 1 };
	Color cubeColor = { .9, 0, .9, 1 };

	axes->init(axesPosition);
	axes->setColor(axesColor);

	terrain.init("Models/mineCraftGround.obj");
	terrain.setColor(terrainColor);
	terrain.scale(100);
}

void World::setupTextures()
{
	
	// Texture Files
	textureFilenames[0] = "Textures/MineCraft.png";
	textureFilenames[1] = "Textures/MineCraftGround.png";

	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		textures[i] = new Texture();
		textures[i]->loadFromFile(textureFilenames[i]);
	}

	//cube.setTexture(textures[0]);
	terrain.setTexture(textures[1]);
	textures[0]->load();
	textures[1]->load();

}

void World::idleFunc()
{
	if (animateQueue.size() > 0)
	{
		switch (animateQueue.front())
		{
		case 1:
			animateBlockCreate();
			break;
		default:
			break;
		}
	}
}

void World::createBlock()
{
	PlaySound(TEXT("Sounds/blockCreate.wav"), NULL, SND_FILENAME);

	Model* temp = new Model();
	temp->init("Models/mineCraftCube.obj");
	temp->setTexture(textures[0]);
	
	temp->translate(0, -cam.getEyeY() + 1, -10);
	temp->updateTransform(glm::inverse(cam.getView()));
	cubes.push_back(temp);

	//animateQueue.push(1);
}

void World::animateBlockCreate()
{
	if (animate_amount == 0)
	{
		Model* temp = new Model();
		temp->init("Models/mineCraftCube.obj");
		temp->setTexture(textures[0]);
		temp->updateTransform(glm::inverse(cam.getView()));
		temp->translate(0, 10, -10);
		cubes.push_back(temp);

		cube_travel_dist = 10 + (-4);
	}

	cubes.back()->translate(0, -animate_factor * cube_travel_dist, 0);

	if (animate_amount == 100)
	{
		animate_amount = 0;
		animateQueue.pop();
	}
	
}