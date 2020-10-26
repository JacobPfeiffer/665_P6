#include "ast.hpp"

namespace holeyc{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

//VarDeclNode at bottom

void FnDeclNode::to3AC(IRProgram * prog){
	//TODO(Implement me)
	//unfinished
	Procedure * proc = new Procedure(prog, myID);
	for (auto formals : *myFormals) {
		formals->to3AC(proc);
	}
}

void FnDeclNode::to3AC(Procedure * proc){
	//This never needs to be implemented,
	// the function only exists because of 
	// inheritance needs
}

void FormalDeclNode::to3AC(IRProgram * prog){
	//TODO(Implement me)
}

void FormalDeclNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

Opd * IntLitNode::flatten(Procedure * proc){
	return new LitOpd(std::to_string(myNum), QUADWORD);
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * CharLitNode::flatten(Procedure * proc){ //test
	//TODO(Implement me)
	return new LitOpd(std::to_string(myVal), ADDR);
}

Opd * NullPtrNode::flatten(Procedure * proc){
	return new LitOpd("0", ADDR);
	//TODO(Implement me)
}

Opd * TrueNode::flatten(Procedure * prog){
	//TODO(Implement me)
}

Opd * FalseNode::flatten(Procedure * prog){
	//TODO(Implement me)
}

Opd * AssignExpNode::flatten(Procedure * proc){
	//TODO(Implement me)
}

Opd * LValNode::flatten(Procedure * proc){
	//TODO(Implement me)
}

Opd * DerefNode::flatten(Procedure * proc){
	//TODO(Implement me)
}

Opd * RefNode::flatten(Procedure * proc){
	//TODO(Implement me)
}

Opd * CallExpNode::flatten(Procedure * proc){
	//TODO(Implement me)
}

Opd * NegNode::flatten(Procedure * proc){
	//TODO(Implement me)
	UnaryOp un = NEG;
	return un;
}

Opd * NotNode::flatten(Procedure * proc){
	//TODO(Implement me)
	UnaryOp un = NOT;
	return un;
}

Opd * PlusNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = ADD;
	return bin;
}

Opd * MinusNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = SUB;
	return bin;
}

Opd * TimesNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = MULT;
	return bin;
}

Opd * DivideNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = DIV;
	return bin;
}

Opd * AndNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = AND;
	return bin;
}

Opd * OrNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = OR;
	return bin;
}

Opd * EqualsNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = EQ;
	return bin;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = NEQ;
	return bin;
}

Opd * LessNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = LT;
	return bin;
}

Opd * GreaterNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = GT;
	return bin;
}

Opd * LessEqNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = LTE;
	return bin;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	//TODO(Implement me)
	BinOp bin = GTE;
	return bin;
}

void AssignStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	myLVal->flatten(proc);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	myLVal->flatten(proc);
}

void FromConsoleStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void ToConsoleStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void IfStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void IfElseStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void WhileStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void CallStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void ReturnStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * sym = ID()->getSymbol();
	if (sym == nullptr){
		throw new InternalError("null sym");
	}
	proc->gatherLocal(sym);
}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sym = ID()->getSymbol();
	if (sym == nullptr){
		throw new InternalError("null sym");
	}
	
	prog->gatherGlobal(sym);
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent) 
Opd * IDNode::flatten(Procedure * proc){
	TODO(Implement me)
}


}
