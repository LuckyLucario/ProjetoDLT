
#ifndef GAME_H
#define GAME_H

/*
* A classe do jogo 
*/

//Bibliotecas e includes
#include <iostream>
#include "Collision.h"
#include "ObjectsQueue.h"
#include "ObjectsList.h"
#include "Player.h"

class Game
{
public:

	//**Construtores e destrutores**
	Game();
	virtual ~Game();

	//**Getters**
	const bool getIsWindowOpen();

	//**Métodos**
	void update(float dt); 
	void render(); 



private:
	//**Atributos**
	sf::RenderWindow* window;//Janela de Renderização
	sf::Event ev;//Variável de evento
	sf::VideoMode videoMode; //Dimensoes da janela

	ObjectsQueue objects; // Fila de Objetos
	ObjectsList spawnedObjects; // Lista de Objetos Ativos
	Player player; // Jogador

	//**Métodos privados**
	void initializeVariables();
	void initializeWindow();
	void initializeObjects();
	void initializePlayer();

	void renderObjects();
	void testCollisions();

	void updateObjects(float dt);
	void pollEvents();
	void generateQueue(int size);
	void spawnObject();

};

#endif // !GAME_H