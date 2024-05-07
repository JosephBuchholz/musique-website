#ifndef MUSIQUE_BASEELEMENT_H
#define MUSIQUE_BASEELEMENT_H

#include <string>

#include "../Types.h"

// the base class of all notation elements
class BaseElement {

public:
    enum class ElementType {
        None = 0, CSLyric, CSChord, CSMeasure, CSStaff, Credit
    };

public:
    BaseElement() {}
    BaseElement(ElementType type)
        : elementType(type) {}
    virtual ~BaseElement() {}
    
    virtual void Delete() { ASSERT(false); }

public:
    std::string id = "";
    
    ElementType elementType = ElementType::None;
    
    Color selectedColor = Color();
};

#endif // MUSIQUE_BASEELEMENT_H