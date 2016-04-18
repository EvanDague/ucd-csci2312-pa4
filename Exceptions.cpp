#include "Exceptions.h"

using namespace std;

namespace Gaming{
    
    //Gaming Exception
    std::ostream &operator<<(std::ostream &os, const GamingException &ex){
        os << ex.getName() << endl;
        return os;
    }
    
    void GamingException::setName(std::string name){
        __name = name;
    }
    //DimensionEx
    
    DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height) :
        __exp_width(expWidth), __exp_height(expHeight), __width(width), __height(height)
    {
        __exp_width = expWidth;
        __exp_height = expHeight;
        __width = width;
        __height = height;
        setName("DimensionsEx");
    }
    unsigned DimensionEx::getExpWidth() const{
        return __exp_width;
    }
    unsigned DimensionEx::getExpHeight() const{
        return __exp_height;
    }
    unsigned DimensionEx::getWidth() const{
        return __width;
    }
    unsigned DimensionEx::getHeight() const{
        return __height;
    }
    
    //InsufficientDimensionsEx

    void InsufficientDimensionsEx::__print_args(std::ostream &os) const{
        os << "Name: " << getName();
        os << "ExpHeight and Height: " << getExpHeight() << ", " << getHeight() << endl;
        os << "ExpWidth and Width: " << getExpWidth() << ", " << getWidth() << endl;
    }
    
    InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight, unsigned width, unsigned height) :
        DimensionEx(minWidth, minHeight, width, height)
    {
        setName("InsufficientDimensionsEx");
    }
    
    //OutOfBoundsEx
    
    void OutOfBoundsEx::__print_args(std::ostream &os) const{
        os << getName() << ": " << getExpHeight() << " < " <<  getHeight() << " or " << getExpWidth() << " < " << getWidth() << endl;
    }
    
    OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width, unsigned height) :
        DimensionEx(maxWidth, maxHeight, width, height)
    {
        setName("OutOfBoundsEx");
    }
    
    //PositionEx
    
    void PositionEx::__print_args(std::ostream &os) const{
        os << getName() << ": " << "(" << __x << "," << __y << ")" << endl;
    }
    PositionEx::PositionEx(unsigned x, unsigned y) :
        GamingException(), __x(x), __y(y)
    {
        __x = x;
        __y = y;
        setName("PositionEx");
    }
    
    //PositionNonemptyEx
    PositionNonemptyEx::PositionNonemptyEx(unsigned x, unsigned y) :
        PositionEx(x, y)
    {
        setName("PositionNonemptyEx");
    }
    
    //PositionEmptyEx
    PositionEmptyEx::PositionEmptyEx(unsigned x, unsigned y) :
        PositionEx(x, y)
    {
        setName("PositionEmptyEx");
    }
    
    //PosVectorEmptyEx
    
    void PosVectorEmptyEx::__print_args(std::ostream &os) const{
        os << getName() << endl;
    }
    PosVectorEmptyEx::PosVectorEmptyEx() :
        GamingException()
    {
        setName("PosVectorEmptyEx");
    }

}