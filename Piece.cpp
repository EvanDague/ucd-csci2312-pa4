#include "Piece.h"

namespace Gaming{
    
    Piece::Piece(const Game &g, const Position &p) :
        __game(g),
        __position(p)
    {
        
    }
    
    Piece::~Piece(){
        
    }
    
    std::ostream &operator<<(std::ostream &os, const Piece &piece){
        
    }
    
}