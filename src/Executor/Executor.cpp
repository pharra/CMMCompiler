//
// Created by chasi on 2018/11/28.
//

#include "Executor.h"

Executor::Executor(long long int programEntry, std::vector<Quaternary> &quaternaryVector) {
    this->programEntry = programEntry;
    this->quaternaryVector = quaternaryVector;
    nextQeVecIndex = programEntry;
    ebp = 0;
    esp = 1;
    level = 0;
    addrMap[0] = (PointAddress) DATA_ADDR;
    curDataAddr = (PointAddress) DATA_ADDR;
    tempMap.clear();
    constVec.clear();
    constMap.clear();
    curConstAddr = (PointAddress) CONST_ADDR;
    globalVec.clear();
    globalMap.clear();
    curGlobalAddr = (PointAddress) GLOBAL_ADDR;
    valueVec.clear();
    valueVec.push_back(Value("$" + std::to_string(0x00000000), ValueType::ADDR));
    isOver = false;
    for (auto i:globalVarVec) {
        nextQeVecIndex = i;
        executeStmt();
    }
    nextQeVecIndex = programEntry;
}

void Executor::execute() {
    try {
        isOver = false;
        while (!isOver)
            executeStmt();
    } catch (ExecutorException &cee) {
        std::cout << cee.message() << std::endl;
        return;
    }
}

void Executor::executeStmt() {
    Quaternary q = quaternaryVector.at(nextQeVecIndex);
    curExecuteStmtLineNo = q.getLine();
    nextQeVecIndex++;
    std::string type = q.getOp();
    if (type == Quaternary::JMP) {
        executeJmp(q);
    } else if (type == Quaternary::READ) {
        executeRead(q);
    } else if (type == Quaternary::WRITE) {
        executeWrite(q);
    } else if (type == Quaternary::IN) {
        executeIn(q);
    } else if (type == Quaternary::OUT) {
        executeOut(q);
    } else if (type == Quaternary::INT) {
        executeInt(q);
    } else if (type == Quaternary::CHAR) {
        executeChar(q);
    } else if (type == Quaternary::REAL) {
        executeReal(q);
    } else if (type == Quaternary::ARRAY) {
        executeArray(q);
    } else if (type == Quaternary::ASSIGN) {
        executeAssign(q);
    } else if (type == Quaternary::PLUS) {
        executePlus(q);
    } else if (type == Quaternary::MINUS) {
        executeMinus(q);
    } else if (type == Quaternary::MUL) {
        executeMul(q);
    } else if (type == Quaternary::DIV) {
        executeDiv(q);
    } else if (type == Quaternary::GT) {
        executeGt(q);
    } else if (type == Quaternary::LT) {
        executeLt(q);
    } else if (type == Quaternary::GET) {
        executeGet(q);
    } else if (type == Quaternary::LET) {
        executeLet(q);
    } else if (type == Quaternary::EQ) {
        executeEq(q);
    } else if (type == Quaternary::NEQ) {
        executeNeq(q);
    } else if (type == Quaternary::CALL) {
        executeCall(q);
    } else if (type == Quaternary::CALLFH) {
        executeCallFH(q);
    } else if (type == Quaternary::RET) {
        executeRet(q);
    }
    if (nextQeVecIndex >= quaternaryVector.size()) {
        isOver = true;
    }
}

void Executor::executeJmp(Quaternary &quaternary) {
    if (quaternary.getArg1() == "" || readValue(quaternary.getArg1()).getValue() == "0") {
        Value jupValue = readValue(quaternary.getResult());
        if (jupValue.getType() == ValueType::ADDR) {
            if (jupValue.getValue() == "$" + std::to_string(0x00000000))
                isOver = true;
            int addrRange, offset;
            addrRange = offset = 0;
            nextQeVecIndex = getValueIndex(jupValue.getValue(), addrRange, offset);
        } else {
            nextQeVecIndex = std::stoi(jupValue.getValue());
        }
    }
}

void Executor::executeRead(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int redeference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    std::string tempValue;
    std::cin >> tempValue;
    inputToStream(tempValue);
    switch (tempSymbol->getType()) {
        case SymbolType::INT:
        case SymbolType::ARRAY_INT:
            int intTemp;
            inputStream >> intTemp;
            assignProcess(*tempSymbol, index, redeference, std::to_string(intTemp));
            break;
        case SymbolType::REAL:
        case SymbolType::ARRAY_REAL:
            double doubleTemp;
            inputStream >> doubleTemp;
            assignProcess(*tempSymbol, index, redeference, std::to_string(doubleTemp));
            break;
        case SymbolType::CHAR:
            char charTemp;
            inputStream >> charTemp;
            assignProcess(*tempSymbol, index, redeference, "'" + ctos(charTemp) + "'");
            break;
        case SymbolType::ARRAY_CHAR:
            if (index < 0 && redeference == 0) {
                std::string strTemp;
                inputStream >> strTemp;
                int i = 0;
                for (i; i < strTemp.size(); ++i)
                    assignProcess(*tempSymbol, i, redeference, "'" + ctos(strTemp.at(i)) + "'");
                assignProcess(*tempSymbol, i, redeference, "'\0'");
            } else {
                char charTemp;
                inputStream >> charTemp;
                assignProcess(*tempSymbol, index, redeference, "'" + ctos(charTemp) + "'");
            }
        default:
            break;
    }
}

void Executor::executeWrite(Quaternary &quaternary) {
    Value tempValue = readValue(quaternary.getResult());
    std::string returnValue = "";
    if (tempValue.getType() == ValueType::CHAR_VALUE) {
        std::string charStr = tempValue.getValue();
        if (charStr.size() <= 2)
            returnValue = "";
        else
            returnValue = ctos(tempValue.getValue().at(1));
    } else if (tempValue.getType() == ValueType::ADDR) {
        int addrRange = 0;
        int offset = 0;
        int addrIndex = getValueIndex(tempValue.getValue(), addrRange, offset);
        if (addrRange == 1) {
            if (valueVec.at(addrIndex).getType() != ValueType::CHAR_VALUE) {
                int intValue = atoi(tempValue.getValue().substr(1, tempValue.getValue().size() - 1).c_str());
                char ch[9];
                sprintf(ch, "%08X", intValue);
                returnValue = ch;
                returnValue = "0X" + returnValue;
            }
            while (addrIndex > 0 && valueVec.at(addrIndex).getType() == ValueType::CHAR_VALUE &&
                   valueVec.at(addrIndex).getValue() != "''") {
                returnValue += ctos(valueVec.at(addrIndex).getValue().at(1));
                addrIndex--;
            }
        } else if (addrRange == 2) {
            if (globalVec.at(addrIndex).getType() != ValueType::CHAR_VALUE) {
                int intValue = atoi(tempValue.getValue().substr(1, tempValue.getValue().size() - 1).c_str());
                char ch[9];
                sprintf(ch, "%08X", intValue);
                returnValue = ch;
                returnValue = "0X" + returnValue;
            }
            while (addrIndex < globalVec.size() && globalVec.at(addrIndex).getType() == ValueType::CHAR_VALUE &&
                   globalVec.at(addrIndex).getValue() != "''") {
                returnValue += ctos(globalVec.at(addrIndex).getValue().at(1));
                addrIndex++;
            }
        } else if (addrRange == 3) {
            if (constVec.at(addrIndex).getType() != ValueType::CHAR_VALUE) {
                int intValue = atoi(tempValue.getValue().substr(1, tempValue.getValue().size() - 1).c_str());
                char ch[9];
                sprintf(ch, "%08X", intValue);
                returnValue = ch;
                returnValue = "0X" + returnValue;
            }
            while (addrIndex < constVec.size() && constVec.at(addrIndex).getType() == ValueType::CHAR_VALUE &&
                   constVec.at(addrIndex).getValue() != "''") {
                returnValue += ctos(constVec.at(addrIndex).getValue().at(1));
                addrIndex++;
            }
        }
    } else {
        returnValue = tempValue.getValue();
    }
    std::cout << returnValue;
}

void Executor::executeIn(Quaternary &quaternary) {
    symbolTable.insertTable();
    previousEsp.push(esp);
    level++;
}

void Executor::executeOut(Quaternary &quaternary) {
    symbolTable.destroyTable();
    level--;
    int tempEsp = previousEsp.top();
    previousEsp.pop();
    valueVec.erase(valueVec.begin() + tempEsp, valueVec.end());
    esp = tempEsp;
}

void Executor::executeInt(Quaternary &quaternary) {
    declareProcess(quaternary, Quaternary::INT);
}

void Executor::executeReal(Quaternary &quaternary) {
    declareProcess(quaternary, Quaternary::REAL);
}

void Executor::executeChar(Quaternary &quaternary) {
    declareProcess(quaternary, Quaternary::CHAR);
}

void Executor::executeArray(Quaternary &quaternary) {
    declareProcess(quaternary, quaternary.getArg1());
}

void Executor::executeAssign(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol;
    if (quaternary.getResult().at(0) == '@')      //说明是向寄存器赋值
    {
        tempSymbol = new VarSymbol(getRegNum(quaternary.getResult()));
//            if (quaternary.getResult() == "@esp")
//            {
//                valueVec.push_back(Value());
//                esp++;
//            }
    } else {
        if (index == -1) {
            tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
        } else {
            tempSymbol = symbolTable.getVarSymbol(quaternary.getResult().substr(0, quaternary.getResult().find('[')));
        }
    }
    assignProcess(*tempSymbol, index, dereference, quaternary.getArg1());
}

void Executor::executePlus(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    if (l_value.getType() == ValueType::ADDR) {
        setAddrUnitSize(l_value.getValue());
    } else if (r_value.getType() == ValueType::ADDR)
        setAddrUnitSize(r_value.getValue());
    std::string result = l_value + r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeMinus(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    if (quaternary.getArg1() == "")
        quaternary.setArg1("0");
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    if (l_value.getType() == ValueType::ADDR)
        setAddrUnitSize(l_value.getValue());
    std::string result = l_value - r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeMul(Quaternary &quaternary) {
    std::string result = "";
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    if (quaternary.getArg1() != "") {
        Value l_value = readValue(quaternary.getArg1());
        Value r_value = readValue(quaternary.getArg2());
        result = l_value * r_value;
    } else {    //说明是解除引用
        Value value = readValue(quaternary.getArg2());
        int addrRange = 0;
        int offset = 0;
        int addrIndex = getValueIndex(value.getValue(), addrRange, offset);
        if (addrRange == 1 || addrRange == 2) {
            if (offset != 0)
                throw ExecutorException(curExecuteStmtLineNo, "无法对该地址取值");
            if (addrRange == 1)
                result = valueVec.at(addrIndex).getValue();
            else
                result = globalVec.at(addrIndex).getValue();
        } else if (addrRange == 3) {
            if (offset != 0)
                throw ExecutorException(curExecuteStmtLineNo, "无法对该地址取值");
            result = constVec.at(addrIndex).getValue();
        }
    }
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeDiv(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = l_value / r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeGt(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = l_value > r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeLt(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = l_value < r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeGet(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = l_value >= r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeLet(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = l_value <= r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeEq(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = l_value == r_value;
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeNeq(Quaternary &quaternary) {
    int index = getScript(quaternary.getResult());
    int dereference = 0;
    VarSymbol *tempSymbol = symbolTable.getVarSymbol(quaternary.getResult());
    Value l_value = readValue(quaternary.getArg1());
    Value r_value = readValue(quaternary.getArg2());
    std::string result = nequal(l_value, r_value);
    assignProcess(*tempSymbol, index, dereference, result);
}

void Executor::executeCall(Quaternary &quaternary) {
    previousEbp.push(ebp);
    ebp = esp - 1;
}

void Executor::executeCallFH(Quaternary &quaternary) {
    //valueVec.erase(valueVec.begin() + ebp, valueVec.end());
    valueVec.erase(valueVec.end() - 1);
    esp = ebp;
    ebp = previousEbp.top();
    previousEbp.pop();
}

void Executor::executeRet(Quaternary &quaternary) {

}

//根据string返回类型(0:int, 1:float, 2:variable, 3.temp)
int Executor::getStringType(const std::string &str) {
    if ('0' < str.at(0) < '9') {
        for (int i = 1; i < str.size(); ++i)
            if (str.at(i) == '.')
                return 1;
        return 0;
    } else if (str.at(0) == '#') {
        return 3;
    } else {
        return 2;
    }
}

//根据字符串读取值
Value Executor::readValue(const std::string &str) {
    if (isdigit(str.at(0)) || str.at(0) == '-') {
        for (int i = 1; i < str.size(); ++i)
            if (str.at(i) == '.')
                return Value(str, ValueType::REAL_VALUE);
        return Value(str, ValueType::INT_VALUE);
    } else if (str.at(0) == '\'') {
        return Value(str, ValueType::CHAR_VALUE);
    } else if (str.at(0) == '$') {
        return Value(str, ValueType::ADDR);
    } else if (str.at(0) == '#') {
        return tempMap[str];
    } else if (str.at(0) == '@') {
        int index = getRegNum(str);
        return valueVec.at(index);
    } else if (str.at(0) == '"') {
        int returnAddr = curConstAddr;
        for (int i = 1; i < str.size() - 1; ++i) {
            constVec.push_back(Value("'" + ctos(str.at(i)) + "'", ValueType::CHAR_VALUE));
            constMap[constVec.size() - 1] = curConstAddr;
            curConstAddr = curConstAddr + 1;
        }
        constVec.push_back(Value("''", ValueType::CHAR_VALUE));
        constMap[constVec.size() - 1] = curConstAddr;
        curConstAddr = curConstAddr + 1;
        return Value("$" + std::to_string(returnAddr), ValueType::ADDR);
    } else {
        int index = getScript(str);
        int dereference = 0;
        std::string resAddr = "";
        int addrRange = 0;
        int offset = 0;
        int addrIndex = 0;
        VarSymbol *tempSymbol;
        if (index != -1) {
            tempSymbol = symbolTable.getVarSymbol(str.substr(0, str.find('[')));
        } else {
            tempSymbol = symbolTable.getVarSymbol(str);
        }

        switch (tempSymbol->getType()) {
            case SymbolType::INT:
            case SymbolType::REAL:
            case SymbolType::CHAR:
                if (tempSymbol->getLevel() == 0)
                    return globalVec.at(tempSymbol->getValueIndex());
                else
                    return valueVec.at(tempSymbol->getValueIndex());
            case SymbolType::ARRAY_INT:
            case SymbolType::ARRAY_REAL:
            case SymbolType::ARRAY_CHAR:
                if (tempSymbol->getLevel() == 0) {
                    if (index == -1)
                        return Value("$" + std::to_string(globalMap[tempSymbol->getValueIndex()]), ValueType::ADDR);
                    return globalVec.at(tempSymbol->getValueIndex() + index);
                } else {
                    if (index == -1)
                        return Value("$" + std::to_string(addrMap[tempSymbol->getValueIndex()]), ValueType::ADDR);
                    return valueVec.at(tempSymbol->getValueIndex() - index);
                }
            default:
                break;
        }
    }
}

//赋值处理
void Executor::assignProcess(const VarSymbol &symbol, int index, int dereference, const std::string &str) {
    Value tempValue = readValue(str);
    if (dereference == 1) {
        int valueIndex = 0;
        int addrRange = 1;
        int offset = 0;
        switch (symbol.getType()) {
            case SymbolType::ARRAY_INT:
                assignUtil(symbol.getValueIndex(), 0, tempValue.getValue(), ValueType::INT_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::ARRAY_REAL:
                assignUtil(symbol.getValueIndex(), 0, tempValue.getValue(), ValueType::REAL_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::ARRAY_CHAR:
                assignUtil(symbol.getValueIndex(), 0, tempValue.getValue(), ValueType::CHAR_VALUE,
                           symbol.getLevel() == 0);
                break;
        }
    } else {
        int addrRange = 0;
        int offset = 0;
        int valueIndex = 0;
        std::string resAddr = "";
        if (offset != 0)
            throw ExecutorException(curExecuteStmtLineNo, "无法对该地址进行赋值");

        switch (symbol.getType()) {
            case SymbolType::TEMP:
                tempMap[symbol.getName()] = tempValue;
                break;
            case SymbolType::REG:
                if (symbol.getValueIndex() == esp) {
                    valueVec.push_back(tempValue);
                    addrMap[esp] = curDataAddr - tempValue.getSize();
                    curDataAddr = addrMap[esp];
                    esp++;
                }
                valueVec.at(symbol.getValueIndex()) = tempValue;
                break;
            case SymbolType::INT:
                assignUtil(symbol.getValueIndex(), 0, tempValue.getValue(), ValueType::INT_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::REAL:
                assignUtil(symbol.getValueIndex(), 0, tempValue.getValue(), ValueType::REAL_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::CHAR:
                assignUtil(symbol.getValueIndex(), 0, tempValue.getValue(), ValueType::CHAR_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::ARRAY_INT:
                assignUtil(symbol.getValueIndex(), index, tempValue.getValue(), ValueType::INT_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::ARRAY_REAL:
                assignUtil(symbol.getValueIndex(), index, tempValue.getValue(), ValueType::REAL_VALUE,
                           symbol.getLevel() == 0);
                break;
            case SymbolType::ARRAY_CHAR:
                assignUtil(symbol.getValueIndex(), index, tempValue.getValue(), ValueType::CHAR_VALUE,
                           symbol.getLevel() == 0);
                break;
        }
    }
}

//赋值辅助函数
void Executor::assignUtil(_dataSize valueIndex, _dataSize index, const std::string valueStr, ValueType valueType,
                          bool isGlobal) {
    if (isGlobal)
        globalVec.at(valueIndex + index) = Value(valueStr, valueType);
    else
        valueVec.at(valueIndex - index) = Value(valueStr, valueType);
}

//声明处理
void Executor::declareProcess(const Quaternary &quaternary, const std::string &type) {
    SymbolType symbolType;
    SymbolType funSymbolType;
    ValueType valueType;
    std::string initValue = "0";
    int elementNum = 0;
    if (quaternary.getArg2().empty())    //非数组
    {
        if (type == Quaternary::INT) {
            symbolType = SymbolType::INT;
            funSymbolType = SymbolType::INT;
            valueType = ValueType::INT_VALUE;
        } else if (type == Quaternary::REAL) {
            symbolType = SymbolType::REAL;
            funSymbolType = SymbolType::REAL;
            valueType = ValueType::REAL_VALUE;
        } else if (type == Quaternary::CHAR) {
            symbolType = SymbolType::CHAR;
            funSymbolType = SymbolType::CHAR;
            valueType = ValueType::CHAR_VALUE;
            initValue = "''";
        }

        if (quaternary.getResult().at(quaternary.getResult().size() - 1) == ')')  //说明是函数
        {
            FunctionSymbol *funSymbol = new FunctionSymbol(
                    quaternary.getResult().substr(0, quaternary.getResult().size() - 2),
                    funSymbolType, quaternary.getLine());
            symbolTable.insertFunc(funSymbol);
        } else {
            if (!quaternary.getArg1().empty() && quaternary.getArg1().at(0) == '@') //说明是函数参数
            {
                VarSymbol *symbol = new VarSymbol(quaternary.getResult(), symbolType, quaternary.getLine(), level,
                                                  elementNum);
                symbol->setValueIndex(getRegNum(quaternary.getArg1()));
                symbolTable.insertVar(symbol);
                assignProcess(*symbol, -1, 0, quaternary.getArg1());
            } else {
                VarSymbol *symbol = new VarSymbol(quaternary.getResult(), symbolType, quaternary.getLine(), level,
                                                  elementNum);
                if (level == 0) {
                    int globalIndex = globalVec.size();
                    symbol->setValueIndex(globalIndex);
                    symbolTable.insertVar(symbol);
                    globalVec.emplace_back(initValue, valueType);
                    globalMap[globalIndex] = curGlobalAddr;
                    curGlobalAddr = globalMap[globalIndex] + symbol->getDataSize();
                } else {
                    symbol->setValueIndex(esp);
                    symbolTable.insertVar(symbol);
                    valueVec.emplace_back(initValue, valueType);
                    addrMap[esp] = curDataAddr - symbol->getDataSize();
                    curDataAddr = addrMap[esp];
                    esp++;
                }
                if (quaternary.getArg1() != "")
                    assignProcess(*symbol, -1, 0, quaternary.getArg1());
            }
        }
    } else {       //数组
        if (type == Quaternary::INT) {
            symbolType = SymbolType::ARRAY_INT;
            valueType = ValueType::INT_VALUE;
        } else if (type == Quaternary::REAL) {
            symbolType = SymbolType::ARRAY_REAL;
            valueType = ValueType::REAL_VALUE;
        } else if (type == Quaternary::CHAR) {
            symbolType = SymbolType::ARRAY_CHAR;
            valueType = ValueType::CHAR_VALUE;
            initValue = "''";
        }

        if (quaternary.getArg2() == "0") {
            VarSymbol *symbol = new VarSymbol(quaternary.getResult(), symbolType, quaternary.getLine(), level);
            symbol->setValueIndex(getRegNum(quaternary.getArg1()));
            symbolTable.insertVar(symbol);
            assignProcess(*symbol, -1, 0, quaternary.getArg1());
        } else {
            VarSymbol *symbol = new VarSymbol(quaternary.getResult(), symbolType, quaternary.getLine(), level,
                                              std::stoi(quaternary.getArg2()));
            if (level == 0) {
                int globalIndex = globalVec.size();
                symbol->setValueIndex(globalIndex);
                symbolTable.insertVar(symbol);
                for (int i = 0; i < symbol->length(); ++i) {
                    globalVec.push_back(Value(initValue, valueType));
                    globalMap[globalIndex] = curGlobalAddr + symbol->getDataSize() * (i + 1);
                    globalIndex++;
                }
                curGlobalAddr = globalMap[globalIndex - 1];
            } else {
                int eleNum = symbol->length();
                int preEsp = esp;
                esp += eleNum;
                symbol->setValueIndex(esp - 1);
                symbolTable.insertVar(symbol);
                for (int i = 0; i < eleNum; ++i) {
                    valueVec.push_back(Value(initValue, valueType));
                    addrMap[preEsp] = curDataAddr - symbol->getDataSize() * (i + 1);
                    preEsp++;
                }
                curDataAddr = addrMap[symbol->getValueIndex()];
            }
        }
    }
}

//判断地址类型是否正确
void Executor::checkAddressIsRight(int pointType, const std::string &addrValue) {
    int addrRange = 0;
    int offset = 0;
    int addrIndex = getValueIndex(addrValue, addrRange, offset);
    if (offset != 0)
        throw ExecutorException(curExecuteStmtLineNo, "无法将该类型的地址赋给指针");
    bool isFalse;
}

//根据地址锁定其位置
int Executor::getValueIndex(const std::string &addrValue, int &addrRange, int &offset) {
    int addr = std::stoi(addrValue.substr(1, addrValue.size() - 1));
    if (addr >= (PointAddress) DATA_ADDR && addr < (PointAddress) DATA_ADDR + 0x10000000)     //数据段
    {
        addrRange = 1;
        for (int i = 0; i < valueVec.size(); ++i) {
            if (addr >= addrMap[i]) {
                if (i + 1 < valueVec.size() && addrMap[i + 1] > addrMap[i]) {
                    i = i + 1;
                    while (i < valueVec.size()) {

                    }
                } else {
                    offset = addr - addrMap[i];
                    return i;
                }
            }
        }
        throw ExecutorException(curExecuteStmtLineNo, "堆溢出");
    } else if (addr >= (PointAddress) GLOBAL_ADDR && addr < (PointAddress) GLOBAL_ADDR + 0x10000000) {      //全局区
        addrRange = 2;
        for (int i = 0; i < globalVec.size(); ++i) {
            if (addr <= globalMap[i]) {
                if (addr < globalMap[i]) {
                    offset = addr - globalMap[i - 1];
                    return i - 1;
                } else {
                    return i;
                }
            }
        }
        throw ExecutorException(curExecuteStmtLineNo, "堆溢出");
    } else if (addr >= (PointAddress) CONST_ADDR && addr < (PointAddress) CONST_ADDR + 0x10000000) {      //常量区
        addrRange = 3;
        for (int i = 0; i < constVec.size(); ++i) {
            if (addr <= constMap[i]) {
                if (addr < constMap[i]) {
                    offset = addr - constMap[i - 1];
                    return i - 1;
                } else {
                    return i;
                }
            }
        }
        throw ExecutorException(curExecuteStmtLineNo, "栈溢出");
    } else if (addr >= (PointAddress) CODE_ADDR && addr < (PointAddress) CODE_ADDR + 0x10000000) {        //代码区
        addrRange = 4;
        return (addr - (PointAddress) CODE_ADDR) / 4;
    } else if (addr > (PointAddress) DATA_ADDR) {
        throw ExecutorException(curExecuteStmtLineNo, "栈溢出");
    } else {
        return -1;
    }
}

//如果是数组，则获取数组下标，否则返回-1
int Executor::getScript(const std::string varStr) {
    int index = -1;
    int start = varStr.find('[');
    int end = varStr.find(']');
    if (start >= 0) {
        std::string indexStr = varStr.substr(start + 1, end - start - 1);
        index = std::stoi(readValue(indexStr).getValue());
    }
    return index;
}


//获取"@ebp..."的值
int Executor::getRegNum(const std::string str) {
    int result = 0;
    if (str.substr(1, 3) == "ebp") {
        if (str.size() > 4 && str.at(4) == '-')
            result = ebp - std::stoi(str.substr(5, str.size() - 5));
        else
            result = ebp;
    } else if (str.substr(1, 3) == "esp") {
        if (str.size() > 4 && str.at(4) == '-')
            result = esp - std::stoi(str.substr(5, str.size() - 5));
        else
            result = esp;
    }
    return result;
}

//根据字符串获取地址
std::string Executor::addrFromStr(const std::string &addr) {
    std::string addrStr;
    int intValue = std::stoi(addr.substr(1, addr.size() - 1));
    char ch[9];
    sprintf(ch, "%08X", intValue);
    addrStr = ch;
    return "0X" + addrStr;
}

//根据int获取地址
std::string Executor::addrFromInt(int addr) {
    std::string addrStr;
    char ch[9];
    sprintf(ch, "%08X", addr);
    addrStr = ch;
    return "0X" + addrStr;
}

//设置addrUntiSize的值
void Executor::setAddrUnitSize(int symbolType) {
    /*switch (symbolType) {
        case SymbolType::POINT_INT:
            ValueType::addrUnitSize = 4;
            break;
        case SymbolType::POINT_REAL:
            ValueType::addrUnitSize = 8;
            break;
        case SymbolType::POINT_CHAR:
            ValueType::addrUnitSize = 1;
            break;
        default:
            break;
    }*/
}

//设置addrUntiSize的值
void Executor::setAddrUnitSize(const std::string &addrStr) {
    //Value value = readValue(quaternary.getArg2());
    Value value;
    int addrRange = 0;
    int offset = 0;
    int addrIndex = getValueIndex(addrStr, addrRange, offset);
    if (addrRange == 1 || addrRange == 2) {
        if (offset != 0)
            throw ExecutorException(curExecuteStmtLineNo, "地址偏移异常");
        if (addrRange == 1)
            value = valueVec.at(addrIndex);
        else
            value = globalVec.at(addrIndex);
    } else if (addrRange == 3) {
        if (offset != 0)
            throw ExecutorException(curExecuteStmtLineNo, "地址偏移异常");
        value = constVec.at(addrIndex);
    }
    Value::addrUnitSize = value.getSize();
}

void Executor::inputToStream(std::string value) {
    inputStream.clear();
    inputStream.str("");
    inputStream << value;
}