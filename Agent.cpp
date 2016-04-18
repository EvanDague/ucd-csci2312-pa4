#include "Agent.h"
#include "Resource.h"

namespace Gaming{
    
    const double Agent::AGENT_FATIGUE_RATE = 0.3;
    
    Agent::Agent(const Game &g, const Position &p, double energy) :
        Piece(g, p),
        __energy(energy)
    {
        
    }
    
    Agent::~Agent(){
        
    }
    
    void Agent::age(){
        __energy -= AGENT_FATIGUE_RATE;
    }
    
    Piece &Agent::operator*(Piece &other){
        return other.interact(this);
    }
    
    Piece &Agent::interact(Agent *agent){
        if(getEnergy() < agent->getEnergy()) {
            finish();
            __energy = 0;
        }
        return *this;
    }
    Piece &Agent::interact(Resource *resource){
        addEnergy(resource->consume());
        return *this;
    }
    
}