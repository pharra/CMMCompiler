//
// Created by chasi on 2018/12/9.
//

#include "ClassSymbol.h"

ClassSymbol::~ClassSymbol() {
};

ClassSymbol::ClassSymbol(const std::string &name, SymbolType type, _dataSize line, int level) : AbstractSymbol(name,
                                                                                                               type,
                                                                                                               line,
                                                                                                               level) {

}

Object *ClassSymbol::getObject() const {
    return object;
}

void ClassSymbol::setObject(Object *object) {
    ClassSymbol::object = object;
}

bool ClassSymbol::addAttr(std::string name, Value &value) {
    auto result = attrs.insert(std::map<std::string, Value>::value_type(name, value));
    return result.second;
}

const Value *ClassSymbol::getAttr(std::string name) const {
    auto i = attrs.find(name);
    if (i != attrs.end()) {
        return &(i->second);
    }
    return nullptr;
}
