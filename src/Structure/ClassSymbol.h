//
// Created by chasi on 2018/12/9.
//

#ifndef CMMCOMPILER_CLASSSYMBOL_H
#define CMMCOMPILER_CLASSSYMBOL_H

#include <map>

#include "AbstractSymbol.h"
#include "Object.h"
#include "Value.h"

class ClassSymbol : public AbstractSymbol {
public:
    ~ClassSymbol() override;

    ClassSymbol(const std::string &name, SymbolType type, _dataSize line, int level);

    Object *getObject() const;

    void setObject(Object *object);

    bool addAttr(std::string name, Value &value);

    const Value *getAttr(std::string name) const;

private:
    std::map<std::string, Value> attrs;
    Object *object;
};


#endif //CMMCOMPILER_CLASSSYMBOL_H
