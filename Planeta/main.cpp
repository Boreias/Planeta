#include <Windows.h>
#include <glfw3.h>
#include <gl/GLU.h>
#include <iostream>
#include <vector>

typedef float cor[3];

cor branco = { 1.0, 1.0, 1.0 };
cor branco_gelo = { 0.88, 0.91, 0.89 };
cor azul = { 0.0, 0.15, 0.35 };
cor preto = { 0.0, 0.0, 0.0 };


static float angulo_estrelas = 0.0;
static float angulo_planeta = 0.0;

struct vec3
{
	float x, y, z;
	vec3() :x(0.0), y(0.0), z(0.0) {}
	vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}
};


void redimensiona(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500);

	glMatrixMode(GL_MODELVIEW);
}

void desenhaAstros(GLfloat raio, GLuint nStacks, GLuint nSectors, float* cor)
{
	std::vector< std::vector<GLuint> > indices;

	std::vector<vec3> pontos;
	const GLfloat PI = 3.14159265359;

	GLfloat deltaPhi = PI / nStacks;
	GLfloat deltaTheta = 2 * PI / nSectors;

	for (GLuint i = 0; i <= nStacks; i++)
	{
		GLfloat phi = -PI / 2.0 + i * deltaPhi;
		GLfloat temp = raio * cos(phi);
		GLfloat y = raio * sin(phi);

		std::vector<GLuint> pt;

		for (GLuint j = 0; j < nSectors; j++)
		{
			GLfloat theta = j * deltaTheta;
			GLfloat x = temp * sin(theta);
			GLfloat z = temp * cos(theta);

			pontos.push_back(vec3(x, y, z));
			GLuint index = pontos.size() - 1;
			pt.push_back(index);
		}

		indices.push_back(pt);
	}

	if (cor == branco)
	{
		glColor3fv(cor);
		glPointSize(2.5);
		glBegin(GL_POINTS);
		for (GLuint h = 0; h < pontos.size(); h++)
		{
			glVertex3fv(&pontos[h].x);
		}
		glEnd();
	}
	else
	{
		for (GLuint i = 0; i < nStacks; i++)
		{
			glColor3fv(cor);

			glBegin(GL_TRIANGLE_STRIP);

			for (GLuint j = 0; j < nSectors; j++)
			{
				GLuint index = indices[i][j];
				glVertex3fv(&pontos[index].x);
				index = indices[i + 1][j];
				glVertex3fv(&pontos[index].x);

				if (j == nSectors - 1)
				{
					index = indices[i][0];
					glVertex3fv(&pontos[index].x);
					index = indices[i + 1][0];
					glVertex3fv(&pontos[index].x);
				}
			}
			glEnd();
		}
	}
}

void desenha()
{
	glLoadIdentity();

	glTranslatef(-5.0, -3.0, -10.0);
	glRotatef(angulo_planeta, 1.0, 1.0, 0.0);
	glScalef(1.0, 1.0, 1.0);
	desenhaAstros(2.0, 20.0, 20.0, azul);

	glPushMatrix();
	glTranslatef(-5.0, -3.0, -10.0);
	glRotatef(angulo_estrelas, 1.0, 1.0, 0.0);
	glScalef(1.0, 1.0, 1.0);
	desenhaAstros(30.0, 20.0, 20.0, branco);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, 10.0, -100.0);
	glRotatef(angulo_planeta, 1.0, 1.0, 0.0);
	glScalef(1.0, 1.0, 1.0);
	desenhaAstros(5.0, 20.0, 20.0, branco_gelo);
	glPopMatrix();

	angulo_estrelas += 0.05;
	angulo_planeta -= 0.005;
}

int main()
{
	GLFWwindow* janela;

	const int LARGURA = 1200;
	const int ALTURA = 800;

	if (!glfwInit())
		return -1;

	janela = glfwCreateWindow(LARGURA, ALTURA, "Universo", NULL, NULL);
	if (!janela)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(janela);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(janela))
	{
		glfwPollEvents();

		if (glfwGetKey(janela, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(janela, GLFW_TRUE);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		desenha();

		redimensiona(LARGURA, ALTURA);

		glfwSwapBuffers(janela);
	}

	glfwTerminate();
	return 0;
}