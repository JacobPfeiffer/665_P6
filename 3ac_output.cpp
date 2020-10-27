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
    // makes a new procedure for function declarations
	Procedure * fnProc = prog->makeProc(myID->getName());
	size_t i = 1;
	// loops over all formals calling to 3AC on all formals
	for (auto formals : *myFormals){
	    formals->to3AC(fnProc);
	    // gets the symbol from the procedure with the coresponding semantic symbol
	    auto form = fnProc->getSymOpd(formals->ID()->getSymbol());
	    // creates an arg quad for each argument using the formal symbol and an index
	    auto arg = new GetArgQuad(i,form);
	    // adds the quad to the procedure
	    fnProc -> addQuad(arg);
	    i++;

	}
	// recurses into the body
	for (auto body : *myBody){
	    body->to3AC(fnProc);
	}	
}

void FnDeclNode::to3AC(Procedure * proc){
	//This never needs to be implemented,
	// the function only exists because of 
	// inheritance needs
}

// throw InternalError
void FormalDeclNode::to3AC(IRProgram * prog){
	throw new InternalError("function not well formed");
}

void FormalDeclNode::to3AC(Procedure * proc){
	proc->gatherFormal(this->ID()->getSymbol());
}

Opd * IntLitNode::flatten(Procedure * proc){
	return new LitOpd(std::to_string(myNum), QUADWORD);
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * CharLitNode::flatten(Procedure * proc){ 
	return new LitOpd(std::to_string(myVal), ADDR);
}

// Optional
Opd * NullPtrNode::flatten(Procedure * proc){
	return new LitOpd("0", ADDR);
}

Opd * TrueNode::flatten(Procedure * prog){
	return new LitOpd("1", BYTE);
}

Opd * FalseNode::flatten(Procedure * prog){
	return new LitOpd("0", BYTE);
}

Opd * AssignExpNode::flatten(Procedure * proc){
    // call flatten on dst and src
    auto dst = myDst->flatten(proc);
    auto src = mySrc->flatten(proc);
    // create an assign quad
    auto assign = new AssignQuad(dst,src);

    proc->addQuad(assign);
    return src;
}

// No implementation needed
Opd * LValNode::flatten(Procedure * proc){
	TODO(Implement me)
}

// optional 
Opd * DerefNode::flatten(Procedure * proc){
	TODO(Implement me)
}

// optional
Opd * RefNode::flatten(Procedure * proc){
	TODO(Implement me)
}

//TODO needs to be completed
Opd * CallExpNode::flatten(Procedure * proc){
    auto calleeIn = myID->getSymbol();
    auto call = new CallQuad(calleeIn);

    size_t i = 1;	
    for(auto args : *myArgs){
	auto flat = args->flatten(proc);
	auto saq = new SetArgQuad(i,flat);
	proc->addQuad(saq);
	i++;
    }
    proc->addQuad(call);

    auto itsVoid = calleeIn->getDataType()->asFn()->getReturnType()->isVoid();
    if(!itsVoid){
	auto tmp = proc->makeTmp(QUADWORD);
	auto ret = new GetRetQuad(tmp);
	proc->addQuad(ret);
	return tmp;
    }
    else{
	return nullptr;
    }
}

Opd * NegNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(QUADWORD);
	auto exp1 = myExp->flatten(proc);
	UnaryOp UnOp = NEG;
	UnaryOpQuad * myQuad =  new UnaryOpQuad(destination, UnOp, exp1);
	proc->addQuad(myQuad);
	return destination;
}

Opd * NotNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(QUADWORD);
	auto exp1 = myExp->flatten(proc);
	UnaryOp UnOp = NOT;
	UnaryOpQuad * myQuad =  new UnaryOpQuad(destination, UnOp, exp1);
	proc->addQuad(myQuad);
	return destination;
}

Opd * PlusNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(QUADWORD);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = ADD;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * MinusNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(QUADWORD);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = SUB;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * TimesNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(QUADWORD);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = MULT;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * DivideNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(QUADWORD);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = DIV;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * AndNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(BYTE);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = AND;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * OrNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(BYTE);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = OR;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

//TODO finish deciding on the desination
Opd * EqualsNode::flatten(Procedure * proc){
    // if exp1 is bool or char destination is BYTE
	auto destination = proc->makeTmp(QUADWORD);
    //else destination is QUADWORD
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = EQ;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

//TODO finish deciding on the desination
Opd * NotEqualsNode::flatten(Procedure * proc){
    // if exp1 is bool or char destination is BYTE
	auto destination = proc->makeTmp(QUADWORD);
    //else destination is QUADWORD
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = NEQ;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * LessNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(BYTE);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = LT;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * GreaterNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(BYTE);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = GT;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * LessEqNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(BYTE);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = LTE;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	auto destination = proc->makeTmp(BYTE);
	auto exp1 = myExp1->flatten(proc);
	auto exp2 = myExp2->flatten(proc);
	BinOp bin = GTE;
	BinOpQuad * myQuad =  new BinOpQuad(destination, bin, exp1, exp2);
	proc->addQuad(myQuad);
	return destination;
}

void AssignStmtNode::to3AC(Procedure * proc){
	myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
   auto lval = myLVal->flatten(proc);
   auto one = new LitOpd("1",QUADWORD);
   auto bin = new BinOpQuad(lval,ADD, lval,one );
    proc->addQuad(bin);

}

void PostDecStmtNode::to3AC(Procedure * proc){
   auto lval = myLVal->flatten(proc);
   auto one = new LitOpd("1",QUADWORD);
   auto bin = new BinOpQuad(lval,SUB, lval,one );
    proc->addQuad(bin);
}

void FromConsoleStmtNode::to3AC(Procedure * proc){
    auto dst = myDst->flatten(proc);
    auto trins = new IntrinsicQuad(OUTPUT,dst);
    proc->addQuad(trins);
}

void ToConsoleStmtNode::to3AC(Procedure * proc){
    auto src = mySrc->flatten(proc);
    auto trins = new IntrinsicQuad(INPUT,src);
    proc->addQuad(trins);
}

void IfStmtNode::to3AC(Procedure * proc){
    auto cond = myCond->flatten(proc);
    auto lbl = proc->makeLabel();
    auto jq = new JmpIfQuad(cond,lbl);
    proc->addQuad(jq);
    for(auto stmt : *myBody){
	stmt->to3AC(proc);
    }
    auto nop = new NopQuad();
    nop->addLabel(lbl);
    proc->addQuad(nop);
    
}

void IfElseStmtNode::to3AC(Procedure * proc){
    auto cond = myCond->flatten(proc);
    auto lbl = proc->makeLabel();
    auto jq = new JmpIfQuad(cond,lbl);
    proc->addQuad(jq);
    for(auto stmt : *myBodyTrue){
	stmt->to3AC(proc);
    }
    // if condition is true jump to nop
    auto nop = new NopQuad();
    nop->addLabel(lbl);

    auto lbl2 = proc->makeLabel();
    // else evaluate the else
    auto jmp = new JmpQuad(lbl2);
    proc->addQuad(jmp);
    proc->addQuad(nop);

    for(auto stmt : *myBodyFalse){
	stmt->to3AC(proc);
    }
    auto nop2 = new NopQuad();
    nop2->addLabel(lbl2);
    proc->addQuad(nop2);

}

void WhileStmtNode::to3AC(Procedure * proc){
    auto lbl = proc->makeLabel();
    auto lbl2 = proc->makeLabel();

    auto nop = new NopQuad();
    nop->addLabel(lbl);
    proc->addQuad(nop);

    auto cond = myCond->flatten(proc);

    auto jq = new JmpIfQuad(cond,lbl2);
    proc->addQuad(jq);
    for(auto stmt : *myBody){
	stmt->to3AC(proc);
    }

    auto jmp = new JmpQuad(lbl);
    proc->addQuad(jmp);

    auto nop2 = new NopQuad();
    nop2->addLabel(lbl2);
    proc->addQuad(nop2);
}

void CallStmtNode::to3AC(Procedure * proc){
    myCallExp->flatten(proc);
}

void ReturnStmtNode::to3AC(Procedure * proc){
    auto exp = myExp->flatten(proc);
    auto sret = new SetRetQuad(exp);
    proc->addQuad(sret);
    //goto proc->getLeaveLabel
    auto jmp = new JmpQuad(proc->getLeaveLabel());
    proc->addQuad(jmp);
	
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
    // return opd pointer to the semantic symbol stored in IDNode
    auto opd = proc->getSymOpd(mySymbol);
    return opd;
}


}
