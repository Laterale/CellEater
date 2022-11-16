#include <iostream>
#include <GL/glew.h>
#include "ShaderMaker.h"
#include "lib.h"
#include <GL/freeglut.h>
#include <random>
#include <time.h>
#include <stdlib.h>
#include "Hermite.h"
#include "Entities.h"

#define  PI   3.14159265358979323846
static unsigned int programId;
int score = 0;
mat4 Projection;
int width = 1280, height = 720;
GLuint MatProj, MatModel;
float  dx = 0, dy = 0, virus_size = 0, grossezza = 0.0, w_update, h_update, w_window, h_window;
int adder = 1;
Figura Sfondo, Mangiatore, Virus, Cellula;
vector<Figura> Scena;


void updateVirus(int value) 
{
	for (int i = 42; i < 56; i++) {
		if (Scena[i].posx >= width - 60 || Scena[i].posx < -60) {
			Scena[i].mover = -Scena[i].mover;
		}
		Scena[i].posx += Scena[i].mover;
	}
	glutTimerFunc(10, updateVirus, 0);
	glutPostRedisplay();
	
}


bool checkCollision(Figura obj1, Figura obj2) {
	// guardo collisioni su asse x
	bool collisionX = obj1.corner_b.x <= obj2.corner_t.x && obj1.corner_t.x >= obj2.corner_b.x;

	// guardo collisioni su asse y
	bool collisionY = obj1.corner_b.y <= obj2.corner_t.y && obj1.corner_t.y >= obj2.corner_b.y;

	//Si ha collisione se c'� collisione sia nella direzione x che nella direzione y
	return collisionX && collisionY;
}

void INIT_VAO(void)
{
	//Costruisco lo sfondo
	vec4 col_top = vec4{ 1.0, 0.0, 1.0, 1.0};
	vec4 col_bottom = vec4{ 1.0, 0.0, 0.0, 1.0};
	costruisci_sfondo(col_top, col_bottom, &Sfondo, width, height);
	crea_VAO_Vector(&Sfondo);
	Scena.push_back(Sfondo);

	//costruisco il mangiatore
	//vec4{ 1.0, 0.81, 0.86, 0.0 };
    col_top = vec4{ 1.0, 0.81, 0.80, 0.0 };
	col_bottom = vec4{ 0.0, 0.0, 0.0, 0.0 };
	costruisci_mangiatore(col_top, col_bottom, &Mangiatore);
	crea_VAO_Vector(&Mangiatore);
	Mangiatore.active = true;
	Scena.push_back(Mangiatore);

	//costruisco le cellule
	for (int i = 0; i < 40; i++) {
		col_top = vec4{ 1.0, 1.0, 1.0, 1.0 };
		col_bottom = vec4{ 1.0, 1.0, 1.0, 1.0 };
		float posx = (float)width - (rand() % width);
		float posy = (float)height - (rand() % height);
		costruisci_cellula(col_top, col_bottom, posx, posy, &Cellula);
		crea_VAO_Vector(&Cellula);
		Scena.push_back(Cellula);
		Scena[i+2].active = true;
	}
	//costruisco i virus
	for (int i = 0; i < 15; i++) {
		col_top = vec4{ 1.0, 0.0, 0.0, 1.0 };
		col_bottom = vec4{ 1.0, 0.0, 0.0, 1.0 };
		float posx = (float)width - (rand() % width);
		float posy = (float)height - (rand() % height);
		costruisci_virus(col_top, col_bottom, &Virus);
		Virus.posx = posx;
		Virus.posy = posy;
		crea_VAO_Vector(&Virus);
		Scena.push_back(Virus);
	}

	MatProj = glGetUniformLocation(programId, "Projection");
	MatModel = glGetUniformLocation(programId, "Model");
	glViewport(0, 0, width, height);
	Projection = ortho(0.0f, float(width), 0.0f, float(height));
}

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader.glsl";
	char* fragmentShader = (char*)"fragmentShader.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}

void drawScene(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	float time = glutGet(GLUT_ELAPSED_TIME) / 1500.0f;
	vec2 resolution = vec2(width, height);
	glUniform1f(glGetUniformLocation(programId, "iTime"), time);
	glUniform2f(glGetUniformLocation(programId,"resolution"), resolution.x, resolution.y);

	//disegno lo sfondo
	glBindVertexArray(Scena[0].VAO);
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[0].Model));
	glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, Scena[0].nv);
	glBindVertexArray(0);

	if (Scena[1].active) {
		//disegno il mangiatore
		Scena[1].Model = mat4(1.0);
		Scena[1].Model = translate(Scena[1].Model, vec3((float)width / 2.0 + dx, dy + 100, 0.0f));
		Scena[1].Model = scale(Scena[1].Model, vec3(25.0 + grossezza , 20.0 + grossezza, 1.0));
		Scena[1].corner_b = Scena[1].corner_b_obj;
		Scena[1].corner_t = Scena[1].corner_t_obj;
		Scena[1].corner_b = Scena[1].Model * Scena[1].corner_b;
		Scena[1].corner_t = Scena[1].Model * Scena[1].corner_t;
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[1].Model));
		glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 2);
		glBindVertexArray(Scena[1].VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[1].vertici.size());
		glBindVertexArray(0);
	}
	//disegno le cellule
	glUniform1i(glGetUniformLocation(programId, "sceltaFS"), 0);
	for (int i = 2; i <= 42 - 1; i++) {
		if (checkCollision(Scena[1], Scena[i]) && Scena[i].active) {
			score++;
			Scena[i].active = false;
			grossezza += 1;

			//checkGameOver();
		}
		if (Scena[i].active == true) {
			Scena[i].Model = mat4(1.0);
			Scena[i].Model = translate(Scena[i].Model, vec3((float)Scena[i].posx, (float)Scena[i].posy, 0.0f));
			Scena[i].Model = scale(Scena[i].Model, vec3(4, 4, 1.0));
			Scena[i].corner_b = Scena[i].corner_b_obj;
			Scena[i].corner_t = Scena[i].corner_t_obj;
			Scena[i].corner_b = Scena[i].Model * Scena[i].corner_b;
			Scena[i].corner_t = Scena[i].Model * Scena[i].corner_t;
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
			glBindVertexArray(Scena[i].VAO);
			glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[i].vertici.size());
			glBindVertexArray(0);
		}
	}
	//disegno i virus
	for (int i = 42; i < 56; i++) {
		if (checkCollision(Scena[1], Scena[i])) {
			score++;
			Scena[1].active = false;
		}
		Scena[i].Model = mat4(1.0);
		Scena[i].Model = translate(Scena[i].Model, vec3((float)Scena[i].posx + Scena[i].mover, (float)Scena[i].posy, 0.0f));
		Scena[i].Model = scale(Scena[i].Model, vec3(6.0, 6.0, 1.0));
		Scena[i].corner_b = Scena[i].corner_b_obj;
		Scena[i].corner_t = Scena[i].corner_t_obj;
		Scena[i].corner_b = Scena[i].Model * Scena[i].corner_b;
		Scena[i].corner_t = Scena[i].Model * Scena[i].corner_t;
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Scena[i].Model));
		glBindVertexArray(Scena[i].VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, Scena[i].vertici.size());
		glBindVertexArray(0);
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	w_window = (float)w;
	h_window = (float)h;
	Projection = ortho(0.0f, (float)width, 0.0f, (float)height);
	float AspectRatio_mondo = (float)(width) / (float)(height); 

	if (AspectRatio_mondo > w / h){
		w_update = (float)w;
		h_update = w / AspectRatio_mondo;
		glViewport(0, 0, w_update, h_update);
	}
	else {
		w_update = h * AspectRatio_mondo;
		h_update = (float)h;
		glViewport(0, 0, w_update, h_update);

	}
}


void myKeyboard(unsigned char key, int x, int y)
{
	{
		switch (key)
		{
		case 'a':
			dx -= 5;
			break;
		case 'd':
			dx += 5;
			break;

		case 'w':
			dy += 5;
			break;
		case 's':
			dy -= 5;
			break;
		default:
			break;
		}
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	srand(time(NULL));
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(200, 60);
	glutCreateWindow("Cell Eater");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(20, updateVirus, 0);
	glutReshapeFunc(reshape);

	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();

	glutMainLoop();
}