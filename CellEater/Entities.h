#include "Hermite.h"
#pragma once

void crea_VAO_Vector(Figura* fig);
void crea_VAO_CP(Figura* fig);
void costruisci_mangiatore(vec4 color_top, vec4 color_bot, Figura* mangiatore);
void costruisci_cellula(vec4 color_top, vec4 color_bot, float posx, float posy, Figura* cellula);
void costruisci_sfondo(vec4 color_top, vec4 color_bot, Figura* cielo, int width, int height);
void costruisci_virus(vec4 color_top, vec4 color_bot, Figura* punte);