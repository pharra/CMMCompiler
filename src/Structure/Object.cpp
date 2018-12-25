//
// Created by chasi on 2018/12/18.
//

#include "Object.h"

_LL Object::getRefCount() const {
    return refCount;
}

void Object::setRefCount(_LL refCount) {
    Object::refCount = refCount;
}

const std::string &Object::getTypeName() const {
    return typeName;
}

void Object::setTypeName(const std::string &typeName) {
    Object::typeName = typeName;
}

const std::map<std::string, _LL> &Object::getMethods() const {
    return methods;
}

const std::map<std::string, _LL> &Object::getAttrs() const {
    return attrs;
}

_LL Object::getAttr(std::string name) {
    auto i = attrs.find(name);
    if (i != attrs.end()) {
        return i->second;
    }
    return -1;
}

_LL Object::getMethod(std::string name) {
    auto i = methods.find(name);
    if (i != methods.end()) {
        return i->second;
    }
    return -1;
}

bool Object::addAttr(std::string name, _LL index) {
    auto result = attrs.insert(std::map<std::string, _LL>::value_type(name, index));
    return result.second;
}

bool Object::addMethod(std::string name, _LL index) {
    auto result = methods.insert(std::map<std::string, _LL>::value_type(name, index));
    return result.second;
}

Object::Object(const std::string &typeName) : typeName(typeName) {

}
