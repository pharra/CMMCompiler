//
// Created by chasi on 2018/11/28.
//

#include <iostream>
#include "Semantic.h"

Semantic::Semantic() = default;

std::vector<Quaternary> Semantic::generateCode(const std::vector<TreeNode *> &treeNodeVec, std::string &errorStr) {
    programEntry = 0;
    symbolTable.clear();
    globalVarVec.clear();
    quaternaryVector.clear();
    curStmtLineNo = 0;
    level = 0;
    curFunLevel = 0;
    while (!typeStack.empty())
        typeStack.pop();
    for (auto i:treeNodeVec) {
        try {
            interpret(i);
        } catch (SemanticException &cge) {
            errorStr += ("\n    " + cge.message());
            std::cout << cge.message() << std::endl;
            continue;
        }
    }
    return quaternaryVector;
}

void Semantic::interpret(const TreeNode *treeNode) {
//    while (true) {
    curStmtLineNo = treeNode->getLine();
    if (treeNode->getTreeNodeType() != TreeNode::DECLARE_STMT &&
        treeNode->getTreeNodeType() != TreeNode::CLASS_DECLARE && treeNode->getLevel() == 0)
        throw SemanticException(curStmtLineNo, "Only Declare in level 0");
    switch (treeNode->getTreeNodeType()) {
        case TreeNode::IF_STMT:
            interpretIfStmt(treeNode);
            break;
        case TreeNode::WHILE_STMT:
            interpretWhileStmt(treeNode);
            break;
        case TreeNode::FOR_STMT:
            interpretForStmt(treeNode);
            break;
        case TreeNode::READ_STMT:
            interpretReadStmt(treeNode);
            break;
        case TreeNode::WRITE_STMT:
            interpretWriteStmt(treeNode);
            break;
        case TreeNode::DECLARE_STMT:
            interpretDecalreStmt(treeNode);
            break;
        case TreeNode::ASSIGN_STMT:
            interpretAssignStmt(treeNode);
            break;
        case TreeNode::FUNCTION_CALL:
            interpretFunctionCall(treeNode);
            break;
        case TreeNode::RETURN_STMT:
            interpretReturnStmt(treeNode);
            break;
        case TreeNode::BLOCK_STMT:
            interpretBlockStmt(treeNode);
            break;
        case TreeNode::CLASS_DECLARE:
            interpretClassStmt(treeNode);
            break;
        default:
            break;
    }
//        symbolTable.clearTempVec();
//        if (treeNode->getNext() != nullptr)
//            treeNode = treeNode->getNext();
//        else
//            break;
//    }
}

//解释if语句
void Semantic::interpretIfStmt(const TreeNode *treeNode) {
    //条件跳转 (jmp, 条件, null, 目标)  条件为假时跳转
    quaternaryVector.emplace_back(Quaternary::JMP, interpretExp(treeNode->getChild()[0]), "", "", curStmtLineNo);
    _dataSize ifFalseJmpIndex = quaternaryVector.size() - 1;

    interpret(treeNode->getChild()[1]);

    if (treeNode->getChild().size() == 3) {
        quaternaryVector.emplace_back(Quaternary::JMP, "", "", "", curStmtLineNo);
        _dataSize outJmpIndex = quaternaryVector.size() - 1;
        quaternaryVector.at(ifFalseJmpIndex).setResult(outJmpIndex + 1);

        interpret(treeNode->getChild()[2]);

        quaternaryVector.at(outJmpIndex).setResult(quaternaryVector.size());
    } else {
        quaternaryVector.at(ifFalseJmpIndex).setResult(quaternaryVector.size());
    }
}

//解释while语句
void Semantic::interpretWhileStmt(const TreeNode *treeNode) {
    _dataSize jumpLine = quaternaryVector.size();
    quaternaryVector.emplace_back(Quaternary::JMP, interpretExp(treeNode->getChild()[0]), "", "", curStmtLineNo);
    _dataSize falseJmpIndex = quaternaryVector.size() - 1;

    interpret(treeNode->getChild()[1]);

    quaternaryVector.emplace_back(Quaternary::JMP, "", "", std::to_string(jumpLine), curStmtLineNo);
    quaternaryVector.at(falseJmpIndex).setResult(quaternaryVector.size());
}

//解释for语句
void Semantic::interpretForStmt(const TreeNode *treeNode) {
    quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
    level++;
    symbolTable.insertTable();
    interpret(treeNode->getChild()[0]);
    _dataSize jumpLine = quaternaryVector.size();
    quaternaryVector.emplace_back(Quaternary::JMP, interpretExp(treeNode->getChild()[1]), "", "", curStmtLineNo);
    _dataSize falseJmpIndex = quaternaryVector.size() - 1;

    quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
    symbolTable.insertTable();
    for (auto &i:treeNode->getChild()[3]->getChild()) {
        interpret(i);
    }

    interpret(treeNode->getChild()[2]);

    symbolTable.destroyTable();
    level--;
    quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
    quaternaryVector.emplace_back(Quaternary::JMP, "", "", std::to_string(jumpLine), curStmtLineNo);
    symbolTable.destroyTable();
    quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
    quaternaryVector.at(falseJmpIndex).setResult(quaternaryVector.size());
}


void Semantic::interpretBlockStmt(const TreeNode *treeNode) {
    quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
    level++;
    symbolTable.insertTable();
    for (auto &i:treeNode->getChild()) {
        interpret(i);
    }
    level--;
    symbolTable.destroyTable();
    quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
}

void Semantic::interpretClassStmt(const TreeNode *treeNode) {
    quaternaryVector.emplace_back(Quaternary::CLASS, treeNode->getValue(), "", "", curStmtLineNo);
    Object *object = new Object(treeNode->getValue());
    quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
    level++;
    symbolTable.insertTable();
    auto attrs = treeNode->getChild()[0];
    auto methods = treeNode->getChild()[1];
    for (auto &i:attrs->getChild()) {
        quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
        interpretDecalreStmt(i, true, object);
        quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
    }
    for (auto &i:methods->getChild()) {
        quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
        interpretFunction(i, true, object);
        quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
    }
    level--;
    symbolTable.destroyTable();
    quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
}

//解释read语句
void Semantic::interpretReadStmt(const TreeNode *treeNode) {
    std::string varStr = treeNode->getChild()[0]->getValue();
    auto varSymbol = symbolTable.getVarSymbol(varStr);
    if (varSymbol == nullptr) {
        throw SemanticException(curStmtLineNo, "The Var don't be declared");
    }
    SymbolType symbolType = varSymbol->getType();
    if (!((int) (symbolType) & 0x10 == 0x10)) { // 数组
        if (treeNode->getChild()[0]->isArray()) {
            varStr += ("[" + interpretScript(treeNode->getChild()[0]->getChild()[0]) + "]");
        } else {
            throw SemanticException(curStmtLineNo, "The Var Data Type isn't basic type");
        }
    }

    quaternaryVector.emplace_back(Quaternary::READ, "", "", varStr, curStmtLineNo);
}

//解释write语句
void Semantic::interpretWriteStmt(const TreeNode *treeNode) {
    std::string varStr = interpretExp(treeNode->getChild()[0]);
    typeStack.pop();
    quaternaryVector.emplace_back(Quaternary::WRITE, "", "", varStr, curStmtLineNo);
}

//解释声明语句
void Semantic::interpretDecalreStmt(const TreeNode *treeNode, bool isClass, Object *object) {
    if (treeNode->getChild()[0]->getTreeNodeType() == TreeNode::FUNCTION_DECLARE) {
        interpretFunction(treeNode);
        return;
    }
    _dataSize startStmtIndex = quaternaryVector.size();
    TreeNode *var = treeNode->getChild()[0];
    if (!var->isArray())     //单值
    {
        std::string value;
        if (treeNode->getChild().size() > 1) {
            if (treeNode->getChild()[1]->getTreeNodeType() == TreeNode::REG) {
                value = treeNode->getChild()[1]->getValue();
            } else {
                value = interpretExp(treeNode->getChild()[1]);
                checkDeclareIsRight(var->getDataType());
            }
        }
        if (var->getDataType() == Token::INT) {
            quaternaryVector.emplace_back(Quaternary::INT, value, "", var->getValue(), curStmtLineNo);
            auto *symbol = new VarSymbol(var->getValue(), SymbolType::INT, var->getLine(), level);
            symbolTable.insertVar(symbol);
        } else if (var->getDataType() == Token::REAL) {
            quaternaryVector.emplace_back(Quaternary::REAL, value, "", var->getValue(), curStmtLineNo);
            auto *symbol = new VarSymbol(var->getValue(), SymbolType::REAL, var->getLine(), level);
            symbolTable.insertVar(symbol);
        } else if (var->getDataType() == Token::CHAR) {
            quaternaryVector.emplace_back(Quaternary::CHAR, value, "", var->getValue(), curStmtLineNo);
            auto *symbol = new VarSymbol(var->getValue(), SymbolType::CHAR, var->getLine(), level);
            symbolTable.insertVar(symbol);
        }
    } else {
        std::string len = "0";
        _dataSize elementNum = 0;
        VarSymbol *symbol;
        if (var->getDataType() == Token::INT) {
            quaternaryVector.emplace_back(Quaternary::ARRAY, var->getDataTypeValue(), len, var->getValue(),
                                          curStmtLineNo);
            symbol = new VarSymbol(var->getValue(), SymbolType::ARRAY_INT, curStmtLineNo, level, elementNum);
            symbolTable.insertVar(symbol);
        } else if (var->getDataType() == Token::REAL) {
            quaternaryVector.emplace_back(Quaternary::ARRAY, var->getDataTypeValue(), len, var->getValue(),
                                          curStmtLineNo);
            symbol = new VarSymbol(var->getValue(), SymbolType::ARRAY_REAL, curStmtLineNo, level, elementNum);
            symbolTable.insertVar(symbol);
        } else if (var->getDataType() == Token::CHAR) {
            quaternaryVector.emplace_back(Quaternary::ARRAY, var->getDataTypeValue(), len, var->getValue(),
                                          curStmtLineNo);
            symbol = new VarSymbol(var->getValue(), SymbolType::ARRAY_CHAR, curStmtLineNo, level, elementNum);
            symbolTable.insertVar(symbol);
        }
        _dataSize arrayDeclareIndex = quaternaryVector.size() - 1;
        if (treeNode->getChild().size() > 1) {
            TreeNode *valueNode = treeNode->getChild()[1];
            _dataSize initNum = 0;
            if (valueNode->getTreeNodeType() == TreeNode::ARRAY) {
                for (auto &i:valueNode->getChild()) {
                    initNum++;
                    std::string value = interpretExp(i);
                    checkDeclareIsRight(var->getDataType());
                    quaternaryVector.emplace_back(Quaternary::ASSIGN, value, "",
                                                  var->getValue() + "[" + std::to_string(initNum - 1) + "]",
                                                  curStmtLineNo);

                }
                symbol->setLength(initNum);
                quaternaryVector.at(arrayDeclareIndex).setArg2(std::to_string(initNum));
            } else if (valueNode->getTreeNodeType() == TreeNode::NEW_STMT) {
                if (valueNode->getDataType() != var->getDataType())
                    throw SemanticException(curStmtLineNo, "Assign type don't match");
                std::string length = interpretExp(valueNode->getChild()[0]);
                quaternaryVector.at(arrayDeclareIndex).setArg2(length);
                symbol->setLength(std::stoull(length));
            } else if (valueNode->getTreeNodeType() == TreeNode::LITERAL) {
                for (const char &i:valueNode->getValue()) {
                    initNum++;
                    std::string value = "'" + std::to_string(i) + "'";
                    quaternaryVector.emplace_back(Quaternary::ASSIGN, value, "",
                                                  var->getValue() + "[" + std::to_string(initNum - 1) + "]",
                                                  curStmtLineNo);
                }
                quaternaryVector.at(arrayDeclareIndex).setArg2(std::to_string(initNum));
                symbol->setLength(initNum);
            } else if (valueNode->getTreeNodeType() == TreeNode::REG) {
                quaternaryVector.back().setArg1(valueNode->getValue());
            }
        }
    }
    if (isClass) {
        object->addAttr(var->getValue(), startStmtIndex);
    }
    if (symbolTable.isRoot()) {
        _dataSize endStmtIndex = quaternaryVector.size() - 1;
        for (_dataSize i = startStmtIndex; i <= endStmtIndex; ++i)
            globalVarVec.push_back(i);
    }

}

//解释赋值语句
void Semantic::interpretAssignStmt(const TreeNode *treeNode) {
    TreeNode *varNode = treeNode->getChild()[0];
    VarSymbol *varSymbol = symbolTable.getVarSymbol(varNode->getValue());
    if (!varSymbol)
        throw SemanticException(curStmtLineNo, "The Var don't be declared");
    bool isArray = varNode->isArray();
    std::string varStr = varNode->getValue();
    std::string value;
    if (isArray && !varNode->getChild().empty()) {
        varStr += ("[" + interpretScript(varNode->getChild()[0]) + "]");
        value = interpretExp(treeNode->getChild()[1]);
    } else {
        value = interpretExp(treeNode->getChild()[1]);
    }
    ValueType targetType;
    switch (varSymbol->getType()) {
        case SymbolType::INT:
            targetType = ValueType::INT_VALUE;
            break;
        case SymbolType::ARRAY_INT:
            if (!varNode->getChild().empty())
                targetType = ValueType::INT_VALUE;
            else
                targetType = ValueType::ADDR;
            break;
        case SymbolType::REAL:
            targetType = ValueType::REAL_VALUE;
            break;
        case SymbolType::ARRAY_REAL:
            if (!varNode->getChild().empty())
                targetType = ValueType::REAL_VALUE;
            else
                targetType = ValueType::ADDR;
            break;
        case SymbolType::CHAR:
            targetType = ValueType::CHAR_VALUE;
            break;
        case SymbolType::ARRAY_CHAR:
            if (!varNode->getChild().empty())
                targetType = ValueType::CHAR_VALUE;
            else
                targetType = ValueType::ADDR;
            break;
        case SymbolType::CLASS:
            targetType = ValueType::ADDR;
            break;
    }

    checkAssignIsRight(targetType);
    quaternaryVector.emplace_back(Quaternary::ASSIGN, value, "", varStr, curStmtLineNo);
}

//解释函数
void Semantic::interpretFunction(const TreeNode *treeNode, bool isClass, Object *object) {
    if (level != 0)
        throw SemanticException(curStmtLineNo, "Cannot define function in function");
    TreeNode *var = treeNode->getChild()[0];
    SymbolType symbolType;
    std::string quaternaryExpType;
    switch (var->getDataType()) {
        case Token::INT:
            symbolType = SymbolType::INT;
            quaternaryExpType = Quaternary::INT;
            if (var->isArray()) {
                symbolType = SymbolType::ARRAY_INT;
                quaternaryExpType = Quaternary::ARRAY;
            }
            break;
        case Token::REAL:
            symbolType = SymbolType::REAL;
            quaternaryExpType = Quaternary::REAL;
            if (var->isArray()) {
                symbolType = SymbolType::ARRAY_REAL;
                quaternaryExpType = Quaternary::ARRAY;
            }
            break;
        case Token::CHAR:
            symbolType = SymbolType::CHAR;
            quaternaryExpType = Quaternary::CHAR;
            if (var->isArray()) {
                symbolType = SymbolType::ARRAY_CHAR;
                quaternaryExpType = Quaternary::ARRAY;
            }
            break;
        default:
            break;
    }
    if (var->getValue() == "main") {
        programEntry = quaternaryVector.size();
    }
    quaternaryVector.emplace_back(quaternaryExpType, "", "", var->getValue() + "()", curStmtLineNo);
    auto *functionSymbol = new FunctionSymbol(var->getValue(), symbolType, quaternaryVector.size());
    quaternaryVector.emplace_back(Quaternary::IN, "", "", "", curStmtLineNo);
    level++;
    curFunLevel = level;
    symbolTable.insertTable();
    auto paramsNode = var->getChild()[0];
    if (!paramsNode->getChild().empty()) {
        for (auto &i:paramsNode->getChild()) {
            interpretDecalreStmt(i);
            functionSymbol->insertParam(symbolTable.getCurVarSymbol()->getType());
        }
    }
    symbolTable.insertFunc(functionSymbol);
    auto blockNode = treeNode->getChild()[1];
    for (auto &i:blockNode->getChild()) {
        interpret(i);
    }

    symbolTable.destroyTable();
    level--;
    quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
    for (int i = 0; i < returnJmpIndexVec.size(); ++i)
        quaternaryVector.at(returnJmpIndexVec.at(i)).setResult(std::to_string(quaternaryVector.size() - 1));
    returnJmpIndexVec.clear();
    quaternaryVector.emplace_back(Quaternary::JMP, "", "", "@ebp", curStmtLineNo);
}

//解释return语句
void Semantic::interpretReturnStmt(const TreeNode *treeNode) {
    if (symbolTable.isRoot())
        throw SemanticException(curStmtLineNo, "Return Stmt should be used in function");
    std::string result = interpretExp(treeNode->getChild()[0]);
    ValueType valueType = transformFunToValue(symbolTable.getCurFuncSymbol()->getType());
    checkAssignIsRight(valueType);
    quaternaryVector.emplace_back(Quaternary::ASSIGN, result, "", "@ebp-1", curStmtLineNo);
    for (int i = level; i > curFunLevel; --i)
        quaternaryVector.emplace_back(Quaternary::OUT, "", "", "", curStmtLineNo);
    quaternaryVector.emplace_back(Quaternary::JMP, "", "", "", curStmtLineNo);
    returnJmpIndexVec.push_back(quaternaryVector.size() - 1);
}

//解释函数调用
std::string Semantic::interpretFunctionCall(const TreeNode *treeNode) {
    if (symbolTable.isRoot())
        throw SemanticException(curStmtLineNo, "Function call should be used in function");
    auto functionSymbol = symbolTable.getFuncSymbol(treeNode->getValue());
    if (!functionSymbol)
        throw SemanticException(curStmtLineNo, "Function don't be declared");
    if (functionSymbol->getName() == "main")
        throw SemanticException(curStmtLineNo, "Can't call function main");
    std::stack<std::string> argStack;
    if (!treeNode->getChild().empty()) {
        TreeNode *argNode = treeNode->getChild()[0];
        if (argNode->getChild().size() != functionSymbol->getParams().size()) {
            throw SemanticException(curStmtLineNo, "function's args numbers don't match");
        }
        for (_dataSize i = 0; i < argNode->getChild().size(); i++) {
            auto targetType = transformFunToValue(functionSymbol->getParams().at(i));
            std::string argValue = interpretExp(argNode->getChild()[i]);
            checkAssignIsRight(targetType);
            argStack.push(argValue);
        }
    }
    quaternaryVector.emplace_back(Quaternary::ASSIGN, "0", "", "@esp", curStmtLineNo);
    while (!argStack.empty()) {
        quaternaryVector.emplace_back(Quaternary::ASSIGN, argStack.top(), "", "@esp", curStmtLineNo);
        argStack.pop();
    }
    auto assignAddrIndex = quaternaryVector.size();
    quaternaryVector.emplace_back(Quaternary::ASSIGN, "", "", "@esp", curStmtLineNo);
    quaternaryVector.emplace_back(Quaternary::CALL, "", "", functionSymbol->getName(), curStmtLineNo);
    quaternaryVector.emplace_back(Quaternary::JMP, "", "", std::to_string(functionSymbol->getLine()), curStmtLineNo);
    quaternaryVector.emplace_back(Quaternary::CALLFH, "", "", "", curStmtLineNo);
    quaternaryVector.at(assignAddrIndex).setArg1(
            "$" + std::to_string((PointAddress) CODE_ADDR + 4 * (quaternaryVector.size() - 1)));
    //设置函数返回类型
    ValueType valueType = transformFunToValue(functionSymbol->getReturnType());
    typeStack.push(valueType);
    return "@esp-1";
}

//解释表达式
std::string Semantic::interpretExp(const TreeNode *treeNode) {
    if (treeNode->getTreeNodeType() == TreeNode::EXP) {
        switch (treeNode->getDataType()) {
            case Token::LOGIC_EXP:
                return interpretLogicExp(treeNode);
            case Token::ADDTIVE_EXP:
                return interpretAddtiveExp(treeNode);
            case Token::TERM_EXP:
                return interpretTermExp(treeNode);
            default:
                throw SemanticException(treeNode->getLine(), "Invalid Exp");
        }
    } else if (treeNode->getTreeNodeType() == TreeNode::FACTOR) {
        if (treeNode->getDataType() == Token::MINUS) {
            std::string nodeStr = interpretExp(treeNode->getChild()[0]);
            ValueType type;
            if (isConstant(nodeStr, type))
                return ("-" + nodeStr);
            std::string temp = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(Quaternary::MINUS, "", nodeStr, temp, curStmtLineNo);
            return temp;
        } else {
            return interpretExp(treeNode->getChild()[0]);
        }
    } else if (treeNode->getTreeNodeType() == TreeNode::VAR) {
        return interpretVar(treeNode);
    } else if (treeNode->getTreeNodeType() == TreeNode::LITERAL) {
        if (treeNode->getDataType() == Token::CHAR_VALUE)
            typeStack.push(ValueType::CHAR_VALUE);
        else if (treeNode->getDataType() == Token::INT_VALUE)
            typeStack.push(ValueType::INT_VALUE);
        else if (treeNode->getDataType() == Token::REAL_VALUE)
            typeStack.push(ValueType::REAL_VALUE);
        return treeNode->getValue();
    } else if (treeNode->getTreeNodeType() == TreeNode::FUNCTION_CALL) {
        return interpretFunctionCall(treeNode);
    }
    throw SemanticException(treeNode->getLine(), "Invalid Exp");
}

//解释逻辑表达式
std::string Semantic::interpretLogicExp(const TreeNode *treeNode) {
    ValueType leftType, rightType;
    bool isAllConstant = false;
    std::string temp;
    std::string leftNodeStr = interpretExp(treeNode->getChild()[0]);
    std::string rightNodeStr = interpretExp(treeNode->getChild()[2]);
    ValueType r_valueType = typeStack.top();
    typeStack.pop();
    ValueType l_valueType = typeStack.top();
    typeStack.pop();
    Token::TokenTag opType = treeNode->getChild()[1]->getDataType();
    if (l_valueType != r_valueType)
        throw SemanticException(curStmtLineNo, "Data type don't match in logical exp");
    if (l_valueType == ValueType::CHAR_VALUE && r_valueType == ValueType::CHAR_VALUE && opType != Token::EQL &&
        opType != Token::NOT_EQL)
        throw SemanticException(curStmtLineNo, "Can't calculate" + treeNode->getChild()[1]->getDataTypeValue());
    typeStack.push((ValueType) ((int) l_valueType | (int) r_valueType));
    if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType))
        isAllConstant = true;
    if (!isAllConstant)
        temp = symbolTable.getNewTempSymbol();
    switch (opType) {
        case Token::GRT:
            if (isAllConstant)
                return Value(leftNodeStr, leftType) > Value(rightNodeStr, rightType);
            quaternaryVector.emplace_back(Quaternary::GT, leftNodeStr, rightNodeStr, temp, curStmtLineNo);
            break;
        case Token::GRT_EQL:
            if (isAllConstant)
                return Value(leftNodeStr, leftType) >= Value(rightNodeStr, rightType);
            quaternaryVector.emplace_back(Quaternary::GET, leftNodeStr, rightNodeStr, temp, curStmtLineNo);
            break;
        case Token::LES:
            if (isAllConstant)
                return Value(leftNodeStr, leftType) < Value(rightNodeStr, rightType);
            quaternaryVector.emplace_back(Quaternary::LT, leftNodeStr, rightNodeStr, temp, curStmtLineNo);
            break;
        case Token::LES_EQL:
            if (isAllConstant)
                return Value(leftNodeStr, leftType) <= Value(rightNodeStr, rightType);
            quaternaryVector.emplace_back(Quaternary::LET, leftNodeStr, rightNodeStr, temp, curStmtLineNo);
            break;
        case Token::EQL:
            if (isAllConstant)
                return Value(leftNodeStr, leftType) == Value(rightNodeStr, rightType);
            quaternaryVector.emplace_back(Quaternary::EQ, leftNodeStr, rightNodeStr, temp, curStmtLineNo);
            break;
        case Token::NOT_EQL:
            if (isAllConstant)
                return nequal(Value(leftNodeStr, leftType), Value(rightNodeStr, rightType));
            quaternaryVector.emplace_back(Quaternary::NEQ, leftNodeStr, rightNodeStr, temp, curStmtLineNo);
            break;
        default:
            throw SemanticException(treeNode->getLine(), "Invalid logical exp");
    }
    return temp;
}

//解释多项式
std::string Semantic::interpretAddtiveExp(const TreeNode *treeNode) {
    ValueType leftType, rightType;
    std::string leftNodeStr, rightNodeStr, temp_1;
    std::string addOp = transformAddOp(treeNode->getChild()[1]->getDataType());
    if (treeNode->getChild()[2]->getTreeNodeType() == TreeNode::FACTOR ||
        treeNode->getChild()[2]->getDataType() == Token::TERM_EXP) {
        leftNodeStr = interpretExp(treeNode->getChild()[0]);
        rightNodeStr = interpretExp(treeNode->getChild()[2]);
        checkAddtiveExpIsRight(treeNode->getChild()[1]->getDataType());
        if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
            temp_1 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
        } else {
            temp_1 = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(addOp, leftNodeStr, rightNodeStr, temp_1, curStmtLineNo);
        }
    } else {
        leftNodeStr = interpretExp(treeNode->getChild()[0]);
        rightNodeStr = interpretExp(treeNode->getChild()[2]->getChild()[1]);
        checkAddtiveExpIsRight(treeNode->getChild()[1]->getDataType());
        if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
            temp_1 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
        } else {
            temp_1 = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(addOp, leftNodeStr, rightNodeStr, temp_1, curStmtLineNo);
        }
        treeNode = treeNode->getChild()[2];
        std::string temp_2;
        while (treeNode->getChild().size() > 2 && treeNode->getChild()[2]->getTreeNodeType() != TreeNode::FACTOR &&
               treeNode->getChild()[2]->getDataType() != Token::TERM_EXP) {
            addOp = transformAddOp(treeNode->getChild()[1]->getDataType());
            leftNodeStr = temp_1;
            rightNodeStr = interpretExp(treeNode->getChild()[2]->getChild()[0]);
            checkAddtiveExpIsRight(treeNode->getChild()[1]->getDataType());
            if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
                temp_2 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
            } else {
                temp_2 = symbolTable.getNewTempSymbol();
                quaternaryVector.emplace_back(addOp, leftNodeStr, rightNodeStr, temp_2, curStmtLineNo);
            }
            treeNode = treeNode->getChild()[2];
            temp_1 = temp_2;
        }
        addOp = transformAddOp(treeNode->getChild()[1]->getDataType());
        leftNodeStr = temp_1;
        rightNodeStr = interpretExp(treeNode->getChild()[2]);
        checkAddtiveExpIsRight(treeNode->getChild()[1]->getDataType());
        if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
            temp_2 = interpretAddtiveExpUtil(addOp, leftNodeStr, rightNodeStr, leftType, rightType);
        } else {
            temp_2 = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(addOp, leftNodeStr, rightNodeStr, temp_2, curStmtLineNo);
        }
        temp_1 = temp_2;
    }
    return temp_1;
}

//解释项
std::string Semantic::interpretTermExp(const TreeNode *treeNode) {
    ValueType leftType, rightType;
    std::string leftNodeStr, rightNodeStr, temp_1;
    std::string mulOp = transformMulOp(treeNode->getChild()[1]->getDataType());
    if (treeNode->getChild()[2]->getTreeNodeType() == TreeNode::FACTOR) {
        leftNodeStr = interpretExp(treeNode->getChild()[0]);
        rightNodeStr = interpretExp(treeNode->getChild()[2]);
        if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
            temp_1 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
        } else {
            temp_1 = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(mulOp, leftNodeStr, rightNodeStr, temp_1, curStmtLineNo);
        }
    } else {
        leftNodeStr = interpretExp(treeNode->getChild()[0]);
        rightNodeStr = interpretExp(treeNode->getChild()[2]->getChild()[0]);
        if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
            temp_1 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
        } else {
            temp_1 = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(mulOp, leftNodeStr, rightNodeStr, temp_1, curStmtLineNo);
        }
        treeNode = treeNode->getChild()[2];
        std::string temp_2;
        while (treeNode->getChild().size() > 2 && treeNode->getChild()[2]->getTreeNodeType() != TreeNode::FACTOR) {
            mulOp = transformMulOp(treeNode->getChild()[1]->getDataType());
            leftNodeStr = temp_1;
            rightNodeStr = interpretExp(treeNode->getChild()[2]->getChild()[0]);
            if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
                temp_2 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
            } else {
                temp_2 = symbolTable.getNewTempSymbol();
                quaternaryVector.emplace_back(mulOp, leftNodeStr, rightNodeStr, temp_2, curStmtLineNo);
            }
            treeNode = treeNode->getChild()[2];
            temp_1 = temp_2;
        }
        mulOp = transformMulOp(treeNode->getChild()[1]->getDataType());
        leftNodeStr = temp_1;
        rightNodeStr = interpretExp(treeNode->getChild()[2]);
        if (isConstant(leftNodeStr, leftType) && isConstant(rightNodeStr, rightType)) {
            temp_2 = interpretTermUtil(mulOp, leftNodeStr, rightNodeStr, leftType, rightType);
        } else {
            temp_2 = symbolTable.getNewTempSymbol();
            quaternaryVector.emplace_back(mulOp, leftNodeStr, rightNodeStr, temp_2, curStmtLineNo);
        }
        temp_1 = temp_2;
    }
    return temp_1;
}

//解释变量
std::string Semantic::interpretVar(const TreeNode *treeNode) {
    auto result = symbolTable.getVarSymbol(treeNode->getValue());
    if (result == nullptr)
        throw SemanticException(curStmtLineNo, "The var don't be declared");
    std::string varStr = treeNode->getValue();
    if (treeNode->isArray()) {
        varStr += ("[" + interpretScript(treeNode->getChild()[0]) + "]"); //这里要判断
    }
    if ((int) (result->getType()) >= 0 && (int) (result->getType()) <= 3)
        typeStack.push((ValueType) (result->getType()));
    else
        typeStack.push(ValueType::ADDR);
    return varStr;
}

/*//解释解除引用(*var)
std::string Semantic::interpretReference(const TreeNode *treeNode) //参数为*var
{
    int varType = symbolTable.getVarSymbol(treeNode->getChild()[0], treeNode->lineNo());
    std::string varStr = treeNode->getChild()[0]->getValue();
    if (treeNode->getChild()[0]->getChild()[0] != NULL)
        varStr += ("[" + interpretScript(treeNode->getChild()[0]->getChild()[0]) + "]");
    switch (varType) {
        case 0:
            throw SemanticException(curStmtLineNo, "无法对单值解除引用");
            break;
        case 1:
        case -1:
            typeStack.push(Value::SINGLE_VALUE);
            break;
        case 2:
            typeStack.push(Value::ADDR);
            break;
    }
    std::string temp = symbolTable.getNewTempSymbol();
    quaternaryVector.emplace_back(Quaternary::MUL, "", varStr, temp, curStmtLineNo);
    return temp;
}*/

//解释数组下标
std::string Semantic::interpretScript(const TreeNode *treeNode) {
    std::string script = interpretExp(treeNode); //这里要判断
    ValueType scriptType = typeStack.top();
    typeStack.pop();
    if (scriptType == ValueType::ADDR)
        throw SemanticException(curStmtLineNo, "Invalid array index");  //!!!!!!!!没有确定到int类型
    return script;
}

//解释声明语句辅助方法
void Semantic::checkDeclareIsRight(Token::TokenTag declarType) {
    ValueType valueType = typeStack.top();
    typeStack.pop();
    switch (declarType) {
        case Token::INT:
        case Token::REAL:
        case Token::CHAR:
            if (valueType == ValueType::ADDR)
                throw SemanticException(curStmtLineNo, "Data type don't match");
            break;
            /*case Token::INT_POINT:
            case Token::REAL_POINT:
                if (valueType != Value::ADDR)
                    throw CodeGeneraterException(m_curStmtLineNo, "类型不匹配");
                break;
            case Token::CHAR_POINT:
                if (valueType != Value::ADDR && valueType != Value::STR_VALUE)
                    throw CodeGeneraterException(m_curStmtLineNo, "类型不匹配");
                break;*/
        default:
            break;
    }
}

//检查赋值类型是否匹配
void Semantic::checkAssignIsRight(ValueType varType) {
    ValueType valueType = typeStack.top();
    typeStack.pop();
    if (varType == ValueType::INT_VALUE) {
        switch (valueType) {
            case ValueType::ADDR:
                throw SemanticException(curStmtLineNo, "Can't assign array to int");
            case ValueType::CHAR_VALUE:
                throw SemanticException(curStmtLineNo, "Can't assign char to int");
            default:
                break;
        }
    } else if (varType == ValueType::REAL_VALUE) {
        switch (valueType) {
            case ValueType::ADDR:
                throw SemanticException(curStmtLineNo, "Can't assign array to float");
            case ValueType::CHAR_VALUE:
                throw SemanticException(curStmtLineNo, "Can't assign char to float");
            default:
                break;
        }
    } else if (varType == ValueType::CHAR_VALUE) {
        switch (valueType) {
            case ValueType::REAL_VALUE:
                throw SemanticException(curStmtLineNo, "Can't assign float to char");
            default:
                break;
        }
    } else if (varType == ValueType::ADDR) {
        switch (valueType) {
            case ValueType::INT_VALUE:
            case ValueType::REAL_VALUE:
            case ValueType::CHAR_VALUE:
                throw SemanticException(curStmtLineNo, "Can't assign value to addr");
            default:
                break;
        }
    }
}

std::string Semantic::transformAddOp(int op) {
    if (op == Token::PLUS)
        return Quaternary::PLUS;
    else
        return Quaternary::MINUS;
}

std::string Semantic::interpretAddtiveExpUtil(const std::string &addOp, const std::string &leftNodeStr,
                                              const std::string &rightNodeStr, ValueType leftType,
                                              ValueType rightType) {
    std::string result;
    if (addOp == Quaternary::PLUS)
        result = Value(leftNodeStr, leftType) + Value(rightNodeStr, rightType);
    else
        result = Value(leftNodeStr, leftType) - Value(rightNodeStr, rightType);
    return result;
}

void Semantic::checkAddtiveExpIsRight(Token::TokenTag opType) {
    ValueType r_valueType = typeStack.top();
    typeStack.pop();
    ValueType l_valueType = typeStack.top();
    typeStack.pop();
    if (l_valueType != ValueType::ADDR && r_valueType != ValueType::ADDR) {
        typeStack.push((ValueType) ((int) l_valueType | (int) r_valueType));
    } else if (l_valueType == ValueType::ADDR && r_valueType != ValueType::ADDR) {
        typeStack.push(ValueType::ADDR);
    } else if (l_valueType != ValueType::ADDR && r_valueType == ValueType::ADDR) {
        if (opType == Token::PLUS)
            typeStack.push(ValueType::ADDR);
        else
            throw SemanticException(curStmtLineNo, "Sub can't be addr");
    } else {
        throw SemanticException(curStmtLineNo, "Data type can't sub");
    }
}

std::string Semantic::transformMulOp(int op) {
    if (op == Token::MUL)
        return Quaternary::MUL;
    else
        return Quaternary::DIV;
}

std::string
Semantic::interpretTermUtil(const std::string &mulOp, const std::string &leftNodeStr, const std::string &rightNodeStr,
                            ValueType leftType, ValueType rightType) {
    std::string result;
    if (mulOp == Quaternary::MUL) {
        result = Value(leftNodeStr, leftType) * Value(rightNodeStr, rightType);
    } else {
        Value::callfrom = curStmtLineNo;
        result = Value(leftNodeStr, leftType) / Value(rightNodeStr, rightType);
    }
    return result;
}

void Semantic::checkTermIsRight() {
    ValueType r_valueType = typeStack.top();
    typeStack.pop();
    ValueType l_valueType = typeStack.top();
    typeStack.pop();
    if (l_valueType != ValueType::ADDR && r_valueType != ValueType::ADDR)
        typeStack.push((ValueType) ((int) l_valueType | (int) r_valueType));
    else
        throw SemanticException(curStmtLineNo, "Data type can't add");
}

//检查是否是常量
bool Semantic::isConstant(const std::string &str, ValueType &type) {
    type = ValueType::INT_VALUE;
    for (_dataSize i = 0; i < str.size(); ++i) {
        char ch = str.at(i);
        if (isalpha(ch) || ch == '_' || ch == '@')
            return false;
        if (ch == '.')
            type = ValueType::REAL_VALUE;
    }
    return true;
}

//检查数组下标
bool Semantic::isSubscriptRight(const std::string &script) {
    for (_dataSize i = 0; i < script.size(); ++i)
        if (!isdigit(script.at(i)))
            return false;
    return true;
}

//将function类型转为Value类型
ValueType Semantic::transformFunToValue(SymbolType funType) {
    switch (funType) {
        case SymbolType::INT:
            return ValueType::INT_VALUE;
        case SymbolType::REAL:
            return ValueType::REAL_VALUE;
        case SymbolType::CHAR:
            return ValueType::CHAR_VALUE;
        default:
            return ValueType::ADDR;
    }
}


Semantic &myCodeGenerater() {
    static Semantic cg;
    return cg;
}
