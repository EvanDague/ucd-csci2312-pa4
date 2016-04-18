#include "Strategic.h"

namespace Gaming{
    
    const char Strategic::STRATEGIC_ID = 'T';
    
    Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s /*= new DefaultAgentStrategy()*/) :
        Agent(g, p, energy),
        __strategy(s)
    {
        __strategy = nullptr; 
    }
    Strategic::~Strategic(){
            
    }
    
    void Strategic::print(std::ostream &os) const{
         os << STRATEGIC_ID << __id;
    }

    ActionType Strategic::takeTurn(const Surroundings &s) const{ 
        return (*__strategy)(s); 
    }
    
}