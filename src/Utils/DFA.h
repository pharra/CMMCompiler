//
// Created by WF on 2018/9/9.
//

/**
 * 文法：
 * S->A
 * A->(
 * A->)
 * A->}
 * A->{
 * A->[
 * A->]
 * A->,
 * A->;
 * A->/B
 * B->*
 * B->/
 * A->*C
 * C->/
 * A->+D
 * A->-D
 * A->*D
 * A->/D
 * D->=
 * D->null
 *
 *
 *
 */

#ifndef CMMCOMPILER_DFA_H
#define CMMCOMPILER_DFA_H


#include <unordered_map>

enum AcceptType {
    ACCEPT
};

struct State {
    AcceptType type;
};



class DFA {
private:
    static std::unordered_map<std::string,State> analysisTable;
};


#endif //CMMCOMPILER_DFA_H
