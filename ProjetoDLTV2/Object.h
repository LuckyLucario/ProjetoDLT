#ifndef OBJECT_H
#define OBJECT_H

/*
* Classe do objeto/obst�culo atirado em dire��o ao jogador
*/

//Bibliotecas e includes
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


class Object
{
public:

	//**Construtores e Destrutores**
	Object();
	~Object();

	//**getters**
	sf::Sprite getBody();
	//**setters**

	//**M�todos**
	void initializeObject();
	void moveObject(float dt);

private:

	sf::Sprite body; //Corpo improvisado pro nosso objeto
	int poskey; //determina a posi��o de spawn do objeto
	float Speed; //multiplicador de velocidade
	//float Speed;
	sf::Texture bodyImage; //textura do corpo do objeto


};

#endif //OBJECT_H

