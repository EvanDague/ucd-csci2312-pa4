#include <iostream>
#include "Game.h"
#include "Simple.h"
#include "Agent.h"
#include "Piece.h"
#include "Strategy.h"
#include "Strategic.h"
#include "Exceptions.h"
#include "Food.h"
#include "Advantage.h"
#include "Gaming.h"

using namespace std;

namespace Gaming{
    
    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;
    PositionRandomizer Game::__posRandomizer;
    
    Game::Game(){
        __round = 0;
        __numInitAgents = 0;
        __numInitResources = 0;
        __width = MIN_WIDTH;
        __height = MIN_HEIGHT;
        for(unsigned i=0; i < __width*__height; i++){
            __grid.push_back(nullptr);
        }
    }
    Game::Game(unsigned width, unsigned height, bool manual /*= true*/){
        if(width >= MIN_WIDTH && height >= MIN_HEIGHT){
            __round = 0;
            __width = width;
            __height = height;
            __numInitAgents = 0;
            __numInitResources = 0;
            for(unsigned i=0; i < __width*__height; i++){
                __grid.push_back(nullptr);
            }
            if(!manual){
                populate();
            }
        }
        else{
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }
    }
    Game::~Game(){
        for(auto it = __grid.begin(); it != __grid.end(); ++it){
            if(*it != nullptr){
                delete *it;
                *it = nullptr;
            }
        }
        __grid.clear();
    }

    void Game::populate(){
        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
        unsigned int numStrategic = __numInitAgents / 2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources / 4;
        unsigned int numFoods = __numInitResources - numAdvantages;
        
        if(__numInitResources%2 == 1)
            numFoods++;
        
        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width*__height);
        
        while (numStrategic > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
                numStrategic --;
            }
        }    
        while (numSimple > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, STARTING_AGENT_ENERGY);
                numSimple --;
            }
        }
        while (numAdvantages > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantages --;
            }
        }
        while (numFoods > 0) {
            int i = d(gen); // random index in the grid vector
            if (__grid[i] == nullptr) { // is position empty
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFoods --;
            }
        } 
    }


    unsigned int Game::getNumPieces() const{
        return(this->getNumAgents() + this->getNumResources());
    }
    unsigned int Game::getNumAgents() const{
        return (this->getNumSimple() + this->getNumStrategic());
    }
    unsigned int Game::getNumSimple() const{
        unsigned count = 0;
        
        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Simple *simple = dynamic_cast<Simple*>(*it);
            if (simple ) count ++;
        }
        
        return count;
    }
    unsigned int Game::getNumStrategic() const{
        unsigned count = 0;
        
        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Strategic *strategic = dynamic_cast<Strategic*>(*it);
            if (strategic) count ++;
        }
        
        return count;
    }
    unsigned int Game::getNumResources() const{
        unsigned count = 0;
        
        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Food *food = dynamic_cast<Food*>(*it);
            if (food) count ++;
            
            Advantage *advantage = dynamic_cast<Advantage*>(*it);
            if (advantage) count ++;
        }
        return count;
    }
    
    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        if(__grid[y*__width + x] != nullptr)
            return __grid[y*__width + x];
        throw PositionEmptyEx(x, y);
    }

    
    void Game::addSimple(const Position &position){
        Simple *newSimple = new Simple(*this, position, STARTING_AGENT_ENERGY);
        
        if((position.y*__width + position.x)>__grid.size() || position.x > __width)
            throw OutOfBoundsEx(__width,__height,position.x,position.y);

        if((__grid[position.y*__width + position.x])!=nullptr)
            throw PositionNonemptyEx(position.x,position.y);
        
        __grid[position.y*__width + position.x] = newSimple;
    }
    void Game::addSimple(const Position &position, double energy){
        Simple *newSimple = new Simple(*this, position, STARTING_AGENT_ENERGY);
        
        if((position.y*__width + position.x)>__grid.size() || position.x > __width)
            throw OutOfBoundsEx(__width,__height,position.x,position.y);

        if((__grid[position.y*__width + position.x])!=nullptr)
            throw PositionNonemptyEx(position.x,position.y);
        
        __grid[position.y*__width + position.x] = newSimple;
    }
    void Game::addSimple(unsigned x, unsigned y){
        Position pos(x, y);
        
        this->addSimple(pos);
    }
    void Game::addSimple(unsigned x, unsigned y, double energy){
        Position pos(x, y);
        
        this->addSimple(pos, energy);
    }
    void Game::addStrategic(const Position &position, Strategy *s/* = new DefaultAgentStrategy()*/){
        Strategic *newStrat = new Strategic(*this, position, STARTING_AGENT_ENERGY,s);
        
        if((position.y*__width + position.x)>__grid.size() || position.x > __width)
            throw OutOfBoundsEx(__width,__height,position.x,position.y);

        if((__grid[position.y*__width + position.x])!=nullptr)
            throw PositionNonemptyEx(position.x,position.y);
        
        __grid[position.y*__width + position.x] = newStrat;
    }
    void Game::addStrategic(unsigned x, unsigned y, Strategy *s/* = new DefaultAgentStrategy()*/){
        Position pos(x, y);
        
        this->addStrategic(pos, s);
    }
    void Game::addFood(const Position &position){
        Food *newFood = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
        
        if((position.y*__width + position.x)>__grid.size() || position.x > __width)
            throw OutOfBoundsEx(__width,__height,position.x,position.y);

        if((__grid[position.y*__width + position.x]) != nullptr)
            throw PositionNonemptyEx(position.x,position.y);
        
        __grid[position.y*__width + position.x] = newFood;
    }
    void Game::addFood(unsigned x, unsigned y){
        Position pos(x, y);
        
        this->addFood(pos);
    }
    void Game::addAdvantage(const Position &position){
        Advantage *newAdvantage = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
        
        if((position.y*__width + position.x)>__grid.size() || position.x > __width)
            throw OutOfBoundsEx(__width,__height,position.x,position.y);

        if((__grid[position.y*__width + position.x])!=nullptr)
            throw PositionNonemptyEx(position.x,position.y);
        
        __grid[position.y*__width + position.x] = newAdvantage;
    }
    void Game::addAdvantage(unsigned x, unsigned y){
        Position pos(x, y);
        
        this->addAdvantage(pos);
    }
    const Surroundings Game::getSurroundings(const Position &pos) const{
        Surroundings *surround = new Surroundings;
        int count;
        
        for(int i=-1; i < 2; i++){
            for(int j=-1; j < 2; j++){
                
                if(pos.x+j < 0 || pos.x+j >= __width || pos.y+i < 0 || pos.y+i >= __width){
                    surround->array[count] = INACCESSIBLE;
                }
                else if(count == 4){
                    surround->array[count] = SELF;
                }
                else if(__grid[__width*(pos.y - i) + (pos.x + j)] == nullptr){
                    surround->array[count] = EMPTY;
                }
                else{
                    surround->array[count] = __grid[__width*(pos.y - i) + (pos.x + j)]->getType();
                }
                count++;
            }
        }
        
        return *surround;
    }
    
    const ActionType Game::reachSurroundings(const Position &from, const Position &to){
        int xDist = to.x - from.x;
        int yDist = to.y - from.y;
        ActionType move = STAY;
        
        if(xDist == -1 && yDist == -1){
            move = NW;
        }else if(xDist == 0 && yDist == -1){
            move = N;
        }else if(xDist == 1 && yDist == -1){
            move = NE;
        }else if(xDist == -1 && yDist == 0){
            move = W;
        }else if(xDist == 1 && yDist == 0){
            move = E;
        }else if(xDist == -1 && yDist == 1){
            move = SW;
        }else if(xDist == 0 && yDist == 1){
            move = S;
        }else if(xDist == 1 && yDist == 1){
            move = SE;
        }
        
        return move;
    }
    
    bool Game::isLegal(const ActionType &ac, const Position &pos) const{
        Surroundings surround = getSurroundings(pos);
        ActionType moves[9] =  {NW,N,NE,W,STAY,E,SW,S,SE};
        int direction;
        bool legal;
        for (int i = 0; i < 9; ++i) {
            if(moves[i] == ac) {
                direction = i;
                break;
            }
        }
        legal = surround.array[direction] != INACCESSIBLE;

        return legal;
    }
    const Position Game::move(const Position &pos, const ActionType &ac) const{
        Position newPos;

        if(ac == NW)
            newPos = Position(pos.x-1,pos.y-1);
        else if(ac == N)
            newPos = Position(pos.x-1,pos.y);
        else if(ac == NE)
            newPos = Position(pos.x-1,pos.y+1);
        else if(ac == W)
            newPos = Position(pos.x,pos.y-1);
        else if(ac == STAY)
            newPos = Position(pos.x,pos.y);
        else if(ac == E)
            newPos = Position(pos.x,pos.y+1);
        else if(ac == SW)
            newPos = Position(pos.x+1,pos.y-1);
        else if(ac == S)
            newPos = Position(pos.x+1,pos.y);
        else if(ac == SE)
            newPos = Position(pos.x+1,pos.y+1);

        return newPos;
    }
    void Game::round(){
        
        if(__round == 0 && __verbose){
            __status = PLAYING;
            cout << endl << *this;
        }
        for (int i = 0; i < __grid.size(); ++i) {
            if(__grid[i]!= nullptr){
                if(__grid[i]->isViable()) {
                    if (!__grid[i]->getTurned()) {
                        Agent * agent = dynamic_cast<Agent*>(__grid[i]);
                        if(agent) {
                            __grid[i]->setTurned(true);
                            Position currentPos = __grid[i]->getPosition();
                            Surroundings surround = getSurroundings(currentPos);
                            ActionType action = __grid[i]->takeTurn(surround);
                            if (action != STAY) {
                                Position newPos = move(currentPos, action);
                                int newPosIndx = (newPos.x * __width + newPos.y);
                                (*__grid[i]) * (*__grid[newPosIndx]);
                                if(!__grid[i]->isViable()){
                                    delete __grid[i];
                                    __grid[i]= nullptr;
                                }
                                else {
                                    __grid[i]->setPosition(newPos);
                                    if (__grid[newPosIndx] != nullptr) {
                                        delete __grid[newPosIndx];
                                        __grid[newPosIndx] = __grid[i];
                                        __grid[i] = nullptr;
                                    }
                                    else {
                                        __grid[newPosIndx] = __grid[i];
                                        __grid[i] = nullptr;
                                    }
                                }
                                if(!__grid[newPosIndx]->isViable()){
                                    delete __grid[newPosIndx];
                                    __grid[newPosIndx]= nullptr;
                                }
                            }
                        }
                    }
                }
            }
        }
        for (int j = 0; j < __grid.size(); ++j) {
            if(__grid[j] != nullptr) {
                if (!__grid[j]->isViable()) {
                    delete __grid[j];
                    __grid[j] = nullptr;
                }
                else {
                    __grid[j]->setTurned(false);
                    __grid[j]->age();
                }
            }
        }
        if(getNumPieces()< 2 || getNumResources() < 1)
            __status = OVER;
        ++__round; //

        if(__verbose)
            cout << endl << *this;

    }
    void Game::play(bool verbose/* = false*/){
        __status = PLAYING;
        __verbose = verbose;
        cout << *this;
        while (__status != OVER) {
            round();
            if (verbose)
                cout << *this;
        }
        if (!verbose)
            cout << *this;
        
    }
    
    std::ostream &operator<<(std::ostream &os, const Game &game){
        int count = 0;
        
        os << "Round " << game.getRound() << endl;
        
        for(int i=0; i < game.getHeight(); i++){
            for(int j=0; j < game.getWidth(); j++){
                if(game.__grid[count] == nullptr){
                    os << "[" << "     " << "]";
                }
                else{
                    os << "[" << *game.__grid[count] << "]";
                }
                os << " ";
                count++;
            }
            os << endl;
        }
        
        if(game.__status == Game::PLAYING){
            os << "Status: Playing..";
        }else if(game.__status == Game::NOT_STARTED){
            os << "Status: Not Started";
        }else{
            os << "Status: Over!";
        }
        
        return os;
    }
    
}