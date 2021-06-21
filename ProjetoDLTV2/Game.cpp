#include "Game.h"
#include <stdio.h>
#include <String>

//Variáveis globais
sf::Clock spawnTimer; //timer para spawnar os objetos

// --> Construtor e Inicializadores <--
Game::Game(){
	this->initializeVariables();
	this->initializeWindow();
	this->initializePlayer();
}
// --> Destrutor <--
Game::~Game(){
	delete this->window;
}

/*
* update():Atualiza os frames do jogo de acordo com cada estado:
*	State 0: Main Screen 
*	State 1: Gerando nova fase
*   State 2: Em jogo
*   State 3: Game over
*/
void Game::update(float dt){

	this->pollEvents(dt);

	//state 0
	if (this->state == 0){
		std::cout << "Estou na tela principal" << std::endl;
		//gera main screen

		//condição de saida : start game
		//this->state = 1;
	}
	//state 1
	if (this->state == 1){
		std::cout << "gerando objetos" << std::endl;
		this->pass = false;
		this->initializeObjects();
		

		//condição de saída: ter gerado os objetos
		this->state = 2;
	}

	//State 2
	if (this->state == 2){

		this->hp = this->player.getHP();
		this->updateObjects(dt);
		this->testCollisions();
		this->player.updateAll(dt);

		//condição de saída: finalizar a fase ou Zerar o hp
		if (this->player.getHP() == 0){
			this->state = 3;
		}
		else if (pass == true){
			std::cout << "YAY passou" << std::endl;
			this->level++;
			this->inGame = false;
			this->state = 1;
		}
	}

	//State 3
	if (this->state == 3) {
		std::cout << "Game Over" << std::endl;

		//condição de saida: Após confirmação do player, voltar para tela inicial
		this->state = 0;
	}
	
}

/*
* render(): Renderiza os objetos do jogo
* 	State 0: Main Screen 
*	State 1: Gerando nova fase
*   State 2: Em jogo
*   State 3: Game over
*/
void Game::render(){
	this->window->clear(sf::Color(19, 22, 28)); //limpa o frame antigo

	//state 0 
	if (this->state == 0){
		//renderiza mainScreen
	}
	
	//state 1
	if (this->state == 1) {
		//renderiza uma tela de loading?
	}

	//state 2
	if (this->state == 2) {

		this->player.renderAll(this->window); //Desenha o Player

		this->renderObjects();

		this->renderScore();

		this->window->display(); //Exibe na tela o desenho realizado no frame
	}

	//state 3
	if (this->state == 3) {
		//renderiza tela de Game over
	}

}


//**Getters**
//getIsWindowOpen(): Retorna true caso a janela do jogo esteja aberta, e false caso contrário
const bool Game::getIsWindowOpen(){
	return this->window->isOpen();
}

//**Outros metodos**

//InitializeVariables() : Inicializa as variáveis iniciais
void Game::initializeVariables(){
	this->window = nullptr;

	//this->state = 0; 
	this->state = 1; //para teste apenas
	this->level = 1;
	this->pass = false;
	this->inGame = false;

	this->objects.initializeQueue();
	this->spawnedObjects.initializeList();

	if(!this->font.loadFromFile("resources/Fonts/Rubik.ttf")){
		std::cout << "Falha no carregamento da fonte" << std::endl;
	}

	//Colocar numa função depois TODO:
	this->tScore.setFont(this->font);
	this->tHP.setFont(this->font);
	this->tHP.setPosition(this->videoMode.width/10*5,0);
	this->tScore.setPosition(this->videoMode.width/10*5, this->tHP.getCharacterSize()+10);
}

//InitializeWindow(): Inicializa a janela com as especificações necessárias
void Game::initializeWindow(){
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Projeto DLT", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

	
	//Carrega e seta a imagem do jogo
	sf::Image icon;
	if(!icon.loadFromFile("resources/icon.png")){ //Imagem quadrada 128x128
		std::cout<<"Falha na leitura do ícone" << std::endl;
	}
	this->window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

/*
* *initializeObjects(): Inicializa a fila de objetos a serem gerados e a lista de objetos spawnados
*/
void Game::initializeObjects(){

	int numObjs = this->level * 5;
	if (numObjs > 200) numObjs = 200;

	std::srand(time(0));  //inicializando seed aleatoria

	this->generateQueue(numObjs); //definir regra de geração pra queue (etapa de fase)
}

void Game::initializePlayer(){

	this->player.setPosition(videoMode.width/2, videoMode.height/2);
}

/*
* generateQueue(int size): Gera a fila de objetos de acordo com um parâmetro decidido no inicio de cada fase
*/
void Game::generateQueue(int size){

	for (int i = 0; i < size; i++){
		this->objects.newObject();
	}
}

//pollEvents(): Verifica eventos (Teclado, Janela aberta)
void Game::pollEvents(float dt){
	//Event polling
	while (this->window->pollEvent(this->ev)){
		if (this->ev.type == sf::Event::Closed) //Fecha a janela do jogo ao clicar no "X" da janela
			this->window->close();

		if (this->ev.type == sf::Event::KeyPressed){
			if(ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::W){
				player.rotateDirection('u', dt);
			}else if(ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::D){
				player.rotateDirection('r', dt);
			}else if(ev.key.code == sf::Keyboard::Down  || ev.key.code == sf::Keyboard::S){
				player.rotateDirection('d', dt);
			}else if(ev.key.code == sf::Keyboard::Left || ev.key.code == sf::Keyboard::A){
				player.rotateDirection('l', dt);
			}
		}

	}
}


// updateObjects(): Atualiza as informações dos objetos
void Game::updateObjects(float dt) {
	sf::Time delay = sf::seconds(0.4f); //delay entre spawns de objetos

	if (spawnTimer.getElapsedTime().asSeconds() >= delay.asSeconds()){
		spawnObject();
		spawnTimer.restart(); //restarta o timer de spawn de objetos
	}
	if(this->inGame == true){
		if (this->spawnedObjects.getNroElementos() != 0) { //se tiver elementos na lista de spawn, os movimenta
			for (int i = 0; i < this->spawnedObjects.getNroElementos(); i++) {
				this->spawnedObjects.getObjects()->moveObject(dt);
			}
		}
		else {
			this->pass = true;
		}
	}
	
}

/*
* testCollision(): Testa colisões entre obstáculos e o jogador
*/
void Game::testCollisions(){

	sf::Sprite testObject;
	for (int i = 0; i < this->spawnedObjects.getNroElementos(); i++){

		testObject = this->spawnedObjects.getObjects()->getBody();

		if(this->player.collidePlayer(testObject)){ // Colisão com player
			this->spawnedObjects.removeObject();
		} 
		else if(this->player.collideShields(testObject)){ // Colisão com escudos
			this->spawnedObjects.removeObject();
			this->score += 100;
		}
	}
}

/*
* spawnObject(): Checa se existem objetos a serem spawnados, e se houver, o coloca na lista de spawn
*/
void Game::spawnObject(){
	if (this->objects.isEmpty() == false){
		Object object = this->objects.removeObject();
		this->spawnedObjects.newObject(&object);
		this->inGame = true;
	}
}

//renderObjects(): Desenha os inimigos na tela
void Game::renderObjects(){
	for (int i = 0; i < this->spawnedObjects.getNroElementos(); i++){
		(this->window)->draw(this->spawnedObjects.getObjects()->getBody());
	}
	
}

//renderScore(): Desenha Pontuação e Vida
void Game::renderScore(){
	tHP.setString("HP: " + std::to_string(this->hp));
	tScore.setString("Score: " + std::to_string(this->score));
	this->window->draw(tHP);
	this->window->draw(tScore);
}


