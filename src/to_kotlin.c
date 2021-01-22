#ifndef WAX_TO_KOTLIN
#define WAX_TO_KOTLIN

#include "text.c"
#include "parser.c"
#include "common.c"

str_t type_to_kotlin(type_t* typ, int is_obj){
  str_t out = str_new();
  if (typ->tag == TYP_INT){
    str_add(&out,"Int");
  }else if (typ->tag == TYP_FLT){
    str_add(&out,"Float");
  }else if (typ->tag == TYP_STT){
    str_add(&out,typ->name.data);
  }else if (typ->tag == TYP_ARR){
    str_add(&out,"ArrayList<");
    str_add(&out,type_to_kotlin(typ->elem0,1).data);
    str_add(&out,">");
  }else if (typ->tag == TYP_VEC){
    str_add(&out,"Array<");
	str_add(&out,type_to_kotlin(typ->elem0,0).data);
	str_add(&out,">");
  }else if (typ->tag == TYP_MAP){
    str_add(&out,"HashMap<");
    str_add(&out,type_to_kotlin(typ->elem0,1).data);
    str_add(&out,",");
    str_add(&out,type_to_kotlin(typ->elem1,1).data);
    str_add(&out,">");
  }else if (typ->tag == TYP_STR){
    str_add(&out,"String");
  }else{
    str_add(&out,"/*type?*/");
  }
  return out;
}
str_t zero_to_kotlin(type_t* typ){
  str_t out = str_new();
  if (typ->tag == TYP_INT){
    str_add(&out,"0");
  }else if (typ->tag == TYP_FLT){
    str_add(&out,"0.0f");
  }else if (typ->tag == TYP_STT){
    str_add(&out,"null");
  }else if (typ->tag == TYP_ARR){
    str_add(&out,"null");
  }else if (typ->tag == TYP_VEC){
    str_add(&out,"null");
  }else if (typ->tag == TYP_MAP){
    str_add(&out,"null");
  }else if (typ->tag == TYP_STR){
    str_add(&out,"null");
  }else{
    str_add(&out,"/*zero?*/");
  }
  return out;
}

str_t vec_init_kotlin(type_t* typ){
  str_t out = str_new();
  int num_br = 0;
  type_t* t = typ->elem0;
  while (t->tag == TYP_VEC){
    num_br ++;
    t = t->elem0;
  }
  //str_addch(&out,'Array<');
  //str_add(&out,type_to_kotlin(t,0).data);
  //str_addch(&out,'>(');
  str_addch(&out,'(');
  char s[32];
  sprintf(s,"%d",typ->size);
  str_add(&out,s);
  //str_addch(&out,')');
  for (int i = 0; i < num_br; i++){
    str_add(&out,"()");
  }
  return out;
}


str_t expr_to_kotlin(expr_t* expr, int indent){
  // print_syntax_tree(expr,4);
  // printf("-----\n");
  str_t out = str_new();
  INDENT2(indent);

  if (expr->key == EXPR_LET){
    str_add(&out,"var ");
    str_add(&out, ((tok_t*)(CHILD1->term))->val.data);
	str_add(&out,":");
	str_add(&out,type_to_kotlin( (type_t*)(CHILD2->term) ,0).data);
    str_add(&out,"=");
    str_add(&out,zero_to_kotlin( (type_t*)(CHILD2->term) ).data);
    
  }else if (expr->key == EXPR_SET){
    str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,"=");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );

  }else if (expr->key == EXPR_TERM){
    tok_t* tok = ((tok_t*)(expr->term));
    if (tok->tag == TOK_INT){
      if (tok->val.data[0] == '\''){
        str_add(&out, "");
      }
    }
    str_add(&out, tok->val.data);
    if (tok->tag == TOK_FLT){
      str_add(&out, "f");
    }
  
  }else if (expr->key == EXPR_IADD || expr->key == EXPR_FADD ||
            expr->key == EXPR_ISUB || expr->key == EXPR_FSUB ||
            expr->key == EXPR_IMUL || expr->key == EXPR_FMUL ||
            expr->key == EXPR_IDIV || expr->key == EXPR_FDIV ||
            expr->key == EXPR_BAND ||
            expr->key == EXPR_BOR  ||

            expr->key == EXPR_IMOD || expr->key == EXPR_FMOD ||
            expr->key == EXPR_XOR  ||
            expr->key == EXPR_SHL  || expr->key == EXPR_SHR
    ){
    str_add(&out, "((");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")");
    str_add(&out, expr->rawkey.data);
    str_add(&out, "(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, "))");

  }else if (expr->key == EXPR_LAND){
	INDENT2(indent);
    str_add(&out, "w_INT(w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")&&w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, "))");

  }else if (expr->key == EXPR_LOR){
    str_add(&out, "w_INT(w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")||w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, "))");

  }else if (expr->key == EXPR_IGT ||
            expr->key == EXPR_ILT ||
            expr->key == EXPR_IGEQ||
            expr->key == EXPR_ILEQ
  ){
	INDENT2(indent);
    str_add(&out, "w_INT((");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")");
    str_add(&out, expr->rawkey.data);
    str_add(&out, "(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, ").toInt()).toInt()");

  }else if (expr->key == EXPR_FGT  ||
            expr->key == EXPR_FLT  ||
            expr->key == EXPR_FGEQ ||
            expr->key == EXPR_FLEQ
  ){
	INDENT2(indent);
    str_add(&out, "w_INT((");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")");
    str_add(&out, expr->rawkey.data);
    str_add(&out, "(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, ").toFloat()).toFloat()");

  }else if (expr->key == EXPR_IEQ || expr->key == EXPR_FEQ || expr->key == EXPR_PTREQL){
    str_add(&out, "w_INT((");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")==(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, "))");

  }else if (expr->key == EXPR_INEQ || expr->key == EXPR_FNEQ || expr->key == EXPR_PTRNEQ){

    str_add(&out, "w_INT((");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")!=(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data );
    str_add(&out, "))");

  }else if (expr->key == EXPR_BNEG ){
    str_add(&out, "(");
    str_add(&out, expr->rawkey.data);
    str_add(&out, "(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, "))");

  }else if (expr->key == EXPR_LNOT ){
    str_add(&out, "w_NOT(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data );
    str_add(&out, ")");

  }else if (expr->key == EXPR_IF){
    str_add(&out, "if(w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out, ")){\n");
    str_add(&out, expr_to_kotlin(CHILD2,indent).data);
    INDENT2(indent);
    str_add(&out, "}");
    if (CHILD3){

      str_add(&out, "else{\n");
      str_add(&out, expr_to_kotlin(CHILD3,indent).data);
      INDENT2(indent);
      str_add(&out, "}\n");
      indent=-1;
    }

  }else if (expr->key == EXPR_TIF){
    str_add(&out, "(w_BOOL(if ");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out, ")(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data);
    str_add(&out, ") else (");
    str_add(&out, expr_to_kotlin(CHILD3,-1).data);
    str_add(&out, "))");

  }else if (expr->key == EXPR_WHILE){
    str_add(&out, "while(w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out, ")){\n");
	INDENT2(indent);
	str_add(&out, expr_to_kotlin(CHILD2,indent).data);
	INDENT2(indent);
    str_add(&out, "}\n");

  }else if (expr->key == EXPR_FOR){
	str_add(&out, "var ");
	str_add(&out, expr_to_kotlin(CHILD1,-1).data);
	str_add(&out, ": Int");
    str_add(&out, "=(");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data);
    str_add(&out, ")\n");
	INDENT2(indent);
    str_add(&out, "while (");
	str_add(&out, "w_BOOL(");
    str_add(&out, expr_to_kotlin(CHILD3,-1).data);
    str_add(&out, ")) {\n");
	//INDENT2(indent);
	str_add(&out, expr_to_kotlin(CHILDN,indent).data);
	//str_add(&out, "\n");
	INDENT2(indent);
	str_add(&out, "}\n");
	INDENT2(indent);
	str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out, "+=");
	str_add(&out, expr_to_kotlin(CHILD4,-1).data);
	
  }else if (expr->key == EXPR_FORIN){
    str_t itname = tmp_name("tmp__it_");

    str_add(&out, "for(Map.Entry<");
    str_add(&out, type_to_kotlin(CHILD3->type->elem0,1).data);
    str_add(&out, ",");
    str_add(&out, type_to_kotlin(CHILD3->type->elem1,1).data);
    str_add(&out, "> ");
    str_add(&out, itname.data);
    str_add(&out, ":(");
    str_add(&out, expr_to_kotlin(CHILD3,-1).data);
    str_add(&out, ").entrySet()){\n");

    INDENT2(indent+1);
    str_add(&out, type_to_kotlin(CHILD3->type->elem0,0).data);
    str_add(&out, " ");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out, "=(");
    str_add(&out, type_to_kotlin(CHILD3->type->elem0,0).data);
    str_add(&out, ")(");
    str_add(&out, itname.data);
    str_add(&out, ".getKey());\n");

    INDENT2(indent+1);
    str_add(&out, type_to_kotlin(CHILD3->type->elem1,0).data);
    str_add(&out, " ");
    str_add(&out, expr_to_kotlin(CHILD2,-1).data);
    str_add(&out, "=(");
    str_add(&out, type_to_kotlin(CHILD3->type->elem1,0).data);
    str_add(&out, ")(");
    str_add(&out, itname.data);
    str_add(&out, ".getValue());\n");

    INDENT2(indent+1);
    str_add(&out, "{\n");
    str_add(&out, expr_to_kotlin(CHILDN,indent+2).data);
    INDENT2(indent+1);
    str_add(&out, "}\n");

    INDENT2(indent);
    str_add(&out, "}");


  }else if (expr->key == EXPR_FUNC || expr->key == EXPR_FUNCHEAD){
    str_add(&out, "public ");
    list_node_t* it = expr->children.head;
    while(it){
      expr_t* ex = (expr_t*)(it->data);
      if (ex->key == EXPR_RESULT){
        break;
      }
      it = it->next;
    }

    str_add(&out,"fun");
    str_add(&out, " ");
    str_t funcname = ((tok_t*)(CHILD1->term))->val;
	
	if (str_eq(&funcname,"main")){
      funcname = str_from("main_",5);
    }
    str_add(&out, funcname.data);
    str_add(&out, "(");
    it = expr->children.head->next;
    while(it){
      expr_t* ex = (expr_t*)(it->data);
      if (ex->key != EXPR_PARAM){
        break;
      }
      if (it != expr->children.head->next){
        str_add(&out,",");
      }
      // str_add(&out,ex->rawkey.data);
	  str_add(&out, ((tok_t*)(((expr_t*)(((expr_t*)(it->data))->children.head->data))->term))->val.data);
	  str_add(&out,":");
      str_add(&out,type_to_kotlin(  (type_t*)(((expr_t*)(((expr_t*)(it->data))->children.head->next->data))->term) ,0).data);
      it = it->next;
    }
    if (expr->key == EXPR_FUNC){
      str_add(&out, ")");
	  if (it && ((expr_t*)(it->data))->key == EXPR_RESULT) {
		str_add(&out, ":");
		str_add(&out,type_to_kotlin(  ((expr_t*)(((expr_t*)(it->data))->children.head->data))->type ,0).data);
	  }
	  str_add(&out, "{\n");
      str_add(&out, expr_to_kotlin(CHILDN,indent).data);
      INDENT2(indent);
      str_add(&out, "}");
    }else{
      str_add(&out, ")");
    }

  }else if (expr->key == EXPR_CALL){
    str_t funcname = ((tok_t*)(CHILD1->term))->val;
    if (str_eq(&funcname,"main")){
      funcname = str_from("main_",5);
    }
    
    str_add(&out, funcname.data);
    str_add(&out, "(");
    list_node_t* it = expr->children.head->next;
    while(it){
      expr_t* ex = (expr_t*)(it->data);
      if (ex->key == EXPR_RESULT){
        break;
      }
      if (it != expr->children.head->next){
        str_add(&out,",");
      }

      str_add(&out, expr_to_kotlin(((expr_t*)(it->data)),-1).data );

      it = it->next;
    }
    str_add(&out, ")");
	if (expr->type->tag == TYP_FLT){
      //fuck double
      (str_add(&out, ".toFloat()"));
    }
  }else if (expr->key == EXPR_THEN || expr->key == EXPR_ELSE || expr->key == EXPR_DO || expr->key == EXPR_FUNCBODY){
    list_node_t* it = expr->children.head;
    if (!it){
      str_add(&out,"\n");
    }
    while(it){
      expr_t* ex = (expr_t*)(it->data);
      if (it==(expr->children.head)){
        str_add(&out,(char*)&expr_to_kotlin(ex,indent+1).data[indent*2]);
      }else{
        str_add(&out,expr_to_kotlin(ex,indent+1).data);
      }
      it = it->next;
    }

    indent=-1;

  }else if (expr->key == EXPR_CAST){
    type_t* typl = CHILD1->type;
    type_t* typr = (type_t*)(CHILD2->term);
    if (typl->tag == TYP_INT && typr->tag == TYP_FLT){
      str_add(&out, "(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ").toFloat()");
    }else if (typl->tag == TYP_FLT && typr->tag == TYP_INT){
      str_add(&out, "(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ").toInt()");
    }else if (typl->tag == TYP_INT && typr->tag == TYP_STR){
      str_add(&out, "String.valueOf(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ")");
    }else if (typl->tag == TYP_FLT && typr->tag == TYP_STR){
      str_add(&out, "String.valueOf(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ")");
    }else if (typl->tag == TYP_STR && typr->tag == TYP_INT){
      str_add(&out, "(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ").toInt()");
    
    }else if (typl->tag == TYP_STR && typr->tag == TYP_FLT){
      str_add(&out, "(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ").toFloat()");
    }else{
      str_add(&out, "(");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out, ")");
    }
  }else if (expr->key == EXPR_RETURN){
    str_add(&out,"return");
    if (CHILD1){
      str_add(&out," ");
      str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    }
  }else if (expr->key == EXPR_STRUCT){
    str_add(&out,"public class ");
    str_add(&out, ((tok_t*)(CHILD1->term))->val.data);
    str_add(&out,"{\n");

    list_node_t* it = expr->children.head->next;

    while(it){
      // expr_t* ex = (expr_t*)(it->data);
      INDENT2(indent+1);
      str_add(&out,"var ");
	  str_add(&out, ((tok_t*)(((expr_t*)(((expr_t*)(it->data))->children.head->data))->term))->val.data);
      str_add(&out,":");
      str_add(&out,type_to_kotlin(  (type_t*)(((expr_t*)(((expr_t*)(it->data))->children.head->next->data))->term) ,0).data);
      str_add(&out,"=");
      str_add(&out,zero_to_kotlin(  (type_t*)(((expr_t*)(((expr_t*)(it->data))->children.head->next->data))->term) ).data);
      str_add(&out,";\n");
      it = it->next;
    }
    INDENT2(indent);
    str_add(&out,"}");

  }else if (expr->key == EXPR_NOTNULL){
	INDENT2(indent);
    str_add(&out,"w_INT(");
    str_add(&out, expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,"!=null)");

  }else if (expr->key == EXPR_SETNULL){

    if (!CHILD2){
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out,"=null");
    }else{
      if (CHILD1->type->tag == TYP_STT){
        str_add(&out,"(");
        str_add(&out,expr_to_kotlin(CHILD1,-1).data);
        str_add(&out,").");
        str_add(&out,expr_to_kotlin(CHILD2,-1).data);
        str_add(&out,"=null");
      }else if (CHILD1->type->tag == TYP_ARR){
        str_add(&out,"(");
        str_add(&out,expr_to_kotlin(CHILD1,-1).data);
        str_add(&out,").set(");
        str_add(&out,expr_to_kotlin(CHILD2,-1).data);
        str_add(&out,",null)");
      }else if (CHILD1->type->tag == TYP_VEC){
        str_add(&out,"((");
        str_add(&out,expr_to_kotlin(CHILD1,-1).data);
        str_add(&out,")[");
        str_add(&out,expr_to_kotlin(CHILD2,-1).data);
        str_add(&out,"])=null");
      }
    }

  }else if (expr->key == EXPR_ALLOC){
    type_t* typ = (type_t*)(CHILD1->term);

    if (typ->tag == TYP_STT){
      str_add(&out,"(");
      str_add(&out,typ->name.data);
      str_add(&out,"())");

    }else if (typ->tag == TYP_ARR){
      if (expr->children.len == 1){
        str_add(&out,"(");
        str_add(&out,type_to_kotlin(typ,0).data);
        str_add(&out,"())");
      }else{
		// This part maybe needed to fix?
        str_add(&out,"( ");
        str_add(&out,type_to_kotlin(typ,0).data);
        str_add(&out,"(Arrays.asList(new Array");
        str_add(&out,"<");
		str_add(&out,type_to_kotlin(typ->elem0,1).data);
		str_add(&out,"> = {");
        list_node_t* it = expr->children.head->next;
        while (it){
          if (it != expr->children.head->next){
            str_add(&out,",");
          }
          str_add(&out,"(");
          str_add(&out,expr_to_kotlin((expr_t*)(it->data),-1).data);
          str_add(&out,")");
          it = it->next;
        }
        str_add(&out,"})))");
        
      }
	// Is vectors in kotlin needed to fix? I think so...
    }else if (typ->tag == TYP_VEC){
	  //str_add(&out,"Array<");
      //str_add(&out,type_to_kotlin(typ,0).data);
      //str_add(&out,">");
      if (expr->children.len == 1){
        //str_add(&out,"<");
		str_add(&out,type_to_kotlin(typ,0).data);
        //str_add(&out,">(");
		str_add(&out,vec_init_kotlin(typ).data);
		//str_add(&out,"(");
		str_add(&out,") = {");
      }else{
		str_add(&out, "= {");
		INDENT2(indent);
        list_node_t* it = expr->children.head->next;
        while (it){
          if (it != expr->children.head->next){
            str_add(&out,",");
          }
          str_add(&out,"(");
          str_add(&out,expr_to_kotlin((expr_t*)(it->data),-1).data);
          str_add(&out,")");
          it = it->next;
        }
        str_add(&out,"})");
      }
	  str_add(&out, "};");
    }else if (typ->tag == TYP_MAP){
        str_add(&out,"(");
        str_add(&out,type_to_kotlin(typ,0).data);
        str_add(&out,"())");
    }else if (typ->tag == TYP_STR){
      if (CHILD2){
        str_add(&out,expr_to_kotlin(CHILD2,-1).data);
      }else{
        str_add(&out,"\"\"");
      }
    }
  }else if (expr->key == EXPR_FREE){
    if (CHILD1->key == EXPR_TERM){
      str_add(&out,"/*GC*/");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out,"=null");
    }else{
      //too complex, don't handle
      str_add(&out,"/*GC `");
      str_add(&out, expr_to_kotlin(CHILD1,-1).data);
      str_add(&out,"=null`*/");
    }

  }else if (expr->key == EXPR_STRUCTGET){
    str_add(&out,"((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_STRUCTSET){
    str_add(&out,"(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"=");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,"");

  }else if (expr->key == EXPR_VECGET){
    str_add(&out,"((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,")[");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"])");
  }else if (expr->key == EXPR_VECSET){
    str_add(&out,"((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,")[");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"])=");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);

  }else if (expr->key == EXPR_ARRGET){
    str_add(&out,"((");
    str_add(&out,type_to_kotlin(CHILD1->type->elem0,0).data);
    str_add(&out,")((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").get(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,")))");

  }else if (expr->key == EXPR_ARRSET){
    str_add(&out,"(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").set(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,",");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_ARRINS){

    str_add(&out,"(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").add((");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"),(");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,"))");

  }else if (expr->key == EXPR_ARRREM){
    
    str_add(&out,"w_arrRemove(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,",");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,",");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_ARRCPY){
    str_add(&out,"w_arrSlice(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,",");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,",");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_ARRLEN){
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,".size()");

  }else if (expr->key == EXPR_MAPLEN){
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,".size()");


  }else if (expr->key == EXPR_MAPGET){
    str_add(&out,"w_mapGet((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,"),(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"),(");
    str_add(&out,zero_to_kotlin(CHILD1->type->elem1).data);
    str_add(&out,"))");

  }else if (expr->key == EXPR_MAPREM){

    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,".remove(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_MAPSET){
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,".put((");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"),(");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,"))");

  }else if (expr->key == EXPR_STRLEN){
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,".length()");

  }else if (expr->key == EXPR_STRGET){
    str_add(&out,"(((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").charAt(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"))).toInt()");

  }else if (expr->key == EXPR_STRADD){
    str_add(&out,"(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,")+=Character.toString((");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,")).toChar()");


  }else if (expr->key == EXPR_STRCAT){

    str_add(&out,"(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,")+=(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_STRCPY){
    str_add(&out,"w_strSlice((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,"),(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"),(");
    str_add(&out,expr_to_kotlin(CHILD3,-1).data);
    str_add(&out,"))");

  }else if (expr->key == EXPR_STREQL){
    str_add(&out,"((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").equals(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,"))");

  }else if (expr->key == EXPR_STRNEQ){
    str_add(&out,"(!((");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,").equals(");
    str_add(&out,expr_to_kotlin(CHILD2,-1).data);
    str_add(&out,")))");

  }else if (expr->key == EXPR_PRINT){
    str_add(&out,"println(");
    str_add(&out,expr_to_kotlin(CHILD1,-1).data);
    str_add(&out,")");

  }else if (expr->key == EXPR_EXTERN){
    //skip
    out.len-=2;
    out.data[out.len] = 0;
    indent=-1;

  }else if (expr->key == EXPR_BREAK){
    str_add(&out,"break");
  }else if (expr->key == EXPR_ASM){
    
    str_add(&out,str_unquote(expr_to_kotlin(CHILD1,-1)).data);
    indent=-1;

  }else{
    str_add(&out,"/**");
    str_add(&out,expr->rawkey.data);
    str_add(&out,"**/");
  }

  if (indent>=0){str_add(&out,"\n");}
  return out;
}

str_t tree_to_kotlin(str_t modname, expr_t* tree, map_t* functable, map_t* stttable, map_t* included){
  str_t out = str_new();
  str_add(&out,"/*****************************************\n * ");
  str_add(&out,modname.data);
  for (int i = 0; i < 38-modname.len; i++){
    str_addch(&out,' ');
  }
  str_add(&out,"*\n*****************************************/\n");
  str_add(&out,"/* Compiled by WAXC (Version ");
  str_add(&out,__DATE__);
  str_add(&out,")*/\n\n");


  str_t mainstr = str_from("main",4);
  func_t* fun = func_lookup(&mainstr,functable);
  if (fun == NULL){
    str_add(&out,"\npackage ");
    str_add(&out,modname.data);
    str_add(&out,";\n\n");
  }

  str_add(&out,"import kotlin.collections.ArrayList;\n");
  str_add(&out,"import kotlin.collections.HashMap;\n");
  str_add(&out,"import kotlin.collections.List;\n");
  str_add(&out,"import kotlin.collections.Map;\n");

  str_add(&out,"\npublic class ");
  str_add(&out,modname.data);
  str_add(&out,"{\n");

  str_add(&out,"/*=== WAX Standard Library BEGIN ===*/\n");
  str_addconst(&out,TEXT_std_kotlin);
  
  str_add(&out,"/*=== WAX Standard Library END   ===*/\n\n");
  str_add(&out,"/*=== User Code            BEGIN ===*/\n");


  list_node_t* it = tree->children.head;

  while(it){
    expr_t* expr = (expr_t*)(it->data);

    if (expr->key == EXPR_LET && it->next){
      expr_t* ex1 = (expr_t*)(it->next->data);
      if (ex1->key == EXPR_SET){
        expr_t* ex2 = (expr_t*)(ex1->children.head->data);

        if (ex2->key == EXPR_TERM){
          if (str_eq( &((tok_t*)(CHILD1->term))->val, ((tok_t*)(ex2->term))->val.data )){
            str_add(&out,"\nvar ");
			str_add(&out, ((tok_t*)(CHILD1->term))->val.data);
            str_add(&out,":");
            str_add(&out,type_to_kotlin( (type_t*)(CHILD2->term) ,0).data);
            str_add(&out,"=");
            str_add(&out,expr_to_kotlin( (expr_t*)(ex1->children.head->next->data),-1).data);

            str_add(&out,"\n");
            it = it -> next -> next;
            continue;
          }
          
        }
      }
    }

    str_add(&out,expr_to_kotlin(expr,1).data);


    it = it->next;
  }
  str_add(&out,"/*=== User Code            END   ===*/\n");


  if (fun != NULL){
    if (!(fun->params.len)){
      str_add(&out,"  public fun main(){\n");
      str_add(&out,"    System.exit(main_());\n");
      str_add(&out,"  }\n");
    }else{
      str_add(&out,"  public fun main(args: Array<String>){\n");
      str_add(&out,"    var aargs: ArrayList<String> = new ArrayList<String>(Arrays.asList(args));\n");
      str_add(&out,"    System.exit(main_(aargs));\n");
      str_add(&out,"  }\n");
    }
  }

  str_add(&out,"}");
  
  return out;

}




#endif
