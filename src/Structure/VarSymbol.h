//
// Created by chasi on 2018/12/9.
//

#ifndef CMMCOMPILER_VARSYMBOL_H
#define CMMCOMPILER_VARSYMBOL_H

#include "AbstractSymbol.h"

class VarSymbol : public AbstractSymbol {
public:

    VarSymbol();

    VarSymbol(const std::string &name, SymbolType type, _dataSize line, int level,
              _dataSize _length = 0);

    VarSymbol(_dataSize valueIndex, SymbolType type = SymbolType::REG);

    ~VarSymbol() override;

    _dataSize getDataSize() const;

    void setDataSize(_dataSize dataSize);

    _dataSize length() const;

    void setLength(_dataSize _length);

    _dataSize getValueIndex() const;

    void setValueIndex(_dataSize valueIndex);

private:
    _dataSize dataSize;
    _dataSize _length;
    _dataSize valueIndex;
};


#endif //CMMCOMPILER_VARSYMBOL_H
