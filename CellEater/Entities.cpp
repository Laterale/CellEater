#include "Hermite.h"

#define  PI   3.14159265358979323846

void crea_VAO_CP(Figura* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->CP.size() * sizeof(vec3), fig->CP.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colCP.size() * sizeof(vec4), fig->colCP.data(), GL_DYNAMIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void crea_VAO_Vector(Figura* fig)
{
	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->vertici.size() * sizeof(vec3), fig->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}
void costruisci_mangiatore(vec4 color_top, vec4 color_bot, Figura* man) 
{
	man->nTriangles = 90;
	int i;
	float stepA = (2 * PI) / man->nTriangles;
	float t;
	man->vertici.push_back(vec3(0.0, 0.0, 0.0));
	man->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

	for (i = 0; i <= man->nTriangles; i++)
	{
		t = (float)i * stepA;
		man->vertici.push_back(vec3(cos(t), sin(t), 0.0));
		//Colore 
		if (i % 3 == 0) {
			man->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
		}
		else {
			man->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
		}
	}
	man->nv = man->vertici.size();

	float xmax = 0.0;
	float xmin = 0.0;
	float ymax = 0.0;
	float ymin = 0.0;
	for (i = 1; i < man->nv; i++) {
		if (man->vertici[i].x < xmin)
			xmin = man->vertici[i].x;
	}
	for (i = 1; i < man->nv; i++) {
		if (man->vertici[i].x > xmax)
			xmax = man->vertici[i].x;
	}
	for (i = 1; i < man->nv; i++) {
		if (man->vertici[i].y <= ymin)
			ymin = man->vertici[i].y;
	}
	for (i = 1; i < man->nv; i++) {
		if (man->vertici[i].y > ymax)
			ymax = man->vertici[i].y;
	}
	//Aggiorno i valori del corner pi� in basso a sinistra (corner_b) e del corner pi� in alto a destra (conrner_t)
	man->corner_b_obj = vec4(xmin, ymin, 0.0, 1.0);
	man->corner_t_obj = vec4(xmax, ymax, 0.0, 1.0);
}
void costruisci_cellula(vec4 color_top, vec4 color_bot, float posx, float posy, Figura* cell) 
{
	cell->nTriangles = 20;
	int i;
	float stepA = (2 * PI) / cell->nTriangles;
	float t;
	cell->vertici.push_back(vec3(0.0, 0.0, 0.0));
	cell->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	cell->posx = posx;
	cell->posy = posy;

	for (i = 0; i <= cell->nTriangles; i++)
	{
		t = (float)i * stepA;
		cell->vertici.push_back(vec3(cos(t), sin(t), 0.0));
		//Colore 
		if (i % 3 == 0) {
			cell->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
		}
		else {
			cell->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
		}
	}
	cell->nv = cell->vertici.size();

	float xmax = 0.0;
	float xmin = 0.0;
	float ymax = 0.0;
	float ymin = 0.0;
	for (i = 1; i < cell->nv; i++) {
		if (cell->vertici[i].x < xmin)
			xmin = cell->vertici[i].x;
	}
	for (i = 1; i < cell->nv; i++) {
		if (cell->vertici[i].x > xmax)
			xmax = cell->vertici[i].x;
	}
	for (i = 1; i < cell->nv; i++) {
		if (cell->vertici[i].y <= ymin)
			ymin = cell->vertici[i].y;
	}
	for (i = 1; i < cell->nv; i++) {
		if (cell->vertici[i].y > ymax)
			ymax = cell->vertici[i].y;
	}
	//Aggiorno i valori del corner pi� in basso a sinistra (corner_b) e del corner pi� in alto a destra (conrner_t)
	cell->corner_b_obj = vec4(xmin, ymin, 0.0, 1.0);
	cell->corner_t_obj = vec4(xmax, ymax, 0.0, 1.0);
}
void costruisci_sfondo(vec4 color_top, vec4 color_bot, Figura* sfondo, int width, int height) 
{
	sfondo->vertici.push_back(vec3(0.0, 0.0, 0.0));
	sfondo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	sfondo->vertici.push_back(vec3(0.0, 1.0, 0.0));
	sfondo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	sfondo->vertici.push_back(vec3(1.0, 0.0, 0.0));
	sfondo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	sfondo->vertici.push_back(vec3(1.0, 1.0, 0.0));
	sfondo->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
	sfondo->Model = mat4(1.0);
	sfondo->Model = scale(sfondo->Model, vec3(float(width), float(height), 1.0));
	sfondo->nv = sfondo->vertici.size();
	sfondo->sceltaFs = 1;
}
void costruisci_virus(vec4 color_top, vec4 color_bot, Figura* virus) {
	Hermite herm;
	herm.costruisci_curva(10.0, 10.0, virus);
	herm.costruisci_curva(8.0, 8.0, virus);
	herm.costruisci_curva(10.0, 9.0, virus);
	herm.costruisci_curva(11.0, 8.0, virus);
	herm.costruisci_curva(11.0, 11.0, virus);
	herm.costruisci_curva(12.0, 10.0, virus);
	herm.costruisci_curva(12.0, 12.0, virus);
	herm.costruisci_curva(10.0, 12.0, virus);
	herm.costruisci_curva(9.0, 11.0, virus);
	herm.costruisci_curva(9.0, 10.0, virus);
	herm.costruisci_formaHermite(color_top, color_bot, virus);
	virus->mover = (rand() % 4) + 1.0;
	virus->sceltaFs = 0;
	virus->corner_b_obj = vec4(8.0, 8.0, 0.0, 1.0);
	virus->corner_t_obj = vec4(12.0, 12.0, 0.0, 1.0);
}