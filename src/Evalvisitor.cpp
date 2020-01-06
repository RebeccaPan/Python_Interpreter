#include "Evalvisitor.h"
#include "BigInt.h"
#include "Alltype.h"

#include <map>
std::map<string, Alltype> varList;

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
  return visitChildren(ctx);
}
//funcStart
antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
  return visitTypedargslist(ctx->typedargslist());
}

antlrcpp::Any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
    return visitChildren(ctx);
}
//funcEnd
antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  if (ctx->simple_stmt() != nullptr) return visitSimple_stmt(ctx->simple_stmt());
  if (ctx->compound_stmt() != nullptr) return visitCompound_stmt(ctx->compound_stmt());
  return nullptr;
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  return visitSmall_stmt(ctx->small_stmt());
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  if (ctx->expr_stmt() != nullptr) return visitExpr_stmt(ctx->expr_stmt());
  if (ctx->flow_stmt() != nullptr) return visitFlow_stmt(ctx->flow_stmt());
  return Alltype();
}

antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
//expr_stmt: testlist ( (augassign testlist) |
//                     ('=' testlist)*);//连等 加等/减等/...
  if (ctx->testlist().size() == 1) return visitTestlist(ctx->testlist()[0]);
  if (ctx->augassign() != nullptr) {
    int op = visitAugassign(ctx->augassign()).as<int>();
    vector<Alltype> lt = visitTestlist(ctx->testlist()[0]).as<vector<Alltype> >();
    vector<Alltype> rt = visitTestlist(ctx->testlist()[1]).as<vector<Alltype> >();
    switch (op) {
    case 0:
      varList[lt[0].valName] = lt[0] + rt[0];
      break;
    case 1:
      varList[lt[0].valName] = lt[0] - rt[0];
      break;
    case 2:
      varList[lt[0].valName] = lt[0] * rt[0];
      break;
    case 3:
      varList[lt[0].valName] = lt[0] / rt[0];
      break;
    case 4:
      varList[lt[0].valName] = lt[0] / rt[0];//need fix
      break;
    case 5:
      varList[lt[0].valName] = lt[0] % rt[0];
      break;
    }
    return Alltype();
  }
  if (ctx->ASSIGN().size() != 0) {
    int varNum = ctx->testlist().size();
    vector<Alltype> allVal = visitTestlist(ctx->testlist()[varNum-1]).as<vector<Alltype> >();
    for (int i = 0; i < ctx->testlist().size()-1; ++i) {
        vector<Alltype> tmpTest = visitTestlist(ctx->testlist(i)).as<vector<Alltype> >();
        for (int j = 0; j < tmpTest.size(); ++j) {
          varList[tmpTest[j].valName] = allVal[j];
        }
    }
    return Alltype();
  }
}

antlrcpp::Any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
  if (ctx->ADD_ASSIGN()  != nullptr) return 0;// +=
  if (ctx->SUB_ASSIGN()  != nullptr) return 1;// -=
  if (ctx->MULT_ASSIGN() != nullptr) return 2;// *=
  if (ctx->DIV_ASSIGN()  != nullptr) return 3;// /=
  if (ctx->IDIV_ASSIGN() != nullptr) return 4;// //=
  if (ctx->MOD_ASSIGN()  != nullptr) return 5;// %=
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  if (ctx->break_stmt() != nullptr) return visitBreak_stmt(ctx->break_stmt()).as<Alltype>();
  if (ctx->continue_stmt() != nullptr) return visitContinue_stmt(ctx->continue_stmt()).as<Alltype>();
  if (ctx->return_stmt() != nullptr) return visitReturn_stmt(ctx->return_stmt()).as<Alltype>();
}

antlrcpp::Any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  if (ctx->if_stmt() != nullptr) return visitIf_stmt(ctx->if_stmt()).as<Alltype>();
  if (ctx->while_stmt() != nullptr) return visitWhile_stmt(ctx->while_stmt()).as<Alltype>();
  if (ctx->funcdef() != nullptr) return visitFuncdef(ctx->funcdef()).as<Alltype>();
  return Alltype();
}

antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  return visitOr_test(ctx->or_test()).as<Alltype>();
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  if (ctx->OR().size() == 0) return visitAnd_test(ctx->and_test()[0]).as<Alltype>();
  Alltype ans = visitAnd_test(ctx->and_test()[0]).as<Alltype>();
  for (int i = 0; i < ctx->OR().size(); ++i) {
    if ((bool)ans == true) return ans;
    ans = ans || visitAnd_test(ctx->and_test()[i]).as<Alltype>();
  }
  return ans;
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  if (ctx->AND().size() == 0) return visitNot_test(ctx->not_test()[0]).as<Alltype>();
  Alltype ans = visitNot_test(ctx->not_test()[0]).as<Alltype>();
  for (int i = 0; i < ctx->AND().size(); ++i) {
    if ((bool)ans == false) return ans;
    ans = ans && visitNot_test(ctx->not_test()[i]).as<Alltype>();
  }
  return ans;
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  if (ctx->not_test() != nullptr) return visitNot_test(ctx->not_test()).as<Alltype>();
  else return visitComparison(ctx->comparison()).as<Alltype>();
}

antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  if (ctx->comp_op().size() == 0) return visitArith_expr(ctx->arith_expr()[0]).as<Alltype>();
  Alltype left = visitArith_expr(ctx->arith_expr()[0]).as<Alltype>();
  for (int i = 0; i < ctx->comp_op().size(); ++i) {
    switch (visitComp_op(ctx->comp_op()[i]).as<int>()) {
    case 0://<
      if (left >= visitArith_expr(ctx->arith_expr()[i+1]).as<Alltype>()) return Alltype(false);
      break;
    case 1://>
      if (left <= visitArith_expr(ctx->arith_expr()[i+1]).as<Alltype>()) return Alltype(false);
      break;
    case 2://==
      if (left != visitArith_expr(ctx->arith_expr()[i+1]).as<Alltype>()) return Alltype(false);
      break;
    case 3://>=
      if (left <  visitArith_expr(ctx->arith_expr()[i+1]).as<Alltype>()) return Alltype(false);
      break;
    case 4://<=
      if (left >  visitArith_expr(ctx->arith_expr()[i+1]).as<Alltype>()) return Alltype(false);
      break;
    case 5://!=
      if (left == visitArith_expr(ctx->arith_expr()[i+1]).as<Alltype>()) return Alltype(false);
      break;
    }
    return Alltype(true);
  }
}

antlrcpp::Any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
  if (ctx->LESS_THAN()    != nullptr) return 0;
  if (ctx->GREATER_THAN() != nullptr) return 1;
  if (ctx->EQUALS()       != nullptr) return 2;
  if (ctx->GT_EQ()        != nullptr) return 3;
  if (ctx->LT_EQ()        != nullptr) return 4;
  if (ctx->NOT_EQ_2()     != nullptr) return 5;
}

antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  if (ctx->add_sub().size() == 0) return visitTerm(ctx->term()[0]).as<Alltype>();
  Alltype ans = visitTerm(ctx->term()[0]).as<Alltype>();
  for (int i = 0; i < ctx->add_sub().size(); ++i) {
    if (ctx->add_sub()[i]->ADD() != nullptr) ans += visitTerm(ctx->term()[i]).as<Alltype>();
    if (ctx->add_sub()[i]->MINUS() != nullptr) ans -= visitTerm(ctx->term()[i]).as<Alltype>();
  }
  return ans;
}

antlrcpp::Any EvalVisitor::visitAdd_sub(Python3Parser::Add_subContext *ctx) {//not used
  return Alltype();
}

antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  if (ctx->mtp_div().size() == 0) {
    return visitFactor(ctx->factor()[0]).as<Alltype>();
  }
  Alltype ans = visitFactor(ctx->factor()[0]).as<Alltype>();
  for (int i = 0; i < ctx->mtp_div().size(); ++i) {
    if (ctx->mtp_div()[i]->STAR() != nullptr) ans *= visitFactor(ctx->factor()[i]).as<Alltype>();
    if (ctx->mtp_div()[i]->DIV()  != nullptr) ans /= visitFactor(ctx->factor()[i]).as<Alltype>();
    if (ctx->mtp_div()[i]->IDIV() != nullptr) ans /= visitFactor(ctx->factor()[i]).as<Alltype>();//need fix
    if (ctx->mtp_div()[i]->MOD()  != nullptr) ans %= visitFactor(ctx->factor()[i]).as<Alltype>();
  }
  return ans;
}

antlrcpp::Any EvalVisitor::visitMtp_div(Python3Parser::Mtp_divContext *ctx) {//not used
  return Alltype();
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  if (ctx->atom_expr() != nullptr) return visitAtom_expr(ctx->atom_expr()).as<Alltype>();
  else {
    bool minus = (ctx->MINUS() != nullptr)?true:false;
    if (!minus) return visitFactor(ctx->factor()).as<Alltype>();
    else return (Alltype(BigInt(0, 0)) - visitFactor(ctx->factor()).as<Alltype>());
  }
}

antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
  if (ctx->trailer() == nullptr) return visitAtom(ctx->atom()).as<Alltype>();
  string funcName = ctx->atom()->NAME()->toString();
  if (funcName == "print") {
    if (ctx->trailer()->arglist() == nullptr) return Alltype();
    //vector<Alltype> toBePrinted;
    //currentWorkPlace
    Alltype rt = visitArgument(ctx->trailer()->arglist()->argument());//
    for (int i = 0; i < ctx->trailer()->arglist()->argument().size(); ++i) {
      //cerr << "CK" << i;
      //toBePrinted.push_back(visitTest(ctx->trailer()->arglist()->argument()[i]->test()).as<Alltype>());
      if (i) cout << ' ';
      cout << visitTest(ctx->trailer()->arglist()->argument()[i]->test()).as<Alltype>();
    }
    cout << endl;
    /*cout << toBePrinted[0];
    for (int i = 1; i < toBePrinted.size(); ++i) {
      cerr << "CK3";
      cout << ' ' << toBePrinted[i];
    }
    cout << std::endl;*/
    return Alltype();
  }
  if (funcName == "int") {
    return Alltype((BigInt)visitTest(ctx->trailer()->arglist()->argument()[0]->test()).as<Alltype>());
  }
  if (funcName == "float"){
    return Alltype((double)visitTest(ctx->trailer()->arglist()->argument()[0]->test()).as<Alltype>());
  }
  if (funcName == "str"){
    return Alltype((string)visitTest(ctx->trailer()->arglist()->argument()[0]->test()).as<Alltype>());
  }
  if (funcName == "bool"){
    return Alltype((bool)visitTest(ctx->trailer()->arglist()->argument()[0]->test()).as<Alltype>());
  }
  //TODO
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  return visitArglist(ctx->arglist()).as<Alltype>();
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
  if (ctx->NAME() != nullptr) {
    Alltype ans; ans.valName = ctx->NAME()->toString();
    if (varList.find(ans.valName) == varList.end()) {//notFound
      //cerr << "1:" << ans.valName << '\n';
      varList.insert(std::pair<std::string, Alltype>(ans.valName, ans));
      return ans;
    }
    else {//found
      //cerr << "2:" << ans.valName << '\n';
      ans = varList[ans.valName];
      return ans;
    }
  }
  if (ctx->NUMBER() != nullptr) {
    string st = ctx->NUMBER()->toString();
    Alltype tmp(st);
    if (st.find('.')) {Alltype db((double)tmp); return db;}
    else {Alltype num((BigInt)tmp); return num;}
  }
  if (ctx->NONE()  != nullptr) return Alltype();
  if (ctx->TRUE()  != nullptr) return Alltype(true);
  if (ctx->FALSE() != nullptr) return Alltype(false);
  if (ctx->test()  != nullptr) return visitTest(ctx->test()).as<Alltype>();
  if (ctx->STRING().size() != 0) {
    string ans = ctx->STRING()[0]->toString();
    for (int i = 1; i < ctx->STRING().size(); ++i) {
      ans = ans + ctx->STRING()[i]->toString();
    }
    string _ans;
    for (int i = 1; i < ans.length()-1; ++i) _ans = _ans + ans[i];
    return Alltype(_ans);
  }
}

antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
   vector<Alltype> tempVctr;
   for (int i = 0; i < ctx->test().size(); ++i) {
     tempVctr.push_back(visitTest(ctx->test()[i]).as<Alltype>());
   }
   return tempVctr;
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
  return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  if (ctx->NAME() == nullptr) return visitTest(ctx->test()).as<Alltype>();
  Alltype rt(visitTest(ctx->test()).as<Alltype>()); rt.valName = ctx->test()->toString();
  if (varList.find(rt.valName) == varList.end()) {//notFound
    varList.insert(std::pair<std::string, Alltype>(rt.valName, rt));
    return rt;
  }
  else {//found
    varList[rt.valName] = rt;
    return rt;
  }
}