//
// Created by chasi on 2018/12/18.
//

#ifndef CMMCOMPILER_OBJECT_H
#define CMMCOMPILER_OBJECT_H

#include <map>

#include "Utils/Common.h"


class Object {
public:
    Object(const std::string &typeName);

    _LL getRefCount() const;

    void setRefCount(_LL refCount);

    const std::map<std::string, _LL> &getMethods() const;

    const std::map<std::string, _LL> &getAttrs() const;

    bool addMethod(std::string name, _LL index);

    bool addAttr(std::string name, _LL index);

    _LL getMethod(std::string name);

    _LL getAttr(std::string name);

    const std::string &getTypeName() const;

    void setTypeName(const std::string &typeName);

private:
    std::map<std::string, _LL> methods;
    std::map<std::string, _LL> attrs;
    _LL refCount;
    std::string typeName;
};

extern std::map<std::string, Object *> globalClassMap;
#endif //CMMCOMPILER_OBJECT_H
