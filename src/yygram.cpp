#line 1 "lang/grammar.acc"

/* GRAMMAR.ACC -- Copyright (c) 1999-2003 Julian Mensch

     Contains the BNF+ grammar for the IncursionScript compiler,

	 along with semantic actions that essentially compose the bulk

	 of the compiler. Designed to be fed into the ACCENT compiler

	 compiler system to produce yygram.cpp.

*/

#include "Incursion.h"
#include <setjmp.h>

#define printf T1->Write

#define GETBIND_MEMFUNC(id)         ((BMemFunc*)(theSymTab.GetBinding(id,0,0,MEM_FUNC)))
#define GETBIND_SYSFUNC(id)         ((BSysFunc*)(theSymTab.GetBinding(id,0,0,SYS_FUNC)))
#define GETBIND_MEMVAR(id)          ((BMemVar*)(theSymTab.GetBinding(id,0,1,MEM_VAR)))
#define GETBIND_RESMEM(id)          ((BResMem*)(theSymTab.GetBinding(id,0,1,RES_MEM)))
#define GETBIND_RESFUNC(id)         ((BResFunc*)(theSymTab.GetBinding(id,0,0,RES_FUNC)))
#define GETBIND_GLOBALVAR(id)       ((BGlobalVar*)(theSymTab.GetBinding(id,0,0,GLOB_VAR)))
#define GETBIND_LOCALVAR(id,xID,Ev) ((BLocalVar*)(theSymTab.GetBinding(id,xID,Ev,LOC_VAR)))
#define GETBIND_RESVAR(id,xID)   ((BResVar*)(theSymTab.GetBinding(id,xID,0,RES_VAR)))
#define GETBIND_SYSOBJ(id)          ((BSysObj*)(theSymTab.GetBinding(id,0,SYS_OBJ)))
#define IDENT_DEFINED(id)           (theSymTab.GetBinding(id,0,ANY))

#define pBSysFunc                 BSysFunc*
int16 currMap;
int16 ArtPowerType;
uint32 TempList[2048]; 
int16 TempListLoc;
extern int16 Errors;
extern long yypos;
extern uint32 CompilerFlags, MemFuncID;
extern String Unresolved;

int yyselect(), yymallocerror(), yyparse(),
  yylex();
void get_lexval();

extern uint16 yyMapSize[512];
extern SymbolTable theSymTab;

int32    CurrAttk, CurrFeat;
int file(), resource(), monster_def(), item_def(), map_def(),
  room_def(), feature_def(), quest_def(), spell_def(), mon_entry(),
  mflag_entry(), color(long*), save_entry(), attack_entry(), attrib_entry(),
  arm_type(), item_entry(), arm_entry(), iflag_entry(), eff_entry(),
  feat_entry(), numlist(long*), effect_def(), map_entry(), tile_entry(),
  region_entry(), object_entry(), race_def(), race_entry(), class_def(),
  class_entry(),  abil_level(), dungeon_def(), god_def(), arti_entry(),
  art_power(), dun_entry(), dconst_entry(), list_entry(), artifact_def(), 
  region_def(), domain_def(), domain_entry(), temp_attack_entry(), flavor_def(),
  flavor_entry(),
  terrain_def(), template_def(), domain_entry(), god_entry(), terrain_entry(),
  template_entry(), behaviour_def(), behaviour_entry(), encounter_def(),
  encounter_entry(), encounter_part();
  
int  event_entry(), event_desc(), prototype(), s_prototype(), 
  s_object_decl(), l_declaration(VBlock*), s_declaration(), g_declaration(), 
  r_declaration(), sg_prototype();

int  res_ref(long*),
     abil_param(long*),
     abil_level(long*,long*),
     special_ability(long*,long*,long*,long*,long*);

int cexpr(long*), cexpr2(long*), cexpr3(long*), cexpr4(long*),
    dice_val(Dice*), mval(MVal*);

int expr(PExp*), expr150(PExp*), expr140(PExp*), expr130(PExp*), 
      expr120(PExp*), expr110(PExp*), expr100(PExp*), expr90(PExp*),
      expr80(PExp*), expr70(PExp*), expr60(PExp*), expr50(PExp*),
      expr40(PExp*), expr30(PExp*), expr20(PExp*), expr10(PExp*),
      fund_expr(PExp*), lvalue(PExp*);
int block(VBlock*), statement(VBlock*), routine(VBlock*),
      param_list(BSysFunc*,long,VBlock*,int*), sep_expr(hCode *hc),
      sep_stat(hCode *hc);

extern jmp_buf jb;
extern TMonster * theMon, * firstMon;
extern TItem * theItem, * firstItem;
extern TFeature * theFeat, * firstFeat;
extern TEffect * theEff, * firstEff;
extern TQuest * theQuest, * firstQuest;
extern TArtifact *theArti, *firstArti;
extern TDungeon *theDgn, *firstDgn;
extern TRoutine *theRou, *firstRou;
extern TNPC *theNPC, *firstNPC;
extern TClass *theClass, *firstClass;
extern TRace *theRace, *firstRace;
extern TDomain *theDom, *firstDom;
extern TGod *theGod, *firstGod;
extern TRegion *theReg, *firstReg;
extern TTerrain *theTer, *firstTer;
extern TText *theTxt, *firstTxt;
extern TVariable *theVar, *firstVar;
extern TTemplate *theTemp, *firstTemp;
extern TFlavor *theFlavor, *firstFlavor;
extern TBehaviour *theBev, *firstBev;
extern TEncounter *theEnc, *firstEnc;
extern TText *theText, *firstText;
extern Module * theModule;
extern Resource *theRes;

extern char preprocFilename[];

extern LocationInfo __Tiles__[];

extern LocationInfo *firstTile, *theTile;

int32 theValsIdx;
#define ESTABLISH_VALS                          \
  EffectValues * theVals = (theValsIdx == 0) ?  \
    (&theEff->ef) : &(theEff->Annot(theValsIdx)->u.ef);  \

int32 theEvent = 0;
int16 currPart = 0;
int16 cText = 0;

EffectValues * ChillShieldSeg3 = NULL;

EffectValues ValArray[32];

/* function should return a value; 'void' return type assumed */
#pragma warning (disable : 4508)

int yyerror(const char*msg) {
   Errors++;
   T1->Color(RED);
   T1->CWrite(Format("\"%s\" (%d): %s\n", preprocFilename, yypos, msg));
   longjmp(jb,1);
}

void yywarning(const char*msg) {
   T1->Color(GREY);
   T1->CWrite(Format("\"%s\" (%d): %s\n", preprocFilename,  yypos, msg));
}


extern "C" int yywrap() {
   return 1;
}



# line 152 "src/yygram.cpp"
#include "yygram.h"

int YYSTART ()
{
   switch(yyselect()) {
   case 478: {
      file();
      get_lexval();
      } break;
   }
return 0;
}

int file ()
{
   switch(yyselect()) {
   case 1: {
#line 193 "lang/grammar.acc"
 currMap = 0; MemFuncID = 0; MemVarID = 0; 
# line 172 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 479: {
            resource();
            } break;
         default: goto yy1;
         }
      }
      yy1: ;
      } break;
   }
return 0;
}

int resource ()
{
   YYSTYPE name;
   YYSTYPE n;
   YYSTYPE fname;
   YYSTYPE nam;
   YYSTYPE text;
   switch(yyselect()) {
   case 2: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
#line 197 "lang/grammar.acc"
 theModule->Name = name; 
# line 202 "src/yygram.cpp"
      } break;
   case 3: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 198 "lang/grammar.acc"
 theModule->Slot = (int16)n; 
# line 210 "src/yygram.cpp"
      } break;
   case 4: {
      get_lexval();
      get_lexval();
      fname = yylval;
      get_lexval();
#line 199 "lang/grammar.acc"
 theModule->FName = fname; 
# line 219 "src/yygram.cpp"
      } break;
   case 5: {
      monster_def();
      } break;
   case 6: {
      item_def();
      } break;
   case 7: {
      feature_def();
      } break;
   case 8: {
      artifact_def();
      } break;
   case 9: {
      dungeon_def();
      } break;
   case 10: {
      quest_def();
      } break;
   case 11: {
      effect_def();
      } break;
   case 12: {
      race_def();
      } break;
   case 13: {
      class_def();
      } break;
   case 14: {
      god_def();
      } break;
   case 15: {
      domain_def();
      } break;
   case 16: {
      region_def();
      } break;
   case 17: {
      terrain_def();
      } break;
   case 18: {
      template_def();
      } break;
   case 19: {
      flavor_def();
      } break;
   case 20: {
      behaviour_def();
      } break;
   case 21: {
      encounter_def();
      } break;
   case 22: {
      prototype();
      } break;
   case 23: {
      s_prototype();
      } break;
   case 24: {
      sg_prototype();
      } break;
   case 25: {
      g_declaration();
      } break;
   case 26: {
      s_declaration();
      } break;
   case 27: {
      s_object_decl();
      } break;
   case 28: {
      get_lexval();
      switch (yyselect()) {
      case 480: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr(&n);
#line 223 "lang/grammar.acc"
 CompilerFlags |= n; 
# line 301 "src/yygram.cpp"
      } break;
   case 29: {
      get_lexval();
      get_lexval();
      nam = yylval;
      get_lexval();
      switch (yyselect()) {
      case 481: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      text = yylval;
      switch (yyselect()) {
      case 482: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 225 "lang/grammar.acc"
 theRes = theTxt; theTxt->Name = nam; theTxt->Desc = text; theTxt++; 
# line 326 "src/yygram.cpp"
      } break;
   }
return 0;
}

int res_ref (rID *res)
{
   YYSTYPE lit;
   YYSTYPE n;
   switch(yyselect()) {
   case 30: {
      get_lexval();
      get_lexval();
      lit = yylval;
#line 229 "lang/grammar.acc"
 PurgeStrings();
                         *res = FIND(theModule->GetText(lit)); 
                         if (!*res) {
                           if (Unresolved.GetLength())
                             Unresolved += ", ";
                           Unresolved += Capitalize(theModule->GetText(lit),true);
                           *res = FIND("unimplemented");
                           }
                        
# line 351 "src/yygram.cpp"
      } break;
   case 31: {
      get_lexval();
      cexpr3(&n);
#line 238 "lang/grammar.acc"
 *res = n; 
# line 358 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr (YYSTYPE *val)
{
   YYSTYPE x;
   YYSTYPE y;
   YYSTYPE val2;
   switch(yyselect()) {
   case 32: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 242 "lang/grammar.acc"
 *val = x+y; 
# line 376 "src/yygram.cpp"
      } break;
   case 33: {
      cexpr(&x);
      get_lexval();
      cexpr2(&y);
#line 243 "lang/grammar.acc"
 *val = x-y; 
# line 384 "src/yygram.cpp"
      } break;
   case 34: {
      cexpr2(&val2);
#line 244 "lang/grammar.acc"
 *val = val2; 
# line 390 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr2 (YYSTYPE *val)
{
   YYSTYPE x;
   YYSTYPE y;
   YYSTYPE val2;
   switch(yyselect()) {
   case 35: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 248 "lang/grammar.acc"
 *val = x * y; 
# line 408 "src/yygram.cpp"
      } break;
   case 36: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 249 "lang/grammar.acc"
 *val = x / y; 
# line 416 "src/yygram.cpp"
      } break;
   case 37: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 250 "lang/grammar.acc"
 *val = x % y; 
# line 424 "src/yygram.cpp"
      } break;
   case 38: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 251 "lang/grammar.acc"
 *val = x | y; 
# line 432 "src/yygram.cpp"
      } break;
   case 39: {
      cexpr2(&x);
      get_lexval();
      cexpr3(&y);
#line 252 "lang/grammar.acc"
 *val = x & y; 
# line 440 "src/yygram.cpp"
      } break;
   case 40: {
      cexpr3(&val2);
#line 254 "lang/grammar.acc"
 *val = val2; 
# line 446 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr3 (YYSTYPE *val)
{
   YYSTYPE val2;
   YYSTYPE n;
   YYSTYPE val3;
   switch(yyselect()) {
   case 41: {
      get_lexval();
      cexpr4(&val2);
#line 258 "lang/grammar.acc"
 *val = val2; 
# line 463 "src/yygram.cpp"
      } break;
   case 42: {
      get_lexval();
      cexpr4(&n);
#line 259 "lang/grammar.acc"
 *val = -n; 
# line 470 "src/yygram.cpp"
      } break;
   case 43: {
      cexpr4(&val3);
#line 260 "lang/grammar.acc"
 *val = val3; 
# line 476 "src/yygram.cpp"
      } break;
   }
return 0;
}

int cexpr4 (YYSTYPE *val)
{
   YYSTYPE val2;
   YYSTYPE val3;
   YYSTYPE val4;
   switch(yyselect()) {
   case 44: {
      get_lexval();
      cexpr(&val2);
      get_lexval();
#line 264 "lang/grammar.acc"
 * val = val2; 
# line 494 "src/yygram.cpp"
      } break;
   case 45: {
      get_lexval();
      val3 = yylval;
#line 265 "lang/grammar.acc"
 *val = val3; 
# line 501 "src/yygram.cpp"
      } break;
   case 46: {
      get_lexval();
#line 266 "lang/grammar.acc"
 *val = 1; 
# line 507 "src/yygram.cpp"
      } break;
   case 47: {
      get_lexval();
#line 267 "lang/grammar.acc"
 *val = 0; 
# line 513 "src/yygram.cpp"
      } break;
   case 48: {
      get_lexval();
      val4 = yylval;
#line 268 "lang/grammar.acc"
 * val = val4; 
# line 520 "src/yygram.cpp"
      } break;
   }
return 0;
}

int dice_val (Dice *d)
{
   YYSTYPE num;
   YYSTYPE sid;
   YYSTYPE bon;
   YYSTYPE x;
   switch(yyselect()) {
   case 49: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      cexpr4(&bon);
#line 273 "lang/grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = (int8)bon; 
# line 541 "src/yygram.cpp"
      } break;
   case 50: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      get_lexval();
      cexpr4(&bon);
#line 275 "lang/grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = (int8)-bon; 
# line 552 "src/yygram.cpp"
      } break;
   case 51: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 277 "lang/grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = 0; 
# line 560 "src/yygram.cpp"
      } break;
   case 52: {
      cexpr3(&x);
#line 279 "lang/grammar.acc"
 d->Number = 0; d->Sides = 0; d->Bonus = (int8)x; 
# line 566 "src/yygram.cpp"
      } break;
   }
return 0;
}

int dice_only (Dice *d)
{
   YYSTYPE num;
   YYSTYPE sid;
   YYSTYPE bon;
   switch(yyselect()) {
   case 53: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
      get_lexval();
      cexpr4(&bon);
#line 284 "lang/grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = (int8)bon; 
# line 586 "src/yygram.cpp"
      } break;
   case 54: {
      cexpr4(&num);
      get_lexval();
      cexpr4(&sid);
#line 286 "lang/grammar.acc"
 d->Number = (int8)num; d->Sides = (int8)sid; d->Bonus = 0; 
# line 594 "src/yygram.cpp"
      } break;
   }
return 0;
}

int gear_entry (int16 *chance, Dice *amt, rID *iID, rID *eID, uint16 *spec, uint32 *qual, uint32 *fl, hCode *cond)
{
   hCode hc;
   YYSTYPE n;
   Dice dv;
   rID rr;
   YYSTYPE wq;
   YYSTYPE a;
   YYSTYPE b;
   YYSTYPE x;
   YYSTYPE y;
   switch(yyselect()) {
   case 55: {
#line 291 "lang/grammar.acc"

    int16 q; q=0;
    *chance = 0;
    *spec = 0;
    *qual = *fl = 0;
    *cond = 0;
    *iID = *eID = 0;
    amt->Set(1,1,0); 
# line 622 "src/yygram.cpp"
      switch (yyselect()) {
      case 483: {
         get_lexval();
         get_lexval();
         sep_expr(&hc);
         get_lexval();
#line 299 "lang/grammar.acc"
 *cond = hc; 
# line 631 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 484: {
         get_lexval();
         n = yylval;
         get_lexval();
#line 300 "lang/grammar.acc"
 *chance = (int16)n; 
# line 642 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 485: {
         dice_only(&dv);
#line 301 "lang/grammar.acc"
 *amt = yyDice(dv); 
# line 651 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 486: {
         get_lexval();
#line 302 "lang/grammar.acc"
 *fl |= GF_CURSED; 
# line 660 "src/yygram.cpp"
         } break;
      case 487: {
         get_lexval();
#line 303 "lang/grammar.acc"
 *fl |= GF_BLESSED; 
# line 666 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 488: {
         res_ref(&rr);
#line 304 "lang/grammar.acc"
 *iID = rr; 
# line 675 "src/yygram.cpp"
         } break;
      case 489: {
         get_lexval();
         cexpr3(&n);
#line 305 "lang/grammar.acc"
 *iID = n; *fl |= GF_RANDOM_ITEM; 
# line 682 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 490: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 306 "lang/grammar.acc"
 *spec = (uint16)n; 
# line 693 "src/yygram.cpp"
         } break;
      case 491: {
         get_lexval();
         get_lexval();
         n = yylval;
#line 306 "lang/grammar.acc"
 *spec = (uint16)-n; 
# line 701 "src/yygram.cpp"
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 494: {
            get_lexval();
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 493: {
                  cexpr(&wq);
                  switch (yyselect()) {
                  case 492: {
                     get_lexval();
                     } break;
                  default: ;
                  }
#line 307 "lang/grammar.acc"
 if (q >= 4)
                                   yyerror("Max 4 qualities in gear lists!");
                                else
                                  ((uint8*)qual)[q++] = (uint8)wq;
# line 725 "src/yygram.cpp"
                  } break;
               default: goto yy3;
               }
            }
            yy3: ;
            get_lexval();
            } break;
         case 495: {
            get_lexval();
            res_ref(&rr);
#line 311 "lang/grammar.acc"
 *eID = rr; 
# line 738 "src/yygram.cpp"
            } break;
         case 496: {
            get_lexval();
            get_lexval();
            cexpr3(&a);
            get_lexval();
            cexpr3(&b);
#line 313 "lang/grammar.acc"
 *eID = a + b*256; *fl |= GF_RANDOM_EFFECT; 
# line 748 "src/yygram.cpp"
            } break;
         case 497: {
            get_lexval();
            get_lexval();
            cexpr3(&b);
#line 315 "lang/grammar.acc"
 *eID = b*256; *fl |= GF_RANDOM_EFFECT; 
# line 756 "src/yygram.cpp"
            } break;
         case 498: {
            get_lexval();
            get_lexval();
#line 317 "lang/grammar.acc"
 *eID = 0; *fl |= GF_RANDOM_EFFECT; 
# line 763 "src/yygram.cpp"
            } break;
         case 499: {
            get_lexval();
            get_lexval();
            cexpr3(&x);
            get_lexval();
            cexpr3(&y);
            get_lexval();
            } break;
         default: goto yy2;
         }
      }
      yy2: ;
      } break;
   }
return 0;
}

int gear_desc ()
{
   int16 chance;
   Dice amt;
   rID iID;
   rID eID;
   uint16 spec;
   uint32 qual;
   uint32 fl;
   hCode cond;
   switch(yyselect()) {
   case 56: {
#line 323 "lang/grammar.acc"
 int16 is_else = 0; 
# line 796 "src/yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 500: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      while (1) {
         switch (yyselect()) {
         case 503: {
            gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 326 "lang/grammar.acc"
 theRes->AddEquip((uint8)chance,amt,iID,eID,(int8)spec,(uint8*)(&qual),(uint8)(fl | is_else),cond); 
# line 810 "src/yygram.cpp"
            switch (yyselect()) {
            case 501: {
               get_lexval();
#line 327 "lang/grammar.acc"
 is_else = 0; 
# line 816 "src/yygram.cpp"
               } break;
            case 502: {
               get_lexval();
#line 327 "lang/grammar.acc"
 is_else = GF_ELSE; 
# line 822 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy4;
         }
      }
      yy4: ;
      gear_entry(&chance, &amt, &iID, &eID, &spec, &qual, &fl, &cond);
#line 329 "lang/grammar.acc"
 theRes->AddEquip((uint8)chance,amt,iID,eID,(int8)spec,(uint8*)(&qual),(uint8)(fl | is_else),cond); 
# line 834 "src/yygram.cpp"
      get_lexval();
      } break;
   }
return 0;
}

int glyph_entry (uint16 *img)
{
   YYSTYPE col;
   YYSTYPE n;
   YYSTYPE back;
   switch(yyselect()) {
   case 57: {
      switch (yyselect()) {
      case 504: {
         get_lexval();
         } break;
      case 505: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 506: {
         get_lexval();
         } break;
      default: ;
      }
#line 333 "lang/grammar.acc"
 *img = 15 << 8; 
# line 865 "src/yygram.cpp"
      switch (yyselect()) {
      case 507: {
         color(&col);
#line 334 "lang/grammar.acc"
 *img = (uint16)(col << 8); 
# line 871 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 508: {
         get_lexval();
         n = yylval;
#line 335 "lang/grammar.acc"
 *img |= n; 
# line 881 "src/yygram.cpp"
         } break;
      case 509: {
         get_lexval();
         n = yylval;
#line 336 "lang/grammar.acc"
 *img |= n; 
# line 888 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 510: {
         get_lexval();
         color(&back);
#line 338 "lang/grammar.acc"
 *img |= back << 12; 
# line 898 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   }
return 0;
}

int glyph_desc (uint16 *img)
{
   YYSTYPE col;
   YYSTYPE n;
   YYSTYPE back;
   switch(yyselect()) {
   case 58: {
#line 342 "lang/grammar.acc"
 *img = 15 << 8; 
# line 917 "src/yygram.cpp"
      switch (yyselect()) {
      case 511: {
         color(&col);
#line 343 "lang/grammar.acc"
 *img = (uint16)(col << 8); 
# line 923 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 512: {
         get_lexval();
         n = yylval;
#line 344 "lang/grammar.acc"
 *img |= n; 
# line 933 "src/yygram.cpp"
         } break;
      case 513: {
         get_lexval();
         n = yylval;
#line 345 "lang/grammar.acc"
 *img |= n; 
# line 940 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 514: {
         get_lexval();
         color(&back);
#line 346 "lang/grammar.acc"
 *img |= back << 12; 
# line 950 "src/yygram.cpp"
         } break;
      default: ;
      }
      } break;
   }
return 0;
}

int event_desc ()
{
   switch(yyselect()) {
   case 59: {
      get_lexval();
      get_lexval();
      event_entry();
      while (1) {
         switch (yyselect()) {
         case 515: {
            get_lexval();
            event_entry();
            } break;
         default: goto yy5;
         }
      }
      yy5: ;
      get_lexval();
      } break;
   }
return 0;
}

int event_entry ()
{
   YYSTYPE n;
   YYSTYPE lv;
   VBlock vc;
   switch(yyselect()) {
   case 60: {
#line 354 "lang/grammar.acc"
 int16 EvList[32]; int16 i, cEvent = 0; 
# line 991 "src/yygram.cpp"
      cexpr(&n);
#line 355 "lang/grammar.acc"
 EvList[cEvent++] = (int16)n; theEvent = n; 
# line 995 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 516: {
            get_lexval();
            cexpr(&n);
#line 356 "lang/grammar.acc"
 EvList[cEvent++] = (int16)n; 
# line 1003 "src/yygram.cpp"
            } break;
         default: goto yy6;
         }
      }
      yy6: ;
      switch (yyselect()) {
      case 518: {
         get_lexval();
         lv = yylval;
#line 357 "lang/grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 1016 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 517: {
               get_lexval();
               get_lexval();
               lv = yylval;
#line 359 "lang/grammar.acc"
 for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(-EvList[i],lv); 
# line 1026 "src/yygram.cpp"
               } break;
            default: goto yy7;
            }
         }
         yy7: ;
         } break;
      case 519: {
         routine(&vc);
#line 364 "lang/grammar.acc"
 hCode hc = theCodeSeg.Add(&vc);
                            theCodeSeg.Generate(HALT);
                            for(i=0;i!=cEvent;i++)
                              theRes->AddEvent(EvList[i],hc);
                            theEvent = 0; 
# line 1041 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      } break;
   }
return 0;
}

int sep_expr (hCode *hc)
{
   PExp ex;
   switch(yyselect()) {
   case 61: {
#line 373 "lang/grammar.acc"
 theEvent = EV_CONDITION; 
# line 1057 "src/yygram.cpp"
      expr(&ex);
#line 375 "lang/grammar.acc"
 *hc = theCodeSeg.Add(ex.Code);
      theCodeSeg.Generate(RET,ex.Storage,ex.Value); 
# line 1062 "src/yygram.cpp"
#line 377 "lang/grammar.acc"
 theEvent = 0; 
# line 1065 "src/yygram.cpp"
      } break;
   }
return 0;
}

int sep_stat (hCode *hc)
{
   VBlock st;
   switch(yyselect()) {
   case 62: {
#line 381 "lang/grammar.acc"
 theEvent = EV_CONDITION; 
# line 1078 "src/yygram.cpp"
      statement(&st);
#line 383 "lang/grammar.acc"
 *hc = theCodeSeg.Add(&st);
      theCodeSeg.Generate(RET,0,0); 
# line 1083 "src/yygram.cpp"
#line 385 "lang/grammar.acc"
 theEvent = 0; 
# line 1086 "src/yygram.cpp"
      } break;
   }
return 0;
}

int mval (MVal *mv)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 63: {
#line 389 "lang/grammar.acc"
 mv->BType = mv->Bound = 0; 
# line 1099 "src/yygram.cpp"
      switch (yyselect()) {
      case 521: {
         switch (yyselect()) {
         case 520: {
            get_lexval();
            } break;
         default: ;
         }
         cexpr4(&n);
#line 390 "lang/grammar.acc"
 mv->Value = n;  mv->VType = MVAL_ADD; 
# line 1111 "src/yygram.cpp"
         } break;
      case 522: {
         get_lexval();
         cexpr4(&n);
#line 391 "lang/grammar.acc"
 mv->Value = -n; mv->VType = MVAL_ADD; 
# line 1118 "src/yygram.cpp"
         } break;
      case 523: {
         get_lexval();
         cexpr4(&n);
#line 392 "lang/grammar.acc"
 mv->Value = n;  mv->VType = MVAL_SET; 
# line 1125 "src/yygram.cpp"
         } break;
      case 524: {
         cexpr3(&n);
         get_lexval();
#line 393 "lang/grammar.acc"
 mv->Value = n;  mv->VType = MVAL_PERCENT; 
# line 1132 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 528: {
         get_lexval();
         switch (yyselect()) {
         case 525: {
            get_lexval();
            cexpr4(&n);
#line 395 "lang/grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MIN; 
# line 1145 "src/yygram.cpp"
            } break;
         case 526: {
            get_lexval();
            cexpr4(&n);
#line 396 "lang/grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_MAX; 
# line 1152 "src/yygram.cpp"
            } break;
         case 527: {
            get_lexval();
            cexpr4(&n);
#line 397 "lang/grammar.acc"
 mv->Bound = n;  mv->BType = MBOUND_NEAR; 
# line 1159 "src/yygram.cpp"
            } break;
         default: printf("???\n"); exit(1);
         }
         get_lexval();
         } break;
      default: ;
      }
      } break;
   }
return 0;
}

int monster_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   YYSTYPE n2;
   YYSTYPE n3;
   switch(yyselect()) {
   case 64: {
      get_lexval();
#line 402 "lang/grammar.acc"
 theMon->Attr[0] = theMon->Attr[1] =
                       theMon->Attr[2] = theMon->Attr[3] =
                         theMon->Attr[4] = theMon->Attr[5] =
                         0; CurrAttk = 0; CurrFeat = 0; theRes = theMon; 
# line 1186 "src/yygram.cpp"
      get_lexval();
      name = yylval;
#line 406 "lang/grammar.acc"
 theMon->Name = name; 
# line 1191 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 407 "lang/grammar.acc"
 theMon->MType[0] = n; 
# line 1196 "src/yygram.cpp"
      switch (yyselect()) {
      case 530: {
         get_lexval();
         cexpr3(&n2);
#line 408 "lang/grammar.acc"
 theMon->MType[1] = n2; 
# line 1203 "src/yygram.cpp"
         switch (yyselect()) {
         case 529: {
            get_lexval();
            cexpr3(&n3);
#line 409 "lang/grammar.acc"
 theMon->MType[2] = n3; 
# line 1210 "src/yygram.cpp"
            } break;
         default: ;
         }
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 531: {
            mon_entry();
            } break;
         default: goto yy8;
         }
      }
      yy8: ;
      get_lexval();
#line 411 "lang/grammar.acc"
 theMon++; 
# line 1230 "src/yygram.cpp"
      } break;
   }
return 0;
}

int mon_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   YYSTYPE n2;
   YYSTYPE n1;
   uint16 img;
   YYSTYPE st;
   YYSTYPE stV;
   YYSTYPE stM;
   rID rr;
   YYSTYPE res;
   YYSTYPE imm;
   YYSTYPE l;
   switch(yyselect()) {
   case 65: {
      get_lexval();
      switch (yyselect()) {
      case 532: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 414 "lang/grammar.acc"
 theMon->Desc = lv; 
# line 1264 "src/yygram.cpp"
      } break;
   case 66: {
      get_lexval();
      switch (yyselect()) {
      case 533: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 415 "lang/grammar.acc"
 theMon->Depth = (int8)n; 
# line 1278 "src/yygram.cpp"
      } break;
   case 67: {
      switch (yyselect()) {
      case 534: {
         get_lexval();
         } break;
      case 535: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 536: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 416 "lang/grammar.acc"
 theMon->CR = (int8)n; 
# line 1300 "src/yygram.cpp"
      } break;
   case 68: {
      switch (yyselect()) {
      case 537: {
         get_lexval();
         } break;
      case 538: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 539: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n2);
      get_lexval();
      cexpr3(&n1);
      get_lexval();
#line 418 "lang/grammar.acc"
 if (n2 != 1 || n1 > 8 || n1 < 2)
                                        yyerror("Fractional CR error!");
                                      theMon->CR = (int8)((-n1) + 2); 
# line 1326 "src/yygram.cpp"
      } break;
   case 69: {
      get_lexval();
      switch (yyselect()) {
      case 540: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 421 "lang/grammar.acc"
 theMon->Terrains |= n; 
# line 1339 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 541: {
            get_lexval();
            cexpr(&n);
#line 422 "lang/grammar.acc"
 theMon->Terrains |= n; 
# line 1347 "src/yygram.cpp"
            } break;
         default: goto yy9;
         }
      }
      yy9: ;
      get_lexval();
      } break;
   case 70: {
      get_lexval();
      switch (yyselect()) {
      case 542: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 423 "lang/grammar.acc"
 theMon->HitDice = (int16)n; 
# line 1367 "src/yygram.cpp"
      } break;
   case 71: {
      get_lexval();
      switch (yyselect()) {
      case 543: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 424 "lang/grammar.acc"
 theMon->HitDice = (int16)n; 
                                       theMon->SetFlag(M_FIXED_HP); 
# line 1382 "src/yygram.cpp"
      } break;
   case 72: {
      switch (yyselect()) {
      case 544: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      switch (yyselect()) {
      case 545: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 426 "lang/grammar.acc"
 theMon->Hit = (int8)n; 
# line 1402 "src/yygram.cpp"
      } break;
   case 73: {
      glyph_entry(&img);
#line 427 "lang/grammar.acc"
 theMon->Image = img; 
# line 1408 "src/yygram.cpp"
      } break;
   case 74: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 429 "lang/grammar.acc"
 if (!theMon->GainStati((uint8)st)) yyerror("too many Stati in monster"); 
# line 1418 "src/yygram.cpp"
      } break;
   case 75: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 431 "lang/grammar.acc"
 if (!theMon->GainStati((uint8)st,(int16)stV)) yyerror("too many Stati in monster"); 
# line 1430 "src/yygram.cpp"
      } break;
   case 76: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      get_lexval();
#line 433 "lang/grammar.acc"
 if (!theMon->GainStati((uint8)st,(int16)stV,(int16)stM)) 
        yyerror("too many Stati in monster"); 
# line 1445 "src/yygram.cpp"
      } break;
   case 77: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      res_ref(&rr);
      get_lexval();
      get_lexval();
#line 436 "lang/grammar.acc"
 if (!theMon->GainStati((uint8)st,(int16)stV,(int16)stM,rr)) 
        yyerror("too many Stati in monster"); 
# line 1462 "src/yygram.cpp"
      } break;
   case 78: {
      get_lexval();
      switch (yyselect()) {
      case 546: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 548: {
            cexpr3(&res);
#line 439 "lang/grammar.acc"
 theMon->Res |= res; 
# line 1478 "src/yygram.cpp"
            switch (yyselect()) {
            case 547: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy10;
         }
      }
      yy10: ;
      get_lexval();
      } break;
   case 79: {
      get_lexval();
      switch (yyselect()) {
      case 549: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 551: {
            cexpr3(&imm);
#line 440 "lang/grammar.acc"
 theMon->Imm |= imm; 
# line 1506 "src/yygram.cpp"
            switch (yyselect()) {
            case 550: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy11;
         }
      }
      yy11: ;
      get_lexval();
      } break;
   case 80: {
      get_lexval();
      switch (yyselect()) {
      case 552: {
         get_lexval();
         } break;
      default: ;
      }
      attack_entry();
      while (1) {
         switch (yyselect()) {
         case 553: {
            get_lexval();
            attack_entry();
            } break;
         default: goto yy12;
         }
      }
      yy12: ;
      get_lexval();
      } break;
   case 81: {
      get_lexval();
      get_lexval();
      l = yylval;
      get_lexval();
#line 442 "lang/grammar.acc"
 for(TMonster*m=firstMon;m!=theMon;m++)
                                         if(stricmp(theModule->GetText(l),theModule->GetText(m->Name))==0)
                                           { memcpy(theMon,m,sizeof(TMonster));
                                             goto DoneCopy; }
                                       Fatal("Monster '%s' not found!",theModule->GetText(l));
                                       DoneCopy:; 
# line 1553 "src/yygram.cpp"
      } break;
   case 82: {
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 448 "lang/grammar.acc"
 if (n > (theMon - firstMon) || n < 0)
                                         Fatal("Bad number in Monster:as!");
                                       memcpy(theMon,&(firstMon[n]),sizeof(TMonster)); 
# line 1563 "src/yygram.cpp"
      } break;
   case 83: {
      get_lexval();
      switch (yyselect()) {
      case 554: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 451 "lang/grammar.acc"
 theMon->Def = (int8)n; 
# line 1577 "src/yygram.cpp"
      } break;
   case 84: {
      get_lexval();
      switch (yyselect()) {
      case 555: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 452 "lang/grammar.acc"
 theMon->Arm = (int8)n; 
# line 1591 "src/yygram.cpp"
      } break;
   case 85: {
      get_lexval();
      switch (yyselect()) {
      case 556: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 453 "lang/grammar.acc"
 theMon->Mana = (int16)n; 
# line 1605 "src/yygram.cpp"
      } break;
   case 86: {
      get_lexval();
      switch (yyselect()) {
      case 557: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 558: {
            cexpr3(&n);
            get_lexval();
#line 454 "lang/grammar.acc"
 theMon->Feats[CurrFeat++] = (uint16)n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1625 "src/yygram.cpp"
            } break;
         default: goto yy13;
         }
      }
      yy13: ;
      cexpr3(&n);
      get_lexval();
#line 458 "lang/grammar.acc"
 theMon->Feats[CurrFeat++] = (uint16)n; 
    if (CurrFeat > 16) Fatal("Too many feats for '%s' (max 16)",
                                theModule->QTextSeg + theMon->Name);
    
# line 1638 "src/yygram.cpp"
      } break;
   case 87: {
      get_lexval();
      switch (yyselect()) {
      case 559: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 560: {
            mflag_entry();
            } break;
         default: goto yy14;
         }
      }
      yy14: ;
      get_lexval();
      } break;
   case 88: {
      attrib_entry();
      while (1) {
         switch (yyselect()) {
         case 561: {
            get_lexval();
            attrib_entry();
            } break;
         default: goto yy15;
         }
      }
      yy15: ;
      get_lexval();
      } break;
   case 89: {
      get_lexval();
      switch (yyselect()) {
      case 562: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 464 "lang/grammar.acc"
 theMon->Size = (int8)n; 
# line 1685 "src/yygram.cpp"
      } break;
   case 90: {
      get_lexval();
      switch (yyselect()) {
      case 563: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 564: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 465 "lang/grammar.acc"
 theMon->Spd = (int8)((n-100)/5); 
# line 1705 "src/yygram.cpp"
      } break;
   case 91: {
      get_lexval();
      switch (yyselect()) {
      case 565: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 566: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 466 "lang/grammar.acc"
 theMon->Mov = (int8)((n-100)/5); 
# line 1725 "src/yygram.cpp"
      } break;
   case 92: {
      get_lexval();
      switch (yyselect()) {
      case 567: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 569: {
            res_ref(&rr);
#line 467 "lang/grammar.acc"
 theMon->SetFlag(M_INNATE);
                                       theMon->AddResID(AN_INNATE, rr); 
# line 1742 "src/yygram.cpp"
            switch (yyselect()) {
            case 568: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy16;
         }
      }
      yy16: ;
      get_lexval();
      } break;
   case 93: {
      get_lexval();
      switch (yyselect()) {
      case 570: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 572: {
            res_ref(&rr);
#line 470 "lang/grammar.acc"
 theMon->AddResID(AN_DISEASE, rr); 
# line 1770 "src/yygram.cpp"
            switch (yyselect()) {
            case 571: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy17;
         }
      }
      yy17: ;
      get_lexval();
      } break;
   case 94: {
      get_lexval();
      switch (yyselect()) {
      case 573: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 575: {
            res_ref(&rr);
#line 472 "lang/grammar.acc"
 theMon->AddResID(AN_POISON, rr); 
# line 1798 "src/yygram.cpp"
            switch (yyselect()) {
            case 574: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy18;
         }
      }
      yy18: ;
      get_lexval();
      } break;
   case 95: {
      gear_desc();
      } break;
   case 96: {
      event_desc();
      } break;
   case 97: {
      dconst_entry();
      } break;
   case 98: {
      r_declaration();
      } break;
   }
return 0;
}

int color (YYSTYPE *col)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 99: {
      get_lexval();
      n = yylval;
#line 481 "lang/grammar.acc"
 *col = n; 
# line 1837 "src/yygram.cpp"
      } break;
   case 100: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 482 "lang/grammar.acc"
 *col = n | 8; 
# line 1845 "src/yygram.cpp"
      } break;
   case 101: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 483 "lang/grammar.acc"
 *col = n | 8; 
# line 1853 "src/yygram.cpp"
      } break;
   case 102: {
      get_lexval();
      get_lexval();
      n = yylval;
#line 484 "lang/grammar.acc"
 *col = n & ~8; 
# line 1861 "src/yygram.cpp"
      } break;
   }
return 0;
}

int attack_entry ()
{
   YYSTYPE at;
   Dice dam;
   YYSTYPE chance;
   YYSTYPE n;
   YYSTYPE dc;
   Dice dam2;
   YYSTYPE n2;
   rID rr;
   YYSTYPE dt;
   switch(yyselect()) {
   case 103: {
#line 488 "lang/grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1883 "src/yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 576: {
         dice_only(&dam);
#line 491 "lang/grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 1891 "src/yygram.cpp"
         } break;
      case 577: {
         cexpr4(&chance);
         get_lexval();
#line 492 "lang/grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg.Bonus = (int8)chance; 
# line 1898 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 578: {
         cexpr4(&n);
#line 493 "lang/grammar.acc"
 dt = (uint8)n; 
# line 1907 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 579: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 583: {
         get_lexval();
         switch (yyselect()) {
         case 580: {
            get_lexval();
            } break;
         case 581: {
            get_lexval();
            } break;
         case 582: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 496 "lang/grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = (int8)dc; 
# line 1935 "src/yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 497 "lang/grammar.acc"
 theMon->Attk[CurrAttk].AType = (int8)at;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 /*

                                 if (at == A_ABIL)

                                   theMon->Attk[CurrAttk].u.a.DC = max(

                                        theMon->Attk[CurrAttk].u.a.DC, 1);

                                        */
                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 1956 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 591: {
            get_lexval();
#line 508 "lang/grammar.acc"
 dt = AD_NORM; dam2.Set(0,0,0); theMon->Attk[CurrAttk].u.a.Dmg.Set(0,0,0); 
# line 1963 "src/yygram.cpp"
            switch (yyselect()) {
            case 584: {
               dice_only(&dam2);
#line 509 "lang/grammar.acc"
 theMon->Attk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 1969 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 585: {
               cexpr3(&n2);
#line 510 "lang/grammar.acc"
 dt = (uint8)n2; 
# line 1978 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 586: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 590: {
               get_lexval();
               switch (yyselect()) {
               case 587: {
                  get_lexval();
                  } break;
               case 588: {
                  get_lexval();
                  } break;
               case 589: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 512 "lang/grammar.acc"
 theMon->Attk[CurrAttk].u.a.DC = (int8)dc; 
# line 2006 "src/yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 513 "lang/grammar.acc"
 theMon->Attk[CurrAttk].AType = A_ALSO;
                                 theMon->Attk[CurrAttk].DType = dt;
                                 CurrAttk++;
                                 if (CurrAttk >= 12)
                                   Fatal("> 12 attacks on a single monster!");
                               
# line 2018 "src/yygram.cpp"
            } break;
         default: goto yy19;
         }
      }
      yy19: ;
      } break;
   case 104: {
      cexpr3(&at);
      get_lexval();
      res_ref(&rr);
#line 520 "lang/grammar.acc"
 theMon->Attk[CurrAttk].AType = (int8)at;
                                  theMon->Attk[CurrAttk].u.xID = rr; 
# line 2032 "src/yygram.cpp"
      switch (yyselect()) {
      case 592: {
         cexpr3(&dt);
#line 522 "lang/grammar.acc"
 theMon->Attk[CurrAttk].DType = (int8)dt; 
# line 2038 "src/yygram.cpp"
         } break;
      default: ;
      }
#line 523 "lang/grammar.acc"

                                 CurrAttk++;
                                 if (CurrAttk >= 32)
                                   yyerror("> 32 attacks on a single monster!");
                               
# line 2048 "src/yygram.cpp"
      } break;
   }
return 0;
}

int mflag_entry ()
{
   YYSTYPE mf;
   switch(yyselect()) {
   case 105: {
      cexpr3(&mf);
      switch (yyselect()) {
      case 593: {
         get_lexval();
         } break;
      default: ;
      }
#line 531 "lang/grammar.acc"
 theMon->SetFlag((uint16)mf); 
# line 2068 "src/yygram.cpp"
      } break;
   case 106: {
      get_lexval();
      cexpr3(&mf);
      switch (yyselect()) {
      case 594: {
         get_lexval();
         } break;
      default: ;
      }
#line 532 "lang/grammar.acc"
 theMon->UnsetFlag((uint16)mf); 
# line 2081 "src/yygram.cpp"
      } break;
   }
return 0;
}

int attrib_entry ()
{
   YYSTYPE at;
   YYSTYPE v;
   switch(yyselect()) {
   case 107: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 595: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&v);
#line 536 "lang/grammar.acc"
 theMon->Attr[at] = (int8)v; 
# line 2104 "src/yygram.cpp"
      } break;
   }
return 0;
}

int item_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 108: {
      get_lexval();
#line 540 "lang/grammar.acc"
 theRes = theItem; 
                               theItem->u.w.Crit = 2; 
                               theItem->u.w.Threat = 1; 
                               theItem->hp = 15; 
# line 2122 "src/yygram.cpp"
      get_lexval();
      name = yylval;
#line 544 "lang/grammar.acc"
 theItem->Name = name; 
# line 2127 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 545 "lang/grammar.acc"
 theItem->IType = (int16)n; 
# line 2132 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 596: {
            item_entry();
            } break;
         default: goto yy20;
         }
      }
      yy20: ;
      get_lexval();
#line 546 "lang/grammar.acc"
 theItem++; 
# line 2146 "src/yygram.cpp"
      } break;
   }
return 0;
}

int item_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   uint16 img;
   Dice dv;
   YYSTYPE m;
   YYSTYPE unit;
   rID rr;
   switch(yyselect()) {
   case 109: {
      get_lexval();
      switch (yyselect()) {
      case 597: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 550 "lang/grammar.acc"
 theItem->Desc = lv; 
# line 2175 "src/yygram.cpp"
      } break;
   case 110: {
      get_lexval();
      switch (yyselect()) {
      case 598: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 551 "lang/grammar.acc"
 theItem->Level = (int8)n; 
# line 2189 "src/yygram.cpp"
      } break;
   case 111: {
      glyph_entry(&img);
#line 552 "lang/grammar.acc"
 theItem->Image = img; 
# line 2195 "src/yygram.cpp"
      } break;
   case 112: {
      get_lexval();
      switch (yyselect()) {
      case 599: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 553 "lang/grammar.acc"
 theItem->u.w.SDmg = yyDice(dv); 
# line 2209 "src/yygram.cpp"
      } break;
   case 113: {
      get_lexval();
      switch (yyselect()) {
      case 600: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&dv);
      get_lexval();
#line 554 "lang/grammar.acc"
 theItem->u.w.LDmg = yyDice(dv); 
# line 2223 "src/yygram.cpp"
      } break;
   case 114: {
      get_lexval();
      switch (yyselect()) {
      case 601: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 555 "lang/grammar.acc"
 theItem->u.w.Crit = (int8)n; 
# line 2237 "src/yygram.cpp"
      } break;
   case 115: {
      get_lexval();
      switch (yyselect()) {
      case 602: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 556 "lang/grammar.acc"
 theItem->u.w.Threat = (int8)n; 
# line 2251 "src/yygram.cpp"
      } break;
   case 116: {
      get_lexval();
      switch (yyselect()) {
      case 603: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 557 "lang/grammar.acc"
 theItem->u.w.Acc = (int8)n; 
# line 2265 "src/yygram.cpp"
      } break;
   case 117: {
      get_lexval();
      switch (yyselect()) {
      case 604: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 558 "lang/grammar.acc"
 theItem->u.w.Spd = (int8)n; 
# line 2279 "src/yygram.cpp"
      } break;
   case 118: {
      get_lexval();
      switch (yyselect()) {
      case 605: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 559 "lang/grammar.acc"
 theItem->u.w.Spd = (int8)(n > 0 ? ((n-100)/5) : n/5); 
# line 2294 "src/yygram.cpp"
      } break;
   case 119: {
      get_lexval();
      switch (yyselect()) {
      case 606: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      cexpr4(&n);
      get_lexval();
      get_lexval();
#line 560 "lang/grammar.acc"
 theItem->u.w.Spd = (int8)(n/5); 
# line 2310 "src/yygram.cpp"
      } break;
   case 120: {
      get_lexval();
      switch (yyselect()) {
      case 607: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 561 "lang/grammar.acc"
 theItem->u.w.ParryMod = (int8)n; 
# line 2324 "src/yygram.cpp"
      } break;
   case 121: {
      get_lexval();
      switch (yyselect()) {
      case 608: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 562 "lang/grammar.acc"
 theItem->u.w.RangeInc = (int8)n; 
# line 2338 "src/yygram.cpp"
      } break;
   case 122: {
      get_lexval();
      switch (yyselect()) {
      case 609: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 563 "lang/grammar.acc"
 theItem->Material = (int8)m; 
# line 2352 "src/yygram.cpp"
      } break;
   case 123: {
      get_lexval();
      switch (yyselect()) {
      case 610: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
#line 564 "lang/grammar.acc"
 theItem->Cost = n * 100; 
# line 2365 "src/yygram.cpp"
      switch (yyselect()) {
      case 611: {
         cexpr3(&unit);
#line 565 "lang/grammar.acc"
 switch (unit)
                                         { case PLATINUM: theItem->Cost *= 5; break;
                                           case GOLD:     break;
                                           case ELECTRUM: theItem->Cost /= 2; break;
                                           case SILVER:   theItem->Cost /= 10; break;
                                           case COPPER:   theItem->Cost /= 100; break;
                                           
                                         }
                                     
# line 2379 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 124: {
      get_lexval();
      switch (yyselect()) {
      case 612: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 575 "lang/grammar.acc"
 theItem->Weight = (int16)n; 
    /* ww: according to the SRD, one inch (thickness) of iron has 30

     * hit point. We assume that the canonical long sword (at 4 lbs = 40

     * weight) is 1/2 inch thick. So a base weight of 80 gets you 30 hit

     * points. */
                               theItem->hp = (uint16)((n * 30) / 80); 
                               if (theItem->hp < 1) theItem->hp = 1; 
# line 2406 "src/yygram.cpp"
      } break;
   case 125: {
      get_lexval();
      switch (yyselect()) {
      case 613: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 582 "lang/grammar.acc"
 theItem->Size = (int8)n; 
# line 2420 "src/yygram.cpp"
      } break;
   case 126: {
      get_lexval();
      switch (yyselect()) {
      case 614: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 583 "lang/grammar.acc"
 theItem->Nutrition = (int8)n; 
# line 2434 "src/yygram.cpp"
      } break;
   case 127: {
      get_lexval();
      switch (yyselect()) {
      case 615: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 584 "lang/grammar.acc"
 theItem->u.a.Def = (int8)n; 
# line 2448 "src/yygram.cpp"
      } break;
   case 128: {
      get_lexval();
      switch (yyselect()) {
      case 616: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 585 "lang/grammar.acc"
 theItem->u.a.Cov = (int8)n; 
# line 2462 "src/yygram.cpp"
      } break;
   case 129: {
      get_lexval();
      switch (yyselect()) {
      case 617: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 586 "lang/grammar.acc"
 theItem->u.a.Penalty = (int8)n; 
# line 2476 "src/yygram.cpp"
      } break;
   case 130: {
      get_lexval();
      switch (yyselect()) {
      case 618: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 619: {
            arm_entry();
            } break;
         default: goto yy21;
         }
      }
      yy21: ;
      get_lexval();
      } break;
   case 131: {
      get_lexval();
      switch (yyselect()) {
      case 620: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 588 "lang/grammar.acc"
 theItem->hp = (uint16)n; 
# line 2509 "src/yygram.cpp"
      } break;
   case 132: {
      get_lexval();
      switch (yyselect()) {
      case 621: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 623: {
            res_ref(&rr);
#line 589 "lang/grammar.acc"
 theItem->AddResID(AN_SPELLS, rr); 
# line 2525 "src/yygram.cpp"
            switch (yyselect()) {
            case 622: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy22;
         }
      }
      yy22: ;
      get_lexval();
      } break;
   case 133: {
      get_lexval();
      switch (yyselect()) {
      case 624: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 626: {
            res_ref(&rr);
#line 591 "lang/grammar.acc"
 theItem->AddResID(AN_FIRES, rr); 
# line 2553 "src/yygram.cpp"
            switch (yyselect()) {
            case 625: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy23;
         }
      }
      yy23: ;
      get_lexval();
      } break;
   case 134: {
      get_lexval();
      switch (yyselect()) {
      case 627: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 628: {
            iflag_entry();
            } break;
         default: goto yy24;
         }
      }
      yy24: ;
      get_lexval();
      } break;
   case 135: {
      get_lexval();
      switch (yyselect()) {
      case 629: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 632: {
            cexpr3(&n);
            switch (yyselect()) {
            case 630: {
               get_lexval();
               } break;
            case 631: {
               get_lexval();
               } break;
            default: ;
            }
#line 594 "lang/grammar.acc"
 theItem->Group |= n; 
# line 2609 "src/yygram.cpp"
            } break;
         default: goto yy25;
         }
      }
      yy25: ;
      get_lexval();
      } break;
   case 136: {
      get_lexval();
      switch (yyselect()) {
      case 633: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 595 "lang/grammar.acc"
 theItem->u.c.Capacity = (int8)n; 
# line 2629 "src/yygram.cpp"
      } break;
   case 137: {
      get_lexval();
      switch (yyselect()) {
      case 634: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 596 "lang/grammar.acc"
 theItem->u.c.WeightLim = (int16)n; 
# line 2643 "src/yygram.cpp"
      } break;
   case 138: {
      get_lexval();
      switch (yyselect()) {
      case 635: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 597 "lang/grammar.acc"
 theItem->u.c.WeightMod = (int8)n; 
# line 2657 "src/yygram.cpp"
      } break;
   case 139: {
      get_lexval();
      switch (yyselect()) {
      case 636: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 598 "lang/grammar.acc"
 theItem->u.c.MaxSize = (int8)n; 
# line 2671 "src/yygram.cpp"
      } break;
   case 140: {
      get_lexval();
      switch (yyselect()) {
      case 637: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 599 "lang/grammar.acc"
 theItem->u.c.Timeout = (int8)n; 
# line 2685 "src/yygram.cpp"
      } break;
   case 141: {
      get_lexval();
      switch (yyselect()) {
      case 638: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 600 "lang/grammar.acc"
 theItem->u.c.CType = (int8)n; 
# line 2699 "src/yygram.cpp"
      } break;
   case 142: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 639: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 601 "lang/grammar.acc"
 theItem->u.l.LightRange = (int8)n; 
# line 2714 "src/yygram.cpp"
      } break;
   case 143: {
      get_lexval();
      switch (yyselect()) {
      case 640: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 602 "lang/grammar.acc"
 theItem->u.l.Lifespan = (int16)n; 
# line 2728 "src/yygram.cpp"
      } break;
   case 144: {
      get_lexval();
      switch (yyselect()) {
      case 641: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 603 "lang/grammar.acc"
 theItem->u.l.Fuel = rr; 
# line 2742 "src/yygram.cpp"
      } break;
   case 145: {
      dconst_entry();
      } break;
   case 146: {
      event_desc();
      } break;
   case 147: {
      r_declaration();
      } break;
   }
return 0;
}

int arm_entry ()
{
   YYSTYPE n;
   YYSTYPE wt;
   switch(yyselect()) {
   case 148: {
      cexpr3(&n);
      switch (yyselect()) {
      case 642: {
         get_lexval();
         } break;
      default: ;
      }
#line 610 "lang/grammar.acc"
 if (theItem->u.a.Arm[0] == 0) theItem->u.a.Arm[0] = (int8)n;
                            if (theItem->u.a.Arm[1] == 0) theItem->u.a.Arm[1] = (int8)n;
                            if (theItem->u.a.Arm[2] == 0) theItem->u.a.Arm[2] = (int8)n;
                         
# line 2775 "src/yygram.cpp"
      } break;
   case 149: {
      get_lexval();
      wt = yylval;
      cexpr3(&n);
      switch (yyselect()) {
      case 643: {
         get_lexval();
         } break;
      default: ;
      }
#line 614 "lang/grammar.acc"
 theItem->u.a.Arm[wt] = (int8)n; 
# line 2789 "src/yygram.cpp"
      } break;
   }
return 0;
}

int iflag_entry ()
{
   YYSTYPE itf;
   switch(yyselect()) {
   case 150: {
      cexpr3(&itf);
      switch (yyselect()) {
      case 644: {
         get_lexval();
         } break;
      default: ;
      }
#line 618 "lang/grammar.acc"
 theItem->SetFlag((uint16)itf); 
# line 2809 "src/yygram.cpp"
      } break;
   case 151: {
      get_lexval();
      cexpr3(&itf);
      switch (yyselect()) {
      case 645: {
         get_lexval();
         } break;
      default: ;
      }
#line 619 "lang/grammar.acc"
 theItem->UnsetFlag((uint16)itf); 
# line 2822 "src/yygram.cpp"
      } break;
   }
return 0;
}

int feature_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 152: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 625 "lang/grammar.acc"
 theRes = theFeat; theFeat->Name = name; theRes = theFeat; 
# line 2839 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 626 "lang/grammar.acc"
 theFeat->FType = (uint8)n; 
# line 2844 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 646: {
            feat_entry();
            } break;
         default: goto yy26;
         }
      }
      yy26: ;
      get_lexval();
#line 627 "lang/grammar.acc"
 theFeat++; 
# line 2858 "src/yygram.cpp"
      } break;
   }
return 0;
}

int feat_entry ()
{
   YYSTYPE lv;
   YYSTYPE m;
   uint16 img;
   YYSTYPE mov;
   YYSTYPE n;
   rID rr;
   Dice d;
   switch(yyselect()) {
   case 153: {
      get_lexval();
      switch (yyselect()) {
      case 647: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 631 "lang/grammar.acc"
 theFeat->Desc = lv; 
# line 2887 "src/yygram.cpp"
      } break;
   case 154: {
      get_lexval();
      switch (yyselect()) {
      case 648: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&m);
      get_lexval();
#line 632 "lang/grammar.acc"
 theFeat->Material = (int8)m; 
# line 2901 "src/yygram.cpp"
      } break;
   case 155: {
      glyph_entry(&img);
#line 633 "lang/grammar.acc"
 theFeat->Image = img; 
# line 2907 "src/yygram.cpp"
      } break;
   case 156: {
      get_lexval();
      switch (yyselect()) {
      case 649: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 634 "lang/grammar.acc"
 theFeat->MoveMod = (int8)((mov-100)/5); 
# line 2922 "src/yygram.cpp"
      } break;
   case 157: {
      get_lexval();
      switch (yyselect()) {
      case 650: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 635 "lang/grammar.acc"
 theFeat->hp = (uint16)n; 
# line 2936 "src/yygram.cpp"
      } break;
   case 158: {
      get_lexval();
      switch (yyselect()) {
      case 651: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 654: {
            cexpr3(&n);
            switch (yyselect()) {
            case 652: {
               get_lexval();
               } break;
            case 653: {
               get_lexval();
               } break;
            default: ;
            }
#line 636 "lang/grammar.acc"
 theFeat->Flags |= n; 
# line 2961 "src/yygram.cpp"
            } break;
         default: goto yy27;
         }
      }
      yy27: ;
      get_lexval();
      } break;
   case 159: {
      switch (yyselect()) {
      case 655: {
         get_lexval();
         } break;
      case 656: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 657: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 637 "lang/grammar.acc"
 theFeat->xID = rr; 
# line 2989 "src/yygram.cpp"
      } break;
   case 160: {
      get_lexval();
      switch (yyselect()) {
      case 658: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 638 "lang/grammar.acc"
 theFeat->xval = (int16)n; 
# line 3003 "src/yygram.cpp"
      } break;
   case 161: {
      get_lexval();
      switch (yyselect()) {
      case 659: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 639 "lang/grammar.acc"
 theFeat->Factor = d; 
# line 3017 "src/yygram.cpp"
      } break;
   case 162: {
      event_desc();
      } break;
   case 163: {
      dconst_entry();
      } break;
   case 164: {
      r_declaration();
      } break;
   }
return 0;
}

int effect_def ()
{
   YYSTYPE n;
   YYSTYPE name;
   switch(yyselect()) {
   case 165: {
#line 645 "lang/grammar.acc"
 theRes = theEff; theEff->ef.sval = NOSAVE; 
# line 3040 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 661: {
            cexpr3(&n);
#line 646 "lang/grammar.acc"
 if (theEff->Sources[2])
                                        theEff->Sources[3] = (int8)n;
                                      else if (theEff->Sources[1])
                                        theEff->Sources[2] = (int8)n;
                                      else if (theEff->Sources[0])
                                        theEff->Sources[1] = (int8)n; 
                                      else
                                        theEff->Sources[0] = (int8)n; 
# line 3054 "src/yygram.cpp"
            switch (yyselect()) {
            case 660: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy28;
         }
      }
      yy28: ;
      switch (yyselect()) {
      case 662: {
         get_lexval();
         } break;
      case 663: {
         get_lexval();
         } break;
      case 664: {
         get_lexval();
#line 656 "lang/grammar.acc"
 theEff->ef.aval = AR_POISON; 
# line 3077 "src/yygram.cpp"
         } break;
      case 665: {
         get_lexval();
#line 657 "lang/grammar.acc"
 theEff->ef.aval = AR_DISEASE; 
# line 3083 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      name = yylval;
#line 658 "lang/grammar.acc"
 theEff->Name = name; 
# line 3091 "src/yygram.cpp"
      get_lexval();
      cexpr3(&n);
#line 659 "lang/grammar.acc"
 theValsIdx = 0; 
                                      theEff->ef.eval = (int8)n; 
# line 3097 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 666: {
            eff_entry();
            } break;
         default: goto yy29;
         }
      }
      yy29: ;
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 668: {
            get_lexval();
            cexpr3(&n);
#line 662 "lang/grammar.acc"
  Annotation *a; EffectValues tmp; int32 t;
                               ESTABLISH_VALS
							                 memcpy(&tmp,theVals,sizeof(EffectValues));
                               if (!theEff->AnHead) {
                                 a = theEff->NewAnnot(AN_EFFECT, &theEff->AnHead);
                                 theValsIdx = theEff->AnHead;
                                 ASSERT(theValsIdx > 0);
                                 }
                               else {
                                 a = theEff->Annot(theEff->AnHead);
                                 while (a->Next) 
                                   a = theEff->Annot(a->Next);
                                 a = theEff->NewAnnot(AN_EFFECT,&a->Next,&t);
                                 theValsIdx = t;
                                 ASSERT(theValsIdx > 0);
                                 }
                               a->Next = 0;
                               memcpy(&(a->u.ef),&tmp,sizeof(EffectValues));
                               a->u.ef.eval = (int8)n;
                             
# line 3135 "src/yygram.cpp"
            get_lexval();
            while (1) {
               switch (yyselect()) {
               case 667: {
                  eff_entry();
                  } break;
               default: goto yy31;
               }
            }
            yy31: ;
            get_lexval();
            } break;
         default: goto yy30;
         }
      }
      yy30: ;
#line 684 "lang/grammar.acc"
 theEff++; 
# line 3154 "src/yygram.cpp"
      } break;
   }
return 0;
}

int eff_entry ()
{
   YYSTYPE lv;
   YYSTYPE sc;
   YYSTYPE n;
   YYSTYPE n2;
   YYSTYPE sv;
   Dice d;
   rID rr;
   switch(yyselect()) {
   case 166: {
      get_lexval();
      switch (yyselect()) {
      case 669: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 688 "lang/grammar.acc"
 theEff->Desc = lv; 
# line 3183 "src/yygram.cpp"
      } break;
   case 167: {
      cexpr(&sc);
      get_lexval();
#line 689 "lang/grammar.acc"
 theEff->Schools |= sc; 
# line 3190 "src/yygram.cpp"
      } break;
   case 168: {
      get_lexval();
      switch (yyselect()) {
      case 670: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 690 "lang/grammar.acc"
 theEff->Level = (int8)n; 
# line 3204 "src/yygram.cpp"
      } break;
   case 169: {
      get_lexval();
      switch (yyselect()) {
      case 671: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 691 "lang/grammar.acc"
 theEff->ManaCost = (uint8)n; 
# line 3218 "src/yygram.cpp"
      } break;
   case 170: {
      get_lexval();
      switch (yyselect()) {
      case 672: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 673: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      switch (yyselect()) {
      case 674: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
#line 692 "lang/grammar.acc"
 theEff->BaseChance = (uint8)n; 
# line 3244 "src/yygram.cpp"
      } break;
   case 171: {
      get_lexval();
      switch (yyselect()) {
      case 675: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
#line 693 "lang/grammar.acc"
 theEff->Purpose = n; 
# line 3257 "src/yygram.cpp"
      switch (yyselect()) {
      case 676: {
         get_lexval();
         cexpr(&n2);
         get_lexval();
#line 694 "lang/grammar.acc"
 theEff->Purpose |= n2<<16; 
# line 3265 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      } break;
   case 172: {
      get_lexval();
      switch (yyselect()) {
      case 677: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&sv);
      switch (yyselect()) {
      case 678: {
         get_lexval();
#line 695 "lang/grammar.acc"
 theEff->SetFlag(EF_PARTIAL); 
# line 3285 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 696 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->sval = (int8)sv; 
# line 3292 "src/yygram.cpp"
      } break;
   case 173: {
      get_lexval();
      switch (yyselect()) {
      case 679: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 697 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->qval = (int8)n; 
# line 3306 "src/yygram.cpp"
      } break;
   case 174: {
      get_lexval();
      switch (yyselect()) {
      case 680: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 698 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->xval = (uint8)n; 
# line 3320 "src/yygram.cpp"
      } break;
   case 175: {
      get_lexval();
      switch (yyselect()) {
      case 681: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 699 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->yval = (int16)n; 
# line 3334 "src/yygram.cpp"
      } break;
   case 176: {
      get_lexval();
      switch (yyselect()) {
      case 682: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 700 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->dval = (int8)n; 
# line 3348 "src/yygram.cpp"
      } break;
   case 177: {
      get_lexval();
      switch (yyselect()) {
      case 683: {
         get_lexval();
         } break;
      default: ;
      }
      dice_val(&d);
      get_lexval();
#line 701 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->pval = d; 
# line 3362 "src/yygram.cpp"
      } break;
   case 178: {
      get_lexval();
      switch (yyselect()) {
      case 684: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 702 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->aval = (int8)n; 
# line 3376 "src/yygram.cpp"
      } break;
   case 179: {
      get_lexval();
      switch (yyselect()) {
      case 685: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 703 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->lval = (int8)n; 
# line 3390 "src/yygram.cpp"
      } break;
   case 180: {
      get_lexval();
      switch (yyselect()) {
      case 686: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 704 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->tval = (int8)n; 
# line 3404 "src/yygram.cpp"
      } break;
   case 181: {
      get_lexval();
      switch (yyselect()) {
      case 687: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 705 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->rval = rr; 
# line 3418 "src/yygram.cpp"
      } break;
   case 182: {
      get_lexval();
      switch (yyselect()) {
      case 688: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 706 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->rval = n; 
# line 3432 "src/yygram.cpp"
      } break;
   case 183: {
      get_lexval();
      switch (yyselect()) {
      case 689: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 707 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->cval = (int8)n; 
# line 3446 "src/yygram.cpp"
      } break;
   case 184: {
      get_lexval();
      switch (yyselect()) {
      case 690: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      n = yylval;
      get_lexval();
#line 708 "lang/grammar.acc"
 ESTABLISH_VALS; theVals->cval = (int8)n; 
# line 3461 "src/yygram.cpp"
      } break;
   case 185: {
      get_lexval();
      switch (yyselect()) {
      case 691: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 694: {
            cexpr3(&n);
#line 709 "lang/grammar.acc"
 theEff->SetFlag((uint16)n); 
# line 3477 "src/yygram.cpp"
            switch (yyselect()) {
            case 692: {
               get_lexval();
               } break;
            case 693: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy32;
         }
      }
      yy32: ;
      get_lexval();
      } break;
   case 186: {
      event_desc();
      } break;
   case 187: {
      r_declaration();
      } break;
   case 188: {
      dconst_entry();
      } break;
   }
return 0;
}

int numlist (YYSTYPE *val)
{
   YYSTYPE n;
   switch(yyselect()) {
   case 189: {
#line 716 "lang/grammar.acc"
 *val = 0; 
# line 3514 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 697: {
            cexpr3(&n);
#line 717 "lang/grammar.acc"
 *val |= n; 
# line 3521 "src/yygram.cpp"
            switch (yyselect()) {
            case 695: {
               get_lexval();
               } break;
            case 696: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy33;
         }
      }
      yy33: ;
      } break;
   }
return 0;
}

int object_entry ()
{
   switch(yyselect()) {
   case 190: {
      get_lexval();
      } break;
   }
return 0;
}

int race_def ()
{
   YYSTYPE name;
   rID base;
   switch(yyselect()) {
   case 191: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 726 "lang/grammar.acc"
 theRes = theRace; theRace->Name = name;
                                    theRace->BaseRace = 0; 
# line 3563 "src/yygram.cpp"
      switch (yyselect()) {
      case 698: {
         get_lexval();
         res_ref(&base);
#line 728 "lang/grammar.acc"
 TRace tr; 
                                    theRace->BaseRace = base;                                        
                                    if (TRACE(base) > theRace)
                                      Fatal("Subrace definition before def of base race!"); 
# line 3573 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 699: {
            race_entry();
            } break;
         default: goto yy34;
         }
      }
      yy34: ;
      get_lexval();
#line 733 "lang/grammar.acc"
 theRace++; 
# line 3590 "src/yygram.cpp"
      } break;
   }
return 0;
}

int race_entry ()
{
   YYSTYPE lv;
   YYSTYPE at;
   YYSTYPE v;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   rID rr;
   rID rr2;
   rID rr3;
   YYSTYPE sk;
   YYSTYPE n;
   switch(yyselect()) {
   case 192: {
      get_lexval();
      switch (yyselect()) {
      case 700: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 737 "lang/grammar.acc"
 theRace->Desc = lv; 
# line 3625 "src/yygram.cpp"
      } break;
   case 193: {
      while (1) {
         switch (yyselect()) {
         case 703: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 701: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr(&v);
#line 738 "lang/grammar.acc"
 theRace->AttrAdj[at] = (int8)v; 
# line 3642 "src/yygram.cpp"
            switch (yyselect()) {
            case 702: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy35;
         }
      }
      yy35: ;
      get_lexval();
      } break;
   case 194: {
      get_lexval();
      switch (yyselect()) {
      case 704: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 706: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 705: {
               get_lexval();
               } break;
            default: ;
            }
#line 741 "lang/grammar.acc"
 theRace->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 3676 "src/yygram.cpp"
            } break;
         default: goto yy36;
         }
      }
      yy36: ;
      get_lexval();
      } break;
   case 195: {
      get_lexval();
      switch (yyselect()) {
      case 707: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 742 "lang/grammar.acc"
 theRace->mID = rr; 
# line 3696 "src/yygram.cpp"
      } break;
   case 196: {
      get_lexval();
      switch (yyselect()) {
      case 708: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 709: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
      res_ref(&rr2);
      get_lexval();
#line 744 "lang/grammar.acc"
 theRace->FavouredClass[0] = rr;
                                  theRace->FavouredClass[1] = rr2; 
# line 3719 "src/yygram.cpp"
      } break;
   case 197: {
      get_lexval();
      switch (yyselect()) {
      case 710: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 711: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
      res_ref(&rr2);
      get_lexval();
      res_ref(&rr3);
      get_lexval();
#line 747 "lang/grammar.acc"
 theRace->FavouredClass[0] = rr;
                                  theRace->FavouredClass[1] = rr2; 
                                  theRace->FavouredClass[2] = rr3; 
# line 3745 "src/yygram.cpp"
      } break;
   case 198: {
      get_lexval();
      switch (yyselect()) {
      case 712: {
         get_lexval();
         } break;
      default: ;
      }
#line 750 "lang/grammar.acc"
int8 skn = 0;
# line 3757 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 714: {
            cexpr3(&sk);
#line 751 "lang/grammar.acc"
 theRace->Skills[skn++] = (int8)sk; 
# line 3764 "src/yygram.cpp"
            switch (yyselect()) {
            case 713: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy37;
         }
      }
      yy37: ;
      get_lexval();
      } break;
   case 199: {
      cexpr3(&n);
      switch (yyselect()) {
      case 715: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 752 "lang/grammar.acc"
 switch(n) {
                                         case MALE_NAMES  : theRace-> MNames = lv; break;
                                         case FEMALE_NAMES: theRace-> FNames = lv; break;
                                         case FAMILY_NAMES: theRace-> SNames = lv; break;
                                         default: yyerror("Illegal name const in race!");
                                         } 
# line 3796 "src/yygram.cpp"
      } break;
   case 200: {
      get_lexval();
      switch (yyselect()) {
      case 716: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 719: {
            cexpr3(&n);
            switch (yyselect()) {
            case 717: {
               get_lexval();
               } break;
            case 718: {
               get_lexval();
               } break;
            default: ;
            }
#line 758 "lang/grammar.acc"
 theRace->SetFlag((uint16)n); 
# line 3821 "src/yygram.cpp"
            } break;
         default: goto yy38;
         }
      }
      yy38: ;
      get_lexval();
      } break;
   case 201: {
      gear_desc();
      } break;
   case 202: {
      dconst_entry();
      } break;
   case 203: {
      event_desc();
      } break;
   case 204: {
      r_declaration();
      } break;
   }
return 0;
}

int class_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 205: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 766 "lang/grammar.acc"
 theRes = theClass; theClass->Name = name; 
# line 3855 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 720: {
            class_entry();
            } break;
         default: goto yy39;
         }
      }
      yy39: ;
      get_lexval();
#line 767 "lang/grammar.acc"
 theClass++; 
# line 3869 "src/yygram.cpp"
      } break;
   }
return 0;
}

int class_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   YYSTYPE x;
   YYSTYPE typ;
   YYSTYPE l;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   YYSTYPE sp;
   YYSTYPE sk;
   YYSTYPE pr;
   switch(yyselect()) {
   case 206: {
      get_lexval();
      switch (yyselect()) {
      case 721: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 771 "lang/grammar.acc"
 theClass->Desc = lv; 
# line 3904 "src/yygram.cpp"
      } break;
   case 207: {
      get_lexval();
      switch (yyselect()) {
      case 722: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 723: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 772 "lang/grammar.acc"
 theClass->HitDie = (uint8)n; 
# line 3924 "src/yygram.cpp"
      } break;
   case 208: {
      get_lexval();
      switch (yyselect()) {
      case 724: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 725: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 773 "lang/grammar.acc"
 theClass->ManaDie = (uint8)n; 
# line 3944 "src/yygram.cpp"
      } break;
   case 209: {
      get_lexval();
      switch (yyselect()) {
      case 726: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&x);
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 774 "lang/grammar.acc"
 theClass->DefMod = (uint8)n;
                                             if (x != 1) yyerror("class defense mod must by 1 per N."); 
# line 3961 "src/yygram.cpp"
      } break;
   case 210: {
      get_lexval();
      switch (yyselect()) {
      case 727: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 729: {
            cexpr3(&typ);
            cexpr3(&l);
#line 777 "lang/grammar.acc"
 theClass->AttkVal[typ] = (uint8)l; 
# line 3978 "src/yygram.cpp"
            switch (yyselect()) {
            case 728: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy40;
         }
      }
      yy40: ;
      get_lexval();
      } break;
   case 211: {
      get_lexval();
      switch (yyselect()) {
      case 730: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 732: {
            cexpr3(&typ);
            cexpr3(&l);
#line 779 "lang/grammar.acc"
 theClass->Saves[typ] = (uint8)l; 
# line 4007 "src/yygram.cpp"
            switch (yyselect()) {
            case 731: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy41;
         }
      }
      yy41: ;
      get_lexval();
      } break;
   case 212: {
      get_lexval();
      switch (yyselect()) {
      case 733: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 735: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 734: {
               get_lexval();
               } break;
            default: ;
            }
#line 781 "lang/grammar.acc"
 theClass->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 4041 "src/yygram.cpp"
            } break;
         default: goto yy42;
         }
      }
      yy42: ;
      get_lexval();
      } break;
   case 213: {
      get_lexval();
      switch (yyselect()) {
      case 736: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&sp);
      switch (yyselect()) {
      case 737: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 782 "lang/grammar.acc"
 theClass->SkillPoints = (uint8)sp; int8 skn = 0; 
# line 4066 "src/yygram.cpp"
      switch (yyselect()) {
      case 738: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 740: {
            cexpr3(&sk);
#line 783 "lang/grammar.acc"
 theClass->Skills[skn++] = (uint8)sk; 
# line 4079 "src/yygram.cpp"
            switch (yyselect()) {
            case 739: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy43;
         }
      }
      yy43: ;
      get_lexval();
      } break;
   case 214: {
      get_lexval();
      switch (yyselect()) {
      case 741: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 743: {
            cexpr3(&pr);
#line 785 "lang/grammar.acc"
 theClass->Proficiencies |= pr; 
# line 4107 "src/yygram.cpp"
            switch (yyselect()) {
            case 742: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy44;
         }
      }
      yy44: ;
      get_lexval();
      } break;
   case 215: {
      get_lexval();
      switch (yyselect()) {
      case 744: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 747: {
            cexpr3(&n);
            switch (yyselect()) {
            case 745: {
               get_lexval();
               } break;
            case 746: {
               get_lexval();
               } break;
            default: ;
            }
#line 786 "lang/grammar.acc"
 theClass->SetFlag((uint16)n); 
# line 4144 "src/yygram.cpp"
            } break;
         default: goto yy45;
         }
      }
      yy45: ;
      get_lexval();
      } break;
   case 216: {
      gear_desc();
      } break;
   case 217: {
      event_desc();
      } break;
   case 218: {
      dconst_entry();
      } break;
   case 219: {
      r_declaration();
      } break;
   }
return 0;
}

int special_ability (YYSTYPE *AbT, YYSTYPE *Abil, YYSTYPE *p, YYSTYPE *l1, YYSTYPE *l2)
{
   YYSTYPE ca;
   YYSTYPE xp;
   YYSTYPE xl1;
   YYSTYPE xl2;
   YYSTYPE st;
   YYSTYPE ft;
   switch(yyselect()) {
   case 220: {
#line 794 "lang/grammar.acc"
 *p = 0; 
# line 4180 "src/yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ca);
      switch (yyselect()) {
      case 748: {
         abil_param(&xp);
#line 794 "lang/grammar.acc"
 *p = xp; 
# line 4189 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 795 "lang/grammar.acc"
 *AbT = AB_ABILITY; *Abil = ca; *l1 =xl1; *l2 = xl2; 
# line 4198 "src/yygram.cpp"
      } break;
   case 221: {
#line 796 "lang/grammar.acc"
 *p = 0; 
# line 4203 "src/yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&st);
      switch (yyselect()) {
      case 749: {
         abil_param(&xp);
#line 796 "lang/grammar.acc"
 *p = xp; 
# line 4212 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 797 "lang/grammar.acc"
 *AbT = AB_STATI;   *Abil = st; *l1 = xl1; *l2 = xl2; 
# line 4221 "src/yygram.cpp"
      } break;
   case 222: {
#line 798 "lang/grammar.acc"
 *p = 0; 
# line 4226 "src/yygram.cpp"
      get_lexval();
      get_lexval();
      cexpr3(&ft);
      switch (yyselect()) {
      case 750: {
         abil_param(&xp);
#line 798 "lang/grammar.acc"
 *p = xp; 
# line 4235 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      abil_level(&xl1, &xl2);
#line 799 "lang/grammar.acc"
 *AbT = AB_FEAT;    *Abil = ft; *l1 = xl1; *l2 = xl2; 
# line 4244 "src/yygram.cpp"
      } break;
   }
return 0;
}

int abil_param (YYSTYPE *p)
{
   YYSTYPE pa;
   rID rr;
   YYSTYPE pa2;
   switch(yyselect()) {
   case 223: {
      get_lexval();
      cexpr3(&pa);
#line 803 "lang/grammar.acc"
 *p = pa; 
# line 4261 "src/yygram.cpp"
      } break;
   case 224: {
      get_lexval();
      res_ref(&rr);
#line 804 "lang/grammar.acc"
 *p = rr; 
# line 4268 "src/yygram.cpp"
      } break;
   case 225: {
      get_lexval();
      cexpr3(&pa);
      get_lexval();
      cexpr3(&pa2);
#line 805 "lang/grammar.acc"
 *p = pa | (pa2 << 16); 
# line 4277 "src/yygram.cpp"
      } break;
   }
return 0;
}

int abil_level (YYSTYPE *l1, YYSTYPE *l2)
{
   YYSTYPE xl1;
   YYSTYPE xl2;
   switch(yyselect()) {
   case 226: {
      cexpr3(&xl1);
      switch (yyselect()) {
      case 751: {
         get_lexval();
         } break;
      default: ;
      }
#line 810 "lang/grammar.acc"
 *l1 = xl1; *l2 = 0; 
# line 4298 "src/yygram.cpp"
      } break;
   case 227: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
#line 812 "lang/grammar.acc"
 *l1 = 1;  *l2 = xl2; 
# line 4306 "src/yygram.cpp"
      } break;
   case 228: {
      get_lexval();
      get_lexval();
#line 814 "lang/grammar.acc"
 *l1 = 1;  *l2 = 1; 
# line 4313 "src/yygram.cpp"
      } break;
   case 229: {
      get_lexval();
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 752: {
         get_lexval();
         } break;
      default: ;
      }
#line 816 "lang/grammar.acc"
 *l1 = xl1; *l2 = 1; 
# line 4329 "src/yygram.cpp"
      } break;
   case 230: {
      get_lexval();
      cexpr3(&xl2);
      get_lexval();
      get_lexval();
      get_lexval();
      cexpr3(&xl1);
      switch (yyselect()) {
      case 753: {
         get_lexval();
         } break;
      default: ;
      }
#line 818 "lang/grammar.acc"
 *l1 = xl1; *l2 = xl2; 
# line 4346 "src/yygram.cpp"
      } break;
   }
return 0;
}

int dungeon_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 231: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 822 "lang/grammar.acc"
 theRes = theDgn; theDgn->Name = name; 
# line 4362 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 754: {
            dun_entry();
            } break;
         default: goto yy46;
         }
      }
      yy46: ;
      get_lexval();
#line 823 "lang/grammar.acc"
 theDgn++; 
# line 4376 "src/yygram.cpp"
      } break;
   }
return 0;
}

int dun_entry ()
{
   YYSTYPE lv;
   switch(yyselect()) {
   case 232: {
      get_lexval();
      switch (yyselect()) {
      case 755: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 827 "lang/grammar.acc"
 theDgn->Desc = lv; 
# line 4399 "src/yygram.cpp"
      } break;
   case 233: {
      dconst_entry();
      } break;
   case 234: {
      event_desc();
      } break;
   case 235: {
      r_declaration();
      } break;
   }
return 0;
}

int dconst_entry ()
{
   YYSTYPE con;
   YYSTYPE val;
   rID rr;
   YYSTYPE lis;
   YYSTYPE n;
   Dice dv;
   switch(yyselect()) {
   case 236: {
      get_lexval();
      switch (yyselect()) {
      case 756: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 758: {
            get_lexval();
            cexpr3(&con);
            cexpr3(&val);
#line 835 "lang/grammar.acc"
 theRes->AddConstant((int8)con,val); 
# line 4439 "src/yygram.cpp"
            switch (yyselect()) {
            case 757: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         case 760: {
            get_lexval();
            cexpr3(&con);
            res_ref(&rr);
#line 836 "lang/grammar.acc"
 theRes->AddConstant((int8)con,rr); 
# line 4453 "src/yygram.cpp"
            switch (yyselect()) {
            case 759: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy47;
         }
      }
      yy47: ;
      get_lexval();
      } break;
   case 237: {
      get_lexval();
      switch (yyselect()) {
      case 761: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 765: {
            get_lexval();
            cexpr4(&lis);
#line 839 "lang/grammar.acc"
 TempListLoc = 0; 
# line 4482 "src/yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 762: {
                  list_entry();
                  } break;
               default: goto yy49;
               }
            }
            yy49: ;
            switch (yyselect()) {
            case 763: {
               get_lexval();
               } break;
            case 764: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
#line 840 "lang/grammar.acc"
 TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        TempList[TempListLoc++] = 0;
        ASSERT(TempListLoc < 1023);
        theRes->AddList((int8)lis,TempList); 
# line 4507 "src/yygram.cpp"
            } break;
         default: goto yy48;
         }
      }
      yy48: ;
      } break;
   case 238: {
      get_lexval();
      switch (yyselect()) {
      case 766: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 772: {
#line 847 "lang/grammar.acc"
 int16 Chance = 100; 
# line 4527 "src/yygram.cpp"
            get_lexval();
            switch (yyselect()) {
            case 767: {
               cexpr4(&n);
               get_lexval();
#line 847 "lang/grammar.acc"
 Chance = (int16)n; 
# line 4535 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 768: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               cexpr3(&n);
#line 848 "lang/grammar.acc"
 theRes->AddSpecial(rr,Chance,(int16)n); 
# line 4547 "src/yygram.cpp"
               } break;
            case 769: {
               res_ref(&rr);
               get_lexval();
               cexpr3(&n);
               get_lexval();
#line 849 "lang/grammar.acc"
 theRes->AddSpecial(rr,Chance,(int16)n); 
# line 4556 "src/yygram.cpp"
               } break;
            case 770: {
               res_ref(&rr);
               get_lexval();
               get_lexval();
               dice_only(&dv);
#line 850 "lang/grammar.acc"
 theRes->AddSpecial(rr,Chance,yyDice(dv)); 
# line 4565 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 771: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy50;
         }
      }
      yy50: ;
      get_lexval();
      } break;
   }
return 0;
}

int list_entry ()
{
   rID rr;
   YYSTYPE n;
   YYSTYPE text;
   switch(yyselect()) {
   case 239: {
      res_ref(&rr);
#line 855 "lang/grammar.acc"
 TempList[TempListLoc++] = rr; ASSERT(TempListLoc < 1023); 
# line 4596 "src/yygram.cpp"
      } break;
   case 240: {
      cexpr4(&n);
#line 856 "lang/grammar.acc"
 TempList[TempListLoc++] = n;  ASSERT(TempListLoc < 1023); 
# line 4602 "src/yygram.cpp"
      } break;
   case 241: {
      get_lexval();
      text = yylval;
#line 857 "lang/grammar.acc"
 TempList[TempListLoc++] = text; ASSERT(TempListLoc < 1023); 
# line 4609 "src/yygram.cpp"
      } break;
   }
return 0;
}

int artifact_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 242: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 861 "lang/grammar.acc"
 theRes = theArti; theArti->Name = name; 
# line 4625 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 773: {
            arti_entry();
            } break;
         default: goto yy51;
         }
      }
      yy51: ;
      get_lexval();
#line 862 "lang/grammar.acc"
 theArti++; 
# line 4639 "src/yygram.cpp"
      } break;
   }
return 0;
}

int arti_entry ()
{
   YYSTYPE lv;
   rID rr;
   YYSTYPE at;
   YYSTYPE v;
   YYSTYPE res;
   YYSTYPE qu;
   switch(yyselect()) {
   case 243: {
      get_lexval();
      switch (yyselect()) {
      case 774: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 866 "lang/grammar.acc"
 theRes = theArti; theArti->Desc = lv; 
# line 4667 "src/yygram.cpp"
      } break;
   case 244: {
      get_lexval();
      switch (yyselect()) {
      case 775: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 867 "lang/grammar.acc"
 theArti->iID = rr; 
# line 4681 "src/yygram.cpp"
      } break;
   case 245: {
      while (1) {
         switch (yyselect()) {
         case 778: {
            get_lexval();
            at = yylval;
            switch (yyselect()) {
            case 776: {
               get_lexval();
               } break;
            default: ;
            }
            cexpr3(&v);
#line 868 "lang/grammar.acc"
 theArti->AttrAdjLow[at] = (int8)v; 
# line 4698 "src/yygram.cpp"
            switch (yyselect()) {
            case 777: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy52;
         }
      }
      yy52: ;
      get_lexval();
      } break;
   case 246: {
      get_lexval();
      switch (yyselect()) {
      case 779: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 781: {
            cexpr3(&res);
#line 869 "lang/grammar.acc"
 theArti->Resists |= res; 
# line 4726 "src/yygram.cpp"
            switch (yyselect()) {
            case 780: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy53;
         }
      }
      yy53: ;
      get_lexval();
      } break;
   case 247: {
      get_lexval();
      switch (yyselect()) {
      case 782: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 784: {
            get_lexval();
            at = yylval;
#line 870 "lang/grammar.acc"
 theArti->Sustains |= 1 << (at-1); 
# line 4755 "src/yygram.cpp"
            switch (yyselect()) {
            case 783: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy54;
         }
      }
      yy54: ;
      get_lexval();
      } break;
   case 248: {
      get_lexval();
      switch (yyselect()) {
      case 785: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 787: {
            cexpr3(&qu);
#line 871 "lang/grammar.acc"
 theArti->Qualities |= qu; 
# line 4783 "src/yygram.cpp"
            switch (yyselect()) {
            case 786: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy55;
         }
      }
      yy55: ;
      get_lexval();
      } break;
   case 249: {
      get_lexval();
#line 873 "lang/grammar.acc"
 ArtPowerType = AN_ART_EQUIP; 
# line 4801 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 789: {
            art_power();
            switch (yyselect()) {
            case 788: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy56;
         }
      }
      yy56: ;
      get_lexval();
      } break;
   case 250: {
      get_lexval();
#line 874 "lang/grammar.acc"
 ArtPowerType = AN_ART_WIELD; 
# line 4824 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 791: {
            art_power();
            switch (yyselect()) {
            case 790: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy57;
         }
      }
      yy57: ;
      get_lexval();
      } break;
   case 251: {
      get_lexval();
#line 875 "lang/grammar.acc"
 ArtPowerType = AN_ART_HIT;   
# line 4847 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 793: {
            art_power();
            switch (yyselect()) {
            case 792: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy58;
         }
      }
      yy58: ;
      get_lexval();
      } break;
   case 252: {
      get_lexval();
#line 876 "lang/grammar.acc"
 ArtPowerType = AN_ART_INVOKE;
# line 4870 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 795: {
            art_power();
            switch (yyselect()) {
            case 794: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy59;
         }
      }
      yy59: ;
      get_lexval();
      } break;
   case 253: {
      event_desc();
      } break;
   case 254: {
      r_declaration();
      } break;
   }
return 0;
}

int art_power ()
{
   YYSTYPE st;
   YYSTYPE p;
   rID rr;
   Dice dv;
   YYSTYPE n;
   YYSTYPE ft;
   switch(yyselect()) {
   case 255: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&p);
      get_lexval();
#line 883 "lang/grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_STATI,st,(int16)p,NULL); 
# line 4917 "src/yygram.cpp"
      } break;
   case 256: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
#line 885 "lang/grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_STATI,st,0,NULL); 
# line 4926 "src/yygram.cpp"
      } break;
   case 257: {
      res_ref(&rr);
      switch (yyselect()) {
      case 796: {
         get_lexval();
         get_lexval();
         } break;
      default: ;
      }
#line 887 "lang/grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,NULL); 
# line 4939 "src/yygram.cpp"
      } break;
   case 258: {
      res_ref(&rr);
      get_lexval();
      dice_val(&dv);
      get_lexval();
#line 889 "lang/grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,&yyDice(dv)); 
# line 4948 "src/yygram.cpp"
      } break;
   case 259: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 797: {
         get_lexval();
         } break;
      case 798: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 891 "lang/grammar.acc"
 Dice d; d.Set(0,(int8)n,1);
        theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4966 "src/yygram.cpp"
      } break;
   case 260: {
      res_ref(&rr);
      cexpr3(&n);
      switch (yyselect()) {
      case 799: {
         get_lexval();
         } break;
      case 800: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
#line 894 "lang/grammar.acc"
 Dice d; d.Set(0,(int8)n,7);
        theArti->AddPower((int8)ArtPowerType,AB_EFFECT,rr,0,&d); 
# line 4984 "src/yygram.cpp"
      } break;
   case 261: {
      get_lexval();
      res_ref(&rr);
#line 897 "lang/grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_INNATE,rr,0,NULL); 
# line 4991 "src/yygram.cpp"
      } break;
   case 262: {
      get_lexval();
      cexpr3(&ft);
#line 899 "lang/grammar.acc"
 theArti->AddPower((int8)ArtPowerType,AB_FEAT,ft,0,NULL); 
# line 4998 "src/yygram.cpp"
      } break;
   }
return 0;
}

int domain_def ()
{
   YYSTYPE name;
   YYSTYPE type;
   switch(yyselect()) {
   case 263: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&type);
#line 903 "lang/grammar.acc"
 theRes = theDom; theDom->Name = name; theDom->DType = (int8)type; 
# line 5017 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 801: {
            domain_entry();
            } break;
         default: goto yy60;
         }
      }
      yy60: ;
      get_lexval();
#line 905 "lang/grammar.acc"
 theDom++; 
# line 5031 "src/yygram.cpp"
      } break;
   }
return 0;
}

int domain_entry ()
{
   rID rr;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   YYSTYPE lv;
   switch(yyselect()) {
   case 264: {
#line 909 "lang/grammar.acc"
 int8 sp = 0; 
# line 5050 "src/yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 802: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 804: {
            res_ref(&rr);
#line 910 "lang/grammar.acc"
 theDom->Spells[min(sp,8)] = rr; sp++; 
# line 5064 "src/yygram.cpp"
            switch (yyselect()) {
            case 803: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy61;
         }
      }
      yy61: ;
      get_lexval();
#line 911 "lang/grammar.acc"
 if (sp > 9) 
      { yyerror(Format("Error: Domain %s has %d spells; must have at most nine.",
        theModule->QTextSeg + theDom->Name,sp)); } 
# line 5081 "src/yygram.cpp"
      } break;
   case 265: {
      get_lexval();
      switch (yyselect()) {
      case 805: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 807: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 806: {
               get_lexval();
               } break;
            default: ;
            }
#line 915 "lang/grammar.acc"
 theDom->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 5103 "src/yygram.cpp"
            } break;
         default: goto yy62;
         }
      }
      yy62: ;
      get_lexval();
      } break;
   case 266: {
      get_lexval();
      switch (yyselect()) {
      case 808: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 916 "lang/grammar.acc"
 theDom->Desc = lv; 
# line 5124 "src/yygram.cpp"
      } break;
   case 267: {
      event_desc();
      } break;
   case 268: {
      dconst_entry();
      } break;
   case 269: {
      r_declaration();
      } break;
   }
return 0;
}

int god_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 270: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 922 "lang/grammar.acc"
 theRes = theGod; theGod->Name = name; 
# line 5149 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 809: {
            god_entry();
            } break;
         default: goto yy63;
         }
      }
      yy63: ;
      get_lexval();
#line 924 "lang/grammar.acc"
 theGod++; 
# line 5163 "src/yygram.cpp"
      } break;
   }
return 0;
}

int god_entry ()
{
   rID rr;
   YYSTYPE n;
   YYSTYPE lv;
   YYSTYPE AbT;
   YYSTYPE Abil;
   YYSTYPE p;
   YYSTYPE l1;
   YYSTYPE l2;
   switch(yyselect()) {
   case 271: {
#line 928 "lang/grammar.acc"
 int8 dm = 0; 
# line 5183 "src/yygram.cpp"
      get_lexval();
      switch (yyselect()) {
      case 810: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 812: {
            res_ref(&rr);
#line 929 "lang/grammar.acc"
 theGod->Domains[min(dm,5)] = rr; dm++; 
# line 5197 "src/yygram.cpp"
            switch (yyselect()) {
            case 811: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy64;
         }
      }
      yy64: ;
      get_lexval();
#line 930 "lang/grammar.acc"
 if (dm < 3 || dm > 6) 
      { yyerror(Format("Error: God %s has %d domains; must have between 3 and 6.",
        theModule->QTextSeg + theGod->Name,dm)); } 
# line 5214 "src/yygram.cpp"
      } break;
   case 272: {
      get_lexval();
      get_lexval();
      } break;
   case 273: {
      get_lexval();
      switch (yyselect()) {
      case 813: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 816: {
            cexpr3(&n);
            switch (yyselect()) {
            case 814: {
               get_lexval();
               } break;
            case 815: {
               get_lexval();
               } break;
            default: ;
            }
#line 934 "lang/grammar.acc"
 theGod->SetFlag((uint16)n); 
# line 5243 "src/yygram.cpp"
            } break;
         default: goto yy65;
         }
      }
      yy65: ;
      get_lexval();
      } break;
   case 274: {
      get_lexval();
      switch (yyselect()) {
      case 817: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 935 "lang/grammar.acc"
 theGod->Desc = lv; 
# line 5264 "src/yygram.cpp"
      } break;
   case 275: {
      get_lexval();
      switch (yyselect()) {
      case 818: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 820: {
            special_ability(&AbT, &Abil, &p, &l1, &l2);
            switch (yyselect()) {
            case 819: {
               get_lexval();
               } break;
            default: ;
            }
#line 937 "lang/grammar.acc"
 theGod->AddAbility((int8)AbT,(int16)Abil,p,(int8)l1,(int8)l2); 
# line 5286 "src/yygram.cpp"
            } break;
         default: goto yy66;
         }
      }
      yy66: ;
      get_lexval();
      } break;
   case 276: {
      event_desc();
      } break;
   case 277: {
      dconst_entry();
      } break;
   case 278: {
      r_declaration();
      } break;
   }
return 0;
}

int terrain_def ()
{
   YYSTYPE name;
   switch(yyselect()) {
   case 279: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 943 "lang/grammar.acc"
 theRes = theTer; 
                                      theTer->Name = name; 
# line 5318 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 821: {
            terrain_entry();
            } break;
         default: goto yy67;
         }
      }
      yy67: ;
      get_lexval();
#line 946 "lang/grammar.acc"
 theTer++; 
# line 5332 "src/yygram.cpp"
      } break;
   }
return 0;
}

int terrain_entry ()
{
   YYSTYPE lv;
   uint16 img;
   YYSTYPE mov;
   YYSTYPE pen;
   YYSTYPE mat;
   YYSTYPE n;
   switch(yyselect()) {
   case 280: {
      get_lexval();
      switch (yyselect()) {
      case 822: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 950 "lang/grammar.acc"
 theTer->Desc = lv; 
# line 5360 "src/yygram.cpp"
      } break;
   case 281: {
      glyph_entry(&img);
#line 951 "lang/grammar.acc"
 theTer->Image = img; 
# line 5366 "src/yygram.cpp"
      } break;
   case 282: {
      get_lexval();
      switch (yyselect()) {
      case 823: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mov);
      get_lexval();
      get_lexval();
#line 952 "lang/grammar.acc"
 theTer->MoveMod = (int8)((mov-100)/5); 
# line 5381 "src/yygram.cpp"
      } break;
   case 283: {
      get_lexval();
      switch (yyselect()) {
      case 824: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&pen);
      get_lexval();
#line 953 "lang/grammar.acc"
 theTer->Penalty = (int8)pen; 
# line 5395 "src/yygram.cpp"
      } break;
   case 284: {
      get_lexval();
      switch (yyselect()) {
      case 825: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mat);
      get_lexval();
#line 954 "lang/grammar.acc"
 theTer->Material = (int8)mat; 
# line 5409 "src/yygram.cpp"
      } break;
   case 285: {
      get_lexval();
      switch (yyselect()) {
      case 826: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 829: {
            cexpr3(&n);
            switch (yyselect()) {
            case 827: {
               get_lexval();
               } break;
            case 828: {
               get_lexval();
               } break;
            default: ;
            }
#line 955 "lang/grammar.acc"
 theTer->SetFlag((uint16)n); 
# line 5434 "src/yygram.cpp"
            } break;
         default: goto yy68;
         }
      }
      yy68: ;
      get_lexval();
      } break;
   case 286: {
      dconst_entry();
      } break;
   case 287: {
      event_desc();
      } break;
   case 288: {
      r_declaration();
      } break;
   }
return 0;
}

int region_def ()
{
   YYSTYPE name;
   YYSTYPE regflag;
   switch(yyselect()) {
   case 289: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&regflag);
#line 962 "lang/grammar.acc"
 theRes = theReg; theReg->Name = name; theReg->SetFlag((uint16)regflag); 
    theReg->Size = SZ_COLLOSAL; 
# line 5469 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 830: {
            region_entry();
            } break;
         default: goto yy69;
         }
      }
      yy69: ;
      get_lexval();
#line 965 "lang/grammar.acc"
 theReg++; 
# line 5483 "src/yygram.cpp"
      } break;
   }
return 0;
}

int region_entry ()
{
   YYSTYPE lv;
   rID rr;
   YYSTYPE n;
   YYSTYPE mg;
   YYSTYPE ch;
   YYSTYPE v;
   rID rr2;
   YYSTYPE v2;
   uint16 g;
   switch(yyselect()) {
   case 290: {
      get_lexval();
      switch (yyselect()) {
      case 831: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 969 "lang/grammar.acc"
 theReg->Desc = lv; 
# line 5514 "src/yygram.cpp"
      } break;
   case 291: {
      get_lexval();
      switch (yyselect()) {
      case 832: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 970 "lang/grammar.acc"
 theReg->Walls = rr; 
# line 5528 "src/yygram.cpp"
      } break;
   case 292: {
      get_lexval();
      switch (yyselect()) {
      case 833: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 971 "lang/grammar.acc"
 theReg->Floor = rr; 
# line 5542 "src/yygram.cpp"
      } break;
   case 293: {
      get_lexval();
      switch (yyselect()) {
      case 834: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 972 "lang/grammar.acc"
 theReg->Door = rr; 
# line 5556 "src/yygram.cpp"
      } break;
   case 294: {
      get_lexval();
      switch (yyselect()) {
      case 835: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 973 "lang/grammar.acc"
 theReg->Depth = (int8)n; 
# line 5570 "src/yygram.cpp"
      } break;
   case 295: {
      get_lexval();
      switch (yyselect()) {
      case 836: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 974 "lang/grammar.acc"
 theReg->Size = (int8)n; 
# line 5584 "src/yygram.cpp"
      } break;
   case 296: {
      get_lexval();
      switch (yyselect()) {
      case 837: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 840: {
            cexpr3(&n);
#line 975 "lang/grammar.acc"
 theReg->RoomTypes |= BIT(n); 
# line 5600 "src/yygram.cpp"
            switch (yyselect()) {
            case 838: {
               get_lexval();
               } break;
            case 839: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy70;
         }
      }
      yy70: ;
      get_lexval();
      } break;
   case 297: {
      get_lexval();
      switch (yyselect()) {
      case 841: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 844: {
            cexpr3(&n);
            switch (yyselect()) {
            case 842: {
               get_lexval();
               } break;
            case 843: {
               get_lexval();
               } break;
            default: ;
            }
#line 977 "lang/grammar.acc"
 theReg->SetFlag((uint16)n); 
# line 5640 "src/yygram.cpp"
            } break;
         default: goto yy71;
         }
      }
      yy71: ;
      get_lexval();
      } break;
   case 298: {
      get_lexval();
      get_lexval();
      get_lexval();
      mg = yylval;
      get_lexval();
#line 978 "lang/grammar.acc"
 theReg->sx   = yyMapSize[currMap] % 256; 
                                    theReg->sy   = yyMapSize[currMap++] / 256;
                                    theReg->Grid = mg; 
# line 5658 "src/yygram.cpp"
      } break;
   case 299: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 857: {
#line 981 "lang/grammar.acc"
 rID tID, xID, xID2; Glyph Img; uint8 fl; 
                                    tID = xID = xID2 = 0; Img = 0; fl = 0; 
# line 5669 "src/yygram.cpp"
            get_lexval();
            ch = yylval;
            get_lexval();
            res_ref(&rr);
#line 983 "lang/grammar.acc"
 tID = rr; 
# line 5676 "src/yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 845: {
                  get_lexval();
                  cexpr(&n);
                  get_lexval();
#line 984 "lang/grammar.acc"
 fl |= n; 
# line 5685 "src/yygram.cpp"
                  } break;
               case 854: {
                  get_lexval();
                  switch (yyselect()) {
                  case 846: {
                     res_ref(&rr);
#line 985 "lang/grammar.acc"
 xID = rr; 
# line 5694 "src/yygram.cpp"
                     } break;
                  case 848: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 847: {
                        cexpr(&v);
#line 986 "lang/grammar.acc"
 xID = v; 
# line 5704 "src/yygram.cpp"
                        } break;
                     default: ;
                     }
#line 987 "lang/grammar.acc"
 fl |= TILE_RANDOM | TILE_ITEM; 
# line 5710 "src/yygram.cpp"
                     } break;
                  case 850: {
                     get_lexval();
                     get_lexval();
                     switch (yyselect()) {
                     case 849: {
                        cexpr(&v);
#line 988 "lang/grammar.acc"
 xID = v; 
# line 5720 "src/yygram.cpp"
                        } break;
                     default: ;
                     }
#line 989 "lang/grammar.acc"
 fl |= TILE_RANDOM | TILE_MONSTER; 
# line 5726 "src/yygram.cpp"
                     } break;
                  default: printf("???\n"); exit(1);
                  }
                  switch (yyselect()) {
                  case 851: {
                     get_lexval();
                     res_ref(&rr2);
#line 990 "lang/grammar.acc"
 xID2 = rr2; 
# line 5736 "src/yygram.cpp"
                     } break;
                  case 852: {
                     get_lexval();
                     get_lexval();
                     cexpr(&v);
                     get_lexval();
                     cexpr(&v2);
#line 992 "lang/grammar.acc"
 fl |= TILE_RANDOM_EFF; xID2 = v+v2*256; 
# line 5746 "src/yygram.cpp"
                     } break;
                  case 853: {
                     get_lexval();
                     cexpr(&v);
#line 993 "lang/grammar.acc"
 xID2 = v; 
# line 5753 "src/yygram.cpp"
                     } break;
                  default: ;
                  }
                  } break;
               case 855: {
                  get_lexval();
                  glyph_desc(&g);
#line 994 "lang/grammar.acc"
 Img = g; 
# line 5763 "src/yygram.cpp"
                  } break;
               default: goto yy73;
               }
            }
            yy73: ;
#line 995 "lang/grammar.acc"
 theReg->AddTile((char)ch,Img,fl,tID,xID,xID2); 
# line 5771 "src/yygram.cpp"
            switch (yyselect()) {
            case 856: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy72;
         }
      }
      yy72: ;
      get_lexval();
      } break;
   case 300: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 858: {
            region_entry();
            } break;
         default: goto yy74;
         }
      }
      yy74: ;
      } break;
   case 301: {
      get_lexval();
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 859: {
            object_entry();
            } break;
         default: goto yy75;
         }
      }
      yy75: ;
      } break;
   case 302: {
      dconst_entry();
      } break;
   case 303: {
      event_desc();
      } break;
   case 304: {
      r_declaration();
      } break;
   }
return 0;
}

int template_def ()
{
   YYSTYPE name;
   YYSTYPE ty;
   YYSTYPE mt;
   switch(yyselect()) {
   case 305: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 1005 "lang/grammar.acc"
 theRes = theTemp; theTemp->Name = name; CurrFeat = 0; CurrAttk = 0; 
# line 5836 "src/yygram.cpp"
      get_lexval();
      cexpr3(&ty);
#line 1006 "lang/grammar.acc"
 theTemp->TType = (uint16)ty; 
# line 5841 "src/yygram.cpp"
      switch (yyselect()) {
      case 860: {
         get_lexval();
         cexpr3(&mt);
#line 1007 "lang/grammar.acc"
 theTemp->ForMType = mt; 
# line 5848 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 861: {
            template_entry();
            } break;
         default: goto yy76;
         }
      }
      yy76: ;
      get_lexval();
#line 1009 "lang/grammar.acc"
 theTemp++; 
# line 5865 "src/yygram.cpp"
      } break;
   }
return 0;
}

int template_entry ()
{
   YYSTYPE lv;
   YYSTYPE mt;
   uint16 img;
   MVal mv;
   YYSTYPE n;
   YYSTYPE at;
   YYSTYPE st;
   YYSTYPE stV;
   YYSTYPE stM;
   rID rr;
   YYSTYPE res;
   YYSTYPE fl;
   switch(yyselect()) {
   case 306: {
      get_lexval();
      switch (yyselect()) {
      case 862: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1013 "lang/grammar.acc"
 theTemp->Desc = lv; 
# line 5899 "src/yygram.cpp"
      } break;
   case 307: {
      get_lexval();
      switch (yyselect()) {
      case 863: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 1014 "lang/grammar.acc"
 theTemp->ForMType = mt; 
# line 5913 "src/yygram.cpp"
      } break;
   case 308: {
      get_lexval();
      switch (yyselect()) {
      case 864: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&mt);
      get_lexval();
#line 1015 "lang/grammar.acc"
 theTemp->AddMType = mt; 
# line 5927 "src/yygram.cpp"
      } break;
   case 309: {
      glyph_entry(&img);
#line 1016 "lang/grammar.acc"
 theTemp->NewImage = img; 
# line 5933 "src/yygram.cpp"
      } break;
   case 310: {
      switch (yyselect()) {
      case 865: {
         get_lexval();
         } break;
      case 866: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 867: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1017 "lang/grammar.acc"
 theTemp->CR = mv; 
# line 5955 "src/yygram.cpp"
      } break;
   case 311: {
      get_lexval();
      switch (yyselect()) {
      case 868: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1018 "lang/grammar.acc"
 theTemp->HitDice = mv; 
# line 5969 "src/yygram.cpp"
      } break;
   case 312: {
      get_lexval();
      switch (yyselect()) {
      case 869: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1019 "lang/grammar.acc"
 theTemp->Hit = mv; 
# line 5983 "src/yygram.cpp"
      } break;
   case 313: {
      get_lexval();
      switch (yyselect()) {
      case 870: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1020 "lang/grammar.acc"
 theTemp->Def = mv; 
# line 5997 "src/yygram.cpp"
      } break;
   case 314: {
      get_lexval();
      switch (yyselect()) {
      case 871: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1021 "lang/grammar.acc"
 theTemp->Arm = mv; 
# line 6011 "src/yygram.cpp"
      } break;
   case 315: {
      get_lexval();
      switch (yyselect()) {
      case 872: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr(&n);
      get_lexval();
#line 1022 "lang/grammar.acc"
 theTemp->DmgMod = (int8)n; 
# line 6025 "src/yygram.cpp"
      } break;
   case 316: {
      get_lexval();
      switch (yyselect()) {
      case 873: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1023 "lang/grammar.acc"
 theTemp->Power = mv; 
# line 6039 "src/yygram.cpp"
      } break;
   case 317: {
      get_lexval();
      get_lexval();
      switch (yyselect()) {
      case 874: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1024 "lang/grammar.acc"
 theTemp->CasterLev = mv; 
# line 6054 "src/yygram.cpp"
      } break;
   case 318: {
      get_lexval();
      switch (yyselect()) {
      case 875: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1025 "lang/grammar.acc"
 theTemp->Size = mv; 
# line 6068 "src/yygram.cpp"
      } break;
   case 319: {
      get_lexval();
      switch (yyselect()) {
      case 876: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1026 "lang/grammar.acc"
 theTemp->Spd = mv; 
# line 6082 "src/yygram.cpp"
      } break;
   case 320: {
      get_lexval();
      switch (yyselect()) {
      case 877: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      get_lexval();
#line 1027 "lang/grammar.acc"
 theTemp->Mov = mv; 
# line 6096 "src/yygram.cpp"
      } break;
   case 321: {
      get_lexval();
      at = yylval;
      switch (yyselect()) {
      case 878: {
         get_lexval();
         } break;
      default: ;
      }
      mval(&mv);
      switch (yyselect()) {
      case 879: {
         get_lexval();
         } break;
      case 880: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1028 "lang/grammar.acc"
 theTemp->Attr[at] = mv; 
# line 6119 "src/yygram.cpp"
      } break;
   case 322: {
      get_lexval();
      switch (yyselect()) {
      case 881: {
         get_lexval();
         } break;
      default: ;
      }
      temp_attack_entry();
      while (1) {
         switch (yyselect()) {
         case 882: {
            get_lexval();
            temp_attack_entry();
            } break;
         default: goto yy77;
         }
      }
      yy77: ;
      get_lexval();
      } break;
   case 323: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      get_lexval();
#line 1032 "lang/grammar.acc"
 if (!theTemp->GainStati((uint8)st)) yyerror("too many Stati in template"); 
# line 6150 "src/yygram.cpp"
      } break;
   case 324: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      get_lexval();
#line 1034 "lang/grammar.acc"
 if (!theTemp->GainStati((uint8)st,(int16)stV)) yyerror("too many Stati in template"); 
# line 6162 "src/yygram.cpp"
      } break;
   case 325: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      get_lexval();
#line 1036 "lang/grammar.acc"
 if (!theTemp->GainStati((uint8)st,(int16)stV,(int16)stM)) 
        yyerror("too many Stati in template"); 
# line 6177 "src/yygram.cpp"
      } break;
   case 326: {
      get_lexval();
      get_lexval();
      cexpr3(&st);
      get_lexval();
      cexpr3(&stV);
      get_lexval();
      cexpr3(&stM);
      get_lexval();
      res_ref(&rr);
      get_lexval();
      get_lexval();
#line 1039 "lang/grammar.acc"
 if (!theTemp->GainStati((uint8)st,(int16)stV,(int16)stM,rr)) 
        yyerror("too many Stati in template"); 
# line 6194 "src/yygram.cpp"
      } break;
   case 327: {
      get_lexval();
      switch (yyselect()) {
      case 883: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 889: {
            switch (yyselect()) {
            case 884: {
               cexpr3(&res);
#line 1042 "lang/grammar.acc"
 theTemp->AddRes |= res; 
# line 6212 "src/yygram.cpp"
               } break;
            case 887: {
               switch (yyselect()) {
               case 885: {
                  get_lexval();
                  } break;
               case 886: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1043 "lang/grammar.acc"
 theTemp->SubRes |= res; 
# line 6227 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 888: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy78;
         }
      }
      yy78: ;
      get_lexval();
      } break;
   case 328: {
      get_lexval();
      switch (yyselect()) {
      case 890: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 896: {
            switch (yyselect()) {
            case 891: {
               cexpr3(&res);
#line 1045 "lang/grammar.acc"
 theTemp->AddImm |= res; 
# line 6260 "src/yygram.cpp"
               } break;
            case 894: {
               switch (yyselect()) {
               case 892: {
                  get_lexval();
                  } break;
               case 893: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&res);
#line 1046 "lang/grammar.acc"
 theTemp->SubImm |= res; 
# line 6275 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 895: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy79;
         }
      }
      yy79: ;
      get_lexval();
      } break;
   case 329: {
      get_lexval();
      switch (yyselect()) {
      case 897: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 898: {
            cexpr3(&n);
            get_lexval();
#line 1048 "lang/grammar.acc"
 theTemp->NewFeats[CurrFeat++] = (uint16)n; 
# line 6307 "src/yygram.cpp"
            } break;
         default: goto yy80;
         }
      }
      yy80: ;
      cexpr3(&n);
      get_lexval();
#line 1049 "lang/grammar.acc"
 theTemp->NewFeats[CurrFeat++] = (uint16)n; 
# line 6317 "src/yygram.cpp"
      } break;
   case 330: {
      get_lexval();
      switch (yyselect()) {
      case 899: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 901: {
            res_ref(&rr);
#line 1050 "lang/grammar.acc"
 theTemp->AddsFlag(M_INNATE);
                                       theTemp->AddResID(AN_INNATE, rr); 
# line 6334 "src/yygram.cpp"
            switch (yyselect()) {
            case 900: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy81;
         }
      }
      yy81: ;
      get_lexval();
      } break;
   case 331: {
      get_lexval();
      switch (yyselect()) {
      case 902: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 904: {
            res_ref(&rr);
#line 1053 "lang/grammar.acc"
 theTemp->AddResID(AN_DISEASE, rr); 
# line 6362 "src/yygram.cpp"
            switch (yyselect()) {
            case 903: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy82;
         }
      }
      yy82: ;
      get_lexval();
      } break;
   case 332: {
      get_lexval();
      switch (yyselect()) {
      case 905: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 907: {
            res_ref(&rr);
#line 1055 "lang/grammar.acc"
 theTemp->AddResID(AN_POISON, rr); 
# line 6390 "src/yygram.cpp"
            switch (yyselect()) {
            case 906: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy83;
         }
      }
      yy83: ;
      get_lexval();
      } break;
   case 333: {
      get_lexval();
      switch (yyselect()) {
      case 908: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 915: {
            switch (yyselect()) {
            case 909: {
               cexpr3(&fl);
#line 1058 "lang/grammar.acc"
 if (fl < 0) 
                                         theTemp->Flags[(-fl)/8] |= 1 << ((-fl)%8);
                                       else
                                         theTemp->AddFlags[fl/8] |= 1 << (fl % 8); 
                                     
# line 6424 "src/yygram.cpp"
               } break;
            case 912: {
               switch (yyselect()) {
               case 910: {
                  get_lexval();
                  } break;
               case 911: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&fl);
#line 1063 "lang/grammar.acc"
 theTemp->SubFlags[fl/8] |= 1 << (fl % 8); 
# line 6439 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            switch (yyselect()) {
            case 913: {
               get_lexval();
               } break;
            case 914: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            } break;
         default: goto yy84;
         }
      }
      yy84: ;
      } break;
   case 334: {
      gear_desc();
      } break;
   case 335: {
      event_desc();
      } break;
   case 336: {
      dconst_entry();
      } break;
   case 337: {
      r_declaration();
      } break;
   }
return 0;
}

int temp_attack_entry ()
{
   YYSTYPE at;
   Dice dam;
   YYSTYPE chance;
   YYSTYPE n;
   YYSTYPE dc;
   Dice dam2;
   YYSTYPE n2;
   rID rr;
   YYSTYPE dt;
   switch(yyselect()) {
   case 338: {
#line 1071 "lang/grammar.acc"
 uint8 dt = AD_NORM; dam.Set(0,0,0);
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0);
# line 6490 "src/yygram.cpp"
      cexpr3(&at);
      get_lexval();
      switch (yyselect()) {
      case 916: {
         dice_only(&dam);
#line 1074 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam); 
# line 6498 "src/yygram.cpp"
         } break;
      case 917: {
         cexpr3(&chance);
         get_lexval();
#line 1075 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg.Bonus = (int8)chance; 
# line 6505 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 918: {
         cexpr3(&n);
#line 1076 "lang/grammar.acc"
 dt = (uint8)n; 
# line 6514 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 919: {
         get_lexval();
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 923: {
         get_lexval();
         switch (yyselect()) {
         case 920: {
            get_lexval();
            } break;
         case 921: {
            get_lexval();
            } break;
         case 922: {
            get_lexval();
            } break;
         default: printf("???\n"); exit(1);
         }
         cexpr3(&dc);
#line 1079 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = (int8)dc; 
# line 6542 "src/yygram.cpp"
         get_lexval();
         } break;
      default: ;
      }
#line 1080 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = (int8)at;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 /*

                                 if (at == A_ABIL)

                                   theTemp->NewAttk[CurrAttk].u.a.DC = max(

                                        theTemp->NewAttk[CurrAttk].u.a.DC, 1);

                                        */
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   yyerror("> 16 attacks on a single template!");
                               
# line 6563 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 931: {
            get_lexval();
#line 1091 "lang/grammar.acc"
 dt = AD_NORM; theTemp->NewAttk[CurrAttk].u.a.Dmg.Set(0,0,0); dam2.Set(0,0,0); 
# line 6570 "src/yygram.cpp"
            switch (yyselect()) {
            case 924: {
               dice_only(&dam2);
#line 1092 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2); 
# line 6576 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 925: {
               cexpr3(&n2);
#line 1093 "lang/grammar.acc"
 dt = (uint8)n2; 
# line 6585 "src/yygram.cpp"
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 926: {
               get_lexval();
               } break;
            default: ;
            }
            switch (yyselect()) {
            case 930: {
               get_lexval();
               switch (yyselect()) {
               case 927: {
                  get_lexval();
                  } break;
               case 928: {
                  get_lexval();
                  } break;
               case 929: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&dc);
#line 1095 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].u.a.DC = (int8)dc; 
# line 6613 "src/yygram.cpp"
               get_lexval();
               } break;
            default: ;
            }
#line 1096 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = A_ALSO;
                                 theTemp->NewAttk[CurrAttk].DType = dt;
                                 theTemp->NewAttk[CurrAttk].u.a.Dmg = yyDice(dam2);
                                 CurrAttk++;
                                 if (CurrAttk >= 16)
                                   Fatal("> 16 attacks on a single monster!");
                               
# line 6626 "src/yygram.cpp"
            } break;
         default: goto yy85;
         }
      }
      yy85: ;
      } break;
   case 339: {
      cexpr3(&at);
      get_lexval();
      res_ref(&rr);
#line 1104 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].AType = (int8)at;
                                  theTemp->NewAttk[CurrAttk].u.xID = rr; 
# line 6640 "src/yygram.cpp"
      switch (yyselect()) {
      case 932: {
         cexpr3(&dt);
#line 1106 "lang/grammar.acc"
 theTemp->NewAttk[CurrAttk].DType = (int8)dt; 
# line 6646 "src/yygram.cpp"
         } break;
      default: ;
      }
      } break;
   }
return 0;
}

int quest_def ()
{
   switch(yyselect()) {
   case 340: {
      get_lexval();
      } break;
   }
return 0;
}

int flavor_def ()
{
   YYSTYPE name;
   YYSTYPE it;
   switch(yyselect()) {
   case 341: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&it);
#line 1113 "lang/grammar.acc"
 theRes = theFlavor; 
                                         theFlavor->Name = name;
                                         theFlavor->IType = (int8)it;
                                         theFlavor->Material = -1;
                                         theFlavor->Color = -1;
                                         theFlavor->Weight = 20; 
# line 6683 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 933: {
            flavor_entry();
            } break;
         default: goto yy86;
         }
      }
      yy86: ;
      get_lexval();
#line 1119 "lang/grammar.acc"
 theFlavor++; 
# line 6697 "src/yygram.cpp"
      } break;
   }
return 0;
}

int flavor_entry ()
{
   YYSTYPE lv;
   YYSTYPE n;
   YYSTYPE col;
   switch(yyselect()) {
   case 342: {
      get_lexval();
      switch (yyselect()) {
      case 934: {
         get_lexval();
         } break;
      default: ;
      }
      get_lexval();
      lv = yylval;
      get_lexval();
#line 1123 "lang/grammar.acc"
 theFlavor->Desc     = lv; 
# line 6722 "src/yygram.cpp"
      } break;
   case 343: {
      get_lexval();
      switch (yyselect()) {
      case 935: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
#line 1124 "lang/grammar.acc"
 theFlavor->Material = (int8)n; 
# line 6736 "src/yygram.cpp"
      } break;
   case 344: {
      get_lexval();
      switch (yyselect()) {
      case 936: {
         get_lexval();
         } break;
      default: ;
      }
      color(&col);
      get_lexval();
#line 1125 "lang/grammar.acc"
 theFlavor->Color    = (int8)col; 
# line 6750 "src/yygram.cpp"
      } break;
   case 345: {
      get_lexval();
      switch (yyselect()) {
      case 937: {
         get_lexval();
         } break;
      default: ;
      }
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1126 "lang/grammar.acc"
 theFlavor->Weight   = (int8)(n/5); 
# line 6765 "src/yygram.cpp"
      } break;
   }
return 0;
}

int encounter_def ()
{
   YYSTYPE name;
   YYSTYPE n;
   switch(yyselect()) {
   case 346: {
      get_lexval();
#line 1131 "lang/grammar.acc"
 theRes = theEnc; 
                     theEnc->Weight = 10; 
# line 6781 "src/yygram.cpp"
      get_lexval();
      name = yylval;
#line 1133 "lang/grammar.acc"
 theEnc->Name = name; 
# line 6786 "src/yygram.cpp"
      switch (yyselect()) {
      case 938: {
         get_lexval();
         cexpr3(&n);
#line 1134 "lang/grammar.acc"
 theEnc->Terrain |= n; 
# line 6793 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 939: {
            encounter_entry();
            } break;
         default: goto yy87;
         }
      }
      yy87: ;
      get_lexval();
#line 1137 "lang/grammar.acc"
 theEnc++; 
# line 6810 "src/yygram.cpp"
      } break;
   }
return 0;
}

int encounter_entry ()
{
   YYSTYPE n;
   YYSTYPE n2;
   switch(yyselect()) {
   case 347: {
      get_lexval();
      get_lexval();
      cexpr(&n);
      get_lexval();
#line 1140 "lang/grammar.acc"
 theEnc->Terrain |= n; 
# line 6828 "src/yygram.cpp"
      } break;
   case 348: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1141 "lang/grammar.acc"
 theEnc->Depth = (int16)n; 
# line 6837 "src/yygram.cpp"
      } break;
   case 349: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1142 "lang/grammar.acc"
 theEnc->Weight = (int16)n; 
# line 6846 "src/yygram.cpp"
      } break;
   case 350: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1143 "lang/grammar.acc"
 theEnc->minCR = theEnc->maxCR = (int16)n; 
# line 6855 "src/yygram.cpp"
      } break;
   case 351: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
      get_lexval();
#line 1144 "lang/grammar.acc"
 theEnc->minCR = (int16)n; theEnc->maxCR = 36; 
# line 6865 "src/yygram.cpp"
      } break;
   case 352: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      switch (yyselect()) {
      case 940: {
         get_lexval();
         } break;
      case 941: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      cexpr3(&n2);
      get_lexval();
#line 1146 "lang/grammar.acc"
 theEnc->minCR = (int16)n; theEnc->maxCR = (int16)n2; 
# line 6884 "src/yygram.cpp"
      } break;
   case 353: {
      get_lexval();
      get_lexval();
      cexpr3(&n);
      get_lexval();
#line 1147 "lang/grammar.acc"
 theEnc->Align = (int16)n; 
# line 6893 "src/yygram.cpp"
      } break;
   case 354: {
      get_lexval();
      get_lexval();
#line 1148 "lang/grammar.acc"
 currPart = 0; 
# line 6900 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 942: {
            encounter_part();
            } break;
         default: goto yy88;
         }
      }
      yy88: ;
      get_lexval();
      } break;
   case 355: {
      get_lexval();
      switch (yyselect()) {
      case 943: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 946: {
            cexpr3(&n);
#line 1151 "lang/grammar.acc"
 theEnc->SetFlag((uint16)n); 
# line 6926 "src/yygram.cpp"
            switch (yyselect()) {
            case 944: {
               get_lexval();
               } break;
            case 945: {
               get_lexval();
               } break;
            default: ;
            }
            } break;
         default: goto yy89;
         }
      }
      yy89: ;
      get_lexval();
      } break;
   case 356: {
      gear_desc();
      } break;
   case 357: {
      event_desc();
      } break;
   case 358: {
      dconst_entry();
      } break;
   case 359: {
      r_declaration();
      } break;
   }
return 0;
}

int encounter_part ()
{
   YYSTYPE n;
   hCode hc;
   YYSTYPE n2;
   Dice d;
   rID rr;
   switch(yyselect()) {
   case 360: {
#line 1160 "lang/grammar.acc"
 ASSERT(currPart < MAX_PARTS); 
# line 6970 "src/yygram.cpp"
      switch (yyselect()) {
      case 947: {
         get_lexval();
         } break;
      case 948: {
         get_lexval();
#line 1162 "lang/grammar.acc"
 theEnc->Parts[currPart].Flags |= EP_ELSE; 
# line 6979 "src/yygram.cpp"
         } break;
      case 949: {
         get_lexval();
#line 1163 "lang/grammar.acc"
 theEnc->Parts[currPart].Flags |= EP_OR; 
# line 6985 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
#line 1165 "lang/grammar.acc"
 theEnc->Parts[currPart].Weight = 10; 
# line 6991 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 950: {
            cexpr4(&n);
            get_lexval();
#line 1167 "lang/grammar.acc"
 theEnc->Parts[currPart].Chance = (uint8)n; 
# line 6999 "src/yygram.cpp"
            } break;
         case 951: {
            get_lexval();
            get_lexval();
            cexpr4(&n);
            get_lexval();
            get_lexval();
#line 1168 "lang/grammar.acc"
 theEnc->Parts[currPart].minCR = (uint8)n; 
# line 7009 "src/yygram.cpp"
            } break;
         case 952: {
            get_lexval();
            cexpr4(&n);
            get_lexval();
#line 1169 "lang/grammar.acc"
 theEnc->Parts[currPart].Weight = (uint8)n; 
# line 7017 "src/yygram.cpp"
            } break;
         case 953: {
            get_lexval();
            sep_expr(&hc);
            get_lexval();
#line 1170 "lang/grammar.acc"
 theEnc->Parts[currPart].Condition = hc; 
# line 7025 "src/yygram.cpp"
            } break;
         case 956: {
            cexpr4(&n);
            switch (yyselect()) {
            case 954: {
               get_lexval();
               } break;
            case 955: {
               get_lexval();
               } break;
            default: printf("???\n"); exit(1);
            }
            cexpr4(&n2);
#line 1171 "lang/grammar.acc"
 theEnc->Parts[currPart].Amt.Number = (int8)n;
                                      theEnc->Parts[currPart].Amt.Bonus  = (int8)n2;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 7043 "src/yygram.cpp"
            } break;
         case 957: {
            cexpr4(&n);
#line 1174 "lang/grammar.acc"
 theEnc->Parts[currPart].Amt.Number = (int8)n;
                                      theEnc->Parts[currPart].Amt.Bonus  = (int8)n;
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 7051 "src/yygram.cpp"
            } break;
         case 958: {
            cexpr4(&n);
            get_lexval();
#line 1177 "lang/grammar.acc"
 theEnc->Parts[currPart].Amt.Number = (int8)n;
                                      theEnc->Parts[currPart].Amt.Bonus  = (int8)(n+50);
                                      theEnc->Parts[currPart].Amt.Sides  = 0; 
# line 7060 "src/yygram.cpp"
            } break;
         case 959: {
            dice_only(&d);
#line 1180 "lang/grammar.acc"
 theEnc->Parts[currPart].Amt        = d; 
# line 7066 "src/yygram.cpp"
            } break;
         default: goto yy90;
         }
      }
      yy90: ;
      get_lexval();
      switch (yyselect()) {
      case 960: {
         res_ref(&rr);
#line 1186 "lang/grammar.acc"
 theEnc->Parts[currPart].xID = rr; 
# line 7078 "src/yygram.cpp"
         } break;
      case 961: {
         cexpr3(&n);
#line 1187 "lang/grammar.acc"
 theEnc->Parts[currPart].xID = n; 
# line 7084 "src/yygram.cpp"
         } break;
      case 962: {
         get_lexval();
         cexpr3(&n);
#line 1188 "lang/grammar.acc"
 theEnc->Parts[currPart].xID = n;
                                      theEnc->Parts[currPart].Flags |= EP_ANYMON; 
# line 7092 "src/yygram.cpp"
         } break;
      default: printf("???\n"); exit(1);
      }
      switch (yyselect()) {
      case 963: {
         res_ref(&rr);
#line 1192 "lang/grammar.acc"
 theEnc->Parts[currPart].xID2 = rr; 
# line 7101 "src/yygram.cpp"
         } break;
      case 964: {
         cexpr3(&n);
#line 1193 "lang/grammar.acc"
 theEnc->Parts[currPart].xID2 = n; 
# line 7107 "src/yygram.cpp"
         } break;
      case 965: {
         get_lexval();
         cexpr3(&n);
#line 1194 "lang/grammar.acc"
 theEnc->Parts[currPart].xID = n;
                                      theEnc->Parts[currPart].Flags |= EP_ANYTEMP; 
# line 7115 "src/yygram.cpp"
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 969: {
         get_lexval();
         cexpr3(&n);
#line 1198 "lang/grammar.acc"
 theEnc->Parts[currPart].Flags |= n; 
# line 7125 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 968: {
               switch (yyselect()) {
               case 966: {
                  get_lexval();
                  } break;
               case 967: {
                  get_lexval();
                  } break;
               default: printf("???\n"); exit(1);
               }
               cexpr3(&n);
#line 1199 "lang/grammar.acc"
 theEnc->Parts[currPart].Flags |= n; 
# line 7141 "src/yygram.cpp"
               } break;
            default: goto yy91;
            }
         }
         yy91: ;
         get_lexval();
         } break;
      default: ;
      }
#line 1202 "lang/grammar.acc"
 currPart++; 
# line 7153 "src/yygram.cpp"
      } break;
   }
return 0;
}

int behaviour_def ()
{
   YYSTYPE name;
   YYSTYPE c;
   switch(yyselect()) {
   case 361: {
      get_lexval();
      get_lexval();
      name = yylval;
      get_lexval();
      cexpr3(&c);
#line 1206 "lang/grammar.acc"
 theRes = theBev; 
                                           theBev->Name = name;
                                           theBev->Conditions = c; 
# line 7174 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 970: {
            behaviour_entry();
            } break;
         default: goto yy92;
         }
      }
      yy92: ;
      get_lexval();
#line 1209 "lang/grammar.acc"
 theBev++; 
# line 7188 "src/yygram.cpp"
      } break;
   }
return 0;
}

int behaviour_entry ()
{
   rID rr;
   YYSTYPE n;
   switch(yyselect()) {
   case 362: {
      get_lexval();
      switch (yyselect()) {
      case 971: {
         get_lexval();
         } break;
      default: ;
      }
      res_ref(&rr);
      get_lexval();
#line 1213 "lang/grammar.acc"
 theBev->spID = rr; 
# line 7211 "src/yygram.cpp"
      } break;
   case 363: {
      get_lexval();
      switch (yyselect()) {
      case 972: {
         get_lexval();
         } break;
      default: ;
      }
      while (1) {
         switch (yyselect()) {
         case 975: {
            cexpr3(&n);
            switch (yyselect()) {
            case 973: {
               get_lexval();
               } break;
            case 974: {
               get_lexval();
               } break;
            default: ;
            }
#line 1214 "lang/grammar.acc"
 theBev->SetFlag((uint16)n); 
# line 7236 "src/yygram.cpp"
            } break;
         default: goto yy93;
         }
      }
      yy93: ;
      get_lexval();
      } break;
   case 364: {
      event_desc();
      } break;
   case 365: {
      dconst_entry();
      } break;
   case 366: {
      r_declaration();
      } break;
   }
return 0;
}

int text_def ()
{
   YYSTYPE name;
   YYSTYPE text;
   rID rr;
   hCode hc;
   switch(yyselect()) {
   case 367: {
      get_lexval();
      get_lexval();
      name = yylval;
#line 1221 "lang/grammar.acc"
 theRes = theText;
                         theText->Name = name;
                         cText = 0; 
# line 7272 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 976: {
            get_lexval();
            text = yylval;
#line 1224 "lang/grammar.acc"
 TempList[cText++] = text; 
# line 7280 "src/yygram.cpp"
            } break;
         case 977: {
            res_ref(&rr);
#line 1225 "lang/grammar.acc"
 TempList[cText++] = rr; 
# line 7286 "src/yygram.cpp"
            } break;
         case 978: {
            get_lexval();
#line 1226 "lang/grammar.acc"
 TempList[cText++] = TC_LPAREN; 
# line 7292 "src/yygram.cpp"
            } break;
         case 979: {
            get_lexval();
#line 1227 "lang/grammar.acc"
 TempList[cText++] = TC_RPAREN; 
# line 7298 "src/yygram.cpp"
            } break;
         case 980: {
            get_lexval();
#line 1228 "lang/grammar.acc"
 TempList[cText++] = TC_CHOICE; 
# line 7304 "src/yygram.cpp"
            } break;
         case 981: {
            get_lexval();
#line 1229 "lang/grammar.acc"
 TempList[cText++] = TC_TERM; 
# line 7310 "src/yygram.cpp"
            } break;
         case 982: {
            get_lexval();
            sep_expr(&hc);
            get_lexval();
            get_lexval();
#line 1231 "lang/grammar.acc"
 TempList[cText++] = TC_CASE;
                      TempList[cText++] = hc; 
# line 7320 "src/yygram.cpp"
            } break;
         case 983: {
            get_lexval();
            sep_stat(&hc);
            get_lexval();
#line 1234 "lang/grammar.acc"
 TempList[cText++] = TC_ACTION;
                      TempList[cText++] = hc; 
# line 7329 "src/yygram.cpp"
            } break;
         default: goto yy94;
         }
      }
      yy94: ;
      get_lexval();
#line 1236 "lang/grammar.acc"
 TempList[cText] = 0;
             theText->AddList(TEXT_LIST,TempList);
             theText++; 
# line 7340 "src/yygram.cpp"
      } break;
   }
return 0;
}

int prototype ()
{
   switch(yyselect()) {
   case 368: {
      get_lexval();
      } break;
   }
return 0;
}

int type_name (YYSTYPE *ty)
{
   switch(yyselect()) {
   case 369: {
      get_lexval();
#line 1244 "lang/grammar.acc"
 *ty = DT_VOID; 
# line 7363 "src/yygram.cpp"
      } break;
   case 370: {
      get_lexval();
#line 1245 "lang/grammar.acc"
 *ty = DT_BOOL; 
# line 7369 "src/yygram.cpp"
      } break;
   case 371: {
      get_lexval();
#line 1246 "lang/grammar.acc"
 *ty = DT_UINT8; 
# line 7375 "src/yygram.cpp"
      } break;
   case 372: {
      get_lexval();
#line 1247 "lang/grammar.acc"
 *ty = DT_INT8; 
# line 7381 "src/yygram.cpp"
      } break;
   case 373: {
      get_lexval();
#line 1248 "lang/grammar.acc"
 *ty = DT_INT16; 
# line 7387 "src/yygram.cpp"
      } break;
   case 374: {
      get_lexval();
#line 1249 "lang/grammar.acc"
 *ty = DT_INT32; 
# line 7393 "src/yygram.cpp"
      } break;
   case 375: {
      get_lexval();
#line 1250 "lang/grammar.acc"
 *ty = DT_HOBJ; 
# line 7399 "src/yygram.cpp"
      } break;
   case 376: {
      get_lexval();
#line 1251 "lang/grammar.acc"
 *ty = DT_HTEXT; 
# line 7405 "src/yygram.cpp"
      } break;
   case 377: {
      get_lexval();
#line 1252 "lang/grammar.acc"
 *ty = DT_RID; 
# line 7411 "src/yygram.cpp"
      } break;
   case 378: {
      get_lexval();
#line 1253 "lang/grammar.acc"
 *ty = DT_STRING; 
# line 7417 "src/yygram.cpp"
      } break;
   case 379: {
      get_lexval();
#line 1254 "lang/grammar.acc"
 *ty = DT_RECT; 
# line 7423 "src/yygram.cpp"
      } break;
   }
return 0;
}

int s_prototype ()
{
   YYSTYPE ty;
   YYSTYPE ot;
   YYSTYPE fn;
   YYSTYPE ty_1;
   YYSTYPE ot_1;
   YYSTYPE paramname;
   YYSTYPE def;
   YYSTYPE ty_n;
   YYSTYPE ot_n;
   switch(yyselect()) {
   case 380: {
#line 1258 "lang/grammar.acc"
 BSysFunc *b; 
# line 7444 "src/yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1260 "lang/grammar.acc"
 if (is_res(ot)) {
        if (theSymTab.GetBinding(fn,0,0,RES_FUNC))
          if (GETBIND_RESFUNC(fn)->RType == ot)
            yyerror(Format("Resource member function '%s' declared for same resource type twice!",theSymTab[fn]));
        b = new BResFunc((int16)MemFuncID++,(int8)ty,(int8)ot);
        theSymTab.Bind(fn,b);
        }
      else {
        if (theSymTab.GetBinding(fn,0,0,MEM_FUNC))
          yyerror(Format("System member function '%s' declared twice!",theSymTab[fn]));
        /*else if (theSymTab.GetBinding(fn,1,0,ANY))

          yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));*/
        b = new BMemFunc((int16)MemFuncID++,(int8)ty,(int8)ot);
        theSymTab.Bind(fn,b); 
      }
    
# line 7470 "src/yygram.cpp"
      switch (yyselect()) {
      case 991: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 984: {
            get_lexval();
            cexpr3(&ot_1);
#line 1276 "lang/grammar.acc"
 b->ParamOType[0] = (int8)ot_1; 
# line 7480 "src/yygram.cpp"
            } break;
         default: ;
         }
#line 1277 "lang/grammar.acc"
 b->ParamType[0] = (int8)ty_1; b->ParamCount++; 
# line 7486 "src/yygram.cpp"
         switch (yyselect()) {
         case 985: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 986: {
            get_lexval();
            cexpr(&def);
#line 1278 "lang/grammar.acc"
 b->Default[0] = (int8)def; b->HasDefault |= BIT(2); 
# line 7500 "src/yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 990: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 987: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1280 "lang/grammar.acc"
 b->ParamOType[b->ParamCount] = (int8)ot_n; 
# line 7515 "src/yygram.cpp"
                  } break;
               default: ;
               }
#line 1281 "lang/grammar.acc"
 b->ParamType[b->ParamCount] = (int8)ty_n; b->ParamCount++; 
# line 7521 "src/yygram.cpp"
               switch (yyselect()) {
               case 988: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 989: {
                  get_lexval();
                  cexpr(&def);
#line 1282 "lang/grammar.acc"
 b->Default[b->ParamCount] = (int8)def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 7535 "src/yygram.cpp"
                  } break;
               default: ;
               }
               } break;
            default: goto yy95;
            }
         }
         yy95: ;
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 992: {
         get_lexval();
         get_lexval();
#line 1284 "lang/grammar.acc"
 b->isVarParam = true; 
# line 7553 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      } break;
   }
return 0;
}

int sg_prototype ()
{
   YYSTYPE ty;
   YYSTYPE fn;
   YYSTYPE ty_1;
   YYSTYPE ot_1;
   YYSTYPE paramname;
   YYSTYPE def;
   YYSTYPE ty_n;
   YYSTYPE ot_n;
   switch(yyselect()) {
   case 381: {
#line 1287 "lang/grammar.acc"
 BSysFunc *b; 
# line 7578 "src/yygram.cpp"
      get_lexval();
      type_name(&ty);
      get_lexval();
      fn = yylval;
      get_lexval();
#line 1289 "lang/grammar.acc"
 if (theSymTab.GetBinding(fn,0,0,SYS_FUNC))
        yyerror(Format("System global function '%s' declared twice!",theSymTab[fn]));
      else if (theSymTab.GetBinding(fn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[fn]));
      b = new BSysFunc((int16)MemFuncID++,(int8)ty);
      theSymTab.Bind(fn,b); 
# line 7591 "src/yygram.cpp"
      switch (yyselect()) {
      case 1000: {
         type_name(&ty_1);
         switch (yyselect()) {
         case 993: {
            get_lexval();
            cexpr3(&ot_1);
#line 1295 "lang/grammar.acc"
 b->ParamOType[0] = (int8)ot_1; 
# line 7601 "src/yygram.cpp"
            } break;
         default: ;
         }
#line 1296 "lang/grammar.acc"
 b->ParamType[0] = (int8)ty_1; b->ParamCount++; 
# line 7607 "src/yygram.cpp"
         switch (yyselect()) {
         case 994: {
            get_lexval();
            paramname = yylval;
            } break;
         default: ;
         }
         switch (yyselect()) {
         case 995: {
            get_lexval();
            cexpr(&def);
#line 1297 "lang/grammar.acc"
 b->Default[0] = (int8)def; b->HasDefault |= BIT(2); 
# line 7621 "src/yygram.cpp"
            } break;
         default: ;
         }
         while (1) {
            switch (yyselect()) {
            case 999: {
               get_lexval();
               type_name(&ty_n);
               switch (yyselect()) {
               case 996: {
                  get_lexval();
                  cexpr3(&ot_n);
#line 1299 "lang/grammar.acc"
 b->ParamOType[b->ParamCount] = (int8)ot_n; 
# line 7636 "src/yygram.cpp"
                  } break;
               default: ;
               }
#line 1300 "lang/grammar.acc"
 b->ParamType[b->ParamCount] = (int8)ty_n; b->ParamCount++; 
# line 7642 "src/yygram.cpp"
               switch (yyselect()) {
               case 997: {
                  get_lexval();
                  paramname = yylval;
                  } break;
               default: ;
               }
               switch (yyselect()) {
               case 998: {
                  get_lexval();
                  cexpr(&def);
#line 1301 "lang/grammar.acc"
 b->Default[b->ParamCount] = (int8)def; b->HasDefault |= BIT(1+b->ParamCount); 
# line 7656 "src/yygram.cpp"
                  } break;
               default: ;
               }
               } break;
            default: goto yy96;
            }
         }
         yy96: ;
         } break;
      default: ;
      }
      switch (yyselect()) {
      case 1001: {
         get_lexval();
         get_lexval();
#line 1303 "lang/grammar.acc"
 b->isVarParam = true; 
# line 7674 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
      get_lexval();
      } break;
   }
return 0;
}

int s_declaration ()
{
   YYSTYPE ty;
   YYSTYPE ot;
   YYSTYPE vn;
   switch(yyselect()) {
   case 382: {
#line 1306 "lang/grammar.acc"
 BMemVar *b; BResMem *b2; 
# line 7694 "src/yygram.cpp"
      get_lexval();
      type_name(&ty);
      cexpr3(&ot);
      get_lexval();
      get_lexval();
      vn = yylval;
      get_lexval();
#line 1308 "lang/grammar.acc"
 
      if (is_res(ot)) {
        if (theSymTab.GetBinding(vn,0,RES_MEM))
          if (((BResMem*)theSymTab.GetBinding(vn,0,0,RES_MEM))->RType == ot)
            yyerror(Format("System resource member '%s' declared twice!",theSymTab[vn]));
        b2 = new BResMem();
        b2->varid = (int16)MemVarID++;
        b2->VarType = (int8)ty;
        b2->RType = (int8)ot;
        b2->xID   = 0;
        b2->Event = 1;
        b2->type  = RES_MEM;
        theSymTab.Bind(vn,b2);
        }
      else {
        if (theSymTab.GetBinding(vn,0,0,MEM_VAR))
          yyerror(Format("System member variable '%s' declared twice!",theSymTab[vn]));
        else if (theSymTab.GetBinding(vn,0,1,ANY))
          yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
        b = new BMemVar();
        b->varid = (int16)MemVarID++; 
        b->VarType = (int8)ty; 
        b->OType = (int8)ot;
        b->xID   = 0;
        b->Event = 1;
        b->type  = MEM_VAR;
        theSymTab.Bind(vn,b); 
        }
    
# line 7732 "src/yygram.cpp"
      } break;
   }
return 0;
}

int s_object_decl ()
{
   YYSTYPE ot;
   YYSTYPE oname;
   YYSTYPE oid;
   switch(yyselect()) {
   case 383: {
#line 1339 "lang/grammar.acc"
 BSysObj *b; 
# line 7747 "src/yygram.cpp"
      get_lexval();
      cexpr3(&ot);
      get_lexval();
      oname = yylval;
      get_lexval();
      cexpr3(&oid);
      get_lexval();
#line 1341 "lang/grammar.acc"
 if (theSymTab.GetBinding(oname,0,0,SYS_OBJ))
        yyerror(Format("System object '%s' declared twice!",theSymTab[oname]));
      else if (theSymTab.GetBinding(oname,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[oname]));
      b = new BSysObj();
      b->ObjNum = (int16)oid;
      b->OType  = (int8)ot;
      b->xID   = 0;
      b->Event = 0;
      b->type  = SYS_OBJ;
      theSymTab.Bind(oname,b); 
# line 7767 "src/yygram.cpp"
      } break;
   }
return 0;
}

int g_declaration ()
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 384: {
#line 1355 "lang/grammar.acc"
 BGlobalVar *b; 
# line 7781 "src/yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1357 "lang/grammar.acc"
 if (ty == DT_STRING)
        Error("Global variables of type String cannot be created!");
      if (theSymTab.GetBinding(vn,0,0,GLOB_VAR))
        Error("Global variable '%s' declared twice!",theSymTab[vn]);
      else if (theSymTab.GetBinding(vn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
      b = new BGlobalVar();
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->xID     = 0;
      b->Event   = 0;
      
      b->type    = GLOB_VAR;
      theSymTab.Bind(vn,b); 
# line 7800 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1002: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1372 "lang/grammar.acc"
 if (theSymTab.GetBinding(vn,0,0,GLOB_VAR))
        Error("Global variable '%s' declared twice!",theSymTab[vn]);
      else if (theSymTab.GetBinding(vn,0,1,ANY))
        yyerror(Format("Identifier conflict: '%s'.", theSymTab[vn]));
      b = new BGlobalVar();
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->xID     = 0;
      b->Event   = 0;
      b->type    = GLOB_VAR;
      theSymTab.Bind(vn,b); 
# line 7819 "src/yygram.cpp"
            } break;
         default: goto yy97;
         }
      }
      yy97: ;
      get_lexval();
      } break;
   }
return 0;
}

int l_declaration (VBlock *code)
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 385: {
#line 1385 "lang/grammar.acc"
 BLocalVar *b; int16 i; String scope_id; 
# line 7839 "src/yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1387 "lang/grammar.acc"
 b = new BLocalVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      ASSERT(theEvent);
      b->Event = (int16)theEvent;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,LOC_VAR))
        Error("Local variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->StackOffset = (int16)++StackHead;
      b->type    = LOC_VAR;
      if (ty == DT_STRING) {
        i = AllocString();
        LockString(i);
        code->Generate(MOV,RT_MEMORY,-(b->StackOffset),RT_CONSTANT,-i);
        code->Generate(ESTR,RT_CONSTANT,-i);
        }
      theSymTab.Bind(vn,b); 
# line 7860 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1003: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1404 "lang/grammar.acc"
 b = new BLocalVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      ASSERT(theEvent);
      b->Event = (int16)theEvent;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,LOC_VAR))
        Error("Local variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->StackOffset = (int16)++StackHead;
      b->type    = LOC_VAR;
      if (ty == DT_STRING) {
        i = AllocString();
        LockString(i);
        code->Generate(MOV,RT_MEMORY,-(b->StackOffset),RT_CONSTANT,-i);
        code->Generate(ESTR,RT_CONSTANT,-i);
        }
      theSymTab.Bind(vn,b); 
# line 7884 "src/yygram.cpp"
            } break;
         default: goto yy98;
         }
      }
      yy98: ;
      get_lexval();
      } break;
   }
return 0;
}

int r_declaration ()
{
   YYSTYPE ty;
   YYSTYPE vn;
   switch(yyselect()) {
   case 386: {
#line 1422 "lang/grammar.acc"
 BResVar *b; String scope_id; 
# line 7904 "src/yygram.cpp"
      type_name(&ty);
      get_lexval();
      vn = yylval;
#line 1424 "lang/grammar.acc"
 if (ty == DT_STRING)
        yyerror("Resource variables of type String cannot be allocated.");
      b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7920 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1004: {
            get_lexval();
            get_lexval();
            vn = yylval;
#line 1436 "lang/grammar.acc"
 b = new BResVar();
      b->xID   = FIND(theModule->GetText(theRes->Name));
      b->Event = 0;
      if (theSymTab.GetBinding(vn,b->xID,b->Event,RES_VAR))
        Error("Resource variable '%s' declared twice!",theSymTab[vn]);
      b->VarType = (int8)ty; 
      b->Address = HeapHead++;
      b->type    = RES_VAR;
      theSymTab.Bind(vn,b); 
# line 7937 "src/yygram.cpp"
            } break;
         default: goto yy99;
         }
      }
      yy99: ;
      get_lexval();
      } break;
   }
return 0;
}

int statement (VBlock *st)
{
   PExp ex;
   VBlock st1;
   VBlock st2;
   PExp ex1;
   PExp ex2;
   PExp ex3;
   YYSTYPE val;
   rID rr;
   VBlock st_n;
   VBlock bl;
   switch(yyselect()) {
   case 387: {
      expr(&ex);
      get_lexval();
#line 1450 "lang/grammar.acc"

      st->Add(ex.Code);
      if (ex.Storage == RT_REGISTER) {
        st->TrimGratuitousMOV();
        FreeRegister((int16)ex.Value);
        }
      if (ex.Type == DT_STRING && ex.Storage == RT_CONSTANT)
        if (ex.Value < -1)
          FreeString((int16)-ex.Value);
    
# line 7976 "src/yygram.cpp"
      } break;
   case 388: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
      get_lexval();
      statement(&st2);
#line 1461 "lang/grammar.acc"

      st->Add(ex.Code);
      /* Here, we compute the address to include the size

         of the JUMP opcode below, which may be one or two

         words, depending upon if it needs data space. */
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+ (st2.GetSize()+1> 511?
          3 : 2));
      if (ex.Storage == RT_REGISTER)
        FreeRegister((int16)ex.Value);                  
      st->Add(&st1);                            
      st->Generate(JUMP, RT_CONSTANT, st2.GetSize()+1);
      st->Add(&st2);
    
# line 8003 "src/yygram.cpp"
      } break;
   case 389: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1476 "lang/grammar.acc"

      st->Add(ex.Code);
      st->Generate(JFAL, ex.Storage, ex.Value, 
        RT_CONSTANT, st1.GetSize()+1);
      if (ex.Storage == RT_REGISTER)
        FreeRegister((int16)ex.Value);
      st->Add(&st1);
    
# line 8020 "src/yygram.cpp"
      } break;
   case 390: {
      get_lexval();
      get_lexval();
      expr(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr(&ex3);
      get_lexval();
      statement(&st1);
#line 1486 "lang/grammar.acc"

      int16 jumpback;
      ex1.Code->TrimGratuitousMOV();
      ex3.Code->TrimGratuitousMOV();
      st->Add(ex1.Code);
      if (ex1.Storage == RT_REGISTER)
        FreeRegister((int16)ex1.Value);
      jumpback = (int16)st->GetSize();
      st->Add(ex2.Code);
      /* Here again, we compensate for the fact that the lower JUMP

         opcode may be either one or two words; there has to be a

         less kludgy way to do this... */
      st->Generate(JFAL, ex2.Storage,ex2.Value,
        RT_CONSTANT,st1.GetSize() + (ex3.Code ? ex3.Code->GetSize() : 0) + 
        (st1.GetSize() + (ex3.Code ? ex3.Code->GetSize() : 0) + 
                         (ex2.Code ? ex2.Code->GetSize() : 0) > 511 ? 3 : 2));
      if (ex2.Storage == RT_REGISTER)
        FreeRegister((int16)ex2.Value);
      st1.ProcessContinues();
      st->Add(&st1);
      st->Add(ex3.Code);
      if (ex3.Storage == RT_REGISTER)
        FreeRegister((int16)ex3.Value);
      st->Generate(JUMP,RT_CONSTANT,jumpback - st->GetSize());
    
# line 8060 "src/yygram.cpp"
      } break;
   case 391: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1512 "lang/grammar.acc"

        int16 CaseAddr[256], DefaultAddr;
        int32 CaseVal[256];
        VBlock *CaseCode[256], *DefaultCode;
        int8 CaseCount, i;
        CaseCount = 0;
        DefaultAddr = 0;
        CaseAddr[0] = 0; 
      
# line 8078 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1008: {
            get_lexval();
            switch (yyselect()) {
            case 1005: {
               cexpr(&val);
               } break;
            case 1006: {
               res_ref(&rr);
#line 1521 "lang/grammar.acc"
 val = rr; 
# line 8091 "src/yygram.cpp"
               } break;
            default: printf("???\n"); exit(1);
            }
            get_lexval();
#line 1522 "lang/grammar.acc"
 CaseCode[CaseCount] = new VBlock; 
# line 8098 "src/yygram.cpp"
            while (1) {
               switch (yyselect()) {
               case 1007: {
#line 1523 "lang/grammar.acc"
 st_n.Clear(); 
# line 8104 "src/yygram.cpp"
                  statement(&st_n);
#line 1523 "lang/grammar.acc"
 CaseCode[CaseCount]->Add(&st_n);  
# line 8108 "src/yygram.cpp"
                  } break;
               default: goto yy101;
               }
            }
            yy101: ;
#line 1524 "lang/grammar.acc"

        /* We know where *this* case ends, which is synonimous with where

           the *next* case starts... */
        CaseAddr[CaseCount + 1] = CaseAddr[CaseCount] + (int16)CaseCode[CaseCount]->GetSize();
        CaseVal[CaseCount] = val;
        CaseCount++;  
      
# line 8123 "src/yygram.cpp"
            } break;
         default: goto yy100;
         }
      }
      yy100: ;
      switch (yyselect()) {
      case 1010: {
         get_lexval();
         get_lexval();
#line 1532 "lang/grammar.acc"
 DefaultCode = new VBlock; 
# line 8135 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 1009: {
#line 1533 "lang/grammar.acc"
 st_n.Clear(); 
# line 8141 "src/yygram.cpp"
               statement(&st_n);
#line 1533 "lang/grammar.acc"
 DefaultCode->Add(&st_n);  
# line 8145 "src/yygram.cpp"
               } break;
            default: goto yy102;
            }
         }
         yy102: ;
#line 1534 "lang/grammar.acc"
 
        DefaultAddr = CaseAddr[CaseCount]; 
        CaseAddr[CaseCount] = DefaultAddr + (int16)DefaultCode->GetSize(); 
      
# line 8156 "src/yygram.cpp"
         } break;
      default: ;
      }
      get_lexval();
#line 1541 "lang/grammar.acc"

        /* Now we code the *entire* switch statement! */
        
        /* Evaluate the Expression */
        st->Add(ex.Code);

        /* Set the break location */
        st->Generate(SBRK, RT_CONSTANT, CaseAddr[CaseCount]+(CaseCount*2+3));

        /* Jump to the proper case */
        st->Generate(JTAB, ex.Storage, ex.Value, RT_CONSTANT, CaseCount);
        
        /* Write out the Jump Table */
        for(i=0;i!=CaseCount;i++) {
          st->GenDWord(CaseVal[i]);
          st->GenDWord(CaseAddr[i] + (CaseCount*2+1));
          }
        /* And then add the default case */
        st->GenDWord((DefaultAddr ? DefaultAddr : CaseAddr[i]) + (CaseCount*2+1));

        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);

        /* Copy in each case's code, in order */
        for (i=0;i!=CaseCount;i++)
          {
            st->Add(CaseCode[i]);
            delete CaseCode[i];
          } 

        /* Don't forget the default case */
        if (DefaultAddr) {
          st->Add(DefaultCode);
          delete DefaultCode;
          }

        /* Free the break location */
        st->Generate(FBRK);
        
      
# line 8202 "src/yygram.cpp"
      } break;
   case 392: {
      get_lexval();
      get_lexval();
#line 1582 "lang/grammar.acc"

        st->Generate(JBRK);
      
# line 8211 "src/yygram.cpp"
      } break;
   case 393: {
      get_lexval();
      get_lexval();
#line 1586 "lang/grammar.acc"

        st->Generate(CONT);
      
# line 8220 "src/yygram.cpp"
      } break;
   case 394: {
      get_lexval();
      statement(&st1);
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      get_lexval();
#line 1590 "lang/grammar.acc"

        st->Add(&st1);
        st->Add(ex.Code);
        st->Generate(JTRU,ex.Storage,ex.Value,
          RT_CONSTANT, -st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);
      
# line 8239 "src/yygram.cpp"
      } break;
   case 395: {
      get_lexval();
      get_lexval();
      expr(&ex);
      get_lexval();
      statement(&st1);
#line 1599 "lang/grammar.acc"

        st->Add(ex.Code);
        st->Generate(JFAL,ex.Storage,ex.Value,
          RT_CONSTANT, st1.GetSize()+
            (st->GetSize() > 511 ? 3 : 2));
        st->Add(&st1);
        st->Generate(JUMP,RT_CONSTANT, - st->GetSize());
        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);
      
# line 8258 "src/yygram.cpp"
      } break;
   case 396: {
      block(&bl);
#line 1610 "lang/grammar.acc"
 
        st->Add(&bl); 
      
# line 8266 "src/yygram.cpp"
      } break;
   case 397: {
      get_lexval();
      expr(&ex);
      get_lexval();
#line 1614 "lang/grammar.acc"

        st->Add(ex.Code);
        if (StackHead) {
          st->Generate(MOV,RT_REGISTER,0,ex.Storage,ex.Value);
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
          st->Generate(RET,RT_REGISTER,0);
          }
        else
          st->Generate(RET,ex.Storage,ex.Value);
        if (ex.Storage == RT_REGISTER)
          FreeRegister((int16)ex.Value);
      
# line 8285 "src/yygram.cpp"
      } break;
   case 398: {
      get_lexval();
      get_lexval();
#line 1627 "lang/grammar.acc"

        if (StackHead)
          st->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        st->Generate(RET);
      
# line 8296 "src/yygram.cpp"
      } break;
   }
return 0;
}

int block (VBlock *bl)
{
   VBlock st;
   switch(yyselect()) {
   case 399: {
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 1011: {
#line 1635 "lang/grammar.acc"
 st.Clear(); 
# line 8313 "src/yygram.cpp"
            statement(&st);
#line 1635 "lang/grammar.acc"
 bl->Add(&st); 
# line 8317 "src/yygram.cpp"
            } break;
         default: goto yy103;
         }
      }
      yy103: ;
      get_lexval();
      } break;
   }
return 0;
}

int routine (VBlock *bl)
{
   VBlock code;
   VBlock st;
   switch(yyselect()) {
   case 400: {
#line 1639 "lang/grammar.acc"
 VBlock decl_code; StackHead = 0; ClearStrings(); 
# line 8337 "src/yygram.cpp"
      get_lexval();
      while (1) {
         switch (yyselect()) {
         case 1012: {
#line 1640 "lang/grammar.acc"
 code.Clear(); 
# line 8344 "src/yygram.cpp"
            l_declaration(&code);
#line 1640 "lang/grammar.acc"
 decl_code.Add(&code); 
# line 8348 "src/yygram.cpp"
            } break;
         case 1013: {
            get_lexval();
            r_declaration();
            } break;
         default: goto yy104;
         }
      }
      yy104: ;
#line 1642 "lang/grammar.acc"
 if (StackHead)
          bl->Generate(INC,RT_REGISTER,63,RT_CONSTANT,StackHead);
        bl->Add(&decl_code); 
# line 8362 "src/yygram.cpp"
      while (1) {
         switch (yyselect()) {
         case 1014: {
#line 1645 "lang/grammar.acc"
 st.Clear(); 
# line 8368 "src/yygram.cpp"
            statement(&st);
#line 1645 "lang/grammar.acc"
 bl->Add(&st); 
# line 8372 "src/yygram.cpp"
            } break;
         default: goto yy105;
         }
      }
      yy105: ;
      get_lexval();
#line 1646 "lang/grammar.acc"
 if (StackHead)
          bl->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,StackHead); 
      
        /* This code is being inserted by FJM May 7 2006 to suppress

           the bug where registers fail to deallocate. Remove it to

           cause that bug to manifest again for debugging purposes,

           or insert a warning here to track WHICH scripts leave

           registers still allocated. */
        int16 i;
        for (i=0;i!=64;i++)
          FreeRegister(i);
      
# line 8396 "src/yygram.cpp"
      } break;
   }
return 0;
}

int fund_expr (PExp *ex)
{
   YYSTYPE n;
   YYSTYPE lv;
   rID rr;
   PExp ex2;
   YYSTYPE id;
   PExp obj;
   YYSTYPE fn;
   pBSysFunc b;
   VBlock bl;
   int narg;
   PExp rect;
   switch(yyselect()) {
   case 401: {
      get_lexval();
      n = yylval;
#line 1682 "lang/grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 8421 "src/yygram.cpp"
      } break;
   case 402: {
      get_lexval();
      n = yylval;
#line 1684 "lang/grammar.acc"
 ex->Type = DT_INT32; ex->Storage = RT_CONSTANT; ex->Value = n; ex->Code = NULL; 
# line 8428 "src/yygram.cpp"
      } break;
   case 403: {
      get_lexval();
      lv = yylval;
#line 1686 "lang/grammar.acc"
 ex->Type = DT_HTEXT; ex->Storage = RT_CONSTANT; ex->Value = lv; ex->Code = NULL; 
# line 8435 "src/yygram.cpp"
      } break;
   case 404: {
      get_lexval();
#line 1688 "lang/grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 1; ex->Code = NULL; 
# line 8441 "src/yygram.cpp"
      } break;
   case 405: {
      get_lexval();
#line 1690 "lang/grammar.acc"
 ex->Type = DT_BOOL; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 8447 "src/yygram.cpp"
      } break;
   case 406: {
      get_lexval();
#line 1692 "lang/grammar.acc"
 ex->Type = DT_UNKNOWN; ex->Storage = RT_CONSTANT; ex->Value = 0; ex->Code = NULL; 
# line 8453 "src/yygram.cpp"
      } break;
   case 407: {
      res_ref(&rr);
#line 1694 "lang/grammar.acc"
 ex->Type = DT_RID;   ex->Storage = RT_CONSTANT; ex->Value = rr; ex->Code = NULL; 
# line 8459 "src/yygram.cpp"
      } break;
   case 408: {
      get_lexval();
      expr(&ex2);
      get_lexval();
#line 1696 "lang/grammar.acc"
 ex->Code = ex2.Code; 
      ex->Type = ex2.Type; ex->Storage = ex2.Storage; ex->Value = ex2.Value; 
# line 8468 "src/yygram.cpp"
      } break;
   case 409: {
      get_lexval();
      id = yylval;
#line 1699 "lang/grammar.acc"
 BGlobalVar *b; BLocalVar *b2; BSysObj *b3; BResVar *b4; rID resID;
      resID = FIND(theModule->GetText(theRes->Name));
      ASSERT(resID);
      ASSERT(theEvent);
      if (b2 = GETBIND_LOCALVAR(id,resID,(int16)theEvent))
        {
          ex->Type = b2->VarType;
          ex->Storage = RT_MEMORY;
          ex->Value = -(b2->StackOffset);
        }
      else if (b4 = GETBIND_RESVAR(id,resID))
        {
          ex->Type = b4->VarType;
          ex->Storage = RT_MEMORY;
          ex->Value = b4->Address;
        }
      else if (b = GETBIND_GLOBALVAR(id))
        {
          ex->Type = b->VarType;
          ex->Storage = RT_MEMORY;
          ex->Value = b->Address;
        }
      else if (b3 = GETBIND_SYSOBJ(id))
        {
          ex->Type = DT_HOBJ;
          ex->Storage = RT_CONSTANT;
          ex->Value   = b3->ObjNum;
          ex->Code = NULL;
        }
      else
        {
          ex->Type = DT_INT32;
          ex->Storage = RT_CONSTANT; ex->Value = 0;
          if (IDENT_DEFINED(id))
            yyerror(Format("Improper use of identifier '%s'",theSymTab[id]));
          else
            yyerror(Format("Undeclared identifier '%s'",theSymTab[id]));

        }
    
# line 8514 "src/yygram.cpp"
      } break;
   case 410: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1015: {
         get_lexval();
         } break;
      case 1016: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1740 "lang/grammar.acc"

      BMemVar *b;
	  BResMem *b2;
      if (obj.Type == DT_RID) {
        if (!(b2 = GETBIND_RESMEM(id))) {
          yyerror(Format("Expected resource member in place of '%s'",theSymTab[id]));
          ex->Type = DT_VOID;
          ex->Storage = RT_CONSTANT;
          ex->Value = 0;
          }
        else {
          ex->Code = obj.Code;
          ex->Type = b2->VarType;
          if (obj.Storage == RT_REGISTER)
            FreeRegister((int16)obj.Value);
          ex->Value = AllocRegister();
          ex->Storage = RT_REGISTER;
          if (!ex->Code)
            ex->Code = new VBlock;
          ex->Code->Generate(GVAR,obj.Storage,obj.Value,
            RT_CONSTANT, b2->varid);
          ex->Code->Generate(MOV,ex->Storage,ex->Value,RT_REGISTER,0);
          }
        }
      else {
        if (obj.Type != DT_HOBJ && obj.Type != DT_STRING && obj.Type != DT_HTEXT)
          yyerror("Type musmatch: non-hObj on left side of '.' operator.");
        if (!(b = GETBIND_MEMVAR(id))) {
          yyerror(Format("Expected Member in place of '%s'",theSymTab[id]));
          ex->Type = DT_VOID;
          ex->Storage = RT_CONSTANT;
          ex->Value = 0;
          }
        else {
          ex->Code = obj.Code;
          ex->Type = b->VarType;
          if (obj.Storage == RT_REGISTER)
            FreeRegister((int16)obj.Value);

          if (!ex->Code)
            ex->Code = new VBlock;
          ex->Code->Generate(GVAR,obj.Storage,obj.Value,
            RT_CONSTANT, b->varid);
          if (ex->Type == DT_STRING) {
            ex->Value   = -AllocString();
            ex->Storage = RT_CONSTANT;
            ex->Code->Generate(WSTR,ex->Storage,ex->Value,RT_CONSTANT,-1);
            }
          else {
            ex->Value = AllocRegister();
            ex->Storage = RT_REGISTER;
            ex->Code->Generate(MOV,ex->Storage,ex->Value,RT_REGISTER,0);
            }
          }
        }
    
# line 8586 "src/yygram.cpp"
      } break;
   case 411: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1017: {
         get_lexval();
         } break;
      case 1018: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      fn = yylval;
#line 1797 "lang/grammar.acc"
 BSysFunc *b;
        if (obj.Type == DT_RID) {
          if (!(b = GETBIND_RESFUNC(fn)))
            yyerror(Format("Expected: member function name after '.' (%s).",
              theSymTab[fn]));
          }
        else if (obj.Type == DT_HOBJ || obj.Type == DT_STRING ||
                 obj.Type == DT_HTEXT) {
          if (!(b = GETBIND_MEMFUNC(fn)))
            yyerror(Format("Expected: member function name after '.' (%s).",
              theSymTab[fn]));
          }
        else
          yyerror("Type mismatch: non-hObj on left side of '.' operator.");
        ex->Code = new VBlock;
        /* Allocate a string, if necessary */
        if (obj.Type == DT_STRING || obj.Type == DT_HTEXT)
          if (obj.Storage == RT_CONSTANT && obj.Value >= 0)
            {
              int16 j = -AllocString();
              if (!obj.Code)
                obj.Code = new VBlock;
              obj.Code->Generate(WSTR,RT_CONSTANT,j,RT_CONSTANT,obj.Value);
              obj.Value = j;
            }          
      
# line 8628 "src/yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1824 "lang/grammar.acc"

        if (obj.Code)
          ex->Code->Add(obj.Code);
        ex->Code->Add(&bl);
        /* Adjust any stack offset to account for PUSH opcodes contained in

           bl. */
        if (obj.Storage == RT_MEMORY)
          if (obj.Value < 0)
            obj.Value -= narg;


        /* Call the Function */
        ex->Code->Generate(CMEM,obj.Storage,obj.Value,RT_CONSTANT,b->funcid);
        /* Clean up the Stack */
        ex->Code->Generate(DEC,RT_REGISTER,63,RT_CONSTANT,narg);
        ex->Type = b->ReturnType;
        if (ex->Type == DT_STRING) {
          ex->Value = -AllocString();
          ex->Storage = RT_CONSTANT;
          ex->Code->Generate(WSTR,ex->Storage,ex->Value,RT_CONSTANT,-1);
          }
        else {
          ex->Value = AllocRegister();
          ex->Storage = RT_REGISTER;
          ex->Code->Generate(MOV,RT_REGISTER,ex->Value,RT_REGISTER,0);
          }
      
# line 8661 "src/yygram.cpp"
      } break;
   case 412: {
      get_lexval();
      fn = yylval;
#line 1852 "lang/grammar.acc"
 BSysFunc *b;
        if (!(b = GETBIND_SYSFUNC(fn)))
          yyerror(Format("Expected: global function name before '(' (%s).",
            theSymTab[fn]));
        ex->Code = new VBlock;
      
# line 8673 "src/yygram.cpp"
      get_lexval();
      param_list(b, fn, &bl, &narg);
      get_lexval();
#line 1859 "lang/grammar.acc"

        ex->Code->Add(&bl);
        /* Call the Function */
        ex->Code->Generate(CMEM,RT_CONSTANT,0,RT_CONSTANT,b->funcid);
        /* Clean up the Stack */
        ex->Code->Generate(POP,RT_CONSTANT,narg);
        ex->Type = b->ReturnType;
        if (ex->Type == DT_STRING) {
          ex->Value = -AllocString();
          ex->Storage = RT_CONSTANT;
          ex->Code->Generate(WSTR,ex->Storage,ex->Value,RT_CONSTANT,-1);
          }
        else {
          ex->Value = AllocRegister();
          ex->Storage = RT_REGISTER;
          ex->Code->Generate(MOV,RT_REGISTER,ex->Value,RT_REGISTER,0);
          }
      
# line 8696 "src/yygram.cpp"
      } break;
   case 413: {
      fund_expr(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1878 "lang/grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *ex = CodeRectMember(&rect,(int16)id);
      
    
# line 8710 "src/yygram.cpp"
      } break;
   }
return 0;
}

int lvalue (LExp *lv)
{
   YYSTYPE id;
   LExp rect;
   PExp obj;
   switch(yyselect()) {
   case 414: {
      get_lexval();
      id = yylval;
#line 1888 "lang/grammar.acc"
 BGlobalVar *b; BLocalVar *b2; BResVar *b3; rID resID;
      resID = FIND(theModule->GetText(theRes->Name));
      ASSERT(resID);
      ASSERT(theEvent);
      
      if (b2 = GETBIND_LOCALVAR(id,resID,(int16)theEvent))
        {
          lv->Type = b2->VarType;
          lv->Storage = RT_MEMORY;
          lv->Value = -(b2->StackOffset);
          lv->RCode = NULL;
          lv->WCode = new VBlock;
          lv->WCode->Generate(MOV,RT_MEMORY,-b2->StackOffset,RT_REGISTER,0);
        }
      else if (b3 = GETBIND_RESVAR(id,resID))
        {
          lv->Type = b3->VarType;
          lv->Storage = RT_MEMORY;
          lv->Value = b3->Address;
          lv->RCode = NULL;
          lv->WCode = new VBlock;
          lv->WCode->Generate(MOV,RT_MEMORY,b3->Address,RT_REGISTER,0);
        }
      else if (b = GETBIND_GLOBALVAR(id))
        {
          lv->Type = b->VarType;
          lv->Storage = RT_MEMORY;
          lv->Value = b->Address;
          lv->RCode = NULL;
          lv->WCode = new VBlock;
          lv->WCode->Generate(MOV,RT_MEMORY,b->Address,RT_REGISTER,0);
        }
      else
        {
          lv->Type = DT_UNKNOWN;
          lv->Storage = RT_MEMORY; 
          lv->Value = 0;
          if (IDENT_DEFINED(id))
            yyerror(Format("Improper use of identifier '%s'",theSymTab[id]));
          else
            yyerror(Format("Undeclared identifier '%s'",theSymTab[id]));

        }
    
# line 8770 "src/yygram.cpp"
      } break;
   case 415: {
      lvalue(&rect);
      get_lexval();
      get_lexval();
      id = yylval;
#line 1933 "lang/grammar.acc"

      if (rect.Type != DT_RECT)
        yyerror("Type mismatch: '@' operator used on non-Rect target.");
      *lv = CodeRectLVal(&rect,(int16)id);
    
# line 8783 "src/yygram.cpp"
      } break;
   case 416: {
      fund_expr(&obj);
      switch (yyselect()) {
      case 1019: {
         get_lexval();
         } break;
      case 1020: {
         get_lexval();
         } break;
      default: printf("???\n"); exit(1);
      }
      get_lexval();
      id = yylval;
#line 1939 "lang/grammar.acc"

      int8 i;
	  BMemVar *b;
      if (obj.Type != DT_HOBJ)
        yyerror("Type mismatch: non-hObj on left side of '.' operator.");
      if (!(b = GETBIND_MEMVAR(id))) {
        yyerror(Format("Expected Member in place of '%s'",theSymTab[id]));
        lv->Type = DT_VOID;
        lv->Storage = RT_CONSTANT;
        lv->Value = 0;
        }
      else {
        lv->Type = b->VarType;
        if (obj.Storage == RT_REGISTER)
          FreeRegister((int16)obj.Value);
        lv->RCode = new VBlock;
        lv->RCode->Add(obj.Code);
        lv->RCode->Generate(GVAR,obj.Storage,obj.Value,
          RT_CONSTANT, b->varid);

        if (lv->Type == DT_STRING) {
          i = -AllocString();
          lv->Storage = RT_CONSTANT;
          lv->Value   = i;
          lv->RCode->Generate(WSTR,RT_CONSTANT,i,RT_CONSTANT,-1);
          }
        else {
          lv->Value = AllocRegister();
          lv->Storage = RT_REGISTER;
          lv->RCode->Generate(MOV,lv->Storage,lv->Value,RT_REGISTER,0);      
          }
        lv->WCode = new VBlock;
        lv->WCode->Add(obj.Code);
        lv->WCode->Generate(MOV,lv->Storage,lv->Value,RT_REGISTER,0);
        lv->WCode->Generate(SVAR,obj.Storage,obj.Value,
          RT_CONSTANT, b->varid);
        }
    
# line 8837 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr10 (PExp *ex)
{
   LExp lv;
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 417: {
      lvalue(&lv);
      get_lexval();
#line 1982 "lang/grammar.acc"
 *ex = CodeAssignment('i',lv,NULL); 
# line 8854 "src/yygram.cpp"
      } break;
   case 418: {
      lvalue(&lv);
      get_lexval();
#line 1984 "lang/grammar.acc"
 *ex = CodeAssignment('d',lv,NULL); 
# line 8861 "src/yygram.cpp"
      } break;
   case 419: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1986 "lang/grammar.acc"
 *ex = CodeOperator('m',ex1,ex2); 
# line 8872 "src/yygram.cpp"
      } break;
   case 420: {
      get_lexval();
      get_lexval();
      expr150(&ex1);
      get_lexval();
      expr150(&ex2);
      get_lexval();
#line 1988 "lang/grammar.acc"
 *ex = CodeOperator('M',ex1,ex2); 
# line 8883 "src/yygram.cpp"
      } break;
   case 421: {
      fund_expr(&ex1);
#line 1990 "lang/grammar.acc"
 *ex = ex1; 
# line 8889 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr20 (PExp *ex)
{
   LExp lv;
   PExp ex1;
   switch(yyselect()) {
   case 422: {
      get_lexval();
      lvalue(&lv);
#line 1995 "lang/grammar.acc"
 *ex = CodeAssignment('I',lv,NULL); 
# line 8905 "src/yygram.cpp"
      } break;
   case 423: {
      get_lexval();
      lvalue(&lv);
#line 1997 "lang/grammar.acc"
 *ex = CodeAssignment('D',lv,NULL); 
# line 8912 "src/yygram.cpp"
      } break;
   case 424: {
      get_lexval();
      expr20(&ex1);
#line 1999 "lang/grammar.acc"
 *ex = ex1; 
# line 8919 "src/yygram.cpp"
      } break;
   case 425: {
      get_lexval();
      expr20(&ex1);
#line 2001 "lang/grammar.acc"
 *ex = CodeOperator('-',ex1,ex1); 
# line 8926 "src/yygram.cpp"
      } break;
   case 426: {
      get_lexval();
      expr20(&ex1);
#line 2003 "lang/grammar.acc"
 *ex = CodeOperator('!',ex1,ex1); 
# line 8933 "src/yygram.cpp"
      } break;
   case 427: {
      get_lexval();
      expr20(&ex1);
#line 2005 "lang/grammar.acc"
 *ex = CodeOperator('~',ex1,ex1); 
# line 8940 "src/yygram.cpp"
      } break;
   case 428: {
      get_lexval();
      expr20(&ex1);
      get_lexval();
#line 2007 "lang/grammar.acc"
 *ex = CodeOperator(':',ex1,ex1); 
# line 8948 "src/yygram.cpp"
      } break;
   case 429: {
      expr10(&ex1);
#line 2009 "lang/grammar.acc"
 *ex = ex1; 
# line 8954 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr30 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 430: {
      expr20(&ex1);
      get_lexval();
      expr20(&ex2);
#line 2014 "lang/grammar.acc"
 *ex = CodeOperator('d',ex1,ex2); 
# line 8971 "src/yygram.cpp"
      } break;
   case 431: {
      expr20(&ex1);
#line 2016 "lang/grammar.acc"
 *ex = ex1; 
# line 8977 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr40 (PExp *ex)
{
   YYSTYPE tn;
   PExp ex1;
   switch(yyselect()) {
   case 432: {
      get_lexval();
      type_name(&tn);
      get_lexval();
      expr40(&ex1);
#line 2021 "lang/grammar.acc"
 *ex = ex1; ex->Type = (int8)tn; 
# line 8995 "src/yygram.cpp"
      } break;
   case 433: {
      expr30(&ex1);
#line 2023 "lang/grammar.acc"
 *ex = ex1; 
# line 9001 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr50 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 434: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2029 "lang/grammar.acc"
 *ex = CodeOperator('*',ex1,ex2); 
# line 9018 "src/yygram.cpp"
      } break;
   case 435: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2031 "lang/grammar.acc"
 *ex = CodeOperator('/',ex1,ex2); 
# line 9026 "src/yygram.cpp"
      } break;
   case 436: {
      expr50(&ex1);
      get_lexval();
      expr40(&ex2);
#line 2033 "lang/grammar.acc"
 *ex = CodeOperator('%',ex1,ex2); 
# line 9034 "src/yygram.cpp"
      } break;
   case 437: {
      expr40(&ex1);
#line 2035 "lang/grammar.acc"
 *ex = ex1; 
# line 9040 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr60 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 438: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 2040 "lang/grammar.acc"
 *ex = CodeOperator('+',ex1,ex2); 
# line 9057 "src/yygram.cpp"
      } break;
   case 439: {
      expr60(&ex1);
      get_lexval();
      expr50(&ex2);
#line 2042 "lang/grammar.acc"
 *ex = CodeOperator('-',ex1,ex2); 
# line 9065 "src/yygram.cpp"
      } break;
   case 440: {
      expr50(&ex1);
#line 2044 "lang/grammar.acc"
 *ex = ex1; 
# line 9071 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr70 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 441: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 2049 "lang/grammar.acc"
 *ex = CodeOperator(LSHIFT_OP,ex1,ex2); 
# line 9088 "src/yygram.cpp"
      } break;
   case 442: {
      expr70(&ex1);
      get_lexval();
      expr60(&ex2);
#line 2051 "lang/grammar.acc"
 *ex = CodeOperator(RSHIFT_OP,ex1,ex2); 
# line 9096 "src/yygram.cpp"
      } break;
   case 443: {
      expr60(&ex1);
#line 2053 "lang/grammar.acc"
 *ex = ex1; 
# line 9102 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr80 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 444: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2062 "lang/grammar.acc"
 *ex = CodeOperator('<',ex1,ex2); 
# line 9119 "src/yygram.cpp"
      } break;
   case 445: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2064 "lang/grammar.acc"
 *ex = CodeOperator('>',ex1,ex2); 
# line 9127 "src/yygram.cpp"
      } break;
   case 446: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2066 "lang/grammar.acc"
 *ex = CodeOperator(GE_OP,ex1,ex2); 
# line 9135 "src/yygram.cpp"
      } break;
   case 447: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2068 "lang/grammar.acc"
 *ex = CodeOperator(LE_OP,ex1,ex2); 
# line 9143 "src/yygram.cpp"
      } break;
   case 448: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2070 "lang/grammar.acc"
 *ex = CodeOperator(EQ_OP,ex1,ex2); 
# line 9151 "src/yygram.cpp"
      } break;
   case 449: {
      expr80(&ex1);
      get_lexval();
      expr70(&ex2);
#line 2072 "lang/grammar.acc"
 *ex = CodeOperator(NE_OP,ex1,ex2); 
# line 9159 "src/yygram.cpp"
      } break;
   case 450: {
      expr70(&ex1);
#line 2074 "lang/grammar.acc"
 *ex = ex1; 
# line 9165 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr90 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 451: {
      expr90(&ex1);
      get_lexval();
      expr80(&ex2);
#line 2079 "lang/grammar.acc"
 *ex = CodeOperator('&',ex1,ex2); 
# line 9182 "src/yygram.cpp"
      } break;
   case 452: {
      expr80(&ex1);
#line 2081 "lang/grammar.acc"
 *ex = ex1; 
# line 9188 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr100 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 453: {
      expr100(&ex1);
      get_lexval();
      expr90(&ex2);
#line 2086 "lang/grammar.acc"
 *ex = CodeOperator('^',ex1,ex2); 
# line 9205 "src/yygram.cpp"
      } break;
   case 454: {
      expr90(&ex1);
#line 2088 "lang/grammar.acc"
 *ex = ex1; 
# line 9211 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr110 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 455: {
      expr110(&ex1);
      get_lexval();
      expr100(&ex2);
#line 2093 "lang/grammar.acc"
 *ex = CodeOperator('|',ex1,ex2); 
# line 9228 "src/yygram.cpp"
      } break;
   case 456: {
      expr100(&ex1);
#line 2095 "lang/grammar.acc"
 *ex = ex1; 
# line 9234 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr120 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 457: {
      expr120(&ex1);
      get_lexval();
      expr110(&ex2);
#line 2100 "lang/grammar.acc"
 *ex = CodeOperator(AND_OP,ex1,ex2); 
# line 9251 "src/yygram.cpp"
      } break;
   case 458: {
      expr110(&ex1);
#line 2102 "lang/grammar.acc"
 *ex = ex1; 
# line 9257 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr130 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 459: {
      expr130(&ex1);
      get_lexval();
      expr120(&ex2);
#line 2107 "lang/grammar.acc"
 *ex = CodeOperator(OR_OP,ex1,ex2); 
# line 9274 "src/yygram.cpp"
      } break;
   case 460: {
      expr120(&ex1);
#line 2109 "lang/grammar.acc"
 *ex = ex1; 
# line 9280 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr140 (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   PExp ex3;
   switch(yyselect()) {
   case 461: {
      expr130(&ex1);
      get_lexval();
      expr(&ex2);
      get_lexval();
      expr140(&ex3);
#line 2114 "lang/grammar.acc"

      VBlock bl;
      if (!AllowedCast(ex1.Type,DT_BOOL))
        yyerror("Type mismatch: first operand of conditional operator "
                "must cast to bool.");
      if (!AllowedCast(ex2.Type,ex3.Type))
        yyerror("Type mismatch: conditional operator with incompatible "
                "types for true and false.");
      ex->Type = ex2.Type;
      ex->Code = new VBlock;
      ex->Code->Add(ex1.Code);
      ex->Code->Add(ex2.Code);
      ex->Code->Add(ex3.Code);
      
      if (ex2.Type == DT_STRING || ex3.Type == DT_STRING) {
        ex->Storage = RT_CONSTANT;
        ex->Value   = -AllocString();
        bl.Clear();
        ex->Code->Generate(CMEQ,ex1.Storage,ex1.Value,RT_CONSTANT,0);
        bl.Generate(WSTR, ex->Storage,ex->Value,ex2.Storage,ex2.Value);
        ex->Code->Generate(JTRU,RT_REGISTER, 0, RT_CONSTANT,2 + bl.GetSize());
        ex->Code->Add(&bl);
        bl.Clear();
        bl.Generate(WSTR, ex->Storage,ex->Value,ex3.Storage,ex3.Value);        
        ex->Code->Generate(JUMP,RT_CONSTANT,1 + bl.GetSize());
        ex->Code->Add(&bl);
        if (ex2.Storage == RT_CONSTANT && ex2.Type == DT_STRING && ex2.Value < -1)
          FreeString((int16)-ex2.Value);
        if (ex3.Storage == RT_CONSTANT && ex3.Type == DT_STRING && ex3.Value < -1)
          FreeString((int16)-ex3.Value);
        }
      else {
        ex->Storage = RT_REGISTER;
        ex->Value   = AllocRegister();
        bl.Clear();
        ex->Code->Generate(CMEQ,ex1.Storage,ex1.Value,RT_CONSTANT,0);
        bl.Generate(MOV, ex->Storage,ex->Value,ex2.Storage,ex2.Value);
        ex->Code->Generate(JTRU,RT_REGISTER, 0, RT_CONSTANT,2 + bl.GetSize());
        ex->Code->Add(&bl);
        bl.Clear();
        bl.Generate(MOV, ex->Storage,ex->Value,ex3.Storage,ex3.Value);        
        ex->Code->Generate(JUMP,RT_CONSTANT,1 + bl.GetSize());
        ex->Code->Add(&bl);
        }        
      if (ex2.Storage == RT_REGISTER)
        FreeRegister((int16)ex2.Value);
      if (ex3.Storage == RT_REGISTER)
        FreeRegister((int16)ex3.Value);
    
# line 9348 "src/yygram.cpp"
      } break;
   case 462: {
      expr130(&ex1);
#line 2164 "lang/grammar.acc"
 *ex = ex1; 
# line 9354 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr150 (PExp *ex)
{
   LExp lv;
   PExp ex2;
   PExp ex1;
   switch(yyselect()) {
   case 463: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2172 "lang/grammar.acc"
 *ex = CodeAssignment('=',lv,&ex2); 
# line 9372 "src/yygram.cpp"
      } break;
   case 464: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2174 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_ADD,lv,&ex2); 
# line 9380 "src/yygram.cpp"
      } break;
   case 465: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2176 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_SUB,lv,&ex2); 
# line 9388 "src/yygram.cpp"
      } break;
   case 466: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2178 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_MULT,lv,&ex2); 
# line 9396 "src/yygram.cpp"
      } break;
   case 467: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2180 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_DIV,lv,&ex2); 
# line 9404 "src/yygram.cpp"
      } break;
   case 468: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2182 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_MOD,lv,&ex2); 
# line 9412 "src/yygram.cpp"
      } break;
   case 469: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2184 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_AND,lv,&ex2); 
# line 9420 "src/yygram.cpp"
      } break;
   case 470: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2186 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_OR,lv,&ex2); 
# line 9428 "src/yygram.cpp"
      } break;
   case 471: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2188 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_LSHIFT,lv,&ex2); 
# line 9436 "src/yygram.cpp"
      } break;
   case 472: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2190 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_RSHIFT,lv,&ex2); 
# line 9444 "src/yygram.cpp"
      } break;
   case 473: {
      lvalue(&lv);
      get_lexval();
      expr140(&ex2);
#line 2192 "lang/grammar.acc"
 *ex = CodeAssignment(ASSIGN_NEG,lv,&ex2); 
# line 9452 "src/yygram.cpp"
      } break;
   case 474: {
      expr140(&ex1);
#line 2194 "lang/grammar.acc"
 *ex = ex1; 
# line 9458 "src/yygram.cpp"
      } break;
   }
return 0;
}

int expr (PExp *ex)
{
   PExp ex1;
   PExp ex2;
   switch(yyselect()) {
   case 475: {
      expr(&ex1);
      get_lexval();
      expr150(&ex2);
#line 2199 "lang/grammar.acc"

      ex->Code = ex1.Code;
      if (!ex->Code)
        ex->Code = new VBlock;
      if (ex2.Code) {
        ex->Code->Add(ex2.Code);
        delete ex2.Code;
        }
      ex->Storage = ex1.Storage;
      ex->Value   = ex1.Value;
      ex->Type    = ex1.Type;
    
# line 9486 "src/yygram.cpp"
      } break;
   case 476: {
      expr150(&ex1);
#line 2212 "lang/grammar.acc"
 *ex = ex1; 
# line 9492 "src/yygram.cpp"
      } break;
   }
return 0;
}

int param_list (pBSysFunc b, YYSTYPE fn, VBlock *bl, int *narg)
{
   PExp p;
   switch(yyselect()) {
   case 477: {
#line 2220 "lang/grammar.acc"
 *narg = 0; int16 i,j; bl->Clear();
    int16 PStorage[10]; int32 PValue[10]; 
# line 9506 "src/yygram.cpp"
      switch (yyselect()) {
      case 1022: {
         expr150(&p);
#line 2223 "lang/grammar.acc"

      if (!b)
        ;
      else {
        if (*narg < b->ParamCount)
          if (!AllowedCast(p.Type,b->ParamType[*narg]))
            yyerror(Format("Type mismatch in parameter %d of call to function %s (%s/%s).",
              (*narg)+1, theSymTab[fn], Lookup(DataTypeNames, b->ParamType[*narg]),
                Lookup(DataTypeNames,p.Type))); 
        bl->Add(p.Code);
        if (p.Type == DT_STRING && p.Storage == RT_CONSTANT)
          if (p.Value < 0)
            FreeString((int16)-p.Value);
        if (p.Storage == RT_MEMORY && p.Value < 0) {
          j = AllocRegister();
          bl->Generate(MOV,RT_REGISTER,j,p.Storage,p.Value);
          PStorage[*narg] = RT_REGISTER;
          PValue[*narg] = j;
          }
        else { 
          PStorage[*narg] = p.Storage;
          PValue[*narg]   = p.Value;
          }
        (*narg)++;
        }
    
# line 9537 "src/yygram.cpp"
         while (1) {
            switch (yyselect()) {
            case 1021: {
               get_lexval();
               expr150(&p);
#line 2250 "lang/grammar.acc"

      if (!b)
        ;
      else if (*narg == 10)
        yyerror("More than maximum of 10 parameters in function call!");
      else if (*narg > 10)
        ;
      else {
        if (*narg < b->ParamCount)
          if (!AllowedCast(p.Type,b->ParamType[*narg]))
            yyerror(Format("Type mismatch in parameter %d of call to function %s (%s/%s).",
              narg+1, theSymTab[fn], Lookup(DataTypeNames, b->ParamType[*narg]),
                Lookup(DataTypeNames,p.Type))); 
        if (*narg > b->ParamCount && !b->isVarParam)
          yyerror(Format("Too many parameters in call to function %s.",theSymTab[fn]));
        else {
          bl->Add(p.Code);
          if (p.Type == DT_STRING && p.Storage == RT_CONSTANT)
            if (p.Value < 0)
              FreeString((int16)-p.Value);
          if (p.Storage == RT_MEMORY && p.Value < 0) {
            j = AllocRegister();
            bl->Generate(MOV,RT_REGISTER,j,p.Storage,p.Value);
            PStorage[*narg] = RT_REGISTER;
            PValue[*narg] = j;
            }
          else { 
            PStorage[*narg] = p.Storage;
            PValue[*narg]   = p.Value;
            }
          }
        (*narg)++;
        }
    
# line 9578 "src/yygram.cpp"
               } break;
            default: goto yy106;
            }
         }
         yy106: ;
         } break;
      default: ;
      }
#line 2285 "lang/grammar.acc"

      if ((*narg) <= b->ParamCount)
        for(i=b->ParamCount;i!=(*narg);i--) {
          if (b->HasDefault & BIT(i+1)) {
            bl->Generate(PUSH,RT_CONSTANT,b->Default[i]);
            }
          else {
            yyerror(Format("Too few parameters in call to function %s.",theSymTab[fn]));
            goto Done;
            }
          }
      for(i=(*narg)-1;i!=-1;i--) {
        bl->Generate(PUSH,(int8)PStorage[i],PValue[i]);
        if (PStorage[i] == RT_REGISTER)
          FreeRegister((int16)PValue[i]);
        }
      *narg = max(*narg,b->ParamCount);
      Done:;
      
# line 9607 "src/yygram.cpp"
      } break;
   }
return 0;
}

extern YYSTYPE yylval;
YYSTYPE yylval;
/* GentleFlag = no */

typedef struct LEXELEMSTRUCT {
   YYSTYPE val;
   long pos;
   long sym;
   char * text;
   struct LEXELEMSTRUCT *next;
} LEXELEM;
   
LEXELEM *first_lexelem, *cur_lexelem;

void init_lexelem()
{
   cur_lexelem = first_lexelem;
}

void first_lexval () {
   LEXELEM *p;
   p = (LEXELEM *)malloc(sizeof(LEXELEM));
   if (! p) yymallocerror();
   p->val = yylval;
   p->pos = yypos;
   p->next = 0;
   cur_lexelem = p;
   first_lexelem = p;
}

void next_lexval() {
   LEXELEM *p;
   p = (LEXELEM *)malloc(sizeof(LEXELEM));
   if (! p) yymallocerror();
   cur_lexelem-> next = p;
   p->val = yylval;
   p->pos = yypos;
   p->next = 0;
   cur_lexelem = p;
}

void get_lexval() {
   extern int FREE_LEXELEMS;
   LEXELEM *p;
   yylval = cur_lexelem->val;
   yypos = cur_lexelem->pos;
   p = cur_lexelem;
   cur_lexelem = cur_lexelem->next;
   free(p);
}

extern int c_length;
int c_length = 6745;
extern int yygrammar[];
int yygrammar[] = {
0,
/* 1 */ 0,
/* 2 */ 6,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 478,
/* 6 */ 0,
/* 7 */ 10,
/* 8 */ -6,
/* 9 */ 1,
/* 10 */ 15,
/* 11 */ 18,
/* 12 */ 10,
/* 13 */ -10,
/* 14 */ 479,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ 1023,
/* 18 */ 24,
/* 19 */ 50380,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ 2,
/* 24 */ 30,
/* 25 */ 50433,
/* 26 */ 223,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ 3,
/* 30 */ 36,
/* 31 */ 50352,
/* 32 */ 50257,
/* 33 */ 50059,
/* 34 */ -18,
/* 35 */ 4,
/* 36 */ 40,
/* 37 */ 652,
/* 38 */ -18,
/* 39 */ 5,
/* 40 */ 44,
/* 41 */ 1431,
/* 42 */ -18,
/* 43 */ 6,
/* 44 */ 48,
/* 45 */ 2085,
/* 46 */ -18,
/* 47 */ 7,
/* 48 */ 52,
/* 49 */ 3583,
/* 50 */ -18,
/* 51 */ 8,
/* 52 */ 56,
/* 53 */ 3381,
/* 54 */ -18,
/* 55 */ 9,
/* 56 */ 60,
/* 57 */ 5320,
/* 58 */ -18,
/* 59 */ 10,
/* 60 */ 64,
/* 61 */ 2260,
/* 62 */ -18,
/* 63 */ 11,
/* 64 */ 68,
/* 65 */ 2697,
/* 66 */ -18,
/* 67 */ 12,
/* 68 */ 72,
/* 69 */ 2954,
/* 70 */ -18,
/* 71 */ 13,
/* 72 */ 76,
/* 73 */ 4015,
/* 74 */ -18,
/* 75 */ 14,
/* 76 */ 80,
/* 77 */ 3911,
/* 78 */ -18,
/* 79 */ 15,
/* 80 */ 84,
/* 81 */ 4279,
/* 82 */ -18,
/* 83 */ 16,
/* 84 */ 88,
/* 85 */ 4156,
/* 86 */ -18,
/* 87 */ 17,
/* 88 */ 92,
/* 89 */ 4596,
/* 90 */ -18,
/* 91 */ 18,
/* 92 */ 96,
/* 93 */ 5324,
/* 94 */ -18,
/* 95 */ 19,
/* 96 */ 100,
/* 97 */ 5689,
/* 98 */ -18,
/* 99 */ 20,
/* 100 */ 104,
/* 101 */ 5399,
/* 102 */ -18,
/* 103 */ 21,
/* 104 */ 108,
/* 105 */ 5822,
/* 106 */ -18,
/* 107 */ 22,
/* 108 */ 112,
/* 109 */ 5870,
/* 110 */ -18,
/* 111 */ 23,
/* 112 */ 116,
/* 113 */ 5960,
/* 114 */ -18,
/* 115 */ 24,
/* 116 */ 120,
/* 117 */ 6066,
/* 118 */ -18,
/* 119 */ 25,
/* 120 */ 124,
/* 121 */ 6048,
/* 122 */ -18,
/* 123 */ 26,
/* 124 */ 128,
/* 125 */ 6057,
/* 126 */ -18,
/* 127 */ 27,
/* 128 */ 134,
/* 129 */ 50350,
/* 130 */ 144,
/* 131 */ 173,
/* 132 */ -18,
/* 133 */ 28,
/* 134 */ 0,
/* 135 */ 50451,
/* 136 */ 50257,
/* 137 */ 50123,
/* 138 */ 148,
/* 139 */ 50257,
/* 140 */ 156,
/* 141 */ 50125,
/* 142 */ -18,
/* 143 */ 29,
/* 144 */ 0,
/* 145 */ 50058,
/* 146 */ -144,
/* 147 */ 480,
/* 148 */ 153,
/* 149 */ 50326,
/* 150 */ 50058,
/* 151 */ -148,
/* 152 */ 481,
/* 153 */ 0,
/* 154 */ -148,
/* 155 */ 1024,
/* 156 */ 160,
/* 157 */ 50059,
/* 158 */ -156,
/* 159 */ 482,
/* 160 */ 0,
/* 161 */ -156,
/* 162 */ 1025,
/* 163 */ 168,
/* 164 */ 50036,
/* 165 */ 50257,
/* 166 */ -163,
/* 167 */ 30,
/* 168 */ 0,
/* 169 */ 50036,
/* 170 */ 223,
/* 171 */ -163,
/* 172 */ 31,
/* 173 */ 179,
/* 174 */ 173,
/* 175 */ 50043,
/* 176 */ 189,
/* 177 */ -173,
/* 178 */ 32,
/* 179 */ 185,
/* 180 */ 173,
/* 181 */ 50045,
/* 182 */ 189,
/* 183 */ -173,
/* 184 */ 33,
/* 185 */ 0,
/* 186 */ 189,
/* 187 */ -173,
/* 188 */ 34,
/* 189 */ 195,
/* 190 */ 189,
/* 191 */ 50042,
/* 192 */ 223,
/* 193 */ -189,
/* 194 */ 35,
/* 195 */ 201,
/* 196 */ 189,
/* 197 */ 50047,
/* 198 */ 223,
/* 199 */ -189,
/* 200 */ 36,
/* 201 */ 207,
/* 202 */ 189,
/* 203 */ 50037,
/* 204 */ 223,
/* 205 */ -189,
/* 206 */ 37,
/* 207 */ 213,
/* 208 */ 189,
/* 209 */ 50124,
/* 210 */ 223,
/* 211 */ -189,
/* 212 */ 38,
/* 213 */ 219,
/* 214 */ 189,
/* 215 */ 50038,
/* 216 */ 223,
/* 217 */ -189,
/* 218 */ 39,
/* 219 */ 0,
/* 220 */ 223,
/* 221 */ -189,
/* 222 */ 40,
/* 223 */ 228,
/* 224 */ 50043,
/* 225 */ 237,
/* 226 */ -223,
/* 227 */ 41,
/* 228 */ 233,
/* 229 */ 50045,
/* 230 */ 237,
/* 231 */ -223,
/* 232 */ 42,
/* 233 */ 0,
/* 234 */ 237,
/* 235 */ -223,
/* 236 */ 43,
/* 237 */ 243,
/* 238 */ 50040,
/* 239 */ 173,
/* 240 */ 50041,
/* 241 */ -237,
/* 242 */ 44,
/* 243 */ 247,
/* 244 */ 50256,
/* 245 */ -237,
/* 246 */ 45,
/* 247 */ 251,
/* 248 */ 50458,
/* 249 */ -237,
/* 250 */ 46,
/* 251 */ 255,
/* 252 */ 50345,
/* 253 */ -237,
/* 254 */ 47,
/* 255 */ 0,
/* 256 */ 50259,
/* 257 */ -237,
/* 258 */ 48,
/* 259 */ 267,
/* 260 */ 237,
/* 261 */ 50100,
/* 262 */ 237,
/* 263 */ 50043,
/* 264 */ 237,
/* 265 */ -259,
/* 266 */ 49,
/* 267 */ 276,
/* 268 */ 237,
/* 269 */ 50100,
/* 270 */ 237,
/* 271 */ 50043,
/* 272 */ 50045,
/* 273 */ 237,
/* 274 */ -259,
/* 275 */ 50,
/* 276 */ 282,
/* 277 */ 237,
/* 278 */ 50100,
/* 279 */ 237,
/* 280 */ -259,
/* 281 */ 51,
/* 282 */ 0,
/* 283 */ 223,
/* 284 */ -259,
/* 285 */ 52,
/* 286 */ 294,
/* 287 */ 237,
/* 288 */ 50100,
/* 289 */ 237,
/* 290 */ 50043,
/* 291 */ 237,
/* 292 */ -286,
/* 293 */ 53,
/* 294 */ 0,
/* 295 */ 237,
/* 296 */ 50100,
/* 297 */ 237,
/* 298 */ -286,
/* 299 */ 54,
/* 300 */ 0,
/* 301 */ 310,
/* 302 */ 320,
/* 303 */ 328,
/* 304 */ 335,
/* 305 */ 346,
/* 306 */ 355,
/* 307 */ 368,
/* 308 */ -300,
/* 309 */ 55,
/* 310 */ 317,
/* 311 */ 50367,
/* 312 */ 50040,
/* 313 */ 588,
/* 314 */ 50041,
/* 315 */ -310,
/* 316 */ 483,
/* 317 */ 0,
/* 318 */ -310,
/* 319 */ 1026,
/* 320 */ 325,
/* 321 */ 50256,
/* 322 */ 50037,
/* 323 */ -320,
/* 324 */ 484,
/* 325 */ 0,
/* 326 */ -320,
/* 327 */ 1027,
/* 328 */ 332,
/* 329 */ 286,
/* 330 */ -328,
/* 331 */ 485,
/* 332 */ 0,
/* 333 */ -328,
/* 334 */ 1028,
/* 335 */ 339,
/* 336 */ 50317,
/* 337 */ -335,
/* 338 */ 486,
/* 339 */ 343,
/* 340 */ 50300,
/* 341 */ -335,
/* 342 */ 487,
/* 343 */ 0,
/* 344 */ -335,
/* 345 */ 1029,
/* 346 */ 350,
/* 347 */ 163,
/* 348 */ -346,
/* 349 */ 488,
/* 350 */ 0,
/* 351 */ 50419,
/* 352 */ 223,
/* 353 */ -346,
/* 354 */ 489,
/* 355 */ 360,
/* 356 */ 50043,
/* 357 */ 50256,
/* 358 */ -355,
/* 359 */ 490,
/* 360 */ 365,
/* 361 */ 50045,
/* 362 */ 50256,
/* 363 */ -355,
/* 364 */ 491,
/* 365 */ 0,
/* 366 */ -355,
/* 367 */ 1030,
/* 368 */ 376,
/* 369 */ 50471,
/* 370 */ 50091,
/* 371 */ 417,
/* 372 */ 50093,
/* 373 */ 368,
/* 374 */ -368,
/* 375 */ 494,
/* 376 */ 382,
/* 377 */ 50399,
/* 378 */ 163,
/* 379 */ 368,
/* 380 */ -368,
/* 381 */ 495,
/* 382 */ 391,
/* 383 */ 50399,
/* 384 */ 50419,
/* 385 */ 223,
/* 386 */ 50457,
/* 387 */ 223,
/* 388 */ 368,
/* 389 */ -368,
/* 390 */ 496,
/* 391 */ 398,
/* 392 */ 50399,
/* 393 */ 50419,
/* 394 */ 223,
/* 395 */ 368,
/* 396 */ -368,
/* 397 */ 497,
/* 398 */ 404,
/* 399 */ 50399,
/* 400 */ 50419,
/* 401 */ 368,
/* 402 */ -368,
/* 403 */ 498,
/* 404 */ 414,
/* 405 */ 50295,
/* 406 */ 50040,
/* 407 */ 223,
/* 408 */ 50044,
/* 409 */ 223,
/* 410 */ 50041,
/* 411 */ 368,
/* 412 */ -368,
/* 413 */ 499,
/* 414 */ 0,
/* 415 */ -368,
/* 416 */ 1033,
/* 417 */ 423,
/* 418 */ 173,
/* 419 */ 426,
/* 420 */ 417,
/* 421 */ -417,
/* 422 */ 493,
/* 423 */ 0,
/* 424 */ -417,
/* 425 */ 1032,
/* 426 */ 430,
/* 427 */ 50044,
/* 428 */ -426,
/* 429 */ 492,
/* 430 */ 0,
/* 431 */ -426,
/* 432 */ 1031,
/* 433 */ 0,
/* 434 */ 50356,
/* 435 */ 441,
/* 436 */ 445,
/* 437 */ 300,
/* 438 */ 50059,
/* 439 */ -433,
/* 440 */ 56,
/* 441 */ 0,
/* 442 */ 50058,
/* 443 */ -441,
/* 444 */ 500,
/* 445 */ 451,
/* 446 */ 300,
/* 447 */ 454,
/* 448 */ 445,
/* 449 */ -445,
/* 450 */ 503,
/* 451 */ 0,
/* 452 */ -445,
/* 453 */ 1034,
/* 454 */ 458,
/* 455 */ 50044,
/* 456 */ -454,
/* 457 */ 501,
/* 458 */ 0,
/* 459 */ 50335,
/* 460 */ -454,
/* 461 */ 502,
/* 462 */ 0,
/* 463 */ 471,
/* 464 */ 479,
/* 465 */ 486,
/* 466 */ 493,
/* 467 */ 501,
/* 468 */ 50059,
/* 469 */ -462,
/* 470 */ 57,
/* 471 */ 475,
/* 472 */ 50368,
/* 473 */ -471,
/* 474 */ 504,
/* 475 */ 0,
/* 476 */ 50357,
/* 477 */ -471,
/* 478 */ 505,
/* 479 */ 483,
/* 480 */ 50058,
/* 481 */ -479,
/* 482 */ 506,
/* 483 */ 0,
/* 484 */ -479,
/* 485 */ 1035,
/* 486 */ 490,
/* 487 */ 1247,
/* 488 */ -486,
/* 489 */ 507,
/* 490 */ 0,
/* 491 */ -486,
/* 492 */ 1036,
/* 493 */ 497,
/* 494 */ 50259,
/* 495 */ -493,
/* 496 */ 508,
/* 497 */ 0,
/* 498 */ 50256,
/* 499 */ -493,
/* 500 */ 509,
/* 501 */ 506,
/* 502 */ 50400,
/* 503 */ 1247,
/* 504 */ -501,
/* 505 */ 510,
/* 506 */ 0,
/* 507 */ -501,
/* 508 */ 1037,
/* 509 */ 0,
/* 510 */ 515,
/* 511 */ 522,
/* 512 */ 530,
/* 513 */ -509,
/* 514 */ 58,
/* 515 */ 519,
/* 516 */ 1247,
/* 517 */ -515,
/* 518 */ 511,
/* 519 */ 0,
/* 520 */ -515,
/* 521 */ 1038,
/* 522 */ 526,
/* 523 */ 50259,
/* 524 */ -522,
/* 525 */ 512,
/* 526 */ 0,
/* 527 */ 50256,
/* 528 */ -522,
/* 529 */ 513,
/* 530 */ 535,
/* 531 */ 50400,
/* 532 */ 1247,
/* 533 */ -530,
/* 534 */ 514,
/* 535 */ 0,
/* 536 */ -530,
/* 537 */ 1039,
/* 538 */ 0,
/* 539 */ 50400,
/* 540 */ 50342,
/* 541 */ 555,
/* 542 */ 546,
/* 543 */ 50059,
/* 544 */ -538,
/* 545 */ 59,
/* 546 */ 552,
/* 547 */ 50044,
/* 548 */ 555,
/* 549 */ 546,
/* 550 */ -546,
/* 551 */ 515,
/* 552 */ 0,
/* 553 */ -546,
/* 554 */ 1040,
/* 555 */ 0,
/* 556 */ 173,
/* 557 */ 561,
/* 558 */ 570,
/* 559 */ -555,
/* 560 */ 60,
/* 561 */ 567,
/* 562 */ 50044,
/* 563 */ 173,
/* 564 */ 561,
/* 565 */ -561,
/* 566 */ 516,
/* 567 */ 0,
/* 568 */ -561,
/* 569 */ 1041,
/* 570 */ 575,
/* 571 */ 50257,
/* 572 */ 579,
/* 573 */ -570,
/* 574 */ 518,
/* 575 */ 0,
/* 576 */ 6261,
/* 577 */ -570,
/* 578 */ 519,
/* 579 */ 585,
/* 580 */ 50047,
/* 581 */ 50257,
/* 582 */ 579,
/* 583 */ -579,
/* 584 */ 517,
/* 585 */ 0,
/* 586 */ -579,
/* 587 */ 1042,
/* 588 */ 0,
/* 589 */ 6715,
/* 590 */ -588,
/* 591 */ 61,
/* 592 */ 0,
/* 593 */ 6114,
/* 594 */ -592,
/* 595 */ 62,
/* 596 */ 0,
/* 597 */ 601,
/* 598 */ 628,
/* 599 */ -596,
/* 600 */ 63,
/* 601 */ 606,
/* 602 */ 621,
/* 603 */ 237,
/* 604 */ -601,
/* 605 */ 521,
/* 606 */ 611,
/* 607 */ 50045,
/* 608 */ 237,
/* 609 */ -601,
/* 610 */ 522,
/* 611 */ 616,
/* 612 */ 50061,
/* 613 */ 237,
/* 614 */ -601,
/* 615 */ 523,
/* 616 */ 0,
/* 617 */ 223,
/* 618 */ 50037,
/* 619 */ -601,
/* 620 */ 524,
/* 621 */ 625,
/* 622 */ 50043,
/* 623 */ -621,
/* 624 */ 520,
/* 625 */ 0,
/* 626 */ -621,
/* 627 */ 1043,
/* 628 */ 634,
/* 629 */ 50040,
/* 630 */ 637,
/* 631 */ 50041,
/* 632 */ -628,
/* 633 */ 528,
/* 634 */ 0,
/* 635 */ -628,
/* 636 */ 1044,
/* 637 */ 642,
/* 638 */ 50387,
/* 639 */ 237,
/* 640 */ -637,
/* 641 */ 525,
/* 642 */ 647,
/* 643 */ 50384,
/* 644 */ 237,
/* 645 */ -637,
/* 646 */ 526,
/* 647 */ 0,
/* 648 */ 50391,
/* 649 */ 237,
/* 650 */ -637,
/* 651 */ 527,
/* 652 */ 0,
/* 653 */ 50388,
/* 654 */ 50257,
/* 655 */ 50058,
/* 656 */ 223,
/* 657 */ 663,
/* 658 */ 50123,
/* 659 */ 680,
/* 660 */ 50125,
/* 661 */ -652,
/* 662 */ 64,
/* 663 */ 669,
/* 664 */ 50044,
/* 665 */ 223,
/* 666 */ 672,
/* 667 */ -663,
/* 668 */ 530,
/* 669 */ 0,
/* 670 */ -663,
/* 671 */ 1046,
/* 672 */ 677,
/* 673 */ 50044,
/* 674 */ 223,
/* 675 */ -672,
/* 676 */ 529,
/* 677 */ 0,
/* 678 */ -672,
/* 679 */ 1045,
/* 680 */ 685,
/* 681 */ 688,
/* 682 */ 680,
/* 683 */ -680,
/* 684 */ 531,
/* 685 */ 0,
/* 686 */ -680,
/* 687 */ 1047,
/* 688 */ 695,
/* 689 */ 50326,
/* 690 */ 932,
/* 691 */ 50257,
/* 692 */ 50059,
/* 693 */ -688,
/* 694 */ 65,
/* 695 */ 702,
/* 696 */ 50325,
/* 697 */ 939,
/* 698 */ 223,
/* 699 */ 50059,
/* 700 */ -688,
/* 701 */ 66,
/* 702 */ 709,
/* 703 */ 946,
/* 704 */ 954,
/* 705 */ 223,
/* 706 */ 50059,
/* 707 */ -688,
/* 708 */ 67,
/* 709 */ 718,
/* 710 */ 961,
/* 711 */ 969,
/* 712 */ 223,
/* 713 */ 50047,
/* 714 */ 223,
/* 715 */ 50059,
/* 716 */ -688,
/* 717 */ 68,
/* 718 */ 726,
/* 719 */ 50450,
/* 720 */ 976,
/* 721 */ 173,
/* 722 */ 983,
/* 723 */ 50059,
/* 724 */ -688,
/* 725 */ 69,
/* 726 */ 733,
/* 727 */ 50364,
/* 728 */ 992,
/* 729 */ 173,
/* 730 */ 50059,
/* 731 */ -688,
/* 732 */ 70,
/* 733 */ 740,
/* 734 */ 50365,
/* 735 */ 999,
/* 736 */ 173,
/* 737 */ 50059,
/* 738 */ -688,
/* 739 */ 71,
/* 740 */ 748,
/* 741 */ 1006,
/* 742 */ 50366,
/* 743 */ 1013,
/* 744 */ 173,
/* 745 */ 50059,
/* 746 */ -688,
/* 747 */ 72,
/* 748 */ 752,
/* 749 */ 462,
/* 750 */ -688,
/* 751 */ 73,
/* 752 */ 760,
/* 753 */ 50440,
/* 754 */ 50091,
/* 755 */ 223,
/* 756 */ 50093,
/* 757 */ 50059,
/* 758 */ -688,
/* 759 */ 74,
/* 760 */ 770,
/* 761 */ 50440,
/* 762 */ 50091,
/* 763 */ 223,
/* 764 */ 50044,
/* 765 */ 223,
/* 766 */ 50093,
/* 767 */ 50059,
/* 768 */ -688,
/* 769 */ 75,
/* 770 */ 782,
/* 771 */ 50440,
/* 772 */ 50091,
/* 773 */ 223,
/* 774 */ 50044,
/* 775 */ 223,
/* 776 */ 50044,
/* 777 */ 223,
/* 778 */ 50093,
/* 779 */ 50059,
/* 780 */ -688,
/* 781 */ 76,
/* 782 */ 796,
/* 783 */ 50440,
/* 784 */ 50091,
/* 785 */ 223,
/* 786 */ 50044,
/* 787 */ 223,
/* 788 */ 50044,
/* 789 */ 223,
/* 790 */ 50044,
/* 791 */ 163,
/* 792 */ 50093,
/* 793 */ 50059,
/* 794 */ -688,
/* 795 */ 77,
/* 796 */ 803,
/* 797 */ 50424,
/* 798 */ 1020,
/* 799 */ 1027,
/* 800 */ 50059,
/* 801 */ -688,
/* 802 */ 78,
/* 803 */ 810,
/* 804 */ 50369,
/* 805 */ 1043,
/* 806 */ 1050,
/* 807 */ 50059,
/* 808 */ -688,
/* 809 */ 79,
/* 810 */ 818,
/* 811 */ 50296,
/* 812 */ 1066,
/* 813 */ 1266,
/* 814 */ 1073,
/* 815 */ 50059,
/* 816 */ -688,
/* 817 */ 80,
/* 818 */ 824,
/* 819 */ 50294,
/* 820 */ 50257,
/* 821 */ 50059,
/* 822 */ -688,
/* 823 */ 81,
/* 824 */ 830,
/* 825 */ 50294,
/* 826 */ 223,
/* 827 */ 50059,
/* 828 */ -688,
/* 829 */ 82,
/* 830 */ 837,
/* 831 */ 50323,
/* 832 */ 1082,
/* 833 */ 173,
/* 834 */ 50059,
/* 835 */ -688,
/* 836 */ 83,
/* 837 */ 844,
/* 838 */ 50292,
/* 839 */ 1089,
/* 840 */ 173,
/* 841 */ 50059,
/* 842 */ -688,
/* 843 */ 84,
/* 844 */ 851,
/* 845 */ 50381,
/* 846 */ 1096,
/* 847 */ 173,
/* 848 */ 50059,
/* 849 */ -688,
/* 850 */ 85,
/* 851 */ 859,
/* 852 */ 50346,
/* 853 */ 1103,
/* 854 */ 1110,
/* 855 */ 223,
/* 856 */ 50059,
/* 857 */ -688,
/* 858 */ 86,
/* 859 */ 866,
/* 860 */ 50350,
/* 861 */ 1119,
/* 862 */ 1126,
/* 863 */ 50059,
/* 864 */ -688,
/* 865 */ 87,
/* 866 */ 872,
/* 867 */ 1418,
/* 868 */ 1134,
/* 869 */ 50059,
/* 870 */ -688,
/* 871 */ 88,
/* 872 */ 879,
/* 873 */ 50431,
/* 874 */ 1143,
/* 875 */ 223,
/* 876 */ 50059,
/* 877 */ -688,
/* 878 */ 89,
/* 879 */ 887,
/* 880 */ 50435,
/* 881 */ 1150,
/* 882 */ 223,
/* 883 */ 1157,
/* 884 */ 50059,
/* 885 */ -688,
/* 886 */ 90,
/* 887 */ 895,
/* 888 */ 50389,
/* 889 */ 1164,
/* 890 */ 223,
/* 891 */ 1171,
/* 892 */ 50059,
/* 893 */ -688,
/* 894 */ 91,
/* 895 */ 902,
/* 896 */ 50437,
/* 897 */ 1178,
/* 898 */ 1185,
/* 899 */ 50059,
/* 900 */ -688,
/* 901 */ 92,
/* 902 */ 909,
/* 903 */ 50330,
/* 904 */ 1201,
/* 905 */ 1208,
/* 906 */ 50059,
/* 907 */ -688,
/* 908 */ 93,
/* 909 */ 916,
/* 910 */ 50409,
/* 911 */ 1224,
/* 912 */ 1231,
/* 913 */ 50059,
/* 914 */ -688,
/* 915 */ 94,
/* 916 */ 920,
/* 917 */ 433,
/* 918 */ -688,
/* 919 */ 95,
/* 920 */ 924,
/* 921 */ 538,
/* 922 */ -688,
/* 923 */ 96,
/* 924 */ 928,
/* 925 */ 3423,
/* 926 */ -688,
/* 927 */ 97,
/* 928 */ 0,
/* 929 */ 6098,
/* 930 */ -688,
/* 931 */ 98,
/* 932 */ 936,
/* 933 */ 50058,
/* 934 */ -932,
/* 935 */ 532,
/* 936 */ 0,
/* 937 */ -932,
/* 938 */ 1048,
/* 939 */ 943,
/* 940 */ 50058,
/* 941 */ -939,
/* 942 */ 533,
/* 943 */ 0,
/* 944 */ -939,
/* 945 */ 1049,
/* 946 */ 950,
/* 947 */ 50374,
/* 948 */ -946,
/* 949 */ 534,
/* 950 */ 0,
/* 951 */ 50307,
/* 952 */ -946,
/* 953 */ 535,
/* 954 */ 958,
/* 955 */ 50058,
/* 956 */ -954,
/* 957 */ 536,
/* 958 */ 0,
/* 959 */ -954,
/* 960 */ 1050,
/* 961 */ 965,
/* 962 */ 50374,
/* 963 */ -961,
/* 964 */ 537,
/* 965 */ 0,
/* 966 */ 50307,
/* 967 */ -961,
/* 968 */ 538,
/* 969 */ 973,
/* 970 */ 50058,
/* 971 */ -969,
/* 972 */ 539,
/* 973 */ 0,
/* 974 */ -969,
/* 975 */ 1051,
/* 976 */ 980,
/* 977 */ 50058,
/* 978 */ -976,
/* 979 */ 540,
/* 980 */ 0,
/* 981 */ -976,
/* 982 */ 1052,
/* 983 */ 989,
/* 984 */ 50044,
/* 985 */ 173,
/* 986 */ 983,
/* 987 */ -983,
/* 988 */ 541,
/* 989 */ 0,
/* 990 */ -983,
/* 991 */ 1053,
/* 992 */ 996,
/* 993 */ 50058,
/* 994 */ -992,
/* 995 */ 542,
/* 996 */ 0,
/* 997 */ -992,
/* 998 */ 1054,
/* 999 */ 1003,
/* 1000 */ 50058,
/* 1001 */ -999,
/* 1002 */ 543,
/* 1003 */ 0,
/* 1004 */ -999,
/* 1005 */ 1055,
/* 1006 */ 1010,
/* 1007 */ 50043,
/* 1008 */ -1006,
/* 1009 */ 544,
/* 1010 */ 0,
/* 1011 */ -1006,
/* 1012 */ 1056,
/* 1013 */ 1017,
/* 1014 */ 50058,
/* 1015 */ -1013,
/* 1016 */ 545,
/* 1017 */ 0,
/* 1018 */ -1013,
/* 1019 */ 1057,
/* 1020 */ 1024,
/* 1021 */ 50058,
/* 1022 */ -1020,
/* 1023 */ 546,
/* 1024 */ 0,
/* 1025 */ -1020,
/* 1026 */ 1058,
/* 1027 */ 1033,
/* 1028 */ 223,
/* 1029 */ 1036,
/* 1030 */ 1027,
/* 1031 */ -1027,
/* 1032 */ 548,
/* 1033 */ 0,
/* 1034 */ -1027,
/* 1035 */ 1060,
/* 1036 */ 1040,
/* 1037 */ 50044,
/* 1038 */ -1036,
/* 1039 */ 547,
/* 1040 */ 0,
/* 1041 */ -1036,
/* 1042 */ 1059,
/* 1043 */ 1047,
/* 1044 */ 50058,
/* 1045 */ -1043,
/* 1046 */ 549,
/* 1047 */ 0,
/* 1048 */ -1043,
/* 1049 */ 1061,
/* 1050 */ 1056,
/* 1051 */ 223,
/* 1052 */ 1059,
/* 1053 */ 1050,
/* 1054 */ -1050,
/* 1055 */ 551,
/* 1056 */ 0,
/* 1057 */ -1050,
/* 1058 */ 1063,
/* 1059 */ 1063,
/* 1060 */ 50044,
/* 1061 */ -1059,
/* 1062 */ 550,
/* 1063 */ 0,
/* 1064 */ -1059,
/* 1065 */ 1062,
/* 1066 */ 1070,
/* 1067 */ 50058,
/* 1068 */ -1066,
/* 1069 */ 552,
/* 1070 */ 0,
/* 1071 */ -1066,
/* 1072 */ 1064,
/* 1073 */ 1079,
/* 1074 */ 50044,
/* 1075 */ 1266,
/* 1076 */ 1073,
/* 1077 */ -1073,
/* 1078 */ 553,
/* 1079 */ 0,
/* 1080 */ -1073,
/* 1081 */ 1065,
/* 1082 */ 1086,
/* 1083 */ 50058,
/* 1084 */ -1082,
/* 1085 */ 554,
/* 1086 */ 0,
/* 1087 */ -1082,
/* 1088 */ 1066,
/* 1089 */ 1093,
/* 1090 */ 50058,
/* 1091 */ -1089,
/* 1092 */ 555,
/* 1093 */ 0,
/* 1094 */ -1089,
/* 1095 */ 1067,
/* 1096 */ 1100,
/* 1097 */ 50058,
/* 1098 */ -1096,
/* 1099 */ 556,
/* 1100 */ 0,
/* 1101 */ -1096,
/* 1102 */ 1068,
/* 1103 */ 1107,
/* 1104 */ 50058,
/* 1105 */ -1103,
/* 1106 */ 557,
/* 1107 */ 0,
/* 1108 */ -1103,
/* 1109 */ 1069,
/* 1110 */ 1116,
/* 1111 */ 223,
/* 1112 */ 50044,
/* 1113 */ 1110,
/* 1114 */ -1110,
/* 1115 */ 558,
/* 1116 */ 0,
/* 1117 */ -1110,
/* 1118 */ 1070,
/* 1119 */ 1123,
/* 1120 */ 50058,
/* 1121 */ -1119,
/* 1122 */ 559,
/* 1123 */ 0,
/* 1124 */ -1119,
/* 1125 */ 1071,
/* 1126 */ 1131,
/* 1127 */ 1393,
/* 1128 */ 1126,
/* 1129 */ -1126,
/* 1130 */ 560,
/* 1131 */ 0,
/* 1132 */ -1126,
/* 1133 */ 1072,
/* 1134 */ 1140,
/* 1135 */ 50044,
/* 1136 */ 1418,
/* 1137 */ 1134,
/* 1138 */ -1134,
/* 1139 */ 561,
/* 1140 */ 0,
/* 1141 */ -1134,
/* 1142 */ 1073,
/* 1143 */ 1147,
/* 1144 */ 50058,
/* 1145 */ -1143,
/* 1146 */ 562,
/* 1147 */ 0,
/* 1148 */ -1143,
/* 1149 */ 1074,
/* 1150 */ 1154,
/* 1151 */ 50058,
/* 1152 */ -1150,
/* 1153 */ 563,
/* 1154 */ 0,
/* 1155 */ -1150,
/* 1156 */ 1075,
/* 1157 */ 1161,
/* 1158 */ 50037,
/* 1159 */ -1157,
/* 1160 */ 564,
/* 1161 */ 0,
/* 1162 */ -1157,
/* 1163 */ 1076,
/* 1164 */ 1168,
/* 1165 */ 50058,
/* 1166 */ -1164,
/* 1167 */ 565,
/* 1168 */ 0,
/* 1169 */ -1164,
/* 1170 */ 1077,
/* 1171 */ 1175,
/* 1172 */ 50037,
/* 1173 */ -1171,
/* 1174 */ 566,
/* 1175 */ 0,
/* 1176 */ -1171,
/* 1177 */ 1078,
/* 1178 */ 1182,
/* 1179 */ 50058,
/* 1180 */ -1178,
/* 1181 */ 567,
/* 1182 */ 0,
/* 1183 */ -1178,
/* 1184 */ 1079,
/* 1185 */ 1191,
/* 1186 */ 163,
/* 1187 */ 1194,
/* 1188 */ 1185,
/* 1189 */ -1185,
/* 1190 */ 569,
/* 1191 */ 0,
/* 1192 */ -1185,
/* 1193 */ 1081,
/* 1194 */ 1198,
/* 1195 */ 50044,
/* 1196 */ -1194,
/* 1197 */ 568,
/* 1198 */ 0,
/* 1199 */ -1194,
/* 1200 */ 1080,
/* 1201 */ 1205,
/* 1202 */ 50058,
/* 1203 */ -1201,
/* 1204 */ 570,
/* 1205 */ 0,
/* 1206 */ -1201,
/* 1207 */ 1082,
/* 1208 */ 1214,
/* 1209 */ 163,
/* 1210 */ 1217,
/* 1211 */ 1208,
/* 1212 */ -1208,
/* 1213 */ 572,
/* 1214 */ 0,
/* 1215 */ -1208,
/* 1216 */ 1084,
/* 1217 */ 1221,
/* 1218 */ 50044,
/* 1219 */ -1217,
/* 1220 */ 571,
/* 1221 */ 0,
/* 1222 */ -1217,
/* 1223 */ 1083,
/* 1224 */ 1228,
/* 1225 */ 50058,
/* 1226 */ -1224,
/* 1227 */ 573,
/* 1228 */ 0,
/* 1229 */ -1224,
/* 1230 */ 1085,
/* 1231 */ 1237,
/* 1232 */ 163,
/* 1233 */ 1240,
/* 1234 */ 1231,
/* 1235 */ -1231,
/* 1236 */ 575,
/* 1237 */ 0,
/* 1238 */ -1231,
/* 1239 */ 1087,
/* 1240 */ 1244,
/* 1241 */ 50044,
/* 1242 */ -1240,
/* 1243 */ 574,
/* 1244 */ 0,
/* 1245 */ -1240,
/* 1246 */ 1086,
/* 1247 */ 1251,
/* 1248 */ 50275,
/* 1249 */ -1247,
/* 1250 */ 99,
/* 1251 */ 1256,
/* 1252 */ 50303,
/* 1253 */ 50275,
/* 1254 */ -1247,
/* 1255 */ 100,
/* 1256 */ 1261,
/* 1257 */ 50376,
/* 1258 */ 50275,
/* 1259 */ -1247,
/* 1260 */ 101,
/* 1261 */ 0,
/* 1262 */ 50320,
/* 1263 */ 50275,
/* 1264 */ -1247,
/* 1265 */ 102,
/* 1266 */ 1276,
/* 1267 */ 223,
/* 1268 */ 50353,
/* 1269 */ 1283,
/* 1270 */ 1295,
/* 1271 */ 1302,
/* 1272 */ 1309,
/* 1273 */ 1331,
/* 1274 */ -1266,
/* 1275 */ 103,
/* 1276 */ 0,
/* 1277 */ 223,
/* 1278 */ 50353,
/* 1279 */ 163,
/* 1280 */ 1386,
/* 1281 */ -1266,
/* 1282 */ 104,
/* 1283 */ 1287,
/* 1284 */ 286,
/* 1285 */ -1283,
/* 1286 */ 576,
/* 1287 */ 1292,
/* 1288 */ 237,
/* 1289 */ 50037,
/* 1290 */ -1283,
/* 1291 */ 577,
/* 1292 */ 0,
/* 1293 */ -1283,
/* 1294 */ 1088,
/* 1295 */ 1299,
/* 1296 */ 237,
/* 1297 */ -1295,
/* 1298 */ 578,
/* 1299 */ 0,
/* 1300 */ -1295,
/* 1301 */ 1089,
/* 1302 */ 1306,
/* 1303 */ 50319,
/* 1304 */ -1302,
/* 1305 */ 579,
/* 1306 */ 0,
/* 1307 */ -1302,
/* 1308 */ 1090,
/* 1309 */ 1316,
/* 1310 */ 50040,
/* 1311 */ 1319,
/* 1312 */ 223,
/* 1313 */ 50041,
/* 1314 */ -1309,
/* 1315 */ 583,
/* 1316 */ 0,
/* 1317 */ -1309,
/* 1318 */ 1091,
/* 1319 */ 1323,
/* 1320 */ 50322,
/* 1321 */ -1319,
/* 1322 */ 580,
/* 1323 */ 1327,
/* 1324 */ 50307,
/* 1325 */ -1319,
/* 1326 */ 581,
/* 1327 */ 0,
/* 1328 */ 50374,
/* 1329 */ -1319,
/* 1330 */ 582,
/* 1331 */ 1340,
/* 1332 */ 50288,
/* 1333 */ 1343,
/* 1334 */ 1350,
/* 1335 */ 1357,
/* 1336 */ 1364,
/* 1337 */ 1331,
/* 1338 */ -1331,
/* 1339 */ 591,
/* 1340 */ 0,
/* 1341 */ -1331,
/* 1342 */ 1096,
/* 1343 */ 1347,
/* 1344 */ 286,
/* 1345 */ -1343,
/* 1346 */ 584,
/* 1347 */ 0,
/* 1348 */ -1343,
/* 1349 */ 1092,
/* 1350 */ 1354,
/* 1351 */ 223,
/* 1352 */ -1350,
/* 1353 */ 585,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ 1093,
/* 1357 */ 1361,
/* 1358 */ 50319,
/* 1359 */ -1357,
/* 1360 */ 586,
/* 1361 */ 0,
/* 1362 */ -1357,
/* 1363 */ 1094,
/* 1364 */ 1371,
/* 1365 */ 50040,
/* 1366 */ 1374,
/* 1367 */ 223,
/* 1368 */ 50041,
/* 1369 */ -1364,
/* 1370 */ 590,
/* 1371 */ 0,
/* 1372 */ -1364,
/* 1373 */ 1095,
/* 1374 */ 1378,
/* 1375 */ 50322,
/* 1376 */ -1374,
/* 1377 */ 587,
/* 1378 */ 1382,
/* 1379 */ 50307,
/* 1380 */ -1374,
/* 1381 */ 588,
/* 1382 */ 0,
/* 1383 */ 50374,
/* 1384 */ -1374,
/* 1385 */ 589,
/* 1386 */ 1390,
/* 1387 */ 223,
/* 1388 */ -1386,
/* 1389 */ 592,
/* 1390 */ 0,
/* 1391 */ -1386,
/* 1392 */ 1097,
/* 1393 */ 1398,
/* 1394 */ 223,
/* 1395 */ 1404,
/* 1396 */ -1393,
/* 1397 */ 105,
/* 1398 */ 0,
/* 1399 */ 50126,
/* 1400 */ 223,
/* 1401 */ 1411,
/* 1402 */ -1393,
/* 1403 */ 106,
/* 1404 */ 1408,
/* 1405 */ 50044,
/* 1406 */ -1404,
/* 1407 */ 593,
/* 1408 */ 0,
/* 1409 */ -1404,
/* 1410 */ 1098,
/* 1411 */ 1415,
/* 1412 */ 50044,
/* 1413 */ -1411,
/* 1414 */ 594,
/* 1415 */ 0,
/* 1416 */ -1411,
/* 1417 */ 1099,
/* 1418 */ 0,
/* 1419 */ 50277,
/* 1420 */ 1424,
/* 1421 */ 223,
/* 1422 */ -1418,
/* 1423 */ 107,
/* 1424 */ 1428,
/* 1425 */ 50058,
/* 1426 */ -1424,
/* 1427 */ 595,
/* 1428 */ 0,
/* 1429 */ -1424,
/* 1430 */ 1100,
/* 1431 */ 0,
/* 1432 */ 50372,
/* 1433 */ 50257,
/* 1434 */ 50058,
/* 1435 */ 223,
/* 1436 */ 50123,
/* 1437 */ 1441,
/* 1438 */ 50125,
/* 1439 */ -1431,
/* 1440 */ 108,
/* 1441 */ 1446,
/* 1442 */ 1449,
/* 1443 */ 1441,
/* 1444 */ -1441,
/* 1445 */ 596,
/* 1446 */ 0,
/* 1447 */ -1441,
/* 1448 */ 1101,
/* 1449 */ 1456,
/* 1450 */ 50326,
/* 1451 */ 1715,
/* 1452 */ 50257,
/* 1453 */ 50059,
/* 1454 */ -1449,
/* 1455 */ 109,
/* 1456 */ 1463,
/* 1457 */ 50374,
/* 1458 */ 1722,
/* 1459 */ 173,
/* 1460 */ 50059,
/* 1461 */ -1449,
/* 1462 */ 110,
/* 1463 */ 1467,
/* 1464 */ 462,
/* 1465 */ -1449,
/* 1466 */ 111,
/* 1467 */ 1474,
/* 1468 */ 50430,
/* 1469 */ 1729,
/* 1470 */ 259,
/* 1471 */ 50059,
/* 1472 */ -1449,
/* 1473 */ 112,
/* 1474 */ 1481,
/* 1475 */ 50373,
/* 1476 */ 1736,
/* 1477 */ 259,
/* 1478 */ 50059,
/* 1479 */ -1449,
/* 1480 */ 113,
/* 1481 */ 1488,
/* 1482 */ 50316,
/* 1483 */ 1743,
/* 1484 */ 223,
/* 1485 */ 50059,
/* 1486 */ -1449,
/* 1487 */ 114,
/* 1488 */ 1495,
/* 1489 */ 50452,
/* 1490 */ 1750,
/* 1491 */ 223,
/* 1492 */ 50059,
/* 1493 */ -1449,
/* 1494 */ 115,
/* 1495 */ 1502,
/* 1496 */ 50284,
/* 1497 */ 1757,
/* 1498 */ 173,
/* 1499 */ 50059,
/* 1500 */ -1449,
/* 1501 */ 116,
/* 1502 */ 1509,
/* 1503 */ 50435,
/* 1504 */ 1764,
/* 1505 */ 173,
/* 1506 */ 50059,
/* 1507 */ -1449,
/* 1508 */ 117,
/* 1509 */ 1517,
/* 1510 */ 50435,
/* 1511 */ 1771,
/* 1512 */ 237,
/* 1513 */ 50037,
/* 1514 */ 50059,
/* 1515 */ -1449,
/* 1516 */ 118,
/* 1517 */ 1526,
/* 1518 */ 50435,
/* 1519 */ 1778,
/* 1520 */ 50043,
/* 1521 */ 237,
/* 1522 */ 50037,
/* 1523 */ 50059,
/* 1524 */ -1449,
/* 1525 */ 119,
/* 1526 */ 1533,
/* 1527 */ 50404,
/* 1528 */ 1785,
/* 1529 */ 173,
/* 1530 */ 50059,
/* 1531 */ -1449,
/* 1532 */ 120,
/* 1533 */ 1540,
/* 1534 */ 50420,
/* 1535 */ 1792,
/* 1536 */ 173,
/* 1537 */ 50059,
/* 1538 */ -1449,
/* 1539 */ 121,
/* 1540 */ 1547,
/* 1541 */ 50383,
/* 1542 */ 1799,
/* 1543 */ 223,
/* 1544 */ 50059,
/* 1545 */ -1449,
/* 1546 */ 122,
/* 1547 */ 1555,
/* 1548 */ 50315,
/* 1549 */ 1806,
/* 1550 */ 223,
/* 1551 */ 1813,
/* 1552 */ 50059,
/* 1553 */ -1449,
/* 1554 */ 123,
/* 1555 */ 1562,
/* 1556 */ 50466,
/* 1557 */ 1820,
/* 1558 */ 223,
/* 1559 */ 50059,
/* 1560 */ -1449,
/* 1561 */ 124,
/* 1562 */ 1569,
/* 1563 */ 50431,
/* 1564 */ 1827,
/* 1565 */ 223,
/* 1566 */ 50059,
/* 1567 */ -1449,
/* 1568 */ 125,
/* 1569 */ 1576,
/* 1570 */ 50396,
/* 1571 */ 1834,
/* 1572 */ 223,
/* 1573 */ 50059,
/* 1574 */ -1449,
/* 1575 */ 126,
/* 1576 */ 1583,
/* 1577 */ 50323,
/* 1578 */ 1841,
/* 1579 */ 223,
/* 1580 */ 50059,
/* 1581 */ -1449,
/* 1582 */ 127,
/* 1583 */ 1590,
/* 1584 */ 50312,
/* 1585 */ 1848,
/* 1586 */ 223,
/* 1587 */ 50059,
/* 1588 */ -1449,
/* 1589 */ 128,
/* 1590 */ 1597,
/* 1591 */ 50407,
/* 1592 */ 1855,
/* 1593 */ 223,
/* 1594 */ 50059,
/* 1595 */ -1449,
/* 1596 */ 129,
/* 1597 */ 1604,
/* 1598 */ 50292,
/* 1599 */ 1862,
/* 1600 */ 1869,
/* 1601 */ 50059,
/* 1602 */ -1449,
/* 1603 */ 130,
/* 1604 */ 1611,
/* 1605 */ 50365,
/* 1606 */ 1877,
/* 1607 */ 223,
/* 1608 */ 50059,
/* 1609 */ -1449,
/* 1610 */ 131,
/* 1611 */ 1618,
/* 1612 */ 50437,
/* 1613 */ 1884,
/* 1614 */ 1891,
/* 1615 */ 50059,
/* 1616 */ -1449,
/* 1617 */ 132,
/* 1618 */ 1625,
/* 1619 */ 50349,
/* 1620 */ 1907,
/* 1621 */ 1914,
/* 1622 */ 50059,
/* 1623 */ -1449,
/* 1624 */ 133,
/* 1625 */ 1632,
/* 1626 */ 50350,
/* 1627 */ 1930,
/* 1628 */ 1937,
/* 1629 */ 50059,
/* 1630 */ -1449,
/* 1631 */ 134,
/* 1632 */ 1639,
/* 1633 */ 50361,
/* 1634 */ 1945,
/* 1635 */ 1952,
/* 1636 */ 50059,
/* 1637 */ -1449,
/* 1638 */ 135,
/* 1639 */ 1646,
/* 1640 */ 50304,
/* 1641 */ 1972,
/* 1642 */ 223,
/* 1643 */ 50059,
/* 1644 */ -1449,
/* 1645 */ 136,
/* 1646 */ 1653,
/* 1647 */ 50468,
/* 1648 */ 1979,
/* 1649 */ 223,
/* 1650 */ 50059,
/* 1651 */ -1449,
/* 1652 */ 137,
/* 1653 */ 1660,
/* 1654 */ 50467,
/* 1655 */ 1986,
/* 1656 */ 223,
/* 1657 */ 50059,
/* 1658 */ -1449,
/* 1659 */ 138,
/* 1660 */ 1667,
/* 1661 */ 50379,
/* 1662 */ 1993,
/* 1663 */ 223,
/* 1664 */ 50059,
/* 1665 */ -1449,
/* 1666 */ 139,
/* 1667 */ 1674,
/* 1668 */ 50456,
/* 1669 */ 2000,
/* 1670 */ 223,
/* 1671 */ 50059,
/* 1672 */ -1449,
/* 1673 */ 140,
/* 1674 */ 1681,
/* 1675 */ 50313,
/* 1676 */ 2007,
/* 1677 */ 223,
/* 1678 */ 50059,
/* 1679 */ -1449,
/* 1680 */ 141,
/* 1681 */ 1689,
/* 1682 */ 50376,
/* 1683 */ 50420,
/* 1684 */ 2014,
/* 1685 */ 223,
/* 1686 */ 50059,
/* 1687 */ -1449,
/* 1688 */ 142,
/* 1689 */ 1696,
/* 1690 */ 50375,
/* 1691 */ 2021,
/* 1692 */ 223,
/* 1693 */ 50059,
/* 1694 */ -1449,
/* 1695 */ 143,
/* 1696 */ 1703,
/* 1697 */ 50355,
/* 1698 */ 2028,
/* 1699 */ 163,
/* 1700 */ 50059,
/* 1701 */ -1449,
/* 1702 */ 144,
/* 1703 */ 1707,
/* 1704 */ 3423,
/* 1705 */ -1449,
/* 1706 */ 145,
/* 1707 */ 1711,
/* 1708 */ 538,
/* 1709 */ -1449,
/* 1710 */ 146,
/* 1711 */ 0,
/* 1712 */ 6098,
/* 1713 */ -1449,
/* 1714 */ 147,
/* 1715 */ 1719,
/* 1716 */ 50058,
/* 1717 */ -1715,
/* 1718 */ 597,
/* 1719 */ 0,
/* 1720 */ -1715,
/* 1721 */ 1102,
/* 1722 */ 1726,
/* 1723 */ 50058,
/* 1724 */ -1722,
/* 1725 */ 598,
/* 1726 */ 0,
/* 1727 */ -1722,
/* 1728 */ 1103,
/* 1729 */ 1733,
/* 1730 */ 50058,
/* 1731 */ -1729,
/* 1732 */ 599,
/* 1733 */ 0,
/* 1734 */ -1729,
/* 1735 */ 1104,
/* 1736 */ 1740,
/* 1737 */ 50058,
/* 1738 */ -1736,
/* 1739 */ 600,
/* 1740 */ 0,
/* 1741 */ -1736,
/* 1742 */ 1105,
/* 1743 */ 1747,
/* 1744 */ 50058,
/* 1745 */ -1743,
/* 1746 */ 601,
/* 1747 */ 0,
/* 1748 */ -1743,
/* 1749 */ 1106,
/* 1750 */ 1754,
/* 1751 */ 50058,
/* 1752 */ -1750,
/* 1753 */ 602,
/* 1754 */ 0,
/* 1755 */ -1750,
/* 1756 */ 1107,
/* 1757 */ 1761,
/* 1758 */ 50058,
/* 1759 */ -1757,
/* 1760 */ 603,
/* 1761 */ 0,
/* 1762 */ -1757,
/* 1763 */ 1108,
/* 1764 */ 1768,
/* 1765 */ 50058,
/* 1766 */ -1764,
/* 1767 */ 604,
/* 1768 */ 0,
/* 1769 */ -1764,
/* 1770 */ 1109,
/* 1771 */ 1775,
/* 1772 */ 50058,
/* 1773 */ -1771,
/* 1774 */ 605,
/* 1775 */ 0,
/* 1776 */ -1771,
/* 1777 */ 1110,
/* 1778 */ 1782,
/* 1779 */ 50058,
/* 1780 */ -1778,
/* 1781 */ 606,
/* 1782 */ 0,
/* 1783 */ -1778,
/* 1784 */ 1111,
/* 1785 */ 1789,
/* 1786 */ 50058,
/* 1787 */ -1785,
/* 1788 */ 607,
/* 1789 */ 0,
/* 1790 */ -1785,
/* 1791 */ 1112,
/* 1792 */ 1796,
/* 1793 */ 50058,
/* 1794 */ -1792,
/* 1795 */ 608,
/* 1796 */ 0,
/* 1797 */ -1792,
/* 1798 */ 1113,
/* 1799 */ 1803,
/* 1800 */ 50058,
/* 1801 */ -1799,
/* 1802 */ 609,
/* 1803 */ 0,
/* 1804 */ -1799,
/* 1805 */ 1114,
/* 1806 */ 1810,
/* 1807 */ 50058,
/* 1808 */ -1806,
/* 1809 */ 610,
/* 1810 */ 0,
/* 1811 */ -1806,
/* 1812 */ 1115,
/* 1813 */ 1817,
/* 1814 */ 223,
/* 1815 */ -1813,
/* 1816 */ 611,
/* 1817 */ 0,
/* 1818 */ -1813,
/* 1819 */ 1116,
/* 1820 */ 1824,
/* 1821 */ 50058,
/* 1822 */ -1820,
/* 1823 */ 612,
/* 1824 */ 0,
/* 1825 */ -1820,
/* 1826 */ 1117,
/* 1827 */ 1831,
/* 1828 */ 50058,
/* 1829 */ -1827,
/* 1830 */ 613,
/* 1831 */ 0,
/* 1832 */ -1827,
/* 1833 */ 1118,
/* 1834 */ 1838,
/* 1835 */ 50058,
/* 1836 */ -1834,
/* 1837 */ 614,
/* 1838 */ 0,
/* 1839 */ -1834,
/* 1840 */ 1119,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ 615,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ 1120,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ 616,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ 1121,
/* 1855 */ 1859,
/* 1856 */ 50058,
/* 1857 */ -1855,
/* 1858 */ 617,
/* 1859 */ 0,
/* 1860 */ -1855,
/* 1861 */ 1122,
/* 1862 */ 1866,
/* 1863 */ 50058,
/* 1864 */ -1862,
/* 1865 */ 618,
/* 1866 */ 0,
/* 1867 */ -1862,
/* 1868 */ 1123,
/* 1869 */ 1874,
/* 1870 */ 2035,
/* 1871 */ 1869,
/* 1872 */ -1869,
/* 1873 */ 619,
/* 1874 */ 0,
/* 1875 */ -1869,
/* 1876 */ 1124,
/* 1877 */ 1881,
/* 1878 */ 50058,
/* 1879 */ -1877,
/* 1880 */ 620,
/* 1881 */ 0,
/* 1882 */ -1877,
/* 1883 */ 1125,
/* 1884 */ 1888,
/* 1885 */ 50058,
/* 1886 */ -1884,
/* 1887 */ 621,
/* 1888 */ 0,
/* 1889 */ -1884,
/* 1890 */ 1126,
/* 1891 */ 1897,
/* 1892 */ 163,
/* 1893 */ 1900,
/* 1894 */ 1891,
/* 1895 */ -1891,
/* 1896 */ 623,
/* 1897 */ 0,
/* 1898 */ -1891,
/* 1899 */ 1128,
/* 1900 */ 1904,
/* 1901 */ 50044,
/* 1902 */ -1900,
/* 1903 */ 622,
/* 1904 */ 0,
/* 1905 */ -1900,
/* 1906 */ 1127,
/* 1907 */ 1911,
/* 1908 */ 50058,
/* 1909 */ -1907,
/* 1910 */ 624,
/* 1911 */ 0,
/* 1912 */ -1907,
/* 1913 */ 1129,
/* 1914 */ 1920,
/* 1915 */ 163,
/* 1916 */ 1923,
/* 1917 */ 1914,
/* 1918 */ -1914,
/* 1919 */ 626,
/* 1920 */ 0,
/* 1921 */ -1914,
/* 1922 */ 1131,
/* 1923 */ 1927,
/* 1924 */ 50044,
/* 1925 */ -1923,
/* 1926 */ 625,
/* 1927 */ 0,
/* 1928 */ -1923,
/* 1929 */ 1130,
/* 1930 */ 1934,
/* 1931 */ 50058,
/* 1932 */ -1930,
/* 1933 */ 627,
/* 1934 */ 0,
/* 1935 */ -1930,
/* 1936 */ 1132,
/* 1937 */ 1942,
/* 1938 */ 2060,
/* 1939 */ 1937,
/* 1940 */ -1937,
/* 1941 */ 628,
/* 1942 */ 0,
/* 1943 */ -1937,
/* 1944 */ 1133,
/* 1945 */ 1949,
/* 1946 */ 50058,
/* 1947 */ -1945,
/* 1948 */ 629,
/* 1949 */ 0,
/* 1950 */ -1945,
/* 1951 */ 1134,
/* 1952 */ 1958,
/* 1953 */ 223,
/* 1954 */ 1961,
/* 1955 */ 1952,
/* 1956 */ -1952,
/* 1957 */ 632,
/* 1958 */ 0,
/* 1959 */ -1952,
/* 1960 */ 1136,
/* 1961 */ 1965,
/* 1962 */ 50044,
/* 1963 */ -1961,
/* 1964 */ 630,
/* 1965 */ 1969,
/* 1966 */ 50124,
/* 1967 */ -1961,
/* 1968 */ 631,
/* 1969 */ 0,
/* 1970 */ -1961,
/* 1971 */ 1135,
/* 1972 */ 1976,
/* 1973 */ 50058,
/* 1974 */ -1972,
/* 1975 */ 633,
/* 1976 */ 0,
/* 1977 */ -1972,
/* 1978 */ 1137,
/* 1979 */ 1983,
/* 1980 */ 50058,
/* 1981 */ -1979,
/* 1982 */ 634,
/* 1983 */ 0,
/* 1984 */ -1979,
/* 1985 */ 1138,
/* 1986 */ 1990,
/* 1987 */ 50058,
/* 1988 */ -1986,
/* 1989 */ 635,
/* 1990 */ 0,
/* 1991 */ -1986,
/* 1992 */ 1139,
/* 1993 */ 1997,
/* 1994 */ 50058,
/* 1995 */ -1993,
/* 1996 */ 636,
/* 1997 */ 0,
/* 1998 */ -1993,
/* 1999 */ 1140,
/* 2000 */ 2004,
/* 2001 */ 50058,
/* 2002 */ -2000,
/* 2003 */ 637,
/* 2004 */ 0,
/* 2005 */ -2000,
/* 2006 */ 1141,
/* 2007 */ 2011,
/* 2008 */ 50058,
/* 2009 */ -2007,
/* 2010 */ 638,
/* 2011 */ 0,
/* 2012 */ -2007,
/* 2013 */ 1142,
/* 2014 */ 2018,
/* 2015 */ 50058,
/* 2016 */ -2014,
/* 2017 */ 639,
/* 2018 */ 0,
/* 2019 */ -2014,
/* 2020 */ 1143,
/* 2021 */ 2025,
/* 2022 */ 50058,
/* 2023 */ -2021,
/* 2024 */ 640,
/* 2025 */ 0,
/* 2026 */ -2021,
/* 2027 */ 1144,
/* 2028 */ 2032,
/* 2029 */ 50058,
/* 2030 */ -2028,
/* 2031 */ 641,
/* 2032 */ 0,
/* 2033 */ -2028,
/* 2034 */ 1145,
/* 2035 */ 2040,
/* 2036 */ 223,
/* 2037 */ 2046,
/* 2038 */ -2035,
/* 2039 */ 148,
/* 2040 */ 0,
/* 2041 */ 50276,
/* 2042 */ 223,
/* 2043 */ 2053,
/* 2044 */ -2035,
/* 2045 */ 149,
/* 2046 */ 2050,
/* 2047 */ 50044,
/* 2048 */ -2046,
/* 2049 */ 642,
/* 2050 */ 0,
/* 2051 */ -2046,
/* 2052 */ 1146,
/* 2053 */ 2057,
/* 2054 */ 50044,
/* 2055 */ -2053,
/* 2056 */ 643,
/* 2057 */ 0,
/* 2058 */ -2053,
/* 2059 */ 1147,
/* 2060 */ 2065,
/* 2061 */ 223,
/* 2062 */ 2071,
/* 2063 */ -2060,
/* 2064 */ 150,
/* 2065 */ 0,
/* 2066 */ 50126,
/* 2067 */ 223,
/* 2068 */ 2078,
/* 2069 */ -2060,
/* 2070 */ 151,
/* 2071 */ 2075,
/* 2072 */ 50044,
/* 2073 */ -2071,
/* 2074 */ 644,
/* 2075 */ 0,
/* 2076 */ -2071,
/* 2077 */ 1148,
/* 2078 */ 2082,
/* 2079 */ 50044,
/* 2080 */ -2078,
/* 2081 */ 645,
/* 2082 */ 0,
/* 2083 */ -2078,
/* 2084 */ 1149,
/* 2085 */ 0,
/* 2086 */ 50348,
/* 2087 */ 50257,
/* 2088 */ 50058,
/* 2089 */ 223,
/* 2090 */ 50123,
/* 2091 */ 2095,
/* 2092 */ 50125,
/* 2093 */ -2085,
/* 2094 */ 152,
/* 2095 */ 2100,
/* 2096 */ 2103,
/* 2097 */ 2095,
/* 2098 */ -2095,
/* 2099 */ 646,
/* 2100 */ 0,
/* 2101 */ -2095,
/* 2102 */ 1150,
/* 2103 */ 2110,
/* 2104 */ 50326,
/* 2105 */ 2176,
/* 2106 */ 50257,
/* 2107 */ 50059,
/* 2108 */ -2103,
/* 2109 */ 153,
/* 2110 */ 2117,
/* 2111 */ 50383,
/* 2112 */ 2183,
/* 2113 */ 223,
/* 2114 */ 50059,
/* 2115 */ -2103,
/* 2116 */ 154,
/* 2117 */ 2121,
/* 2118 */ 462,
/* 2119 */ -2103,
/* 2120 */ 155,
/* 2121 */ 2129,
/* 2122 */ 50389,
/* 2123 */ 2190,
/* 2124 */ 223,
/* 2125 */ 50037,
/* 2126 */ 50059,
/* 2127 */ -2103,
/* 2128 */ 156,
/* 2129 */ 2136,
/* 2130 */ 50365,
/* 2131 */ 2197,
/* 2132 */ 223,
/* 2133 */ 50059,
/* 2134 */ -2103,
/* 2135 */ 157,
/* 2136 */ 2143,
/* 2137 */ 50350,
/* 2138 */ 2204,
/* 2139 */ 2211,
/* 2140 */ 50059,
/* 2141 */ -2103,
/* 2142 */ 158,
/* 2143 */ 2150,
/* 2144 */ 2231,
/* 2145 */ 2239,
/* 2146 */ 163,
/* 2147 */ 50059,
/* 2148 */ -2103,
/* 2149 */ 159,
/* 2150 */ 2157,
/* 2151 */ 50472,
/* 2152 */ 2246,
/* 2153 */ 173,
/* 2154 */ 50059,
/* 2155 */ -2103,
/* 2156 */ 160,
/* 2157 */ 2164,
/* 2158 */ 50344,
/* 2159 */ 2253,
/* 2160 */ 259,
/* 2161 */ 50059,
/* 2162 */ -2103,
/* 2163 */ 161,
/* 2164 */ 2168,
/* 2165 */ 538,
/* 2166 */ -2103,
/* 2167 */ 162,
/* 2168 */ 2172,
/* 2169 */ 3423,
/* 2170 */ -2103,
/* 2171 */ 163,
/* 2172 */ 0,
/* 2173 */ 6098,
/* 2174 */ -2103,
/* 2175 */ 164,
/* 2176 */ 2180,
/* 2177 */ 50058,
/* 2178 */ -2176,
/* 2179 */ 647,
/* 2180 */ 0,
/* 2181 */ -2176,
/* 2182 */ 1151,
/* 2183 */ 2187,
/* 2184 */ 50058,
/* 2185 */ -2183,
/* 2186 */ 648,
/* 2187 */ 0,
/* 2188 */ -2183,
/* 2189 */ 1152,
/* 2190 */ 2194,
/* 2191 */ 50058,
/* 2192 */ -2190,
/* 2193 */ 649,
/* 2194 */ 0,
/* 2195 */ -2190,
/* 2196 */ 1153,
/* 2197 */ 2201,
/* 2198 */ 50058,
/* 2199 */ -2197,
/* 2200 */ 650,
/* 2201 */ 0,
/* 2202 */ -2197,
/* 2203 */ 1154,
/* 2204 */ 2208,
/* 2205 */ 50058,
/* 2206 */ -2204,
/* 2207 */ 651,
/* 2208 */ 0,
/* 2209 */ -2204,
/* 2210 */ 1155,
/* 2211 */ 2217,
/* 2212 */ 223,
/* 2213 */ 2220,
/* 2214 */ 2211,
/* 2215 */ -2211,
/* 2216 */ 654,
/* 2217 */ 0,
/* 2218 */ -2211,
/* 2219 */ 1157,
/* 2220 */ 2224,
/* 2221 */ 50124,
/* 2222 */ -2220,
/* 2223 */ 652,
/* 2224 */ 2228,
/* 2225 */ 50044,
/* 2226 */ -2220,
/* 2227 */ 653,
/* 2228 */ 0,
/* 2229 */ -2220,
/* 2230 */ 1156,
/* 2231 */ 2235,
/* 2232 */ 50448,
/* 2233 */ -2231,
/* 2234 */ 655,
/* 2235 */ 0,
/* 2236 */ 50334,
/* 2237 */ -2231,
/* 2238 */ 656,
/* 2239 */ 2243,
/* 2240 */ 50058,
/* 2241 */ -2239,
/* 2242 */ 657,
/* 2243 */ 0,
/* 2244 */ -2239,
/* 2245 */ 1158,
/* 2246 */ 2250,
/* 2247 */ 50058,
/* 2248 */ -2246,
/* 2249 */ 658,
/* 2250 */ 0,
/* 2251 */ -2246,
/* 2252 */ 1159,
/* 2253 */ 2257,
/* 2254 */ 50058,
/* 2255 */ -2253,
/* 2256 */ 659,
/* 2257 */ 0,
/* 2258 */ -2253,
/* 2259 */ 1160,
/* 2260 */ 0,
/* 2261 */ 2272,
/* 2262 */ 2288,
/* 2263 */ 50257,
/* 2264 */ 50058,
/* 2265 */ 223,
/* 2266 */ 50123,
/* 2267 */ 2304,
/* 2268 */ 50125,
/* 2269 */ 2312,
/* 2270 */ -2260,
/* 2271 */ 165,
/* 2272 */ 2278,
/* 2273 */ 223,
/* 2274 */ 2281,
/* 2275 */ 2272,
/* 2276 */ -2272,
/* 2277 */ 661,
/* 2278 */ 0,
/* 2279 */ -2272,
/* 2280 */ 1162,
/* 2281 */ 2285,
/* 2282 */ 50047,
/* 2283 */ -2281,
/* 2284 */ 660,
/* 2285 */ 0,
/* 2286 */ -2281,
/* 2287 */ 1161,
/* 2288 */ 2292,
/* 2289 */ 50436,
/* 2290 */ -2288,
/* 2291 */ 662,
/* 2292 */ 2296,
/* 2293 */ 50334,
/* 2294 */ -2288,
/* 2295 */ 663,
/* 2296 */ 2300,
/* 2297 */ 50409,
/* 2298 */ -2288,
/* 2299 */ 664,
/* 2300 */ 0,
/* 2301 */ 50330,
/* 2302 */ -2288,
/* 2303 */ 665,
/* 2304 */ 2309,
/* 2305 */ 2332,
/* 2306 */ 2304,
/* 2307 */ -2304,
/* 2308 */ 666,
/* 2309 */ 0,
/* 2310 */ -2304,
/* 2311 */ 1163,
/* 2312 */ 2321,
/* 2313 */ 50288,
/* 2314 */ 223,
/* 2315 */ 50123,
/* 2316 */ 2324,
/* 2317 */ 50125,
/* 2318 */ 2312,
/* 2319 */ -2312,
/* 2320 */ 668,
/* 2321 */ 0,
/* 2322 */ -2312,
/* 2323 */ 1165,
/* 2324 */ 2329,
/* 2325 */ 2332,
/* 2326 */ 2324,
/* 2327 */ -2324,
/* 2328 */ 667,
/* 2329 */ 0,
/* 2330 */ -2324,
/* 2331 */ 1164,
/* 2332 */ 2339,
/* 2333 */ 50326,
/* 2334 */ 2486,
/* 2335 */ 50257,
/* 2336 */ 50059,
/* 2337 */ -2332,
/* 2338 */ 166,
/* 2339 */ 2344,
/* 2340 */ 173,
/* 2341 */ 50059,
/* 2342 */ -2332,
/* 2343 */ 167,
/* 2344 */ 2351,
/* 2345 */ 50374,
/* 2346 */ 2493,
/* 2347 */ 223,
/* 2348 */ 50059,
/* 2349 */ -2332,
/* 2350 */ 168,
/* 2351 */ 2358,
/* 2352 */ 50315,
/* 2353 */ 2500,
/* 2354 */ 223,
/* 2355 */ 50059,
/* 2356 */ -2332,
/* 2357 */ 169,
/* 2358 */ 2367,
/* 2359 */ 50298,
/* 2360 */ 2507,
/* 2361 */ 2514,
/* 2362 */ 223,
/* 2363 */ 2521,
/* 2364 */ 50059,
/* 2365 */ -2332,
/* 2366 */ 170,
/* 2367 */ 2375,
/* 2368 */ 50413,
/* 2369 */ 2528,
/* 2370 */ 173,
/* 2371 */ 2535,
/* 2372 */ 50059,
/* 2373 */ -2332,
/* 2374 */ 171,
/* 2375 */ 2383,
/* 2376 */ 50445,
/* 2377 */ 2544,
/* 2378 */ 173,
/* 2379 */ 2551,
/* 2380 */ 50059,
/* 2381 */ -2332,
/* 2382 */ 172,
/* 2383 */ 2390,
/* 2384 */ 50417,
/* 2385 */ 2558,
/* 2386 */ 173,
/* 2387 */ 50059,
/* 2388 */ -2332,
/* 2389 */ 173,
/* 2390 */ 2397,
/* 2391 */ 50472,
/* 2392 */ 2565,
/* 2393 */ 173,
/* 2394 */ 50059,
/* 2395 */ -2332,
/* 2396 */ 174,
/* 2397 */ 2404,
/* 2398 */ 50473,
/* 2399 */ 2572,
/* 2400 */ 173,
/* 2401 */ 50059,
/* 2402 */ -2332,
/* 2403 */ 175,
/* 2404 */ 2411,
/* 2405 */ 50333,
/* 2406 */ 2579,
/* 2407 */ 173,
/* 2408 */ 50059,
/* 2409 */ -2332,
/* 2410 */ 176,
/* 2411 */ 2418,
/* 2412 */ 50414,
/* 2413 */ 2586,
/* 2414 */ 259,
/* 2415 */ 50059,
/* 2416 */ -2332,
/* 2417 */ 177,
/* 2418 */ 2425,
/* 2419 */ 50297,
/* 2420 */ 2593,
/* 2421 */ 173,
/* 2422 */ 50059,
/* 2423 */ -2332,
/* 2424 */ 178,
/* 2425 */ 2432,
/* 2426 */ 50378,
/* 2427 */ 2600,
/* 2428 */ 173,
/* 2429 */ 50059,
/* 2430 */ -2332,
/* 2431 */ 179,
/* 2432 */ 2439,
/* 2433 */ 50461,
/* 2434 */ 2607,
/* 2435 */ 173,
/* 2436 */ 50059,
/* 2437 */ -2332,
/* 2438 */ 180,
/* 2439 */ 2446,
/* 2440 */ 50428,
/* 2441 */ 2614,
/* 2442 */ 163,
/* 2443 */ 50059,
/* 2444 */ -2332,
/* 2445 */ 181,
/* 2446 */ 2453,
/* 2447 */ 50428,
/* 2448 */ 2621,
/* 2449 */ 173,
/* 2450 */ 50059,
/* 2451 */ -2332,
/* 2452 */ 182,
/* 2453 */ 2460,
/* 2454 */ 50318,
/* 2455 */ 2628,
/* 2456 */ 173,
/* 2457 */ 50059,
/* 2458 */ -2332,
/* 2459 */ 183,
/* 2460 */ 2467,
/* 2461 */ 50318,
/* 2462 */ 2635,
/* 2463 */ 50275,
/* 2464 */ 50059,
/* 2465 */ -2332,
/* 2466 */ 184,
/* 2467 */ 2474,
/* 2468 */ 50350,
/* 2469 */ 2642,
/* 2470 */ 2649,
/* 2471 */ 50059,
/* 2472 */ -2332,
/* 2473 */ 185,
/* 2474 */ 2478,
/* 2475 */ 538,
/* 2476 */ -2332,
/* 2477 */ 186,
/* 2478 */ 2482,
/* 2479 */ 6098,
/* 2480 */ -2332,
/* 2481 */ 187,
/* 2482 */ 0,
/* 2483 */ 3423,
/* 2484 */ -2332,
/* 2485 */ 188,
/* 2486 */ 2490,
/* 2487 */ 50058,
/* 2488 */ -2486,
/* 2489 */ 669,
/* 2490 */ 0,
/* 2491 */ -2486,
/* 2492 */ 1166,
/* 2493 */ 2497,
/* 2494 */ 50058,
/* 2495 */ -2493,
/* 2496 */ 670,
/* 2497 */ 0,
/* 2498 */ -2493,
/* 2499 */ 1167,
/* 2500 */ 2504,
/* 2501 */ 50058,
/* 2502 */ -2500,
/* 2503 */ 671,
/* 2504 */ 0,
/* 2505 */ -2500,
/* 2506 */ 1168,
/* 2507 */ 2511,
/* 2508 */ 50308,
/* 2509 */ -2507,
/* 2510 */ 672,
/* 2511 */ 0,
/* 2512 */ -2507,
/* 2513 */ 1169,
/* 2514 */ 2518,
/* 2515 */ 50058,
/* 2516 */ -2514,
/* 2517 */ 673,
/* 2518 */ 0,
/* 2519 */ -2514,
/* 2520 */ 1170,
/* 2521 */ 2525,
/* 2522 */ 50037,
/* 2523 */ -2521,
/* 2524 */ 674,
/* 2525 */ 0,
/* 2526 */ -2521,
/* 2527 */ 1171,
/* 2528 */ 2532,
/* 2529 */ 50058,
/* 2530 */ -2528,
/* 2531 */ 675,
/* 2532 */ 0,
/* 2533 */ -2528,
/* 2534 */ 1172,
/* 2535 */ 2541,
/* 2536 */ 50040,
/* 2537 */ 173,
/* 2538 */ 50041,
/* 2539 */ -2535,
/* 2540 */ 676,
/* 2541 */ 0,
/* 2542 */ -2535,
/* 2543 */ 1173,
/* 2544 */ 2548,
/* 2545 */ 50058,
/* 2546 */ -2544,
/* 2547 */ 677,
/* 2548 */ 0,
/* 2549 */ -2544,
/* 2550 */ 1174,
/* 2551 */ 2555,
/* 2552 */ 50405,
/* 2553 */ -2551,
/* 2554 */ 678,
/* 2555 */ 0,
/* 2556 */ -2551,
/* 2557 */ 1175,
/* 2558 */ 2562,
/* 2559 */ 50058,
/* 2560 */ -2558,
/* 2561 */ 679,
/* 2562 */ 0,
/* 2563 */ -2558,
/* 2564 */ 1176,
/* 2565 */ 2569,
/* 2566 */ 50058,
/* 2567 */ -2565,
/* 2568 */ 680,
/* 2569 */ 0,
/* 2570 */ -2565,
/* 2571 */ 1177,
/* 2572 */ 2576,
/* 2573 */ 50058,
/* 2574 */ -2572,
/* 2575 */ 681,
/* 2576 */ 0,
/* 2577 */ -2572,
/* 2578 */ 1178,
/* 2579 */ 2583,
/* 2580 */ 50058,
/* 2581 */ -2579,
/* 2582 */ 682,
/* 2583 */ 0,
/* 2584 */ -2579,
/* 2585 */ 1179,
/* 2586 */ 2590,
/* 2587 */ 50058,
/* 2588 */ -2586,
/* 2589 */ 683,
/* 2590 */ 0,
/* 2591 */ -2586,
/* 2592 */ 1180,
/* 2593 */ 2597,
/* 2594 */ 50058,
/* 2595 */ -2593,
/* 2596 */ 684,
/* 2597 */ 0,
/* 2598 */ -2593,
/* 2599 */ 1181,
/* 2600 */ 2604,
/* 2601 */ 50058,
/* 2602 */ -2600,
/* 2603 */ 685,
/* 2604 */ 0,
/* 2605 */ -2600,
/* 2606 */ 1182,
/* 2607 */ 2611,
/* 2608 */ 50058,
/* 2609 */ -2607,
/* 2610 */ 686,
/* 2611 */ 0,
/* 2612 */ -2607,
/* 2613 */ 1183,
/* 2614 */ 2618,
/* 2615 */ 50058,
/* 2616 */ -2614,
/* 2617 */ 687,
/* 2618 */ 0,
/* 2619 */ -2614,
/* 2620 */ 1184,
/* 2621 */ 2625,
/* 2622 */ 50058,
/* 2623 */ -2621,
/* 2624 */ 688,
/* 2625 */ 0,
/* 2626 */ -2621,
/* 2627 */ 1185,
/* 2628 */ 2632,
/* 2629 */ 50058,
/* 2630 */ -2628,
/* 2631 */ 689,
/* 2632 */ 0,
/* 2633 */ -2628,
/* 2634 */ 1186,
/* 2635 */ 2639,
/* 2636 */ 50058,
/* 2637 */ -2635,
/* 2638 */ 690,
/* 2639 */ 0,
/* 2640 */ -2635,
/* 2641 */ 1187,
/* 2642 */ 2646,
/* 2643 */ 50058,
/* 2644 */ -2642,
/* 2645 */ 691,
/* 2646 */ 0,
/* 2647 */ -2642,
/* 2648 */ 1188,
/* 2649 */ 2655,
/* 2650 */ 223,
/* 2651 */ 2658,
/* 2652 */ 2649,
/* 2653 */ -2649,
/* 2654 */ 694,
/* 2655 */ 0,
/* 2656 */ -2649,
/* 2657 */ 1190,
/* 2658 */ 2662,
/* 2659 */ 50044,
/* 2660 */ -2658,
/* 2661 */ 692,
/* 2662 */ 2666,
/* 2663 */ 50124,
/* 2664 */ -2658,
/* 2665 */ 693,
/* 2666 */ 0,
/* 2667 */ -2658,
/* 2668 */ 1189,
/* 2669 */ 0,
/* 2670 */ 2673,
/* 2671 */ -2669,
/* 2672 */ 189,
/* 2673 */ 2679,
/* 2674 */ 223,
/* 2675 */ 2682,
/* 2676 */ 2673,
/* 2677 */ -2673,
/* 2678 */ 697,
/* 2679 */ 0,
/* 2680 */ -2673,
/* 2681 */ 1192,
/* 2682 */ 2686,
/* 2683 */ 50044,
/* 2684 */ -2682,
/* 2685 */ 695,
/* 2686 */ 2690,
/* 2687 */ 50124,
/* 2688 */ -2682,
/* 2689 */ 696,
/* 2690 */ 0,
/* 2691 */ -2682,
/* 2692 */ 1191,
/* 2693 */ 0,
/* 2694 */ 50261,
/* 2695 */ -2693,
/* 2696 */ 190,
/* 2697 */ 0,
/* 2698 */ 50418,
/* 2699 */ 50257,
/* 2700 */ 2706,
/* 2701 */ 50123,
/* 2702 */ 2714,
/* 2703 */ 50125,
/* 2704 */ -2697,
/* 2705 */ 191,
/* 2706 */ 2711,
/* 2707 */ 50058,
/* 2708 */ 163,
/* 2709 */ -2706,
/* 2710 */ 698,
/* 2711 */ 0,
/* 2712 */ -2706,
/* 2713 */ 1193,
/* 2714 */ 2719,
/* 2715 */ 2722,
/* 2716 */ 2714,
/* 2717 */ -2714,
/* 2718 */ 699,
/* 2719 */ 0,
/* 2720 */ -2714,
/* 2721 */ 1194,
/* 2722 */ 2729,
/* 2723 */ 50326,
/* 2724 */ 2807,
/* 2725 */ 50257,
/* 2726 */ 50059,
/* 2727 */ -2722,
/* 2728 */ 192,
/* 2729 */ 2734,
/* 2730 */ 2814,
/* 2731 */ 50059,
/* 2732 */ -2722,
/* 2733 */ 193,
/* 2734 */ 2741,
/* 2735 */ 50359,
/* 2736 */ 2839,
/* 2737 */ 2846,
/* 2738 */ 50059,
/* 2739 */ -2722,
/* 2740 */ 194,
/* 2741 */ 2748,
/* 2742 */ 50388,
/* 2743 */ 2862,
/* 2744 */ 163,
/* 2745 */ 50059,
/* 2746 */ -2722,
/* 2747 */ 195,
/* 2748 */ 2758,
/* 2749 */ 50340,
/* 2750 */ 2869,
/* 2751 */ 2876,
/* 2752 */ 163,
/* 2753 */ 50044,
/* 2754 */ 163,
/* 2755 */ 50059,
/* 2756 */ -2722,
/* 2757 */ 196,
/* 2758 */ 2770,
/* 2759 */ 50340,
/* 2760 */ 2883,
/* 2761 */ 2890,
/* 2762 */ 163,
/* 2763 */ 50044,
/* 2764 */ 163,
/* 2765 */ 50044,
/* 2766 */ 163,
/* 2767 */ 50059,
/* 2768 */ -2722,
/* 2769 */ 197,
/* 2770 */ 2777,
/* 2771 */ 50432,
/* 2772 */ 2897,
/* 2773 */ 2904,
/* 2774 */ 50059,
/* 2775 */ -2722,
/* 2776 */ 198,
/* 2777 */ 2784,
/* 2778 */ 223,
/* 2779 */ 2920,
/* 2780 */ 50257,
/* 2781 */ 50059,
/* 2782 */ -2722,
/* 2783 */ 199,
/* 2784 */ 2791,
/* 2785 */ 50350,
/* 2786 */ 2927,
/* 2787 */ 2934,
/* 2788 */ 50059,
/* 2789 */ -2722,
/* 2790 */ 200,
/* 2791 */ 2795,
/* 2792 */ 433,
/* 2793 */ -2722,
/* 2794 */ 201,
/* 2795 */ 2799,
/* 2796 */ 3423,
/* 2797 */ -2722,
/* 2798 */ 202,
/* 2799 */ 2803,
/* 2800 */ 538,
/* 2801 */ -2722,
/* 2802 */ 203,
/* 2803 */ 0,
/* 2804 */ 6098,
/* 2805 */ -2722,
/* 2806 */ 204,
/* 2807 */ 2811,
/* 2808 */ 50058,
/* 2809 */ -2807,
/* 2810 */ 700,
/* 2811 */ 0,
/* 2812 */ -2807,
/* 2813 */ 1195,
/* 2814 */ 2822,
/* 2815 */ 50277,
/* 2816 */ 2825,
/* 2817 */ 173,
/* 2818 */ 2832,
/* 2819 */ 2814,
/* 2820 */ -2814,
/* 2821 */ 703,
/* 2822 */ 0,
/* 2823 */ -2814,
/* 2824 */ 1198,
/* 2825 */ 2829,
/* 2826 */ 50058,
/* 2827 */ -2825,
/* 2828 */ 701,
/* 2829 */ 0,
/* 2830 */ -2825,
/* 2831 */ 1196,
/* 2832 */ 2836,
/* 2833 */ 50044,
/* 2834 */ -2832,
/* 2835 */ 702,
/* 2836 */ 0,
/* 2837 */ -2832,
/* 2838 */ 1197,
/* 2839 */ 2843,
/* 2840 */ 50058,
/* 2841 */ -2839,
/* 2842 */ 704,
/* 2843 */ 0,
/* 2844 */ -2839,
/* 2845 */ 1199,
/* 2846 */ 2852,
/* 2847 */ 3257,
/* 2848 */ 2855,
/* 2849 */ 2846,
/* 2850 */ -2846,
/* 2851 */ 706,
/* 2852 */ 0,
/* 2853 */ -2846,
/* 2854 */ 1201,
/* 2855 */ 2859,
/* 2856 */ 50044,
/* 2857 */ -2855,
/* 2858 */ 705,
/* 2859 */ 0,
/* 2860 */ -2855,
/* 2861 */ 1200,
/* 2862 */ 2866,
/* 2863 */ 50058,
/* 2864 */ -2862,
/* 2865 */ 707,
/* 2866 */ 0,
/* 2867 */ -2862,
/* 2868 */ 1202,
/* 2869 */ 2873,
/* 2870 */ 50309,
/* 2871 */ -2869,
/* 2872 */ 708,
/* 2873 */ 0,
/* 2874 */ -2869,
/* 2875 */ 1203,
/* 2876 */ 2880,
/* 2877 */ 50058,
/* 2878 */ -2876,
/* 2879 */ 709,
/* 2880 */ 0,
/* 2881 */ -2876,
/* 2882 */ 1204,
/* 2883 */ 2887,
/* 2884 */ 50309,
/* 2885 */ -2883,
/* 2886 */ 710,
/* 2887 */ 0,
/* 2888 */ -2883,
/* 2889 */ 1205,
/* 2890 */ 2894,
/* 2891 */ 50058,
/* 2892 */ -2890,
/* 2893 */ 711,
/* 2894 */ 0,
/* 2895 */ -2890,
/* 2896 */ 1206,
/* 2897 */ 2901,
/* 2898 */ 50058,
/* 2899 */ -2897,
/* 2900 */ 712,
/* 2901 */ 0,
/* 2902 */ -2897,
/* 2903 */ 1207,
/* 2904 */ 2910,
/* 2905 */ 223,
/* 2906 */ 2913,
/* 2907 */ 2904,
/* 2908 */ -2904,
/* 2909 */ 714,
/* 2910 */ 0,
/* 2911 */ -2904,
/* 2912 */ 1209,
/* 2913 */ 2917,
/* 2914 */ 50044,
/* 2915 */ -2913,
/* 2916 */ 713,
/* 2917 */ 0,
/* 2918 */ -2913,
/* 2919 */ 1208,
/* 2920 */ 2924,
/* 2921 */ 50058,
/* 2922 */ -2920,
/* 2923 */ 715,
/* 2924 */ 0,
/* 2925 */ -2920,
/* 2926 */ 1210,
/* 2927 */ 2931,
/* 2928 */ 50058,
/* 2929 */ -2927,
/* 2930 */ 716,
/* 2931 */ 0,
/* 2932 */ -2927,
/* 2933 */ 1211,
/* 2934 */ 2940,
/* 2935 */ 223,
/* 2936 */ 2943,
/* 2937 */ 2934,
/* 2938 */ -2934,
/* 2939 */ 719,
/* 2940 */ 0,
/* 2941 */ -2934,
/* 2942 */ 1213,
/* 2943 */ 2947,
/* 2944 */ 50124,
/* 2945 */ -2943,
/* 2946 */ 717,
/* 2947 */ 2951,
/* 2948 */ 50044,
/* 2949 */ -2943,
/* 2950 */ 718,
/* 2951 */ 0,
/* 2952 */ -2943,
/* 2953 */ 1212,
/* 2954 */ 0,
/* 2955 */ 50309,
/* 2956 */ 50257,
/* 2957 */ 50123,
/* 2958 */ 2962,
/* 2959 */ 50125,
/* 2960 */ -2954,
/* 2961 */ 205,
/* 2962 */ 2967,
/* 2963 */ 2970,
/* 2964 */ 2962,
/* 2965 */ -2962,
/* 2966 */ 720,
/* 2967 */ 0,
/* 2968 */ -2962,
/* 2969 */ 1214,
/* 2970 */ 2977,
/* 2971 */ 50326,
/* 2972 */ 3063,
/* 2973 */ 50257,
/* 2974 */ 50059,
/* 2975 */ -2970,
/* 2976 */ 206,
/* 2977 */ 2985,
/* 2978 */ 50364,
/* 2979 */ 3070,
/* 2980 */ 3077,
/* 2981 */ 223,
/* 2982 */ 50059,
/* 2983 */ -2970,
/* 2984 */ 207,
/* 2985 */ 2993,
/* 2986 */ 50381,
/* 2987 */ 3084,
/* 2988 */ 3091,
/* 2989 */ 223,
/* 2990 */ 50059,
/* 2991 */ -2970,
/* 2992 */ 208,
/* 2993 */ 3002,
/* 2994 */ 50323,
/* 2995 */ 3098,
/* 2996 */ 223,
/* 2997 */ 50047,
/* 2998 */ 223,
/* 2999 */ 50059,
/* 3000 */ -2970,
/* 3001 */ 209,
/* 3002 */ 3009,
/* 3003 */ 50296,
/* 3004 */ 3105,
/* 3005 */ 3112,
/* 3006 */ 50059,
/* 3007 */ -2970,
/* 3008 */ 210,
/* 3009 */ 3016,
/* 3010 */ 50429,
/* 3011 */ 3129,
/* 3012 */ 3136,
/* 3013 */ 50059,
/* 3014 */ -2970,
/* 3015 */ 211,
/* 3016 */ 3023,
/* 3017 */ 50359,
/* 3018 */ 3153,
/* 3019 */ 3160,
/* 3020 */ 50059,
/* 3021 */ -2970,
/* 3022 */ 212,
/* 3023 */ 3033,
/* 3024 */ 50432,
/* 3025 */ 3176,
/* 3026 */ 223,
/* 3027 */ 3180,
/* 3028 */ 3184,
/* 3029 */ 3191,
/* 3030 */ 50059,
/* 3031 */ -2970,
/* 3032 */ 213,
/* 3033 */ 3040,
/* 3034 */ 50411,
/* 3035 */ 3207,
/* 3036 */ 3214,
/* 3037 */ 50059,
/* 3038 */ -2970,
/* 3039 */ 214,
/* 3040 */ 3047,
/* 3041 */ 50350,
/* 3042 */ 3230,
/* 3043 */ 3237,
/* 3044 */ 50059,
/* 3045 */ -2970,
/* 3046 */ 215,
/* 3047 */ 3051,
/* 3048 */ 433,
/* 3049 */ -2970,
/* 3050 */ 216,
/* 3051 */ 3055,
/* 3052 */ 538,
/* 3053 */ -2970,
/* 3054 */ 217,
/* 3055 */ 3059,
/* 3056 */ 3423,
/* 3057 */ -2970,
/* 3058 */ 218,
/* 3059 */ 0,
/* 3060 */ 6098,
/* 3061 */ -2970,
/* 3062 */ 219,
/* 3063 */ 3067,
/* 3064 */ 50058,
/* 3065 */ -3063,
/* 3066 */ 721,
/* 3067 */ 0,
/* 3068 */ -3063,
/* 3069 */ 1215,
/* 3070 */ 3074,
/* 3071 */ 50058,
/* 3072 */ -3070,
/* 3073 */ 722,
/* 3074 */ 0,
/* 3075 */ -3070,
/* 3076 */ 1216,
/* 3077 */ 3081,
/* 3078 */ 50100,
/* 3079 */ -3077,
/* 3080 */ 723,
/* 3081 */ 0,
/* 3082 */ -3077,
/* 3083 */ 1217,
/* 3084 */ 3088,
/* 3085 */ 50058,
/* 3086 */ -3084,
/* 3087 */ 724,
/* 3088 */ 0,
/* 3089 */ -3084,
/* 3090 */ 1218,
/* 3091 */ 3095,
/* 3092 */ 50100,
/* 3093 */ -3091,
/* 3094 */ 725,
/* 3095 */ 0,
/* 3096 */ -3091,
/* 3097 */ 1219,
/* 3098 */ 3102,
/* 3099 */ 50058,
/* 3100 */ -3098,
/* 3101 */ 726,
/* 3102 */ 0,
/* 3103 */ -3098,
/* 3104 */ 1220,
/* 3105 */ 3109,
/* 3106 */ 50058,
/* 3107 */ -3105,
/* 3108 */ 727,
/* 3109 */ 0,
/* 3110 */ -3105,
/* 3111 */ 1221,
/* 3112 */ 3119,
/* 3113 */ 223,
/* 3114 */ 223,
/* 3115 */ 3122,
/* 3116 */ 3112,
/* 3117 */ -3112,
/* 3118 */ 729,
/* 3119 */ 0,
/* 3120 */ -3112,
/* 3121 */ 1223,
/* 3122 */ 3126,
/* 3123 */ 50044,
/* 3124 */ -3122,
/* 3125 */ 728,
/* 3126 */ 0,
/* 3127 */ -3122,
/* 3128 */ 1222,
/* 3129 */ 3133,
/* 3130 */ 50058,
/* 3131 */ -3129,
/* 3132 */ 730,
/* 3133 */ 0,
/* 3134 */ -3129,
/* 3135 */ 1224,
/* 3136 */ 3143,
/* 3137 */ 223,
/* 3138 */ 223,
/* 3139 */ 3146,
/* 3140 */ 3136,
/* 3141 */ -3136,
/* 3142 */ 732,
/* 3143 */ 0,
/* 3144 */ -3136,
/* 3145 */ 1226,
/* 3146 */ 3150,
/* 3147 */ 50044,
/* 3148 */ -3146,
/* 3149 */ 731,
/* 3150 */ 0,
/* 3151 */ -3146,
/* 3152 */ 1225,
/* 3153 */ 3157,
/* 3154 */ 50058,
/* 3155 */ -3153,
/* 3156 */ 733,
/* 3157 */ 0,
/* 3158 */ -3153,
/* 3159 */ 1227,
/* 3160 */ 3166,
/* 3161 */ 3257,
/* 3162 */ 3169,
/* 3163 */ 3160,
/* 3164 */ -3160,
/* 3165 */ 735,
/* 3166 */ 0,
/* 3167 */ -3160,
/* 3168 */ 1229,
/* 3169 */ 3173,
/* 3170 */ 50044,
/* 3171 */ -3169,
/* 3172 */ 734,
/* 3173 */ 0,
/* 3174 */ -3169,
/* 3175 */ 1228,
/* 3176 */ 0,
/* 3177 */ 50091,
/* 3178 */ -3176,
/* 3179 */ 736,
/* 3180 */ 0,
/* 3181 */ 50093,
/* 3182 */ -3180,
/* 3183 */ 737,
/* 3184 */ 3188,
/* 3185 */ 50058,
/* 3186 */ -3184,
/* 3187 */ 738,
/* 3188 */ 0,
/* 3189 */ -3184,
/* 3190 */ 1230,
/* 3191 */ 3197,
/* 3192 */ 223,
/* 3193 */ 3200,
/* 3194 */ 3191,
/* 3195 */ -3191,
/* 3196 */ 740,
/* 3197 */ 0,
/* 3198 */ -3191,
/* 3199 */ 1232,
/* 3200 */ 3204,
/* 3201 */ 50044,
/* 3202 */ -3200,
/* 3203 */ 739,
/* 3204 */ 0,
/* 3205 */ -3200,
/* 3206 */ 1231,
/* 3207 */ 3211,
/* 3208 */ 50058,
/* 3209 */ -3207,
/* 3210 */ 741,
/* 3211 */ 0,
/* 3212 */ -3207,
/* 3213 */ 1233,
/* 3214 */ 3220,
/* 3215 */ 223,
/* 3216 */ 3223,
/* 3217 */ 3214,
/* 3218 */ -3214,
/* 3219 */ 743,
/* 3220 */ 0,
/* 3221 */ -3214,
/* 3222 */ 1235,
/* 3223 */ 3227,
/* 3224 */ 50044,
/* 3225 */ -3223,
/* 3226 */ 742,
/* 3227 */ 0,
/* 3228 */ -3223,
/* 3229 */ 1234,
/* 3230 */ 3234,
/* 3231 */ 50058,
/* 3232 */ -3230,
/* 3233 */ 744,
/* 3234 */ 0,
/* 3235 */ -3230,
/* 3236 */ 1236,
/* 3237 */ 3243,
/* 3238 */ 223,
/* 3239 */ 3246,
/* 3240 */ 3237,
/* 3241 */ -3237,
/* 3242 */ 747,
/* 3243 */ 0,
/* 3244 */ -3237,
/* 3245 */ 1238,
/* 3246 */ 3250,
/* 3247 */ 50124,
/* 3248 */ -3246,
/* 3249 */ 745,
/* 3250 */ 3254,
/* 3251 */ 50044,
/* 3252 */ -3246,
/* 3253 */ 746,
/* 3254 */ 0,
/* 3255 */ -3246,
/* 3256 */ 1237,
/* 3257 */ 3267,
/* 3258 */ 50282,
/* 3259 */ 50091,
/* 3260 */ 223,
/* 3261 */ 3287,
/* 3262 */ 50093,
/* 3263 */ 50295,
/* 3264 */ 3325,
/* 3265 */ -3257,
/* 3266 */ 220,
/* 3267 */ 3277,
/* 3268 */ 50440,
/* 3269 */ 50091,
/* 3270 */ 223,
/* 3271 */ 3294,
/* 3272 */ 50093,
/* 3273 */ 50295,
/* 3274 */ 3325,
/* 3275 */ -3257,
/* 3276 */ 221,
/* 3277 */ 0,
/* 3278 */ 50347,
/* 3279 */ 50091,
/* 3280 */ 223,
/* 3281 */ 3301,
/* 3282 */ 50093,
/* 3283 */ 50295,
/* 3284 */ 3325,
/* 3285 */ -3257,
/* 3286 */ 222,
/* 3287 */ 3291,
/* 3288 */ 3308,
/* 3289 */ -3287,
/* 3290 */ 748,
/* 3291 */ 0,
/* 3292 */ -3287,
/* 3293 */ 1239,
/* 3294 */ 3298,
/* 3295 */ 3308,
/* 3296 */ -3294,
/* 3297 */ 749,
/* 3298 */ 0,
/* 3299 */ -3294,
/* 3300 */ 1240,
/* 3301 */ 3305,
/* 3302 */ 3308,
/* 3303 */ -3301,
/* 3304 */ 750,
/* 3305 */ 0,
/* 3306 */ -3301,
/* 3307 */ 1241,
/* 3308 */ 3313,
/* 3309 */ 50044,
/* 3310 */ 223,
/* 3311 */ -3308,
/* 3312 */ 223,
/* 3313 */ 3318,
/* 3314 */ 50044,
/* 3315 */ 163,
/* 3316 */ -3308,
/* 3317 */ 224,
/* 3318 */ 0,
/* 3319 */ 50044,
/* 3320 */ 223,
/* 3321 */ 50044,
/* 3322 */ 223,
/* 3323 */ -3308,
/* 3324 */ 225,
/* 3325 */ 3330,
/* 3326 */ 223,
/* 3327 */ 3360,
/* 3328 */ -3325,
/* 3329 */ 226,
/* 3330 */ 3336,
/* 3331 */ 50343,
/* 3332 */ 223,
/* 3333 */ 50374,
/* 3334 */ -3325,
/* 3335 */ 227,
/* 3336 */ 3341,
/* 3337 */ 50343,
/* 3338 */ 50374,
/* 3339 */ -3325,
/* 3340 */ 228,
/* 3341 */ 3350,
/* 3342 */ 50343,
/* 3343 */ 50374,
/* 3344 */ 50439,
/* 3345 */ 50295,
/* 3346 */ 223,
/* 3347 */ 3367,
/* 3348 */ -3325,
/* 3349 */ 229,
/* 3350 */ 0,
/* 3351 */ 50343,
/* 3352 */ 223,
/* 3353 */ 50374,
/* 3354 */ 50439,
/* 3355 */ 50295,
/* 3356 */ 223,
/* 3357 */ 3374,
/* 3358 */ -3325,
/* 3359 */ 230,
/* 3360 */ 3364,
/* 3361 */ 50374,
/* 3362 */ -3360,
/* 3363 */ 751,
/* 3364 */ 0,
/* 3365 */ -3360,
/* 3366 */ 1242,
/* 3367 */ 3371,
/* 3368 */ 50374,
/* 3369 */ -3367,
/* 3370 */ 752,
/* 3371 */ 0,
/* 3372 */ -3367,
/* 3373 */ 1243,
/* 3374 */ 3378,
/* 3375 */ 50374,
/* 3376 */ -3374,
/* 3377 */ 753,
/* 3378 */ 0,
/* 3379 */ -3374,
/* 3380 */ 1244,
/* 3381 */ 0,
/* 3382 */ 50332,
/* 3383 */ 50257,
/* 3384 */ 50123,
/* 3385 */ 3389,
/* 3386 */ 50125,
/* 3387 */ -3381,
/* 3388 */ 231,
/* 3389 */ 3394,
/* 3390 */ 3397,
/* 3391 */ 3389,
/* 3392 */ -3389,
/* 3393 */ 754,
/* 3394 */ 0,
/* 3395 */ -3389,
/* 3396 */ 1245,
/* 3397 */ 3404,
/* 3398 */ 50326,
/* 3399 */ 3416,
/* 3400 */ 50257,
/* 3401 */ 50059,
/* 3402 */ -3397,
/* 3403 */ 232,
/* 3404 */ 3408,
/* 3405 */ 3423,
/* 3406 */ -3397,
/* 3407 */ 233,
/* 3408 */ 3412,
/* 3409 */ 538,
/* 3410 */ -3397,
/* 3411 */ 234,
/* 3412 */ 0,
/* 3413 */ 6098,
/* 3414 */ -3397,
/* 3415 */ 235,
/* 3416 */ 3420,
/* 3417 */ 50058,
/* 3418 */ -3416,
/* 3419 */ 755,
/* 3420 */ 0,
/* 3421 */ -3416,
/* 3422 */ 1246,
/* 3423 */ 3430,
/* 3424 */ 50311,
/* 3425 */ 3443,
/* 3426 */ 3450,
/* 3427 */ 50059,
/* 3428 */ -3423,
/* 3429 */ 236,
/* 3430 */ 3436,
/* 3431 */ 50377,
/* 3432 */ 3483,
/* 3433 */ 3490,
/* 3434 */ -3423,
/* 3435 */ 237,
/* 3436 */ 0,
/* 3437 */ 50434,
/* 3438 */ 3517,
/* 3439 */ 3524,
/* 3440 */ 50059,
/* 3441 */ -3423,
/* 3442 */ 238,
/* 3443 */ 3447,
/* 3444 */ 50058,
/* 3445 */ -3443,
/* 3446 */ 756,
/* 3447 */ 0,
/* 3448 */ -3443,
/* 3449 */ 1247,
/* 3450 */ 3458,
/* 3451 */ 50042,
/* 3452 */ 223,
/* 3453 */ 223,
/* 3454 */ 3469,
/* 3455 */ 3450,
/* 3456 */ -3450,
/* 3457 */ 758,
/* 3458 */ 3466,
/* 3459 */ 50042,
/* 3460 */ 223,
/* 3461 */ 163,
/* 3462 */ 3476,
/* 3463 */ 3450,
/* 3464 */ -3450,
/* 3465 */ 760,
/* 3466 */ 0,
/* 3467 */ -3450,
/* 3468 */ 1250,
/* 3469 */ 3473,
/* 3470 */ 50044,
/* 3471 */ -3469,
/* 3472 */ 757,
/* 3473 */ 0,
/* 3474 */ -3469,
/* 3475 */ 1248,
/* 3476 */ 3480,
/* 3477 */ 50044,
/* 3478 */ -3476,
/* 3479 */ 759,
/* 3480 */ 0,
/* 3481 */ -3476,
/* 3482 */ 1249,
/* 3483 */ 3487,
/* 3484 */ 50058,
/* 3485 */ -3483,
/* 3486 */ 761,
/* 3487 */ 0,
/* 3488 */ -3483,
/* 3489 */ 1251,
/* 3490 */ 3498,
/* 3491 */ 50042,
/* 3492 */ 237,
/* 3493 */ 3501,
/* 3494 */ 3509,
/* 3495 */ 3490,
/* 3496 */ -3490,
/* 3497 */ 765,
/* 3498 */ 0,
/* 3499 */ -3490,
/* 3500 */ 1253,
/* 3501 */ 3506,
/* 3502 */ 3571,
/* 3503 */ 3501,
/* 3504 */ -3501,
/* 3505 */ 762,
/* 3506 */ 0,
/* 3507 */ -3501,
/* 3508 */ 1252,
/* 3509 */ 3513,
/* 3510 */ 50044,
/* 3511 */ -3509,
/* 3512 */ 763,
/* 3513 */ 0,
/* 3514 */ 50059,
/* 3515 */ -3509,
/* 3516 */ 764,
/* 3517 */ 3521,
/* 3518 */ 50058,
/* 3519 */ -3517,
/* 3520 */ 766,
/* 3521 */ 0,
/* 3522 */ -3517,
/* 3523 */ 1254,
/* 3524 */ 3532,
/* 3525 */ 50042,
/* 3526 */ 3535,
/* 3527 */ 3543,
/* 3528 */ 3564,
/* 3529 */ 3524,
/* 3530 */ -3524,
/* 3531 */ 772,
/* 3532 */ 0,
/* 3533 */ -3524,
/* 3534 */ 1257,
/* 3535 */ 3540,
/* 3536 */ 237,
/* 3537 */ 50037,
/* 3538 */ -3535,
/* 3539 */ 767,
/* 3540 */ 0,
/* 3541 */ -3535,
/* 3542 */ 1255,
/* 3543 */ 3550,
/* 3544 */ 163,
/* 3545 */ 50295,
/* 3546 */ 50374,
/* 3547 */ 223,
/* 3548 */ -3543,
/* 3549 */ 768,
/* 3550 */ 3557,
/* 3551 */ 163,
/* 3552 */ 50295,
/* 3553 */ 223,
/* 3554 */ 50374,
/* 3555 */ -3543,
/* 3556 */ 769,
/* 3557 */ 0,
/* 3558 */ 163,
/* 3559 */ 50295,
/* 3560 */ 50374,
/* 3561 */ 286,
/* 3562 */ -3543,
/* 3563 */ 770,
/* 3564 */ 3568,
/* 3565 */ 50044,
/* 3566 */ -3564,
/* 3567 */ 771,
/* 3568 */ 0,
/* 3569 */ -3564,
/* 3570 */ 1256,
/* 3571 */ 3575,
/* 3572 */ 163,
/* 3573 */ -3571,
/* 3574 */ 239,
/* 3575 */ 3579,
/* 3576 */ 237,
/* 3577 */ -3571,
/* 3578 */ 240,
/* 3579 */ 0,
/* 3580 */ 50257,
/* 3581 */ -3571,
/* 3582 */ 241,
/* 3583 */ 0,
/* 3584 */ 50293,
/* 3585 */ 50257,
/* 3586 */ 50123,
/* 3587 */ 3591,
/* 3588 */ 50125,
/* 3589 */ -3583,
/* 3590 */ 242,
/* 3591 */ 3596,
/* 3592 */ 3599,
/* 3593 */ 3591,
/* 3594 */ -3591,
/* 3595 */ 773,
/* 3596 */ 0,
/* 3597 */ -3591,
/* 3598 */ 1258,
/* 3599 */ 3606,
/* 3600 */ 50326,
/* 3601 */ 3675,
/* 3602 */ 50257,
/* 3603 */ 50059,
/* 3604 */ -3599,
/* 3605 */ 243,
/* 3606 */ 3613,
/* 3607 */ 50372,
/* 3608 */ 3682,
/* 3609 */ 163,
/* 3610 */ 50059,
/* 3611 */ -3599,
/* 3612 */ 244,
/* 3613 */ 3618,
/* 3614 */ 3689,
/* 3615 */ 50059,
/* 3616 */ -3599,
/* 3617 */ 245,
/* 3618 */ 3625,
/* 3619 */ 50424,
/* 3620 */ 3714,
/* 3621 */ 3721,
/* 3622 */ 50059,
/* 3623 */ -3599,
/* 3624 */ 246,
/* 3625 */ 3632,
/* 3626 */ 50444,
/* 3627 */ 3737,
/* 3628 */ 3744,
/* 3629 */ 50059,
/* 3630 */ -3599,
/* 3631 */ 247,
/* 3632 */ 3639,
/* 3633 */ 50415,
/* 3634 */ 3760,
/* 3635 */ 3767,
/* 3636 */ 50059,
/* 3637 */ -3599,
/* 3638 */ 248,
/* 3639 */ 3646,
/* 3640 */ 50338,
/* 3641 */ 50353,
/* 3642 */ 3783,
/* 3643 */ 50059,
/* 3644 */ -3599,
/* 3645 */ 249,
/* 3646 */ 3653,
/* 3647 */ 50470,
/* 3648 */ 50353,
/* 3649 */ 3796,
/* 3650 */ 50059,
/* 3651 */ -3599,
/* 3652 */ 250,
/* 3653 */ 3660,
/* 3654 */ 50366,
/* 3655 */ 50353,
/* 3656 */ 3809,
/* 3657 */ 50059,
/* 3658 */ -3599,
/* 3659 */ 251,
/* 3660 */ 3667,
/* 3661 */ 50371,
/* 3662 */ 50353,
/* 3663 */ 3822,
/* 3664 */ 50059,
/* 3665 */ -3599,
/* 3666 */ 252,
/* 3667 */ 3671,
/* 3668 */ 538,
/* 3669 */ -3599,
/* 3670 */ 253,
/* 3671 */ 0,
/* 3672 */ 6098,
/* 3673 */ -3599,
/* 3674 */ 254,
/* 3675 */ 3679,
/* 3676 */ 50058,
/* 3677 */ -3675,
/* 3678 */ 774,
/* 3679 */ 0,
/* 3680 */ -3675,
/* 3681 */ 1259,
/* 3682 */ 3686,
/* 3683 */ 50058,
/* 3684 */ -3682,
/* 3685 */ 775,
/* 3686 */ 0,
/* 3687 */ -3682,
/* 3688 */ 1260,
/* 3689 */ 3697,
/* 3690 */ 50277,
/* 3691 */ 3700,
/* 3692 */ 223,
/* 3693 */ 3707,
/* 3694 */ 3689,
/* 3695 */ -3689,
/* 3696 */ 778,
/* 3697 */ 0,
/* 3698 */ -3689,
/* 3699 */ 1263,
/* 3700 */ 3704,
/* 3701 */ 50058,
/* 3702 */ -3700,
/* 3703 */ 776,
/* 3704 */ 0,
/* 3705 */ -3700,
/* 3706 */ 1261,
/* 3707 */ 3711,
/* 3708 */ 50044,
/* 3709 */ -3707,
/* 3710 */ 777,
/* 3711 */ 0,
/* 3712 */ -3707,
/* 3713 */ 1262,
/* 3714 */ 3718,
/* 3715 */ 50058,
/* 3716 */ -3714,
/* 3717 */ 779,
/* 3718 */ 0,
/* 3719 */ -3714,
/* 3720 */ 1264,
/* 3721 */ 3727,
/* 3722 */ 223,
/* 3723 */ 3730,
/* 3724 */ 3721,
/* 3725 */ -3721,
/* 3726 */ 781,
/* 3727 */ 0,
/* 3728 */ -3721,
/* 3729 */ 1266,
/* 3730 */ 3734,
/* 3731 */ 50044,
/* 3732 */ -3730,
/* 3733 */ 780,
/* 3734 */ 0,
/* 3735 */ -3730,
/* 3736 */ 1265,
/* 3737 */ 3741,
/* 3738 */ 50058,
/* 3739 */ -3737,
/* 3740 */ 782,
/* 3741 */ 0,
/* 3742 */ -3737,
/* 3743 */ 1267,
/* 3744 */ 3750,
/* 3745 */ 50277,
/* 3746 */ 3753,
/* 3747 */ 3744,
/* 3748 */ -3744,
/* 3749 */ 784,
/* 3750 */ 0,
/* 3751 */ -3744,
/* 3752 */ 1269,
/* 3753 */ 3757,
/* 3754 */ 50044,
/* 3755 */ -3753,
/* 3756 */ 783,
/* 3757 */ 0,
/* 3758 */ -3753,
/* 3759 */ 1268,
/* 3760 */ 3764,
/* 3761 */ 50058,
/* 3762 */ -3760,
/* 3763 */ 785,
/* 3764 */ 0,
/* 3765 */ -3760,
/* 3766 */ 1270,
/* 3767 */ 3773,
/* 3768 */ 223,
/* 3769 */ 3776,
/* 3770 */ 3767,
/* 3771 */ -3767,
/* 3772 */ 787,
/* 3773 */ 0,
/* 3774 */ -3767,
/* 3775 */ 1272,
/* 3776 */ 3780,
/* 3777 */ 50044,
/* 3778 */ -3776,
/* 3779 */ 786,
/* 3780 */ 0,
/* 3781 */ -3776,
/* 3782 */ 1271,
/* 3783 */ 3789,
/* 3784 */ 3835,
/* 3785 */ 3792,
/* 3786 */ 3783,
/* 3787 */ -3783,
/* 3788 */ 789,
/* 3789 */ 0,
/* 3790 */ -3783,
/* 3791 */ 1273,
/* 3792 */ 0,
/* 3793 */ 50044,
/* 3794 */ -3792,
/* 3795 */ 788,
/* 3796 */ 3802,
/* 3797 */ 3835,
/* 3798 */ 3805,
/* 3799 */ 3796,
/* 3800 */ -3796,
/* 3801 */ 791,
/* 3802 */ 0,
/* 3803 */ -3796,
/* 3804 */ 1274,
/* 3805 */ 0,
/* 3806 */ 50044,
/* 3807 */ -3805,
/* 3808 */ 790,
/* 3809 */ 3815,
/* 3810 */ 3835,
/* 3811 */ 3818,
/* 3812 */ 3809,
/* 3813 */ -3809,
/* 3814 */ 793,
/* 3815 */ 0,
/* 3816 */ -3809,
/* 3817 */ 1275,
/* 3818 */ 0,
/* 3819 */ 50044,
/* 3820 */ -3818,
/* 3821 */ 792,
/* 3822 */ 3828,
/* 3823 */ 3835,
/* 3824 */ 3831,
/* 3825 */ 3822,
/* 3826 */ -3822,
/* 3827 */ 795,
/* 3828 */ 0,
/* 3829 */ -3822,
/* 3830 */ 1276,
/* 3831 */ 0,
/* 3832 */ 50044,
/* 3833 */ -3831,
/* 3834 */ 794,
/* 3835 */ 3844,
/* 3836 */ 50440,
/* 3837 */ 50091,
/* 3838 */ 223,
/* 3839 */ 50044,
/* 3840 */ 223,
/* 3841 */ 50093,
/* 3842 */ -3835,
/* 3843 */ 255,
/* 3844 */ 3851,
/* 3845 */ 50440,
/* 3846 */ 50091,
/* 3847 */ 223,
/* 3848 */ 50093,
/* 3849 */ -3835,
/* 3850 */ 256,
/* 3851 */ 3856,
/* 3852 */ 163,
/* 3853 */ 3887,
/* 3854 */ -3835,
/* 3855 */ 257,
/* 3856 */ 3863,
/* 3857 */ 163,
/* 3858 */ 50343,
/* 3859 */ 259,
/* 3860 */ 50459,
/* 3861 */ -3835,
/* 3862 */ 258,
/* 3863 */ 3870,
/* 3864 */ 163,
/* 3865 */ 223,
/* 3866 */ 3895,
/* 3867 */ 50321,
/* 3868 */ -3835,
/* 3869 */ 259,
/* 3870 */ 3877,
/* 3871 */ 163,
/* 3872 */ 223,
/* 3873 */ 3903,
/* 3874 */ 50465,
/* 3875 */ -3835,
/* 3876 */ 260,
/* 3877 */ 3882,
/* 3878 */ 50370,
/* 3879 */ 163,
/* 3880 */ -3835,
/* 3881 */ 261,
/* 3882 */ 0,
/* 3883 */ 50347,
/* 3884 */ 223,
/* 3885 */ -3835,
/* 3886 */ 262,
/* 3887 */ 3892,
/* 3888 */ 50295,
/* 3889 */ 50279,
/* 3890 */ -3887,
/* 3891 */ 796,
/* 3892 */ 0,
/* 3893 */ -3887,
/* 3894 */ 1277,
/* 3895 */ 3899,
/* 3896 */ 50047,
/* 3897 */ -3895,
/* 3898 */ 797,
/* 3899 */ 0,
/* 3900 */ 50408,
/* 3901 */ -3895,
/* 3902 */ 798,
/* 3903 */ 3907,
/* 3904 */ 50047,
/* 3905 */ -3903,
/* 3906 */ 799,
/* 3907 */ 0,
/* 3908 */ 50408,
/* 3909 */ -3903,
/* 3910 */ 800,
/* 3911 */ 0,
/* 3912 */ 50328,
/* 3913 */ 50257,
/* 3914 */ 50058,
/* 3915 */ 223,
/* 3916 */ 50123,
/* 3917 */ 3921,
/* 3918 */ 50125,
/* 3919 */ -3911,
/* 3920 */ 263,
/* 3921 */ 3926,
/* 3922 */ 3929,
/* 3923 */ 3921,
/* 3924 */ -3921,
/* 3925 */ 801,
/* 3926 */ 0,
/* 3927 */ -3921,
/* 3928 */ 1278,
/* 3929 */ 3936,
/* 3930 */ 50437,
/* 3931 */ 3962,
/* 3932 */ 3969,
/* 3933 */ 50059,
/* 3934 */ -3929,
/* 3935 */ 264,
/* 3936 */ 3943,
/* 3937 */ 50359,
/* 3938 */ 3985,
/* 3939 */ 3992,
/* 3940 */ 50059,
/* 3941 */ -3929,
/* 3942 */ 265,
/* 3943 */ 3950,
/* 3944 */ 50326,
/* 3945 */ 4008,
/* 3946 */ 50257,
/* 3947 */ 50059,
/* 3948 */ -3929,
/* 3949 */ 266,
/* 3950 */ 3954,
/* 3951 */ 538,
/* 3952 */ -3929,
/* 3953 */ 267,
/* 3954 */ 3958,
/* 3955 */ 3423,
/* 3956 */ -3929,
/* 3957 */ 268,
/* 3958 */ 0,
/* 3959 */ 6098,
/* 3960 */ -3929,
/* 3961 */ 269,
/* 3962 */ 3966,
/* 3963 */ 50058,
/* 3964 */ -3962,
/* 3965 */ 802,
/* 3966 */ 0,
/* 3967 */ -3962,
/* 3968 */ 1279,
/* 3969 */ 3975,
/* 3970 */ 163,
/* 3971 */ 3978,
/* 3972 */ 3969,
/* 3973 */ -3969,
/* 3974 */ 804,
/* 3975 */ 0,
/* 3976 */ -3969,
/* 3977 */ 1281,
/* 3978 */ 3982,
/* 3979 */ 50044,
/* 3980 */ -3978,
/* 3981 */ 803,
/* 3982 */ 0,
/* 3983 */ -3978,
/* 3984 */ 1280,
/* 3985 */ 3989,
/* 3986 */ 50058,
/* 3987 */ -3985,
/* 3988 */ 805,
/* 3989 */ 0,
/* 3990 */ -3985,
/* 3991 */ 1282,
/* 3992 */ 3998,
/* 3993 */ 3257,
/* 3994 */ 4001,
/* 3995 */ 3992,
/* 3996 */ -3992,
/* 3997 */ 807,
/* 3998 */ 0,
/* 3999 */ -3992,
/* 4000 */ 1284,
/* 4001 */ 4005,
/* 4002 */ 50044,
/* 4003 */ -4001,
/* 4004 */ 806,
/* 4005 */ 0,
/* 4006 */ -4001,
/* 4007 */ 1283,
/* 4008 */ 4012,
/* 4009 */ 50058,
/* 4010 */ -4008,
/* 4011 */ 808,
/* 4012 */ 0,
/* 4013 */ -4008,
/* 4014 */ 1285,
/* 4015 */ 0,
/* 4016 */ 50358,
/* 4017 */ 50257,
/* 4018 */ 50123,
/* 4019 */ 4023,
/* 4020 */ 50125,
/* 4021 */ -4015,
/* 4022 */ 270,
/* 4023 */ 4028,
/* 4024 */ 4031,
/* 4025 */ 4023,
/* 4026 */ -4023,
/* 4027 */ 809,
/* 4028 */ 0,
/* 4029 */ -4023,
/* 4030 */ 1286,
/* 4031 */ 4038,
/* 4032 */ 50329,
/* 4033 */ 4076,
/* 4034 */ 4083,
/* 4035 */ 50059,
/* 4036 */ -4031,
/* 4037 */ 271,
/* 4038 */ 4043,
/* 4039 */ 50311,
/* 4040 */ 50059,
/* 4041 */ -4031,
/* 4042 */ 272,
/* 4043 */ 4050,
/* 4044 */ 50350,
/* 4045 */ 4099,
/* 4046 */ 4106,
/* 4047 */ 50059,
/* 4048 */ -4031,
/* 4049 */ 273,
/* 4050 */ 4057,
/* 4051 */ 50326,
/* 4052 */ 4126,
/* 4053 */ 50257,
/* 4054 */ 50059,
/* 4055 */ -4031,
/* 4056 */ 274,
/* 4057 */ 4064,
/* 4058 */ 50359,
/* 4059 */ 4133,
/* 4060 */ 4140,
/* 4061 */ 50059,
/* 4062 */ -4031,
/* 4063 */ 275,
/* 4064 */ 4068,
/* 4065 */ 538,
/* 4066 */ -4031,
/* 4067 */ 276,
/* 4068 */ 4072,
/* 4069 */ 3423,
/* 4070 */ -4031,
/* 4071 */ 277,
/* 4072 */ 0,
/* 4073 */ 6098,
/* 4074 */ -4031,
/* 4075 */ 278,
/* 4076 */ 4080,
/* 4077 */ 50058,
/* 4078 */ -4076,
/* 4079 */ 810,
/* 4080 */ 0,
/* 4081 */ -4076,
/* 4082 */ 1287,
/* 4083 */ 4089,
/* 4084 */ 163,
/* 4085 */ 4092,
/* 4086 */ 4083,
/* 4087 */ -4083,
/* 4088 */ 812,
/* 4089 */ 0,
/* 4090 */ -4083,
/* 4091 */ 1289,
/* 4092 */ 4096,
/* 4093 */ 50044,
/* 4094 */ -4092,
/* 4095 */ 811,
/* 4096 */ 0,
/* 4097 */ -4092,
/* 4098 */ 1288,
/* 4099 */ 4103,
/* 4100 */ 50058,
/* 4101 */ -4099,
/* 4102 */ 813,
/* 4103 */ 0,
/* 4104 */ -4099,
/* 4105 */ 1290,
/* 4106 */ 4112,
/* 4107 */ 223,
/* 4108 */ 4115,
/* 4109 */ 4106,
/* 4110 */ -4106,
/* 4111 */ 816,
/* 4112 */ 0,
/* 4113 */ -4106,
/* 4114 */ 1292,
/* 4115 */ 4119,
/* 4116 */ 50124,
/* 4117 */ -4115,
/* 4118 */ 814,
/* 4119 */ 4123,
/* 4120 */ 50044,
/* 4121 */ -4115,
/* 4122 */ 815,
/* 4123 */ 0,
/* 4124 */ -4115,
/* 4125 */ 1291,
/* 4126 */ 4130,
/* 4127 */ 50058,
/* 4128 */ -4126,
/* 4129 */ 817,
/* 4130 */ 0,
/* 4131 */ -4126,
/* 4132 */ 1293,
/* 4133 */ 4137,
/* 4134 */ 50058,
/* 4135 */ -4133,
/* 4136 */ 818,
/* 4137 */ 0,
/* 4138 */ -4133,
/* 4139 */ 1294,
/* 4140 */ 4146,
/* 4141 */ 3257,
/* 4142 */ 4149,
/* 4143 */ 4140,
/* 4144 */ -4140,
/* 4145 */ 820,
/* 4146 */ 0,
/* 4147 */ -4140,
/* 4148 */ 1296,
/* 4149 */ 4153,
/* 4150 */ 50044,
/* 4151 */ -4149,
/* 4152 */ 819,
/* 4153 */ 0,
/* 4154 */ -4149,
/* 4155 */ 1295,
/* 4156 */ 0,
/* 4157 */ 50450,
/* 4158 */ 50257,
/* 4159 */ 50123,
/* 4160 */ 4164,
/* 4161 */ 50125,
/* 4162 */ -4156,
/* 4163 */ 279,
/* 4164 */ 4169,
/* 4165 */ 4172,
/* 4166 */ 4164,
/* 4167 */ -4164,
/* 4168 */ 821,
/* 4169 */ 0,
/* 4170 */ -4164,
/* 4171 */ 1297,
/* 4172 */ 4179,
/* 4173 */ 50326,
/* 4174 */ 4224,
/* 4175 */ 50257,
/* 4176 */ 50059,
/* 4177 */ -4172,
/* 4178 */ 280,
/* 4179 */ 4183,
/* 4180 */ 462,
/* 4181 */ -4172,
/* 4182 */ 281,
/* 4183 */ 4191,
/* 4184 */ 50389,
/* 4185 */ 4231,
/* 4186 */ 223,
/* 4187 */ 50037,
/* 4188 */ 50059,
/* 4189 */ -4172,
/* 4190 */ 282,
/* 4191 */ 4198,
/* 4192 */ 50407,
/* 4193 */ 4238,
/* 4194 */ 223,
/* 4195 */ 50059,
/* 4196 */ -4172,
/* 4197 */ 283,
/* 4198 */ 4205,
/* 4199 */ 50383,
/* 4200 */ 4245,
/* 4201 */ 223,
/* 4202 */ 50059,
/* 4203 */ -4172,
/* 4204 */ 284,
/* 4205 */ 4212,
/* 4206 */ 50350,
/* 4207 */ 4252,
/* 4208 */ 4259,
/* 4209 */ 50059,
/* 4210 */ -4172,
/* 4211 */ 285,
/* 4212 */ 4216,
/* 4213 */ 3423,
/* 4214 */ -4172,
/* 4215 */ 286,
/* 4216 */ 4220,
/* 4217 */ 538,
/* 4218 */ -4172,
/* 4219 */ 287,
/* 4220 */ 0,
/* 4221 */ 6098,
/* 4222 */ -4172,
/* 4223 */ 288,
/* 4224 */ 4228,
/* 4225 */ 50058,
/* 4226 */ -4224,
/* 4227 */ 822,
/* 4228 */ 0,
/* 4229 */ -4224,
/* 4230 */ 1298,
/* 4231 */ 4235,
/* 4232 */ 50058,
/* 4233 */ -4231,
/* 4234 */ 823,
/* 4235 */ 0,
/* 4236 */ -4231,
/* 4237 */ 1299,
/* 4238 */ 4242,
/* 4239 */ 50058,
/* 4240 */ -4238,
/* 4241 */ 824,
/* 4242 */ 0,
/* 4243 */ -4238,
/* 4244 */ 1300,
/* 4245 */ 4249,
/* 4246 */ 50058,
/* 4247 */ -4245,
/* 4248 */ 825,
/* 4249 */ 0,
/* 4250 */ -4245,
/* 4251 */ 1301,
/* 4252 */ 4256,
/* 4253 */ 50058,
/* 4254 */ -4252,
/* 4255 */ 826,
/* 4256 */ 0,
/* 4257 */ -4252,
/* 4258 */ 1302,
/* 4259 */ 4265,
/* 4260 */ 223,
/* 4261 */ 4268,
/* 4262 */ 4259,
/* 4263 */ -4259,
/* 4264 */ 829,
/* 4265 */ 0,
/* 4266 */ -4259,
/* 4267 */ 1304,
/* 4268 */ 4272,
/* 4269 */ 50124,
/* 4270 */ -4268,
/* 4271 */ 827,
/* 4272 */ 4276,
/* 4273 */ 50044,
/* 4274 */ -4268,
/* 4275 */ 828,
/* 4276 */ 0,
/* 4277 */ -4268,
/* 4278 */ 1303,
/* 4279 */ 0,
/* 4280 */ 50422,
/* 4281 */ 50257,
/* 4282 */ 50058,
/* 4283 */ 223,
/* 4284 */ 50123,
/* 4285 */ 4289,
/* 4286 */ 50125,
/* 4287 */ -4279,
/* 4288 */ 289,
/* 4289 */ 4294,
/* 4290 */ 4297,
/* 4291 */ 4289,
/* 4292 */ -4289,
/* 4293 */ 830,
/* 4294 */ 0,
/* 4295 */ -4289,
/* 4296 */ 1305,
/* 4297 */ 4304,
/* 4298 */ 50326,
/* 4299 */ 4391,
/* 4300 */ 50257,
/* 4301 */ 50059,
/* 4302 */ -4297,
/* 4303 */ 290,
/* 4304 */ 4311,
/* 4305 */ 50464,
/* 4306 */ 4398,
/* 4307 */ 163,
/* 4308 */ 50059,
/* 4309 */ -4297,
/* 4310 */ 291,
/* 4311 */ 4318,
/* 4312 */ 50351,
/* 4313 */ 4405,
/* 4314 */ 163,
/* 4315 */ 50059,
/* 4316 */ -4297,
/* 4317 */ 292,
/* 4318 */ 4325,
/* 4319 */ 50327,
/* 4320 */ 4412,
/* 4321 */ 163,
/* 4322 */ 50059,
/* 4323 */ -4297,
/* 4324 */ 293,
/* 4325 */ 4332,
/* 4326 */ 50325,
/* 4327 */ 4419,
/* 4328 */ 173,
/* 4329 */ 50059,
/* 4330 */ -4297,
/* 4331 */ 294,
/* 4332 */ 4339,
/* 4333 */ 50431,
/* 4334 */ 4426,
/* 4335 */ 173,
/* 4336 */ 50059,
/* 4337 */ -4297,
/* 4338 */ 295,
/* 4339 */ 4346,
/* 4340 */ 50426,
/* 4341 */ 4433,
/* 4342 */ 4440,
/* 4343 */ 50059,
/* 4344 */ -4297,
/* 4345 */ 296,
/* 4346 */ 4353,
/* 4347 */ 50350,
/* 4348 */ 4460,
/* 4349 */ 4467,
/* 4350 */ 50059,
/* 4351 */ -4297,
/* 4352 */ 297,
/* 4353 */ 4360,
/* 4354 */ 50360,
/* 4355 */ 50058,
/* 4356 */ 50260,
/* 4357 */ 50059,
/* 4358 */ -4297,
/* 4359 */ 298,
/* 4360 */ 4367,
/* 4361 */ 50455,
/* 4362 */ 50058,
/* 4363 */ 4487,
/* 4364 */ 50059,
/* 4365 */ -4297,
/* 4366 */ 299,
/* 4367 */ 4373,
/* 4368 */ 50423,
/* 4369 */ 50058,
/* 4370 */ 4580,
/* 4371 */ -4297,
/* 4372 */ 300,
/* 4373 */ 4379,
/* 4374 */ 50398,
/* 4375 */ 50058,
/* 4376 */ 4588,
/* 4377 */ -4297,
/* 4378 */ 301,
/* 4379 */ 4383,
/* 4380 */ 3423,
/* 4381 */ -4297,
/* 4382 */ 302,
/* 4383 */ 4387,
/* 4384 */ 538,
/* 4385 */ -4297,
/* 4386 */ 303,
/* 4387 */ 0,
/* 4388 */ 6098,
/* 4389 */ -4297,
/* 4390 */ 304,
/* 4391 */ 4395,
/* 4392 */ 50058,
/* 4393 */ -4391,
/* 4394 */ 831,
/* 4395 */ 0,
/* 4396 */ -4391,
/* 4397 */ 1306,
/* 4398 */ 4402,
/* 4399 */ 50058,
/* 4400 */ -4398,
/* 4401 */ 832,
/* 4402 */ 0,
/* 4403 */ -4398,
/* 4404 */ 1307,
/* 4405 */ 4409,
/* 4406 */ 50058,
/* 4407 */ -4405,
/* 4408 */ 833,
/* 4409 */ 0,
/* 4410 */ -4405,
/* 4411 */ 1308,
/* 4412 */ 4416,
/* 4413 */ 50058,
/* 4414 */ -4412,
/* 4415 */ 834,
/* 4416 */ 0,
/* 4417 */ -4412,
/* 4418 */ 1309,
/* 4419 */ 4423,
/* 4420 */ 50058,
/* 4421 */ -4419,
/* 4422 */ 835,
/* 4423 */ 0,
/* 4424 */ -4419,
/* 4425 */ 1310,
/* 4426 */ 4430,
/* 4427 */ 50058,
/* 4428 */ -4426,
/* 4429 */ 836,
/* 4430 */ 0,
/* 4431 */ -4426,
/* 4432 */ 1311,
/* 4433 */ 4437,
/* 4434 */ 50058,
/* 4435 */ -4433,
/* 4436 */ 837,
/* 4437 */ 0,
/* 4438 */ -4433,
/* 4439 */ 1312,
/* 4440 */ 4446,
/* 4441 */ 223,
/* 4442 */ 4449,
/* 4443 */ 4440,
/* 4444 */ -4440,
/* 4445 */ 840,
/* 4446 */ 0,
/* 4447 */ -4440,
/* 4448 */ 1314,
/* 4449 */ 4453,
/* 4450 */ 50044,
/* 4451 */ -4449,
/* 4452 */ 838,
/* 4453 */ 4457,
/* 4454 */ 50124,
/* 4455 */ -4449,
/* 4456 */ 839,
/* 4457 */ 0,
/* 4458 */ -4449,
/* 4459 */ 1313,
/* 4460 */ 4464,
/* 4461 */ 50058,
/* 4462 */ -4460,
/* 4463 */ 841,
/* 4464 */ 0,
/* 4465 */ -4460,
/* 4466 */ 1315,
/* 4467 */ 4473,
/* 4468 */ 223,
/* 4469 */ 4476,
/* 4470 */ 4467,
/* 4471 */ -4467,
/* 4472 */ 844,
/* 4473 */ 0,
/* 4474 */ -4467,
/* 4475 */ 1317,
/* 4476 */ 4480,
/* 4477 */ 50124,
/* 4478 */ -4476,
/* 4479 */ 842,
/* 4480 */ 4484,
/* 4481 */ 50044,
/* 4482 */ -4476,
/* 4483 */ 843,
/* 4484 */ 0,
/* 4485 */ -4476,
/* 4486 */ 1316,
/* 4487 */ 4496,
/* 4488 */ 50259,
/* 4489 */ 50058,
/* 4490 */ 163,
/* 4491 */ 4499,
/* 4492 */ 4573,
/* 4493 */ 4487,
/* 4494 */ -4487,
/* 4495 */ 857,
/* 4496 */ 0,
/* 4497 */ -4487,
/* 4498 */ 1323,
/* 4499 */ 4506,
/* 4500 */ 50091,
/* 4501 */ 173,
/* 4502 */ 50093,
/* 4503 */ 4499,
/* 4504 */ -4499,
/* 4505 */ 845,
/* 4506 */ 4513,
/* 4507 */ 50471,
/* 4508 */ 4522,
/* 4509 */ 4552,
/* 4510 */ 4499,
/* 4511 */ -4499,
/* 4512 */ 854,
/* 4513 */ 4519,
/* 4514 */ 50294,
/* 4515 */ 509,
/* 4516 */ 4499,
/* 4517 */ -4499,
/* 4518 */ 855,
/* 4519 */ 0,
/* 4520 */ -4499,
/* 4521 */ 1321,
/* 4522 */ 4526,
/* 4523 */ 163,
/* 4524 */ -4522,
/* 4525 */ 846,
/* 4526 */ 4532,
/* 4527 */ 50419,
/* 4528 */ 50372,
/* 4529 */ 4538,
/* 4530 */ -4522,
/* 4531 */ 848,
/* 4532 */ 0,
/* 4533 */ 50419,
/* 4534 */ 50388,
/* 4535 */ 4545,
/* 4536 */ -4522,
/* 4537 */ 850,
/* 4538 */ 4542,
/* 4539 */ 173,
/* 4540 */ -4538,
/* 4541 */ 847,
/* 4542 */ 0,
/* 4543 */ -4538,
/* 4544 */ 1318,
/* 4545 */ 4549,
/* 4546 */ 173,
/* 4547 */ -4545,
/* 4548 */ 849,
/* 4549 */ 0,
/* 4550 */ -4545,
/* 4551 */ 1319,
/* 4552 */ 4557,
/* 4553 */ 50399,
/* 4554 */ 163,
/* 4555 */ -4552,
/* 4556 */ 851,
/* 4557 */ 4565,
/* 4558 */ 50399,
/* 4559 */ 50419,
/* 4560 */ 173,
/* 4561 */ 50457,
/* 4562 */ 173,
/* 4563 */ -4552,
/* 4564 */ 852,
/* 4565 */ 4570,
/* 4566 */ 50399,
/* 4567 */ 173,
/* 4568 */ -4552,
/* 4569 */ 853,
/* 4570 */ 0,
/* 4571 */ -4552,
/* 4572 */ 1320,
/* 4573 */ 4577,
/* 4574 */ 50044,
/* 4575 */ -4573,
/* 4576 */ 856,
/* 4577 */ 0,
/* 4578 */ -4573,
/* 4579 */ 1322,
/* 4580 */ 4585,
/* 4581 */ 4297,
/* 4582 */ 4580,
/* 4583 */ -4580,
/* 4584 */ 858,
/* 4585 */ 0,
/* 4586 */ -4580,
/* 4587 */ 1324,
/* 4588 */ 4593,
/* 4589 */ 2693,
/* 4590 */ 4588,
/* 4591 */ -4588,
/* 4592 */ 859,
/* 4593 */ 0,
/* 4594 */ -4588,
/* 4595 */ 1325,
/* 4596 */ 0,
/* 4597 */ 50449,
/* 4598 */ 50257,
/* 4599 */ 50058,
/* 4600 */ 223,
/* 4601 */ 4607,
/* 4602 */ 50123,
/* 4603 */ 4615,
/* 4604 */ 50125,
/* 4605 */ -4596,
/* 4606 */ 305,
/* 4607 */ 4612,
/* 4608 */ 50353,
/* 4609 */ 223,
/* 4610 */ -4607,
/* 4611 */ 860,
/* 4612 */ 0,
/* 4613 */ -4607,
/* 4614 */ 1326,
/* 4615 */ 4620,
/* 4616 */ 4623,
/* 4617 */ 4615,
/* 4618 */ -4615,
/* 4619 */ 861,
/* 4620 */ 0,
/* 4621 */ -4615,
/* 4622 */ 1327,
/* 4623 */ 4630,
/* 4624 */ 50326,
/* 4625 */ 4850,
/* 4626 */ 50257,
/* 4627 */ 50059,
/* 4628 */ -4623,
/* 4629 */ 306,
/* 4630 */ 4637,
/* 4631 */ 50353,
/* 4632 */ 4857,
/* 4633 */ 223,
/* 4634 */ 50059,
/* 4635 */ -4623,
/* 4636 */ 307,
/* 4637 */ 4644,
/* 4638 */ 50390,
/* 4639 */ 4864,
/* 4640 */ 223,
/* 4641 */ 50059,
/* 4642 */ -4623,
/* 4643 */ 308,
/* 4644 */ 4648,
/* 4645 */ 462,
/* 4646 */ -4623,
/* 4647 */ 309,
/* 4648 */ 4655,
/* 4649 */ 4871,
/* 4650 */ 4879,
/* 4651 */ 596,
/* 4652 */ 50059,
/* 4653 */ -4623,
/* 4654 */ 310,
/* 4655 */ 4662,
/* 4656 */ 50364,
/* 4657 */ 4886,
/* 4658 */ 596,
/* 4659 */ 50059,
/* 4660 */ -4623,
/* 4661 */ 311,
/* 4662 */ 4669,
/* 4663 */ 50366,
/* 4664 */ 4893,
/* 4665 */ 596,
/* 4666 */ 50059,
/* 4667 */ -4623,
/* 4668 */ 312,
/* 4669 */ 4676,
/* 4670 */ 50323,
/* 4671 */ 4900,
/* 4672 */ 596,
/* 4673 */ 50059,
/* 4674 */ -4623,
/* 4675 */ 313,
/* 4676 */ 4683,
/* 4677 */ 50292,
/* 4678 */ 4907,
/* 4679 */ 596,
/* 4680 */ 50059,
/* 4681 */ -4623,
/* 4682 */ 314,
/* 4683 */ 4690,
/* 4684 */ 50319,
/* 4685 */ 4914,
/* 4686 */ 173,
/* 4687 */ 50059,
/* 4688 */ -4623,
/* 4689 */ 315,
/* 4690 */ 4697,
/* 4691 */ 50410,
/* 4692 */ 4921,
/* 4693 */ 596,
/* 4694 */ 50059,
/* 4695 */ -4623,
/* 4696 */ 316,
/* 4697 */ 4705,
/* 4698 */ 50306,
/* 4699 */ 50374,
/* 4700 */ 4928,
/* 4701 */ 596,
/* 4702 */ 50059,
/* 4703 */ -4623,
/* 4704 */ 317,
/* 4705 */ 4712,
/* 4706 */ 50431,
/* 4707 */ 4935,
/* 4708 */ 596,
/* 4709 */ 50059,
/* 4710 */ -4623,
/* 4711 */ 318,
/* 4712 */ 4719,
/* 4713 */ 50435,
/* 4714 */ 4942,
/* 4715 */ 596,
/* 4716 */ 50059,
/* 4717 */ -4623,
/* 4718 */ 319,
/* 4719 */ 4726,
/* 4720 */ 50389,
/* 4721 */ 4949,
/* 4722 */ 596,
/* 4723 */ 50059,
/* 4724 */ -4623,
/* 4725 */ 320,
/* 4726 */ 4733,
/* 4727 */ 50277,
/* 4728 */ 4956,
/* 4729 */ 596,
/* 4730 */ 4963,
/* 4731 */ -4623,
/* 4732 */ 321,
/* 4733 */ 4741,
/* 4734 */ 50296,
/* 4735 */ 4971,
/* 4736 */ 5193,
/* 4737 */ 4978,
/* 4738 */ 50059,
/* 4739 */ -4623,
/* 4740 */ 322,
/* 4741 */ 4749,
/* 4742 */ 50440,
/* 4743 */ 50091,
/* 4744 */ 223,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4623,
/* 4748 */ 323,
/* 4749 */ 4759,
/* 4750 */ 50440,
/* 4751 */ 50091,
/* 4752 */ 223,
/* 4753 */ 50044,
/* 4754 */ 223,
/* 4755 */ 50093,
/* 4756 */ 50059,
/* 4757 */ -4623,
/* 4758 */ 324,
/* 4759 */ 4771,
/* 4760 */ 50440,
/* 4761 */ 50091,
/* 4762 */ 223,
/* 4763 */ 50044,
/* 4764 */ 223,
/* 4765 */ 50044,
/* 4766 */ 223,
/* 4767 */ 50093,
/* 4768 */ 50059,
/* 4769 */ -4623,
/* 4770 */ 325,
/* 4771 */ 4785,
/* 4772 */ 50440,
/* 4773 */ 50091,
/* 4774 */ 223,
/* 4775 */ 50044,
/* 4776 */ 223,
/* 4777 */ 50044,
/* 4778 */ 223,
/* 4779 */ 50044,
/* 4780 */ 163,
/* 4781 */ 50093,
/* 4782 */ 50059,
/* 4783 */ -4623,
/* 4784 */ 326,
/* 4785 */ 4792,
/* 4786 */ 50424,
/* 4787 */ 4987,
/* 4788 */ 4994,
/* 4789 */ 50059,
/* 4790 */ -4623,
/* 4791 */ 327,
/* 4792 */ 4799,
/* 4793 */ 50369,
/* 4794 */ 5027,
/* 4795 */ 5034,
/* 4796 */ 50059,
/* 4797 */ -4623,
/* 4798 */ 328,
/* 4799 */ 4807,
/* 4800 */ 50346,
/* 4801 */ 5067,
/* 4802 */ 5074,
/* 4803 */ 223,
/* 4804 */ 50059,
/* 4805 */ -4623,
/* 4806 */ 329,
/* 4807 */ 4814,
/* 4808 */ 50437,
/* 4809 */ 5083,
/* 4810 */ 5090,
/* 4811 */ 50059,
/* 4812 */ -4623,
/* 4813 */ 330,
/* 4814 */ 4821,
/* 4815 */ 50330,
/* 4816 */ 5106,
/* 4817 */ 5113,
/* 4818 */ 50059,
/* 4819 */ -4623,
/* 4820 */ 331,
/* 4821 */ 4828,
/* 4822 */ 50409,
/* 4823 */ 5129,
/* 4824 */ 5136,
/* 4825 */ 50059,
/* 4826 */ -4623,
/* 4827 */ 332,
/* 4828 */ 4834,
/* 4829 */ 50350,
/* 4830 */ 5152,
/* 4831 */ 5159,
/* 4832 */ -4623,
/* 4833 */ 333,
/* 4834 */ 4838,
/* 4835 */ 433,
/* 4836 */ -4623,
/* 4837 */ 334,
/* 4838 */ 4842,
/* 4839 */ 538,
/* 4840 */ -4623,
/* 4841 */ 335,
/* 4842 */ 4846,
/* 4843 */ 3423,
/* 4844 */ -4623,
/* 4845 */ 336,
/* 4846 */ 0,
/* 4847 */ 6098,
/* 4848 */ -4623,
/* 4849 */ 337,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ 862,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ 1328,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ 863,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ 1329,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ 864,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ 1330,
/* 4871 */ 4875,
/* 4872 */ 50374,
/* 4873 */ -4871,
/* 4874 */ 865,
/* 4875 */ 0,
/* 4876 */ 50307,
/* 4877 */ -4871,
/* 4878 */ 866,
/* 4879 */ 4883,
/* 4880 */ 50058,
/* 4881 */ -4879,
/* 4882 */ 867,
/* 4883 */ 0,
/* 4884 */ -4879,
/* 4885 */ 1331,
/* 4886 */ 4890,
/* 4887 */ 50058,
/* 4888 */ -4886,
/* 4889 */ 868,
/* 4890 */ 0,
/* 4891 */ -4886,
/* 4892 */ 1332,
/* 4893 */ 4897,
/* 4894 */ 50058,
/* 4895 */ -4893,
/* 4896 */ 869,
/* 4897 */ 0,
/* 4898 */ -4893,
/* 4899 */ 1333,
/* 4900 */ 4904,
/* 4901 */ 50058,
/* 4902 */ -4900,
/* 4903 */ 870,
/* 4904 */ 0,
/* 4905 */ -4900,
/* 4906 */ 1334,
/* 4907 */ 4911,
/* 4908 */ 50058,
/* 4909 */ -4907,
/* 4910 */ 871,
/* 4911 */ 0,
/* 4912 */ -4907,
/* 4913 */ 1335,
/* 4914 */ 4918,
/* 4915 */ 50058,
/* 4916 */ -4914,
/* 4917 */ 872,
/* 4918 */ 0,
/* 4919 */ -4914,
/* 4920 */ 1336,
/* 4921 */ 4925,
/* 4922 */ 50058,
/* 4923 */ -4921,
/* 4924 */ 873,
/* 4925 */ 0,
/* 4926 */ -4921,
/* 4927 */ 1337,
/* 4928 */ 4932,
/* 4929 */ 50058,
/* 4930 */ -4928,
/* 4931 */ 874,
/* 4932 */ 0,
/* 4933 */ -4928,
/* 4934 */ 1338,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ 875,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ 1339,
/* 4942 */ 4946,
/* 4943 */ 50058,
/* 4944 */ -4942,
/* 4945 */ 876,
/* 4946 */ 0,
/* 4947 */ -4942,
/* 4948 */ 1340,
/* 4949 */ 4953,
/* 4950 */ 50058,
/* 4951 */ -4949,
/* 4952 */ 877,
/* 4953 */ 0,
/* 4954 */ -4949,
/* 4955 */ 1341,
/* 4956 */ 4960,
/* 4957 */ 50058,
/* 4958 */ -4956,
/* 4959 */ 878,
/* 4960 */ 0,
/* 4961 */ -4956,
/* 4962 */ 1342,
/* 4963 */ 4967,
/* 4964 */ 50044,
/* 4965 */ -4963,
/* 4966 */ 879,
/* 4967 */ 0,
/* 4968 */ 50059,
/* 4969 */ -4963,
/* 4970 */ 880,
/* 4971 */ 4975,
/* 4972 */ 50058,
/* 4973 */ -4971,
/* 4974 */ 881,
/* 4975 */ 0,
/* 4976 */ -4971,
/* 4977 */ 1343,
/* 4978 */ 4984,
/* 4979 */ 50044,
/* 4980 */ 5193,
/* 4981 */ 4978,
/* 4982 */ -4978,
/* 4983 */ 882,
/* 4984 */ 0,
/* 4985 */ -4978,
/* 4986 */ 1344,
/* 4987 */ 4991,
/* 4988 */ 50058,
/* 4989 */ -4987,
/* 4990 */ 883,
/* 4991 */ 0,
/* 4992 */ -4987,
/* 4993 */ 1345,
/* 4994 */ 5000,
/* 4995 */ 5003,
/* 4996 */ 5020,
/* 4997 */ 4994,
/* 4998 */ -4994,
/* 4999 */ 889,
/* 5000 */ 0,
/* 5001 */ -4994,
/* 5002 */ 1347,
/* 5003 */ 5007,
/* 5004 */ 223,
/* 5005 */ -5003,
/* 5006 */ 884,
/* 5007 */ 0,
/* 5008 */ 5012,
/* 5009 */ 223,
/* 5010 */ -5003,
/* 5011 */ 887,
/* 5012 */ 5016,
/* 5013 */ 50126,
/* 5014 */ -5012,
/* 5015 */ 885,
/* 5016 */ 0,
/* 5017 */ 50033,
/* 5018 */ -5012,
/* 5019 */ 886,
/* 5020 */ 5024,
/* 5021 */ 50044,
/* 5022 */ -5020,
/* 5023 */ 888,
/* 5024 */ 0,
/* 5025 */ -5020,
/* 5026 */ 1346,
/* 5027 */ 5031,
/* 5028 */ 50058,
/* 5029 */ -5027,
/* 5030 */ 890,
/* 5031 */ 0,
/* 5032 */ -5027,
/* 5033 */ 1348,
/* 5034 */ 5040,
/* 5035 */ 5043,
/* 5036 */ 5060,
/* 5037 */ 5034,
/* 5038 */ -5034,
/* 5039 */ 896,
/* 5040 */ 0,
/* 5041 */ -5034,
/* 5042 */ 1350,
/* 5043 */ 5047,
/* 5044 */ 223,
/* 5045 */ -5043,
/* 5046 */ 891,
/* 5047 */ 0,
/* 5048 */ 5052,
/* 5049 */ 223,
/* 5050 */ -5043,
/* 5051 */ 894,
/* 5052 */ 5056,
/* 5053 */ 50126,
/* 5054 */ -5052,
/* 5055 */ 892,
/* 5056 */ 0,
/* 5057 */ 50033,
/* 5058 */ -5052,
/* 5059 */ 893,
/* 5060 */ 5064,
/* 5061 */ 50044,
/* 5062 */ -5060,
/* 5063 */ 895,
/* 5064 */ 0,
/* 5065 */ -5060,
/* 5066 */ 1349,
/* 5067 */ 5071,
/* 5068 */ 50058,
/* 5069 */ -5067,
/* 5070 */ 897,
/* 5071 */ 0,
/* 5072 */ -5067,
/* 5073 */ 1351,
/* 5074 */ 5080,
/* 5075 */ 223,
/* 5076 */ 50044,
/* 5077 */ 5074,
/* 5078 */ -5074,
/* 5079 */ 898,
/* 5080 */ 0,
/* 5081 */ -5074,
/* 5082 */ 1352,
/* 5083 */ 5087,
/* 5084 */ 50058,
/* 5085 */ -5083,
/* 5086 */ 899,
/* 5087 */ 0,
/* 5088 */ -5083,
/* 5089 */ 1353,
/* 5090 */ 5096,
/* 5091 */ 163,
/* 5092 */ 5099,
/* 5093 */ 5090,
/* 5094 */ -5090,
/* 5095 */ 901,
/* 5096 */ 0,
/* 5097 */ -5090,
/* 5098 */ 1355,
/* 5099 */ 5103,
/* 5100 */ 50044,
/* 5101 */ -5099,
/* 5102 */ 900,
/* 5103 */ 0,
/* 5104 */ -5099,
/* 5105 */ 1354,
/* 5106 */ 5110,
/* 5107 */ 50058,
/* 5108 */ -5106,
/* 5109 */ 902,
/* 5110 */ 0,
/* 5111 */ -5106,
/* 5112 */ 1356,
/* 5113 */ 5119,
/* 5114 */ 163,
/* 5115 */ 5122,
/* 5116 */ 5113,
/* 5117 */ -5113,
/* 5118 */ 904,
/* 5119 */ 0,
/* 5120 */ -5113,
/* 5121 */ 1358,
/* 5122 */ 5126,
/* 5123 */ 50044,
/* 5124 */ -5122,
/* 5125 */ 903,
/* 5126 */ 0,
/* 5127 */ -5122,
/* 5128 */ 1357,
/* 5129 */ 5133,
/* 5130 */ 50058,
/* 5131 */ -5129,
/* 5132 */ 905,
/* 5133 */ 0,
/* 5134 */ -5129,
/* 5135 */ 1359,
/* 5136 */ 5142,
/* 5137 */ 163,
/* 5138 */ 5145,
/* 5139 */ 5136,
/* 5140 */ -5136,
/* 5141 */ 907,
/* 5142 */ 0,
/* 5143 */ -5136,
/* 5144 */ 1361,
/* 5145 */ 5149,
/* 5146 */ 50044,
/* 5147 */ -5145,
/* 5148 */ 906,
/* 5149 */ 0,
/* 5150 */ -5145,
/* 5151 */ 1360,
/* 5152 */ 5156,
/* 5153 */ 50058,
/* 5154 */ -5152,
/* 5155 */ 908,
/* 5156 */ 0,
/* 5157 */ -5152,
/* 5158 */ 1362,
/* 5159 */ 5165,
/* 5160 */ 5168,
/* 5161 */ 5185,
/* 5162 */ 5159,
/* 5163 */ -5159,
/* 5164 */ 915,
/* 5165 */ 0,
/* 5166 */ -5159,
/* 5167 */ 1363,
/* 5168 */ 5172,
/* 5169 */ 223,
/* 5170 */ -5168,
/* 5171 */ 909,
/* 5172 */ 0,
/* 5173 */ 5177,
/* 5174 */ 223,
/* 5175 */ -5168,
/* 5176 */ 912,
/* 5177 */ 5181,
/* 5178 */ 50126,
/* 5179 */ -5177,
/* 5180 */ 910,
/* 5181 */ 0,
/* 5182 */ 50033,
/* 5183 */ -5177,
/* 5184 */ 911,
/* 5185 */ 5189,
/* 5186 */ 50044,
/* 5187 */ -5185,
/* 5188 */ 913,
/* 5189 */ 0,
/* 5190 */ 50059,
/* 5191 */ -5185,
/* 5192 */ 914,
/* 5193 */ 5203,
/* 5194 */ 223,
/* 5195 */ 50353,
/* 5196 */ 5210,
/* 5197 */ 5222,
/* 5198 */ 5229,
/* 5199 */ 5236,
/* 5200 */ 5258,
/* 5201 */ -5193,
/* 5202 */ 338,
/* 5203 */ 0,
/* 5204 */ 223,
/* 5205 */ 50353,
/* 5206 */ 163,
/* 5207 */ 5313,
/* 5208 */ -5193,
/* 5209 */ 339,
/* 5210 */ 5214,
/* 5211 */ 286,
/* 5212 */ -5210,
/* 5213 */ 916,
/* 5214 */ 5219,
/* 5215 */ 223,
/* 5216 */ 50037,
/* 5217 */ -5210,
/* 5218 */ 917,
/* 5219 */ 0,
/* 5220 */ -5210,
/* 5221 */ 1364,
/* 5222 */ 5226,
/* 5223 */ 223,
/* 5224 */ -5222,
/* 5225 */ 918,
/* 5226 */ 0,
/* 5227 */ -5222,
/* 5228 */ 1365,
/* 5229 */ 5233,
/* 5230 */ 50319,
/* 5231 */ -5229,
/* 5232 */ 919,
/* 5233 */ 0,
/* 5234 */ -5229,
/* 5235 */ 1366,
/* 5236 */ 5243,
/* 5237 */ 50040,
/* 5238 */ 5246,
/* 5239 */ 223,
/* 5240 */ 50041,
/* 5241 */ -5236,
/* 5242 */ 923,
/* 5243 */ 0,
/* 5244 */ -5236,
/* 5245 */ 1367,
/* 5246 */ 5250,
/* 5247 */ 50322,
/* 5248 */ -5246,
/* 5249 */ 920,
/* 5250 */ 5254,
/* 5251 */ 50307,
/* 5252 */ -5246,
/* 5253 */ 921,
/* 5254 */ 0,
/* 5255 */ 50374,
/* 5256 */ -5246,
/* 5257 */ 922,
/* 5258 */ 5267,
/* 5259 */ 50288,
/* 5260 */ 5270,
/* 5261 */ 5277,
/* 5262 */ 5284,
/* 5263 */ 5291,
/* 5264 */ 5258,
/* 5265 */ -5258,
/* 5266 */ 931,
/* 5267 */ 0,
/* 5268 */ -5258,
/* 5269 */ 1372,
/* 5270 */ 5274,
/* 5271 */ 286,
/* 5272 */ -5270,
/* 5273 */ 924,
/* 5274 */ 0,
/* 5275 */ -5270,
/* 5276 */ 1368,
/* 5277 */ 5281,
/* 5278 */ 223,
/* 5279 */ -5277,
/* 5280 */ 925,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ 1369,
/* 5284 */ 5288,
/* 5285 */ 50319,
/* 5286 */ -5284,
/* 5287 */ 926,
/* 5288 */ 0,
/* 5289 */ -5284,
/* 5290 */ 1370,
/* 5291 */ 5298,
/* 5292 */ 50040,
/* 5293 */ 5301,
/* 5294 */ 223,
/* 5295 */ 50041,
/* 5296 */ -5291,
/* 5297 */ 930,
/* 5298 */ 0,
/* 5299 */ -5291,
/* 5300 */ 1371,
/* 5301 */ 5305,
/* 5302 */ 50322,
/* 5303 */ -5301,
/* 5304 */ 927,
/* 5305 */ 5309,
/* 5306 */ 50307,
/* 5307 */ -5301,
/* 5308 */ 928,
/* 5309 */ 0,
/* 5310 */ 50374,
/* 5311 */ -5301,
/* 5312 */ 929,
/* 5313 */ 5317,
/* 5314 */ 223,
/* 5315 */ -5313,
/* 5316 */ 932,
/* 5317 */ 0,
/* 5318 */ -5313,
/* 5319 */ 1373,
/* 5320 */ 0,
/* 5321 */ 50261,
/* 5322 */ -5320,
/* 5323 */ 340,
/* 5324 */ 0,
/* 5325 */ 50341,
/* 5326 */ 50257,
/* 5327 */ 50058,
/* 5328 */ 223,
/* 5329 */ 50123,
/* 5330 */ 5334,
/* 5331 */ 50125,
/* 5332 */ -5324,
/* 5333 */ 341,
/* 5334 */ 5339,
/* 5335 */ 5342,
/* 5336 */ 5334,
/* 5337 */ -5334,
/* 5338 */ 933,
/* 5339 */ 0,
/* 5340 */ -5334,
/* 5341 */ 1374,
/* 5342 */ 5349,
/* 5343 */ 50326,
/* 5344 */ 5371,
/* 5345 */ 50257,
/* 5346 */ 50059,
/* 5347 */ -5342,
/* 5348 */ 342,
/* 5349 */ 5356,
/* 5350 */ 50383,
/* 5351 */ 5378,
/* 5352 */ 223,
/* 5353 */ 50059,
/* 5354 */ -5342,
/* 5355 */ 343,
/* 5356 */ 5363,
/* 5357 */ 50310,
/* 5358 */ 5385,
/* 5359 */ 1247,
/* 5360 */ 50059,
/* 5361 */ -5342,
/* 5362 */ 344,
/* 5363 */ 0,
/* 5364 */ 50466,
/* 5365 */ 5392,
/* 5366 */ 223,
/* 5367 */ 50037,
/* 5368 */ 50059,
/* 5369 */ -5342,
/* 5370 */ 345,
/* 5371 */ 5375,
/* 5372 */ 50058,
/* 5373 */ -5371,
/* 5374 */ 934,
/* 5375 */ 0,
/* 5376 */ -5371,
/* 5377 */ 1375,
/* 5378 */ 5382,
/* 5379 */ 50058,
/* 5380 */ -5378,
/* 5381 */ 935,
/* 5382 */ 0,
/* 5383 */ -5378,
/* 5384 */ 1376,
/* 5385 */ 5389,
/* 5386 */ 50058,
/* 5387 */ -5385,
/* 5388 */ 936,
/* 5389 */ 0,
/* 5390 */ -5385,
/* 5391 */ 1377,
/* 5392 */ 5396,
/* 5393 */ 50058,
/* 5394 */ -5392,
/* 5395 */ 937,
/* 5396 */ 0,
/* 5397 */ -5392,
/* 5398 */ 1378,
/* 5399 */ 0,
/* 5400 */ 50336,
/* 5401 */ 50257,
/* 5402 */ 5408,
/* 5403 */ 50123,
/* 5404 */ 5416,
/* 5405 */ 50125,
/* 5406 */ -5399,
/* 5407 */ 346,
/* 5408 */ 5413,
/* 5409 */ 50058,
/* 5410 */ 223,
/* 5411 */ -5408,
/* 5412 */ 938,
/* 5413 */ 0,
/* 5414 */ -5408,
/* 5415 */ 1379,
/* 5416 */ 5421,
/* 5417 */ 5424,
/* 5418 */ 5416,
/* 5419 */ -5416,
/* 5420 */ 939,
/* 5421 */ 0,
/* 5422 */ -5416,
/* 5423 */ 1380,
/* 5424 */ 5431,
/* 5425 */ 50450,
/* 5426 */ 50058,
/* 5427 */ 173,
/* 5428 */ 50059,
/* 5429 */ -5424,
/* 5430 */ 347,
/* 5431 */ 5438,
/* 5432 */ 50325,
/* 5433 */ 50058,
/* 5434 */ 223,
/* 5435 */ 50059,
/* 5436 */ -5424,
/* 5437 */ 348,
/* 5438 */ 5445,
/* 5439 */ 50466,
/* 5440 */ 50058,
/* 5441 */ 223,
/* 5442 */ 50059,
/* 5443 */ -5424,
/* 5444 */ 349,
/* 5445 */ 5452,
/* 5446 */ 50307,
/* 5447 */ 50058,
/* 5448 */ 223,
/* 5449 */ 50059,
/* 5450 */ -5424,
/* 5451 */ 350,
/* 5452 */ 5460,
/* 5453 */ 50307,
/* 5454 */ 50058,
/* 5455 */ 223,
/* 5456 */ 50043,
/* 5457 */ 50059,
/* 5458 */ -5424,
/* 5459 */ 351,
/* 5460 */ 5469,
/* 5461 */ 50307,
/* 5462 */ 50058,
/* 5463 */ 223,
/* 5464 */ 5506,
/* 5465 */ 223,
/* 5466 */ 50059,
/* 5467 */ -5424,
/* 5468 */ 352,
/* 5469 */ 5476,
/* 5470 */ 50285,
/* 5471 */ 50058,
/* 5472 */ 223,
/* 5473 */ 50059,
/* 5474 */ -5424,
/* 5475 */ 353,
/* 5476 */ 5483,
/* 5477 */ 50406,
/* 5478 */ 50058,
/* 5479 */ 5514,
/* 5480 */ 50059,
/* 5481 */ -5424,
/* 5482 */ 354,
/* 5483 */ 5490,
/* 5484 */ 50350,
/* 5485 */ 5522,
/* 5486 */ 5529,
/* 5487 */ 50059,
/* 5488 */ -5424,
/* 5489 */ 355,
/* 5490 */ 5494,
/* 5491 */ 433,
/* 5492 */ -5424,
/* 5493 */ 356,
/* 5494 */ 5498,
/* 5495 */ 538,
/* 5496 */ -5424,
/* 5497 */ 357,
/* 5498 */ 5502,
/* 5499 */ 3423,
/* 5500 */ -5424,
/* 5501 */ 358,
/* 5502 */ 0,
/* 5503 */ 6098,
/* 5504 */ -5424,
/* 5505 */ 359,
/* 5506 */ 5510,
/* 5507 */ 50457,
/* 5508 */ -5506,
/* 5509 */ 940,
/* 5510 */ 0,
/* 5511 */ 50045,
/* 5512 */ -5506,
/* 5513 */ 941,
/* 5514 */ 5519,
/* 5515 */ 5549,
/* 5516 */ 5514,
/* 5517 */ -5514,
/* 5518 */ 942,
/* 5519 */ 0,
/* 5520 */ -5514,
/* 5521 */ 1381,
/* 5522 */ 5526,
/* 5523 */ 50058,
/* 5524 */ -5522,
/* 5525 */ 943,
/* 5526 */ 0,
/* 5527 */ -5522,
/* 5528 */ 1382,
/* 5529 */ 5535,
/* 5530 */ 223,
/* 5531 */ 5538,
/* 5532 */ 5529,
/* 5533 */ -5529,
/* 5534 */ 946,
/* 5535 */ 0,
/* 5536 */ -5529,
/* 5537 */ 1384,
/* 5538 */ 5542,
/* 5539 */ 50044,
/* 5540 */ -5538,
/* 5541 */ 944,
/* 5542 */ 5546,
/* 5543 */ 50124,
/* 5544 */ -5538,
/* 5545 */ 945,
/* 5546 */ 0,
/* 5547 */ -5538,
/* 5548 */ 1383,
/* 5549 */ 0,
/* 5550 */ 5558,
/* 5551 */ 5570,
/* 5552 */ 50399,
/* 5553 */ 5633,
/* 5554 */ 5646,
/* 5555 */ 5662,
/* 5556 */ -5549,
/* 5557 */ 360,
/* 5558 */ 5562,
/* 5559 */ 50042,
/* 5560 */ -5558,
/* 5561 */ 947,
/* 5562 */ 5566,
/* 5563 */ 50335,
/* 5564 */ -5558,
/* 5565 */ 948,
/* 5566 */ 0,
/* 5567 */ 50124,
/* 5568 */ -5558,
/* 5569 */ 949,
/* 5570 */ 5576,
/* 5571 */ 237,
/* 5572 */ 50037,
/* 5573 */ 5570,
/* 5574 */ -5570,
/* 5575 */ 950,
/* 5576 */ 5585,
/* 5577 */ 50060,
/* 5578 */ 50307,
/* 5579 */ 237,
/* 5580 */ 50043,
/* 5581 */ 50062,
/* 5582 */ 5570,
/* 5583 */ -5570,
/* 5584 */ 951,
/* 5585 */ 5592,
/* 5586 */ 50091,
/* 5587 */ 237,
/* 5588 */ 50093,
/* 5589 */ 5570,
/* 5590 */ -5570,
/* 5591 */ 952,
/* 5592 */ 5599,
/* 5593 */ 50123,
/* 5594 */ 588,
/* 5595 */ 50125,
/* 5596 */ 5570,
/* 5597 */ -5570,
/* 5598 */ 953,
/* 5599 */ 5606,
/* 5600 */ 237,
/* 5601 */ 5625,
/* 5602 */ 237,
/* 5603 */ 5570,
/* 5604 */ -5570,
/* 5605 */ 956,
/* 5606 */ 5611,
/* 5607 */ 237,
/* 5608 */ 5570,
/* 5609 */ -5570,
/* 5610 */ 957,
/* 5611 */ 5617,
/* 5612 */ 237,
/* 5613 */ 50043,
/* 5614 */ 5570,
/* 5615 */ -5570,
/* 5616 */ 958,
/* 5617 */ 5622,
/* 5618 */ 286,
/* 5619 */ 5570,
/* 5620 */ -5570,
/* 5621 */ 959,
/* 5622 */ 0,
/* 5623 */ -5570,
/* 5624 */ 1385,
/* 5625 */ 5629,
/* 5626 */ 50045,
/* 5627 */ -5625,
/* 5628 */ 954,
/* 5629 */ 0,
/* 5630 */ 50457,
/* 5631 */ -5625,
/* 5632 */ 955,
/* 5633 */ 5637,
/* 5634 */ 163,
/* 5635 */ -5633,
/* 5636 */ 960,
/* 5637 */ 5641,
/* 5638 */ 223,
/* 5639 */ -5633,
/* 5640 */ 961,
/* 5641 */ 0,
/* 5642 */ 50289,
/* 5643 */ 223,
/* 5644 */ -5633,
/* 5645 */ 962,
/* 5646 */ 5650,
/* 5647 */ 163,
/* 5648 */ -5646,
/* 5649 */ 963,
/* 5650 */ 5654,
/* 5651 */ 223,
/* 5652 */ -5646,
/* 5653 */ 964,
/* 5654 */ 5659,
/* 5655 */ 50289,
/* 5656 */ 223,
/* 5657 */ -5646,
/* 5658 */ 965,
/* 5659 */ 0,
/* 5660 */ -5646,
/* 5661 */ 1386,
/* 5662 */ 5669,
/* 5663 */ 50091,
/* 5664 */ 223,
/* 5665 */ 5672,
/* 5666 */ 50093,
/* 5667 */ -5662,
/* 5668 */ 969,
/* 5669 */ 0,
/* 5670 */ -5662,
/* 5671 */ 1388,
/* 5672 */ 5678,
/* 5673 */ 5681,
/* 5674 */ 223,
/* 5675 */ 5672,
/* 5676 */ -5672,
/* 5677 */ 968,
/* 5678 */ 0,
/* 5679 */ -5672,
/* 5680 */ 1387,
/* 5681 */ 5685,
/* 5682 */ 50124,
/* 5683 */ -5681,
/* 5684 */ 966,
/* 5685 */ 0,
/* 5686 */ 50044,
/* 5687 */ -5681,
/* 5688 */ 967,
/* 5689 */ 0,
/* 5690 */ 50299,
/* 5691 */ 50257,
/* 5692 */ 50058,
/* 5693 */ 223,
/* 5694 */ 50123,
/* 5695 */ 5699,
/* 5696 */ 50125,
/* 5697 */ -5689,
/* 5698 */ 361,
/* 5699 */ 5704,
/* 5700 */ 5707,
/* 5701 */ 5699,
/* 5702 */ -5699,
/* 5703 */ 970,
/* 5704 */ 0,
/* 5705 */ -5699,
/* 5706 */ 1389,
/* 5707 */ 5714,
/* 5708 */ 50436,
/* 5709 */ 5733,
/* 5710 */ 163,
/* 5711 */ 50059,
/* 5712 */ -5707,
/* 5713 */ 362,
/* 5714 */ 5721,
/* 5715 */ 50350,
/* 5716 */ 5740,
/* 5717 */ 5747,
/* 5718 */ 50059,
/* 5719 */ -5707,
/* 5720 */ 363,
/* 5721 */ 5725,
/* 5722 */ 538,
/* 5723 */ -5707,
/* 5724 */ 364,
/* 5725 */ 5729,
/* 5726 */ 3423,
/* 5727 */ -5707,
/* 5728 */ 365,
/* 5729 */ 0,
/* 5730 */ 6098,
/* 5731 */ -5707,
/* 5732 */ 366,
/* 5733 */ 5737,
/* 5734 */ 50058,
/* 5735 */ -5733,
/* 5736 */ 971,
/* 5737 */ 0,
/* 5738 */ -5733,
/* 5739 */ 1390,
/* 5740 */ 5744,
/* 5741 */ 50058,
/* 5742 */ -5740,
/* 5743 */ 972,
/* 5744 */ 0,
/* 5745 */ -5740,
/* 5746 */ 1391,
/* 5747 */ 5753,
/* 5748 */ 223,
/* 5749 */ 5756,
/* 5750 */ 5747,
/* 5751 */ -5747,
/* 5752 */ 975,
/* 5753 */ 0,
/* 5754 */ -5747,
/* 5755 */ 1393,
/* 5756 */ 5760,
/* 5757 */ 50124,
/* 5758 */ -5756,
/* 5759 */ 973,
/* 5760 */ 5764,
/* 5761 */ 50044,
/* 5762 */ -5756,
/* 5763 */ 974,
/* 5764 */ 0,
/* 5765 */ -5756,
/* 5766 */ 1392,
/* 5767 */ 0,
/* 5768 */ 50451,
/* 5769 */ 50257,
/* 5770 */ 5774,
/* 5771 */ 50059,
/* 5772 */ -5767,
/* 5773 */ 367,
/* 5774 */ 5779,
/* 5775 */ 50257,
/* 5776 */ 5774,
/* 5777 */ -5774,
/* 5778 */ 976,
/* 5779 */ 5784,
/* 5780 */ 163,
/* 5781 */ 5774,
/* 5782 */ -5774,
/* 5783 */ 977,
/* 5784 */ 5789,
/* 5785 */ 50040,
/* 5786 */ 5774,
/* 5787 */ -5774,
/* 5788 */ 978,
/* 5789 */ 5794,
/* 5790 */ 50041,
/* 5791 */ 5774,
/* 5792 */ -5774,
/* 5793 */ 979,
/* 5794 */ 5799,
/* 5795 */ 50124,
/* 5796 */ 5774,
/* 5797 */ -5774,
/* 5798 */ 980,
/* 5799 */ 5804,
/* 5800 */ 50059,
/* 5801 */ 5774,
/* 5802 */ -5774,
/* 5803 */ 981,
/* 5804 */ 5812,
/* 5805 */ 50091,
/* 5806 */ 588,
/* 5807 */ 50093,
/* 5808 */ 50058,
/* 5809 */ 5774,
/* 5810 */ -5774,
/* 5811 */ 982,
/* 5812 */ 5819,
/* 5813 */ 50091,
/* 5814 */ 592,
/* 5815 */ 50093,
/* 5816 */ 5774,
/* 5817 */ -5774,
/* 5818 */ 983,
/* 5819 */ 0,
/* 5820 */ -5774,
/* 5821 */ 1394,
/* 5822 */ 0,
/* 5823 */ 50261,
/* 5824 */ -5822,
/* 5825 */ 368,
/* 5826 */ 5830,
/* 5827 */ 50264,
/* 5828 */ -5826,
/* 5829 */ 369,
/* 5830 */ 5834,
/* 5831 */ 50274,
/* 5832 */ -5826,
/* 5833 */ 370,
/* 5834 */ 5838,
/* 5835 */ 50268,
/* 5836 */ -5826,
/* 5837 */ 371,
/* 5838 */ 5842,
/* 5839 */ 50265,
/* 5840 */ -5826,
/* 5841 */ 372,
/* 5842 */ 5846,
/* 5843 */ 50266,
/* 5844 */ -5826,
/* 5845 */ 373,
/* 5846 */ 5850,
/* 5847 */ 50267,
/* 5848 */ -5826,
/* 5849 */ 374,
/* 5850 */ 5854,
/* 5851 */ 50263,
/* 5852 */ -5826,
/* 5853 */ 375,
/* 5854 */ 5858,
/* 5855 */ 50271,
/* 5856 */ -5826,
/* 5857 */ 376,
/* 5858 */ 5862,
/* 5859 */ 50272,
/* 5860 */ -5826,
/* 5861 */ 377,
/* 5862 */ 5866,
/* 5863 */ 50273,
/* 5864 */ -5826,
/* 5865 */ 378,
/* 5866 */ 0,
/* 5867 */ 50474,
/* 5868 */ -5826,
/* 5869 */ 379,
/* 5870 */ 0,
/* 5871 */ 50447,
/* 5872 */ 5826,
/* 5873 */ 223,
/* 5874 */ 50262,
/* 5875 */ 50258,
/* 5876 */ 50040,
/* 5877 */ 5883,
/* 5878 */ 5952,
/* 5879 */ 50041,
/* 5880 */ 50059,
/* 5881 */ -5870,
/* 5882 */ 380,
/* 5883 */ 5891,
/* 5884 */ 5826,
/* 5885 */ 5894,
/* 5886 */ 5902,
/* 5887 */ 5909,
/* 5888 */ 5917,
/* 5889 */ -5883,
/* 5890 */ 991,
/* 5891 */ 0,
/* 5892 */ -5883,
/* 5893 */ 1402,
/* 5894 */ 5899,
/* 5895 */ 50058,
/* 5896 */ 223,
/* 5897 */ -5894,
/* 5898 */ 984,
/* 5899 */ 0,
/* 5900 */ -5894,
/* 5901 */ 1395,
/* 5902 */ 5906,
/* 5903 */ 50258,
/* 5904 */ -5902,
/* 5905 */ 985,
/* 5906 */ 0,
/* 5907 */ -5902,
/* 5908 */ 1396,
/* 5909 */ 5914,
/* 5910 */ 50061,
/* 5911 */ 173,
/* 5912 */ -5909,
/* 5913 */ 986,
/* 5914 */ 0,
/* 5915 */ -5909,
/* 5916 */ 1397,
/* 5917 */ 5926,
/* 5918 */ 50044,
/* 5919 */ 5826,
/* 5920 */ 5929,
/* 5921 */ 5937,
/* 5922 */ 5944,
/* 5923 */ 5917,
/* 5924 */ -5917,
/* 5925 */ 990,
/* 5926 */ 0,
/* 5927 */ -5917,
/* 5928 */ 1401,
/* 5929 */ 5934,
/* 5930 */ 50058,
/* 5931 */ 223,
/* 5932 */ -5929,
/* 5933 */ 987,
/* 5934 */ 0,
/* 5935 */ -5929,
/* 5936 */ 1398,
/* 5937 */ 5941,
/* 5938 */ 50258,
/* 5939 */ -5937,
/* 5940 */ 988,
/* 5941 */ 0,
/* 5942 */ -5937,
/* 5943 */ 1399,
/* 5944 */ 5949,
/* 5945 */ 50061,
/* 5946 */ 173,
/* 5947 */ -5944,
/* 5948 */ 989,
/* 5949 */ 0,
/* 5950 */ -5944,
/* 5951 */ 1400,
/* 5952 */ 5957,
/* 5953 */ 50044,
/* 5954 */ 50495,
/* 5955 */ -5952,
/* 5956 */ 992,
/* 5957 */ 0,
/* 5958 */ -5952,
/* 5959 */ 1403,
/* 5960 */ 0,
/* 5961 */ 50447,
/* 5962 */ 5826,
/* 5963 */ 50258,
/* 5964 */ 50040,
/* 5965 */ 5971,
/* 5966 */ 6040,
/* 5967 */ 50041,
/* 5968 */ 50059,
/* 5969 */ -5960,
/* 5970 */ 381,
/* 5971 */ 5979,
/* 5972 */ 5826,
/* 5973 */ 5982,
/* 5974 */ 5990,
/* 5975 */ 5997,
/* 5976 */ 6005,
/* 5977 */ -5971,
/* 5978 */ 1000,
/* 5979 */ 0,
/* 5980 */ -5971,
/* 5981 */ 1411,
/* 5982 */ 5987,
/* 5983 */ 50058,
/* 5984 */ 223,
/* 5985 */ -5982,
/* 5986 */ 993,
/* 5987 */ 0,
/* 5988 */ -5982,
/* 5989 */ 1404,
/* 5990 */ 5994,
/* 5991 */ 50258,
/* 5992 */ -5990,
/* 5993 */ 994,
/* 5994 */ 0,
/* 5995 */ -5990,
/* 5996 */ 1405,
/* 5997 */ 6002,
/* 5998 */ 50061,
/* 5999 */ 173,
/* 6000 */ -5997,
/* 6001 */ 995,
/* 6002 */ 0,
/* 6003 */ -5997,
/* 6004 */ 1406,
/* 6005 */ 6014,
/* 6006 */ 50044,
/* 6007 */ 5826,
/* 6008 */ 6017,
/* 6009 */ 6025,
/* 6010 */ 6032,
/* 6011 */ 6005,
/* 6012 */ -6005,
/* 6013 */ 999,
/* 6014 */ 0,
/* 6015 */ -6005,
/* 6016 */ 1410,
/* 6017 */ 6022,
/* 6018 */ 50058,
/* 6019 */ 223,
/* 6020 */ -6017,
/* 6021 */ 996,
/* 6022 */ 0,
/* 6023 */ -6017,
/* 6024 */ 1407,
/* 6025 */ 6029,
/* 6026 */ 50258,
/* 6027 */ -6025,
/* 6028 */ 997,
/* 6029 */ 0,
/* 6030 */ -6025,
/* 6031 */ 1408,
/* 6032 */ 6037,
/* 6033 */ 50061,
/* 6034 */ 173,
/* 6035 */ -6032,
/* 6036 */ 998,
/* 6037 */ 0,
/* 6038 */ -6032,
/* 6039 */ 1409,
/* 6040 */ 6045,
/* 6041 */ 50044,
/* 6042 */ 50495,
/* 6043 */ -6040,
/* 6044 */ 1001,
/* 6045 */ 0,
/* 6046 */ -6040,
/* 6047 */ 1412,
/* 6048 */ 0,
/* 6049 */ 50447,
/* 6050 */ 5826,
/* 6051 */ 223,
/* 6052 */ 50262,
/* 6053 */ 50258,
/* 6054 */ 50059,
/* 6055 */ -6048,
/* 6056 */ 382,
/* 6057 */ 0,
/* 6058 */ 50447,
/* 6059 */ 223,
/* 6060 */ 50258,
/* 6061 */ 50481,
/* 6062 */ 223,
/* 6063 */ 50059,
/* 6064 */ -6057,
/* 6065 */ 383,
/* 6066 */ 0,
/* 6067 */ 5826,
/* 6068 */ 50258,
/* 6069 */ 6073,
/* 6070 */ 50059,
/* 6071 */ -6066,
/* 6072 */ 384,
/* 6073 */ 6079,
/* 6074 */ 50044,
/* 6075 */ 50258,
/* 6076 */ 6073,
/* 6077 */ -6073,
/* 6078 */ 1002,
/* 6079 */ 0,
/* 6080 */ -6073,
/* 6081 */ 1413,
/* 6082 */ 0,
/* 6083 */ 5826,
/* 6084 */ 50258,
/* 6085 */ 6089,
/* 6086 */ 50059,
/* 6087 */ -6082,
/* 6088 */ 385,
/* 6089 */ 6095,
/* 6090 */ 50044,
/* 6091 */ 50258,
/* 6092 */ 6089,
/* 6093 */ -6089,
/* 6094 */ 1003,
/* 6095 */ 0,
/* 6096 */ -6089,
/* 6097 */ 1414,
/* 6098 */ 0,
/* 6099 */ 5826,
/* 6100 */ 50258,
/* 6101 */ 6105,
/* 6102 */ 50059,
/* 6103 */ -6098,
/* 6104 */ 386,
/* 6105 */ 6111,
/* 6106 */ 50044,
/* 6107 */ 50258,
/* 6108 */ 6105,
/* 6109 */ -6105,
/* 6110 */ 1004,
/* 6111 */ 0,
/* 6112 */ -6105,
/* 6113 */ 1415,
/* 6114 */ 6119,
/* 6115 */ 6715,
/* 6116 */ 50059,
/* 6117 */ -6114,
/* 6118 */ 387,
/* 6119 */ 6129,
/* 6120 */ 50367,
/* 6121 */ 50040,
/* 6122 */ 6715,
/* 6123 */ 50041,
/* 6124 */ 6114,
/* 6125 */ 50335,
/* 6126 */ 6114,
/* 6127 */ -6114,
/* 6128 */ 388,
/* 6129 */ 6137,
/* 6130 */ 50367,
/* 6131 */ 50040,
/* 6132 */ 6715,
/* 6133 */ 50041,
/* 6134 */ 6114,
/* 6135 */ -6114,
/* 6136 */ 389,
/* 6137 */ 6149,
/* 6138 */ 50353,
/* 6139 */ 50040,
/* 6140 */ 6715,
/* 6141 */ 50059,
/* 6142 */ 6715,
/* 6143 */ 50059,
/* 6144 */ 6715,
/* 6145 */ 50041,
/* 6146 */ 6114,
/* 6147 */ -6114,
/* 6148 */ 390,
/* 6149 */ 6160,
/* 6150 */ 50446,
/* 6151 */ 50040,
/* 6152 */ 6715,
/* 6153 */ 50041,
/* 6154 */ 50123,
/* 6155 */ 6203,
/* 6156 */ 6230,
/* 6157 */ 50125,
/* 6158 */ -6114,
/* 6159 */ 391,
/* 6160 */ 6165,
/* 6161 */ 50302,
/* 6162 */ 50059,
/* 6163 */ -6114,
/* 6164 */ 392,
/* 6165 */ 6170,
/* 6166 */ 50314,
/* 6167 */ 50059,
/* 6168 */ -6114,
/* 6169 */ 393,
/* 6170 */ 6180,
/* 6171 */ 50331,
/* 6172 */ 6114,
/* 6173 */ 50469,
/* 6174 */ 50040,
/* 6175 */ 6715,
/* 6176 */ 50041,
/* 6177 */ 50059,
/* 6178 */ -6114,
/* 6179 */ 394,
/* 6180 */ 6188,
/* 6181 */ 50469,
/* 6182 */ 50040,
/* 6183 */ 6715,
/* 6184 */ 50041,
/* 6185 */ 6114,
/* 6186 */ -6114,
/* 6187 */ 395,
/* 6188 */ 6192,
/* 6189 */ 6247,
/* 6190 */ -6114,
/* 6191 */ 396,
/* 6192 */ 6198,
/* 6193 */ 50425,
/* 6194 */ 6715,
/* 6195 */ 50059,
/* 6196 */ -6114,
/* 6197 */ 397,
/* 6198 */ 0,
/* 6199 */ 50425,
/* 6200 */ 50059,
/* 6201 */ -6114,
/* 6202 */ 398,
/* 6203 */ 6211,
/* 6204 */ 50305,
/* 6205 */ 6214,
/* 6206 */ 50058,
/* 6207 */ 6222,
/* 6208 */ 6203,
/* 6209 */ -6203,
/* 6210 */ 1008,
/* 6211 */ 0,
/* 6212 */ -6203,
/* 6213 */ 1417,
/* 6214 */ 6218,
/* 6215 */ 173,
/* 6216 */ -6214,
/* 6217 */ 1005,
/* 6218 */ 0,
/* 6219 */ 163,
/* 6220 */ -6214,
/* 6221 */ 1006,
/* 6222 */ 6227,
/* 6223 */ 6114,
/* 6224 */ 6222,
/* 6225 */ -6222,
/* 6226 */ 1007,
/* 6227 */ 0,
/* 6228 */ -6222,
/* 6229 */ 1416,
/* 6230 */ 6236,
/* 6231 */ 50324,
/* 6232 */ 50058,
/* 6233 */ 6239,
/* 6234 */ -6230,
/* 6235 */ 1010,
/* 6236 */ 0,
/* 6237 */ -6230,
/* 6238 */ 1419,
/* 6239 */ 6244,
/* 6240 */ 6114,
/* 6241 */ 6239,
/* 6242 */ -6239,
/* 6243 */ 1009,
/* 6244 */ 0,
/* 6245 */ -6239,
/* 6246 */ 1418,
/* 6247 */ 0,
/* 6248 */ 50123,
/* 6249 */ 6253,
/* 6250 */ 50125,
/* 6251 */ -6247,
/* 6252 */ 399,
/* 6253 */ 6258,
/* 6254 */ 6114,
/* 6255 */ 6253,
/* 6256 */ -6253,
/* 6257 */ 1011,
/* 6258 */ 0,
/* 6259 */ -6253,
/* 6260 */ 1420,
/* 6261 */ 0,
/* 6262 */ 50123,
/* 6263 */ 6268,
/* 6264 */ 6282,
/* 6265 */ 50125,
/* 6266 */ -6261,
/* 6267 */ 400,
/* 6268 */ 6273,
/* 6269 */ 6082,
/* 6270 */ 6268,
/* 6271 */ -6268,
/* 6272 */ 1012,
/* 6273 */ 6279,
/* 6274 */ 50441,
/* 6275 */ 6098,
/* 6276 */ 6268,
/* 6277 */ -6268,
/* 6278 */ 1013,
/* 6279 */ 0,
/* 6280 */ -6268,
/* 6281 */ 1421,
/* 6282 */ 6287,
/* 6283 */ 6114,
/* 6284 */ 6282,
/* 6285 */ -6282,
/* 6286 */ 1014,
/* 6287 */ 0,
/* 6288 */ -6282,
/* 6289 */ 1422,
/* 6290 */ 6294,
/* 6291 */ 50256,
/* 6292 */ -6290,
/* 6293 */ 401,
/* 6294 */ 6298,
/* 6295 */ 50259,
/* 6296 */ -6290,
/* 6297 */ 402,
/* 6298 */ 6302,
/* 6299 */ 50257,
/* 6300 */ -6290,
/* 6301 */ 403,
/* 6302 */ 6306,
/* 6303 */ 50458,
/* 6304 */ -6290,
/* 6305 */ 404,
/* 6306 */ 6310,
/* 6307 */ 50345,
/* 6308 */ -6290,
/* 6309 */ 405,
/* 6310 */ 6314,
/* 6311 */ 50395,
/* 6312 */ -6290,
/* 6313 */ 406,
/* 6314 */ 6318,
/* 6315 */ 163,
/* 6316 */ -6290,
/* 6317 */ 407,
/* 6318 */ 6324,
/* 6319 */ 50040,
/* 6320 */ 6715,
/* 6321 */ 50041,
/* 6322 */ -6290,
/* 6323 */ 408,
/* 6324 */ 6328,
/* 6325 */ 50258,
/* 6326 */ -6290,
/* 6327 */ 409,
/* 6328 */ 6334,
/* 6329 */ 6290,
/* 6330 */ 6356,
/* 6331 */ 50258,
/* 6332 */ -6290,
/* 6333 */ 410,
/* 6334 */ 6343,
/* 6335 */ 6290,
/* 6336 */ 6364,
/* 6337 */ 50258,
/* 6338 */ 50040,
/* 6339 */ 6725,
/* 6340 */ 50041,
/* 6341 */ -6290,
/* 6342 */ 411,
/* 6343 */ 6350,
/* 6344 */ 50258,
/* 6345 */ 50040,
/* 6346 */ 6725,
/* 6347 */ 50041,
/* 6348 */ -6290,
/* 6349 */ 412,
/* 6350 */ 0,
/* 6351 */ 6290,
/* 6352 */ 50064,
/* 6353 */ 50258,
/* 6354 */ -6290,
/* 6355 */ 413,
/* 6356 */ 6360,
/* 6357 */ 50046,
/* 6358 */ -6356,
/* 6359 */ 1015,
/* 6360 */ 0,
/* 6361 */ 50496,
/* 6362 */ -6356,
/* 6363 */ 1016,
/* 6364 */ 6368,
/* 6365 */ 50046,
/* 6366 */ -6364,
/* 6367 */ 1017,
/* 6368 */ 0,
/* 6369 */ 50496,
/* 6370 */ -6364,
/* 6371 */ 1018,
/* 6372 */ 6376,
/* 6373 */ 50258,
/* 6374 */ -6372,
/* 6375 */ 414,
/* 6376 */ 6382,
/* 6377 */ 6372,
/* 6378 */ 50064,
/* 6379 */ 50258,
/* 6380 */ -6372,
/* 6381 */ 415,
/* 6382 */ 0,
/* 6383 */ 6290,
/* 6384 */ 6388,
/* 6385 */ 50258,
/* 6386 */ -6372,
/* 6387 */ 416,
/* 6388 */ 6392,
/* 6389 */ 50046,
/* 6390 */ -6388,
/* 6391 */ 1019,
/* 6392 */ 0,
/* 6393 */ 50496,
/* 6394 */ -6388,
/* 6395 */ 1020,
/* 6396 */ 6401,
/* 6397 */ 6372,
/* 6398 */ 50475,
/* 6399 */ -6396,
/* 6400 */ 417,
/* 6401 */ 6406,
/* 6402 */ 6372,
/* 6403 */ 50476,
/* 6404 */ -6396,
/* 6405 */ 418,
/* 6406 */ 6415,
/* 6407 */ 50387,
/* 6408 */ 50040,
/* 6409 */ 6645,
/* 6410 */ 50044,
/* 6411 */ 6645,
/* 6412 */ 50041,
/* 6413 */ -6396,
/* 6414 */ 419,
/* 6415 */ 6424,
/* 6416 */ 50384,
/* 6417 */ 50040,
/* 6418 */ 6645,
/* 6419 */ 50044,
/* 6420 */ 6645,
/* 6421 */ 50041,
/* 6422 */ -6396,
/* 6423 */ 420,
/* 6424 */ 0,
/* 6425 */ 6290,
/* 6426 */ -6396,
/* 6427 */ 421,
/* 6428 */ 6433,
/* 6429 */ 50475,
/* 6430 */ 6372,
/* 6431 */ -6428,
/* 6432 */ 422,
/* 6433 */ 6438,
/* 6434 */ 50476,
/* 6435 */ 6372,
/* 6436 */ -6428,
/* 6437 */ 423,
/* 6438 */ 6443,
/* 6439 */ 50043,
/* 6440 */ 6428,
/* 6441 */ -6428,
/* 6442 */ 424,
/* 6443 */ 6448,
/* 6444 */ 50045,
/* 6445 */ 6428,
/* 6446 */ -6428,
/* 6447 */ 425,
/* 6448 */ 6453,
/* 6449 */ 50033,
/* 6450 */ 6428,
/* 6451 */ -6428,
/* 6452 */ 426,
/* 6453 */ 6458,
/* 6454 */ 50126,
/* 6455 */ 6428,
/* 6456 */ -6428,
/* 6457 */ 427,
/* 6458 */ 6464,
/* 6459 */ 50058,
/* 6460 */ 6428,
/* 6461 */ 50058,
/* 6462 */ -6428,
/* 6463 */ 428,
/* 6464 */ 0,
/* 6465 */ 6396,
/* 6466 */ -6428,
/* 6467 */ 429,
/* 6468 */ 6474,
/* 6469 */ 6428,
/* 6470 */ 50100,
/* 6471 */ 6428,
/* 6472 */ -6468,
/* 6473 */ 430,
/* 6474 */ 0,
/* 6475 */ 6428,
/* 6476 */ -6468,
/* 6477 */ 431,
/* 6478 */ 6485,
/* 6479 */ 50040,
/* 6480 */ 5826,
/* 6481 */ 50041,
/* 6482 */ 6478,
/* 6483 */ -6478,
/* 6484 */ 432,
/* 6485 */ 0,
/* 6486 */ 6468,
/* 6487 */ -6478,
/* 6488 */ 433,
/* 6489 */ 6495,
/* 6490 */ 6489,
/* 6491 */ 50042,
/* 6492 */ 6478,
/* 6493 */ -6489,
/* 6494 */ 434,
/* 6495 */ 6501,
/* 6496 */ 6489,
/* 6497 */ 50047,
/* 6498 */ 6478,
/* 6499 */ -6489,
/* 6500 */ 435,
/* 6501 */ 6507,
/* 6502 */ 6489,
/* 6503 */ 50037,
/* 6504 */ 6478,
/* 6505 */ -6489,
/* 6506 */ 436,
/* 6507 */ 0,
/* 6508 */ 6478,
/* 6509 */ -6489,
/* 6510 */ 437,
/* 6511 */ 6517,
/* 6512 */ 6511,
/* 6513 */ 50043,
/* 6514 */ 6489,
/* 6515 */ -6511,
/* 6516 */ 438,
/* 6517 */ 6523,
/* 6518 */ 6511,
/* 6519 */ 50045,
/* 6520 */ 6489,
/* 6521 */ -6511,
/* 6522 */ 439,
/* 6523 */ 0,
/* 6524 */ 6489,
/* 6525 */ -6511,
/* 6526 */ 440,
/* 6527 */ 6533,
/* 6528 */ 6527,
/* 6529 */ 50477,
/* 6530 */ 6511,
/* 6531 */ -6527,
/* 6532 */ 441,
/* 6533 */ 6539,
/* 6534 */ 6527,
/* 6535 */ 50478,
/* 6536 */ 6511,
/* 6537 */ -6527,
/* 6538 */ 442,
/* 6539 */ 0,
/* 6540 */ 6511,
/* 6541 */ -6527,
/* 6542 */ 443,
/* 6543 */ 6549,
/* 6544 */ 6543,
/* 6545 */ 50060,
/* 6546 */ 6527,
/* 6547 */ -6543,
/* 6548 */ 444,
/* 6549 */ 6555,
/* 6550 */ 6543,
/* 6551 */ 50062,
/* 6552 */ 6527,
/* 6553 */ -6543,
/* 6554 */ 445,
/* 6555 */ 6561,
/* 6556 */ 6543,
/* 6557 */ 50479,
/* 6558 */ 6527,
/* 6559 */ -6543,
/* 6560 */ 446,
/* 6561 */ 6567,
/* 6562 */ 6543,
/* 6563 */ 50480,
/* 6564 */ 6527,
/* 6565 */ -6543,
/* 6566 */ 447,
/* 6567 */ 6573,
/* 6568 */ 6543,
/* 6569 */ 50481,
/* 6570 */ 6527,
/* 6571 */ -6543,
/* 6572 */ 448,
/* 6573 */ 6579,
/* 6574 */ 6543,
/* 6575 */ 50482,
/* 6576 */ 6527,
/* 6577 */ -6543,
/* 6578 */ 449,
/* 6579 */ 0,
/* 6580 */ 6527,
/* 6581 */ -6543,
/* 6582 */ 450,
/* 6583 */ 6589,
/* 6584 */ 6583,
/* 6585 */ 50038,
/* 6586 */ 6543,
/* 6587 */ -6583,
/* 6588 */ 451,
/* 6589 */ 0,
/* 6590 */ 6543,
/* 6591 */ -6583,
/* 6592 */ 452,
/* 6593 */ 6599,
/* 6594 */ 6593,
/* 6595 */ 50094,
/* 6596 */ 6583,
/* 6597 */ -6593,
/* 6598 */ 453,
/* 6599 */ 0,
/* 6600 */ 6583,
/* 6601 */ -6593,
/* 6602 */ 454,
/* 6603 */ 6609,
/* 6604 */ 6603,
/* 6605 */ 50124,
/* 6606 */ 6593,
/* 6607 */ -6603,
/* 6608 */ 455,
/* 6609 */ 0,
/* 6610 */ 6593,
/* 6611 */ -6603,
/* 6612 */ 456,
/* 6613 */ 6619,
/* 6614 */ 6613,
/* 6615 */ 50483,
/* 6616 */ 6603,
/* 6617 */ -6613,
/* 6618 */ 457,
/* 6619 */ 0,
/* 6620 */ 6603,
/* 6621 */ -6613,
/* 6622 */ 458,
/* 6623 */ 6629,
/* 6624 */ 6623,
/* 6625 */ 50484,
/* 6626 */ 6613,
/* 6627 */ -6623,
/* 6628 */ 459,
/* 6629 */ 0,
/* 6630 */ 6613,
/* 6631 */ -6623,
/* 6632 */ 460,
/* 6633 */ 6641,
/* 6634 */ 6623,
/* 6635 */ 50063,
/* 6636 */ 6715,
/* 6637 */ 50058,
/* 6638 */ 6633,
/* 6639 */ -6633,
/* 6640 */ 461,
/* 6641 */ 0,
/* 6642 */ 6623,
/* 6643 */ -6633,
/* 6644 */ 462,
/* 6645 */ 6651,
/* 6646 */ 6372,
/* 6647 */ 50061,
/* 6648 */ 6633,
/* 6649 */ -6645,
/* 6650 */ 463,
/* 6651 */ 6657,
/* 6652 */ 6372,
/* 6653 */ 50486,
/* 6654 */ 6633,
/* 6655 */ -6645,
/* 6656 */ 464,
/* 6657 */ 6663,
/* 6658 */ 6372,
/* 6659 */ 50494,
/* 6660 */ 6633,
/* 6661 */ -6645,
/* 6662 */ 465,
/* 6663 */ 6669,
/* 6664 */ 6372,
/* 6665 */ 50485,
/* 6666 */ 6633,
/* 6667 */ -6645,
/* 6668 */ 466,
/* 6669 */ 6675,
/* 6670 */ 6372,
/* 6671 */ 50487,
/* 6672 */ 6633,
/* 6673 */ -6645,
/* 6674 */ 467,
/* 6675 */ 6681,
/* 6676 */ 6372,
/* 6677 */ 50488,
/* 6678 */ 6633,
/* 6679 */ -6645,
/* 6680 */ 468,
/* 6681 */ 6687,
/* 6682 */ 6372,
/* 6683 */ 50489,
/* 6684 */ 6633,
/* 6685 */ -6645,
/* 6686 */ 469,
/* 6687 */ 6693,
/* 6688 */ 6372,
/* 6689 */ 50490,
/* 6690 */ 6633,
/* 6691 */ -6645,
/* 6692 */ 470,
/* 6693 */ 6699,
/* 6694 */ 6372,
/* 6695 */ 50492,
/* 6696 */ 6633,
/* 6697 */ -6645,
/* 6698 */ 471,
/* 6699 */ 6705,
/* 6700 */ 6372,
/* 6701 */ 50493,
/* 6702 */ 6633,
/* 6703 */ -6645,
/* 6704 */ 472,
/* 6705 */ 6711,
/* 6706 */ 6372,
/* 6707 */ 50491,
/* 6708 */ 6633,
/* 6709 */ -6645,
/* 6710 */ 473,
/* 6711 */ 0,
/* 6712 */ 6633,
/* 6713 */ -6645,
/* 6714 */ 474,
/* 6715 */ 6721,
/* 6716 */ 6715,
/* 6717 */ 50044,
/* 6718 */ 6645,
/* 6719 */ -6715,
/* 6720 */ 475,
/* 6721 */ 0,
/* 6722 */ 6645,
/* 6723 */ -6715,
/* 6724 */ 476,
/* 6725 */ 0,
/* 6726 */ 6729,
/* 6727 */ -6725,
/* 6728 */ 477,
/* 6729 */ 6734,
/* 6730 */ 6645,
/* 6731 */ 6737,
/* 6732 */ -6729,
/* 6733 */ 1022,
/* 6734 */ 0,
/* 6735 */ -6729,
/* 6736 */ 1424,
/* 6737 */ 6743,
/* 6738 */ 50044,
/* 6739 */ 6645,
/* 6740 */ 6737,
/* 6741 */ -6737,
/* 6742 */ 1021,
/* 6743 */ 0,
/* 6744 */ -6737,
/* 6745 */ 1423,
0
};
extern int yyannotation[];
int yyannotation[] = {
0,
/* 1 */ 0,
/* 2 */ 0,
/* 3 */ 50000,
/* 4 */ -1,
/* 5 */ 0,
/* 6 */ 0,
/* 7 */ 1,
/* 8 */ -6,
/* 9 */ 1,
/* 10 */ 15,
/* 11 */ 1,
/* 12 */ 1,
/* 13 */ -10,
/* 14 */ 1,
/* 15 */ 0,
/* 16 */ -10,
/* 17 */ 2,
/* 18 */ 24,
/* 19 */ 50380,
/* 20 */ 50257,
/* 21 */ 50059,
/* 22 */ -18,
/* 23 */ 1,
/* 24 */ 30,
/* 25 */ 50433,
/* 26 */ 1,
/* 27 */ 50059,
/* 28 */ -18,
/* 29 */ 2,
/* 30 */ 36,
/* 31 */ 50352,
/* 32 */ 50257,
/* 33 */ 50059,
/* 34 */ -18,
/* 35 */ 3,
/* 36 */ 40,
/* 37 */ 1,
/* 38 */ -18,
/* 39 */ 4,
/* 40 */ 44,
/* 41 */ 1,
/* 42 */ -18,
/* 43 */ 5,
/* 44 */ 48,
/* 45 */ 1,
/* 46 */ -18,
/* 47 */ 6,
/* 48 */ 52,
/* 49 */ 1,
/* 50 */ -18,
/* 51 */ 7,
/* 52 */ 56,
/* 53 */ 1,
/* 54 */ -18,
/* 55 */ 8,
/* 56 */ 60,
/* 57 */ 1,
/* 58 */ -18,
/* 59 */ 9,
/* 60 */ 64,
/* 61 */ 1,
/* 62 */ -18,
/* 63 */ 10,
/* 64 */ 68,
/* 65 */ 1,
/* 66 */ -18,
/* 67 */ 11,
/* 68 */ 72,
/* 69 */ 1,
/* 70 */ -18,
/* 71 */ 12,
/* 72 */ 76,
/* 73 */ 1,
/* 74 */ -18,
/* 75 */ 13,
/* 76 */ 80,
/* 77 */ 1,
/* 78 */ -18,
/* 79 */ 14,
/* 80 */ 84,
/* 81 */ 1,
/* 82 */ -18,
/* 83 */ 15,
/* 84 */ 88,
/* 85 */ 1,
/* 86 */ -18,
/* 87 */ 16,
/* 88 */ 92,
/* 89 */ 1,
/* 90 */ -18,
/* 91 */ 17,
/* 92 */ 96,
/* 93 */ 1,
/* 94 */ -18,
/* 95 */ 18,
/* 96 */ 100,
/* 97 */ 1,
/* 98 */ -18,
/* 99 */ 19,
/* 100 */ 104,
/* 101 */ 1,
/* 102 */ -18,
/* 103 */ 20,
/* 104 */ 108,
/* 105 */ 1,
/* 106 */ -18,
/* 107 */ 21,
/* 108 */ 112,
/* 109 */ 1,
/* 110 */ -18,
/* 111 */ 22,
/* 112 */ 116,
/* 113 */ 1,
/* 114 */ -18,
/* 115 */ 23,
/* 116 */ 120,
/* 117 */ 1,
/* 118 */ -18,
/* 119 */ 24,
/* 120 */ 124,
/* 121 */ 1,
/* 122 */ -18,
/* 123 */ 25,
/* 124 */ 128,
/* 125 */ 1,
/* 126 */ -18,
/* 127 */ 26,
/* 128 */ 134,
/* 129 */ 50350,
/* 130 */ 1,
/* 131 */ 1,
/* 132 */ -18,
/* 133 */ 27,
/* 134 */ 0,
/* 135 */ 50451,
/* 136 */ 50257,
/* 137 */ 50123,
/* 138 */ 1,
/* 139 */ 50257,
/* 140 */ 1,
/* 141 */ 50125,
/* 142 */ -18,
/* 143 */ 28,
/* 144 */ 0,
/* 145 */ 50058,
/* 146 */ -144,
/* 147 */ 1,
/* 148 */ 153,
/* 149 */ 50326,
/* 150 */ 50058,
/* 151 */ -148,
/* 152 */ 1,
/* 153 */ 0,
/* 154 */ -148,
/* 155 */ 2,
/* 156 */ 160,
/* 157 */ 50059,
/* 158 */ -156,
/* 159 */ 1,
/* 160 */ 0,
/* 161 */ -156,
/* 162 */ 2,
/* 163 */ 168,
/* 164 */ 50036,
/* 165 */ 50257,
/* 166 */ -163,
/* 167 */ 1,
/* 168 */ 0,
/* 169 */ 50036,
/* 170 */ 1,
/* 171 */ -163,
/* 172 */ 2,
/* 173 */ 179,
/* 174 */ 1,
/* 175 */ 50043,
/* 176 */ 1,
/* 177 */ -173,
/* 178 */ 1,
/* 179 */ 185,
/* 180 */ 1,
/* 181 */ 50045,
/* 182 */ 1,
/* 183 */ -173,
/* 184 */ 2,
/* 185 */ 0,
/* 186 */ 1,
/* 187 */ -173,
/* 188 */ 3,
/* 189 */ 195,
/* 190 */ 1,
/* 191 */ 50042,
/* 192 */ 1,
/* 193 */ -189,
/* 194 */ 1,
/* 195 */ 201,
/* 196 */ 1,
/* 197 */ 50047,
/* 198 */ 1,
/* 199 */ -189,
/* 200 */ 2,
/* 201 */ 207,
/* 202 */ 1,
/* 203 */ 50037,
/* 204 */ 1,
/* 205 */ -189,
/* 206 */ 3,
/* 207 */ 213,
/* 208 */ 1,
/* 209 */ 50124,
/* 210 */ 1,
/* 211 */ -189,
/* 212 */ 4,
/* 213 */ 219,
/* 214 */ 1,
/* 215 */ 50038,
/* 216 */ 1,
/* 217 */ -189,
/* 218 */ 5,
/* 219 */ 0,
/* 220 */ 1,
/* 221 */ -189,
/* 222 */ 6,
/* 223 */ 228,
/* 224 */ 50043,
/* 225 */ 1,
/* 226 */ -223,
/* 227 */ 1,
/* 228 */ 233,
/* 229 */ 50045,
/* 230 */ 1,
/* 231 */ -223,
/* 232 */ 2,
/* 233 */ 0,
/* 234 */ 1,
/* 235 */ -223,
/* 236 */ 3,
/* 237 */ 243,
/* 238 */ 50040,
/* 239 */ 1,
/* 240 */ 50041,
/* 241 */ -237,
/* 242 */ 1,
/* 243 */ 247,
/* 244 */ 50256,
/* 245 */ -237,
/* 246 */ 2,
/* 247 */ 251,
/* 248 */ 50458,
/* 249 */ -237,
/* 250 */ 3,
/* 251 */ 255,
/* 252 */ 50345,
/* 253 */ -237,
/* 254 */ 4,
/* 255 */ 0,
/* 256 */ 50259,
/* 257 */ -237,
/* 258 */ 5,
/* 259 */ 267,
/* 260 */ 1,
/* 261 */ 50100,
/* 262 */ 1,
/* 263 */ 50043,
/* 264 */ 1,
/* 265 */ -259,
/* 266 */ 1,
/* 267 */ 276,
/* 268 */ 1,
/* 269 */ 50100,
/* 270 */ 1,
/* 271 */ 50043,
/* 272 */ 50045,
/* 273 */ 1,
/* 274 */ -259,
/* 275 */ 2,
/* 276 */ 282,
/* 277 */ 1,
/* 278 */ 50100,
/* 279 */ 1,
/* 280 */ -259,
/* 281 */ 3,
/* 282 */ 0,
/* 283 */ 1,
/* 284 */ -259,
/* 285 */ 4,
/* 286 */ 294,
/* 287 */ 1,
/* 288 */ 50100,
/* 289 */ 1,
/* 290 */ 50043,
/* 291 */ 1,
/* 292 */ -286,
/* 293 */ 1,
/* 294 */ 0,
/* 295 */ 1,
/* 296 */ 50100,
/* 297 */ 1,
/* 298 */ -286,
/* 299 */ 2,
/* 300 */ 0,
/* 301 */ 1,
/* 302 */ 1,
/* 303 */ 1,
/* 304 */ 1,
/* 305 */ 1,
/* 306 */ 1,
/* 307 */ 1,
/* 308 */ -300,
/* 309 */ 1,
/* 310 */ 317,
/* 311 */ 50367,
/* 312 */ 50040,
/* 313 */ 1,
/* 314 */ 50041,
/* 315 */ -310,
/* 316 */ 1,
/* 317 */ 0,
/* 318 */ -310,
/* 319 */ 2,
/* 320 */ 325,
/* 321 */ 50256,
/* 322 */ 50037,
/* 323 */ -320,
/* 324 */ 1,
/* 325 */ 0,
/* 326 */ -320,
/* 327 */ 2,
/* 328 */ 332,
/* 329 */ 1,
/* 330 */ -328,
/* 331 */ 1,
/* 332 */ 0,
/* 333 */ -328,
/* 334 */ 2,
/* 335 */ 339,
/* 336 */ 50317,
/* 337 */ -335,
/* 338 */ 1,
/* 339 */ 343,
/* 340 */ 50300,
/* 341 */ -335,
/* 342 */ 2,
/* 343 */ 0,
/* 344 */ -335,
/* 345 */ 3,
/* 346 */ 350,
/* 347 */ 1,
/* 348 */ -346,
/* 349 */ 1,
/* 350 */ 0,
/* 351 */ 50419,
/* 352 */ 1,
/* 353 */ -346,
/* 354 */ 2,
/* 355 */ 360,
/* 356 */ 50043,
/* 357 */ 50256,
/* 358 */ -355,
/* 359 */ 1,
/* 360 */ 365,
/* 361 */ 50045,
/* 362 */ 50256,
/* 363 */ -355,
/* 364 */ 2,
/* 365 */ 0,
/* 366 */ -355,
/* 367 */ 3,
/* 368 */ 376,
/* 369 */ 50471,
/* 370 */ 50091,
/* 371 */ 1,
/* 372 */ 50093,
/* 373 */ 1,
/* 374 */ -368,
/* 375 */ 1,
/* 376 */ 382,
/* 377 */ 50399,
/* 378 */ 1,
/* 379 */ 1,
/* 380 */ -368,
/* 381 */ 2,
/* 382 */ 391,
/* 383 */ 50399,
/* 384 */ 50419,
/* 385 */ 1,
/* 386 */ 50457,
/* 387 */ 1,
/* 388 */ 1,
/* 389 */ -368,
/* 390 */ 3,
/* 391 */ 398,
/* 392 */ 50399,
/* 393 */ 50419,
/* 394 */ 1,
/* 395 */ 1,
/* 396 */ -368,
/* 397 */ 4,
/* 398 */ 404,
/* 399 */ 50399,
/* 400 */ 50419,
/* 401 */ 1,
/* 402 */ -368,
/* 403 */ 5,
/* 404 */ 414,
/* 405 */ 50295,
/* 406 */ 50040,
/* 407 */ 1,
/* 408 */ 50044,
/* 409 */ 1,
/* 410 */ 50041,
/* 411 */ 1,
/* 412 */ -368,
/* 413 */ 6,
/* 414 */ 0,
/* 415 */ -368,
/* 416 */ 7,
/* 417 */ 423,
/* 418 */ 1,
/* 419 */ 1,
/* 420 */ 1,
/* 421 */ -417,
/* 422 */ 1,
/* 423 */ 0,
/* 424 */ -417,
/* 425 */ 2,
/* 426 */ 430,
/* 427 */ 50044,
/* 428 */ -426,
/* 429 */ 1,
/* 430 */ 0,
/* 431 */ -426,
/* 432 */ 2,
/* 433 */ 0,
/* 434 */ 50356,
/* 435 */ 1,
/* 436 */ 1,
/* 437 */ 1,
/* 438 */ 50059,
/* 439 */ -433,
/* 440 */ 1,
/* 441 */ 0,
/* 442 */ 50058,
/* 443 */ -441,
/* 444 */ 1,
/* 445 */ 451,
/* 446 */ 1,
/* 447 */ 1,
/* 448 */ 1,
/* 449 */ -445,
/* 450 */ 1,
/* 451 */ 0,
/* 452 */ -445,
/* 453 */ 2,
/* 454 */ 458,
/* 455 */ 50044,
/* 456 */ -454,
/* 457 */ 1,
/* 458 */ 0,
/* 459 */ 50335,
/* 460 */ -454,
/* 461 */ 2,
/* 462 */ 0,
/* 463 */ 1,
/* 464 */ 1,
/* 465 */ 1,
/* 466 */ 1,
/* 467 */ 1,
/* 468 */ 50059,
/* 469 */ -462,
/* 470 */ 1,
/* 471 */ 475,
/* 472 */ 50368,
/* 473 */ -471,
/* 474 */ 1,
/* 475 */ 0,
/* 476 */ 50357,
/* 477 */ -471,
/* 478 */ 2,
/* 479 */ 483,
/* 480 */ 50058,
/* 481 */ -479,
/* 482 */ 1,
/* 483 */ 0,
/* 484 */ -479,
/* 485 */ 2,
/* 486 */ 490,
/* 487 */ 1,
/* 488 */ -486,
/* 489 */ 1,
/* 490 */ 0,
/* 491 */ -486,
/* 492 */ 2,
/* 493 */ 497,
/* 494 */ 50259,
/* 495 */ -493,
/* 496 */ 1,
/* 497 */ 0,
/* 498 */ 50256,
/* 499 */ -493,
/* 500 */ 2,
/* 501 */ 506,
/* 502 */ 50400,
/* 503 */ 1,
/* 504 */ -501,
/* 505 */ 1,
/* 506 */ 0,
/* 507 */ -501,
/* 508 */ 2,
/* 509 */ 0,
/* 510 */ 1,
/* 511 */ 1,
/* 512 */ 1,
/* 513 */ -509,
/* 514 */ 1,
/* 515 */ 519,
/* 516 */ 1,
/* 517 */ -515,
/* 518 */ 1,
/* 519 */ 0,
/* 520 */ -515,
/* 521 */ 2,
/* 522 */ 526,
/* 523 */ 50259,
/* 524 */ -522,
/* 525 */ 1,
/* 526 */ 0,
/* 527 */ 50256,
/* 528 */ -522,
/* 529 */ 2,
/* 530 */ 535,
/* 531 */ 50400,
/* 532 */ 1,
/* 533 */ -530,
/* 534 */ 1,
/* 535 */ 0,
/* 536 */ -530,
/* 537 */ 2,
/* 538 */ 0,
/* 539 */ 50400,
/* 540 */ 50342,
/* 541 */ 1,
/* 542 */ 1,
/* 543 */ 50059,
/* 544 */ -538,
/* 545 */ 1,
/* 546 */ 552,
/* 547 */ 50044,
/* 548 */ 1,
/* 549 */ 1,
/* 550 */ -546,
/* 551 */ 1,
/* 552 */ 0,
/* 553 */ -546,
/* 554 */ 2,
/* 555 */ 0,
/* 556 */ 1,
/* 557 */ 1,
/* 558 */ 1,
/* 559 */ -555,
/* 560 */ 1,
/* 561 */ 567,
/* 562 */ 50044,
/* 563 */ 1,
/* 564 */ 1,
/* 565 */ -561,
/* 566 */ 1,
/* 567 */ 0,
/* 568 */ -561,
/* 569 */ 2,
/* 570 */ 575,
/* 571 */ 50257,
/* 572 */ 1,
/* 573 */ -570,
/* 574 */ 1,
/* 575 */ 0,
/* 576 */ 1,
/* 577 */ -570,
/* 578 */ 2,
/* 579 */ 585,
/* 580 */ 50047,
/* 581 */ 50257,
/* 582 */ 1,
/* 583 */ -579,
/* 584 */ 1,
/* 585 */ 0,
/* 586 */ -579,
/* 587 */ 2,
/* 588 */ 0,
/* 589 */ 1,
/* 590 */ -588,
/* 591 */ 1,
/* 592 */ 0,
/* 593 */ 1,
/* 594 */ -592,
/* 595 */ 1,
/* 596 */ 0,
/* 597 */ 1,
/* 598 */ 1,
/* 599 */ -596,
/* 600 */ 1,
/* 601 */ 606,
/* 602 */ 1,
/* 603 */ 1,
/* 604 */ -601,
/* 605 */ 1,
/* 606 */ 611,
/* 607 */ 50045,
/* 608 */ 1,
/* 609 */ -601,
/* 610 */ 2,
/* 611 */ 616,
/* 612 */ 50061,
/* 613 */ 1,
/* 614 */ -601,
/* 615 */ 3,
/* 616 */ 0,
/* 617 */ 1,
/* 618 */ 50037,
/* 619 */ -601,
/* 620 */ 4,
/* 621 */ 625,
/* 622 */ 50043,
/* 623 */ -621,
/* 624 */ 1,
/* 625 */ 0,
/* 626 */ -621,
/* 627 */ 2,
/* 628 */ 634,
/* 629 */ 50040,
/* 630 */ 1,
/* 631 */ 50041,
/* 632 */ -628,
/* 633 */ 1,
/* 634 */ 0,
/* 635 */ -628,
/* 636 */ 2,
/* 637 */ 642,
/* 638 */ 50387,
/* 639 */ 1,
/* 640 */ -637,
/* 641 */ 1,
/* 642 */ 647,
/* 643 */ 50384,
/* 644 */ 1,
/* 645 */ -637,
/* 646 */ 2,
/* 647 */ 0,
/* 648 */ 50391,
/* 649 */ 1,
/* 650 */ -637,
/* 651 */ 3,
/* 652 */ 0,
/* 653 */ 50388,
/* 654 */ 50257,
/* 655 */ 50058,
/* 656 */ 1,
/* 657 */ 1,
/* 658 */ 50123,
/* 659 */ 1,
/* 660 */ 50125,
/* 661 */ -652,
/* 662 */ 1,
/* 663 */ 669,
/* 664 */ 50044,
/* 665 */ 1,
/* 666 */ 1,
/* 667 */ -663,
/* 668 */ 1,
/* 669 */ 0,
/* 670 */ -663,
/* 671 */ 2,
/* 672 */ 677,
/* 673 */ 50044,
/* 674 */ 1,
/* 675 */ -672,
/* 676 */ 1,
/* 677 */ 0,
/* 678 */ -672,
/* 679 */ 2,
/* 680 */ 685,
/* 681 */ 1,
/* 682 */ 1,
/* 683 */ -680,
/* 684 */ 1,
/* 685 */ 0,
/* 686 */ -680,
/* 687 */ 2,
/* 688 */ 695,
/* 689 */ 50326,
/* 690 */ 1,
/* 691 */ 50257,
/* 692 */ 50059,
/* 693 */ -688,
/* 694 */ 1,
/* 695 */ 702,
/* 696 */ 50325,
/* 697 */ 1,
/* 698 */ 1,
/* 699 */ 50059,
/* 700 */ -688,
/* 701 */ 2,
/* 702 */ 709,
/* 703 */ 1,
/* 704 */ 1,
/* 705 */ 1,
/* 706 */ 50059,
/* 707 */ -688,
/* 708 */ 3,
/* 709 */ 718,
/* 710 */ 1,
/* 711 */ 1,
/* 712 */ 1,
/* 713 */ 50047,
/* 714 */ 1,
/* 715 */ 50059,
/* 716 */ -688,
/* 717 */ 4,
/* 718 */ 726,
/* 719 */ 50450,
/* 720 */ 1,
/* 721 */ 1,
/* 722 */ 1,
/* 723 */ 50059,
/* 724 */ -688,
/* 725 */ 5,
/* 726 */ 733,
/* 727 */ 50364,
/* 728 */ 1,
/* 729 */ 1,
/* 730 */ 50059,
/* 731 */ -688,
/* 732 */ 6,
/* 733 */ 740,
/* 734 */ 50365,
/* 735 */ 1,
/* 736 */ 1,
/* 737 */ 50059,
/* 738 */ -688,
/* 739 */ 7,
/* 740 */ 748,
/* 741 */ 1,
/* 742 */ 50366,
/* 743 */ 1,
/* 744 */ 1,
/* 745 */ 50059,
/* 746 */ -688,
/* 747 */ 8,
/* 748 */ 752,
/* 749 */ 1,
/* 750 */ -688,
/* 751 */ 9,
/* 752 */ 760,
/* 753 */ 50440,
/* 754 */ 50091,
/* 755 */ 1,
/* 756 */ 50093,
/* 757 */ 50059,
/* 758 */ -688,
/* 759 */ 10,
/* 760 */ 770,
/* 761 */ 50440,
/* 762 */ 50091,
/* 763 */ 1,
/* 764 */ 50044,
/* 765 */ 1,
/* 766 */ 50093,
/* 767 */ 50059,
/* 768 */ -688,
/* 769 */ 11,
/* 770 */ 782,
/* 771 */ 50440,
/* 772 */ 50091,
/* 773 */ 1,
/* 774 */ 50044,
/* 775 */ 1,
/* 776 */ 50044,
/* 777 */ 1,
/* 778 */ 50093,
/* 779 */ 50059,
/* 780 */ -688,
/* 781 */ 12,
/* 782 */ 796,
/* 783 */ 50440,
/* 784 */ 50091,
/* 785 */ 1,
/* 786 */ 50044,
/* 787 */ 1,
/* 788 */ 50044,
/* 789 */ 1,
/* 790 */ 50044,
/* 791 */ 1,
/* 792 */ 50093,
/* 793 */ 50059,
/* 794 */ -688,
/* 795 */ 13,
/* 796 */ 803,
/* 797 */ 50424,
/* 798 */ 1,
/* 799 */ 1,
/* 800 */ 50059,
/* 801 */ -688,
/* 802 */ 14,
/* 803 */ 810,
/* 804 */ 50369,
/* 805 */ 1,
/* 806 */ 1,
/* 807 */ 50059,
/* 808 */ -688,
/* 809 */ 15,
/* 810 */ 818,
/* 811 */ 50296,
/* 812 */ 1,
/* 813 */ 1,
/* 814 */ 1,
/* 815 */ 50059,
/* 816 */ -688,
/* 817 */ 16,
/* 818 */ 824,
/* 819 */ 50294,
/* 820 */ 50257,
/* 821 */ 50059,
/* 822 */ -688,
/* 823 */ 17,
/* 824 */ 830,
/* 825 */ 50294,
/* 826 */ 1,
/* 827 */ 50059,
/* 828 */ -688,
/* 829 */ 18,
/* 830 */ 837,
/* 831 */ 50323,
/* 832 */ 1,
/* 833 */ 1,
/* 834 */ 50059,
/* 835 */ -688,
/* 836 */ 19,
/* 837 */ 844,
/* 838 */ 50292,
/* 839 */ 1,
/* 840 */ 1,
/* 841 */ 50059,
/* 842 */ -688,
/* 843 */ 20,
/* 844 */ 851,
/* 845 */ 50381,
/* 846 */ 1,
/* 847 */ 1,
/* 848 */ 50059,
/* 849 */ -688,
/* 850 */ 21,
/* 851 */ 859,
/* 852 */ 50346,
/* 853 */ 1,
/* 854 */ 1,
/* 855 */ 1,
/* 856 */ 50059,
/* 857 */ -688,
/* 858 */ 22,
/* 859 */ 866,
/* 860 */ 50350,
/* 861 */ 1,
/* 862 */ 1,
/* 863 */ 50059,
/* 864 */ -688,
/* 865 */ 23,
/* 866 */ 872,
/* 867 */ 1,
/* 868 */ 1,
/* 869 */ 50059,
/* 870 */ -688,
/* 871 */ 24,
/* 872 */ 879,
/* 873 */ 50431,
/* 874 */ 1,
/* 875 */ 1,
/* 876 */ 50059,
/* 877 */ -688,
/* 878 */ 25,
/* 879 */ 887,
/* 880 */ 50435,
/* 881 */ 1,
/* 882 */ 1,
/* 883 */ 1,
/* 884 */ 50059,
/* 885 */ -688,
/* 886 */ 26,
/* 887 */ 895,
/* 888 */ 50389,
/* 889 */ 1,
/* 890 */ 1,
/* 891 */ 1,
/* 892 */ 50059,
/* 893 */ -688,
/* 894 */ 27,
/* 895 */ 902,
/* 896 */ 50437,
/* 897 */ 1,
/* 898 */ 1,
/* 899 */ 50059,
/* 900 */ -688,
/* 901 */ 28,
/* 902 */ 909,
/* 903 */ 50330,
/* 904 */ 1,
/* 905 */ 1,
/* 906 */ 50059,
/* 907 */ -688,
/* 908 */ 29,
/* 909 */ 916,
/* 910 */ 50409,
/* 911 */ 1,
/* 912 */ 1,
/* 913 */ 50059,
/* 914 */ -688,
/* 915 */ 30,
/* 916 */ 920,
/* 917 */ 1,
/* 918 */ -688,
/* 919 */ 31,
/* 920 */ 924,
/* 921 */ 1,
/* 922 */ -688,
/* 923 */ 32,
/* 924 */ 928,
/* 925 */ 1,
/* 926 */ -688,
/* 927 */ 33,
/* 928 */ 0,
/* 929 */ 1,
/* 930 */ -688,
/* 931 */ 34,
/* 932 */ 936,
/* 933 */ 50058,
/* 934 */ -932,
/* 935 */ 1,
/* 936 */ 0,
/* 937 */ -932,
/* 938 */ 2,
/* 939 */ 943,
/* 940 */ 50058,
/* 941 */ -939,
/* 942 */ 1,
/* 943 */ 0,
/* 944 */ -939,
/* 945 */ 2,
/* 946 */ 950,
/* 947 */ 50374,
/* 948 */ -946,
/* 949 */ 1,
/* 950 */ 0,
/* 951 */ 50307,
/* 952 */ -946,
/* 953 */ 2,
/* 954 */ 958,
/* 955 */ 50058,
/* 956 */ -954,
/* 957 */ 1,
/* 958 */ 0,
/* 959 */ -954,
/* 960 */ 2,
/* 961 */ 965,
/* 962 */ 50374,
/* 963 */ -961,
/* 964 */ 1,
/* 965 */ 0,
/* 966 */ 50307,
/* 967 */ -961,
/* 968 */ 2,
/* 969 */ 973,
/* 970 */ 50058,
/* 971 */ -969,
/* 972 */ 1,
/* 973 */ 0,
/* 974 */ -969,
/* 975 */ 2,
/* 976 */ 980,
/* 977 */ 50058,
/* 978 */ -976,
/* 979 */ 1,
/* 980 */ 0,
/* 981 */ -976,
/* 982 */ 2,
/* 983 */ 989,
/* 984 */ 50044,
/* 985 */ 1,
/* 986 */ 1,
/* 987 */ -983,
/* 988 */ 1,
/* 989 */ 0,
/* 990 */ -983,
/* 991 */ 2,
/* 992 */ 996,
/* 993 */ 50058,
/* 994 */ -992,
/* 995 */ 1,
/* 996 */ 0,
/* 997 */ -992,
/* 998 */ 2,
/* 999 */ 1003,
/* 1000 */ 50058,
/* 1001 */ -999,
/* 1002 */ 1,
/* 1003 */ 0,
/* 1004 */ -999,
/* 1005 */ 2,
/* 1006 */ 1010,
/* 1007 */ 50043,
/* 1008 */ -1006,
/* 1009 */ 1,
/* 1010 */ 0,
/* 1011 */ -1006,
/* 1012 */ 2,
/* 1013 */ 1017,
/* 1014 */ 50058,
/* 1015 */ -1013,
/* 1016 */ 1,
/* 1017 */ 0,
/* 1018 */ -1013,
/* 1019 */ 2,
/* 1020 */ 1024,
/* 1021 */ 50058,
/* 1022 */ -1020,
/* 1023 */ 1,
/* 1024 */ 0,
/* 1025 */ -1020,
/* 1026 */ 2,
/* 1027 */ 1033,
/* 1028 */ 1,
/* 1029 */ 1,
/* 1030 */ 1,
/* 1031 */ -1027,
/* 1032 */ 1,
/* 1033 */ 0,
/* 1034 */ -1027,
/* 1035 */ 2,
/* 1036 */ 1040,
/* 1037 */ 50044,
/* 1038 */ -1036,
/* 1039 */ 1,
/* 1040 */ 0,
/* 1041 */ -1036,
/* 1042 */ 2,
/* 1043 */ 1047,
/* 1044 */ 50058,
/* 1045 */ -1043,
/* 1046 */ 1,
/* 1047 */ 0,
/* 1048 */ -1043,
/* 1049 */ 2,
/* 1050 */ 1056,
/* 1051 */ 1,
/* 1052 */ 1,
/* 1053 */ 1,
/* 1054 */ -1050,
/* 1055 */ 1,
/* 1056 */ 0,
/* 1057 */ -1050,
/* 1058 */ 2,
/* 1059 */ 1063,
/* 1060 */ 50044,
/* 1061 */ -1059,
/* 1062 */ 1,
/* 1063 */ 0,
/* 1064 */ -1059,
/* 1065 */ 2,
/* 1066 */ 1070,
/* 1067 */ 50058,
/* 1068 */ -1066,
/* 1069 */ 1,
/* 1070 */ 0,
/* 1071 */ -1066,
/* 1072 */ 2,
/* 1073 */ 1079,
/* 1074 */ 50044,
/* 1075 */ 1,
/* 1076 */ 1,
/* 1077 */ -1073,
/* 1078 */ 1,
/* 1079 */ 0,
/* 1080 */ -1073,
/* 1081 */ 2,
/* 1082 */ 1086,
/* 1083 */ 50058,
/* 1084 */ -1082,
/* 1085 */ 1,
/* 1086 */ 0,
/* 1087 */ -1082,
/* 1088 */ 2,
/* 1089 */ 1093,
/* 1090 */ 50058,
/* 1091 */ -1089,
/* 1092 */ 1,
/* 1093 */ 0,
/* 1094 */ -1089,
/* 1095 */ 2,
/* 1096 */ 1100,
/* 1097 */ 50058,
/* 1098 */ -1096,
/* 1099 */ 1,
/* 1100 */ 0,
/* 1101 */ -1096,
/* 1102 */ 2,
/* 1103 */ 1107,
/* 1104 */ 50058,
/* 1105 */ -1103,
/* 1106 */ 1,
/* 1107 */ 0,
/* 1108 */ -1103,
/* 1109 */ 2,
/* 1110 */ 1116,
/* 1111 */ 1,
/* 1112 */ 50044,
/* 1113 */ 1,
/* 1114 */ -1110,
/* 1115 */ 1,
/* 1116 */ 0,
/* 1117 */ -1110,
/* 1118 */ 2,
/* 1119 */ 1123,
/* 1120 */ 50058,
/* 1121 */ -1119,
/* 1122 */ 1,
/* 1123 */ 0,
/* 1124 */ -1119,
/* 1125 */ 2,
/* 1126 */ 1131,
/* 1127 */ 1,
/* 1128 */ 1,
/* 1129 */ -1126,
/* 1130 */ 1,
/* 1131 */ 0,
/* 1132 */ -1126,
/* 1133 */ 2,
/* 1134 */ 1140,
/* 1135 */ 50044,
/* 1136 */ 1,
/* 1137 */ 1,
/* 1138 */ -1134,
/* 1139 */ 1,
/* 1140 */ 0,
/* 1141 */ -1134,
/* 1142 */ 2,
/* 1143 */ 1147,
/* 1144 */ 50058,
/* 1145 */ -1143,
/* 1146 */ 1,
/* 1147 */ 0,
/* 1148 */ -1143,
/* 1149 */ 2,
/* 1150 */ 1154,
/* 1151 */ 50058,
/* 1152 */ -1150,
/* 1153 */ 1,
/* 1154 */ 0,
/* 1155 */ -1150,
/* 1156 */ 2,
/* 1157 */ 1161,
/* 1158 */ 50037,
/* 1159 */ -1157,
/* 1160 */ 1,
/* 1161 */ 0,
/* 1162 */ -1157,
/* 1163 */ 2,
/* 1164 */ 1168,
/* 1165 */ 50058,
/* 1166 */ -1164,
/* 1167 */ 1,
/* 1168 */ 0,
/* 1169 */ -1164,
/* 1170 */ 2,
/* 1171 */ 1175,
/* 1172 */ 50037,
/* 1173 */ -1171,
/* 1174 */ 1,
/* 1175 */ 0,
/* 1176 */ -1171,
/* 1177 */ 2,
/* 1178 */ 1182,
/* 1179 */ 50058,
/* 1180 */ -1178,
/* 1181 */ 1,
/* 1182 */ 0,
/* 1183 */ -1178,
/* 1184 */ 2,
/* 1185 */ 1191,
/* 1186 */ 1,
/* 1187 */ 1,
/* 1188 */ 1,
/* 1189 */ -1185,
/* 1190 */ 1,
/* 1191 */ 0,
/* 1192 */ -1185,
/* 1193 */ 2,
/* 1194 */ 1198,
/* 1195 */ 50044,
/* 1196 */ -1194,
/* 1197 */ 1,
/* 1198 */ 0,
/* 1199 */ -1194,
/* 1200 */ 2,
/* 1201 */ 1205,
/* 1202 */ 50058,
/* 1203 */ -1201,
/* 1204 */ 1,
/* 1205 */ 0,
/* 1206 */ -1201,
/* 1207 */ 2,
/* 1208 */ 1214,
/* 1209 */ 1,
/* 1210 */ 1,
/* 1211 */ 1,
/* 1212 */ -1208,
/* 1213 */ 1,
/* 1214 */ 0,
/* 1215 */ -1208,
/* 1216 */ 2,
/* 1217 */ 1221,
/* 1218 */ 50044,
/* 1219 */ -1217,
/* 1220 */ 1,
/* 1221 */ 0,
/* 1222 */ -1217,
/* 1223 */ 2,
/* 1224 */ 1228,
/* 1225 */ 50058,
/* 1226 */ -1224,
/* 1227 */ 1,
/* 1228 */ 0,
/* 1229 */ -1224,
/* 1230 */ 2,
/* 1231 */ 1237,
/* 1232 */ 1,
/* 1233 */ 1,
/* 1234 */ 1,
/* 1235 */ -1231,
/* 1236 */ 1,
/* 1237 */ 0,
/* 1238 */ -1231,
/* 1239 */ 2,
/* 1240 */ 1244,
/* 1241 */ 50044,
/* 1242 */ -1240,
/* 1243 */ 1,
/* 1244 */ 0,
/* 1245 */ -1240,
/* 1246 */ 2,
/* 1247 */ 1251,
/* 1248 */ 50275,
/* 1249 */ -1247,
/* 1250 */ 1,
/* 1251 */ 1256,
/* 1252 */ 50303,
/* 1253 */ 50275,
/* 1254 */ -1247,
/* 1255 */ 2,
/* 1256 */ 1261,
/* 1257 */ 50376,
/* 1258 */ 50275,
/* 1259 */ -1247,
/* 1260 */ 3,
/* 1261 */ 0,
/* 1262 */ 50320,
/* 1263 */ 50275,
/* 1264 */ -1247,
/* 1265 */ 4,
/* 1266 */ 1276,
/* 1267 */ 1,
/* 1268 */ 50353,
/* 1269 */ 1,
/* 1270 */ 1,
/* 1271 */ 1,
/* 1272 */ 1,
/* 1273 */ 1,
/* 1274 */ -1266,
/* 1275 */ 1,
/* 1276 */ 0,
/* 1277 */ 1,
/* 1278 */ 50353,
/* 1279 */ 1,
/* 1280 */ 1,
/* 1281 */ -1266,
/* 1282 */ 2,
/* 1283 */ 1287,
/* 1284 */ 1,
/* 1285 */ -1283,
/* 1286 */ 1,
/* 1287 */ 1292,
/* 1288 */ 1,
/* 1289 */ 50037,
/* 1290 */ -1283,
/* 1291 */ 2,
/* 1292 */ 0,
/* 1293 */ -1283,
/* 1294 */ 3,
/* 1295 */ 1299,
/* 1296 */ 1,
/* 1297 */ -1295,
/* 1298 */ 1,
/* 1299 */ 0,
/* 1300 */ -1295,
/* 1301 */ 2,
/* 1302 */ 1306,
/* 1303 */ 50319,
/* 1304 */ -1302,
/* 1305 */ 1,
/* 1306 */ 0,
/* 1307 */ -1302,
/* 1308 */ 2,
/* 1309 */ 1316,
/* 1310 */ 50040,
/* 1311 */ 1,
/* 1312 */ 1,
/* 1313 */ 50041,
/* 1314 */ -1309,
/* 1315 */ 1,
/* 1316 */ 0,
/* 1317 */ -1309,
/* 1318 */ 2,
/* 1319 */ 1323,
/* 1320 */ 50322,
/* 1321 */ -1319,
/* 1322 */ 1,
/* 1323 */ 1327,
/* 1324 */ 50307,
/* 1325 */ -1319,
/* 1326 */ 2,
/* 1327 */ 0,
/* 1328 */ 50374,
/* 1329 */ -1319,
/* 1330 */ 3,
/* 1331 */ 1340,
/* 1332 */ 50288,
/* 1333 */ 1,
/* 1334 */ 1,
/* 1335 */ 1,
/* 1336 */ 1,
/* 1337 */ 1,
/* 1338 */ -1331,
/* 1339 */ 1,
/* 1340 */ 0,
/* 1341 */ -1331,
/* 1342 */ 2,
/* 1343 */ 1347,
/* 1344 */ 1,
/* 1345 */ -1343,
/* 1346 */ 1,
/* 1347 */ 0,
/* 1348 */ -1343,
/* 1349 */ 2,
/* 1350 */ 1354,
/* 1351 */ 1,
/* 1352 */ -1350,
/* 1353 */ 1,
/* 1354 */ 0,
/* 1355 */ -1350,
/* 1356 */ 2,
/* 1357 */ 1361,
/* 1358 */ 50319,
/* 1359 */ -1357,
/* 1360 */ 1,
/* 1361 */ 0,
/* 1362 */ -1357,
/* 1363 */ 2,
/* 1364 */ 1371,
/* 1365 */ 50040,
/* 1366 */ 1,
/* 1367 */ 1,
/* 1368 */ 50041,
/* 1369 */ -1364,
/* 1370 */ 1,
/* 1371 */ 0,
/* 1372 */ -1364,
/* 1373 */ 2,
/* 1374 */ 1378,
/* 1375 */ 50322,
/* 1376 */ -1374,
/* 1377 */ 1,
/* 1378 */ 1382,
/* 1379 */ 50307,
/* 1380 */ -1374,
/* 1381 */ 2,
/* 1382 */ 0,
/* 1383 */ 50374,
/* 1384 */ -1374,
/* 1385 */ 3,
/* 1386 */ 1390,
/* 1387 */ 1,
/* 1388 */ -1386,
/* 1389 */ 1,
/* 1390 */ 0,
/* 1391 */ -1386,
/* 1392 */ 2,
/* 1393 */ 1398,
/* 1394 */ 1,
/* 1395 */ 1,
/* 1396 */ -1393,
/* 1397 */ 1,
/* 1398 */ 0,
/* 1399 */ 50126,
/* 1400 */ 1,
/* 1401 */ 1,
/* 1402 */ -1393,
/* 1403 */ 2,
/* 1404 */ 1408,
/* 1405 */ 50044,
/* 1406 */ -1404,
/* 1407 */ 1,
/* 1408 */ 0,
/* 1409 */ -1404,
/* 1410 */ 2,
/* 1411 */ 1415,
/* 1412 */ 50044,
/* 1413 */ -1411,
/* 1414 */ 1,
/* 1415 */ 0,
/* 1416 */ -1411,
/* 1417 */ 2,
/* 1418 */ 0,
/* 1419 */ 50277,
/* 1420 */ 1,
/* 1421 */ 1,
/* 1422 */ -1418,
/* 1423 */ 1,
/* 1424 */ 1428,
/* 1425 */ 50058,
/* 1426 */ -1424,
/* 1427 */ 1,
/* 1428 */ 0,
/* 1429 */ -1424,
/* 1430 */ 2,
/* 1431 */ 0,
/* 1432 */ 50372,
/* 1433 */ 50257,
/* 1434 */ 50058,
/* 1435 */ 1,
/* 1436 */ 50123,
/* 1437 */ 1,
/* 1438 */ 50125,
/* 1439 */ -1431,
/* 1440 */ 1,
/* 1441 */ 1446,
/* 1442 */ 1,
/* 1443 */ 1,
/* 1444 */ -1441,
/* 1445 */ 1,
/* 1446 */ 0,
/* 1447 */ -1441,
/* 1448 */ 2,
/* 1449 */ 1456,
/* 1450 */ 50326,
/* 1451 */ 1,
/* 1452 */ 50257,
/* 1453 */ 50059,
/* 1454 */ -1449,
/* 1455 */ 1,
/* 1456 */ 1463,
/* 1457 */ 50374,
/* 1458 */ 1,
/* 1459 */ 1,
/* 1460 */ 50059,
/* 1461 */ -1449,
/* 1462 */ 2,
/* 1463 */ 1467,
/* 1464 */ 1,
/* 1465 */ -1449,
/* 1466 */ 3,
/* 1467 */ 1474,
/* 1468 */ 50430,
/* 1469 */ 1,
/* 1470 */ 1,
/* 1471 */ 50059,
/* 1472 */ -1449,
/* 1473 */ 4,
/* 1474 */ 1481,
/* 1475 */ 50373,
/* 1476 */ 1,
/* 1477 */ 1,
/* 1478 */ 50059,
/* 1479 */ -1449,
/* 1480 */ 5,
/* 1481 */ 1488,
/* 1482 */ 50316,
/* 1483 */ 1,
/* 1484 */ 1,
/* 1485 */ 50059,
/* 1486 */ -1449,
/* 1487 */ 6,
/* 1488 */ 1495,
/* 1489 */ 50452,
/* 1490 */ 1,
/* 1491 */ 1,
/* 1492 */ 50059,
/* 1493 */ -1449,
/* 1494 */ 7,
/* 1495 */ 1502,
/* 1496 */ 50284,
/* 1497 */ 1,
/* 1498 */ 1,
/* 1499 */ 50059,
/* 1500 */ -1449,
/* 1501 */ 8,
/* 1502 */ 1509,
/* 1503 */ 50435,
/* 1504 */ 1,
/* 1505 */ 1,
/* 1506 */ 50059,
/* 1507 */ -1449,
/* 1508 */ 9,
/* 1509 */ 1517,
/* 1510 */ 50435,
/* 1511 */ 1,
/* 1512 */ 1,
/* 1513 */ 50037,
/* 1514 */ 50059,
/* 1515 */ -1449,
/* 1516 */ 10,
/* 1517 */ 1526,
/* 1518 */ 50435,
/* 1519 */ 1,
/* 1520 */ 50043,
/* 1521 */ 1,
/* 1522 */ 50037,
/* 1523 */ 50059,
/* 1524 */ -1449,
/* 1525 */ 11,
/* 1526 */ 1533,
/* 1527 */ 50404,
/* 1528 */ 1,
/* 1529 */ 1,
/* 1530 */ 50059,
/* 1531 */ -1449,
/* 1532 */ 12,
/* 1533 */ 1540,
/* 1534 */ 50420,
/* 1535 */ 1,
/* 1536 */ 1,
/* 1537 */ 50059,
/* 1538 */ -1449,
/* 1539 */ 13,
/* 1540 */ 1547,
/* 1541 */ 50383,
/* 1542 */ 1,
/* 1543 */ 1,
/* 1544 */ 50059,
/* 1545 */ -1449,
/* 1546 */ 14,
/* 1547 */ 1555,
/* 1548 */ 50315,
/* 1549 */ 1,
/* 1550 */ 1,
/* 1551 */ 1,
/* 1552 */ 50059,
/* 1553 */ -1449,
/* 1554 */ 15,
/* 1555 */ 1562,
/* 1556 */ 50466,
/* 1557 */ 1,
/* 1558 */ 1,
/* 1559 */ 50059,
/* 1560 */ -1449,
/* 1561 */ 16,
/* 1562 */ 1569,
/* 1563 */ 50431,
/* 1564 */ 1,
/* 1565 */ 1,
/* 1566 */ 50059,
/* 1567 */ -1449,
/* 1568 */ 17,
/* 1569 */ 1576,
/* 1570 */ 50396,
/* 1571 */ 1,
/* 1572 */ 1,
/* 1573 */ 50059,
/* 1574 */ -1449,
/* 1575 */ 18,
/* 1576 */ 1583,
/* 1577 */ 50323,
/* 1578 */ 1,
/* 1579 */ 1,
/* 1580 */ 50059,
/* 1581 */ -1449,
/* 1582 */ 19,
/* 1583 */ 1590,
/* 1584 */ 50312,
/* 1585 */ 1,
/* 1586 */ 1,
/* 1587 */ 50059,
/* 1588 */ -1449,
/* 1589 */ 20,
/* 1590 */ 1597,
/* 1591 */ 50407,
/* 1592 */ 1,
/* 1593 */ 1,
/* 1594 */ 50059,
/* 1595 */ -1449,
/* 1596 */ 21,
/* 1597 */ 1604,
/* 1598 */ 50292,
/* 1599 */ 1,
/* 1600 */ 1,
/* 1601 */ 50059,
/* 1602 */ -1449,
/* 1603 */ 22,
/* 1604 */ 1611,
/* 1605 */ 50365,
/* 1606 */ 1,
/* 1607 */ 1,
/* 1608 */ 50059,
/* 1609 */ -1449,
/* 1610 */ 23,
/* 1611 */ 1618,
/* 1612 */ 50437,
/* 1613 */ 1,
/* 1614 */ 1,
/* 1615 */ 50059,
/* 1616 */ -1449,
/* 1617 */ 24,
/* 1618 */ 1625,
/* 1619 */ 50349,
/* 1620 */ 1,
/* 1621 */ 1,
/* 1622 */ 50059,
/* 1623 */ -1449,
/* 1624 */ 25,
/* 1625 */ 1632,
/* 1626 */ 50350,
/* 1627 */ 1,
/* 1628 */ 1,
/* 1629 */ 50059,
/* 1630 */ -1449,
/* 1631 */ 26,
/* 1632 */ 1639,
/* 1633 */ 50361,
/* 1634 */ 1,
/* 1635 */ 1,
/* 1636 */ 50059,
/* 1637 */ -1449,
/* 1638 */ 27,
/* 1639 */ 1646,
/* 1640 */ 50304,
/* 1641 */ 1,
/* 1642 */ 1,
/* 1643 */ 50059,
/* 1644 */ -1449,
/* 1645 */ 28,
/* 1646 */ 1653,
/* 1647 */ 50468,
/* 1648 */ 1,
/* 1649 */ 1,
/* 1650 */ 50059,
/* 1651 */ -1449,
/* 1652 */ 29,
/* 1653 */ 1660,
/* 1654 */ 50467,
/* 1655 */ 1,
/* 1656 */ 1,
/* 1657 */ 50059,
/* 1658 */ -1449,
/* 1659 */ 30,
/* 1660 */ 1667,
/* 1661 */ 50379,
/* 1662 */ 1,
/* 1663 */ 1,
/* 1664 */ 50059,
/* 1665 */ -1449,
/* 1666 */ 31,
/* 1667 */ 1674,
/* 1668 */ 50456,
/* 1669 */ 1,
/* 1670 */ 1,
/* 1671 */ 50059,
/* 1672 */ -1449,
/* 1673 */ 32,
/* 1674 */ 1681,
/* 1675 */ 50313,
/* 1676 */ 1,
/* 1677 */ 1,
/* 1678 */ 50059,
/* 1679 */ -1449,
/* 1680 */ 33,
/* 1681 */ 1689,
/* 1682 */ 50376,
/* 1683 */ 50420,
/* 1684 */ 1,
/* 1685 */ 1,
/* 1686 */ 50059,
/* 1687 */ -1449,
/* 1688 */ 34,
/* 1689 */ 1696,
/* 1690 */ 50375,
/* 1691 */ 1,
/* 1692 */ 1,
/* 1693 */ 50059,
/* 1694 */ -1449,
/* 1695 */ 35,
/* 1696 */ 1703,
/* 1697 */ 50355,
/* 1698 */ 1,
/* 1699 */ 1,
/* 1700 */ 50059,
/* 1701 */ -1449,
/* 1702 */ 36,
/* 1703 */ 1707,
/* 1704 */ 1,
/* 1705 */ -1449,
/* 1706 */ 37,
/* 1707 */ 1711,
/* 1708 */ 1,
/* 1709 */ -1449,
/* 1710 */ 38,
/* 1711 */ 0,
/* 1712 */ 1,
/* 1713 */ -1449,
/* 1714 */ 39,
/* 1715 */ 1719,
/* 1716 */ 50058,
/* 1717 */ -1715,
/* 1718 */ 1,
/* 1719 */ 0,
/* 1720 */ -1715,
/* 1721 */ 2,
/* 1722 */ 1726,
/* 1723 */ 50058,
/* 1724 */ -1722,
/* 1725 */ 1,
/* 1726 */ 0,
/* 1727 */ -1722,
/* 1728 */ 2,
/* 1729 */ 1733,
/* 1730 */ 50058,
/* 1731 */ -1729,
/* 1732 */ 1,
/* 1733 */ 0,
/* 1734 */ -1729,
/* 1735 */ 2,
/* 1736 */ 1740,
/* 1737 */ 50058,
/* 1738 */ -1736,
/* 1739 */ 1,
/* 1740 */ 0,
/* 1741 */ -1736,
/* 1742 */ 2,
/* 1743 */ 1747,
/* 1744 */ 50058,
/* 1745 */ -1743,
/* 1746 */ 1,
/* 1747 */ 0,
/* 1748 */ -1743,
/* 1749 */ 2,
/* 1750 */ 1754,
/* 1751 */ 50058,
/* 1752 */ -1750,
/* 1753 */ 1,
/* 1754 */ 0,
/* 1755 */ -1750,
/* 1756 */ 2,
/* 1757 */ 1761,
/* 1758 */ 50058,
/* 1759 */ -1757,
/* 1760 */ 1,
/* 1761 */ 0,
/* 1762 */ -1757,
/* 1763 */ 2,
/* 1764 */ 1768,
/* 1765 */ 50058,
/* 1766 */ -1764,
/* 1767 */ 1,
/* 1768 */ 0,
/* 1769 */ -1764,
/* 1770 */ 2,
/* 1771 */ 1775,
/* 1772 */ 50058,
/* 1773 */ -1771,
/* 1774 */ 1,
/* 1775 */ 0,
/* 1776 */ -1771,
/* 1777 */ 2,
/* 1778 */ 1782,
/* 1779 */ 50058,
/* 1780 */ -1778,
/* 1781 */ 1,
/* 1782 */ 0,
/* 1783 */ -1778,
/* 1784 */ 2,
/* 1785 */ 1789,
/* 1786 */ 50058,
/* 1787 */ -1785,
/* 1788 */ 1,
/* 1789 */ 0,
/* 1790 */ -1785,
/* 1791 */ 2,
/* 1792 */ 1796,
/* 1793 */ 50058,
/* 1794 */ -1792,
/* 1795 */ 1,
/* 1796 */ 0,
/* 1797 */ -1792,
/* 1798 */ 2,
/* 1799 */ 1803,
/* 1800 */ 50058,
/* 1801 */ -1799,
/* 1802 */ 1,
/* 1803 */ 0,
/* 1804 */ -1799,
/* 1805 */ 2,
/* 1806 */ 1810,
/* 1807 */ 50058,
/* 1808 */ -1806,
/* 1809 */ 1,
/* 1810 */ 0,
/* 1811 */ -1806,
/* 1812 */ 2,
/* 1813 */ 1817,
/* 1814 */ 1,
/* 1815 */ -1813,
/* 1816 */ 1,
/* 1817 */ 0,
/* 1818 */ -1813,
/* 1819 */ 2,
/* 1820 */ 1824,
/* 1821 */ 50058,
/* 1822 */ -1820,
/* 1823 */ 1,
/* 1824 */ 0,
/* 1825 */ -1820,
/* 1826 */ 2,
/* 1827 */ 1831,
/* 1828 */ 50058,
/* 1829 */ -1827,
/* 1830 */ 1,
/* 1831 */ 0,
/* 1832 */ -1827,
/* 1833 */ 2,
/* 1834 */ 1838,
/* 1835 */ 50058,
/* 1836 */ -1834,
/* 1837 */ 1,
/* 1838 */ 0,
/* 1839 */ -1834,
/* 1840 */ 2,
/* 1841 */ 1845,
/* 1842 */ 50058,
/* 1843 */ -1841,
/* 1844 */ 1,
/* 1845 */ 0,
/* 1846 */ -1841,
/* 1847 */ 2,
/* 1848 */ 1852,
/* 1849 */ 50058,
/* 1850 */ -1848,
/* 1851 */ 1,
/* 1852 */ 0,
/* 1853 */ -1848,
/* 1854 */ 2,
/* 1855 */ 1859,
/* 1856 */ 50058,
/* 1857 */ -1855,
/* 1858 */ 1,
/* 1859 */ 0,
/* 1860 */ -1855,
/* 1861 */ 2,
/* 1862 */ 1866,
/* 1863 */ 50058,
/* 1864 */ -1862,
/* 1865 */ 1,
/* 1866 */ 0,
/* 1867 */ -1862,
/* 1868 */ 2,
/* 1869 */ 1874,
/* 1870 */ 1,
/* 1871 */ 1,
/* 1872 */ -1869,
/* 1873 */ 1,
/* 1874 */ 0,
/* 1875 */ -1869,
/* 1876 */ 2,
/* 1877 */ 1881,
/* 1878 */ 50058,
/* 1879 */ -1877,
/* 1880 */ 1,
/* 1881 */ 0,
/* 1882 */ -1877,
/* 1883 */ 2,
/* 1884 */ 1888,
/* 1885 */ 50058,
/* 1886 */ -1884,
/* 1887 */ 1,
/* 1888 */ 0,
/* 1889 */ -1884,
/* 1890 */ 2,
/* 1891 */ 1897,
/* 1892 */ 1,
/* 1893 */ 1,
/* 1894 */ 1,
/* 1895 */ -1891,
/* 1896 */ 1,
/* 1897 */ 0,
/* 1898 */ -1891,
/* 1899 */ 2,
/* 1900 */ 1904,
/* 1901 */ 50044,
/* 1902 */ -1900,
/* 1903 */ 1,
/* 1904 */ 0,
/* 1905 */ -1900,
/* 1906 */ 2,
/* 1907 */ 1911,
/* 1908 */ 50058,
/* 1909 */ -1907,
/* 1910 */ 1,
/* 1911 */ 0,
/* 1912 */ -1907,
/* 1913 */ 2,
/* 1914 */ 1920,
/* 1915 */ 1,
/* 1916 */ 1,
/* 1917 */ 1,
/* 1918 */ -1914,
/* 1919 */ 1,
/* 1920 */ 0,
/* 1921 */ -1914,
/* 1922 */ 2,
/* 1923 */ 1927,
/* 1924 */ 50044,
/* 1925 */ -1923,
/* 1926 */ 1,
/* 1927 */ 0,
/* 1928 */ -1923,
/* 1929 */ 2,
/* 1930 */ 1934,
/* 1931 */ 50058,
/* 1932 */ -1930,
/* 1933 */ 1,
/* 1934 */ 0,
/* 1935 */ -1930,
/* 1936 */ 2,
/* 1937 */ 1942,
/* 1938 */ 1,
/* 1939 */ 1,
/* 1940 */ -1937,
/* 1941 */ 1,
/* 1942 */ 0,
/* 1943 */ -1937,
/* 1944 */ 2,
/* 1945 */ 1949,
/* 1946 */ 50058,
/* 1947 */ -1945,
/* 1948 */ 1,
/* 1949 */ 0,
/* 1950 */ -1945,
/* 1951 */ 2,
/* 1952 */ 1958,
/* 1953 */ 1,
/* 1954 */ 1,
/* 1955 */ 1,
/* 1956 */ -1952,
/* 1957 */ 1,
/* 1958 */ 0,
/* 1959 */ -1952,
/* 1960 */ 2,
/* 1961 */ 1965,
/* 1962 */ 50044,
/* 1963 */ -1961,
/* 1964 */ 1,
/* 1965 */ 1969,
/* 1966 */ 50124,
/* 1967 */ -1961,
/* 1968 */ 2,
/* 1969 */ 0,
/* 1970 */ -1961,
/* 1971 */ 3,
/* 1972 */ 1976,
/* 1973 */ 50058,
/* 1974 */ -1972,
/* 1975 */ 1,
/* 1976 */ 0,
/* 1977 */ -1972,
/* 1978 */ 2,
/* 1979 */ 1983,
/* 1980 */ 50058,
/* 1981 */ -1979,
/* 1982 */ 1,
/* 1983 */ 0,
/* 1984 */ -1979,
/* 1985 */ 2,
/* 1986 */ 1990,
/* 1987 */ 50058,
/* 1988 */ -1986,
/* 1989 */ 1,
/* 1990 */ 0,
/* 1991 */ -1986,
/* 1992 */ 2,
/* 1993 */ 1997,
/* 1994 */ 50058,
/* 1995 */ -1993,
/* 1996 */ 1,
/* 1997 */ 0,
/* 1998 */ -1993,
/* 1999 */ 2,
/* 2000 */ 2004,
/* 2001 */ 50058,
/* 2002 */ -2000,
/* 2003 */ 1,
/* 2004 */ 0,
/* 2005 */ -2000,
/* 2006 */ 2,
/* 2007 */ 2011,
/* 2008 */ 50058,
/* 2009 */ -2007,
/* 2010 */ 1,
/* 2011 */ 0,
/* 2012 */ -2007,
/* 2013 */ 2,
/* 2014 */ 2018,
/* 2015 */ 50058,
/* 2016 */ -2014,
/* 2017 */ 1,
/* 2018 */ 0,
/* 2019 */ -2014,
/* 2020 */ 2,
/* 2021 */ 2025,
/* 2022 */ 50058,
/* 2023 */ -2021,
/* 2024 */ 1,
/* 2025 */ 0,
/* 2026 */ -2021,
/* 2027 */ 2,
/* 2028 */ 2032,
/* 2029 */ 50058,
/* 2030 */ -2028,
/* 2031 */ 1,
/* 2032 */ 0,
/* 2033 */ -2028,
/* 2034 */ 2,
/* 2035 */ 2040,
/* 2036 */ 1,
/* 2037 */ 1,
/* 2038 */ -2035,
/* 2039 */ 1,
/* 2040 */ 0,
/* 2041 */ 50276,
/* 2042 */ 1,
/* 2043 */ 1,
/* 2044 */ -2035,
/* 2045 */ 2,
/* 2046 */ 2050,
/* 2047 */ 50044,
/* 2048 */ -2046,
/* 2049 */ 1,
/* 2050 */ 0,
/* 2051 */ -2046,
/* 2052 */ 2,
/* 2053 */ 2057,
/* 2054 */ 50044,
/* 2055 */ -2053,
/* 2056 */ 1,
/* 2057 */ 0,
/* 2058 */ -2053,
/* 2059 */ 2,
/* 2060 */ 2065,
/* 2061 */ 1,
/* 2062 */ 1,
/* 2063 */ -2060,
/* 2064 */ 1,
/* 2065 */ 0,
/* 2066 */ 50126,
/* 2067 */ 1,
/* 2068 */ 1,
/* 2069 */ -2060,
/* 2070 */ 2,
/* 2071 */ 2075,
/* 2072 */ 50044,
/* 2073 */ -2071,
/* 2074 */ 1,
/* 2075 */ 0,
/* 2076 */ -2071,
/* 2077 */ 2,
/* 2078 */ 2082,
/* 2079 */ 50044,
/* 2080 */ -2078,
/* 2081 */ 1,
/* 2082 */ 0,
/* 2083 */ -2078,
/* 2084 */ 2,
/* 2085 */ 0,
/* 2086 */ 50348,
/* 2087 */ 50257,
/* 2088 */ 50058,
/* 2089 */ 1,
/* 2090 */ 50123,
/* 2091 */ 1,
/* 2092 */ 50125,
/* 2093 */ -2085,
/* 2094 */ 1,
/* 2095 */ 2100,
/* 2096 */ 1,
/* 2097 */ 1,
/* 2098 */ -2095,
/* 2099 */ 1,
/* 2100 */ 0,
/* 2101 */ -2095,
/* 2102 */ 2,
/* 2103 */ 2110,
/* 2104 */ 50326,
/* 2105 */ 1,
/* 2106 */ 50257,
/* 2107 */ 50059,
/* 2108 */ -2103,
/* 2109 */ 1,
/* 2110 */ 2117,
/* 2111 */ 50383,
/* 2112 */ 1,
/* 2113 */ 1,
/* 2114 */ 50059,
/* 2115 */ -2103,
/* 2116 */ 2,
/* 2117 */ 2121,
/* 2118 */ 1,
/* 2119 */ -2103,
/* 2120 */ 3,
/* 2121 */ 2129,
/* 2122 */ 50389,
/* 2123 */ 1,
/* 2124 */ 1,
/* 2125 */ 50037,
/* 2126 */ 50059,
/* 2127 */ -2103,
/* 2128 */ 4,
/* 2129 */ 2136,
/* 2130 */ 50365,
/* 2131 */ 1,
/* 2132 */ 1,
/* 2133 */ 50059,
/* 2134 */ -2103,
/* 2135 */ 5,
/* 2136 */ 2143,
/* 2137 */ 50350,
/* 2138 */ 1,
/* 2139 */ 1,
/* 2140 */ 50059,
/* 2141 */ -2103,
/* 2142 */ 6,
/* 2143 */ 2150,
/* 2144 */ 1,
/* 2145 */ 1,
/* 2146 */ 1,
/* 2147 */ 50059,
/* 2148 */ -2103,
/* 2149 */ 7,
/* 2150 */ 2157,
/* 2151 */ 50472,
/* 2152 */ 1,
/* 2153 */ 1,
/* 2154 */ 50059,
/* 2155 */ -2103,
/* 2156 */ 8,
/* 2157 */ 2164,
/* 2158 */ 50344,
/* 2159 */ 1,
/* 2160 */ 1,
/* 2161 */ 50059,
/* 2162 */ -2103,
/* 2163 */ 9,
/* 2164 */ 2168,
/* 2165 */ 1,
/* 2166 */ -2103,
/* 2167 */ 10,
/* 2168 */ 2172,
/* 2169 */ 1,
/* 2170 */ -2103,
/* 2171 */ 11,
/* 2172 */ 0,
/* 2173 */ 1,
/* 2174 */ -2103,
/* 2175 */ 12,
/* 2176 */ 2180,
/* 2177 */ 50058,
/* 2178 */ -2176,
/* 2179 */ 1,
/* 2180 */ 0,
/* 2181 */ -2176,
/* 2182 */ 2,
/* 2183 */ 2187,
/* 2184 */ 50058,
/* 2185 */ -2183,
/* 2186 */ 1,
/* 2187 */ 0,
/* 2188 */ -2183,
/* 2189 */ 2,
/* 2190 */ 2194,
/* 2191 */ 50058,
/* 2192 */ -2190,
/* 2193 */ 1,
/* 2194 */ 0,
/* 2195 */ -2190,
/* 2196 */ 2,
/* 2197 */ 2201,
/* 2198 */ 50058,
/* 2199 */ -2197,
/* 2200 */ 1,
/* 2201 */ 0,
/* 2202 */ -2197,
/* 2203 */ 2,
/* 2204 */ 2208,
/* 2205 */ 50058,
/* 2206 */ -2204,
/* 2207 */ 1,
/* 2208 */ 0,
/* 2209 */ -2204,
/* 2210 */ 2,
/* 2211 */ 2217,
/* 2212 */ 1,
/* 2213 */ 1,
/* 2214 */ 1,
/* 2215 */ -2211,
/* 2216 */ 1,
/* 2217 */ 0,
/* 2218 */ -2211,
/* 2219 */ 2,
/* 2220 */ 2224,
/* 2221 */ 50124,
/* 2222 */ -2220,
/* 2223 */ 1,
/* 2224 */ 2228,
/* 2225 */ 50044,
/* 2226 */ -2220,
/* 2227 */ 2,
/* 2228 */ 0,
/* 2229 */ -2220,
/* 2230 */ 3,
/* 2231 */ 2235,
/* 2232 */ 50448,
/* 2233 */ -2231,
/* 2234 */ 1,
/* 2235 */ 0,
/* 2236 */ 50334,
/* 2237 */ -2231,
/* 2238 */ 2,
/* 2239 */ 2243,
/* 2240 */ 50058,
/* 2241 */ -2239,
/* 2242 */ 1,
/* 2243 */ 0,
/* 2244 */ -2239,
/* 2245 */ 2,
/* 2246 */ 2250,
/* 2247 */ 50058,
/* 2248 */ -2246,
/* 2249 */ 1,
/* 2250 */ 0,
/* 2251 */ -2246,
/* 2252 */ 2,
/* 2253 */ 2257,
/* 2254 */ 50058,
/* 2255 */ -2253,
/* 2256 */ 1,
/* 2257 */ 0,
/* 2258 */ -2253,
/* 2259 */ 2,
/* 2260 */ 0,
/* 2261 */ 1,
/* 2262 */ 1,
/* 2263 */ 50257,
/* 2264 */ 50058,
/* 2265 */ 1,
/* 2266 */ 50123,
/* 2267 */ 1,
/* 2268 */ 50125,
/* 2269 */ 1,
/* 2270 */ -2260,
/* 2271 */ 1,
/* 2272 */ 2278,
/* 2273 */ 1,
/* 2274 */ 1,
/* 2275 */ 1,
/* 2276 */ -2272,
/* 2277 */ 1,
/* 2278 */ 0,
/* 2279 */ -2272,
/* 2280 */ 2,
/* 2281 */ 2285,
/* 2282 */ 50047,
/* 2283 */ -2281,
/* 2284 */ 1,
/* 2285 */ 0,
/* 2286 */ -2281,
/* 2287 */ 2,
/* 2288 */ 2292,
/* 2289 */ 50436,
/* 2290 */ -2288,
/* 2291 */ 1,
/* 2292 */ 2296,
/* 2293 */ 50334,
/* 2294 */ -2288,
/* 2295 */ 2,
/* 2296 */ 2300,
/* 2297 */ 50409,
/* 2298 */ -2288,
/* 2299 */ 3,
/* 2300 */ 0,
/* 2301 */ 50330,
/* 2302 */ -2288,
/* 2303 */ 4,
/* 2304 */ 2309,
/* 2305 */ 1,
/* 2306 */ 1,
/* 2307 */ -2304,
/* 2308 */ 1,
/* 2309 */ 0,
/* 2310 */ -2304,
/* 2311 */ 2,
/* 2312 */ 2321,
/* 2313 */ 50288,
/* 2314 */ 1,
/* 2315 */ 50123,
/* 2316 */ 1,
/* 2317 */ 50125,
/* 2318 */ 1,
/* 2319 */ -2312,
/* 2320 */ 1,
/* 2321 */ 0,
/* 2322 */ -2312,
/* 2323 */ 2,
/* 2324 */ 2329,
/* 2325 */ 1,
/* 2326 */ 1,
/* 2327 */ -2324,
/* 2328 */ 1,
/* 2329 */ 0,
/* 2330 */ -2324,
/* 2331 */ 2,
/* 2332 */ 2339,
/* 2333 */ 50326,
/* 2334 */ 1,
/* 2335 */ 50257,
/* 2336 */ 50059,
/* 2337 */ -2332,
/* 2338 */ 1,
/* 2339 */ 2344,
/* 2340 */ 1,
/* 2341 */ 50059,
/* 2342 */ -2332,
/* 2343 */ 2,
/* 2344 */ 2351,
/* 2345 */ 50374,
/* 2346 */ 1,
/* 2347 */ 1,
/* 2348 */ 50059,
/* 2349 */ -2332,
/* 2350 */ 3,
/* 2351 */ 2358,
/* 2352 */ 50315,
/* 2353 */ 1,
/* 2354 */ 1,
/* 2355 */ 50059,
/* 2356 */ -2332,
/* 2357 */ 4,
/* 2358 */ 2367,
/* 2359 */ 50298,
/* 2360 */ 1,
/* 2361 */ 1,
/* 2362 */ 1,
/* 2363 */ 1,
/* 2364 */ 50059,
/* 2365 */ -2332,
/* 2366 */ 5,
/* 2367 */ 2375,
/* 2368 */ 50413,
/* 2369 */ 1,
/* 2370 */ 1,
/* 2371 */ 1,
/* 2372 */ 50059,
/* 2373 */ -2332,
/* 2374 */ 6,
/* 2375 */ 2383,
/* 2376 */ 50445,
/* 2377 */ 1,
/* 2378 */ 1,
/* 2379 */ 1,
/* 2380 */ 50059,
/* 2381 */ -2332,
/* 2382 */ 7,
/* 2383 */ 2390,
/* 2384 */ 50417,
/* 2385 */ 1,
/* 2386 */ 1,
/* 2387 */ 50059,
/* 2388 */ -2332,
/* 2389 */ 8,
/* 2390 */ 2397,
/* 2391 */ 50472,
/* 2392 */ 1,
/* 2393 */ 1,
/* 2394 */ 50059,
/* 2395 */ -2332,
/* 2396 */ 9,
/* 2397 */ 2404,
/* 2398 */ 50473,
/* 2399 */ 1,
/* 2400 */ 1,
/* 2401 */ 50059,
/* 2402 */ -2332,
/* 2403 */ 10,
/* 2404 */ 2411,
/* 2405 */ 50333,
/* 2406 */ 1,
/* 2407 */ 1,
/* 2408 */ 50059,
/* 2409 */ -2332,
/* 2410 */ 11,
/* 2411 */ 2418,
/* 2412 */ 50414,
/* 2413 */ 1,
/* 2414 */ 1,
/* 2415 */ 50059,
/* 2416 */ -2332,
/* 2417 */ 12,
/* 2418 */ 2425,
/* 2419 */ 50297,
/* 2420 */ 1,
/* 2421 */ 1,
/* 2422 */ 50059,
/* 2423 */ -2332,
/* 2424 */ 13,
/* 2425 */ 2432,
/* 2426 */ 50378,
/* 2427 */ 1,
/* 2428 */ 1,
/* 2429 */ 50059,
/* 2430 */ -2332,
/* 2431 */ 14,
/* 2432 */ 2439,
/* 2433 */ 50461,
/* 2434 */ 1,
/* 2435 */ 1,
/* 2436 */ 50059,
/* 2437 */ -2332,
/* 2438 */ 15,
/* 2439 */ 2446,
/* 2440 */ 50428,
/* 2441 */ 1,
/* 2442 */ 1,
/* 2443 */ 50059,
/* 2444 */ -2332,
/* 2445 */ 16,
/* 2446 */ 2453,
/* 2447 */ 50428,
/* 2448 */ 1,
/* 2449 */ 1,
/* 2450 */ 50059,
/* 2451 */ -2332,
/* 2452 */ 17,
/* 2453 */ 2460,
/* 2454 */ 50318,
/* 2455 */ 1,
/* 2456 */ 1,
/* 2457 */ 50059,
/* 2458 */ -2332,
/* 2459 */ 18,
/* 2460 */ 2467,
/* 2461 */ 50318,
/* 2462 */ 1,
/* 2463 */ 50275,
/* 2464 */ 50059,
/* 2465 */ -2332,
/* 2466 */ 19,
/* 2467 */ 2474,
/* 2468 */ 50350,
/* 2469 */ 1,
/* 2470 */ 1,
/* 2471 */ 50059,
/* 2472 */ -2332,
/* 2473 */ 20,
/* 2474 */ 2478,
/* 2475 */ 1,
/* 2476 */ -2332,
/* 2477 */ 21,
/* 2478 */ 2482,
/* 2479 */ 1,
/* 2480 */ -2332,
/* 2481 */ 22,
/* 2482 */ 0,
/* 2483 */ 1,
/* 2484 */ -2332,
/* 2485 */ 23,
/* 2486 */ 2490,
/* 2487 */ 50058,
/* 2488 */ -2486,
/* 2489 */ 1,
/* 2490 */ 0,
/* 2491 */ -2486,
/* 2492 */ 2,
/* 2493 */ 2497,
/* 2494 */ 50058,
/* 2495 */ -2493,
/* 2496 */ 1,
/* 2497 */ 0,
/* 2498 */ -2493,
/* 2499 */ 2,
/* 2500 */ 2504,
/* 2501 */ 50058,
/* 2502 */ -2500,
/* 2503 */ 1,
/* 2504 */ 0,
/* 2505 */ -2500,
/* 2506 */ 2,
/* 2507 */ 2511,
/* 2508 */ 50308,
/* 2509 */ -2507,
/* 2510 */ 1,
/* 2511 */ 0,
/* 2512 */ -2507,
/* 2513 */ 2,
/* 2514 */ 2518,
/* 2515 */ 50058,
/* 2516 */ -2514,
/* 2517 */ 1,
/* 2518 */ 0,
/* 2519 */ -2514,
/* 2520 */ 2,
/* 2521 */ 2525,
/* 2522 */ 50037,
/* 2523 */ -2521,
/* 2524 */ 1,
/* 2525 */ 0,
/* 2526 */ -2521,
/* 2527 */ 2,
/* 2528 */ 2532,
/* 2529 */ 50058,
/* 2530 */ -2528,
/* 2531 */ 1,
/* 2532 */ 0,
/* 2533 */ -2528,
/* 2534 */ 2,
/* 2535 */ 2541,
/* 2536 */ 50040,
/* 2537 */ 1,
/* 2538 */ 50041,
/* 2539 */ -2535,
/* 2540 */ 1,
/* 2541 */ 0,
/* 2542 */ -2535,
/* 2543 */ 2,
/* 2544 */ 2548,
/* 2545 */ 50058,
/* 2546 */ -2544,
/* 2547 */ 1,
/* 2548 */ 0,
/* 2549 */ -2544,
/* 2550 */ 2,
/* 2551 */ 2555,
/* 2552 */ 50405,
/* 2553 */ -2551,
/* 2554 */ 1,
/* 2555 */ 0,
/* 2556 */ -2551,
/* 2557 */ 2,
/* 2558 */ 2562,
/* 2559 */ 50058,
/* 2560 */ -2558,
/* 2561 */ 1,
/* 2562 */ 0,
/* 2563 */ -2558,
/* 2564 */ 2,
/* 2565 */ 2569,
/* 2566 */ 50058,
/* 2567 */ -2565,
/* 2568 */ 1,
/* 2569 */ 0,
/* 2570 */ -2565,
/* 2571 */ 2,
/* 2572 */ 2576,
/* 2573 */ 50058,
/* 2574 */ -2572,
/* 2575 */ 1,
/* 2576 */ 0,
/* 2577 */ -2572,
/* 2578 */ 2,
/* 2579 */ 2583,
/* 2580 */ 50058,
/* 2581 */ -2579,
/* 2582 */ 1,
/* 2583 */ 0,
/* 2584 */ -2579,
/* 2585 */ 2,
/* 2586 */ 2590,
/* 2587 */ 50058,
/* 2588 */ -2586,
/* 2589 */ 1,
/* 2590 */ 0,
/* 2591 */ -2586,
/* 2592 */ 2,
/* 2593 */ 2597,
/* 2594 */ 50058,
/* 2595 */ -2593,
/* 2596 */ 1,
/* 2597 */ 0,
/* 2598 */ -2593,
/* 2599 */ 2,
/* 2600 */ 2604,
/* 2601 */ 50058,
/* 2602 */ -2600,
/* 2603 */ 1,
/* 2604 */ 0,
/* 2605 */ -2600,
/* 2606 */ 2,
/* 2607 */ 2611,
/* 2608 */ 50058,
/* 2609 */ -2607,
/* 2610 */ 1,
/* 2611 */ 0,
/* 2612 */ -2607,
/* 2613 */ 2,
/* 2614 */ 2618,
/* 2615 */ 50058,
/* 2616 */ -2614,
/* 2617 */ 1,
/* 2618 */ 0,
/* 2619 */ -2614,
/* 2620 */ 2,
/* 2621 */ 2625,
/* 2622 */ 50058,
/* 2623 */ -2621,
/* 2624 */ 1,
/* 2625 */ 0,
/* 2626 */ -2621,
/* 2627 */ 2,
/* 2628 */ 2632,
/* 2629 */ 50058,
/* 2630 */ -2628,
/* 2631 */ 1,
/* 2632 */ 0,
/* 2633 */ -2628,
/* 2634 */ 2,
/* 2635 */ 2639,
/* 2636 */ 50058,
/* 2637 */ -2635,
/* 2638 */ 1,
/* 2639 */ 0,
/* 2640 */ -2635,
/* 2641 */ 2,
/* 2642 */ 2646,
/* 2643 */ 50058,
/* 2644 */ -2642,
/* 2645 */ 1,
/* 2646 */ 0,
/* 2647 */ -2642,
/* 2648 */ 2,
/* 2649 */ 2655,
/* 2650 */ 1,
/* 2651 */ 1,
/* 2652 */ 1,
/* 2653 */ -2649,
/* 2654 */ 1,
/* 2655 */ 0,
/* 2656 */ -2649,
/* 2657 */ 2,
/* 2658 */ 2662,
/* 2659 */ 50044,
/* 2660 */ -2658,
/* 2661 */ 1,
/* 2662 */ 2666,
/* 2663 */ 50124,
/* 2664 */ -2658,
/* 2665 */ 2,
/* 2666 */ 0,
/* 2667 */ -2658,
/* 2668 */ 3,
/* 2669 */ 0,
/* 2670 */ 1,
/* 2671 */ -2669,
/* 2672 */ 1,
/* 2673 */ 2679,
/* 2674 */ 1,
/* 2675 */ 1,
/* 2676 */ 1,
/* 2677 */ -2673,
/* 2678 */ 1,
/* 2679 */ 0,
/* 2680 */ -2673,
/* 2681 */ 2,
/* 2682 */ 2686,
/* 2683 */ 50044,
/* 2684 */ -2682,
/* 2685 */ 1,
/* 2686 */ 2690,
/* 2687 */ 50124,
/* 2688 */ -2682,
/* 2689 */ 2,
/* 2690 */ 0,
/* 2691 */ -2682,
/* 2692 */ 3,
/* 2693 */ 0,
/* 2694 */ 50261,
/* 2695 */ -2693,
/* 2696 */ 1,
/* 2697 */ 0,
/* 2698 */ 50418,
/* 2699 */ 50257,
/* 2700 */ 1,
/* 2701 */ 50123,
/* 2702 */ 1,
/* 2703 */ 50125,
/* 2704 */ -2697,
/* 2705 */ 1,
/* 2706 */ 2711,
/* 2707 */ 50058,
/* 2708 */ 1,
/* 2709 */ -2706,
/* 2710 */ 1,
/* 2711 */ 0,
/* 2712 */ -2706,
/* 2713 */ 2,
/* 2714 */ 2719,
/* 2715 */ 1,
/* 2716 */ 1,
/* 2717 */ -2714,
/* 2718 */ 1,
/* 2719 */ 0,
/* 2720 */ -2714,
/* 2721 */ 2,
/* 2722 */ 2729,
/* 2723 */ 50326,
/* 2724 */ 1,
/* 2725 */ 50257,
/* 2726 */ 50059,
/* 2727 */ -2722,
/* 2728 */ 1,
/* 2729 */ 2734,
/* 2730 */ 1,
/* 2731 */ 50059,
/* 2732 */ -2722,
/* 2733 */ 2,
/* 2734 */ 2741,
/* 2735 */ 50359,
/* 2736 */ 1,
/* 2737 */ 1,
/* 2738 */ 50059,
/* 2739 */ -2722,
/* 2740 */ 3,
/* 2741 */ 2748,
/* 2742 */ 50388,
/* 2743 */ 1,
/* 2744 */ 1,
/* 2745 */ 50059,
/* 2746 */ -2722,
/* 2747 */ 4,
/* 2748 */ 2758,
/* 2749 */ 50340,
/* 2750 */ 1,
/* 2751 */ 1,
/* 2752 */ 1,
/* 2753 */ 50044,
/* 2754 */ 1,
/* 2755 */ 50059,
/* 2756 */ -2722,
/* 2757 */ 5,
/* 2758 */ 2770,
/* 2759 */ 50340,
/* 2760 */ 1,
/* 2761 */ 1,
/* 2762 */ 1,
/* 2763 */ 50044,
/* 2764 */ 1,
/* 2765 */ 50044,
/* 2766 */ 1,
/* 2767 */ 50059,
/* 2768 */ -2722,
/* 2769 */ 6,
/* 2770 */ 2777,
/* 2771 */ 50432,
/* 2772 */ 1,
/* 2773 */ 1,
/* 2774 */ 50059,
/* 2775 */ -2722,
/* 2776 */ 7,
/* 2777 */ 2784,
/* 2778 */ 1,
/* 2779 */ 1,
/* 2780 */ 50257,
/* 2781 */ 50059,
/* 2782 */ -2722,
/* 2783 */ 8,
/* 2784 */ 2791,
/* 2785 */ 50350,
/* 2786 */ 1,
/* 2787 */ 1,
/* 2788 */ 50059,
/* 2789 */ -2722,
/* 2790 */ 9,
/* 2791 */ 2795,
/* 2792 */ 1,
/* 2793 */ -2722,
/* 2794 */ 10,
/* 2795 */ 2799,
/* 2796 */ 1,
/* 2797 */ -2722,
/* 2798 */ 11,
/* 2799 */ 2803,
/* 2800 */ 1,
/* 2801 */ -2722,
/* 2802 */ 12,
/* 2803 */ 0,
/* 2804 */ 1,
/* 2805 */ -2722,
/* 2806 */ 13,
/* 2807 */ 2811,
/* 2808 */ 50058,
/* 2809 */ -2807,
/* 2810 */ 1,
/* 2811 */ 0,
/* 2812 */ -2807,
/* 2813 */ 2,
/* 2814 */ 2822,
/* 2815 */ 50277,
/* 2816 */ 1,
/* 2817 */ 1,
/* 2818 */ 1,
/* 2819 */ 1,
/* 2820 */ -2814,
/* 2821 */ 1,
/* 2822 */ 0,
/* 2823 */ -2814,
/* 2824 */ 2,
/* 2825 */ 2829,
/* 2826 */ 50058,
/* 2827 */ -2825,
/* 2828 */ 1,
/* 2829 */ 0,
/* 2830 */ -2825,
/* 2831 */ 2,
/* 2832 */ 2836,
/* 2833 */ 50044,
/* 2834 */ -2832,
/* 2835 */ 1,
/* 2836 */ 0,
/* 2837 */ -2832,
/* 2838 */ 2,
/* 2839 */ 2843,
/* 2840 */ 50058,
/* 2841 */ -2839,
/* 2842 */ 1,
/* 2843 */ 0,
/* 2844 */ -2839,
/* 2845 */ 2,
/* 2846 */ 2852,
/* 2847 */ 1,
/* 2848 */ 1,
/* 2849 */ 1,
/* 2850 */ -2846,
/* 2851 */ 1,
/* 2852 */ 0,
/* 2853 */ -2846,
/* 2854 */ 2,
/* 2855 */ 2859,
/* 2856 */ 50044,
/* 2857 */ -2855,
/* 2858 */ 1,
/* 2859 */ 0,
/* 2860 */ -2855,
/* 2861 */ 2,
/* 2862 */ 2866,
/* 2863 */ 50058,
/* 2864 */ -2862,
/* 2865 */ 1,
/* 2866 */ 0,
/* 2867 */ -2862,
/* 2868 */ 2,
/* 2869 */ 2873,
/* 2870 */ 50309,
/* 2871 */ -2869,
/* 2872 */ 1,
/* 2873 */ 0,
/* 2874 */ -2869,
/* 2875 */ 2,
/* 2876 */ 2880,
/* 2877 */ 50058,
/* 2878 */ -2876,
/* 2879 */ 1,
/* 2880 */ 0,
/* 2881 */ -2876,
/* 2882 */ 2,
/* 2883 */ 2887,
/* 2884 */ 50309,
/* 2885 */ -2883,
/* 2886 */ 1,
/* 2887 */ 0,
/* 2888 */ -2883,
/* 2889 */ 2,
/* 2890 */ 2894,
/* 2891 */ 50058,
/* 2892 */ -2890,
/* 2893 */ 1,
/* 2894 */ 0,
/* 2895 */ -2890,
/* 2896 */ 2,
/* 2897 */ 2901,
/* 2898 */ 50058,
/* 2899 */ -2897,
/* 2900 */ 1,
/* 2901 */ 0,
/* 2902 */ -2897,
/* 2903 */ 2,
/* 2904 */ 2910,
/* 2905 */ 1,
/* 2906 */ 1,
/* 2907 */ 1,
/* 2908 */ -2904,
/* 2909 */ 1,
/* 2910 */ 0,
/* 2911 */ -2904,
/* 2912 */ 2,
/* 2913 */ 2917,
/* 2914 */ 50044,
/* 2915 */ -2913,
/* 2916 */ 1,
/* 2917 */ 0,
/* 2918 */ -2913,
/* 2919 */ 2,
/* 2920 */ 2924,
/* 2921 */ 50058,
/* 2922 */ -2920,
/* 2923 */ 1,
/* 2924 */ 0,
/* 2925 */ -2920,
/* 2926 */ 2,
/* 2927 */ 2931,
/* 2928 */ 50058,
/* 2929 */ -2927,
/* 2930 */ 1,
/* 2931 */ 0,
/* 2932 */ -2927,
/* 2933 */ 2,
/* 2934 */ 2940,
/* 2935 */ 1,
/* 2936 */ 1,
/* 2937 */ 1,
/* 2938 */ -2934,
/* 2939 */ 1,
/* 2940 */ 0,
/* 2941 */ -2934,
/* 2942 */ 2,
/* 2943 */ 2947,
/* 2944 */ 50124,
/* 2945 */ -2943,
/* 2946 */ 1,
/* 2947 */ 2951,
/* 2948 */ 50044,
/* 2949 */ -2943,
/* 2950 */ 2,
/* 2951 */ 0,
/* 2952 */ -2943,
/* 2953 */ 3,
/* 2954 */ 0,
/* 2955 */ 50309,
/* 2956 */ 50257,
/* 2957 */ 50123,
/* 2958 */ 1,
/* 2959 */ 50125,
/* 2960 */ -2954,
/* 2961 */ 1,
/* 2962 */ 2967,
/* 2963 */ 1,
/* 2964 */ 1,
/* 2965 */ -2962,
/* 2966 */ 1,
/* 2967 */ 0,
/* 2968 */ -2962,
/* 2969 */ 2,
/* 2970 */ 2977,
/* 2971 */ 50326,
/* 2972 */ 1,
/* 2973 */ 50257,
/* 2974 */ 50059,
/* 2975 */ -2970,
/* 2976 */ 1,
/* 2977 */ 2985,
/* 2978 */ 50364,
/* 2979 */ 1,
/* 2980 */ 1,
/* 2981 */ 1,
/* 2982 */ 50059,
/* 2983 */ -2970,
/* 2984 */ 2,
/* 2985 */ 2993,
/* 2986 */ 50381,
/* 2987 */ 1,
/* 2988 */ 1,
/* 2989 */ 1,
/* 2990 */ 50059,
/* 2991 */ -2970,
/* 2992 */ 3,
/* 2993 */ 3002,
/* 2994 */ 50323,
/* 2995 */ 1,
/* 2996 */ 1,
/* 2997 */ 50047,
/* 2998 */ 1,
/* 2999 */ 50059,
/* 3000 */ -2970,
/* 3001 */ 4,
/* 3002 */ 3009,
/* 3003 */ 50296,
/* 3004 */ 1,
/* 3005 */ 1,
/* 3006 */ 50059,
/* 3007 */ -2970,
/* 3008 */ 5,
/* 3009 */ 3016,
/* 3010 */ 50429,
/* 3011 */ 1,
/* 3012 */ 1,
/* 3013 */ 50059,
/* 3014 */ -2970,
/* 3015 */ 6,
/* 3016 */ 3023,
/* 3017 */ 50359,
/* 3018 */ 1,
/* 3019 */ 1,
/* 3020 */ 50059,
/* 3021 */ -2970,
/* 3022 */ 7,
/* 3023 */ 3033,
/* 3024 */ 50432,
/* 3025 */ 1,
/* 3026 */ 1,
/* 3027 */ 1,
/* 3028 */ 1,
/* 3029 */ 1,
/* 3030 */ 50059,
/* 3031 */ -2970,
/* 3032 */ 8,
/* 3033 */ 3040,
/* 3034 */ 50411,
/* 3035 */ 1,
/* 3036 */ 1,
/* 3037 */ 50059,
/* 3038 */ -2970,
/* 3039 */ 9,
/* 3040 */ 3047,
/* 3041 */ 50350,
/* 3042 */ 1,
/* 3043 */ 1,
/* 3044 */ 50059,
/* 3045 */ -2970,
/* 3046 */ 10,
/* 3047 */ 3051,
/* 3048 */ 1,
/* 3049 */ -2970,
/* 3050 */ 11,
/* 3051 */ 3055,
/* 3052 */ 1,
/* 3053 */ -2970,
/* 3054 */ 12,
/* 3055 */ 3059,
/* 3056 */ 1,
/* 3057 */ -2970,
/* 3058 */ 13,
/* 3059 */ 0,
/* 3060 */ 1,
/* 3061 */ -2970,
/* 3062 */ 14,
/* 3063 */ 3067,
/* 3064 */ 50058,
/* 3065 */ -3063,
/* 3066 */ 1,
/* 3067 */ 0,
/* 3068 */ -3063,
/* 3069 */ 2,
/* 3070 */ 3074,
/* 3071 */ 50058,
/* 3072 */ -3070,
/* 3073 */ 1,
/* 3074 */ 0,
/* 3075 */ -3070,
/* 3076 */ 2,
/* 3077 */ 3081,
/* 3078 */ 50100,
/* 3079 */ -3077,
/* 3080 */ 1,
/* 3081 */ 0,
/* 3082 */ -3077,
/* 3083 */ 2,
/* 3084 */ 3088,
/* 3085 */ 50058,
/* 3086 */ -3084,
/* 3087 */ 1,
/* 3088 */ 0,
/* 3089 */ -3084,
/* 3090 */ 2,
/* 3091 */ 3095,
/* 3092 */ 50100,
/* 3093 */ -3091,
/* 3094 */ 1,
/* 3095 */ 0,
/* 3096 */ -3091,
/* 3097 */ 2,
/* 3098 */ 3102,
/* 3099 */ 50058,
/* 3100 */ -3098,
/* 3101 */ 1,
/* 3102 */ 0,
/* 3103 */ -3098,
/* 3104 */ 2,
/* 3105 */ 3109,
/* 3106 */ 50058,
/* 3107 */ -3105,
/* 3108 */ 1,
/* 3109 */ 0,
/* 3110 */ -3105,
/* 3111 */ 2,
/* 3112 */ 3119,
/* 3113 */ 1,
/* 3114 */ 1,
/* 3115 */ 1,
/* 3116 */ 1,
/* 3117 */ -3112,
/* 3118 */ 1,
/* 3119 */ 0,
/* 3120 */ -3112,
/* 3121 */ 2,
/* 3122 */ 3126,
/* 3123 */ 50044,
/* 3124 */ -3122,
/* 3125 */ 1,
/* 3126 */ 0,
/* 3127 */ -3122,
/* 3128 */ 2,
/* 3129 */ 3133,
/* 3130 */ 50058,
/* 3131 */ -3129,
/* 3132 */ 1,
/* 3133 */ 0,
/* 3134 */ -3129,
/* 3135 */ 2,
/* 3136 */ 3143,
/* 3137 */ 1,
/* 3138 */ 1,
/* 3139 */ 1,
/* 3140 */ 1,
/* 3141 */ -3136,
/* 3142 */ 1,
/* 3143 */ 0,
/* 3144 */ -3136,
/* 3145 */ 2,
/* 3146 */ 3150,
/* 3147 */ 50044,
/* 3148 */ -3146,
/* 3149 */ 1,
/* 3150 */ 0,
/* 3151 */ -3146,
/* 3152 */ 2,
/* 3153 */ 3157,
/* 3154 */ 50058,
/* 3155 */ -3153,
/* 3156 */ 1,
/* 3157 */ 0,
/* 3158 */ -3153,
/* 3159 */ 2,
/* 3160 */ 3166,
/* 3161 */ 1,
/* 3162 */ 1,
/* 3163 */ 1,
/* 3164 */ -3160,
/* 3165 */ 1,
/* 3166 */ 0,
/* 3167 */ -3160,
/* 3168 */ 2,
/* 3169 */ 3173,
/* 3170 */ 50044,
/* 3171 */ -3169,
/* 3172 */ 1,
/* 3173 */ 0,
/* 3174 */ -3169,
/* 3175 */ 2,
/* 3176 */ 0,
/* 3177 */ 50091,
/* 3178 */ -3176,
/* 3179 */ 1,
/* 3180 */ 0,
/* 3181 */ 50093,
/* 3182 */ -3180,
/* 3183 */ 1,
/* 3184 */ 3188,
/* 3185 */ 50058,
/* 3186 */ -3184,
/* 3187 */ 1,
/* 3188 */ 0,
/* 3189 */ -3184,
/* 3190 */ 2,
/* 3191 */ 3197,
/* 3192 */ 1,
/* 3193 */ 1,
/* 3194 */ 1,
/* 3195 */ -3191,
/* 3196 */ 1,
/* 3197 */ 0,
/* 3198 */ -3191,
/* 3199 */ 2,
/* 3200 */ 3204,
/* 3201 */ 50044,
/* 3202 */ -3200,
/* 3203 */ 1,
/* 3204 */ 0,
/* 3205 */ -3200,
/* 3206 */ 2,
/* 3207 */ 3211,
/* 3208 */ 50058,
/* 3209 */ -3207,
/* 3210 */ 1,
/* 3211 */ 0,
/* 3212 */ -3207,
/* 3213 */ 2,
/* 3214 */ 3220,
/* 3215 */ 1,
/* 3216 */ 1,
/* 3217 */ 1,
/* 3218 */ -3214,
/* 3219 */ 1,
/* 3220 */ 0,
/* 3221 */ -3214,
/* 3222 */ 2,
/* 3223 */ 3227,
/* 3224 */ 50044,
/* 3225 */ -3223,
/* 3226 */ 1,
/* 3227 */ 0,
/* 3228 */ -3223,
/* 3229 */ 2,
/* 3230 */ 3234,
/* 3231 */ 50058,
/* 3232 */ -3230,
/* 3233 */ 1,
/* 3234 */ 0,
/* 3235 */ -3230,
/* 3236 */ 2,
/* 3237 */ 3243,
/* 3238 */ 1,
/* 3239 */ 1,
/* 3240 */ 1,
/* 3241 */ -3237,
/* 3242 */ 1,
/* 3243 */ 0,
/* 3244 */ -3237,
/* 3245 */ 2,
/* 3246 */ 3250,
/* 3247 */ 50124,
/* 3248 */ -3246,
/* 3249 */ 1,
/* 3250 */ 3254,
/* 3251 */ 50044,
/* 3252 */ -3246,
/* 3253 */ 2,
/* 3254 */ 0,
/* 3255 */ -3246,
/* 3256 */ 3,
/* 3257 */ 3267,
/* 3258 */ 50282,
/* 3259 */ 50091,
/* 3260 */ 1,
/* 3261 */ 1,
/* 3262 */ 50093,
/* 3263 */ 50295,
/* 3264 */ 1,
/* 3265 */ -3257,
/* 3266 */ 1,
/* 3267 */ 3277,
/* 3268 */ 50440,
/* 3269 */ 50091,
/* 3270 */ 1,
/* 3271 */ 1,
/* 3272 */ 50093,
/* 3273 */ 50295,
/* 3274 */ 1,
/* 3275 */ -3257,
/* 3276 */ 2,
/* 3277 */ 0,
/* 3278 */ 50347,
/* 3279 */ 50091,
/* 3280 */ 1,
/* 3281 */ 1,
/* 3282 */ 50093,
/* 3283 */ 50295,
/* 3284 */ 1,
/* 3285 */ -3257,
/* 3286 */ 3,
/* 3287 */ 3291,
/* 3288 */ 1,
/* 3289 */ -3287,
/* 3290 */ 1,
/* 3291 */ 0,
/* 3292 */ -3287,
/* 3293 */ 2,
/* 3294 */ 3298,
/* 3295 */ 1,
/* 3296 */ -3294,
/* 3297 */ 1,
/* 3298 */ 0,
/* 3299 */ -3294,
/* 3300 */ 2,
/* 3301 */ 3305,
/* 3302 */ 1,
/* 3303 */ -3301,
/* 3304 */ 1,
/* 3305 */ 0,
/* 3306 */ -3301,
/* 3307 */ 2,
/* 3308 */ 3313,
/* 3309 */ 50044,
/* 3310 */ 1,
/* 3311 */ -3308,
/* 3312 */ 1,
/* 3313 */ 3318,
/* 3314 */ 50044,
/* 3315 */ 1,
/* 3316 */ -3308,
/* 3317 */ 2,
/* 3318 */ 0,
/* 3319 */ 50044,
/* 3320 */ 1,
/* 3321 */ 50044,
/* 3322 */ 1,
/* 3323 */ -3308,
/* 3324 */ 3,
/* 3325 */ 3330,
/* 3326 */ 1,
/* 3327 */ 1,
/* 3328 */ -3325,
/* 3329 */ 1,
/* 3330 */ 3336,
/* 3331 */ 50343,
/* 3332 */ 1,
/* 3333 */ 50374,
/* 3334 */ -3325,
/* 3335 */ 2,
/* 3336 */ 3341,
/* 3337 */ 50343,
/* 3338 */ 50374,
/* 3339 */ -3325,
/* 3340 */ 3,
/* 3341 */ 3350,
/* 3342 */ 50343,
/* 3343 */ 50374,
/* 3344 */ 50439,
/* 3345 */ 50295,
/* 3346 */ 1,
/* 3347 */ 1,
/* 3348 */ -3325,
/* 3349 */ 4,
/* 3350 */ 0,
/* 3351 */ 50343,
/* 3352 */ 1,
/* 3353 */ 50374,
/* 3354 */ 50439,
/* 3355 */ 50295,
/* 3356 */ 1,
/* 3357 */ 1,
/* 3358 */ -3325,
/* 3359 */ 5,
/* 3360 */ 3364,
/* 3361 */ 50374,
/* 3362 */ -3360,
/* 3363 */ 1,
/* 3364 */ 0,
/* 3365 */ -3360,
/* 3366 */ 2,
/* 3367 */ 3371,
/* 3368 */ 50374,
/* 3369 */ -3367,
/* 3370 */ 1,
/* 3371 */ 0,
/* 3372 */ -3367,
/* 3373 */ 2,
/* 3374 */ 3378,
/* 3375 */ 50374,
/* 3376 */ -3374,
/* 3377 */ 1,
/* 3378 */ 0,
/* 3379 */ -3374,
/* 3380 */ 2,
/* 3381 */ 0,
/* 3382 */ 50332,
/* 3383 */ 50257,
/* 3384 */ 50123,
/* 3385 */ 1,
/* 3386 */ 50125,
/* 3387 */ -3381,
/* 3388 */ 1,
/* 3389 */ 3394,
/* 3390 */ 1,
/* 3391 */ 1,
/* 3392 */ -3389,
/* 3393 */ 1,
/* 3394 */ 0,
/* 3395 */ -3389,
/* 3396 */ 2,
/* 3397 */ 3404,
/* 3398 */ 50326,
/* 3399 */ 1,
/* 3400 */ 50257,
/* 3401 */ 50059,
/* 3402 */ -3397,
/* 3403 */ 1,
/* 3404 */ 3408,
/* 3405 */ 1,
/* 3406 */ -3397,
/* 3407 */ 2,
/* 3408 */ 3412,
/* 3409 */ 1,
/* 3410 */ -3397,
/* 3411 */ 3,
/* 3412 */ 0,
/* 3413 */ 1,
/* 3414 */ -3397,
/* 3415 */ 4,
/* 3416 */ 3420,
/* 3417 */ 50058,
/* 3418 */ -3416,
/* 3419 */ 1,
/* 3420 */ 0,
/* 3421 */ -3416,
/* 3422 */ 2,
/* 3423 */ 3430,
/* 3424 */ 50311,
/* 3425 */ 1,
/* 3426 */ 1,
/* 3427 */ 50059,
/* 3428 */ -3423,
/* 3429 */ 1,
/* 3430 */ 3436,
/* 3431 */ 50377,
/* 3432 */ 1,
/* 3433 */ 1,
/* 3434 */ -3423,
/* 3435 */ 2,
/* 3436 */ 0,
/* 3437 */ 50434,
/* 3438 */ 1,
/* 3439 */ 1,
/* 3440 */ 50059,
/* 3441 */ -3423,
/* 3442 */ 3,
/* 3443 */ 3447,
/* 3444 */ 50058,
/* 3445 */ -3443,
/* 3446 */ 1,
/* 3447 */ 0,
/* 3448 */ -3443,
/* 3449 */ 2,
/* 3450 */ 3458,
/* 3451 */ 50042,
/* 3452 */ 1,
/* 3453 */ 1,
/* 3454 */ 1,
/* 3455 */ 1,
/* 3456 */ -3450,
/* 3457 */ 1,
/* 3458 */ 3466,
/* 3459 */ 50042,
/* 3460 */ 1,
/* 3461 */ 1,
/* 3462 */ 1,
/* 3463 */ 1,
/* 3464 */ -3450,
/* 3465 */ 2,
/* 3466 */ 0,
/* 3467 */ -3450,
/* 3468 */ 3,
/* 3469 */ 3473,
/* 3470 */ 50044,
/* 3471 */ -3469,
/* 3472 */ 1,
/* 3473 */ 0,
/* 3474 */ -3469,
/* 3475 */ 2,
/* 3476 */ 3480,
/* 3477 */ 50044,
/* 3478 */ -3476,
/* 3479 */ 1,
/* 3480 */ 0,
/* 3481 */ -3476,
/* 3482 */ 2,
/* 3483 */ 3487,
/* 3484 */ 50058,
/* 3485 */ -3483,
/* 3486 */ 1,
/* 3487 */ 0,
/* 3488 */ -3483,
/* 3489 */ 2,
/* 3490 */ 3498,
/* 3491 */ 50042,
/* 3492 */ 1,
/* 3493 */ 1,
/* 3494 */ 1,
/* 3495 */ 1,
/* 3496 */ -3490,
/* 3497 */ 1,
/* 3498 */ 0,
/* 3499 */ -3490,
/* 3500 */ 2,
/* 3501 */ 3506,
/* 3502 */ 1,
/* 3503 */ 1,
/* 3504 */ -3501,
/* 3505 */ 1,
/* 3506 */ 0,
/* 3507 */ -3501,
/* 3508 */ 2,
/* 3509 */ 3513,
/* 3510 */ 50044,
/* 3511 */ -3509,
/* 3512 */ 1,
/* 3513 */ 0,
/* 3514 */ 50059,
/* 3515 */ -3509,
/* 3516 */ 2,
/* 3517 */ 3521,
/* 3518 */ 50058,
/* 3519 */ -3517,
/* 3520 */ 1,
/* 3521 */ 0,
/* 3522 */ -3517,
/* 3523 */ 2,
/* 3524 */ 3532,
/* 3525 */ 50042,
/* 3526 */ 1,
/* 3527 */ 1,
/* 3528 */ 1,
/* 3529 */ 1,
/* 3530 */ -3524,
/* 3531 */ 1,
/* 3532 */ 0,
/* 3533 */ -3524,
/* 3534 */ 2,
/* 3535 */ 3540,
/* 3536 */ 1,
/* 3537 */ 50037,
/* 3538 */ -3535,
/* 3539 */ 1,
/* 3540 */ 0,
/* 3541 */ -3535,
/* 3542 */ 2,
/* 3543 */ 3550,
/* 3544 */ 1,
/* 3545 */ 50295,
/* 3546 */ 50374,
/* 3547 */ 1,
/* 3548 */ -3543,
/* 3549 */ 1,
/* 3550 */ 3557,
/* 3551 */ 1,
/* 3552 */ 50295,
/* 3553 */ 1,
/* 3554 */ 50374,
/* 3555 */ -3543,
/* 3556 */ 2,
/* 3557 */ 0,
/* 3558 */ 1,
/* 3559 */ 50295,
/* 3560 */ 50374,
/* 3561 */ 1,
/* 3562 */ -3543,
/* 3563 */ 3,
/* 3564 */ 3568,
/* 3565 */ 50044,
/* 3566 */ -3564,
/* 3567 */ 1,
/* 3568 */ 0,
/* 3569 */ -3564,
/* 3570 */ 2,
/* 3571 */ 3575,
/* 3572 */ 1,
/* 3573 */ -3571,
/* 3574 */ 1,
/* 3575 */ 3579,
/* 3576 */ 1,
/* 3577 */ -3571,
/* 3578 */ 2,
/* 3579 */ 0,
/* 3580 */ 50257,
/* 3581 */ -3571,
/* 3582 */ 3,
/* 3583 */ 0,
/* 3584 */ 50293,
/* 3585 */ 50257,
/* 3586 */ 50123,
/* 3587 */ 1,
/* 3588 */ 50125,
/* 3589 */ -3583,
/* 3590 */ 1,
/* 3591 */ 3596,
/* 3592 */ 1,
/* 3593 */ 1,
/* 3594 */ -3591,
/* 3595 */ 1,
/* 3596 */ 0,
/* 3597 */ -3591,
/* 3598 */ 2,
/* 3599 */ 3606,
/* 3600 */ 50326,
/* 3601 */ 1,
/* 3602 */ 50257,
/* 3603 */ 50059,
/* 3604 */ -3599,
/* 3605 */ 1,
/* 3606 */ 3613,
/* 3607 */ 50372,
/* 3608 */ 1,
/* 3609 */ 1,
/* 3610 */ 50059,
/* 3611 */ -3599,
/* 3612 */ 2,
/* 3613 */ 3618,
/* 3614 */ 1,
/* 3615 */ 50059,
/* 3616 */ -3599,
/* 3617 */ 3,
/* 3618 */ 3625,
/* 3619 */ 50424,
/* 3620 */ 1,
/* 3621 */ 1,
/* 3622 */ 50059,
/* 3623 */ -3599,
/* 3624 */ 4,
/* 3625 */ 3632,
/* 3626 */ 50444,
/* 3627 */ 1,
/* 3628 */ 1,
/* 3629 */ 50059,
/* 3630 */ -3599,
/* 3631 */ 5,
/* 3632 */ 3639,
/* 3633 */ 50415,
/* 3634 */ 1,
/* 3635 */ 1,
/* 3636 */ 50059,
/* 3637 */ -3599,
/* 3638 */ 6,
/* 3639 */ 3646,
/* 3640 */ 50338,
/* 3641 */ 50353,
/* 3642 */ 1,
/* 3643 */ 50059,
/* 3644 */ -3599,
/* 3645 */ 7,
/* 3646 */ 3653,
/* 3647 */ 50470,
/* 3648 */ 50353,
/* 3649 */ 1,
/* 3650 */ 50059,
/* 3651 */ -3599,
/* 3652 */ 8,
/* 3653 */ 3660,
/* 3654 */ 50366,
/* 3655 */ 50353,
/* 3656 */ 1,
/* 3657 */ 50059,
/* 3658 */ -3599,
/* 3659 */ 9,
/* 3660 */ 3667,
/* 3661 */ 50371,
/* 3662 */ 50353,
/* 3663 */ 1,
/* 3664 */ 50059,
/* 3665 */ -3599,
/* 3666 */ 10,
/* 3667 */ 3671,
/* 3668 */ 1,
/* 3669 */ -3599,
/* 3670 */ 11,
/* 3671 */ 0,
/* 3672 */ 1,
/* 3673 */ -3599,
/* 3674 */ 12,
/* 3675 */ 3679,
/* 3676 */ 50058,
/* 3677 */ -3675,
/* 3678 */ 1,
/* 3679 */ 0,
/* 3680 */ -3675,
/* 3681 */ 2,
/* 3682 */ 3686,
/* 3683 */ 50058,
/* 3684 */ -3682,
/* 3685 */ 1,
/* 3686 */ 0,
/* 3687 */ -3682,
/* 3688 */ 2,
/* 3689 */ 3697,
/* 3690 */ 50277,
/* 3691 */ 1,
/* 3692 */ 1,
/* 3693 */ 1,
/* 3694 */ 1,
/* 3695 */ -3689,
/* 3696 */ 1,
/* 3697 */ 0,
/* 3698 */ -3689,
/* 3699 */ 2,
/* 3700 */ 3704,
/* 3701 */ 50058,
/* 3702 */ -3700,
/* 3703 */ 1,
/* 3704 */ 0,
/* 3705 */ -3700,
/* 3706 */ 2,
/* 3707 */ 3711,
/* 3708 */ 50044,
/* 3709 */ -3707,
/* 3710 */ 1,
/* 3711 */ 0,
/* 3712 */ -3707,
/* 3713 */ 2,
/* 3714 */ 3718,
/* 3715 */ 50058,
/* 3716 */ -3714,
/* 3717 */ 1,
/* 3718 */ 0,
/* 3719 */ -3714,
/* 3720 */ 2,
/* 3721 */ 3727,
/* 3722 */ 1,
/* 3723 */ 1,
/* 3724 */ 1,
/* 3725 */ -3721,
/* 3726 */ 1,
/* 3727 */ 0,
/* 3728 */ -3721,
/* 3729 */ 2,
/* 3730 */ 3734,
/* 3731 */ 50044,
/* 3732 */ -3730,
/* 3733 */ 1,
/* 3734 */ 0,
/* 3735 */ -3730,
/* 3736 */ 2,
/* 3737 */ 3741,
/* 3738 */ 50058,
/* 3739 */ -3737,
/* 3740 */ 1,
/* 3741 */ 0,
/* 3742 */ -3737,
/* 3743 */ 2,
/* 3744 */ 3750,
/* 3745 */ 50277,
/* 3746 */ 1,
/* 3747 */ 1,
/* 3748 */ -3744,
/* 3749 */ 1,
/* 3750 */ 0,
/* 3751 */ -3744,
/* 3752 */ 2,
/* 3753 */ 3757,
/* 3754 */ 50044,
/* 3755 */ -3753,
/* 3756 */ 1,
/* 3757 */ 0,
/* 3758 */ -3753,
/* 3759 */ 2,
/* 3760 */ 3764,
/* 3761 */ 50058,
/* 3762 */ -3760,
/* 3763 */ 1,
/* 3764 */ 0,
/* 3765 */ -3760,
/* 3766 */ 2,
/* 3767 */ 3773,
/* 3768 */ 1,
/* 3769 */ 1,
/* 3770 */ 1,
/* 3771 */ -3767,
/* 3772 */ 1,
/* 3773 */ 0,
/* 3774 */ -3767,
/* 3775 */ 2,
/* 3776 */ 3780,
/* 3777 */ 50044,
/* 3778 */ -3776,
/* 3779 */ 1,
/* 3780 */ 0,
/* 3781 */ -3776,
/* 3782 */ 2,
/* 3783 */ 3789,
/* 3784 */ 1,
/* 3785 */ 1,
/* 3786 */ 1,
/* 3787 */ -3783,
/* 3788 */ 1,
/* 3789 */ 0,
/* 3790 */ -3783,
/* 3791 */ 2,
/* 3792 */ 0,
/* 3793 */ 50044,
/* 3794 */ -3792,
/* 3795 */ 1,
/* 3796 */ 3802,
/* 3797 */ 1,
/* 3798 */ 1,
/* 3799 */ 1,
/* 3800 */ -3796,
/* 3801 */ 1,
/* 3802 */ 0,
/* 3803 */ -3796,
/* 3804 */ 2,
/* 3805 */ 0,
/* 3806 */ 50044,
/* 3807 */ -3805,
/* 3808 */ 1,
/* 3809 */ 3815,
/* 3810 */ 1,
/* 3811 */ 1,
/* 3812 */ 1,
/* 3813 */ -3809,
/* 3814 */ 1,
/* 3815 */ 0,
/* 3816 */ -3809,
/* 3817 */ 2,
/* 3818 */ 0,
/* 3819 */ 50044,
/* 3820 */ -3818,
/* 3821 */ 1,
/* 3822 */ 3828,
/* 3823 */ 1,
/* 3824 */ 1,
/* 3825 */ 1,
/* 3826 */ -3822,
/* 3827 */ 1,
/* 3828 */ 0,
/* 3829 */ -3822,
/* 3830 */ 2,
/* 3831 */ 0,
/* 3832 */ 50044,
/* 3833 */ -3831,
/* 3834 */ 1,
/* 3835 */ 3844,
/* 3836 */ 50440,
/* 3837 */ 50091,
/* 3838 */ 1,
/* 3839 */ 50044,
/* 3840 */ 1,
/* 3841 */ 50093,
/* 3842 */ -3835,
/* 3843 */ 1,
/* 3844 */ 3851,
/* 3845 */ 50440,
/* 3846 */ 50091,
/* 3847 */ 1,
/* 3848 */ 50093,
/* 3849 */ -3835,
/* 3850 */ 2,
/* 3851 */ 3856,
/* 3852 */ 1,
/* 3853 */ 1,
/* 3854 */ -3835,
/* 3855 */ 3,
/* 3856 */ 3863,
/* 3857 */ 1,
/* 3858 */ 50343,
/* 3859 */ 1,
/* 3860 */ 50459,
/* 3861 */ -3835,
/* 3862 */ 4,
/* 3863 */ 3870,
/* 3864 */ 1,
/* 3865 */ 1,
/* 3866 */ 1,
/* 3867 */ 50321,
/* 3868 */ -3835,
/* 3869 */ 5,
/* 3870 */ 3877,
/* 3871 */ 1,
/* 3872 */ 1,
/* 3873 */ 1,
/* 3874 */ 50465,
/* 3875 */ -3835,
/* 3876 */ 6,
/* 3877 */ 3882,
/* 3878 */ 50370,
/* 3879 */ 1,
/* 3880 */ -3835,
/* 3881 */ 7,
/* 3882 */ 0,
/* 3883 */ 50347,
/* 3884 */ 1,
/* 3885 */ -3835,
/* 3886 */ 8,
/* 3887 */ 3892,
/* 3888 */ 50295,
/* 3889 */ 50279,
/* 3890 */ -3887,
/* 3891 */ 1,
/* 3892 */ 0,
/* 3893 */ -3887,
/* 3894 */ 2,
/* 3895 */ 3899,
/* 3896 */ 50047,
/* 3897 */ -3895,
/* 3898 */ 1,
/* 3899 */ 0,
/* 3900 */ 50408,
/* 3901 */ -3895,
/* 3902 */ 2,
/* 3903 */ 3907,
/* 3904 */ 50047,
/* 3905 */ -3903,
/* 3906 */ 1,
/* 3907 */ 0,
/* 3908 */ 50408,
/* 3909 */ -3903,
/* 3910 */ 2,
/* 3911 */ 0,
/* 3912 */ 50328,
/* 3913 */ 50257,
/* 3914 */ 50058,
/* 3915 */ 1,
/* 3916 */ 50123,
/* 3917 */ 1,
/* 3918 */ 50125,
/* 3919 */ -3911,
/* 3920 */ 1,
/* 3921 */ 3926,
/* 3922 */ 1,
/* 3923 */ 1,
/* 3924 */ -3921,
/* 3925 */ 1,
/* 3926 */ 0,
/* 3927 */ -3921,
/* 3928 */ 2,
/* 3929 */ 3936,
/* 3930 */ 50437,
/* 3931 */ 1,
/* 3932 */ 1,
/* 3933 */ 50059,
/* 3934 */ -3929,
/* 3935 */ 1,
/* 3936 */ 3943,
/* 3937 */ 50359,
/* 3938 */ 1,
/* 3939 */ 1,
/* 3940 */ 50059,
/* 3941 */ -3929,
/* 3942 */ 2,
/* 3943 */ 3950,
/* 3944 */ 50326,
/* 3945 */ 1,
/* 3946 */ 50257,
/* 3947 */ 50059,
/* 3948 */ -3929,
/* 3949 */ 3,
/* 3950 */ 3954,
/* 3951 */ 1,
/* 3952 */ -3929,
/* 3953 */ 4,
/* 3954 */ 3958,
/* 3955 */ 1,
/* 3956 */ -3929,
/* 3957 */ 5,
/* 3958 */ 0,
/* 3959 */ 1,
/* 3960 */ -3929,
/* 3961 */ 6,
/* 3962 */ 3966,
/* 3963 */ 50058,
/* 3964 */ -3962,
/* 3965 */ 1,
/* 3966 */ 0,
/* 3967 */ -3962,
/* 3968 */ 2,
/* 3969 */ 3975,
/* 3970 */ 1,
/* 3971 */ 1,
/* 3972 */ 1,
/* 3973 */ -3969,
/* 3974 */ 1,
/* 3975 */ 0,
/* 3976 */ -3969,
/* 3977 */ 2,
/* 3978 */ 3982,
/* 3979 */ 50044,
/* 3980 */ -3978,
/* 3981 */ 1,
/* 3982 */ 0,
/* 3983 */ -3978,
/* 3984 */ 2,
/* 3985 */ 3989,
/* 3986 */ 50058,
/* 3987 */ -3985,
/* 3988 */ 1,
/* 3989 */ 0,
/* 3990 */ -3985,
/* 3991 */ 2,
/* 3992 */ 3998,
/* 3993 */ 1,
/* 3994 */ 1,
/* 3995 */ 1,
/* 3996 */ -3992,
/* 3997 */ 1,
/* 3998 */ 0,
/* 3999 */ -3992,
/* 4000 */ 2,
/* 4001 */ 4005,
/* 4002 */ 50044,
/* 4003 */ -4001,
/* 4004 */ 1,
/* 4005 */ 0,
/* 4006 */ -4001,
/* 4007 */ 2,
/* 4008 */ 4012,
/* 4009 */ 50058,
/* 4010 */ -4008,
/* 4011 */ 1,
/* 4012 */ 0,
/* 4013 */ -4008,
/* 4014 */ 2,
/* 4015 */ 0,
/* 4016 */ 50358,
/* 4017 */ 50257,
/* 4018 */ 50123,
/* 4019 */ 1,
/* 4020 */ 50125,
/* 4021 */ -4015,
/* 4022 */ 1,
/* 4023 */ 4028,
/* 4024 */ 1,
/* 4025 */ 1,
/* 4026 */ -4023,
/* 4027 */ 1,
/* 4028 */ 0,
/* 4029 */ -4023,
/* 4030 */ 2,
/* 4031 */ 4038,
/* 4032 */ 50329,
/* 4033 */ 1,
/* 4034 */ 1,
/* 4035 */ 50059,
/* 4036 */ -4031,
/* 4037 */ 1,
/* 4038 */ 4043,
/* 4039 */ 50311,
/* 4040 */ 50059,
/* 4041 */ -4031,
/* 4042 */ 2,
/* 4043 */ 4050,
/* 4044 */ 50350,
/* 4045 */ 1,
/* 4046 */ 1,
/* 4047 */ 50059,
/* 4048 */ -4031,
/* 4049 */ 3,
/* 4050 */ 4057,
/* 4051 */ 50326,
/* 4052 */ 1,
/* 4053 */ 50257,
/* 4054 */ 50059,
/* 4055 */ -4031,
/* 4056 */ 4,
/* 4057 */ 4064,
/* 4058 */ 50359,
/* 4059 */ 1,
/* 4060 */ 1,
/* 4061 */ 50059,
/* 4062 */ -4031,
/* 4063 */ 5,
/* 4064 */ 4068,
/* 4065 */ 1,
/* 4066 */ -4031,
/* 4067 */ 6,
/* 4068 */ 4072,
/* 4069 */ 1,
/* 4070 */ -4031,
/* 4071 */ 7,
/* 4072 */ 0,
/* 4073 */ 1,
/* 4074 */ -4031,
/* 4075 */ 8,
/* 4076 */ 4080,
/* 4077 */ 50058,
/* 4078 */ -4076,
/* 4079 */ 1,
/* 4080 */ 0,
/* 4081 */ -4076,
/* 4082 */ 2,
/* 4083 */ 4089,
/* 4084 */ 1,
/* 4085 */ 1,
/* 4086 */ 1,
/* 4087 */ -4083,
/* 4088 */ 1,
/* 4089 */ 0,
/* 4090 */ -4083,
/* 4091 */ 2,
/* 4092 */ 4096,
/* 4093 */ 50044,
/* 4094 */ -4092,
/* 4095 */ 1,
/* 4096 */ 0,
/* 4097 */ -4092,
/* 4098 */ 2,
/* 4099 */ 4103,
/* 4100 */ 50058,
/* 4101 */ -4099,
/* 4102 */ 1,
/* 4103 */ 0,
/* 4104 */ -4099,
/* 4105 */ 2,
/* 4106 */ 4112,
/* 4107 */ 1,
/* 4108 */ 1,
/* 4109 */ 1,
/* 4110 */ -4106,
/* 4111 */ 1,
/* 4112 */ 0,
/* 4113 */ -4106,
/* 4114 */ 2,
/* 4115 */ 4119,
/* 4116 */ 50124,
/* 4117 */ -4115,
/* 4118 */ 1,
/* 4119 */ 4123,
/* 4120 */ 50044,
/* 4121 */ -4115,
/* 4122 */ 2,
/* 4123 */ 0,
/* 4124 */ -4115,
/* 4125 */ 3,
/* 4126 */ 4130,
/* 4127 */ 50058,
/* 4128 */ -4126,
/* 4129 */ 1,
/* 4130 */ 0,
/* 4131 */ -4126,
/* 4132 */ 2,
/* 4133 */ 4137,
/* 4134 */ 50058,
/* 4135 */ -4133,
/* 4136 */ 1,
/* 4137 */ 0,
/* 4138 */ -4133,
/* 4139 */ 2,
/* 4140 */ 4146,
/* 4141 */ 1,
/* 4142 */ 1,
/* 4143 */ 1,
/* 4144 */ -4140,
/* 4145 */ 1,
/* 4146 */ 0,
/* 4147 */ -4140,
/* 4148 */ 2,
/* 4149 */ 4153,
/* 4150 */ 50044,
/* 4151 */ -4149,
/* 4152 */ 1,
/* 4153 */ 0,
/* 4154 */ -4149,
/* 4155 */ 2,
/* 4156 */ 0,
/* 4157 */ 50450,
/* 4158 */ 50257,
/* 4159 */ 50123,
/* 4160 */ 1,
/* 4161 */ 50125,
/* 4162 */ -4156,
/* 4163 */ 1,
/* 4164 */ 4169,
/* 4165 */ 1,
/* 4166 */ 1,
/* 4167 */ -4164,
/* 4168 */ 1,
/* 4169 */ 0,
/* 4170 */ -4164,
/* 4171 */ 2,
/* 4172 */ 4179,
/* 4173 */ 50326,
/* 4174 */ 1,
/* 4175 */ 50257,
/* 4176 */ 50059,
/* 4177 */ -4172,
/* 4178 */ 1,
/* 4179 */ 4183,
/* 4180 */ 1,
/* 4181 */ -4172,
/* 4182 */ 2,
/* 4183 */ 4191,
/* 4184 */ 50389,
/* 4185 */ 1,
/* 4186 */ 1,
/* 4187 */ 50037,
/* 4188 */ 50059,
/* 4189 */ -4172,
/* 4190 */ 3,
/* 4191 */ 4198,
/* 4192 */ 50407,
/* 4193 */ 1,
/* 4194 */ 1,
/* 4195 */ 50059,
/* 4196 */ -4172,
/* 4197 */ 4,
/* 4198 */ 4205,
/* 4199 */ 50383,
/* 4200 */ 1,
/* 4201 */ 1,
/* 4202 */ 50059,
/* 4203 */ -4172,
/* 4204 */ 5,
/* 4205 */ 4212,
/* 4206 */ 50350,
/* 4207 */ 1,
/* 4208 */ 1,
/* 4209 */ 50059,
/* 4210 */ -4172,
/* 4211 */ 6,
/* 4212 */ 4216,
/* 4213 */ 1,
/* 4214 */ -4172,
/* 4215 */ 7,
/* 4216 */ 4220,
/* 4217 */ 1,
/* 4218 */ -4172,
/* 4219 */ 8,
/* 4220 */ 0,
/* 4221 */ 1,
/* 4222 */ -4172,
/* 4223 */ 9,
/* 4224 */ 4228,
/* 4225 */ 50058,
/* 4226 */ -4224,
/* 4227 */ 1,
/* 4228 */ 0,
/* 4229 */ -4224,
/* 4230 */ 2,
/* 4231 */ 4235,
/* 4232 */ 50058,
/* 4233 */ -4231,
/* 4234 */ 1,
/* 4235 */ 0,
/* 4236 */ -4231,
/* 4237 */ 2,
/* 4238 */ 4242,
/* 4239 */ 50058,
/* 4240 */ -4238,
/* 4241 */ 1,
/* 4242 */ 0,
/* 4243 */ -4238,
/* 4244 */ 2,
/* 4245 */ 4249,
/* 4246 */ 50058,
/* 4247 */ -4245,
/* 4248 */ 1,
/* 4249 */ 0,
/* 4250 */ -4245,
/* 4251 */ 2,
/* 4252 */ 4256,
/* 4253 */ 50058,
/* 4254 */ -4252,
/* 4255 */ 1,
/* 4256 */ 0,
/* 4257 */ -4252,
/* 4258 */ 2,
/* 4259 */ 4265,
/* 4260 */ 1,
/* 4261 */ 1,
/* 4262 */ 1,
/* 4263 */ -4259,
/* 4264 */ 1,
/* 4265 */ 0,
/* 4266 */ -4259,
/* 4267 */ 2,
/* 4268 */ 4272,
/* 4269 */ 50124,
/* 4270 */ -4268,
/* 4271 */ 1,
/* 4272 */ 4276,
/* 4273 */ 50044,
/* 4274 */ -4268,
/* 4275 */ 2,
/* 4276 */ 0,
/* 4277 */ -4268,
/* 4278 */ 3,
/* 4279 */ 0,
/* 4280 */ 50422,
/* 4281 */ 50257,
/* 4282 */ 50058,
/* 4283 */ 1,
/* 4284 */ 50123,
/* 4285 */ 1,
/* 4286 */ 50125,
/* 4287 */ -4279,
/* 4288 */ 1,
/* 4289 */ 4294,
/* 4290 */ 1,
/* 4291 */ 1,
/* 4292 */ -4289,
/* 4293 */ 1,
/* 4294 */ 0,
/* 4295 */ -4289,
/* 4296 */ 2,
/* 4297 */ 4304,
/* 4298 */ 50326,
/* 4299 */ 1,
/* 4300 */ 50257,
/* 4301 */ 50059,
/* 4302 */ -4297,
/* 4303 */ 1,
/* 4304 */ 4311,
/* 4305 */ 50464,
/* 4306 */ 1,
/* 4307 */ 1,
/* 4308 */ 50059,
/* 4309 */ -4297,
/* 4310 */ 2,
/* 4311 */ 4318,
/* 4312 */ 50351,
/* 4313 */ 1,
/* 4314 */ 1,
/* 4315 */ 50059,
/* 4316 */ -4297,
/* 4317 */ 3,
/* 4318 */ 4325,
/* 4319 */ 50327,
/* 4320 */ 1,
/* 4321 */ 1,
/* 4322 */ 50059,
/* 4323 */ -4297,
/* 4324 */ 4,
/* 4325 */ 4332,
/* 4326 */ 50325,
/* 4327 */ 1,
/* 4328 */ 1,
/* 4329 */ 50059,
/* 4330 */ -4297,
/* 4331 */ 5,
/* 4332 */ 4339,
/* 4333 */ 50431,
/* 4334 */ 1,
/* 4335 */ 1,
/* 4336 */ 50059,
/* 4337 */ -4297,
/* 4338 */ 6,
/* 4339 */ 4346,
/* 4340 */ 50426,
/* 4341 */ 1,
/* 4342 */ 1,
/* 4343 */ 50059,
/* 4344 */ -4297,
/* 4345 */ 7,
/* 4346 */ 4353,
/* 4347 */ 50350,
/* 4348 */ 1,
/* 4349 */ 1,
/* 4350 */ 50059,
/* 4351 */ -4297,
/* 4352 */ 8,
/* 4353 */ 4360,
/* 4354 */ 50360,
/* 4355 */ 50058,
/* 4356 */ 50260,
/* 4357 */ 50059,
/* 4358 */ -4297,
/* 4359 */ 9,
/* 4360 */ 4367,
/* 4361 */ 50455,
/* 4362 */ 50058,
/* 4363 */ 1,
/* 4364 */ 50059,
/* 4365 */ -4297,
/* 4366 */ 10,
/* 4367 */ 4373,
/* 4368 */ 50423,
/* 4369 */ 50058,
/* 4370 */ 1,
/* 4371 */ -4297,
/* 4372 */ 11,
/* 4373 */ 4379,
/* 4374 */ 50398,
/* 4375 */ 50058,
/* 4376 */ 1,
/* 4377 */ -4297,
/* 4378 */ 12,
/* 4379 */ 4383,
/* 4380 */ 1,
/* 4381 */ -4297,
/* 4382 */ 13,
/* 4383 */ 4387,
/* 4384 */ 1,
/* 4385 */ -4297,
/* 4386 */ 14,
/* 4387 */ 0,
/* 4388 */ 1,
/* 4389 */ -4297,
/* 4390 */ 15,
/* 4391 */ 4395,
/* 4392 */ 50058,
/* 4393 */ -4391,
/* 4394 */ 1,
/* 4395 */ 0,
/* 4396 */ -4391,
/* 4397 */ 2,
/* 4398 */ 4402,
/* 4399 */ 50058,
/* 4400 */ -4398,
/* 4401 */ 1,
/* 4402 */ 0,
/* 4403 */ -4398,
/* 4404 */ 2,
/* 4405 */ 4409,
/* 4406 */ 50058,
/* 4407 */ -4405,
/* 4408 */ 1,
/* 4409 */ 0,
/* 4410 */ -4405,
/* 4411 */ 2,
/* 4412 */ 4416,
/* 4413 */ 50058,
/* 4414 */ -4412,
/* 4415 */ 1,
/* 4416 */ 0,
/* 4417 */ -4412,
/* 4418 */ 2,
/* 4419 */ 4423,
/* 4420 */ 50058,
/* 4421 */ -4419,
/* 4422 */ 1,
/* 4423 */ 0,
/* 4424 */ -4419,
/* 4425 */ 2,
/* 4426 */ 4430,
/* 4427 */ 50058,
/* 4428 */ -4426,
/* 4429 */ 1,
/* 4430 */ 0,
/* 4431 */ -4426,
/* 4432 */ 2,
/* 4433 */ 4437,
/* 4434 */ 50058,
/* 4435 */ -4433,
/* 4436 */ 1,
/* 4437 */ 0,
/* 4438 */ -4433,
/* 4439 */ 2,
/* 4440 */ 4446,
/* 4441 */ 1,
/* 4442 */ 1,
/* 4443 */ 1,
/* 4444 */ -4440,
/* 4445 */ 1,
/* 4446 */ 0,
/* 4447 */ -4440,
/* 4448 */ 2,
/* 4449 */ 4453,
/* 4450 */ 50044,
/* 4451 */ -4449,
/* 4452 */ 1,
/* 4453 */ 4457,
/* 4454 */ 50124,
/* 4455 */ -4449,
/* 4456 */ 2,
/* 4457 */ 0,
/* 4458 */ -4449,
/* 4459 */ 3,
/* 4460 */ 4464,
/* 4461 */ 50058,
/* 4462 */ -4460,
/* 4463 */ 1,
/* 4464 */ 0,
/* 4465 */ -4460,
/* 4466 */ 2,
/* 4467 */ 4473,
/* 4468 */ 1,
/* 4469 */ 1,
/* 4470 */ 1,
/* 4471 */ -4467,
/* 4472 */ 1,
/* 4473 */ 0,
/* 4474 */ -4467,
/* 4475 */ 2,
/* 4476 */ 4480,
/* 4477 */ 50124,
/* 4478 */ -4476,
/* 4479 */ 1,
/* 4480 */ 4484,
/* 4481 */ 50044,
/* 4482 */ -4476,
/* 4483 */ 2,
/* 4484 */ 0,
/* 4485 */ -4476,
/* 4486 */ 3,
/* 4487 */ 4496,
/* 4488 */ 50259,
/* 4489 */ 50058,
/* 4490 */ 1,
/* 4491 */ 1,
/* 4492 */ 1,
/* 4493 */ 1,
/* 4494 */ -4487,
/* 4495 */ 1,
/* 4496 */ 0,
/* 4497 */ -4487,
/* 4498 */ 2,
/* 4499 */ 4506,
/* 4500 */ 50091,
/* 4501 */ 1,
/* 4502 */ 50093,
/* 4503 */ 1,
/* 4504 */ -4499,
/* 4505 */ 1,
/* 4506 */ 4513,
/* 4507 */ 50471,
/* 4508 */ 1,
/* 4509 */ 1,
/* 4510 */ 1,
/* 4511 */ -4499,
/* 4512 */ 2,
/* 4513 */ 4519,
/* 4514 */ 50294,
/* 4515 */ 1,
/* 4516 */ 1,
/* 4517 */ -4499,
/* 4518 */ 3,
/* 4519 */ 0,
/* 4520 */ -4499,
/* 4521 */ 4,
/* 4522 */ 4526,
/* 4523 */ 1,
/* 4524 */ -4522,
/* 4525 */ 1,
/* 4526 */ 4532,
/* 4527 */ 50419,
/* 4528 */ 50372,
/* 4529 */ 1,
/* 4530 */ -4522,
/* 4531 */ 2,
/* 4532 */ 0,
/* 4533 */ 50419,
/* 4534 */ 50388,
/* 4535 */ 1,
/* 4536 */ -4522,
/* 4537 */ 3,
/* 4538 */ 4542,
/* 4539 */ 1,
/* 4540 */ -4538,
/* 4541 */ 1,
/* 4542 */ 0,
/* 4543 */ -4538,
/* 4544 */ 2,
/* 4545 */ 4549,
/* 4546 */ 1,
/* 4547 */ -4545,
/* 4548 */ 1,
/* 4549 */ 0,
/* 4550 */ -4545,
/* 4551 */ 2,
/* 4552 */ 4557,
/* 4553 */ 50399,
/* 4554 */ 1,
/* 4555 */ -4552,
/* 4556 */ 1,
/* 4557 */ 4565,
/* 4558 */ 50399,
/* 4559 */ 50419,
/* 4560 */ 1,
/* 4561 */ 50457,
/* 4562 */ 1,
/* 4563 */ -4552,
/* 4564 */ 2,
/* 4565 */ 4570,
/* 4566 */ 50399,
/* 4567 */ 1,
/* 4568 */ -4552,
/* 4569 */ 3,
/* 4570 */ 0,
/* 4571 */ -4552,
/* 4572 */ 4,
/* 4573 */ 4577,
/* 4574 */ 50044,
/* 4575 */ -4573,
/* 4576 */ 1,
/* 4577 */ 0,
/* 4578 */ -4573,
/* 4579 */ 2,
/* 4580 */ 4585,
/* 4581 */ 1,
/* 4582 */ 1,
/* 4583 */ -4580,
/* 4584 */ 1,
/* 4585 */ 0,
/* 4586 */ -4580,
/* 4587 */ 2,
/* 4588 */ 4593,
/* 4589 */ 1,
/* 4590 */ 1,
/* 4591 */ -4588,
/* 4592 */ 1,
/* 4593 */ 0,
/* 4594 */ -4588,
/* 4595 */ 2,
/* 4596 */ 0,
/* 4597 */ 50449,
/* 4598 */ 50257,
/* 4599 */ 50058,
/* 4600 */ 1,
/* 4601 */ 1,
/* 4602 */ 50123,
/* 4603 */ 1,
/* 4604 */ 50125,
/* 4605 */ -4596,
/* 4606 */ 1,
/* 4607 */ 4612,
/* 4608 */ 50353,
/* 4609 */ 1,
/* 4610 */ -4607,
/* 4611 */ 1,
/* 4612 */ 0,
/* 4613 */ -4607,
/* 4614 */ 2,
/* 4615 */ 4620,
/* 4616 */ 1,
/* 4617 */ 1,
/* 4618 */ -4615,
/* 4619 */ 1,
/* 4620 */ 0,
/* 4621 */ -4615,
/* 4622 */ 2,
/* 4623 */ 4630,
/* 4624 */ 50326,
/* 4625 */ 1,
/* 4626 */ 50257,
/* 4627 */ 50059,
/* 4628 */ -4623,
/* 4629 */ 1,
/* 4630 */ 4637,
/* 4631 */ 50353,
/* 4632 */ 1,
/* 4633 */ 1,
/* 4634 */ 50059,
/* 4635 */ -4623,
/* 4636 */ 2,
/* 4637 */ 4644,
/* 4638 */ 50390,
/* 4639 */ 1,
/* 4640 */ 1,
/* 4641 */ 50059,
/* 4642 */ -4623,
/* 4643 */ 3,
/* 4644 */ 4648,
/* 4645 */ 1,
/* 4646 */ -4623,
/* 4647 */ 4,
/* 4648 */ 4655,
/* 4649 */ 1,
/* 4650 */ 1,
/* 4651 */ 1,
/* 4652 */ 50059,
/* 4653 */ -4623,
/* 4654 */ 5,
/* 4655 */ 4662,
/* 4656 */ 50364,
/* 4657 */ 1,
/* 4658 */ 1,
/* 4659 */ 50059,
/* 4660 */ -4623,
/* 4661 */ 6,
/* 4662 */ 4669,
/* 4663 */ 50366,
/* 4664 */ 1,
/* 4665 */ 1,
/* 4666 */ 50059,
/* 4667 */ -4623,
/* 4668 */ 7,
/* 4669 */ 4676,
/* 4670 */ 50323,
/* 4671 */ 1,
/* 4672 */ 1,
/* 4673 */ 50059,
/* 4674 */ -4623,
/* 4675 */ 8,
/* 4676 */ 4683,
/* 4677 */ 50292,
/* 4678 */ 1,
/* 4679 */ 1,
/* 4680 */ 50059,
/* 4681 */ -4623,
/* 4682 */ 9,
/* 4683 */ 4690,
/* 4684 */ 50319,
/* 4685 */ 1,
/* 4686 */ 1,
/* 4687 */ 50059,
/* 4688 */ -4623,
/* 4689 */ 10,
/* 4690 */ 4697,
/* 4691 */ 50410,
/* 4692 */ 1,
/* 4693 */ 1,
/* 4694 */ 50059,
/* 4695 */ -4623,
/* 4696 */ 11,
/* 4697 */ 4705,
/* 4698 */ 50306,
/* 4699 */ 50374,
/* 4700 */ 1,
/* 4701 */ 1,
/* 4702 */ 50059,
/* 4703 */ -4623,
/* 4704 */ 12,
/* 4705 */ 4712,
/* 4706 */ 50431,
/* 4707 */ 1,
/* 4708 */ 1,
/* 4709 */ 50059,
/* 4710 */ -4623,
/* 4711 */ 13,
/* 4712 */ 4719,
/* 4713 */ 50435,
/* 4714 */ 1,
/* 4715 */ 1,
/* 4716 */ 50059,
/* 4717 */ -4623,
/* 4718 */ 14,
/* 4719 */ 4726,
/* 4720 */ 50389,
/* 4721 */ 1,
/* 4722 */ 1,
/* 4723 */ 50059,
/* 4724 */ -4623,
/* 4725 */ 15,
/* 4726 */ 4733,
/* 4727 */ 50277,
/* 4728 */ 1,
/* 4729 */ 1,
/* 4730 */ 1,
/* 4731 */ -4623,
/* 4732 */ 16,
/* 4733 */ 4741,
/* 4734 */ 50296,
/* 4735 */ 1,
/* 4736 */ 1,
/* 4737 */ 1,
/* 4738 */ 50059,
/* 4739 */ -4623,
/* 4740 */ 17,
/* 4741 */ 4749,
/* 4742 */ 50440,
/* 4743 */ 50091,
/* 4744 */ 1,
/* 4745 */ 50093,
/* 4746 */ 50059,
/* 4747 */ -4623,
/* 4748 */ 18,
/* 4749 */ 4759,
/* 4750 */ 50440,
/* 4751 */ 50091,
/* 4752 */ 1,
/* 4753 */ 50044,
/* 4754 */ 1,
/* 4755 */ 50093,
/* 4756 */ 50059,
/* 4757 */ -4623,
/* 4758 */ 19,
/* 4759 */ 4771,
/* 4760 */ 50440,
/* 4761 */ 50091,
/* 4762 */ 1,
/* 4763 */ 50044,
/* 4764 */ 1,
/* 4765 */ 50044,
/* 4766 */ 1,
/* 4767 */ 50093,
/* 4768 */ 50059,
/* 4769 */ -4623,
/* 4770 */ 20,
/* 4771 */ 4785,
/* 4772 */ 50440,
/* 4773 */ 50091,
/* 4774 */ 1,
/* 4775 */ 50044,
/* 4776 */ 1,
/* 4777 */ 50044,
/* 4778 */ 1,
/* 4779 */ 50044,
/* 4780 */ 1,
/* 4781 */ 50093,
/* 4782 */ 50059,
/* 4783 */ -4623,
/* 4784 */ 21,
/* 4785 */ 4792,
/* 4786 */ 50424,
/* 4787 */ 1,
/* 4788 */ 1,
/* 4789 */ 50059,
/* 4790 */ -4623,
/* 4791 */ 22,
/* 4792 */ 4799,
/* 4793 */ 50369,
/* 4794 */ 1,
/* 4795 */ 1,
/* 4796 */ 50059,
/* 4797 */ -4623,
/* 4798 */ 23,
/* 4799 */ 4807,
/* 4800 */ 50346,
/* 4801 */ 1,
/* 4802 */ 1,
/* 4803 */ 1,
/* 4804 */ 50059,
/* 4805 */ -4623,
/* 4806 */ 24,
/* 4807 */ 4814,
/* 4808 */ 50437,
/* 4809 */ 1,
/* 4810 */ 1,
/* 4811 */ 50059,
/* 4812 */ -4623,
/* 4813 */ 25,
/* 4814 */ 4821,
/* 4815 */ 50330,
/* 4816 */ 1,
/* 4817 */ 1,
/* 4818 */ 50059,
/* 4819 */ -4623,
/* 4820 */ 26,
/* 4821 */ 4828,
/* 4822 */ 50409,
/* 4823 */ 1,
/* 4824 */ 1,
/* 4825 */ 50059,
/* 4826 */ -4623,
/* 4827 */ 27,
/* 4828 */ 4834,
/* 4829 */ 50350,
/* 4830 */ 1,
/* 4831 */ 1,
/* 4832 */ -4623,
/* 4833 */ 28,
/* 4834 */ 4838,
/* 4835 */ 1,
/* 4836 */ -4623,
/* 4837 */ 29,
/* 4838 */ 4842,
/* 4839 */ 1,
/* 4840 */ -4623,
/* 4841 */ 30,
/* 4842 */ 4846,
/* 4843 */ 1,
/* 4844 */ -4623,
/* 4845 */ 31,
/* 4846 */ 0,
/* 4847 */ 1,
/* 4848 */ -4623,
/* 4849 */ 32,
/* 4850 */ 4854,
/* 4851 */ 50058,
/* 4852 */ -4850,
/* 4853 */ 1,
/* 4854 */ 0,
/* 4855 */ -4850,
/* 4856 */ 2,
/* 4857 */ 4861,
/* 4858 */ 50058,
/* 4859 */ -4857,
/* 4860 */ 1,
/* 4861 */ 0,
/* 4862 */ -4857,
/* 4863 */ 2,
/* 4864 */ 4868,
/* 4865 */ 50058,
/* 4866 */ -4864,
/* 4867 */ 1,
/* 4868 */ 0,
/* 4869 */ -4864,
/* 4870 */ 2,
/* 4871 */ 4875,
/* 4872 */ 50374,
/* 4873 */ -4871,
/* 4874 */ 1,
/* 4875 */ 0,
/* 4876 */ 50307,
/* 4877 */ -4871,
/* 4878 */ 2,
/* 4879 */ 4883,
/* 4880 */ 50058,
/* 4881 */ -4879,
/* 4882 */ 1,
/* 4883 */ 0,
/* 4884 */ -4879,
/* 4885 */ 2,
/* 4886 */ 4890,
/* 4887 */ 50058,
/* 4888 */ -4886,
/* 4889 */ 1,
/* 4890 */ 0,
/* 4891 */ -4886,
/* 4892 */ 2,
/* 4893 */ 4897,
/* 4894 */ 50058,
/* 4895 */ -4893,
/* 4896 */ 1,
/* 4897 */ 0,
/* 4898 */ -4893,
/* 4899 */ 2,
/* 4900 */ 4904,
/* 4901 */ 50058,
/* 4902 */ -4900,
/* 4903 */ 1,
/* 4904 */ 0,
/* 4905 */ -4900,
/* 4906 */ 2,
/* 4907 */ 4911,
/* 4908 */ 50058,
/* 4909 */ -4907,
/* 4910 */ 1,
/* 4911 */ 0,
/* 4912 */ -4907,
/* 4913 */ 2,
/* 4914 */ 4918,
/* 4915 */ 50058,
/* 4916 */ -4914,
/* 4917 */ 1,
/* 4918 */ 0,
/* 4919 */ -4914,
/* 4920 */ 2,
/* 4921 */ 4925,
/* 4922 */ 50058,
/* 4923 */ -4921,
/* 4924 */ 1,
/* 4925 */ 0,
/* 4926 */ -4921,
/* 4927 */ 2,
/* 4928 */ 4932,
/* 4929 */ 50058,
/* 4930 */ -4928,
/* 4931 */ 1,
/* 4932 */ 0,
/* 4933 */ -4928,
/* 4934 */ 2,
/* 4935 */ 4939,
/* 4936 */ 50058,
/* 4937 */ -4935,
/* 4938 */ 1,
/* 4939 */ 0,
/* 4940 */ -4935,
/* 4941 */ 2,
/* 4942 */ 4946,
/* 4943 */ 50058,
/* 4944 */ -4942,
/* 4945 */ 1,
/* 4946 */ 0,
/* 4947 */ -4942,
/* 4948 */ 2,
/* 4949 */ 4953,
/* 4950 */ 50058,
/* 4951 */ -4949,
/* 4952 */ 1,
/* 4953 */ 0,
/* 4954 */ -4949,
/* 4955 */ 2,
/* 4956 */ 4960,
/* 4957 */ 50058,
/* 4958 */ -4956,
/* 4959 */ 1,
/* 4960 */ 0,
/* 4961 */ -4956,
/* 4962 */ 2,
/* 4963 */ 4967,
/* 4964 */ 50044,
/* 4965 */ -4963,
/* 4966 */ 1,
/* 4967 */ 0,
/* 4968 */ 50059,
/* 4969 */ -4963,
/* 4970 */ 2,
/* 4971 */ 4975,
/* 4972 */ 50058,
/* 4973 */ -4971,
/* 4974 */ 1,
/* 4975 */ 0,
/* 4976 */ -4971,
/* 4977 */ 2,
/* 4978 */ 4984,
/* 4979 */ 50044,
/* 4980 */ 1,
/* 4981 */ 1,
/* 4982 */ -4978,
/* 4983 */ 1,
/* 4984 */ 0,
/* 4985 */ -4978,
/* 4986 */ 2,
/* 4987 */ 4991,
/* 4988 */ 50058,
/* 4989 */ -4987,
/* 4990 */ 1,
/* 4991 */ 0,
/* 4992 */ -4987,
/* 4993 */ 2,
/* 4994 */ 5000,
/* 4995 */ 1,
/* 4996 */ 1,
/* 4997 */ 1,
/* 4998 */ -4994,
/* 4999 */ 1,
/* 5000 */ 0,
/* 5001 */ -4994,
/* 5002 */ 2,
/* 5003 */ 5007,
/* 5004 */ 1,
/* 5005 */ -5003,
/* 5006 */ 1,
/* 5007 */ 0,
/* 5008 */ 1,
/* 5009 */ 1,
/* 5010 */ -5003,
/* 5011 */ 2,
/* 5012 */ 5016,
/* 5013 */ 50126,
/* 5014 */ -5012,
/* 5015 */ 1,
/* 5016 */ 0,
/* 5017 */ 50033,
/* 5018 */ -5012,
/* 5019 */ 2,
/* 5020 */ 5024,
/* 5021 */ 50044,
/* 5022 */ -5020,
/* 5023 */ 1,
/* 5024 */ 0,
/* 5025 */ -5020,
/* 5026 */ 2,
/* 5027 */ 5031,
/* 5028 */ 50058,
/* 5029 */ -5027,
/* 5030 */ 1,
/* 5031 */ 0,
/* 5032 */ -5027,
/* 5033 */ 2,
/* 5034 */ 5040,
/* 5035 */ 1,
/* 5036 */ 1,
/* 5037 */ 1,
/* 5038 */ -5034,
/* 5039 */ 1,
/* 5040 */ 0,
/* 5041 */ -5034,
/* 5042 */ 2,
/* 5043 */ 5047,
/* 5044 */ 1,
/* 5045 */ -5043,
/* 5046 */ 1,
/* 5047 */ 0,
/* 5048 */ 1,
/* 5049 */ 1,
/* 5050 */ -5043,
/* 5051 */ 2,
/* 5052 */ 5056,
/* 5053 */ 50126,
/* 5054 */ -5052,
/* 5055 */ 1,
/* 5056 */ 0,
/* 5057 */ 50033,
/* 5058 */ -5052,
/* 5059 */ 2,
/* 5060 */ 5064,
/* 5061 */ 50044,
/* 5062 */ -5060,
/* 5063 */ 1,
/* 5064 */ 0,
/* 5065 */ -5060,
/* 5066 */ 2,
/* 5067 */ 5071,
/* 5068 */ 50058,
/* 5069 */ -5067,
/* 5070 */ 1,
/* 5071 */ 0,
/* 5072 */ -5067,
/* 5073 */ 2,
/* 5074 */ 5080,
/* 5075 */ 1,
/* 5076 */ 50044,
/* 5077 */ 1,
/* 5078 */ -5074,
/* 5079 */ 1,
/* 5080 */ 0,
/* 5081 */ -5074,
/* 5082 */ 2,
/* 5083 */ 5087,
/* 5084 */ 50058,
/* 5085 */ -5083,
/* 5086 */ 1,
/* 5087 */ 0,
/* 5088 */ -5083,
/* 5089 */ 2,
/* 5090 */ 5096,
/* 5091 */ 1,
/* 5092 */ 1,
/* 5093 */ 1,
/* 5094 */ -5090,
/* 5095 */ 1,
/* 5096 */ 0,
/* 5097 */ -5090,
/* 5098 */ 2,
/* 5099 */ 5103,
/* 5100 */ 50044,
/* 5101 */ -5099,
/* 5102 */ 1,
/* 5103 */ 0,
/* 5104 */ -5099,
/* 5105 */ 2,
/* 5106 */ 5110,
/* 5107 */ 50058,
/* 5108 */ -5106,
/* 5109 */ 1,
/* 5110 */ 0,
/* 5111 */ -5106,
/* 5112 */ 2,
/* 5113 */ 5119,
/* 5114 */ 1,
/* 5115 */ 1,
/* 5116 */ 1,
/* 5117 */ -5113,
/* 5118 */ 1,
/* 5119 */ 0,
/* 5120 */ -5113,
/* 5121 */ 2,
/* 5122 */ 5126,
/* 5123 */ 50044,
/* 5124 */ -5122,
/* 5125 */ 1,
/* 5126 */ 0,
/* 5127 */ -5122,
/* 5128 */ 2,
/* 5129 */ 5133,
/* 5130 */ 50058,
/* 5131 */ -5129,
/* 5132 */ 1,
/* 5133 */ 0,
/* 5134 */ -5129,
/* 5135 */ 2,
/* 5136 */ 5142,
/* 5137 */ 1,
/* 5138 */ 1,
/* 5139 */ 1,
/* 5140 */ -5136,
/* 5141 */ 1,
/* 5142 */ 0,
/* 5143 */ -5136,
/* 5144 */ 2,
/* 5145 */ 5149,
/* 5146 */ 50044,
/* 5147 */ -5145,
/* 5148 */ 1,
/* 5149 */ 0,
/* 5150 */ -5145,
/* 5151 */ 2,
/* 5152 */ 5156,
/* 5153 */ 50058,
/* 5154 */ -5152,
/* 5155 */ 1,
/* 5156 */ 0,
/* 5157 */ -5152,
/* 5158 */ 2,
/* 5159 */ 5165,
/* 5160 */ 1,
/* 5161 */ 1,
/* 5162 */ 1,
/* 5163 */ -5159,
/* 5164 */ 1,
/* 5165 */ 0,
/* 5166 */ -5159,
/* 5167 */ 2,
/* 5168 */ 5172,
/* 5169 */ 1,
/* 5170 */ -5168,
/* 5171 */ 1,
/* 5172 */ 0,
/* 5173 */ 1,
/* 5174 */ 1,
/* 5175 */ -5168,
/* 5176 */ 2,
/* 5177 */ 5181,
/* 5178 */ 50126,
/* 5179 */ -5177,
/* 5180 */ 1,
/* 5181 */ 0,
/* 5182 */ 50033,
/* 5183 */ -5177,
/* 5184 */ 2,
/* 5185 */ 5189,
/* 5186 */ 50044,
/* 5187 */ -5185,
/* 5188 */ 1,
/* 5189 */ 0,
/* 5190 */ 50059,
/* 5191 */ -5185,
/* 5192 */ 2,
/* 5193 */ 5203,
/* 5194 */ 1,
/* 5195 */ 50353,
/* 5196 */ 1,
/* 5197 */ 1,
/* 5198 */ 1,
/* 5199 */ 1,
/* 5200 */ 1,
/* 5201 */ -5193,
/* 5202 */ 1,
/* 5203 */ 0,
/* 5204 */ 1,
/* 5205 */ 50353,
/* 5206 */ 1,
/* 5207 */ 1,
/* 5208 */ -5193,
/* 5209 */ 2,
/* 5210 */ 5214,
/* 5211 */ 1,
/* 5212 */ -5210,
/* 5213 */ 1,
/* 5214 */ 5219,
/* 5215 */ 1,
/* 5216 */ 50037,
/* 5217 */ -5210,
/* 5218 */ 2,
/* 5219 */ 0,
/* 5220 */ -5210,
/* 5221 */ 3,
/* 5222 */ 5226,
/* 5223 */ 1,
/* 5224 */ -5222,
/* 5225 */ 1,
/* 5226 */ 0,
/* 5227 */ -5222,
/* 5228 */ 2,
/* 5229 */ 5233,
/* 5230 */ 50319,
/* 5231 */ -5229,
/* 5232 */ 1,
/* 5233 */ 0,
/* 5234 */ -5229,
/* 5235 */ 2,
/* 5236 */ 5243,
/* 5237 */ 50040,
/* 5238 */ 1,
/* 5239 */ 1,
/* 5240 */ 50041,
/* 5241 */ -5236,
/* 5242 */ 1,
/* 5243 */ 0,
/* 5244 */ -5236,
/* 5245 */ 2,
/* 5246 */ 5250,
/* 5247 */ 50322,
/* 5248 */ -5246,
/* 5249 */ 1,
/* 5250 */ 5254,
/* 5251 */ 50307,
/* 5252 */ -5246,
/* 5253 */ 2,
/* 5254 */ 0,
/* 5255 */ 50374,
/* 5256 */ -5246,
/* 5257 */ 3,
/* 5258 */ 5267,
/* 5259 */ 50288,
/* 5260 */ 1,
/* 5261 */ 1,
/* 5262 */ 1,
/* 5263 */ 1,
/* 5264 */ 1,
/* 5265 */ -5258,
/* 5266 */ 1,
/* 5267 */ 0,
/* 5268 */ -5258,
/* 5269 */ 2,
/* 5270 */ 5274,
/* 5271 */ 1,
/* 5272 */ -5270,
/* 5273 */ 1,
/* 5274 */ 0,
/* 5275 */ -5270,
/* 5276 */ 2,
/* 5277 */ 5281,
/* 5278 */ 1,
/* 5279 */ -5277,
/* 5280 */ 1,
/* 5281 */ 0,
/* 5282 */ -5277,
/* 5283 */ 2,
/* 5284 */ 5288,
/* 5285 */ 50319,
/* 5286 */ -5284,
/* 5287 */ 1,
/* 5288 */ 0,
/* 5289 */ -5284,
/* 5290 */ 2,
/* 5291 */ 5298,
/* 5292 */ 50040,
/* 5293 */ 1,
/* 5294 */ 1,
/* 5295 */ 50041,
/* 5296 */ -5291,
/* 5297 */ 1,
/* 5298 */ 0,
/* 5299 */ -5291,
/* 5300 */ 2,
/* 5301 */ 5305,
/* 5302 */ 50322,
/* 5303 */ -5301,
/* 5304 */ 1,
/* 5305 */ 5309,
/* 5306 */ 50307,
/* 5307 */ -5301,
/* 5308 */ 2,
/* 5309 */ 0,
/* 5310 */ 50374,
/* 5311 */ -5301,
/* 5312 */ 3,
/* 5313 */ 5317,
/* 5314 */ 1,
/* 5315 */ -5313,
/* 5316 */ 1,
/* 5317 */ 0,
/* 5318 */ -5313,
/* 5319 */ 2,
/* 5320 */ 0,
/* 5321 */ 50261,
/* 5322 */ -5320,
/* 5323 */ 1,
/* 5324 */ 0,
/* 5325 */ 50341,
/* 5326 */ 50257,
/* 5327 */ 50058,
/* 5328 */ 1,
/* 5329 */ 50123,
/* 5330 */ 1,
/* 5331 */ 50125,
/* 5332 */ -5324,
/* 5333 */ 1,
/* 5334 */ 5339,
/* 5335 */ 1,
/* 5336 */ 1,
/* 5337 */ -5334,
/* 5338 */ 1,
/* 5339 */ 0,
/* 5340 */ -5334,
/* 5341 */ 2,
/* 5342 */ 5349,
/* 5343 */ 50326,
/* 5344 */ 1,
/* 5345 */ 50257,
/* 5346 */ 50059,
/* 5347 */ -5342,
/* 5348 */ 1,
/* 5349 */ 5356,
/* 5350 */ 50383,
/* 5351 */ 1,
/* 5352 */ 1,
/* 5353 */ 50059,
/* 5354 */ -5342,
/* 5355 */ 2,
/* 5356 */ 5363,
/* 5357 */ 50310,
/* 5358 */ 1,
/* 5359 */ 1,
/* 5360 */ 50059,
/* 5361 */ -5342,
/* 5362 */ 3,
/* 5363 */ 0,
/* 5364 */ 50466,
/* 5365 */ 1,
/* 5366 */ 1,
/* 5367 */ 50037,
/* 5368 */ 50059,
/* 5369 */ -5342,
/* 5370 */ 4,
/* 5371 */ 5375,
/* 5372 */ 50058,
/* 5373 */ -5371,
/* 5374 */ 1,
/* 5375 */ 0,
/* 5376 */ -5371,
/* 5377 */ 2,
/* 5378 */ 5382,
/* 5379 */ 50058,
/* 5380 */ -5378,
/* 5381 */ 1,
/* 5382 */ 0,
/* 5383 */ -5378,
/* 5384 */ 2,
/* 5385 */ 5389,
/* 5386 */ 50058,
/* 5387 */ -5385,
/* 5388 */ 1,
/* 5389 */ 0,
/* 5390 */ -5385,
/* 5391 */ 2,
/* 5392 */ 5396,
/* 5393 */ 50058,
/* 5394 */ -5392,
/* 5395 */ 1,
/* 5396 */ 0,
/* 5397 */ -5392,
/* 5398 */ 2,
/* 5399 */ 0,
/* 5400 */ 50336,
/* 5401 */ 50257,
/* 5402 */ 1,
/* 5403 */ 50123,
/* 5404 */ 1,
/* 5405 */ 50125,
/* 5406 */ -5399,
/* 5407 */ 1,
/* 5408 */ 5413,
/* 5409 */ 50058,
/* 5410 */ 1,
/* 5411 */ -5408,
/* 5412 */ 1,
/* 5413 */ 0,
/* 5414 */ -5408,
/* 5415 */ 2,
/* 5416 */ 5421,
/* 5417 */ 1,
/* 5418 */ 1,
/* 5419 */ -5416,
/* 5420 */ 1,
/* 5421 */ 0,
/* 5422 */ -5416,
/* 5423 */ 2,
/* 5424 */ 5431,
/* 5425 */ 50450,
/* 5426 */ 50058,
/* 5427 */ 1,
/* 5428 */ 50059,
/* 5429 */ -5424,
/* 5430 */ 1,
/* 5431 */ 5438,
/* 5432 */ 50325,
/* 5433 */ 50058,
/* 5434 */ 1,
/* 5435 */ 50059,
/* 5436 */ -5424,
/* 5437 */ 2,
/* 5438 */ 5445,
/* 5439 */ 50466,
/* 5440 */ 50058,
/* 5441 */ 1,
/* 5442 */ 50059,
/* 5443 */ -5424,
/* 5444 */ 3,
/* 5445 */ 5452,
/* 5446 */ 50307,
/* 5447 */ 50058,
/* 5448 */ 1,
/* 5449 */ 50059,
/* 5450 */ -5424,
/* 5451 */ 4,
/* 5452 */ 5460,
/* 5453 */ 50307,
/* 5454 */ 50058,
/* 5455 */ 1,
/* 5456 */ 50043,
/* 5457 */ 50059,
/* 5458 */ -5424,
/* 5459 */ 5,
/* 5460 */ 5469,
/* 5461 */ 50307,
/* 5462 */ 50058,
/* 5463 */ 1,
/* 5464 */ 1,
/* 5465 */ 1,
/* 5466 */ 50059,
/* 5467 */ -5424,
/* 5468 */ 6,
/* 5469 */ 5476,
/* 5470 */ 50285,
/* 5471 */ 50058,
/* 5472 */ 1,
/* 5473 */ 50059,
/* 5474 */ -5424,
/* 5475 */ 7,
/* 5476 */ 5483,
/* 5477 */ 50406,
/* 5478 */ 50058,
/* 5479 */ 1,
/* 5480 */ 50059,
/* 5481 */ -5424,
/* 5482 */ 8,
/* 5483 */ 5490,
/* 5484 */ 50350,
/* 5485 */ 1,
/* 5486 */ 1,
/* 5487 */ 50059,
/* 5488 */ -5424,
/* 5489 */ 9,
/* 5490 */ 5494,
/* 5491 */ 1,
/* 5492 */ -5424,
/* 5493 */ 10,
/* 5494 */ 5498,
/* 5495 */ 1,
/* 5496 */ -5424,
/* 5497 */ 11,
/* 5498 */ 5502,
/* 5499 */ 1,
/* 5500 */ -5424,
/* 5501 */ 12,
/* 5502 */ 0,
/* 5503 */ 1,
/* 5504 */ -5424,
/* 5505 */ 13,
/* 5506 */ 5510,
/* 5507 */ 50457,
/* 5508 */ -5506,
/* 5509 */ 1,
/* 5510 */ 0,
/* 5511 */ 50045,
/* 5512 */ -5506,
/* 5513 */ 2,
/* 5514 */ 5519,
/* 5515 */ 1,
/* 5516 */ 1,
/* 5517 */ -5514,
/* 5518 */ 1,
/* 5519 */ 0,
/* 5520 */ -5514,
/* 5521 */ 2,
/* 5522 */ 5526,
/* 5523 */ 50058,
/* 5524 */ -5522,
/* 5525 */ 1,
/* 5526 */ 0,
/* 5527 */ -5522,
/* 5528 */ 2,
/* 5529 */ 5535,
/* 5530 */ 1,
/* 5531 */ 1,
/* 5532 */ 1,
/* 5533 */ -5529,
/* 5534 */ 1,
/* 5535 */ 0,
/* 5536 */ -5529,
/* 5537 */ 2,
/* 5538 */ 5542,
/* 5539 */ 50044,
/* 5540 */ -5538,
/* 5541 */ 1,
/* 5542 */ 5546,
/* 5543 */ 50124,
/* 5544 */ -5538,
/* 5545 */ 2,
/* 5546 */ 0,
/* 5547 */ -5538,
/* 5548 */ 3,
/* 5549 */ 0,
/* 5550 */ 1,
/* 5551 */ 1,
/* 5552 */ 50399,
/* 5553 */ 1,
/* 5554 */ 1,
/* 5555 */ 1,
/* 5556 */ -5549,
/* 5557 */ 1,
/* 5558 */ 5562,
/* 5559 */ 50042,
/* 5560 */ -5558,
/* 5561 */ 1,
/* 5562 */ 5566,
/* 5563 */ 50335,
/* 5564 */ -5558,
/* 5565 */ 2,
/* 5566 */ 0,
/* 5567 */ 50124,
/* 5568 */ -5558,
/* 5569 */ 3,
/* 5570 */ 5576,
/* 5571 */ 1,
/* 5572 */ 50037,
/* 5573 */ 1,
/* 5574 */ -5570,
/* 5575 */ 1,
/* 5576 */ 5585,
/* 5577 */ 50060,
/* 5578 */ 50307,
/* 5579 */ 1,
/* 5580 */ 50043,
/* 5581 */ 50062,
/* 5582 */ 1,
/* 5583 */ -5570,
/* 5584 */ 2,
/* 5585 */ 5592,
/* 5586 */ 50091,
/* 5587 */ 1,
/* 5588 */ 50093,
/* 5589 */ 1,
/* 5590 */ -5570,
/* 5591 */ 3,
/* 5592 */ 5599,
/* 5593 */ 50123,
/* 5594 */ 1,
/* 5595 */ 50125,
/* 5596 */ 1,
/* 5597 */ -5570,
/* 5598 */ 4,
/* 5599 */ 5606,
/* 5600 */ 1,
/* 5601 */ 1,
/* 5602 */ 1,
/* 5603 */ 1,
/* 5604 */ -5570,
/* 5605 */ 5,
/* 5606 */ 5611,
/* 5607 */ 1,
/* 5608 */ 1,
/* 5609 */ -5570,
/* 5610 */ 6,
/* 5611 */ 5617,
/* 5612 */ 1,
/* 5613 */ 50043,
/* 5614 */ 1,
/* 5615 */ -5570,
/* 5616 */ 7,
/* 5617 */ 5622,
/* 5618 */ 1,
/* 5619 */ 1,
/* 5620 */ -5570,
/* 5621 */ 8,
/* 5622 */ 0,
/* 5623 */ -5570,
/* 5624 */ 9,
/* 5625 */ 5629,
/* 5626 */ 50045,
/* 5627 */ -5625,
/* 5628 */ 1,
/* 5629 */ 0,
/* 5630 */ 50457,
/* 5631 */ -5625,
/* 5632 */ 2,
/* 5633 */ 5637,
/* 5634 */ 1,
/* 5635 */ -5633,
/* 5636 */ 1,
/* 5637 */ 5641,
/* 5638 */ 1,
/* 5639 */ -5633,
/* 5640 */ 2,
/* 5641 */ 0,
/* 5642 */ 50289,
/* 5643 */ 1,
/* 5644 */ -5633,
/* 5645 */ 3,
/* 5646 */ 5650,
/* 5647 */ 1,
/* 5648 */ -5646,
/* 5649 */ 1,
/* 5650 */ 5654,
/* 5651 */ 1,
/* 5652 */ -5646,
/* 5653 */ 2,
/* 5654 */ 5659,
/* 5655 */ 50289,
/* 5656 */ 1,
/* 5657 */ -5646,
/* 5658 */ 3,
/* 5659 */ 0,
/* 5660 */ -5646,
/* 5661 */ 4,
/* 5662 */ 5669,
/* 5663 */ 50091,
/* 5664 */ 1,
/* 5665 */ 1,
/* 5666 */ 50093,
/* 5667 */ -5662,
/* 5668 */ 1,
/* 5669 */ 0,
/* 5670 */ -5662,
/* 5671 */ 2,
/* 5672 */ 5678,
/* 5673 */ 1,
/* 5674 */ 1,
/* 5675 */ 1,
/* 5676 */ -5672,
/* 5677 */ 1,
/* 5678 */ 0,
/* 5679 */ -5672,
/* 5680 */ 2,
/* 5681 */ 5685,
/* 5682 */ 50124,
/* 5683 */ -5681,
/* 5684 */ 1,
/* 5685 */ 0,
/* 5686 */ 50044,
/* 5687 */ -5681,
/* 5688 */ 2,
/* 5689 */ 0,
/* 5690 */ 50299,
/* 5691 */ 50257,
/* 5692 */ 50058,
/* 5693 */ 1,
/* 5694 */ 50123,
/* 5695 */ 1,
/* 5696 */ 50125,
/* 5697 */ -5689,
/* 5698 */ 1,
/* 5699 */ 5704,
/* 5700 */ 1,
/* 5701 */ 1,
/* 5702 */ -5699,
/* 5703 */ 1,
/* 5704 */ 0,
/* 5705 */ -5699,
/* 5706 */ 2,
/* 5707 */ 5714,
/* 5708 */ 50436,
/* 5709 */ 1,
/* 5710 */ 1,
/* 5711 */ 50059,
/* 5712 */ -5707,
/* 5713 */ 1,
/* 5714 */ 5721,
/* 5715 */ 50350,
/* 5716 */ 1,
/* 5717 */ 1,
/* 5718 */ 50059,
/* 5719 */ -5707,
/* 5720 */ 2,
/* 5721 */ 5725,
/* 5722 */ 1,
/* 5723 */ -5707,
/* 5724 */ 3,
/* 5725 */ 5729,
/* 5726 */ 1,
/* 5727 */ -5707,
/* 5728 */ 4,
/* 5729 */ 0,
/* 5730 */ 1,
/* 5731 */ -5707,
/* 5732 */ 5,
/* 5733 */ 5737,
/* 5734 */ 50058,
/* 5735 */ -5733,
/* 5736 */ 1,
/* 5737 */ 0,
/* 5738 */ -5733,
/* 5739 */ 2,
/* 5740 */ 5744,
/* 5741 */ 50058,
/* 5742 */ -5740,
/* 5743 */ 1,
/* 5744 */ 0,
/* 5745 */ -5740,
/* 5746 */ 2,
/* 5747 */ 5753,
/* 5748 */ 1,
/* 5749 */ 1,
/* 5750 */ 1,
/* 5751 */ -5747,
/* 5752 */ 1,
/* 5753 */ 0,
/* 5754 */ -5747,
/* 5755 */ 2,
/* 5756 */ 5760,
/* 5757 */ 50124,
/* 5758 */ -5756,
/* 5759 */ 1,
/* 5760 */ 5764,
/* 5761 */ 50044,
/* 5762 */ -5756,
/* 5763 */ 2,
/* 5764 */ 0,
/* 5765 */ -5756,
/* 5766 */ 3,
/* 5767 */ 0,
/* 5768 */ 50451,
/* 5769 */ 50257,
/* 5770 */ 1,
/* 5771 */ 50059,
/* 5772 */ -5767,
/* 5773 */ 1,
/* 5774 */ 5779,
/* 5775 */ 50257,
/* 5776 */ 1,
/* 5777 */ -5774,
/* 5778 */ 1,
/* 5779 */ 5784,
/* 5780 */ 1,
/* 5781 */ 1,
/* 5782 */ -5774,
/* 5783 */ 2,
/* 5784 */ 5789,
/* 5785 */ 50040,
/* 5786 */ 1,
/* 5787 */ -5774,
/* 5788 */ 3,
/* 5789 */ 5794,
/* 5790 */ 50041,
/* 5791 */ 1,
/* 5792 */ -5774,
/* 5793 */ 4,
/* 5794 */ 5799,
/* 5795 */ 50124,
/* 5796 */ 1,
/* 5797 */ -5774,
/* 5798 */ 5,
/* 5799 */ 5804,
/* 5800 */ 50059,
/* 5801 */ 1,
/* 5802 */ -5774,
/* 5803 */ 6,
/* 5804 */ 5812,
/* 5805 */ 50091,
/* 5806 */ 1,
/* 5807 */ 50093,
/* 5808 */ 50058,
/* 5809 */ 1,
/* 5810 */ -5774,
/* 5811 */ 7,
/* 5812 */ 5819,
/* 5813 */ 50091,
/* 5814 */ 1,
/* 5815 */ 50093,
/* 5816 */ 1,
/* 5817 */ -5774,
/* 5818 */ 8,
/* 5819 */ 0,
/* 5820 */ -5774,
/* 5821 */ 9,
/* 5822 */ 0,
/* 5823 */ 50261,
/* 5824 */ -5822,
/* 5825 */ 1,
/* 5826 */ 5830,
/* 5827 */ 50264,
/* 5828 */ -5826,
/* 5829 */ 1,
/* 5830 */ 5834,
/* 5831 */ 50274,
/* 5832 */ -5826,
/* 5833 */ 2,
/* 5834 */ 5838,
/* 5835 */ 50268,
/* 5836 */ -5826,
/* 5837 */ 3,
/* 5838 */ 5842,
/* 5839 */ 50265,
/* 5840 */ -5826,
/* 5841 */ 4,
/* 5842 */ 5846,
/* 5843 */ 50266,
/* 5844 */ -5826,
/* 5845 */ 5,
/* 5846 */ 5850,
/* 5847 */ 50267,
/* 5848 */ -5826,
/* 5849 */ 6,
/* 5850 */ 5854,
/* 5851 */ 50263,
/* 5852 */ -5826,
/* 5853 */ 7,
/* 5854 */ 5858,
/* 5855 */ 50271,
/* 5856 */ -5826,
/* 5857 */ 8,
/* 5858 */ 5862,
/* 5859 */ 50272,
/* 5860 */ -5826,
/* 5861 */ 9,
/* 5862 */ 5866,
/* 5863 */ 50273,
/* 5864 */ -5826,
/* 5865 */ 10,
/* 5866 */ 0,
/* 5867 */ 50474,
/* 5868 */ -5826,
/* 5869 */ 11,
/* 5870 */ 0,
/* 5871 */ 50447,
/* 5872 */ 1,
/* 5873 */ 1,
/* 5874 */ 50262,
/* 5875 */ 50258,
/* 5876 */ 50040,
/* 5877 */ 1,
/* 5878 */ 1,
/* 5879 */ 50041,
/* 5880 */ 50059,
/* 5881 */ -5870,
/* 5882 */ 1,
/* 5883 */ 5891,
/* 5884 */ 1,
/* 5885 */ 1,
/* 5886 */ 1,
/* 5887 */ 1,
/* 5888 */ 1,
/* 5889 */ -5883,
/* 5890 */ 1,
/* 5891 */ 0,
/* 5892 */ -5883,
/* 5893 */ 2,
/* 5894 */ 5899,
/* 5895 */ 50058,
/* 5896 */ 1,
/* 5897 */ -5894,
/* 5898 */ 1,
/* 5899 */ 0,
/* 5900 */ -5894,
/* 5901 */ 2,
/* 5902 */ 5906,
/* 5903 */ 50258,
/* 5904 */ -5902,
/* 5905 */ 1,
/* 5906 */ 0,
/* 5907 */ -5902,
/* 5908 */ 2,
/* 5909 */ 5914,
/* 5910 */ 50061,
/* 5911 */ 1,
/* 5912 */ -5909,
/* 5913 */ 1,
/* 5914 */ 0,
/* 5915 */ -5909,
/* 5916 */ 2,
/* 5917 */ 5926,
/* 5918 */ 50044,
/* 5919 */ 1,
/* 5920 */ 1,
/* 5921 */ 1,
/* 5922 */ 1,
/* 5923 */ 1,
/* 5924 */ -5917,
/* 5925 */ 1,
/* 5926 */ 0,
/* 5927 */ -5917,
/* 5928 */ 2,
/* 5929 */ 5934,
/* 5930 */ 50058,
/* 5931 */ 1,
/* 5932 */ -5929,
/* 5933 */ 1,
/* 5934 */ 0,
/* 5935 */ -5929,
/* 5936 */ 2,
/* 5937 */ 5941,
/* 5938 */ 50258,
/* 5939 */ -5937,
/* 5940 */ 1,
/* 5941 */ 0,
/* 5942 */ -5937,
/* 5943 */ 2,
/* 5944 */ 5949,
/* 5945 */ 50061,
/* 5946 */ 1,
/* 5947 */ -5944,
/* 5948 */ 1,
/* 5949 */ 0,
/* 5950 */ -5944,
/* 5951 */ 2,
/* 5952 */ 5957,
/* 5953 */ 50044,
/* 5954 */ 50495,
/* 5955 */ -5952,
/* 5956 */ 1,
/* 5957 */ 0,
/* 5958 */ -5952,
/* 5959 */ 2,
/* 5960 */ 0,
/* 5961 */ 50447,
/* 5962 */ 1,
/* 5963 */ 50258,
/* 5964 */ 50040,
/* 5965 */ 1,
/* 5966 */ 1,
/* 5967 */ 50041,
/* 5968 */ 50059,
/* 5969 */ -5960,
/* 5970 */ 1,
/* 5971 */ 5979,
/* 5972 */ 1,
/* 5973 */ 1,
/* 5974 */ 1,
/* 5975 */ 1,
/* 5976 */ 1,
/* 5977 */ -5971,
/* 5978 */ 1,
/* 5979 */ 0,
/* 5980 */ -5971,
/* 5981 */ 2,
/* 5982 */ 5987,
/* 5983 */ 50058,
/* 5984 */ 1,
/* 5985 */ -5982,
/* 5986 */ 1,
/* 5987 */ 0,
/* 5988 */ -5982,
/* 5989 */ 2,
/* 5990 */ 5994,
/* 5991 */ 50258,
/* 5992 */ -5990,
/* 5993 */ 1,
/* 5994 */ 0,
/* 5995 */ -5990,
/* 5996 */ 2,
/* 5997 */ 6002,
/* 5998 */ 50061,
/* 5999 */ 1,
/* 6000 */ -5997,
/* 6001 */ 1,
/* 6002 */ 0,
/* 6003 */ -5997,
/* 6004 */ 2,
/* 6005 */ 6014,
/* 6006 */ 50044,
/* 6007 */ 1,
/* 6008 */ 1,
/* 6009 */ 1,
/* 6010 */ 1,
/* 6011 */ 1,
/* 6012 */ -6005,
/* 6013 */ 1,
/* 6014 */ 0,
/* 6015 */ -6005,
/* 6016 */ 2,
/* 6017 */ 6022,
/* 6018 */ 50058,
/* 6019 */ 1,
/* 6020 */ -6017,
/* 6021 */ 1,
/* 6022 */ 0,
/* 6023 */ -6017,
/* 6024 */ 2,
/* 6025 */ 6029,
/* 6026 */ 50258,
/* 6027 */ -6025,
/* 6028 */ 1,
/* 6029 */ 0,
/* 6030 */ -6025,
/* 6031 */ 2,
/* 6032 */ 6037,
/* 6033 */ 50061,
/* 6034 */ 1,
/* 6035 */ -6032,
/* 6036 */ 1,
/* 6037 */ 0,
/* 6038 */ -6032,
/* 6039 */ 2,
/* 6040 */ 6045,
/* 6041 */ 50044,
/* 6042 */ 50495,
/* 6043 */ -6040,
/* 6044 */ 1,
/* 6045 */ 0,
/* 6046 */ -6040,
/* 6047 */ 2,
/* 6048 */ 0,
/* 6049 */ 50447,
/* 6050 */ 1,
/* 6051 */ 1,
/* 6052 */ 50262,
/* 6053 */ 50258,
/* 6054 */ 50059,
/* 6055 */ -6048,
/* 6056 */ 1,
/* 6057 */ 0,
/* 6058 */ 50447,
/* 6059 */ 1,
/* 6060 */ 50258,
/* 6061 */ 50481,
/* 6062 */ 1,
/* 6063 */ 50059,
/* 6064 */ -6057,
/* 6065 */ 1,
/* 6066 */ 0,
/* 6067 */ 1,
/* 6068 */ 50258,
/* 6069 */ 1,
/* 6070 */ 50059,
/* 6071 */ -6066,
/* 6072 */ 1,
/* 6073 */ 6079,
/* 6074 */ 50044,
/* 6075 */ 50258,
/* 6076 */ 1,
/* 6077 */ -6073,
/* 6078 */ 1,
/* 6079 */ 0,
/* 6080 */ -6073,
/* 6081 */ 2,
/* 6082 */ 0,
/* 6083 */ 1,
/* 6084 */ 50258,
/* 6085 */ 1,
/* 6086 */ 50059,
/* 6087 */ -6082,
/* 6088 */ 1,
/* 6089 */ 6095,
/* 6090 */ 50044,
/* 6091 */ 50258,
/* 6092 */ 1,
/* 6093 */ -6089,
/* 6094 */ 1,
/* 6095 */ 0,
/* 6096 */ -6089,
/* 6097 */ 2,
/* 6098 */ 0,
/* 6099 */ 1,
/* 6100 */ 50258,
/* 6101 */ 1,
/* 6102 */ 50059,
/* 6103 */ -6098,
/* 6104 */ 1,
/* 6105 */ 6111,
/* 6106 */ 50044,
/* 6107 */ 50258,
/* 6108 */ 1,
/* 6109 */ -6105,
/* 6110 */ 1,
/* 6111 */ 0,
/* 6112 */ -6105,
/* 6113 */ 2,
/* 6114 */ 6119,
/* 6115 */ 1,
/* 6116 */ 50059,
/* 6117 */ -6114,
/* 6118 */ 1,
/* 6119 */ 6129,
/* 6120 */ 50367,
/* 6121 */ 50040,
/* 6122 */ 1,
/* 6123 */ 50041,
/* 6124 */ 1,
/* 6125 */ 50335,
/* 6126 */ 1,
/* 6127 */ -6114,
/* 6128 */ 2,
/* 6129 */ 6137,
/* 6130 */ 50367,
/* 6131 */ 50040,
/* 6132 */ 1,
/* 6133 */ 50041,
/* 6134 */ 1,
/* 6135 */ -6114,
/* 6136 */ 3,
/* 6137 */ 6149,
/* 6138 */ 50353,
/* 6139 */ 50040,
/* 6140 */ 1,
/* 6141 */ 50059,
/* 6142 */ 1,
/* 6143 */ 50059,
/* 6144 */ 1,
/* 6145 */ 50041,
/* 6146 */ 1,
/* 6147 */ -6114,
/* 6148 */ 4,
/* 6149 */ 6160,
/* 6150 */ 50446,
/* 6151 */ 50040,
/* 6152 */ 1,
/* 6153 */ 50041,
/* 6154 */ 50123,
/* 6155 */ 1,
/* 6156 */ 1,
/* 6157 */ 50125,
/* 6158 */ -6114,
/* 6159 */ 5,
/* 6160 */ 6165,
/* 6161 */ 50302,
/* 6162 */ 50059,
/* 6163 */ -6114,
/* 6164 */ 6,
/* 6165 */ 6170,
/* 6166 */ 50314,
/* 6167 */ 50059,
/* 6168 */ -6114,
/* 6169 */ 7,
/* 6170 */ 6180,
/* 6171 */ 50331,
/* 6172 */ 1,
/* 6173 */ 50469,
/* 6174 */ 50040,
/* 6175 */ 1,
/* 6176 */ 50041,
/* 6177 */ 50059,
/* 6178 */ -6114,
/* 6179 */ 8,
/* 6180 */ 6188,
/* 6181 */ 50469,
/* 6182 */ 50040,
/* 6183 */ 1,
/* 6184 */ 50041,
/* 6185 */ 1,
/* 6186 */ -6114,
/* 6187 */ 9,
/* 6188 */ 6192,
/* 6189 */ 1,
/* 6190 */ -6114,
/* 6191 */ 10,
/* 6192 */ 6198,
/* 6193 */ 50425,
/* 6194 */ 1,
/* 6195 */ 50059,
/* 6196 */ -6114,
/* 6197 */ 11,
/* 6198 */ 0,
/* 6199 */ 50425,
/* 6200 */ 50059,
/* 6201 */ -6114,
/* 6202 */ 12,
/* 6203 */ 6211,
/* 6204 */ 50305,
/* 6205 */ 1,
/* 6206 */ 50058,
/* 6207 */ 1,
/* 6208 */ 1,
/* 6209 */ -6203,
/* 6210 */ 1,
/* 6211 */ 0,
/* 6212 */ -6203,
/* 6213 */ 2,
/* 6214 */ 6218,
/* 6215 */ 1,
/* 6216 */ -6214,
/* 6217 */ 1,
/* 6218 */ 0,
/* 6219 */ 1,
/* 6220 */ -6214,
/* 6221 */ 2,
/* 6222 */ 6227,
/* 6223 */ 1,
/* 6224 */ 1,
/* 6225 */ -6222,
/* 6226 */ 1,
/* 6227 */ 0,
/* 6228 */ -6222,
/* 6229 */ 2,
/* 6230 */ 6236,
/* 6231 */ 50324,
/* 6232 */ 50058,
/* 6233 */ 1,
/* 6234 */ -6230,
/* 6235 */ 1,
/* 6236 */ 0,
/* 6237 */ -6230,
/* 6238 */ 2,
/* 6239 */ 6244,
/* 6240 */ 1,
/* 6241 */ 1,
/* 6242 */ -6239,
/* 6243 */ 1,
/* 6244 */ 0,
/* 6245 */ -6239,
/* 6246 */ 2,
/* 6247 */ 0,
/* 6248 */ 50123,
/* 6249 */ 1,
/* 6250 */ 50125,
/* 6251 */ -6247,
/* 6252 */ 1,
/* 6253 */ 6258,
/* 6254 */ 1,
/* 6255 */ 1,
/* 6256 */ -6253,
/* 6257 */ 1,
/* 6258 */ 0,
/* 6259 */ -6253,
/* 6260 */ 2,
/* 6261 */ 0,
/* 6262 */ 50123,
/* 6263 */ 1,
/* 6264 */ 1,
/* 6265 */ 50125,
/* 6266 */ -6261,
/* 6267 */ 1,
/* 6268 */ 6273,
/* 6269 */ 1,
/* 6270 */ 1,
/* 6271 */ -6268,
/* 6272 */ 1,
/* 6273 */ 6279,
/* 6274 */ 50441,
/* 6275 */ 1,
/* 6276 */ 1,
/* 6277 */ -6268,
/* 6278 */ 2,
/* 6279 */ 0,
/* 6280 */ -6268,
/* 6281 */ 3,
/* 6282 */ 6287,
/* 6283 */ 1,
/* 6284 */ 1,
/* 6285 */ -6282,
/* 6286 */ 1,
/* 6287 */ 0,
/* 6288 */ -6282,
/* 6289 */ 2,
/* 6290 */ 6294,
/* 6291 */ 50256,
/* 6292 */ -6290,
/* 6293 */ 1,
/* 6294 */ 6298,
/* 6295 */ 50259,
/* 6296 */ -6290,
/* 6297 */ 2,
/* 6298 */ 6302,
/* 6299 */ 50257,
/* 6300 */ -6290,
/* 6301 */ 3,
/* 6302 */ 6306,
/* 6303 */ 50458,
/* 6304 */ -6290,
/* 6305 */ 4,
/* 6306 */ 6310,
/* 6307 */ 50345,
/* 6308 */ -6290,
/* 6309 */ 5,
/* 6310 */ 6314,
/* 6311 */ 50395,
/* 6312 */ -6290,
/* 6313 */ 6,
/* 6314 */ 6318,
/* 6315 */ 1,
/* 6316 */ -6290,
/* 6317 */ 7,
/* 6318 */ 6324,
/* 6319 */ 50040,
/* 6320 */ 1,
/* 6321 */ 50041,
/* 6322 */ -6290,
/* 6323 */ 8,
/* 6324 */ 6328,
/* 6325 */ 50258,
/* 6326 */ -6290,
/* 6327 */ 9,
/* 6328 */ 6334,
/* 6329 */ 1,
/* 6330 */ 1,
/* 6331 */ 50258,
/* 6332 */ -6290,
/* 6333 */ 10,
/* 6334 */ 6343,
/* 6335 */ 1,
/* 6336 */ 1,
/* 6337 */ 50258,
/* 6338 */ 50040,
/* 6339 */ 1,
/* 6340 */ 50041,
/* 6341 */ -6290,
/* 6342 */ 11,
/* 6343 */ 6350,
/* 6344 */ 50258,
/* 6345 */ 50040,
/* 6346 */ 1,
/* 6347 */ 50041,
/* 6348 */ -6290,
/* 6349 */ 12,
/* 6350 */ 0,
/* 6351 */ 1,
/* 6352 */ 50064,
/* 6353 */ 50258,
/* 6354 */ -6290,
/* 6355 */ 13,
/* 6356 */ 6360,
/* 6357 */ 50046,
/* 6358 */ -6356,
/* 6359 */ 1,
/* 6360 */ 0,
/* 6361 */ 50496,
/* 6362 */ -6356,
/* 6363 */ 2,
/* 6364 */ 6368,
/* 6365 */ 50046,
/* 6366 */ -6364,
/* 6367 */ 1,
/* 6368 */ 0,
/* 6369 */ 50496,
/* 6370 */ -6364,
/* 6371 */ 2,
/* 6372 */ 6376,
/* 6373 */ 50258,
/* 6374 */ -6372,
/* 6375 */ 1,
/* 6376 */ 6382,
/* 6377 */ 1,
/* 6378 */ 50064,
/* 6379 */ 50258,
/* 6380 */ -6372,
/* 6381 */ 2,
/* 6382 */ 0,
/* 6383 */ 1,
/* 6384 */ 1,
/* 6385 */ 50258,
/* 6386 */ -6372,
/* 6387 */ 3,
/* 6388 */ 6392,
/* 6389 */ 50046,
/* 6390 */ -6388,
/* 6391 */ 1,
/* 6392 */ 0,
/* 6393 */ 50496,
/* 6394 */ -6388,
/* 6395 */ 2,
/* 6396 */ 6401,
/* 6397 */ 1,
/* 6398 */ 50475,
/* 6399 */ -6396,
/* 6400 */ 1,
/* 6401 */ 6406,
/* 6402 */ 1,
/* 6403 */ 50476,
/* 6404 */ -6396,
/* 6405 */ 2,
/* 6406 */ 6415,
/* 6407 */ 50387,
/* 6408 */ 50040,
/* 6409 */ 1,
/* 6410 */ 50044,
/* 6411 */ 1,
/* 6412 */ 50041,
/* 6413 */ -6396,
/* 6414 */ 3,
/* 6415 */ 6424,
/* 6416 */ 50384,
/* 6417 */ 50040,
/* 6418 */ 1,
/* 6419 */ 50044,
/* 6420 */ 1,
/* 6421 */ 50041,
/* 6422 */ -6396,
/* 6423 */ 4,
/* 6424 */ 0,
/* 6425 */ 1,
/* 6426 */ -6396,
/* 6427 */ 5,
/* 6428 */ 6433,
/* 6429 */ 50475,
/* 6430 */ 1,
/* 6431 */ -6428,
/* 6432 */ 1,
/* 6433 */ 6438,
/* 6434 */ 50476,
/* 6435 */ 1,
/* 6436 */ -6428,
/* 6437 */ 2,
/* 6438 */ 6443,
/* 6439 */ 50043,
/* 6440 */ 1,
/* 6441 */ -6428,
/* 6442 */ 3,
/* 6443 */ 6448,
/* 6444 */ 50045,
/* 6445 */ 1,
/* 6446 */ -6428,
/* 6447 */ 4,
/* 6448 */ 6453,
/* 6449 */ 50033,
/* 6450 */ 1,
/* 6451 */ -6428,
/* 6452 */ 5,
/* 6453 */ 6458,
/* 6454 */ 50126,
/* 6455 */ 1,
/* 6456 */ -6428,
/* 6457 */ 6,
/* 6458 */ 6464,
/* 6459 */ 50058,
/* 6460 */ 1,
/* 6461 */ 50058,
/* 6462 */ -6428,
/* 6463 */ 7,
/* 6464 */ 0,
/* 6465 */ 1,
/* 6466 */ -6428,
/* 6467 */ 8,
/* 6468 */ 6474,
/* 6469 */ 1,
/* 6470 */ 50100,
/* 6471 */ 1,
/* 6472 */ -6468,
/* 6473 */ 1,
/* 6474 */ 0,
/* 6475 */ 1,
/* 6476 */ -6468,
/* 6477 */ 2,
/* 6478 */ 6485,
/* 6479 */ 50040,
/* 6480 */ 1,
/* 6481 */ 50041,
/* 6482 */ 1,
/* 6483 */ -6478,
/* 6484 */ 1,
/* 6485 */ 0,
/* 6486 */ 1,
/* 6487 */ -6478,
/* 6488 */ 2,
/* 6489 */ 6495,
/* 6490 */ 1,
/* 6491 */ 50042,
/* 6492 */ 1,
/* 6493 */ -6489,
/* 6494 */ 1,
/* 6495 */ 6501,
/* 6496 */ 1,
/* 6497 */ 50047,
/* 6498 */ 1,
/* 6499 */ -6489,
/* 6500 */ 2,
/* 6501 */ 6507,
/* 6502 */ 1,
/* 6503 */ 50037,
/* 6504 */ 1,
/* 6505 */ -6489,
/* 6506 */ 3,
/* 6507 */ 0,
/* 6508 */ 1,
/* 6509 */ -6489,
/* 6510 */ 4,
/* 6511 */ 6517,
/* 6512 */ 1,
/* 6513 */ 50043,
/* 6514 */ 1,
/* 6515 */ -6511,
/* 6516 */ 1,
/* 6517 */ 6523,
/* 6518 */ 1,
/* 6519 */ 50045,
/* 6520 */ 1,
/* 6521 */ -6511,
/* 6522 */ 2,
/* 6523 */ 0,
/* 6524 */ 1,
/* 6525 */ -6511,
/* 6526 */ 3,
/* 6527 */ 6533,
/* 6528 */ 1,
/* 6529 */ 50477,
/* 6530 */ 1,
/* 6531 */ -6527,
/* 6532 */ 1,
/* 6533 */ 6539,
/* 6534 */ 1,
/* 6535 */ 50478,
/* 6536 */ 1,
/* 6537 */ -6527,
/* 6538 */ 2,
/* 6539 */ 0,
/* 6540 */ 1,
/* 6541 */ -6527,
/* 6542 */ 3,
/* 6543 */ 6549,
/* 6544 */ 1,
/* 6545 */ 50060,
/* 6546 */ 1,
/* 6547 */ -6543,
/* 6548 */ 1,
/* 6549 */ 6555,
/* 6550 */ 1,
/* 6551 */ 50062,
/* 6552 */ 1,
/* 6553 */ -6543,
/* 6554 */ 2,
/* 6555 */ 6561,
/* 6556 */ 1,
/* 6557 */ 50479,
/* 6558 */ 1,
/* 6559 */ -6543,
/* 6560 */ 3,
/* 6561 */ 6567,
/* 6562 */ 1,
/* 6563 */ 50480,
/* 6564 */ 1,
/* 6565 */ -6543,
/* 6566 */ 4,
/* 6567 */ 6573,
/* 6568 */ 1,
/* 6569 */ 50481,
/* 6570 */ 1,
/* 6571 */ -6543,
/* 6572 */ 5,
/* 6573 */ 6579,
/* 6574 */ 1,
/* 6575 */ 50482,
/* 6576 */ 1,
/* 6577 */ -6543,
/* 6578 */ 6,
/* 6579 */ 0,
/* 6580 */ 1,
/* 6581 */ -6543,
/* 6582 */ 7,
/* 6583 */ 6589,
/* 6584 */ 1,
/* 6585 */ 50038,
/* 6586 */ 1,
/* 6587 */ -6583,
/* 6588 */ 1,
/* 6589 */ 0,
/* 6590 */ 1,
/* 6591 */ -6583,
/* 6592 */ 2,
/* 6593 */ 6599,
/* 6594 */ 1,
/* 6595 */ 50094,
/* 6596 */ 1,
/* 6597 */ -6593,
/* 6598 */ 1,
/* 6599 */ 0,
/* 6600 */ 1,
/* 6601 */ -6593,
/* 6602 */ 2,
/* 6603 */ 6609,
/* 6604 */ 1,
/* 6605 */ 50124,
/* 6606 */ 1,
/* 6607 */ -6603,
/* 6608 */ 1,
/* 6609 */ 0,
/* 6610 */ 1,
/* 6611 */ -6603,
/* 6612 */ 2,
/* 6613 */ 6619,
/* 6614 */ 1,
/* 6615 */ 50483,
/* 6616 */ 1,
/* 6617 */ -6613,
/* 6618 */ 1,
/* 6619 */ 0,
/* 6620 */ 1,
/* 6621 */ -6613,
/* 6622 */ 2,
/* 6623 */ 6629,
/* 6624 */ 1,
/* 6625 */ 50484,
/* 6626 */ 1,
/* 6627 */ -6623,
/* 6628 */ 1,
/* 6629 */ 0,
/* 6630 */ 1,
/* 6631 */ -6623,
/* 6632 */ 2,
/* 6633 */ 6641,
/* 6634 */ 1,
/* 6635 */ 50063,
/* 6636 */ 1,
/* 6637 */ 50058,
/* 6638 */ 1,
/* 6639 */ -6633,
/* 6640 */ 1,
/* 6641 */ 0,
/* 6642 */ 1,
/* 6643 */ -6633,
/* 6644 */ 2,
/* 6645 */ 6651,
/* 6646 */ 1,
/* 6647 */ 50061,
/* 6648 */ 1,
/* 6649 */ -6645,
/* 6650 */ 1,
/* 6651 */ 6657,
/* 6652 */ 1,
/* 6653 */ 50486,
/* 6654 */ 1,
/* 6655 */ -6645,
/* 6656 */ 2,
/* 6657 */ 6663,
/* 6658 */ 1,
/* 6659 */ 50494,
/* 6660 */ 1,
/* 6661 */ -6645,
/* 6662 */ 3,
/* 6663 */ 6669,
/* 6664 */ 1,
/* 6665 */ 50485,
/* 6666 */ 1,
/* 6667 */ -6645,
/* 6668 */ 4,
/* 6669 */ 6675,
/* 6670 */ 1,
/* 6671 */ 50487,
/* 6672 */ 1,
/* 6673 */ -6645,
/* 6674 */ 5,
/* 6675 */ 6681,
/* 6676 */ 1,
/* 6677 */ 50488,
/* 6678 */ 1,
/* 6679 */ -6645,
/* 6680 */ 6,
/* 6681 */ 6687,
/* 6682 */ 1,
/* 6683 */ 50489,
/* 6684 */ 1,
/* 6685 */ -6645,
/* 6686 */ 7,
/* 6687 */ 6693,
/* 6688 */ 1,
/* 6689 */ 50490,
/* 6690 */ 1,
/* 6691 */ -6645,
/* 6692 */ 8,
/* 6693 */ 6699,
/* 6694 */ 1,
/* 6695 */ 50492,
/* 6696 */ 1,
/* 6697 */ -6645,
/* 6698 */ 9,
/* 6699 */ 6705,
/* 6700 */ 1,
/* 6701 */ 50493,
/* 6702 */ 1,
/* 6703 */ -6645,
/* 6704 */ 10,
/* 6705 */ 6711,
/* 6706 */ 1,
/* 6707 */ 50491,
/* 6708 */ 1,
/* 6709 */ -6645,
/* 6710 */ 11,
/* 6711 */ 0,
/* 6712 */ 1,
/* 6713 */ -6645,
/* 6714 */ 12,
/* 6715 */ 6721,
/* 6716 */ 1,
/* 6717 */ 50044,
/* 6718 */ 1,
/* 6719 */ -6715,
/* 6720 */ 1,
/* 6721 */ 0,
/* 6722 */ 1,
/* 6723 */ -6715,
/* 6724 */ 2,
/* 6725 */ 0,
/* 6726 */ 1,
/* 6727 */ -6725,
/* 6728 */ 1,
/* 6729 */ 6734,
/* 6730 */ 1,
/* 6731 */ 1,
/* 6732 */ -6729,
/* 6733 */ 1,
/* 6734 */ 0,
/* 6735 */ -6729,
/* 6736 */ 2,
/* 6737 */ 6743,
/* 6738 */ 50044,
/* 6739 */ 1,
/* 6740 */ 1,
/* 6741 */ -6737,
/* 6742 */ 1,
/* 6743 */ 0,
/* 6744 */ -6737,
/* 6745 */ 2,
0
};
extern int yycoordinate[];
int yycoordinate[] = {
0,
/* 1 */ 9999,
/* 2 */ 192005,
/* 3 */ 9999,
/* 4 */ 9999,
/* 5 */ 192005,
/* 6 */ 9999,
/* 7 */ 194003,
/* 8 */ 9999,
/* 9 */ 193003,
/* 10 */ 9999,
/* 11 */ 194004,
/* 12 */ 194011,
/* 13 */ 9999,
/* 14 */ 194011,
/* 15 */ 9999,
/* 16 */ 9999,
/* 17 */ 194011,
/* 18 */ 9999,
/* 19 */ 9999,
/* 20 */ 9999,
/* 21 */ 9999,
/* 22 */ 9999,
/* 23 */ 197010,
/* 24 */ 9999,
/* 25 */ 9999,
/* 26 */ 198010,
/* 27 */ 9999,
/* 28 */ 9999,
/* 29 */ 198008,
/* 30 */ 9999,
/* 31 */ 9999,
/* 32 */ 9999,
/* 33 */ 9999,
/* 34 */ 9999,
/* 35 */ 199013,
/* 36 */ 9999,
/* 37 */ 200005,
/* 38 */ 9999,
/* 39 */ 200015,
/* 40 */ 9999,
/* 41 */ 201005,
/* 42 */ 9999,
/* 43 */ 201012,
/* 44 */ 9999,
/* 45 */ 202005,
/* 46 */ 9999,
/* 47 */ 202015,
/* 48 */ 9999,
/* 49 */ 203005,
/* 50 */ 9999,
/* 51 */ 203016,
/* 52 */ 9999,
/* 53 */ 204005,
/* 54 */ 9999,
/* 55 */ 204015,
/* 56 */ 9999,
/* 57 */ 205005,
/* 58 */ 9999,
/* 59 */ 205013,
/* 60 */ 9999,
/* 61 */ 206005,
/* 62 */ 9999,
/* 63 */ 206014,
/* 64 */ 9999,
/* 65 */ 207005,
/* 66 */ 9999,
/* 67 */ 207012,
/* 68 */ 9999,
/* 69 */ 208005,
/* 70 */ 9999,
/* 71 */ 208013,
/* 72 */ 9999,
/* 73 */ 209005,
/* 74 */ 9999,
/* 75 */ 209011,
/* 76 */ 9999,
/* 77 */ 210005,
/* 78 */ 9999,
/* 79 */ 210014,
/* 80 */ 9999,
/* 81 */ 211005,
/* 82 */ 9999,
/* 83 */ 211014,
/* 84 */ 9999,
/* 85 */ 212005,
/* 86 */ 9999,
/* 87 */ 212015,
/* 88 */ 9999,
/* 89 */ 213005,
/* 90 */ 9999,
/* 91 */ 213016,
/* 92 */ 9999,
/* 93 */ 214005,
/* 94 */ 9999,
/* 95 */ 214014,
/* 96 */ 9999,
/* 97 */ 215005,
/* 98 */ 9999,
/* 99 */ 215017,
/* 100 */ 9999,
/* 101 */ 216005,
/* 102 */ 9999,
/* 103 */ 216017,
/* 104 */ 9999,
/* 105 */ 217005,
/* 106 */ 9999,
/* 107 */ 217013,
/* 108 */ 9999,
/* 109 */ 218005,
/* 110 */ 9999,
/* 111 */ 218015,
/* 112 */ 9999,
/* 113 */ 219005,
/* 114 */ 9999,
/* 115 */ 219016,
/* 116 */ 9999,
/* 117 */ 220005,
/* 118 */ 9999,
/* 119 */ 220017,
/* 120 */ 9999,
/* 121 */ 221005,
/* 122 */ 9999,
/* 123 */ 221017,
/* 124 */ 9999,
/* 125 */ 222005,
/* 126 */ 9999,
/* 127 */ 222017,
/* 128 */ 9999,
/* 129 */ 9999,
/* 130 */ 223011,
/* 131 */ 223017,
/* 132 */ 9999,
/* 133 */ 223009,
/* 134 */ 9999,
/* 135 */ 9999,
/* 136 */ 9999,
/* 137 */ 9999,
/* 138 */ 224027,
/* 139 */ 9999,
/* 140 */ 224058,
/* 141 */ 9999,
/* 142 */ 9999,
/* 143 */ 224008,
/* 144 */ 9999,
/* 145 */ 9999,
/* 146 */ 9999,
/* 147 */ 223012,
/* 148 */ 9999,
/* 149 */ 9999,
/* 150 */ 9999,
/* 151 */ 9999,
/* 152 */ 224036,
/* 153 */ 9999,
/* 154 */ 9999,
/* 155 */ 224036,
/* 156 */ 9999,
/* 157 */ 9999,
/* 158 */ 9999,
/* 159 */ 224059,
/* 160 */ 9999,
/* 161 */ 9999,
/* 162 */ 224059,
/* 163 */ 9999,
/* 164 */ 9999,
/* 165 */ 9999,
/* 166 */ 9999,
/* 167 */ 229005,
/* 168 */ 9999,
/* 169 */ 9999,
/* 170 */ 238009,
/* 171 */ 9999,
/* 172 */ 238005,
/* 173 */ 9999,
/* 174 */ 242005,
/* 175 */ 9999,
/* 176 */ 242018,
/* 177 */ 9999,
/* 178 */ 242009,
/* 179 */ 9999,
/* 180 */ 243005,
/* 181 */ 9999,
/* 182 */ 243018,
/* 183 */ 9999,
/* 184 */ 243009,
/* 185 */ 9999,
/* 186 */ 244005,
/* 187 */ 9999,
/* 188 */ 244010,
/* 189 */ 9999,
/* 190 */ 248005,
/* 191 */ 9999,
/* 192 */ 248019,
/* 193 */ 9999,
/* 194 */ 248010,
/* 195 */ 9999,
/* 196 */ 249005,
/* 197 */ 9999,
/* 198 */ 249019,
/* 199 */ 9999,
/* 200 */ 249010,
/* 201 */ 9999,
/* 202 */ 250005,
/* 203 */ 9999,
/* 204 */ 250019,
/* 205 */ 9999,
/* 206 */ 250010,
/* 207 */ 9999,
/* 208 */ 251005,
/* 209 */ 9999,
/* 210 */ 251019,
/* 211 */ 9999,
/* 212 */ 251010,
/* 213 */ 9999,
/* 214 */ 252005,
/* 215 */ 9999,
/* 216 */ 252019,
/* 217 */ 9999,
/* 218 */ 252010,
/* 219 */ 9999,
/* 220 */ 254005,
/* 221 */ 9999,
/* 222 */ 254010,
/* 223 */ 9999,
/* 224 */ 9999,
/* 225 */ 258009,
/* 226 */ 9999,
/* 227 */ 258005,
/* 228 */ 9999,
/* 229 */ 9999,
/* 230 */ 259009,
/* 231 */ 9999,
/* 232 */ 259005,
/* 233 */ 9999,
/* 234 */ 260005,
/* 235 */ 9999,
/* 236 */ 260010,
/* 237 */ 9999,
/* 238 */ 9999,
/* 239 */ 264009,
/* 240 */ 9999,
/* 241 */ 9999,
/* 242 */ 264005,
/* 243 */ 9999,
/* 244 */ 9999,
/* 245 */ 9999,
/* 246 */ 265010,
/* 247 */ 9999,
/* 248 */ 9999,
/* 249 */ 9999,
/* 250 */ 266013,
/* 251 */ 9999,
/* 252 */ 9999,
/* 253 */ 9999,
/* 254 */ 267014,
/* 255 */ 9999,
/* 256 */ 9999,
/* 257 */ 9999,
/* 258 */ 268014,
/* 259 */ 9999,
/* 260 */ 272005,
/* 261 */ 9999,
/* 262 */ 272021,
/* 263 */ 9999,
/* 264 */ 272037,
/* 265 */ 9999,
/* 266 */ 272010,
/* 267 */ 9999,
/* 268 */ 274005,
/* 269 */ 9999,
/* 270 */ 274021,
/* 271 */ 9999,
/* 272 */ 9999,
/* 273 */ 274041,
/* 274 */ 9999,
/* 275 */ 274010,
/* 276 */ 9999,
/* 277 */ 276005,
/* 278 */ 9999,
/* 279 */ 276021,
/* 280 */ 9999,
/* 281 */ 276010,
/* 282 */ 9999,
/* 283 */ 278005,
/* 284 */ 9999,
/* 285 */ 278010,
/* 286 */ 9999,
/* 287 */ 283005,
/* 288 */ 9999,
/* 289 */ 283021,
/* 290 */ 9999,
/* 291 */ 283037,
/* 292 */ 9999,
/* 293 */ 283010,
/* 294 */ 9999,
/* 295 */ 285005,
/* 296 */ 9999,
/* 297 */ 285021,
/* 298 */ 9999,
/* 299 */ 285010,
/* 300 */ 9999,
/* 301 */ 299003,
/* 302 */ 300003,
/* 303 */ 301003,
/* 304 */ 302003,
/* 305 */ 304003,
/* 306 */ 306003,
/* 307 */ 307003,
/* 308 */ 9999,
/* 309 */ 291003,
/* 310 */ 9999,
/* 311 */ 9999,
/* 312 */ 9999,
/* 313 */ 299011,
/* 314 */ 9999,
/* 315 */ 9999,
/* 316 */ 299005,
/* 317 */ 9999,
/* 318 */ 9999,
/* 319 */ 299005,
/* 320 */ 9999,
/* 321 */ 9999,
/* 322 */ 9999,
/* 323 */ 9999,
/* 324 */ 300009,
/* 325 */ 9999,
/* 326 */ 9999,
/* 327 */ 300009,
/* 328 */ 9999,
/* 329 */ 301004,
/* 330 */ 9999,
/* 331 */ 301012,
/* 332 */ 9999,
/* 333 */ 9999,
/* 334 */ 301012,
/* 335 */ 9999,
/* 336 */ 9999,
/* 337 */ 9999,
/* 338 */ 302009,
/* 339 */ 9999,
/* 340 */ 9999,
/* 341 */ 9999,
/* 342 */ 303012,
/* 343 */ 9999,
/* 344 */ 9999,
/* 345 */ 303012,
/* 346 */ 9999,
/* 347 */ 304004,
/* 348 */ 9999,
/* 349 */ 304010,
/* 350 */ 9999,
/* 351 */ 9999,
/* 352 */ 305013,
/* 353 */ 9999,
/* 354 */ 305011,
/* 355 */ 9999,
/* 356 */ 9999,
/* 357 */ 9999,
/* 358 */ 9999,
/* 359 */ 306005,
/* 360 */ 9999,
/* 361 */ 9999,
/* 362 */ 9999,
/* 363 */ 9999,
/* 364 */ 306044,
/* 365 */ 9999,
/* 366 */ 9999,
/* 367 */ 306044,
/* 368 */ 9999,
/* 369 */ 9999,
/* 370 */ 9999,
/* 371 */ 307013,
/* 372 */ 9999,
/* 373 */ 307007,
/* 374 */ 9999,
/* 375 */ 307007,
/* 376 */ 9999,
/* 377 */ 9999,
/* 378 */ 311009,
/* 379 */ 311007,
/* 380 */ 9999,
/* 381 */ 311007,
/* 382 */ 9999,
/* 383 */ 9999,
/* 384 */ 9999,
/* 385 */ 312016,
/* 386 */ 9999,
/* 387 */ 312029,
/* 388 */ 312007,
/* 389 */ 9999,
/* 390 */ 312007,
/* 391 */ 9999,
/* 392 */ 9999,
/* 393 */ 9999,
/* 394 */ 314016,
/* 395 */ 314007,
/* 396 */ 9999,
/* 397 */ 314007,
/* 398 */ 9999,
/* 399 */ 9999,
/* 400 */ 9999,
/* 401 */ 316007,
/* 402 */ 9999,
/* 403 */ 316007,
/* 404 */ 9999,
/* 405 */ 9999,
/* 406 */ 9999,
/* 407 */ 318013,
/* 408 */ 9999,
/* 409 */ 318027,
/* 410 */ 9999,
/* 411 */ 318007,
/* 412 */ 9999,
/* 413 */ 318007,
/* 414 */ 9999,
/* 415 */ 9999,
/* 416 */ 318007,
/* 417 */ 9999,
/* 418 */ 307014,
/* 419 */ 307024,
/* 420 */ 307018,
/* 421 */ 9999,
/* 422 */ 307018,
/* 423 */ 9999,
/* 424 */ 9999,
/* 425 */ 307018,
/* 426 */ 9999,
/* 427 */ 9999,
/* 428 */ 9999,
/* 429 */ 307025,
/* 430 */ 9999,
/* 431 */ 9999,
/* 432 */ 307025,
/* 433 */ 9999,
/* 434 */ 9999,
/* 435 */ 324008,
/* 436 */ 325005,
/* 437 */ 328006,
/* 438 */ 9999,
/* 439 */ 9999,
/* 440 */ 323004,
/* 441 */ 9999,
/* 442 */ 9999,
/* 443 */ 9999,
/* 444 */ 324009,
/* 445 */ 9999,
/* 446 */ 325007,
/* 447 */ 327007,
/* 448 */ 325016,
/* 449 */ 9999,
/* 450 */ 325016,
/* 451 */ 9999,
/* 452 */ 9999,
/* 453 */ 325016,
/* 454 */ 9999,
/* 455 */ 9999,
/* 456 */ 9999,
/* 457 */ 327008,
/* 458 */ 9999,
/* 459 */ 9999,
/* 460 */ 9999,
/* 461 */ 327034,
/* 462 */ 9999,
/* 463 */ 333003,
/* 464 */ 333017,
/* 465 */ 334005,
/* 466 */ 335008,
/* 467 */ 338005,
/* 468 */ 9999,
/* 469 */ 9999,
/* 470 */ 333003,
/* 471 */ 9999,
/* 472 */ 9999,
/* 473 */ 9999,
/* 474 */ 333008,
/* 475 */ 9999,
/* 476 */ 9999,
/* 477 */ 9999,
/* 478 */ 333014,
/* 479 */ 9999,
/* 480 */ 9999,
/* 481 */ 9999,
/* 482 */ 333018,
/* 483 */ 9999,
/* 484 */ 9999,
/* 485 */ 333018,
/* 486 */ 9999,
/* 487 */ 334006,
/* 488 */ 9999,
/* 489 */ 334010,
/* 490 */ 9999,
/* 491 */ 9999,
/* 492 */ 334010,
/* 493 */ 9999,
/* 494 */ 9999,
/* 495 */ 9999,
/* 496 */ 335018,
/* 497 */ 9999,
/* 498 */ 9999,
/* 499 */ 9999,
/* 500 */ 336012,
/* 501 */ 9999,
/* 502 */ 9999,
/* 503 */ 338009,
/* 504 */ 9999,
/* 505 */ 338007,
/* 506 */ 9999,
/* 507 */ 9999,
/* 508 */ 338007,
/* 509 */ 9999,
/* 510 */ 343003,
/* 511 */ 344003,
/* 512 */ 346003,
/* 513 */ 9999,
/* 514 */ 342038,
/* 515 */ 9999,
/* 516 */ 343005,
/* 517 */ 9999,
/* 518 */ 343009,
/* 519 */ 9999,
/* 520 */ 9999,
/* 521 */ 343009,
/* 522 */ 9999,
/* 523 */ 9999,
/* 524 */ 9999,
/* 525 */ 344013,
/* 526 */ 9999,
/* 527 */ 9999,
/* 528 */ 9999,
/* 529 */ 345012,
/* 530 */ 9999,
/* 531 */ 9999,
/* 532 */ 346007,
/* 533 */ 9999,
/* 534 */ 346005,
/* 535 */ 9999,
/* 536 */ 9999,
/* 537 */ 346005,
/* 538 */ 9999,
/* 539 */ 9999,
/* 540 */ 9999,
/* 541 */ 350012,
/* 542 */ 350024,
/* 543 */ 9999,
/* 544 */ 9999,
/* 545 */ 350004,
/* 546 */ 9999,
/* 547 */ 9999,
/* 548 */ 350029,
/* 549 */ 350025,
/* 550 */ 9999,
/* 551 */ 350025,
/* 552 */ 9999,
/* 553 */ 9999,
/* 554 */ 350025,
/* 555 */ 9999,
/* 556 */ 355003,
/* 557 */ 356003,
/* 558 */ 357003,
/* 559 */ 9999,
/* 560 */ 354027,
/* 561 */ 9999,
/* 562 */ 9999,
/* 563 */ 356008,
/* 564 */ 356004,
/* 565 */ 9999,
/* 566 */ 356004,
/* 567 */ 9999,
/* 568 */ 9999,
/* 569 */ 356004,
/* 570 */ 9999,
/* 571 */ 9999,
/* 572 */ 359005,
/* 573 */ 9999,
/* 574 */ 357011,
/* 575 */ 9999,
/* 576 */ 364007,
/* 577 */ 9999,
/* 578 */ 364013,
/* 579 */ 9999,
/* 580 */ 9999,
/* 581 */ 9999,
/* 582 */ 359007,
/* 583 */ 9999,
/* 584 */ 359007,
/* 585 */ 9999,
/* 586 */ 9999,
/* 587 */ 359007,
/* 588 */ 9999,
/* 589 */ 374003,
/* 590 */ 9999,
/* 591 */ 373003,
/* 592 */ 9999,
/* 593 */ 382003,
/* 594 */ 9999,
/* 595 */ 381003,
/* 596 */ 9999,
/* 597 */ 390003,
/* 598 */ 394003,
/* 599 */ 9999,
/* 600 */ 389003,
/* 601 */ 9999,
/* 602 */ 390007,
/* 603 */ 390014,
/* 604 */ 9999,
/* 605 */ 390007,
/* 606 */ 9999,
/* 607 */ 9999,
/* 608 */ 391011,
/* 609 */ 9999,
/* 610 */ 391007,
/* 611 */ 9999,
/* 612 */ 9999,
/* 613 */ 392011,
/* 614 */ 9999,
/* 615 */ 392007,
/* 616 */ 9999,
/* 617 */ 393007,
/* 618 */ 9999,
/* 619 */ 9999,
/* 620 */ 393012,
/* 621 */ 9999,
/* 622 */ 9999,
/* 623 */ 9999,
/* 624 */ 390008,
/* 625 */ 9999,
/* 626 */ 9999,
/* 627 */ 390008,
/* 628 */ 9999,
/* 629 */ 9999,
/* 630 */ 394009,
/* 631 */ 9999,
/* 632 */ 9999,
/* 633 */ 394005,
/* 634 */ 9999,
/* 635 */ 9999,
/* 636 */ 394005,
/* 637 */ 9999,
/* 638 */ 9999,
/* 639 */ 395016,
/* 640 */ 9999,
/* 641 */ 395014,
/* 642 */ 9999,
/* 643 */ 9999,
/* 644 */ 396016,
/* 645 */ 9999,
/* 646 */ 396014,
/* 647 */ 9999,
/* 648 */ 9999,
/* 649 */ 397012,
/* 650 */ 9999,
/* 651 */ 397010,
/* 652 */ 9999,
/* 653 */ 9999,
/* 654 */ 9999,
/* 655 */ 9999,
/* 656 */ 407009,
/* 657 */ 408005,
/* 658 */ 9999,
/* 659 */ 410009,
/* 660 */ 9999,
/* 661 */ 9999,
/* 662 */ 402009,
/* 663 */ 9999,
/* 664 */ 9999,
/* 665 */ 408011,
/* 666 */ 409005,
/* 667 */ 9999,
/* 668 */ 408007,
/* 669 */ 9999,
/* 670 */ 9999,
/* 671 */ 408007,
/* 672 */ 9999,
/* 673 */ 9999,
/* 674 */ 409011,
/* 675 */ 9999,
/* 676 */ 409007,
/* 677 */ 9999,
/* 678 */ 9999,
/* 679 */ 409007,
/* 680 */ 9999,
/* 681 */ 410010,
/* 682 */ 410018,
/* 683 */ 9999,
/* 684 */ 410018,
/* 685 */ 9999,
/* 686 */ 9999,
/* 687 */ 410018,
/* 688 */ 9999,
/* 689 */ 9999,
/* 690 */ 414015,
/* 691 */ 9999,
/* 692 */ 9999,
/* 693 */ 9999,
/* 694 */ 414013,
/* 695 */ 9999,
/* 696 */ 9999,
/* 697 */ 415011,
/* 698 */ 415018,
/* 699 */ 9999,
/* 700 */ 9999,
/* 701 */ 415009,
/* 702 */ 9999,
/* 703 */ 416005,
/* 704 */ 416028,
/* 705 */ 416035,
/* 706 */ 9999,
/* 707 */ 9999,
/* 708 */ 416005,
/* 709 */ 9999,
/* 710 */ 417005,
/* 711 */ 417028,
/* 712 */ 417035,
/* 713 */ 9999,
/* 714 */ 417050,
/* 715 */ 9999,
/* 716 */ 9999,
/* 717 */ 417005,
/* 718 */ 9999,
/* 719 */ 9999,
/* 720 */ 421013,
/* 721 */ 421020,
/* 722 */ 422018,
/* 723 */ 9999,
/* 724 */ 9999,
/* 725 */ 421011,
/* 726 */ 9999,
/* 727 */ 9999,
/* 728 */ 423013,
/* 729 */ 423020,
/* 730 */ 9999,
/* 731 */ 9999,
/* 732 */ 423011,
/* 733 */ 9999,
/* 734 */ 9999,
/* 735 */ 424015,
/* 736 */ 424022,
/* 737 */ 9999,
/* 738 */ 9999,
/* 739 */ 424013,
/* 740 */ 9999,
/* 741 */ 426005,
/* 742 */ 9999,
/* 743 */ 426016,
/* 744 */ 426023,
/* 745 */ 9999,
/* 746 */ 9999,
/* 747 */ 426005,
/* 748 */ 9999,
/* 749 */ 427005,
/* 750 */ 9999,
/* 751 */ 427015,
/* 752 */ 9999,
/* 753 */ 9999,
/* 754 */ 9999,
/* 755 */ 428015,
/* 756 */ 9999,
/* 757 */ 9999,
/* 758 */ 9999,
/* 759 */ 428009,
/* 760 */ 9999,
/* 761 */ 9999,
/* 762 */ 9999,
/* 763 */ 430015,
/* 764 */ 9999,
/* 765 */ 430030,
/* 766 */ 9999,
/* 767 */ 9999,
/* 768 */ 9999,
/* 769 */ 430009,
/* 770 */ 9999,
/* 771 */ 9999,
/* 772 */ 9999,
/* 773 */ 432015,
/* 774 */ 9999,
/* 775 */ 432030,
/* 776 */ 9999,
/* 777 */ 432046,
/* 778 */ 9999,
/* 779 */ 9999,
/* 780 */ 9999,
/* 781 */ 432009,
/* 782 */ 9999,
/* 783 */ 9999,
/* 784 */ 9999,
/* 785 */ 435015,
/* 786 */ 9999,
/* 787 */ 435030,
/* 788 */ 9999,
/* 789 */ 435046,
/* 790 */ 9999,
/* 791 */ 435062,
/* 792 */ 9999,
/* 793 */ 9999,
/* 794 */ 9999,
/* 795 */ 435009,
/* 796 */ 9999,
/* 797 */ 9999,
/* 798 */ 439013,
/* 799 */ 439020,
/* 800 */ 9999,
/* 801 */ 9999,
/* 802 */ 439011,
/* 803 */ 9999,
/* 804 */ 9999,
/* 805 */ 440012,
/* 806 */ 440019,
/* 807 */ 9999,
/* 808 */ 9999,
/* 809 */ 440010,
/* 810 */ 9999,
/* 811 */ 9999,
/* 812 */ 441012,
/* 813 */ 441019,
/* 814 */ 441032,
/* 815 */ 9999,
/* 816 */ 9999,
/* 817 */ 441010,
/* 818 */ 9999,
/* 819 */ 9999,
/* 820 */ 9999,
/* 821 */ 9999,
/* 822 */ 9999,
/* 823 */ 442006,
/* 824 */ 9999,
/* 825 */ 9999,
/* 826 */ 448008,
/* 827 */ 9999,
/* 828 */ 9999,
/* 829 */ 448006,
/* 830 */ 9999,
/* 831 */ 9999,
/* 832 */ 451009,
/* 833 */ 451016,
/* 834 */ 9999,
/* 835 */ 9999,
/* 836 */ 451007,
/* 837 */ 9999,
/* 838 */ 9999,
/* 839 */ 452009,
/* 840 */ 452016,
/* 841 */ 9999,
/* 842 */ 9999,
/* 843 */ 452007,
/* 844 */ 9999,
/* 845 */ 9999,
/* 846 */ 453010,
/* 847 */ 453017,
/* 848 */ 9999,
/* 849 */ 9999,
/* 850 */ 453008,
/* 851 */ 9999,
/* 852 */ 9999,
/* 853 */ 454011,
/* 854 */ 454018,
/* 855 */ 458019,
/* 856 */ 9999,
/* 857 */ 9999,
/* 858 */ 454009,
/* 859 */ 9999,
/* 860 */ 9999,
/* 861 */ 462011,
/* 862 */ 462018,
/* 863 */ 9999,
/* 864 */ 9999,
/* 865 */ 462009,
/* 866 */ 9999,
/* 867 */ 463005,
/* 868 */ 463018,
/* 869 */ 9999,
/* 870 */ 9999,
/* 871 */ 463016,
/* 872 */ 9999,
/* 873 */ 9999,
/* 874 */ 464010,
/* 875 */ 464017,
/* 876 */ 9999,
/* 877 */ 9999,
/* 878 */ 464008,
/* 879 */ 9999,
/* 880 */ 9999,
/* 881 */ 465011,
/* 882 */ 465018,
/* 883 */ 465028,
/* 884 */ 9999,
/* 885 */ 9999,
/* 886 */ 465009,
/* 887 */ 9999,
/* 888 */ 9999,
/* 889 */ 466010,
/* 890 */ 466017,
/* 891 */ 466027,
/* 892 */ 9999,
/* 893 */ 9999,
/* 894 */ 466008,
/* 895 */ 9999,
/* 896 */ 9999,
/* 897 */ 467012,
/* 898 */ 467019,
/* 899 */ 9999,
/* 900 */ 9999,
/* 901 */ 467010,
/* 902 */ 9999,
/* 903 */ 9999,
/* 904 */ 470013,
/* 905 */ 470020,
/* 906 */ 9999,
/* 907 */ 9999,
/* 908 */ 470011,
/* 909 */ 9999,
/* 910 */ 9999,
/* 911 */ 472012,
/* 912 */ 472019,
/* 913 */ 9999,
/* 914 */ 9999,
/* 915 */ 472010,
/* 916 */ 9999,
/* 917 */ 474005,
/* 918 */ 9999,
/* 919 */ 474013,
/* 920 */ 9999,
/* 921 */ 475005,
/* 922 */ 9999,
/* 923 */ 475014,
/* 924 */ 9999,
/* 925 */ 476005,
/* 926 */ 9999,
/* 927 */ 476016,
/* 928 */ 9999,
/* 929 */ 477005,
/* 930 */ 9999,
/* 931 */ 477017,
/* 932 */ 9999,
/* 933 */ 9999,
/* 934 */ 9999,
/* 935 */ 414016,
/* 936 */ 9999,
/* 937 */ 9999,
/* 938 */ 414016,
/* 939 */ 9999,
/* 940 */ 9999,
/* 941 */ 9999,
/* 942 */ 415012,
/* 943 */ 9999,
/* 944 */ 9999,
/* 945 */ 415012,
/* 946 */ 9999,
/* 947 */ 9999,
/* 948 */ 9999,
/* 949 */ 416015,
/* 950 */ 9999,
/* 951 */ 9999,
/* 952 */ 9999,
/* 953 */ 416025,
/* 954 */ 9999,
/* 955 */ 9999,
/* 956 */ 9999,
/* 957 */ 416029,
/* 958 */ 9999,
/* 959 */ 9999,
/* 960 */ 416029,
/* 961 */ 9999,
/* 962 */ 9999,
/* 963 */ 9999,
/* 964 */ 417015,
/* 965 */ 9999,
/* 966 */ 9999,
/* 967 */ 9999,
/* 968 */ 417025,
/* 969 */ 9999,
/* 970 */ 9999,
/* 971 */ 9999,
/* 972 */ 417029,
/* 973 */ 9999,
/* 974 */ 9999,
/* 975 */ 417029,
/* 976 */ 9999,
/* 977 */ 9999,
/* 978 */ 9999,
/* 979 */ 421014,
/* 980 */ 9999,
/* 981 */ 9999,
/* 982 */ 421014,
/* 983 */ 9999,
/* 984 */ 9999,
/* 985 */ 422024,
/* 986 */ 422020,
/* 987 */ 9999,
/* 988 */ 422020,
/* 989 */ 9999,
/* 990 */ 9999,
/* 991 */ 422020,
/* 992 */ 9999,
/* 993 */ 9999,
/* 994 */ 9999,
/* 995 */ 423014,
/* 996 */ 9999,
/* 997 */ 9999,
/* 998 */ 423014,
/* 999 */ 9999,
/* 1000 */ 9999,
/* 1001 */ 9999,
/* 1002 */ 424016,
/* 1003 */ 9999,
/* 1004 */ 9999,
/* 1005 */ 424016,
/* 1006 */ 9999,
/* 1007 */ 9999,
/* 1008 */ 9999,
/* 1009 */ 426006,
/* 1010 */ 9999,
/* 1011 */ 9999,
/* 1012 */ 426006,
/* 1013 */ 9999,
/* 1014 */ 9999,
/* 1015 */ 9999,
/* 1016 */ 426017,
/* 1017 */ 9999,
/* 1018 */ 9999,
/* 1019 */ 426017,
/* 1020 */ 9999,
/* 1021 */ 9999,
/* 1022 */ 9999,
/* 1023 */ 439014,
/* 1024 */ 9999,
/* 1025 */ 9999,
/* 1026 */ 439014,
/* 1027 */ 9999,
/* 1028 */ 439021,
/* 1029 */ 439057,
/* 1030 */ 439026,
/* 1031 */ 9999,
/* 1032 */ 439026,
/* 1033 */ 9999,
/* 1034 */ 9999,
/* 1035 */ 439026,
/* 1036 */ 9999,
/* 1037 */ 9999,
/* 1038 */ 9999,
/* 1039 */ 439058,
/* 1040 */ 9999,
/* 1041 */ 9999,
/* 1042 */ 439058,
/* 1043 */ 9999,
/* 1044 */ 9999,
/* 1045 */ 9999,
/* 1046 */ 440013,
/* 1047 */ 9999,
/* 1048 */ 9999,
/* 1049 */ 440013,
/* 1050 */ 9999,
/* 1051 */ 440020,
/* 1052 */ 440056,
/* 1053 */ 440025,
/* 1054 */ 9999,
/* 1055 */ 440025,
/* 1056 */ 9999,
/* 1057 */ 9999,
/* 1058 */ 440025,
/* 1059 */ 9999,
/* 1060 */ 9999,
/* 1061 */ 9999,
/* 1062 */ 440057,
/* 1063 */ 9999,
/* 1064 */ 9999,
/* 1065 */ 440057,
/* 1066 */ 9999,
/* 1067 */ 9999,
/* 1068 */ 9999,
/* 1069 */ 441013,
/* 1070 */ 9999,
/* 1071 */ 9999,
/* 1072 */ 441013,
/* 1073 */ 9999,
/* 1074 */ 9999,
/* 1075 */ 441037,
/* 1076 */ 441033,
/* 1077 */ 9999,
/* 1078 */ 441033,
/* 1079 */ 9999,
/* 1080 */ 9999,
/* 1081 */ 441033,
/* 1082 */ 9999,
/* 1083 */ 9999,
/* 1084 */ 9999,
/* 1085 */ 451010,
/* 1086 */ 9999,
/* 1087 */ 9999,
/* 1088 */ 451010,
/* 1089 */ 9999,
/* 1090 */ 9999,
/* 1091 */ 9999,
/* 1092 */ 452010,
/* 1093 */ 9999,
/* 1094 */ 9999,
/* 1095 */ 452010,
/* 1096 */ 9999,
/* 1097 */ 9999,
/* 1098 */ 9999,
/* 1099 */ 453011,
/* 1100 */ 9999,
/* 1101 */ 9999,
/* 1102 */ 453011,
/* 1103 */ 9999,
/* 1104 */ 9999,
/* 1105 */ 9999,
/* 1106 */ 454012,
/* 1107 */ 9999,
/* 1108 */ 9999,
/* 1109 */ 454012,
/* 1110 */ 9999,
/* 1111 */ 454019,
/* 1112 */ 9999,
/* 1113 */ 454024,
/* 1114 */ 9999,
/* 1115 */ 454024,
/* 1116 */ 9999,
/* 1117 */ 9999,
/* 1118 */ 454024,
/* 1119 */ 9999,
/* 1120 */ 9999,
/* 1121 */ 9999,
/* 1122 */ 462012,
/* 1123 */ 9999,
/* 1124 */ 9999,
/* 1125 */ 462012,
/* 1126 */ 9999,
/* 1127 */ 462019,
/* 1128 */ 462029,
/* 1129 */ 9999,
/* 1130 */ 462029,
/* 1131 */ 9999,
/* 1132 */ 9999,
/* 1133 */ 462029,
/* 1134 */ 9999,
/* 1135 */ 9999,
/* 1136 */ 463023,
/* 1137 */ 463019,
/* 1138 */ 9999,
/* 1139 */ 463019,
/* 1140 */ 9999,
/* 1141 */ 9999,
/* 1142 */ 463019,
/* 1143 */ 9999,
/* 1144 */ 9999,
/* 1145 */ 9999,
/* 1146 */ 464011,
/* 1147 */ 9999,
/* 1148 */ 9999,
/* 1149 */ 464011,
/* 1150 */ 9999,
/* 1151 */ 9999,
/* 1152 */ 9999,
/* 1153 */ 465012,
/* 1154 */ 9999,
/* 1155 */ 9999,
/* 1156 */ 465012,
/* 1157 */ 9999,
/* 1158 */ 9999,
/* 1159 */ 9999,
/* 1160 */ 465029,
/* 1161 */ 9999,
/* 1162 */ 9999,
/* 1163 */ 465029,
/* 1164 */ 9999,
/* 1165 */ 9999,
/* 1166 */ 9999,
/* 1167 */ 466011,
/* 1168 */ 9999,
/* 1169 */ 9999,
/* 1170 */ 466011,
/* 1171 */ 9999,
/* 1172 */ 9999,
/* 1173 */ 9999,
/* 1174 */ 466028,
/* 1175 */ 9999,
/* 1176 */ 9999,
/* 1177 */ 466028,
/* 1178 */ 9999,
/* 1179 */ 9999,
/* 1180 */ 9999,
/* 1181 */ 467013,
/* 1182 */ 9999,
/* 1183 */ 9999,
/* 1184 */ 467013,
/* 1185 */ 9999,
/* 1186 */ 467021,
/* 1187 */ 469006,
/* 1188 */ 467027,
/* 1189 */ 9999,
/* 1190 */ 467027,
/* 1191 */ 9999,
/* 1192 */ 9999,
/* 1193 */ 467027,
/* 1194 */ 9999,
/* 1195 */ 9999,
/* 1196 */ 9999,
/* 1197 */ 469007,
/* 1198 */ 9999,
/* 1199 */ 9999,
/* 1200 */ 469007,
/* 1201 */ 9999,
/* 1202 */ 9999,
/* 1203 */ 9999,
/* 1204 */ 470014,
/* 1205 */ 9999,
/* 1206 */ 9999,
/* 1207 */ 470014,
/* 1208 */ 9999,
/* 1209 */ 470022,
/* 1210 */ 471006,
/* 1211 */ 470028,
/* 1212 */ 9999,
/* 1213 */ 470028,
/* 1214 */ 9999,
/* 1215 */ 9999,
/* 1216 */ 470028,
/* 1217 */ 9999,
/* 1218 */ 9999,
/* 1219 */ 9999,
/* 1220 */ 471007,
/* 1221 */ 9999,
/* 1222 */ 9999,
/* 1223 */ 471007,
/* 1224 */ 9999,
/* 1225 */ 9999,
/* 1226 */ 9999,
/* 1227 */ 472013,
/* 1228 */ 9999,
/* 1229 */ 9999,
/* 1230 */ 472013,
/* 1231 */ 9999,
/* 1232 */ 472021,
/* 1233 */ 473006,
/* 1234 */ 472027,
/* 1235 */ 9999,
/* 1236 */ 472027,
/* 1237 */ 9999,
/* 1238 */ 9999,
/* 1239 */ 472027,
/* 1240 */ 9999,
/* 1241 */ 9999,
/* 1242 */ 9999,
/* 1243 */ 473007,
/* 1244 */ 9999,
/* 1245 */ 9999,
/* 1246 */ 473007,
/* 1247 */ 9999,
/* 1248 */ 9999,
/* 1249 */ 9999,
/* 1250 */ 481007,
/* 1251 */ 9999,
/* 1252 */ 9999,
/* 1253 */ 9999,
/* 1254 */ 9999,
/* 1255 */ 482010,
/* 1256 */ 9999,
/* 1257 */ 9999,
/* 1258 */ 9999,
/* 1259 */ 9999,
/* 1260 */ 483009,
/* 1261 */ 9999,
/* 1262 */ 9999,
/* 1263 */ 9999,
/* 1264 */ 9999,
/* 1265 */ 484008,
/* 1266 */ 9999,
/* 1267 */ 490003,
/* 1268 */ 9999,
/* 1269 */ 491005,
/* 1270 */ 493003,
/* 1271 */ 495003,
/* 1272 */ 496005,
/* 1273 */ 508003,
/* 1274 */ 9999,
/* 1275 */ 488032,
/* 1276 */ 9999,
/* 1277 */ 520005,
/* 1278 */ 9999,
/* 1279 */ 520020,
/* 1280 */ 522019,
/* 1281 */ 9999,
/* 1282 */ 520010,
/* 1283 */ 9999,
/* 1284 */ 491007,
/* 1285 */ 9999,
/* 1286 */ 491015,
/* 1287 */ 9999,
/* 1288 */ 492009,
/* 1289 */ 9999,
/* 1290 */ 9999,
/* 1291 */ 492014,
/* 1292 */ 9999,
/* 1293 */ 9999,
/* 1294 */ 492014,
/* 1295 */ 9999,
/* 1296 */ 493004,
/* 1297 */ 9999,
/* 1298 */ 493009,
/* 1299 */ 9999,
/* 1300 */ 9999,
/* 1301 */ 493009,
/* 1302 */ 9999,
/* 1303 */ 9999,
/* 1304 */ 9999,
/* 1305 */ 495006,
/* 1306 */ 9999,
/* 1307 */ 9999,
/* 1308 */ 495006,
/* 1309 */ 9999,
/* 1310 */ 9999,
/* 1311 */ 496011,
/* 1312 */ 496040,
/* 1313 */ 9999,
/* 1314 */ 9999,
/* 1315 */ 496007,
/* 1316 */ 9999,
/* 1317 */ 9999,
/* 1318 */ 496007,
/* 1319 */ 9999,
/* 1320 */ 9999,
/* 1321 */ 9999,
/* 1322 */ 496018,
/* 1323 */ 9999,
/* 1324 */ 9999,
/* 1325 */ 9999,
/* 1326 */ 496026,
/* 1327 */ 9999,
/* 1328 */ 9999,
/* 1329 */ 9999,
/* 1330 */ 496037,
/* 1331 */ 9999,
/* 1332 */ 9999,
/* 1333 */ 509005,
/* 1334 */ 510005,
/* 1335 */ 511005,
/* 1336 */ 512005,
/* 1337 */ 508006,
/* 1338 */ 9999,
/* 1339 */ 508006,
/* 1340 */ 9999,
/* 1341 */ 9999,
/* 1342 */ 508006,
/* 1343 */ 9999,
/* 1344 */ 509007,
/* 1345 */ 9999,
/* 1346 */ 509015,
/* 1347 */ 9999,
/* 1348 */ 9999,
/* 1349 */ 509015,
/* 1350 */ 9999,
/* 1351 */ 510006,
/* 1352 */ 9999,
/* 1353 */ 510011,
/* 1354 */ 9999,
/* 1355 */ 9999,
/* 1356 */ 510011,
/* 1357 */ 9999,
/* 1358 */ 9999,
/* 1359 */ 9999,
/* 1360 */ 511008,
/* 1361 */ 9999,
/* 1362 */ 9999,
/* 1363 */ 511008,
/* 1364 */ 9999,
/* 1365 */ 9999,
/* 1366 */ 512011,
/* 1367 */ 512040,
/* 1368 */ 9999,
/* 1369 */ 9999,
/* 1370 */ 512007,
/* 1371 */ 9999,
/* 1372 */ 9999,
/* 1373 */ 512007,
/* 1374 */ 9999,
/* 1375 */ 9999,
/* 1376 */ 9999,
/* 1377 */ 512018,
/* 1378 */ 9999,
/* 1379 */ 9999,
/* 1380 */ 9999,
/* 1381 */ 512026,
/* 1382 */ 9999,
/* 1383 */ 9999,
/* 1384 */ 9999,
/* 1385 */ 512037,
/* 1386 */ 9999,
/* 1387 */ 522021,
/* 1388 */ 9999,
/* 1389 */ 522026,
/* 1390 */ 9999,
/* 1391 */ 9999,
/* 1392 */ 522026,
/* 1393 */ 9999,
/* 1394 */ 531003,
/* 1395 */ 531015,
/* 1396 */ 9999,
/* 1397 */ 531008,
/* 1398 */ 9999,
/* 1399 */ 9999,
/* 1400 */ 532009,
/* 1401 */ 532020,
/* 1402 */ 9999,
/* 1403 */ 532005,
/* 1404 */ 9999,
/* 1405 */ 9999,
/* 1406 */ 9999,
/* 1407 */ 531016,
/* 1408 */ 9999,
/* 1409 */ 9999,
/* 1410 */ 531016,
/* 1411 */ 9999,
/* 1412 */ 9999,
/* 1413 */ 9999,
/* 1414 */ 532021,
/* 1415 */ 9999,
/* 1416 */ 9999,
/* 1417 */ 532021,
/* 1418 */ 9999,
/* 1419 */ 9999,
/* 1420 */ 536017,
/* 1421 */ 536024,
/* 1422 */ 9999,
/* 1423 */ 536011,
/* 1424 */ 9999,
/* 1425 */ 9999,
/* 1426 */ 9999,
/* 1427 */ 536018,
/* 1428 */ 9999,
/* 1429 */ 9999,
/* 1430 */ 536018,
/* 1431 */ 9999,
/* 1432 */ 9999,
/* 1433 */ 9999,
/* 1434 */ 9999,
/* 1435 */ 545009,
/* 1436 */ 9999,
/* 1437 */ 546009,
/* 1438 */ 9999,
/* 1439 */ 9999,
/* 1440 */ 540006,
/* 1441 */ 9999,
/* 1442 */ 546010,
/* 1443 */ 546019,
/* 1444 */ 9999,
/* 1445 */ 546019,
/* 1446 */ 9999,
/* 1447 */ 9999,
/* 1448 */ 546019,
/* 1449 */ 9999,
/* 1450 */ 9999,
/* 1451 */ 550015,
/* 1452 */ 9999,
/* 1453 */ 9999,
/* 1454 */ 9999,
/* 1455 */ 550013,
/* 1456 */ 9999,
/* 1457 */ 9999,
/* 1458 */ 551017,
/* 1459 */ 551024,
/* 1460 */ 9999,
/* 1461 */ 9999,
/* 1462 */ 551015,
/* 1463 */ 9999,
/* 1464 */ 552005,
/* 1465 */ 9999,
/* 1466 */ 552015,
/* 1467 */ 9999,
/* 1468 */ 9999,
/* 1469 */ 553010,
/* 1470 */ 553017,
/* 1471 */ 9999,
/* 1472 */ 9999,
/* 1473 */ 553008,
/* 1474 */ 9999,
/* 1475 */ 9999,
/* 1476 */ 554010,
/* 1477 */ 554017,
/* 1478 */ 9999,
/* 1479 */ 9999,
/* 1480 */ 554008,
/* 1481 */ 9999,
/* 1482 */ 9999,
/* 1483 */ 555010,
/* 1484 */ 555017,
/* 1485 */ 9999,
/* 1486 */ 9999,
/* 1487 */ 555008,
/* 1488 */ 9999,
/* 1489 */ 9999,
/* 1490 */ 556012,
/* 1491 */ 556019,
/* 1492 */ 9999,
/* 1493 */ 9999,
/* 1494 */ 556010,
/* 1495 */ 9999,
/* 1496 */ 9999,
/* 1497 */ 557009,
/* 1498 */ 557016,
/* 1499 */ 9999,
/* 1500 */ 9999,
/* 1501 */ 557007,
/* 1502 */ 9999,
/* 1503 */ 9999,
/* 1504 */ 558011,
/* 1505 */ 558018,
/* 1506 */ 9999,
/* 1507 */ 9999,
/* 1508 */ 558009,
/* 1509 */ 9999,
/* 1510 */ 9999,
/* 1511 */ 559011,
/* 1512 */ 559018,
/* 1513 */ 9999,
/* 1514 */ 9999,
/* 1515 */ 9999,
/* 1516 */ 559009,
/* 1517 */ 9999,
/* 1518 */ 9999,
/* 1519 */ 560011,
/* 1520 */ 9999,
/* 1521 */ 560022,
/* 1522 */ 9999,
/* 1523 */ 9999,
/* 1524 */ 9999,
/* 1525 */ 560009,
/* 1526 */ 9999,
/* 1527 */ 9999,
/* 1528 */ 561011,
/* 1529 */ 561018,
/* 1530 */ 9999,
/* 1531 */ 9999,
/* 1532 */ 561009,
/* 1533 */ 9999,
/* 1534 */ 9999,
/* 1535 */ 562011,
/* 1536 */ 562018,
/* 1537 */ 9999,
/* 1538 */ 9999,
/* 1539 */ 562009,
/* 1540 */ 9999,
/* 1541 */ 9999,
/* 1542 */ 563014,
/* 1543 */ 563021,
/* 1544 */ 9999,
/* 1545 */ 9999,
/* 1546 */ 563012,
/* 1547 */ 9999,
/* 1548 */ 9999,
/* 1549 */ 564010,
/* 1550 */ 564017,
/* 1551 */ 565005,
/* 1552 */ 9999,
/* 1553 */ 9999,
/* 1554 */ 564008,
/* 1555 */ 9999,
/* 1556 */ 9999,
/* 1557 */ 575012,
/* 1558 */ 575019,
/* 1559 */ 9999,
/* 1560 */ 9999,
/* 1561 */ 575010,
/* 1562 */ 9999,
/* 1563 */ 9999,
/* 1564 */ 582010,
/* 1565 */ 582017,
/* 1566 */ 9999,
/* 1567 */ 9999,
/* 1568 */ 582008,
/* 1569 */ 9999,
/* 1570 */ 9999,
/* 1571 */ 583015,
/* 1572 */ 583022,
/* 1573 */ 9999,
/* 1574 */ 9999,
/* 1575 */ 583013,
/* 1576 */ 9999,
/* 1577 */ 9999,
/* 1578 */ 584009,
/* 1579 */ 584016,
/* 1580 */ 9999,
/* 1581 */ 9999,
/* 1582 */ 584007,
/* 1583 */ 9999,
/* 1584 */ 9999,
/* 1585 */ 585014,
/* 1586 */ 585021,
/* 1587 */ 9999,
/* 1588 */ 9999,
/* 1589 */ 585012,
/* 1590 */ 9999,
/* 1591 */ 9999,
/* 1592 */ 586013,
/* 1593 */ 586020,
/* 1594 */ 9999,
/* 1595 */ 9999,
/* 1596 */ 586011,
/* 1597 */ 9999,
/* 1598 */ 9999,
/* 1599 */ 587009,
/* 1600 */ 587016,
/* 1601 */ 9999,
/* 1602 */ 9999,
/* 1603 */ 587007,
/* 1604 */ 9999,
/* 1605 */ 9999,
/* 1606 */ 588015,
/* 1607 */ 588022,
/* 1608 */ 9999,
/* 1609 */ 9999,
/* 1610 */ 588013,
/* 1611 */ 9999,
/* 1612 */ 9999,
/* 1613 */ 589012,
/* 1614 */ 589019,
/* 1615 */ 9999,
/* 1616 */ 9999,
/* 1617 */ 589010,
/* 1618 */ 9999,
/* 1619 */ 9999,
/* 1620 */ 591011,
/* 1621 */ 591018,
/* 1622 */ 9999,
/* 1623 */ 9999,
/* 1624 */ 591009,
/* 1625 */ 9999,
/* 1626 */ 9999,
/* 1627 */ 593011,
/* 1628 */ 593018,
/* 1629 */ 9999,
/* 1630 */ 9999,
/* 1631 */ 593009,
/* 1632 */ 9999,
/* 1633 */ 9999,
/* 1634 */ 594011,
/* 1635 */ 594018,
/* 1636 */ 9999,
/* 1637 */ 9999,
/* 1638 */ 594009,
/* 1639 */ 9999,
/* 1640 */ 9999,
/* 1641 */ 595014,
/* 1642 */ 595021,
/* 1643 */ 9999,
/* 1644 */ 9999,
/* 1645 */ 595012,
/* 1646 */ 9999,
/* 1647 */ 9999,
/* 1648 */ 596016,
/* 1649 */ 596023,
/* 1650 */ 9999,
/* 1651 */ 9999,
/* 1652 */ 596014,
/* 1653 */ 9999,
/* 1654 */ 9999,
/* 1655 */ 597016,
/* 1656 */ 597023,
/* 1657 */ 9999,
/* 1658 */ 9999,
/* 1659 */ 597014,
/* 1660 */ 9999,
/* 1661 */ 9999,
/* 1662 */ 598014,
/* 1663 */ 598021,
/* 1664 */ 9999,
/* 1665 */ 9999,
/* 1666 */ 598012,
/* 1667 */ 9999,
/* 1668 */ 9999,
/* 1669 */ 599013,
/* 1670 */ 599020,
/* 1671 */ 9999,
/* 1672 */ 9999,
/* 1673 */ 599011,
/* 1674 */ 9999,
/* 1675 */ 9999,
/* 1676 */ 600011,
/* 1677 */ 600018,
/* 1678 */ 9999,
/* 1679 */ 9999,
/* 1680 */ 600009,
/* 1681 */ 9999,
/* 1682 */ 9999,
/* 1683 */ 9999,
/* 1684 */ 601017,
/* 1685 */ 601024,
/* 1686 */ 9999,
/* 1687 */ 9999,
/* 1688 */ 601009,
/* 1689 */ 9999,
/* 1690 */ 9999,
/* 1691 */ 602014,
/* 1692 */ 602021,
/* 1693 */ 9999,
/* 1694 */ 9999,
/* 1695 */ 602012,
/* 1696 */ 9999,
/* 1697 */ 9999,
/* 1698 */ 603010,
/* 1699 */ 603017,
/* 1700 */ 9999,
/* 1701 */ 9999,
/* 1702 */ 603008,
/* 1703 */ 9999,
/* 1704 */ 604005,
/* 1705 */ 9999,
/* 1706 */ 604016,
/* 1707 */ 9999,
/* 1708 */ 605005,
/* 1709 */ 9999,
/* 1710 */ 605014,
/* 1711 */ 9999,
/* 1712 */ 606005,
/* 1713 */ 9999,
/* 1714 */ 606017,
/* 1715 */ 9999,
/* 1716 */ 9999,
/* 1717 */ 9999,
/* 1718 */ 550016,
/* 1719 */ 9999,
/* 1720 */ 9999,
/* 1721 */ 550016,
/* 1722 */ 9999,
/* 1723 */ 9999,
/* 1724 */ 9999,
/* 1725 */ 551018,
/* 1726 */ 9999,
/* 1727 */ 9999,
/* 1728 */ 551018,
/* 1729 */ 9999,
/* 1730 */ 9999,
/* 1731 */ 9999,
/* 1732 */ 553011,
/* 1733 */ 9999,
/* 1734 */ 9999,
/* 1735 */ 553011,
/* 1736 */ 9999,
/* 1737 */ 9999,
/* 1738 */ 9999,
/* 1739 */ 554011,
/* 1740 */ 9999,
/* 1741 */ 9999,
/* 1742 */ 554011,
/* 1743 */ 9999,
/* 1744 */ 9999,
/* 1745 */ 9999,
/* 1746 */ 555011,
/* 1747 */ 9999,
/* 1748 */ 9999,
/* 1749 */ 555011,
/* 1750 */ 9999,
/* 1751 */ 9999,
/* 1752 */ 9999,
/* 1753 */ 556013,
/* 1754 */ 9999,
/* 1755 */ 9999,
/* 1756 */ 556013,
/* 1757 */ 9999,
/* 1758 */ 9999,
/* 1759 */ 9999,
/* 1760 */ 557010,
/* 1761 */ 9999,
/* 1762 */ 9999,
/* 1763 */ 557010,
/* 1764 */ 9999,
/* 1765 */ 9999,
/* 1766 */ 9999,
/* 1767 */ 558012,
/* 1768 */ 9999,
/* 1769 */ 9999,
/* 1770 */ 558012,
/* 1771 */ 9999,
/* 1772 */ 9999,
/* 1773 */ 9999,
/* 1774 */ 559012,
/* 1775 */ 9999,
/* 1776 */ 9999,
/* 1777 */ 559012,
/* 1778 */ 9999,
/* 1779 */ 9999,
/* 1780 */ 9999,
/* 1781 */ 560012,
/* 1782 */ 9999,
/* 1783 */ 9999,
/* 1784 */ 560012,
/* 1785 */ 9999,
/* 1786 */ 9999,
/* 1787 */ 9999,
/* 1788 */ 561012,
/* 1789 */ 9999,
/* 1790 */ 9999,
/* 1791 */ 561012,
/* 1792 */ 9999,
/* 1793 */ 9999,
/* 1794 */ 9999,
/* 1795 */ 562012,
/* 1796 */ 9999,
/* 1797 */ 9999,
/* 1798 */ 562012,
/* 1799 */ 9999,
/* 1800 */ 9999,
/* 1801 */ 9999,
/* 1802 */ 563015,
/* 1803 */ 9999,
/* 1804 */ 9999,
/* 1805 */ 563015,
/* 1806 */ 9999,
/* 1807 */ 9999,
/* 1808 */ 9999,
/* 1809 */ 564011,
/* 1810 */ 9999,
/* 1811 */ 9999,
/* 1812 */ 564011,
/* 1813 */ 9999,
/* 1814 */ 565007,
/* 1815 */ 9999,
/* 1816 */ 565012,
/* 1817 */ 9999,
/* 1818 */ 9999,
/* 1819 */ 565012,
/* 1820 */ 9999,
/* 1821 */ 9999,
/* 1822 */ 9999,
/* 1823 */ 575013,
/* 1824 */ 9999,
/* 1825 */ 9999,
/* 1826 */ 575013,
/* 1827 */ 9999,
/* 1828 */ 9999,
/* 1829 */ 9999,
/* 1830 */ 582011,
/* 1831 */ 9999,
/* 1832 */ 9999,
/* 1833 */ 582011,
/* 1834 */ 9999,
/* 1835 */ 9999,
/* 1836 */ 9999,
/* 1837 */ 583016,
/* 1838 */ 9999,
/* 1839 */ 9999,
/* 1840 */ 583016,
/* 1841 */ 9999,
/* 1842 */ 9999,
/* 1843 */ 9999,
/* 1844 */ 584010,
/* 1845 */ 9999,
/* 1846 */ 9999,
/* 1847 */ 584010,
/* 1848 */ 9999,
/* 1849 */ 9999,
/* 1850 */ 9999,
/* 1851 */ 585015,
/* 1852 */ 9999,
/* 1853 */ 9999,
/* 1854 */ 585015,
/* 1855 */ 9999,
/* 1856 */ 9999,
/* 1857 */ 9999,
/* 1858 */ 586014,
/* 1859 */ 9999,
/* 1860 */ 9999,
/* 1861 */ 586014,
/* 1862 */ 9999,
/* 1863 */ 9999,
/* 1864 */ 9999,
/* 1865 */ 587010,
/* 1866 */ 9999,
/* 1867 */ 9999,
/* 1868 */ 587010,
/* 1869 */ 9999,
/* 1870 */ 587017,
/* 1871 */ 587025,
/* 1872 */ 9999,
/* 1873 */ 587025,
/* 1874 */ 9999,
/* 1875 */ 9999,
/* 1876 */ 587025,
/* 1877 */ 9999,
/* 1878 */ 9999,
/* 1879 */ 9999,
/* 1880 */ 588016,
/* 1881 */ 9999,
/* 1882 */ 9999,
/* 1883 */ 588016,
/* 1884 */ 9999,
/* 1885 */ 9999,
/* 1886 */ 9999,
/* 1887 */ 589013,
/* 1888 */ 9999,
/* 1889 */ 9999,
/* 1890 */ 589013,
/* 1891 */ 9999,
/* 1892 */ 589021,
/* 1893 */ 590006,
/* 1894 */ 589027,
/* 1895 */ 9999,
/* 1896 */ 589027,
/* 1897 */ 9999,
/* 1898 */ 9999,
/* 1899 */ 589027,
/* 1900 */ 9999,
/* 1901 */ 9999,
/* 1902 */ 9999,
/* 1903 */ 590007,
/* 1904 */ 9999,
/* 1905 */ 9999,
/* 1906 */ 590007,
/* 1907 */ 9999,
/* 1908 */ 9999,
/* 1909 */ 9999,
/* 1910 */ 591012,
/* 1911 */ 9999,
/* 1912 */ 9999,
/* 1913 */ 591012,
/* 1914 */ 9999,
/* 1915 */ 591020,
/* 1916 */ 592006,
/* 1917 */ 591026,
/* 1918 */ 9999,
/* 1919 */ 591026,
/* 1920 */ 9999,
/* 1921 */ 9999,
/* 1922 */ 591026,
/* 1923 */ 9999,
/* 1924 */ 9999,
/* 1925 */ 9999,
/* 1926 */ 592007,
/* 1927 */ 9999,
/* 1928 */ 9999,
/* 1929 */ 592007,
/* 1930 */ 9999,
/* 1931 */ 9999,
/* 1932 */ 9999,
/* 1933 */ 593012,
/* 1934 */ 9999,
/* 1935 */ 9999,
/* 1936 */ 593012,
/* 1937 */ 9999,
/* 1938 */ 593019,
/* 1939 */ 593029,
/* 1940 */ 9999,
/* 1941 */ 593029,
/* 1942 */ 9999,
/* 1943 */ 9999,
/* 1944 */ 593029,
/* 1945 */ 9999,
/* 1946 */ 9999,
/* 1947 */ 9999,
/* 1948 */ 594012,
/* 1949 */ 9999,
/* 1950 */ 9999,
/* 1951 */ 594012,
/* 1952 */ 9999,
/* 1953 */ 594020,
/* 1954 */ 594030,
/* 1955 */ 594025,
/* 1956 */ 9999,
/* 1957 */ 594025,
/* 1958 */ 9999,
/* 1959 */ 9999,
/* 1960 */ 594025,
/* 1961 */ 9999,
/* 1962 */ 9999,
/* 1963 */ 9999,
/* 1964 */ 594031,
/* 1965 */ 9999,
/* 1966 */ 9999,
/* 1967 */ 9999,
/* 1968 */ 594035,
/* 1969 */ 9999,
/* 1970 */ 9999,
/* 1971 */ 594035,
/* 1972 */ 9999,
/* 1973 */ 9999,
/* 1974 */ 9999,
/* 1975 */ 595015,
/* 1976 */ 9999,
/* 1977 */ 9999,
/* 1978 */ 595015,
/* 1979 */ 9999,
/* 1980 */ 9999,
/* 1981 */ 9999,
/* 1982 */ 596017,
/* 1983 */ 9999,
/* 1984 */ 9999,
/* 1985 */ 596017,
/* 1986 */ 9999,
/* 1987 */ 9999,
/* 1988 */ 9999,
/* 1989 */ 597017,
/* 1990 */ 9999,
/* 1991 */ 9999,
/* 1992 */ 597017,
/* 1993 */ 9999,
/* 1994 */ 9999,
/* 1995 */ 9999,
/* 1996 */ 598015,
/* 1997 */ 9999,
/* 1998 */ 9999,
/* 1999 */ 598015,
/* 2000 */ 9999,
/* 2001 */ 9999,
/* 2002 */ 9999,
/* 2003 */ 599014,
/* 2004 */ 9999,
/* 2005 */ 9999,
/* 2006 */ 599014,
/* 2007 */ 9999,
/* 2008 */ 9999,
/* 2009 */ 9999,
/* 2010 */ 600012,
/* 2011 */ 9999,
/* 2012 */ 9999,
/* 2013 */ 600012,
/* 2014 */ 9999,
/* 2015 */ 9999,
/* 2016 */ 9999,
/* 2017 */ 601018,
/* 2018 */ 9999,
/* 2019 */ 9999,
/* 2020 */ 601018,
/* 2021 */ 9999,
/* 2022 */ 9999,
/* 2023 */ 9999,
/* 2024 */ 602015,
/* 2025 */ 9999,
/* 2026 */ 9999,
/* 2027 */ 602015,
/* 2028 */ 9999,
/* 2029 */ 9999,
/* 2030 */ 9999,
/* 2031 */ 603011,
/* 2032 */ 9999,
/* 2033 */ 9999,
/* 2034 */ 603011,
/* 2035 */ 9999,
/* 2036 */ 610003,
/* 2037 */ 610014,
/* 2038 */ 9999,
/* 2039 */ 610008,
/* 2040 */ 9999,
/* 2041 */ 9999,
/* 2042 */ 614018,
/* 2043 */ 614028,
/* 2044 */ 9999,
/* 2045 */ 614012,
/* 2046 */ 9999,
/* 2047 */ 9999,
/* 2048 */ 9999,
/* 2049 */ 610015,
/* 2050 */ 9999,
/* 2051 */ 9999,
/* 2052 */ 610015,
/* 2053 */ 9999,
/* 2054 */ 9999,
/* 2055 */ 9999,
/* 2056 */ 614029,
/* 2057 */ 9999,
/* 2058 */ 9999,
/* 2059 */ 614029,
/* 2060 */ 9999,
/* 2061 */ 618005,
/* 2062 */ 618018,
/* 2063 */ 9999,
/* 2064 */ 618010,
/* 2065 */ 9999,
/* 2066 */ 9999,
/* 2067 */ 619009,
/* 2068 */ 619021,
/* 2069 */ 9999,
/* 2070 */ 619005,
/* 2071 */ 9999,
/* 2072 */ 9999,
/* 2073 */ 9999,
/* 2074 */ 618019,
/* 2075 */ 9999,
/* 2076 */ 9999,
/* 2077 */ 618019,
/* 2078 */ 9999,
/* 2079 */ 9999,
/* 2080 */ 9999,
/* 2081 */ 619022,
/* 2082 */ 9999,
/* 2083 */ 9999,
/* 2084 */ 619022,
/* 2085 */ 9999,
/* 2086 */ 9999,
/* 2087 */ 9999,
/* 2088 */ 9999,
/* 2089 */ 626007,
/* 2090 */ 9999,
/* 2091 */ 627007,
/* 2092 */ 9999,
/* 2093 */ 9999,
/* 2094 */ 624009,
/* 2095 */ 9999,
/* 2096 */ 627008,
/* 2097 */ 627017,
/* 2098 */ 9999,
/* 2099 */ 627017,
/* 2100 */ 9999,
/* 2101 */ 9999,
/* 2102 */ 627017,
/* 2103 */ 9999,
/* 2104 */ 9999,
/* 2105 */ 631016,
/* 2106 */ 9999,
/* 2107 */ 9999,
/* 2108 */ 9999,
/* 2109 */ 631014,
/* 2110 */ 9999,
/* 2111 */ 9999,
/* 2112 */ 632015,
/* 2113 */ 632022,
/* 2114 */ 9999,
/* 2115 */ 9999,
/* 2116 */ 632013,
/* 2117 */ 9999,
/* 2118 */ 633005,
/* 2119 */ 9999,
/* 2120 */ 633015,
/* 2121 */ 9999,
/* 2122 */ 9999,
/* 2123 */ 634010,
/* 2124 */ 634017,
/* 2125 */ 9999,
/* 2126 */ 9999,
/* 2127 */ 9999,
/* 2128 */ 634008,
/* 2129 */ 9999,
/* 2130 */ 9999,
/* 2131 */ 635015,
/* 2132 */ 635022,
/* 2133 */ 9999,
/* 2134 */ 9999,
/* 2135 */ 635013,
/* 2136 */ 9999,
/* 2137 */ 9999,
/* 2138 */ 636011,
/* 2139 */ 636019,
/* 2140 */ 9999,
/* 2141 */ 9999,
/* 2142 */ 636009,
/* 2143 */ 9999,
/* 2144 */ 637005,
/* 2145 */ 637028,
/* 2146 */ 637035,
/* 2147 */ 9999,
/* 2148 */ 9999,
/* 2149 */ 637005,
/* 2150 */ 9999,
/* 2151 */ 9999,
/* 2152 */ 638010,
/* 2153 */ 638017,
/* 2154 */ 9999,
/* 2155 */ 9999,
/* 2156 */ 638008,
/* 2157 */ 9999,
/* 2158 */ 9999,
/* 2159 */ 639012,
/* 2160 */ 639019,
/* 2161 */ 9999,
/* 2162 */ 9999,
/* 2163 */ 639010,
/* 2164 */ 9999,
/* 2165 */ 640005,
/* 2166 */ 9999,
/* 2167 */ 640014,
/* 2168 */ 9999,
/* 2169 */ 641005,
/* 2170 */ 9999,
/* 2171 */ 641016,
/* 2172 */ 9999,
/* 2173 */ 642005,
/* 2174 */ 9999,
/* 2175 */ 642017,
/* 2176 */ 9999,
/* 2177 */ 9999,
/* 2178 */ 9999,
/* 2179 */ 631017,
/* 2180 */ 9999,
/* 2181 */ 9999,
/* 2182 */ 631017,
/* 2183 */ 9999,
/* 2184 */ 9999,
/* 2185 */ 9999,
/* 2186 */ 632016,
/* 2187 */ 9999,
/* 2188 */ 9999,
/* 2189 */ 632016,
/* 2190 */ 9999,
/* 2191 */ 9999,
/* 2192 */ 9999,
/* 2193 */ 634011,
/* 2194 */ 9999,
/* 2195 */ 9999,
/* 2196 */ 634011,
/* 2197 */ 9999,
/* 2198 */ 9999,
/* 2199 */ 9999,
/* 2200 */ 635016,
/* 2201 */ 9999,
/* 2202 */ 9999,
/* 2203 */ 635016,
/* 2204 */ 9999,
/* 2205 */ 9999,
/* 2206 */ 9999,
/* 2207 */ 636012,
/* 2208 */ 9999,
/* 2209 */ 9999,
/* 2210 */ 636012,
/* 2211 */ 9999,
/* 2212 */ 636020,
/* 2213 */ 636030,
/* 2214 */ 636025,
/* 2215 */ 9999,
/* 2216 */ 636025,
/* 2217 */ 9999,
/* 2218 */ 9999,
/* 2219 */ 636025,
/* 2220 */ 9999,
/* 2221 */ 9999,
/* 2222 */ 9999,
/* 2223 */ 636031,
/* 2224 */ 9999,
/* 2225 */ 9999,
/* 2226 */ 9999,
/* 2227 */ 636037,
/* 2228 */ 9999,
/* 2229 */ 9999,
/* 2230 */ 636037,
/* 2231 */ 9999,
/* 2232 */ 9999,
/* 2233 */ 9999,
/* 2234 */ 637016,
/* 2235 */ 9999,
/* 2236 */ 9999,
/* 2237 */ 9999,
/* 2238 */ 637025,
/* 2239 */ 9999,
/* 2240 */ 9999,
/* 2241 */ 9999,
/* 2242 */ 637029,
/* 2243 */ 9999,
/* 2244 */ 9999,
/* 2245 */ 637029,
/* 2246 */ 9999,
/* 2247 */ 9999,
/* 2248 */ 9999,
/* 2249 */ 638011,
/* 2250 */ 9999,
/* 2251 */ 9999,
/* 2252 */ 638011,
/* 2253 */ 9999,
/* 2254 */ 9999,
/* 2255 */ 9999,
/* 2256 */ 639013,
/* 2257 */ 9999,
/* 2258 */ 9999,
/* 2259 */ 639013,
/* 2260 */ 9999,
/* 2261 */ 646003,
/* 2262 */ 655003,
/* 2263 */ 9999,
/* 2264 */ 9999,
/* 2265 */ 659007,
/* 2266 */ 9999,
/* 2267 */ 661007,
/* 2268 */ 9999,
/* 2269 */ 662003,
/* 2270 */ 9999,
/* 2271 */ 645037,
/* 2272 */ 9999,
/* 2273 */ 646005,
/* 2274 */ 654005,
/* 2275 */ 646010,
/* 2276 */ 9999,
/* 2277 */ 646010,
/* 2278 */ 9999,
/* 2279 */ 9999,
/* 2280 */ 646010,
/* 2281 */ 9999,
/* 2282 */ 9999,
/* 2283 */ 9999,
/* 2284 */ 654006,
/* 2285 */ 9999,
/* 2286 */ 9999,
/* 2287 */ 654006,
/* 2288 */ 9999,
/* 2289 */ 9999,
/* 2290 */ 9999,
/* 2291 */ 655008,
/* 2292 */ 9999,
/* 2293 */ 9999,
/* 2294 */ 9999,
/* 2295 */ 655017,
/* 2296 */ 9999,
/* 2297 */ 9999,
/* 2298 */ 9999,
/* 2299 */ 656012,
/* 2300 */ 9999,
/* 2301 */ 9999,
/* 2302 */ 9999,
/* 2303 */ 657013,
/* 2304 */ 9999,
/* 2305 */ 661008,
/* 2306 */ 661016,
/* 2307 */ 9999,
/* 2308 */ 661016,
/* 2309 */ 9999,
/* 2310 */ 9999,
/* 2311 */ 661016,
/* 2312 */ 9999,
/* 2313 */ 9999,
/* 2314 */ 662009,
/* 2315 */ 9999,
/* 2316 */ 683008,
/* 2317 */ 9999,
/* 2318 */ 662007,
/* 2319 */ 9999,
/* 2320 */ 662007,
/* 2321 */ 9999,
/* 2322 */ 9999,
/* 2323 */ 662007,
/* 2324 */ 9999,
/* 2325 */ 683009,
/* 2326 */ 683017,
/* 2327 */ 9999,
/* 2328 */ 683017,
/* 2329 */ 9999,
/* 2330 */ 9999,
/* 2331 */ 683017,
/* 2332 */ 9999,
/* 2333 */ 9999,
/* 2334 */ 688015,
/* 2335 */ 9999,
/* 2336 */ 9999,
/* 2337 */ 9999,
/* 2338 */ 688013,
/* 2339 */ 9999,
/* 2340 */ 689005,
/* 2341 */ 9999,
/* 2342 */ 9999,
/* 2343 */ 689009,
/* 2344 */ 9999,
/* 2345 */ 9999,
/* 2346 */ 690016,
/* 2347 */ 690023,
/* 2348 */ 9999,
/* 2349 */ 9999,
/* 2350 */ 690014,
/* 2351 */ 9999,
/* 2352 */ 9999,
/* 2353 */ 691010,
/* 2354 */ 691017,
/* 2355 */ 9999,
/* 2356 */ 9999,
/* 2357 */ 691008,
/* 2358 */ 9999,
/* 2359 */ 9999,
/* 2360 */ 692010,
/* 2361 */ 692020,
/* 2362 */ 692027,
/* 2363 */ 692037,
/* 2364 */ 9999,
/* 2365 */ 9999,
/* 2366 */ 692008,
/* 2367 */ 9999,
/* 2368 */ 9999,
/* 2369 */ 693013,
/* 2370 */ 693020,
/* 2371 */ 694007,
/* 2372 */ 9999,
/* 2373 */ 9999,
/* 2374 */ 693011,
/* 2375 */ 9999,
/* 2376 */ 9999,
/* 2377 */ 695010,
/* 2378 */ 695017,
/* 2379 */ 695027,
/* 2380 */ 9999,
/* 2381 */ 9999,
/* 2382 */ 695008,
/* 2383 */ 9999,
/* 2384 */ 9999,
/* 2385 */ 697010,
/* 2386 */ 697017,
/* 2387 */ 9999,
/* 2388 */ 9999,
/* 2389 */ 697008,
/* 2390 */ 9999,
/* 2391 */ 9999,
/* 2392 */ 698010,
/* 2393 */ 698017,
/* 2394 */ 9999,
/* 2395 */ 9999,
/* 2396 */ 698008,
/* 2397 */ 9999,
/* 2398 */ 9999,
/* 2399 */ 699010,
/* 2400 */ 699017,
/* 2401 */ 9999,
/* 2402 */ 9999,
/* 2403 */ 699008,
/* 2404 */ 9999,
/* 2405 */ 9999,
/* 2406 */ 700010,
/* 2407 */ 700017,
/* 2408 */ 9999,
/* 2409 */ 9999,
/* 2410 */ 700008,
/* 2411 */ 9999,
/* 2412 */ 9999,
/* 2413 */ 701010,
/* 2414 */ 701017,
/* 2415 */ 9999,
/* 2416 */ 9999,
/* 2417 */ 701008,
/* 2418 */ 9999,
/* 2419 */ 9999,
/* 2420 */ 702010,
/* 2421 */ 702017,
/* 2422 */ 9999,
/* 2423 */ 9999,
/* 2424 */ 702008,
/* 2425 */ 9999,
/* 2426 */ 9999,
/* 2427 */ 703010,
/* 2428 */ 703017,
/* 2429 */ 9999,
/* 2430 */ 9999,
/* 2431 */ 703008,
/* 2432 */ 9999,
/* 2433 */ 9999,
/* 2434 */ 704010,
/* 2435 */ 704017,
/* 2436 */ 9999,
/* 2437 */ 9999,
/* 2438 */ 704008,
/* 2439 */ 9999,
/* 2440 */ 9999,
/* 2441 */ 705010,
/* 2442 */ 705017,
/* 2443 */ 9999,
/* 2444 */ 9999,
/* 2445 */ 705008,
/* 2446 */ 9999,
/* 2447 */ 9999,
/* 2448 */ 706010,
/* 2449 */ 706017,
/* 2450 */ 9999,
/* 2451 */ 9999,
/* 2452 */ 706008,
/* 2453 */ 9999,
/* 2454 */ 9999,
/* 2455 */ 707010,
/* 2456 */ 707017,
/* 2457 */ 9999,
/* 2458 */ 9999,
/* 2459 */ 707008,
/* 2460 */ 9999,
/* 2461 */ 9999,
/* 2462 */ 708010,
/* 2463 */ 9999,
/* 2464 */ 9999,
/* 2465 */ 9999,
/* 2466 */ 708008,
/* 2467 */ 9999,
/* 2468 */ 9999,
/* 2469 */ 709011,
/* 2470 */ 709018,
/* 2471 */ 9999,
/* 2472 */ 9999,
/* 2473 */ 709009,
/* 2474 */ 9999,
/* 2475 */ 711005,
/* 2476 */ 9999,
/* 2477 */ 711014,
/* 2478 */ 9999,
/* 2479 */ 712005,
/* 2480 */ 9999,
/* 2481 */ 712017,
/* 2482 */ 9999,
/* 2483 */ 713005,
/* 2484 */ 9999,
/* 2485 */ 713016,
/* 2486 */ 9999,
/* 2487 */ 9999,
/* 2488 */ 9999,
/* 2489 */ 688016,
/* 2490 */ 9999,
/* 2491 */ 9999,
/* 2492 */ 688016,
/* 2493 */ 9999,
/* 2494 */ 9999,
/* 2495 */ 9999,
/* 2496 */ 690017,
/* 2497 */ 9999,
/* 2498 */ 9999,
/* 2499 */ 690017,
/* 2500 */ 9999,
/* 2501 */ 9999,
/* 2502 */ 9999,
/* 2503 */ 691011,
/* 2504 */ 9999,
/* 2505 */ 9999,
/* 2506 */ 691011,
/* 2507 */ 9999,
/* 2508 */ 9999,
/* 2509 */ 9999,
/* 2510 */ 692016,
/* 2511 */ 9999,
/* 2512 */ 9999,
/* 2513 */ 692016,
/* 2514 */ 9999,
/* 2515 */ 9999,
/* 2516 */ 9999,
/* 2517 */ 692021,
/* 2518 */ 9999,
/* 2519 */ 9999,
/* 2520 */ 692021,
/* 2521 */ 9999,
/* 2522 */ 9999,
/* 2523 */ 9999,
/* 2524 */ 692038,
/* 2525 */ 9999,
/* 2526 */ 9999,
/* 2527 */ 692038,
/* 2528 */ 9999,
/* 2529 */ 9999,
/* 2530 */ 9999,
/* 2531 */ 693014,
/* 2532 */ 9999,
/* 2533 */ 9999,
/* 2534 */ 693014,
/* 2535 */ 9999,
/* 2536 */ 9999,
/* 2537 */ 694013,
/* 2538 */ 9999,
/* 2539 */ 9999,
/* 2540 */ 694009,
/* 2541 */ 9999,
/* 2542 */ 9999,
/* 2543 */ 694009,
/* 2544 */ 9999,
/* 2545 */ 9999,
/* 2546 */ 9999,
/* 2547 */ 695011,
/* 2548 */ 9999,
/* 2549 */ 9999,
/* 2550 */ 695011,
/* 2551 */ 9999,
/* 2552 */ 9999,
/* 2553 */ 9999,
/* 2554 */ 695035,
/* 2555 */ 9999,
/* 2556 */ 9999,
/* 2557 */ 695035,
/* 2558 */ 9999,
/* 2559 */ 9999,
/* 2560 */ 9999,
/* 2561 */ 697011,
/* 2562 */ 9999,
/* 2563 */ 9999,
/* 2564 */ 697011,
/* 2565 */ 9999,
/* 2566 */ 9999,
/* 2567 */ 9999,
/* 2568 */ 698011,
/* 2569 */ 9999,
/* 2570 */ 9999,
/* 2571 */ 698011,
/* 2572 */ 9999,
/* 2573 */ 9999,
/* 2574 */ 9999,
/* 2575 */ 699011,
/* 2576 */ 9999,
/* 2577 */ 9999,
/* 2578 */ 699011,
/* 2579 */ 9999,
/* 2580 */ 9999,
/* 2581 */ 9999,
/* 2582 */ 700011,
/* 2583 */ 9999,
/* 2584 */ 9999,
/* 2585 */ 700011,
/* 2586 */ 9999,
/* 2587 */ 9999,
/* 2588 */ 9999,
/* 2589 */ 701011,
/* 2590 */ 9999,
/* 2591 */ 9999,
/* 2592 */ 701011,
/* 2593 */ 9999,
/* 2594 */ 9999,
/* 2595 */ 9999,
/* 2596 */ 702011,
/* 2597 */ 9999,
/* 2598 */ 9999,
/* 2599 */ 702011,
/* 2600 */ 9999,
/* 2601 */ 9999,
/* 2602 */ 9999,
/* 2603 */ 703011,
/* 2604 */ 9999,
/* 2605 */ 9999,
/* 2606 */ 703011,
/* 2607 */ 9999,
/* 2608 */ 9999,
/* 2609 */ 9999,
/* 2610 */ 704011,
/* 2611 */ 9999,
/* 2612 */ 9999,
/* 2613 */ 704011,
/* 2614 */ 9999,
/* 2615 */ 9999,
/* 2616 */ 9999,
/* 2617 */ 705011,
/* 2618 */ 9999,
/* 2619 */ 9999,
/* 2620 */ 705011,
/* 2621 */ 9999,
/* 2622 */ 9999,
/* 2623 */ 9999,
/* 2624 */ 706011,
/* 2625 */ 9999,
/* 2626 */ 9999,
/* 2627 */ 706011,
/* 2628 */ 9999,
/* 2629 */ 9999,
/* 2630 */ 9999,
/* 2631 */ 707011,
/* 2632 */ 9999,
/* 2633 */ 9999,
/* 2634 */ 707011,
/* 2635 */ 9999,
/* 2636 */ 9999,
/* 2637 */ 9999,
/* 2638 */ 708011,
/* 2639 */ 9999,
/* 2640 */ 9999,
/* 2641 */ 708011,
/* 2642 */ 9999,
/* 2643 */ 9999,
/* 2644 */ 9999,
/* 2645 */ 709012,
/* 2646 */ 9999,
/* 2647 */ 9999,
/* 2648 */ 709012,
/* 2649 */ 9999,
/* 2650 */ 709019,
/* 2651 */ 710005,
/* 2652 */ 709024,
/* 2653 */ 9999,
/* 2654 */ 709024,
/* 2655 */ 9999,
/* 2656 */ 9999,
/* 2657 */ 709024,
/* 2658 */ 9999,
/* 2659 */ 9999,
/* 2660 */ 9999,
/* 2661 */ 710006,
/* 2662 */ 9999,
/* 2663 */ 9999,
/* 2664 */ 9999,
/* 2665 */ 710012,
/* 2666 */ 9999,
/* 2667 */ 9999,
/* 2668 */ 710012,
/* 2669 */ 9999,
/* 2670 */ 717003,
/* 2671 */ 9999,
/* 2672 */ 716015,
/* 2673 */ 9999,
/* 2674 */ 717004,
/* 2675 */ 717029,
/* 2676 */ 717009,
/* 2677 */ 9999,
/* 2678 */ 717009,
/* 2679 */ 9999,
/* 2680 */ 9999,
/* 2681 */ 717009,
/* 2682 */ 9999,
/* 2683 */ 9999,
/* 2684 */ 9999,
/* 2685 */ 717030,
/* 2686 */ 9999,
/* 2687 */ 9999,
/* 2688 */ 9999,
/* 2689 */ 717036,
/* 2690 */ 9999,
/* 2691 */ 9999,
/* 2692 */ 717036,
/* 2693 */ 9999,
/* 2694 */ 9999,
/* 2695 */ 9999,
/* 2696 */ 723009,
/* 2697 */ 9999,
/* 2698 */ 9999,
/* 2699 */ 9999,
/* 2700 */ 728005,
/* 2701 */ 9999,
/* 2702 */ 733007,
/* 2703 */ 9999,
/* 2704 */ 9999,
/* 2705 */ 726006,
/* 2706 */ 9999,
/* 2707 */ 9999,
/* 2708 */ 728011,
/* 2709 */ 9999,
/* 2710 */ 728007,
/* 2711 */ 9999,
/* 2712 */ 9999,
/* 2713 */ 728007,
/* 2714 */ 9999,
/* 2715 */ 733008,
/* 2716 */ 733017,
/* 2717 */ 9999,
/* 2718 */ 733017,
/* 2719 */ 9999,
/* 2720 */ 9999,
/* 2721 */ 733017,
/* 2722 */ 9999,
/* 2723 */ 9999,
/* 2724 */ 737014,
/* 2725 */ 9999,
/* 2726 */ 9999,
/* 2727 */ 9999,
/* 2728 */ 737012,
/* 2729 */ 9999,
/* 2730 */ 738004,
/* 2731 */ 9999,
/* 2732 */ 9999,
/* 2733 */ 738004,
/* 2734 */ 9999,
/* 2735 */ 9999,
/* 2736 */ 740012,
/* 2737 */ 740019,
/* 2738 */ 9999,
/* 2739 */ 9999,
/* 2740 */ 740010,
/* 2741 */ 9999,
/* 2742 */ 9999,
/* 2743 */ 742013,
/* 2744 */ 742020,
/* 2745 */ 9999,
/* 2746 */ 9999,
/* 2747 */ 742011,
/* 2748 */ 9999,
/* 2749 */ 9999,
/* 2750 */ 743013,
/* 2751 */ 743022,
/* 2752 */ 743029,
/* 2753 */ 9999,
/* 2754 */ 743045,
/* 2755 */ 9999,
/* 2756 */ 9999,
/* 2757 */ 743011,
/* 2758 */ 9999,
/* 2759 */ 9999,
/* 2760 */ 746013,
/* 2761 */ 746022,
/* 2762 */ 746029,
/* 2763 */ 9999,
/* 2764 */ 746045,
/* 2765 */ 9999,
/* 2766 */ 746062,
/* 2767 */ 9999,
/* 2768 */ 9999,
/* 2769 */ 746011,
/* 2770 */ 9999,
/* 2771 */ 9999,
/* 2772 */ 750012,
/* 2773 */ 751007,
/* 2774 */ 9999,
/* 2775 */ 9999,
/* 2776 */ 750010,
/* 2777 */ 9999,
/* 2778 */ 752005,
/* 2779 */ 752015,
/* 2780 */ 9999,
/* 2781 */ 9999,
/* 2782 */ 9999,
/* 2783 */ 752010,
/* 2784 */ 9999,
/* 2785 */ 9999,
/* 2786 */ 758011,
/* 2787 */ 758019,
/* 2788 */ 9999,
/* 2789 */ 9999,
/* 2790 */ 758009,
/* 2791 */ 9999,
/* 2792 */ 759005,
/* 2793 */ 9999,
/* 2794 */ 759013,
/* 2795 */ 9999,
/* 2796 */ 760005,
/* 2797 */ 9999,
/* 2798 */ 760016,
/* 2799 */ 9999,
/* 2800 */ 761005,
/* 2801 */ 9999,
/* 2802 */ 761014,
/* 2803 */ 9999,
/* 2804 */ 762005,
/* 2805 */ 9999,
/* 2806 */ 762017,
/* 2807 */ 9999,
/* 2808 */ 9999,
/* 2809 */ 9999,
/* 2810 */ 737015,
/* 2811 */ 9999,
/* 2812 */ 9999,
/* 2813 */ 737015,
/* 2814 */ 9999,
/* 2815 */ 9999,
/* 2816 */ 738019,
/* 2817 */ 738026,
/* 2818 */ 739003,
/* 2819 */ 738013,
/* 2820 */ 9999,
/* 2821 */ 738013,
/* 2822 */ 9999,
/* 2823 */ 9999,
/* 2824 */ 738013,
/* 2825 */ 9999,
/* 2826 */ 9999,
/* 2827 */ 9999,
/* 2828 */ 738020,
/* 2829 */ 9999,
/* 2830 */ 9999,
/* 2831 */ 738020,
/* 2832 */ 9999,
/* 2833 */ 9999,
/* 2834 */ 9999,
/* 2835 */ 739004,
/* 2836 */ 9999,
/* 2837 */ 9999,
/* 2838 */ 739004,
/* 2839 */ 9999,
/* 2840 */ 9999,
/* 2841 */ 9999,
/* 2842 */ 740013,
/* 2843 */ 9999,
/* 2844 */ 9999,
/* 2845 */ 740013,
/* 2846 */ 9999,
/* 2847 */ 740020,
/* 2848 */ 740054,
/* 2849 */ 740034,
/* 2850 */ 9999,
/* 2851 */ 740034,
/* 2852 */ 9999,
/* 2853 */ 9999,
/* 2854 */ 740034,
/* 2855 */ 9999,
/* 2856 */ 9999,
/* 2857 */ 9999,
/* 2858 */ 740055,
/* 2859 */ 9999,
/* 2860 */ 9999,
/* 2861 */ 740055,
/* 2862 */ 9999,
/* 2863 */ 9999,
/* 2864 */ 9999,
/* 2865 */ 742014,
/* 2866 */ 9999,
/* 2867 */ 9999,
/* 2868 */ 742014,
/* 2869 */ 9999,
/* 2870 */ 9999,
/* 2871 */ 9999,
/* 2872 */ 743018,
/* 2873 */ 9999,
/* 2874 */ 9999,
/* 2875 */ 743018,
/* 2876 */ 9999,
/* 2877 */ 9999,
/* 2878 */ 9999,
/* 2879 */ 743023,
/* 2880 */ 9999,
/* 2881 */ 9999,
/* 2882 */ 743023,
/* 2883 */ 9999,
/* 2884 */ 9999,
/* 2885 */ 9999,
/* 2886 */ 746018,
/* 2887 */ 9999,
/* 2888 */ 9999,
/* 2889 */ 746018,
/* 2890 */ 9999,
/* 2891 */ 9999,
/* 2892 */ 9999,
/* 2893 */ 746023,
/* 2894 */ 9999,
/* 2895 */ 9999,
/* 2896 */ 746023,
/* 2897 */ 9999,
/* 2898 */ 9999,
/* 2899 */ 9999,
/* 2900 */ 750013,
/* 2901 */ 9999,
/* 2902 */ 9999,
/* 2903 */ 750013,
/* 2904 */ 9999,
/* 2905 */ 751009,
/* 2906 */ 751059,
/* 2907 */ 751014,
/* 2908 */ 9999,
/* 2909 */ 751014,
/* 2910 */ 9999,
/* 2911 */ 9999,
/* 2912 */ 751014,
/* 2913 */ 9999,
/* 2914 */ 9999,
/* 2915 */ 9999,
/* 2916 */ 751060,
/* 2917 */ 9999,
/* 2918 */ 9999,
/* 2919 */ 751060,
/* 2920 */ 9999,
/* 2921 */ 9999,
/* 2922 */ 9999,
/* 2923 */ 752016,
/* 2924 */ 9999,
/* 2925 */ 9999,
/* 2926 */ 752016,
/* 2927 */ 9999,
/* 2928 */ 9999,
/* 2929 */ 9999,
/* 2930 */ 758012,
/* 2931 */ 9999,
/* 2932 */ 9999,
/* 2933 */ 758012,
/* 2934 */ 9999,
/* 2935 */ 758020,
/* 2936 */ 758030,
/* 2937 */ 758025,
/* 2938 */ 9999,
/* 2939 */ 758025,
/* 2940 */ 9999,
/* 2941 */ 9999,
/* 2942 */ 758025,
/* 2943 */ 9999,
/* 2944 */ 9999,
/* 2945 */ 9999,
/* 2946 */ 758031,
/* 2947 */ 9999,
/* 2948 */ 9999,
/* 2949 */ 9999,
/* 2950 */ 758037,
/* 2951 */ 9999,
/* 2952 */ 9999,
/* 2953 */ 758037,
/* 2954 */ 9999,
/* 2955 */ 9999,
/* 2956 */ 9999,
/* 2957 */ 9999,
/* 2958 */ 767007,
/* 2959 */ 9999,
/* 2960 */ 9999,
/* 2961 */ 766007,
/* 2962 */ 9999,
/* 2963 */ 767008,
/* 2964 */ 767018,
/* 2965 */ 9999,
/* 2966 */ 767018,
/* 2967 */ 9999,
/* 2968 */ 9999,
/* 2969 */ 767018,
/* 2970 */ 9999,
/* 2971 */ 9999,
/* 2972 */ 771015,
/* 2973 */ 9999,
/* 2974 */ 9999,
/* 2975 */ 9999,
/* 2976 */ 771013,
/* 2977 */ 9999,
/* 2978 */ 9999,
/* 2979 */ 772013,
/* 2980 */ 772020,
/* 2981 */ 772027,
/* 2982 */ 9999,
/* 2983 */ 9999,
/* 2984 */ 772011,
/* 2985 */ 9999,
/* 2986 */ 9999,
/* 2987 */ 773010,
/* 2988 */ 773017,
/* 2989 */ 773024,
/* 2990 */ 9999,
/* 2991 */ 9999,
/* 2992 */ 773008,
/* 2993 */ 9999,
/* 2994 */ 9999,
/* 2995 */ 774009,
/* 2996 */ 774016,
/* 2997 */ 9999,
/* 2998 */ 774030,
/* 2999 */ 9999,
/* 3000 */ 9999,
/* 3001 */ 774007,
/* 3002 */ 9999,
/* 3003 */ 9999,
/* 3004 */ 776012,
/* 3005 */ 777007,
/* 3006 */ 9999,
/* 3007 */ 9999,
/* 3008 */ 776010,
/* 3009 */ 9999,
/* 3010 */ 9999,
/* 3011 */ 778011,
/* 3012 */ 779007,
/* 3013 */ 9999,
/* 3014 */ 9999,
/* 3015 */ 778009,
/* 3016 */ 9999,
/* 3017 */ 9999,
/* 3018 */ 780012,
/* 3019 */ 780019,
/* 3020 */ 9999,
/* 3021 */ 9999,
/* 3022 */ 780010,
/* 3023 */ 9999,
/* 3024 */ 9999,
/* 3025 */ 782012,
/* 3026 */ 782018,
/* 3027 */ 782029,
/* 3028 */ 783007,
/* 3029 */ 783014,
/* 3030 */ 9999,
/* 3031 */ 9999,
/* 3032 */ 782010,
/* 3033 */ 9999,
/* 3034 */ 9999,
/* 3035 */ 784019,
/* 3036 */ 785007,
/* 3037 */ 9999,
/* 3038 */ 9999,
/* 3039 */ 784017,
/* 3040 */ 9999,
/* 3041 */ 9999,
/* 3042 */ 786011,
/* 3043 */ 786019,
/* 3044 */ 9999,
/* 3045 */ 9999,
/* 3046 */ 786009,
/* 3047 */ 9999,
/* 3048 */ 787005,
/* 3049 */ 9999,
/* 3050 */ 787013,
/* 3051 */ 9999,
/* 3052 */ 788005,
/* 3053 */ 9999,
/* 3054 */ 788014,
/* 3055 */ 9999,
/* 3056 */ 789005,
/* 3057 */ 9999,
/* 3058 */ 789016,
/* 3059 */ 9999,
/* 3060 */ 790005,
/* 3061 */ 9999,
/* 3062 */ 790017,
/* 3063 */ 9999,
/* 3064 */ 9999,
/* 3065 */ 9999,
/* 3066 */ 771016,
/* 3067 */ 9999,
/* 3068 */ 9999,
/* 3069 */ 771016,
/* 3070 */ 9999,
/* 3071 */ 9999,
/* 3072 */ 9999,
/* 3073 */ 772014,
/* 3074 */ 9999,
/* 3075 */ 9999,
/* 3076 */ 772014,
/* 3077 */ 9999,
/* 3078 */ 9999,
/* 3079 */ 9999,
/* 3080 */ 772021,
/* 3081 */ 9999,
/* 3082 */ 9999,
/* 3083 */ 772021,
/* 3084 */ 9999,
/* 3085 */ 9999,
/* 3086 */ 9999,
/* 3087 */ 773011,
/* 3088 */ 9999,
/* 3089 */ 9999,
/* 3090 */ 773011,
/* 3091 */ 9999,
/* 3092 */ 9999,
/* 3093 */ 9999,
/* 3094 */ 773018,
/* 3095 */ 9999,
/* 3096 */ 9999,
/* 3097 */ 773018,
/* 3098 */ 9999,
/* 3099 */ 9999,
/* 3100 */ 9999,
/* 3101 */ 774010,
/* 3102 */ 9999,
/* 3103 */ 9999,
/* 3104 */ 774010,
/* 3105 */ 9999,
/* 3106 */ 9999,
/* 3107 */ 9999,
/* 3108 */ 776013,
/* 3109 */ 9999,
/* 3110 */ 9999,
/* 3111 */ 776013,
/* 3112 */ 9999,
/* 3113 */ 777008,
/* 3114 */ 777020,
/* 3115 */ 777069,
/* 3116 */ 777013,
/* 3117 */ 9999,
/* 3118 */ 777013,
/* 3119 */ 9999,
/* 3120 */ 9999,
/* 3121 */ 777013,
/* 3122 */ 9999,
/* 3123 */ 9999,
/* 3124 */ 9999,
/* 3125 */ 777070,
/* 3126 */ 9999,
/* 3127 */ 9999,
/* 3128 */ 777070,
/* 3129 */ 9999,
/* 3130 */ 9999,
/* 3131 */ 9999,
/* 3132 */ 778012,
/* 3133 */ 9999,
/* 3134 */ 9999,
/* 3135 */ 778012,
/* 3136 */ 9999,
/* 3137 */ 779008,
/* 3138 */ 779020,
/* 3139 */ 779067,
/* 3140 */ 779013,
/* 3141 */ 9999,
/* 3142 */ 779013,
/* 3143 */ 9999,
/* 3144 */ 9999,
/* 3145 */ 779013,
/* 3146 */ 9999,
/* 3147 */ 9999,
/* 3148 */ 9999,
/* 3149 */ 779068,
/* 3150 */ 9999,
/* 3151 */ 9999,
/* 3152 */ 779068,
/* 3153 */ 9999,
/* 3154 */ 9999,
/* 3155 */ 9999,
/* 3156 */ 780013,
/* 3157 */ 9999,
/* 3158 */ 9999,
/* 3159 */ 780013,
/* 3160 */ 9999,
/* 3161 */ 780020,
/* 3162 */ 780054,
/* 3163 */ 780034,
/* 3164 */ 9999,
/* 3165 */ 780034,
/* 3166 */ 9999,
/* 3167 */ 9999,
/* 3168 */ 780034,
/* 3169 */ 9999,
/* 3170 */ 9999,
/* 3171 */ 9999,
/* 3172 */ 780055,
/* 3173 */ 9999,
/* 3174 */ 9999,
/* 3175 */ 780055,
/* 3176 */ 9999,
/* 3177 */ 9999,
/* 3178 */ 9999,
/* 3179 */ 782013,
/* 3180 */ 9999,
/* 3181 */ 9999,
/* 3182 */ 9999,
/* 3183 */ 782030,
/* 3184 */ 9999,
/* 3185 */ 9999,
/* 3186 */ 9999,
/* 3187 */ 783008,
/* 3188 */ 9999,
/* 3189 */ 9999,
/* 3190 */ 783008,
/* 3191 */ 9999,
/* 3192 */ 783016,
/* 3193 */ 783068,
/* 3194 */ 783021,
/* 3195 */ 9999,
/* 3196 */ 783021,
/* 3197 */ 9999,
/* 3198 */ 9999,
/* 3199 */ 783021,
/* 3200 */ 9999,
/* 3201 */ 9999,
/* 3202 */ 9999,
/* 3203 */ 783069,
/* 3204 */ 9999,
/* 3205 */ 9999,
/* 3206 */ 783069,
/* 3207 */ 9999,
/* 3208 */ 9999,
/* 3209 */ 9999,
/* 3210 */ 784020,
/* 3211 */ 9999,
/* 3212 */ 9999,
/* 3213 */ 784020,
/* 3214 */ 9999,
/* 3215 */ 785008,
/* 3216 */ 785054,
/* 3217 */ 785013,
/* 3218 */ 9999,
/* 3219 */ 785013,
/* 3220 */ 9999,
/* 3221 */ 9999,
/* 3222 */ 785013,
/* 3223 */ 9999,
/* 3224 */ 9999,
/* 3225 */ 9999,
/* 3226 */ 785055,
/* 3227 */ 9999,
/* 3228 */ 9999,
/* 3229 */ 785055,
/* 3230 */ 9999,
/* 3231 */ 9999,
/* 3232 */ 9999,
/* 3233 */ 786012,
/* 3234 */ 9999,
/* 3235 */ 9999,
/* 3236 */ 786012,
/* 3237 */ 9999,
/* 3238 */ 786020,
/* 3239 */ 786030,
/* 3240 */ 786025,
/* 3241 */ 9999,
/* 3242 */ 786025,
/* 3243 */ 9999,
/* 3244 */ 9999,
/* 3245 */ 786025,
/* 3246 */ 9999,
/* 3247 */ 9999,
/* 3248 */ 9999,
/* 3249 */ 786031,
/* 3250 */ 9999,
/* 3251 */ 9999,
/* 3252 */ 9999,
/* 3253 */ 786037,
/* 3254 */ 9999,
/* 3255 */ 9999,
/* 3256 */ 786037,
/* 3257 */ 9999,
/* 3258 */ 9999,
/* 3259 */ 9999,
/* 3260 */ 794027,
/* 3261 */ 794038,
/* 3262 */ 9999,
/* 3263 */ 9999,
/* 3264 */ 794076,
/* 3265 */ 9999,
/* 3266 */ 794003,
/* 3267 */ 9999,
/* 3268 */ 9999,
/* 3269 */ 9999,
/* 3270 */ 796029,
/* 3271 */ 796040,
/* 3272 */ 9999,
/* 3273 */ 9999,
/* 3274 */ 796078,
/* 3275 */ 9999,
/* 3276 */ 796005,
/* 3277 */ 9999,
/* 3278 */ 9999,
/* 3279 */ 9999,
/* 3280 */ 798029,
/* 3281 */ 798040,
/* 3282 */ 9999,
/* 3283 */ 9999,
/* 3284 */ 798078,
/* 3285 */ 9999,
/* 3286 */ 798005,
/* 3287 */ 9999,
/* 3288 */ 794039,
/* 3289 */ 9999,
/* 3290 */ 794048,
/* 3291 */ 9999,
/* 3292 */ 9999,
/* 3293 */ 794048,
/* 3294 */ 9999,
/* 3295 */ 796041,
/* 3296 */ 9999,
/* 3297 */ 796050,
/* 3298 */ 9999,
/* 3299 */ 9999,
/* 3300 */ 796050,
/* 3301 */ 9999,
/* 3302 */ 798041,
/* 3303 */ 9999,
/* 3304 */ 798050,
/* 3305 */ 9999,
/* 3306 */ 9999,
/* 3307 */ 798050,
/* 3308 */ 9999,
/* 3309 */ 9999,
/* 3310 */ 803007,
/* 3311 */ 9999,
/* 3312 */ 803003,
/* 3313 */ 9999,
/* 3314 */ 9999,
/* 3315 */ 804009,
/* 3316 */ 9999,
/* 3317 */ 804005,
/* 3318 */ 9999,
/* 3319 */ 9999,
/* 3320 */ 805009,
/* 3321 */ 9999,
/* 3322 */ 805024,
/* 3323 */ 9999,
/* 3324 */ 805005,
/* 3325 */ 9999,
/* 3326 */ 809003,
/* 3327 */ 809015,
/* 3328 */ 9999,
/* 3329 */ 809008,
/* 3330 */ 9999,
/* 3331 */ 9999,
/* 3332 */ 811011,
/* 3333 */ 9999,
/* 3334 */ 9999,
/* 3335 */ 811009,
/* 3336 */ 9999,
/* 3337 */ 9999,
/* 3338 */ 9999,
/* 3339 */ 9999,
/* 3340 */ 813009,
/* 3341 */ 9999,
/* 3342 */ 9999,
/* 3343 */ 9999,
/* 3344 */ 9999,
/* 3345 */ 9999,
/* 3346 */ 815034,
/* 3347 */ 815046,
/* 3348 */ 9999,
/* 3349 */ 815009,
/* 3350 */ 9999,
/* 3351 */ 9999,
/* 3352 */ 817011,
/* 3353 */ 9999,
/* 3354 */ 9999,
/* 3355 */ 9999,
/* 3356 */ 817046,
/* 3357 */ 817058,
/* 3358 */ 9999,
/* 3359 */ 817009,
/* 3360 */ 9999,
/* 3361 */ 9999,
/* 3362 */ 9999,
/* 3363 */ 809025,
/* 3364 */ 9999,
/* 3365 */ 9999,
/* 3366 */ 809025,
/* 3367 */ 9999,
/* 3368 */ 9999,
/* 3369 */ 9999,
/* 3370 */ 815056,
/* 3371 */ 9999,
/* 3372 */ 9999,
/* 3373 */ 815056,
/* 3374 */ 9999,
/* 3375 */ 9999,
/* 3376 */ 9999,
/* 3377 */ 817068,
/* 3378 */ 9999,
/* 3379 */ 9999,
/* 3380 */ 817068,
/* 3381 */ 9999,
/* 3382 */ 9999,
/* 3383 */ 9999,
/* 3384 */ 9999,
/* 3385 */ 823007,
/* 3386 */ 9999,
/* 3387 */ 9999,
/* 3388 */ 822009,
/* 3389 */ 9999,
/* 3390 */ 823008,
/* 3391 */ 823016,
/* 3392 */ 9999,
/* 3393 */ 823016,
/* 3394 */ 9999,
/* 3395 */ 9999,
/* 3396 */ 823016,
/* 3397 */ 9999,
/* 3398 */ 9999,
/* 3399 */ 827015,
/* 3400 */ 9999,
/* 3401 */ 9999,
/* 3402 */ 9999,
/* 3403 */ 827013,
/* 3404 */ 9999,
/* 3405 */ 828005,
/* 3406 */ 9999,
/* 3407 */ 828016,
/* 3408 */ 9999,
/* 3409 */ 829005,
/* 3410 */ 9999,
/* 3411 */ 829014,
/* 3412 */ 9999,
/* 3413 */ 830005,
/* 3414 */ 9999,
/* 3415 */ 830017,
/* 3416 */ 9999,
/* 3417 */ 9999,
/* 3418 */ 9999,
/* 3419 */ 827016,
/* 3420 */ 9999,
/* 3421 */ 9999,
/* 3422 */ 827016,
/* 3423 */ 9999,
/* 3424 */ 9999,
/* 3425 */ 834013,
/* 3426 */ 835005,
/* 3427 */ 9999,
/* 3428 */ 9999,
/* 3429 */ 834011,
/* 3430 */ 9999,
/* 3431 */ 9999,
/* 3432 */ 838011,
/* 3433 */ 839005,
/* 3434 */ 9999,
/* 3435 */ 838009,
/* 3436 */ 9999,
/* 3437 */ 9999,
/* 3438 */ 846014,
/* 3439 */ 847005,
/* 3440 */ 9999,
/* 3441 */ 9999,
/* 3442 */ 846012,
/* 3443 */ 9999,
/* 3444 */ 9999,
/* 3445 */ 9999,
/* 3446 */ 834014,
/* 3447 */ 9999,
/* 3448 */ 9999,
/* 3449 */ 834014,
/* 3450 */ 9999,
/* 3451 */ 9999,
/* 3452 */ 835011,
/* 3453 */ 835023,
/* 3454 */ 835077,
/* 3455 */ 835007,
/* 3456 */ 9999,
/* 3457 */ 835007,
/* 3458 */ 9999,
/* 3459 */ 9999,
/* 3460 */ 836011,
/* 3461 */ 836023,
/* 3462 */ 836076,
/* 3463 */ 836007,
/* 3464 */ 9999,
/* 3465 */ 836007,
/* 3466 */ 9999,
/* 3467 */ 9999,
/* 3468 */ 836007,
/* 3469 */ 9999,
/* 3470 */ 9999,
/* 3471 */ 9999,
/* 3472 */ 835078,
/* 3473 */ 9999,
/* 3474 */ 9999,
/* 3475 */ 835078,
/* 3476 */ 9999,
/* 3477 */ 9999,
/* 3478 */ 9999,
/* 3479 */ 836077,
/* 3480 */ 9999,
/* 3481 */ 9999,
/* 3482 */ 836077,
/* 3483 */ 9999,
/* 3484 */ 9999,
/* 3485 */ 9999,
/* 3486 */ 838012,
/* 3487 */ 9999,
/* 3488 */ 9999,
/* 3489 */ 838012,
/* 3490 */ 9999,
/* 3491 */ 9999,
/* 3492 */ 839011,
/* 3493 */ 839044,
/* 3494 */ 839058,
/* 3495 */ 839007,
/* 3496 */ 9999,
/* 3497 */ 839007,
/* 3498 */ 9999,
/* 3499 */ 9999,
/* 3500 */ 839007,
/* 3501 */ 9999,
/* 3502 */ 839045,
/* 3503 */ 839054,
/* 3504 */ 9999,
/* 3505 */ 839054,
/* 3506 */ 9999,
/* 3507 */ 9999,
/* 3508 */ 839054,
/* 3509 */ 9999,
/* 3510 */ 9999,
/* 3511 */ 9999,
/* 3512 */ 839059,
/* 3513 */ 9999,
/* 3514 */ 9999,
/* 3515 */ 9999,
/* 3516 */ 839063,
/* 3517 */ 9999,
/* 3518 */ 9999,
/* 3519 */ 9999,
/* 3520 */ 846015,
/* 3521 */ 9999,
/* 3522 */ 9999,
/* 3523 */ 846015,
/* 3524 */ 9999,
/* 3525 */ 9999,
/* 3526 */ 847035,
/* 3527 */ 848007,
/* 3528 */ 851011,
/* 3529 */ 847007,
/* 3530 */ 9999,
/* 3531 */ 847007,
/* 3532 */ 9999,
/* 3533 */ 9999,
/* 3534 */ 847007,
/* 3535 */ 9999,
/* 3536 */ 847037,
/* 3537 */ 9999,
/* 3538 */ 9999,
/* 3539 */ 847042,
/* 3540 */ 9999,
/* 3541 */ 9999,
/* 3542 */ 847042,
/* 3543 */ 9999,
/* 3544 */ 848011,
/* 3545 */ 9999,
/* 3546 */ 9999,
/* 3547 */ 848037,
/* 3548 */ 9999,
/* 3549 */ 848017,
/* 3550 */ 9999,
/* 3551 */ 849011,
/* 3552 */ 9999,
/* 3553 */ 849026,
/* 3554 */ 9999,
/* 3555 */ 9999,
/* 3556 */ 849017,
/* 3557 */ 9999,
/* 3558 */ 850011,
/* 3559 */ 9999,
/* 3560 */ 9999,
/* 3561 */ 850037,
/* 3562 */ 9999,
/* 3563 */ 850017,
/* 3564 */ 9999,
/* 3565 */ 9999,
/* 3566 */ 9999,
/* 3567 */ 851012,
/* 3568 */ 9999,
/* 3569 */ 9999,
/* 3570 */ 851012,
/* 3571 */ 9999,
/* 3572 */ 855005,
/* 3573 */ 9999,
/* 3574 */ 855011,
/* 3575 */ 9999,
/* 3576 */ 856005,
/* 3577 */ 9999,
/* 3578 */ 856010,
/* 3579 */ 9999,
/* 3580 */ 9999,
/* 3581 */ 9999,
/* 3582 */ 857011,
/* 3583 */ 9999,
/* 3584 */ 9999,
/* 3585 */ 9999,
/* 3586 */ 9999,
/* 3587 */ 862007,
/* 3588 */ 9999,
/* 3589 */ 9999,
/* 3590 */ 861010,
/* 3591 */ 9999,
/* 3592 */ 862008,
/* 3593 */ 862017,
/* 3594 */ 9999,
/* 3595 */ 862017,
/* 3596 */ 9999,
/* 3597 */ 9999,
/* 3598 */ 862017,
/* 3599 */ 9999,
/* 3600 */ 9999,
/* 3601 */ 866015,
/* 3602 */ 9999,
/* 3603 */ 9999,
/* 3604 */ 9999,
/* 3605 */ 866013,
/* 3606 */ 9999,
/* 3607 */ 9999,
/* 3608 */ 867010,
/* 3609 */ 867017,
/* 3610 */ 9999,
/* 3611 */ 9999,
/* 3612 */ 867008,
/* 3613 */ 9999,
/* 3614 */ 868005,
/* 3615 */ 9999,
/* 3616 */ 9999,
/* 3617 */ 868005,
/* 3618 */ 9999,
/* 3619 */ 9999,
/* 3620 */ 869013,
/* 3621 */ 869020,
/* 3622 */ 9999,
/* 3623 */ 9999,
/* 3624 */ 869011,
/* 3625 */ 9999,
/* 3626 */ 9999,
/* 3627 */ 870014,
/* 3628 */ 870021,
/* 3629 */ 9999,
/* 3630 */ 9999,
/* 3631 */ 870012,
/* 3632 */ 9999,
/* 3633 */ 9999,
/* 3634 */ 871015,
/* 3635 */ 871022,
/* 3636 */ 9999,
/* 3637 */ 9999,
/* 3638 */ 871013,
/* 3639 */ 9999,
/* 3640 */ 9999,
/* 3641 */ 9999,
/* 3642 */ 873049,
/* 3643 */ 9999,
/* 3644 */ 9999,
/* 3645 */ 873009,
/* 3646 */ 9999,
/* 3647 */ 9999,
/* 3648 */ 9999,
/* 3649 */ 874049,
/* 3650 */ 9999,
/* 3651 */ 9999,
/* 3652 */ 874009,
/* 3653 */ 9999,
/* 3654 */ 9999,
/* 3655 */ 9999,
/* 3656 */ 875049,
/* 3657 */ 9999,
/* 3658 */ 9999,
/* 3659 */ 875007,
/* 3660 */ 9999,
/* 3661 */ 9999,
/* 3662 */ 9999,
/* 3663 */ 876049,
/* 3664 */ 9999,
/* 3665 */ 9999,
/* 3666 */ 876010,
/* 3667 */ 9999,
/* 3668 */ 877005,
/* 3669 */ 9999,
/* 3670 */ 877014,
/* 3671 */ 9999,
/* 3672 */ 878005,
/* 3673 */ 9999,
/* 3674 */ 878017,
/* 3675 */ 9999,
/* 3676 */ 9999,
/* 3677 */ 9999,
/* 3678 */ 866016,
/* 3679 */ 9999,
/* 3680 */ 9999,
/* 3681 */ 866016,
/* 3682 */ 9999,
/* 3683 */ 9999,
/* 3684 */ 9999,
/* 3685 */ 867011,
/* 3686 */ 9999,
/* 3687 */ 9999,
/* 3688 */ 867011,
/* 3689 */ 9999,
/* 3690 */ 9999,
/* 3691 */ 868020,
/* 3692 */ 868027,
/* 3693 */ 868076,
/* 3694 */ 868014,
/* 3695 */ 9999,
/* 3696 */ 868014,
/* 3697 */ 9999,
/* 3698 */ 9999,
/* 3699 */ 868014,
/* 3700 */ 9999,
/* 3701 */ 9999,
/* 3702 */ 9999,
/* 3703 */ 868021,
/* 3704 */ 9999,
/* 3705 */ 9999,
/* 3706 */ 868021,
/* 3707 */ 9999,
/* 3708 */ 9999,
/* 3709 */ 9999,
/* 3710 */ 868077,
/* 3711 */ 9999,
/* 3712 */ 9999,
/* 3713 */ 868077,
/* 3714 */ 9999,
/* 3715 */ 9999,
/* 3716 */ 9999,
/* 3717 */ 869014,
/* 3718 */ 9999,
/* 3719 */ 9999,
/* 3720 */ 869014,
/* 3721 */ 9999,
/* 3722 */ 869021,
/* 3723 */ 869062,
/* 3724 */ 869026,
/* 3725 */ 9999,
/* 3726 */ 869026,
/* 3727 */ 9999,
/* 3728 */ 9999,
/* 3729 */ 869026,
/* 3730 */ 9999,
/* 3731 */ 9999,
/* 3732 */ 9999,
/* 3733 */ 869063,
/* 3734 */ 9999,
/* 3735 */ 9999,
/* 3736 */ 869063,
/* 3737 */ 9999,
/* 3738 */ 9999,
/* 3739 */ 9999,
/* 3740 */ 870015,
/* 3741 */ 9999,
/* 3742 */ 9999,
/* 3743 */ 870015,
/* 3744 */ 9999,
/* 3745 */ 9999,
/* 3746 */ 870074,
/* 3747 */ 870030,
/* 3748 */ 9999,
/* 3749 */ 870030,
/* 3750 */ 9999,
/* 3751 */ 9999,
/* 3752 */ 870030,
/* 3753 */ 9999,
/* 3754 */ 9999,
/* 3755 */ 9999,
/* 3756 */ 870075,
/* 3757 */ 9999,
/* 3758 */ 9999,
/* 3759 */ 870075,
/* 3760 */ 9999,
/* 3761 */ 9999,
/* 3762 */ 9999,
/* 3763 */ 871016,
/* 3764 */ 9999,
/* 3765 */ 9999,
/* 3766 */ 871016,
/* 3767 */ 9999,
/* 3768 */ 871023,
/* 3769 */ 871064,
/* 3770 */ 871028,
/* 3771 */ 9999,
/* 3772 */ 871028,
/* 3773 */ 9999,
/* 3774 */ 9999,
/* 3775 */ 871028,
/* 3776 */ 9999,
/* 3777 */ 9999,
/* 3778 */ 9999,
/* 3779 */ 871065,
/* 3780 */ 9999,
/* 3781 */ 9999,
/* 3782 */ 871065,
/* 3783 */ 9999,
/* 3784 */ 873050,
/* 3785 */ 873060,
/* 3786 */ 873058,
/* 3787 */ 9999,
/* 3788 */ 873058,
/* 3789 */ 9999,
/* 3790 */ 9999,
/* 3791 */ 873058,
/* 3792 */ 9999,
/* 3793 */ 9999,
/* 3794 */ 9999,
/* 3795 */ 873061,
/* 3796 */ 9999,
/* 3797 */ 874050,
/* 3798 */ 874060,
/* 3799 */ 874058,
/* 3800 */ 9999,
/* 3801 */ 874058,
/* 3802 */ 9999,
/* 3803 */ 9999,
/* 3804 */ 874058,
/* 3805 */ 9999,
/* 3806 */ 9999,
/* 3807 */ 9999,
/* 3808 */ 874061,
/* 3809 */ 9999,
/* 3810 */ 875050,
/* 3811 */ 875060,
/* 3812 */ 875058,
/* 3813 */ 9999,
/* 3814 */ 875058,
/* 3815 */ 9999,
/* 3816 */ 9999,
/* 3817 */ 875058,
/* 3818 */ 9999,
/* 3819 */ 9999,
/* 3820 */ 9999,
/* 3821 */ 875061,
/* 3822 */ 9999,
/* 3823 */ 876050,
/* 3824 */ 876060,
/* 3825 */ 876058,
/* 3826 */ 9999,
/* 3827 */ 876058,
/* 3828 */ 9999,
/* 3829 */ 9999,
/* 3830 */ 876058,
/* 3831 */ 9999,
/* 3832 */ 9999,
/* 3833 */ 9999,
/* 3834 */ 876061,
/* 3835 */ 9999,
/* 3836 */ 9999,
/* 3837 */ 9999,
/* 3838 */ 882015,
/* 3839 */ 9999,
/* 3840 */ 882030,
/* 3841 */ 9999,
/* 3842 */ 9999,
/* 3843 */ 882009,
/* 3844 */ 9999,
/* 3845 */ 9999,
/* 3846 */ 9999,
/* 3847 */ 884015,
/* 3848 */ 9999,
/* 3849 */ 9999,
/* 3850 */ 884009,
/* 3851 */ 9999,
/* 3852 */ 886005,
/* 3853 */ 886017,
/* 3854 */ 9999,
/* 3855 */ 886011,
/* 3856 */ 9999,
/* 3857 */ 888005,
/* 3858 */ 9999,
/* 3859 */ 888023,
/* 3860 */ 9999,
/* 3861 */ 9999,
/* 3862 */ 888011,
/* 3863 */ 9999,
/* 3864 */ 890005,
/* 3865 */ 890017,
/* 3866 */ 890027,
/* 3867 */ 9999,
/* 3868 */ 9999,
/* 3869 */ 890011,
/* 3870 */ 9999,
/* 3871 */ 893005,
/* 3872 */ 893017,
/* 3873 */ 893027,
/* 3874 */ 9999,
/* 3875 */ 9999,
/* 3876 */ 893011,
/* 3877 */ 9999,
/* 3878 */ 9999,
/* 3879 */ 896012,
/* 3880 */ 9999,
/* 3881 */ 896010,
/* 3882 */ 9999,
/* 3883 */ 9999,
/* 3884 */ 898010,
/* 3885 */ 9999,
/* 3886 */ 898008,
/* 3887 */ 9999,
/* 3888 */ 9999,
/* 3889 */ 9999,
/* 3890 */ 9999,
/* 3891 */ 886019,
/* 3892 */ 9999,
/* 3893 */ 9999,
/* 3894 */ 886019,
/* 3895 */ 9999,
/* 3896 */ 9999,
/* 3897 */ 9999,
/* 3898 */ 890028,
/* 3899 */ 9999,
/* 3900 */ 9999,
/* 3901 */ 9999,
/* 3902 */ 890034,
/* 3903 */ 9999,
/* 3904 */ 9999,
/* 3905 */ 9999,
/* 3906 */ 893028,
/* 3907 */ 9999,
/* 3908 */ 9999,
/* 3909 */ 9999,
/* 3910 */ 893034,
/* 3911 */ 9999,
/* 3912 */ 9999,
/* 3913 */ 9999,
/* 3914 */ 9999,
/* 3915 */ 902038,
/* 3916 */ 9999,
/* 3917 */ 904007,
/* 3918 */ 9999,
/* 3919 */ 9999,
/* 3920 */ 902018,
/* 3921 */ 9999,
/* 3922 */ 904008,
/* 3923 */ 904019,
/* 3924 */ 9999,
/* 3925 */ 904019,
/* 3926 */ 9999,
/* 3927 */ 9999,
/* 3928 */ 904019,
/* 3929 */ 9999,
/* 3930 */ 9999,
/* 3931 */ 910010,
/* 3932 */ 910017,
/* 3933 */ 9999,
/* 3934 */ 9999,
/* 3935 */ 909003,
/* 3936 */ 9999,
/* 3937 */ 9999,
/* 3938 */ 914012,
/* 3939 */ 914019,
/* 3940 */ 9999,
/* 3941 */ 9999,
/* 3942 */ 914010,
/* 3943 */ 9999,
/* 3944 */ 9999,
/* 3945 */ 916015,
/* 3946 */ 9999,
/* 3947 */ 9999,
/* 3948 */ 9999,
/* 3949 */ 916013,
/* 3950 */ 9999,
/* 3951 */ 917005,
/* 3952 */ 9999,
/* 3953 */ 917014,
/* 3954 */ 9999,
/* 3955 */ 918005,
/* 3956 */ 9999,
/* 3957 */ 918016,
/* 3958 */ 9999,
/* 3959 */ 919005,
/* 3960 */ 9999,
/* 3961 */ 919017,
/* 3962 */ 9999,
/* 3963 */ 9999,
/* 3964 */ 9999,
/* 3965 */ 910011,
/* 3966 */ 9999,
/* 3967 */ 9999,
/* 3968 */ 910011,
/* 3969 */ 9999,
/* 3970 */ 910018,
/* 3971 */ 910072,
/* 3972 */ 910024,
/* 3973 */ 9999,
/* 3974 */ 910024,
/* 3975 */ 9999,
/* 3976 */ 9999,
/* 3977 */ 910024,
/* 3978 */ 9999,
/* 3979 */ 9999,
/* 3980 */ 9999,
/* 3981 */ 910073,
/* 3982 */ 9999,
/* 3983 */ 9999,
/* 3984 */ 910073,
/* 3985 */ 9999,
/* 3986 */ 9999,
/* 3987 */ 9999,
/* 3988 */ 914013,
/* 3989 */ 9999,
/* 3990 */ 9999,
/* 3991 */ 914013,
/* 3992 */ 9999,
/* 3993 */ 914020,
/* 3994 */ 914054,
/* 3995 */ 914034,
/* 3996 */ 9999,
/* 3997 */ 914034,
/* 3998 */ 9999,
/* 3999 */ 9999,
/* 4000 */ 914034,
/* 4001 */ 9999,
/* 4002 */ 9999,
/* 4003 */ 9999,
/* 4004 */ 914055,
/* 4005 */ 9999,
/* 4006 */ 9999,
/* 4007 */ 914055,
/* 4008 */ 9999,
/* 4009 */ 9999,
/* 4010 */ 9999,
/* 4011 */ 916016,
/* 4012 */ 9999,
/* 4013 */ 9999,
/* 4014 */ 916016,
/* 4015 */ 9999,
/* 4016 */ 9999,
/* 4017 */ 9999,
/* 4018 */ 9999,
/* 4019 */ 923007,
/* 4020 */ 9999,
/* 4021 */ 9999,
/* 4022 */ 922012,
/* 4023 */ 9999,
/* 4024 */ 923008,
/* 4025 */ 923016,
/* 4026 */ 9999,
/* 4027 */ 923016,
/* 4028 */ 9999,
/* 4029 */ 9999,
/* 4030 */ 923016,
/* 4031 */ 9999,
/* 4032 */ 9999,
/* 4033 */ 929011,
/* 4034 */ 929018,
/* 4035 */ 9999,
/* 4036 */ 9999,
/* 4037 */ 928003,
/* 4038 */ 9999,
/* 4039 */ 9999,
/* 4040 */ 9999,
/* 4041 */ 9999,
/* 4042 */ 933013,
/* 4043 */ 9999,
/* 4044 */ 9999,
/* 4045 */ 934011,
/* 4046 */ 934018,
/* 4047 */ 9999,
/* 4048 */ 9999,
/* 4049 */ 934009,
/* 4050 */ 9999,
/* 4051 */ 9999,
/* 4052 */ 935015,
/* 4053 */ 9999,
/* 4054 */ 9999,
/* 4055 */ 9999,
/* 4056 */ 935013,
/* 4057 */ 9999,
/* 4058 */ 9999,
/* 4059 */ 936012,
/* 4060 */ 936019,
/* 4061 */ 9999,
/* 4062 */ 9999,
/* 4063 */ 936010,
/* 4064 */ 9999,
/* 4065 */ 938005,
/* 4066 */ 9999,
/* 4067 */ 938014,
/* 4068 */ 9999,
/* 4069 */ 939005,
/* 4070 */ 9999,
/* 4071 */ 939016,
/* 4072 */ 9999,
/* 4073 */ 940005,
/* 4074 */ 9999,
/* 4075 */ 940017,
/* 4076 */ 9999,
/* 4077 */ 9999,
/* 4078 */ 9999,
/* 4079 */ 929012,
/* 4080 */ 9999,
/* 4081 */ 9999,
/* 4082 */ 929012,
/* 4083 */ 9999,
/* 4084 */ 929019,
/* 4085 */ 929074,
/* 4086 */ 929025,
/* 4087 */ 9999,
/* 4088 */ 929025,
/* 4089 */ 9999,
/* 4090 */ 9999,
/* 4091 */ 929025,
/* 4092 */ 9999,
/* 4093 */ 9999,
/* 4094 */ 9999,
/* 4095 */ 929075,
/* 4096 */ 9999,
/* 4097 */ 9999,
/* 4098 */ 929075,
/* 4099 */ 9999,
/* 4100 */ 9999,
/* 4101 */ 9999,
/* 4102 */ 934012,
/* 4103 */ 9999,
/* 4104 */ 9999,
/* 4105 */ 934012,
/* 4106 */ 9999,
/* 4107 */ 934019,
/* 4108 */ 934029,
/* 4109 */ 934024,
/* 4110 */ 9999,
/* 4111 */ 934024,
/* 4112 */ 9999,
/* 4113 */ 9999,
/* 4114 */ 934024,
/* 4115 */ 9999,
/* 4116 */ 9999,
/* 4117 */ 9999,
/* 4118 */ 934030,
/* 4119 */ 9999,
/* 4120 */ 9999,
/* 4121 */ 9999,
/* 4122 */ 934036,
/* 4123 */ 9999,
/* 4124 */ 9999,
/* 4125 */ 934036,
/* 4126 */ 9999,
/* 4127 */ 9999,
/* 4128 */ 9999,
/* 4129 */ 935016,
/* 4130 */ 9999,
/* 4131 */ 9999,
/* 4132 */ 935016,
/* 4133 */ 9999,
/* 4134 */ 9999,
/* 4135 */ 9999,
/* 4136 */ 936013,
/* 4137 */ 9999,
/* 4138 */ 9999,
/* 4139 */ 936013,
/* 4140 */ 9999,
/* 4141 */ 936020,
/* 4142 */ 936054,
/* 4143 */ 936034,
/* 4144 */ 9999,
/* 4145 */ 936034,
/* 4146 */ 9999,
/* 4147 */ 9999,
/* 4148 */ 936034,
/* 4149 */ 9999,
/* 4150 */ 9999,
/* 4151 */ 9999,
/* 4152 */ 936055,
/* 4153 */ 9999,
/* 4154 */ 9999,
/* 4155 */ 936055,
/* 4156 */ 9999,
/* 4157 */ 9999,
/* 4158 */ 9999,
/* 4159 */ 9999,
/* 4160 */ 945007,
/* 4161 */ 9999,
/* 4162 */ 9999,
/* 4163 */ 943020,
/* 4164 */ 9999,
/* 4165 */ 945008,
/* 4166 */ 945020,
/* 4167 */ 9999,
/* 4168 */ 945020,
/* 4169 */ 9999,
/* 4170 */ 9999,
/* 4171 */ 945020,
/* 4172 */ 9999,
/* 4173 */ 9999,
/* 4174 */ 950015,
/* 4175 */ 9999,
/* 4176 */ 9999,
/* 4177 */ 9999,
/* 4178 */ 950013,
/* 4179 */ 9999,
/* 4180 */ 951005,
/* 4181 */ 9999,
/* 4182 */ 951015,
/* 4183 */ 9999,
/* 4184 */ 9999,
/* 4185 */ 952010,
/* 4186 */ 952017,
/* 4187 */ 9999,
/* 4188 */ 9999,
/* 4189 */ 9999,
/* 4190 */ 952008,
/* 4191 */ 9999,
/* 4192 */ 9999,
/* 4193 */ 953013,
/* 4194 */ 953020,
/* 4195 */ 9999,
/* 4196 */ 9999,
/* 4197 */ 953011,
/* 4198 */ 9999,
/* 4199 */ 9999,
/* 4200 */ 954014,
/* 4201 */ 954021,
/* 4202 */ 9999,
/* 4203 */ 9999,
/* 4204 */ 954012,
/* 4205 */ 9999,
/* 4206 */ 9999,
/* 4207 */ 955011,
/* 4208 */ 955019,
/* 4209 */ 9999,
/* 4210 */ 9999,
/* 4211 */ 955009,
/* 4212 */ 9999,
/* 4213 */ 956005,
/* 4214 */ 9999,
/* 4215 */ 956016,
/* 4216 */ 9999,
/* 4217 */ 957005,
/* 4218 */ 9999,
/* 4219 */ 957014,
/* 4220 */ 9999,
/* 4221 */ 958005,
/* 4222 */ 9999,
/* 4223 */ 958017,
/* 4224 */ 9999,
/* 4225 */ 9999,
/* 4226 */ 9999,
/* 4227 */ 950016,
/* 4228 */ 9999,
/* 4229 */ 9999,
/* 4230 */ 950016,
/* 4231 */ 9999,
/* 4232 */ 9999,
/* 4233 */ 9999,
/* 4234 */ 952011,
/* 4235 */ 9999,
/* 4236 */ 9999,
/* 4237 */ 952011,
/* 4238 */ 9999,
/* 4239 */ 9999,
/* 4240 */ 9999,
/* 4241 */ 953014,
/* 4242 */ 9999,
/* 4243 */ 9999,
/* 4244 */ 953014,
/* 4245 */ 9999,
/* 4246 */ 9999,
/* 4247 */ 9999,
/* 4248 */ 954015,
/* 4249 */ 9999,
/* 4250 */ 9999,
/* 4251 */ 954015,
/* 4252 */ 9999,
/* 4253 */ 9999,
/* 4254 */ 9999,
/* 4255 */ 955012,
/* 4256 */ 9999,
/* 4257 */ 9999,
/* 4258 */ 955012,
/* 4259 */ 9999,
/* 4260 */ 955020,
/* 4261 */ 955030,
/* 4262 */ 955025,
/* 4263 */ 9999,
/* 4264 */ 955025,
/* 4265 */ 9999,
/* 4266 */ 9999,
/* 4267 */ 955025,
/* 4268 */ 9999,
/* 4269 */ 9999,
/* 4270 */ 9999,
/* 4271 */ 955031,
/* 4272 */ 9999,
/* 4273 */ 9999,
/* 4274 */ 9999,
/* 4275 */ 955037,
/* 4276 */ 9999,
/* 4277 */ 9999,
/* 4278 */ 955037,
/* 4279 */ 9999,
/* 4280 */ 9999,
/* 4281 */ 9999,
/* 4282 */ 9999,
/* 4283 */ 961038,
/* 4284 */ 9999,
/* 4285 */ 964007,
/* 4286 */ 9999,
/* 4287 */ 9999,
/* 4288 */ 961018,
/* 4289 */ 9999,
/* 4290 */ 964008,
/* 4291 */ 964019,
/* 4292 */ 9999,
/* 4293 */ 964019,
/* 4294 */ 9999,
/* 4295 */ 9999,
/* 4296 */ 964019,
/* 4297 */ 9999,
/* 4298 */ 9999,
/* 4299 */ 969015,
/* 4300 */ 9999,
/* 4301 */ 9999,
/* 4302 */ 9999,
/* 4303 */ 969013,
/* 4304 */ 9999,
/* 4305 */ 9999,
/* 4306 */ 970011,
/* 4307 */ 970018,
/* 4308 */ 9999,
/* 4309 */ 9999,
/* 4310 */ 970009,
/* 4311 */ 9999,
/* 4312 */ 9999,
/* 4313 */ 971011,
/* 4314 */ 971018,
/* 4315 */ 9999,
/* 4316 */ 9999,
/* 4317 */ 971009,
/* 4318 */ 9999,
/* 4319 */ 9999,
/* 4320 */ 972011,
/* 4321 */ 972018,
/* 4322 */ 9999,
/* 4323 */ 9999,
/* 4324 */ 972008,
/* 4325 */ 9999,
/* 4326 */ 9999,
/* 4327 */ 973011,
/* 4328 */ 973018,
/* 4329 */ 9999,
/* 4330 */ 9999,
/* 4331 */ 973009,
/* 4332 */ 9999,
/* 4333 */ 9999,
/* 4334 */ 974011,
/* 4335 */ 974018,
/* 4336 */ 9999,
/* 4337 */ 9999,
/* 4338 */ 974008,
/* 4339 */ 9999,
/* 4340 */ 9999,
/* 4341 */ 975015,
/* 4342 */ 975022,
/* 4343 */ 9999,
/* 4344 */ 9999,
/* 4345 */ 975013,
/* 4346 */ 9999,
/* 4347 */ 9999,
/* 4348 */ 977011,
/* 4349 */ 977019,
/* 4350 */ 9999,
/* 4351 */ 9999,
/* 4352 */ 977009,
/* 4353 */ 9999,
/* 4354 */ 9999,
/* 4355 */ 9999,
/* 4356 */ 9999,
/* 4357 */ 9999,
/* 4358 */ 9999,
/* 4359 */ 978008,
/* 4360 */ 9999,
/* 4361 */ 9999,
/* 4362 */ 9999,
/* 4363 */ 981015,
/* 4364 */ 9999,
/* 4365 */ 9999,
/* 4366 */ 981009,
/* 4367 */ 9999,
/* 4368 */ 9999,
/* 4369 */ 9999,
/* 4370 */ 997017,
/* 4371 */ 9999,
/* 4372 */ 997011,
/* 4373 */ 9999,
/* 4374 */ 9999,
/* 4375 */ 9999,
/* 4376 */ 998017,
/* 4377 */ 9999,
/* 4378 */ 998011,
/* 4379 */ 9999,
/* 4380 */ 999005,
/* 4381 */ 9999,
/* 4382 */ 999016,
/* 4383 */ 9999,
/* 4384 */ 1000005,
/* 4385 */ 9999,
/* 4386 */ 1000014,
/* 4387 */ 9999,
/* 4388 */ 1001005,
/* 4389 */ 9999,
/* 4390 */ 1001017,
/* 4391 */ 9999,
/* 4392 */ 9999,
/* 4393 */ 9999,
/* 4394 */ 969016,
/* 4395 */ 9999,
/* 4396 */ 9999,
/* 4397 */ 969016,
/* 4398 */ 9999,
/* 4399 */ 9999,
/* 4400 */ 9999,
/* 4401 */ 970012,
/* 4402 */ 9999,
/* 4403 */ 9999,
/* 4404 */ 970012,
/* 4405 */ 9999,
/* 4406 */ 9999,
/* 4407 */ 9999,
/* 4408 */ 971012,
/* 4409 */ 9999,
/* 4410 */ 9999,
/* 4411 */ 971012,
/* 4412 */ 9999,
/* 4413 */ 9999,
/* 4414 */ 9999,
/* 4415 */ 972012,
/* 4416 */ 9999,
/* 4417 */ 9999,
/* 4418 */ 972012,
/* 4419 */ 9999,
/* 4420 */ 9999,
/* 4421 */ 9999,
/* 4422 */ 973012,
/* 4423 */ 9999,
/* 4424 */ 9999,
/* 4425 */ 973012,
/* 4426 */ 9999,
/* 4427 */ 9999,
/* 4428 */ 9999,
/* 4429 */ 974012,
/* 4430 */ 9999,
/* 4431 */ 9999,
/* 4432 */ 974012,
/* 4433 */ 9999,
/* 4434 */ 9999,
/* 4435 */ 9999,
/* 4436 */ 975016,
/* 4437 */ 9999,
/* 4438 */ 9999,
/* 4439 */ 975016,
/* 4440 */ 9999,
/* 4441 */ 975023,
/* 4442 */ 976006,
/* 4443 */ 975028,
/* 4444 */ 9999,
/* 4445 */ 975028,
/* 4446 */ 9999,
/* 4447 */ 9999,
/* 4448 */ 975028,
/* 4449 */ 9999,
/* 4450 */ 9999,
/* 4451 */ 9999,
/* 4452 */ 976007,
/* 4453 */ 9999,
/* 4454 */ 9999,
/* 4455 */ 9999,
/* 4456 */ 976013,
/* 4457 */ 9999,
/* 4458 */ 9999,
/* 4459 */ 976013,
/* 4460 */ 9999,
/* 4461 */ 9999,
/* 4462 */ 9999,
/* 4463 */ 977012,
/* 4464 */ 9999,
/* 4465 */ 9999,
/* 4466 */ 977012,
/* 4467 */ 9999,
/* 4468 */ 977020,
/* 4469 */ 977030,
/* 4470 */ 977025,
/* 4471 */ 9999,
/* 4472 */ 977025,
/* 4473 */ 9999,
/* 4474 */ 9999,
/* 4475 */ 977025,
/* 4476 */ 9999,
/* 4477 */ 9999,
/* 4478 */ 9999,
/* 4479 */ 977031,
/* 4480 */ 9999,
/* 4481 */ 9999,
/* 4482 */ 9999,
/* 4483 */ 977037,
/* 4484 */ 9999,
/* 4485 */ 9999,
/* 4486 */ 977037,
/* 4487 */ 9999,
/* 4488 */ 9999,
/* 4489 */ 9999,
/* 4490 */ 983025,
/* 4491 */ 984006,
/* 4492 */ 996003,
/* 4493 */ 981035,
/* 4494 */ 9999,
/* 4495 */ 981035,
/* 4496 */ 9999,
/* 4497 */ 9999,
/* 4498 */ 981035,
/* 4499 */ 9999,
/* 4500 */ 9999,
/* 4501 */ 984014,
/* 4502 */ 9999,
/* 4503 */ 984010,
/* 4504 */ 9999,
/* 4505 */ 984010,
/* 4506 */ 9999,
/* 4507 */ 9999,
/* 4508 */ 985015,
/* 4509 */ 990010,
/* 4510 */ 985013,
/* 4511 */ 9999,
/* 4512 */ 985013,
/* 4513 */ 9999,
/* 4514 */ 9999,
/* 4515 */ 994013,
/* 4516 */ 994011,
/* 4517 */ 9999,
/* 4518 */ 994011,
/* 4519 */ 9999,
/* 4520 */ 9999,
/* 4521 */ 994011,
/* 4522 */ 9999,
/* 4523 */ 985017,
/* 4524 */ 9999,
/* 4525 */ 985023,
/* 4526 */ 9999,
/* 4527 */ 9999,
/* 4528 */ 9999,
/* 4529 */ 986031,
/* 4530 */ 9999,
/* 4531 */ 986024,
/* 4532 */ 9999,
/* 4533 */ 9999,
/* 4534 */ 9999,
/* 4535 */ 988034,
/* 4536 */ 9999,
/* 4537 */ 988024,
/* 4538 */ 9999,
/* 4539 */ 986032,
/* 4540 */ 9999,
/* 4541 */ 986036,
/* 4542 */ 9999,
/* 4543 */ 9999,
/* 4544 */ 986036,
/* 4545 */ 9999,
/* 4546 */ 988035,
/* 4547 */ 9999,
/* 4548 */ 988039,
/* 4549 */ 9999,
/* 4550 */ 9999,
/* 4551 */ 988039,
/* 4552 */ 9999,
/* 4553 */ 9999,
/* 4554 */ 990014,
/* 4555 */ 9999,
/* 4556 */ 990012,
/* 4557 */ 9999,
/* 4558 */ 9999,
/* 4559 */ 9999,
/* 4560 */ 991023,
/* 4561 */ 9999,
/* 4562 */ 991035,
/* 4563 */ 9999,
/* 4564 */ 991014,
/* 4565 */ 9999,
/* 4566 */ 9999,
/* 4567 */ 993016,
/* 4568 */ 9999,
/* 4569 */ 993014,
/* 4570 */ 9999,
/* 4571 */ 9999,
/* 4572 */ 993014,
/* 4573 */ 9999,
/* 4574 */ 9999,
/* 4575 */ 9999,
/* 4576 */ 996004,
/* 4577 */ 9999,
/* 4578 */ 9999,
/* 4579 */ 996004,
/* 4580 */ 9999,
/* 4581 */ 997018,
/* 4582 */ 997029,
/* 4583 */ 9999,
/* 4584 */ 997029,
/* 4585 */ 9999,
/* 4586 */ 9999,
/* 4587 */ 997029,
/* 4588 */ 9999,
/* 4589 */ 998018,
/* 4590 */ 998029,
/* 4591 */ 9999,
/* 4592 */ 998029,
/* 4593 */ 9999,
/* 4594 */ 9999,
/* 4595 */ 998029,
/* 4596 */ 9999,
/* 4597 */ 9999,
/* 4598 */ 9999,
/* 4599 */ 9999,
/* 4600 */ 1006009,
/* 4601 */ 1007005,
/* 4602 */ 9999,
/* 4603 */ 1008007,
/* 4604 */ 9999,
/* 4605 */ 9999,
/* 4606 */ 1004027,
/* 4607 */ 9999,
/* 4608 */ 9999,
/* 4609 */ 1007010,
/* 4610 */ 9999,
/* 4611 */ 1007008,
/* 4612 */ 9999,
/* 4613 */ 9999,
/* 4614 */ 1007008,
/* 4615 */ 9999,
/* 4616 */ 1008008,
/* 4617 */ 1008021,
/* 4618 */ 9999,
/* 4619 */ 1008021,
/* 4620 */ 9999,
/* 4621 */ 9999,
/* 4622 */ 1008021,
/* 4623 */ 9999,
/* 4624 */ 9999,
/* 4625 */ 1013015,
/* 4626 */ 9999,
/* 4627 */ 9999,
/* 4628 */ 9999,
/* 4629 */ 1013013,
/* 4630 */ 9999,
/* 4631 */ 9999,
/* 4632 */ 1014009,
/* 4633 */ 1014016,
/* 4634 */ 9999,
/* 4635 */ 9999,
/* 4636 */ 1014007,
/* 4637 */ 9999,
/* 4638 */ 9999,
/* 4639 */ 1015011,
/* 4640 */ 1015018,
/* 4641 */ 9999,
/* 4642 */ 9999,
/* 4643 */ 1015009,
/* 4644 */ 9999,
/* 4645 */ 1016005,
/* 4646 */ 9999,
/* 4647 */ 1016015,
/* 4648 */ 9999,
/* 4649 */ 1017005,
/* 4650 */ 1017028,
/* 4651 */ 1017035,
/* 4652 */ 9999,
/* 4653 */ 9999,
/* 4654 */ 1017005,
/* 4655 */ 9999,
/* 4656 */ 9999,
/* 4657 */ 1018013,
/* 4658 */ 1018020,
/* 4659 */ 9999,
/* 4660 */ 9999,
/* 4661 */ 1018011,
/* 4662 */ 9999,
/* 4663 */ 9999,
/* 4664 */ 1019013,
/* 4665 */ 1019020,
/* 4666 */ 9999,
/* 4667 */ 9999,
/* 4668 */ 1019007,
/* 4669 */ 9999,
/* 4670 */ 9999,
/* 4671 */ 1020013,
/* 4672 */ 1020020,
/* 4673 */ 9999,
/* 4674 */ 9999,
/* 4675 */ 1020007,
/* 4676 */ 9999,
/* 4677 */ 9999,
/* 4678 */ 1021013,
/* 4679 */ 1021020,
/* 4680 */ 9999,
/* 4681 */ 9999,
/* 4682 */ 1021007,
/* 4683 */ 9999,
/* 4684 */ 9999,
/* 4685 */ 1022013,
/* 4686 */ 1022020,
/* 4687 */ 9999,
/* 4688 */ 9999,
/* 4689 */ 1022007,
/* 4690 */ 9999,
/* 4691 */ 9999,
/* 4692 */ 1023013,
/* 4693 */ 1023020,
/* 4694 */ 9999,
/* 4695 */ 9999,
/* 4696 */ 1023009,
/* 4697 */ 9999,
/* 4698 */ 9999,
/* 4699 */ 9999,
/* 4700 */ 1024024,
/* 4701 */ 1024031,
/* 4702 */ 9999,
/* 4703 */ 9999,
/* 4704 */ 1024010,
/* 4705 */ 9999,
/* 4706 */ 9999,
/* 4707 */ 1025013,
/* 4708 */ 1025020,
/* 4709 */ 9999,
/* 4710 */ 9999,
/* 4711 */ 1025008,
/* 4712 */ 9999,
/* 4713 */ 9999,
/* 4714 */ 1026013,
/* 4715 */ 1026020,
/* 4716 */ 9999,
/* 4717 */ 9999,
/* 4718 */ 1026009,
/* 4719 */ 9999,
/* 4720 */ 9999,
/* 4721 */ 1027013,
/* 4722 */ 1027020,
/* 4723 */ 9999,
/* 4724 */ 9999,
/* 4725 */ 1027008,
/* 4726 */ 9999,
/* 4727 */ 9999,
/* 4728 */ 1028019,
/* 4729 */ 1028026,
/* 4730 */ 1028035,
/* 4731 */ 9999,
/* 4732 */ 1028013,
/* 4733 */ 9999,
/* 4734 */ 9999,
/* 4735 */ 1029012,
/* 4736 */ 1029019,
/* 4737 */ 1029037,
/* 4738 */ 9999,
/* 4739 */ 9999,
/* 4740 */ 1029010,
/* 4741 */ 9999,
/* 4742 */ 9999,
/* 4743 */ 9999,
/* 4744 */ 1031015,
/* 4745 */ 9999,
/* 4746 */ 9999,
/* 4747 */ 9999,
/* 4748 */ 1031009,
/* 4749 */ 9999,
/* 4750 */ 9999,
/* 4751 */ 9999,
/* 4752 */ 1033015,
/* 4753 */ 9999,
/* 4754 */ 1033030,
/* 4755 */ 9999,
/* 4756 */ 9999,
/* 4757 */ 9999,
/* 4758 */ 1033009,
/* 4759 */ 9999,
/* 4760 */ 9999,
/* 4761 */ 9999,
/* 4762 */ 1035015,
/* 4763 */ 9999,
/* 4764 */ 1035030,
/* 4765 */ 9999,
/* 4766 */ 1035046,
/* 4767 */ 9999,
/* 4768 */ 9999,
/* 4769 */ 9999,
/* 4770 */ 1035009,
/* 4771 */ 9999,
/* 4772 */ 9999,
/* 4773 */ 9999,
/* 4774 */ 1038015,
/* 4775 */ 9999,
/* 4776 */ 1038030,
/* 4777 */ 9999,
/* 4778 */ 1038046,
/* 4779 */ 9999,
/* 4780 */ 1038062,
/* 4781 */ 9999,
/* 4782 */ 9999,
/* 4783 */ 9999,
/* 4784 */ 1038009,
/* 4785 */ 9999,
/* 4786 */ 9999,
/* 4787 */ 1042013,
/* 4788 */ 1042020,
/* 4789 */ 9999,
/* 4790 */ 9999,
/* 4791 */ 1042011,
/* 4792 */ 9999,
/* 4793 */ 9999,
/* 4794 */ 1045013,
/* 4795 */ 1045020,
/* 4796 */ 9999,
/* 4797 */ 9999,
/* 4798 */ 1045010,
/* 4799 */ 9999,
/* 4800 */ 9999,
/* 4801 */ 1048011,
/* 4802 */ 1048018,
/* 4803 */ 1049019,
/* 4804 */ 9999,
/* 4805 */ 9999,
/* 4806 */ 1048009,
/* 4807 */ 9999,
/* 4808 */ 9999,
/* 4809 */ 1050012,
/* 4810 */ 1050019,
/* 4811 */ 9999,
/* 4812 */ 9999,
/* 4813 */ 1050010,
/* 4814 */ 9999,
/* 4815 */ 9999,
/* 4816 */ 1053013,
/* 4817 */ 1053020,
/* 4818 */ 9999,
/* 4819 */ 9999,
/* 4820 */ 1053011,
/* 4821 */ 9999,
/* 4822 */ 9999,
/* 4823 */ 1055012,
/* 4824 */ 1055019,
/* 4825 */ 9999,
/* 4826 */ 9999,
/* 4827 */ 1055010,
/* 4828 */ 9999,
/* 4829 */ 9999,
/* 4830 */ 1057011,
/* 4831 */ 1057018,
/* 4832 */ 9999,
/* 4833 */ 1057009,
/* 4834 */ 9999,
/* 4835 */ 1065005,
/* 4836 */ 9999,
/* 4837 */ 1065013,
/* 4838 */ 9999,
/* 4839 */ 1066005,
/* 4840 */ 9999,
/* 4841 */ 1066014,
/* 4842 */ 9999,
/* 4843 */ 1067005,
/* 4844 */ 9999,
/* 4845 */ 1067016,
/* 4846 */ 9999,
/* 4847 */ 1068005,
/* 4848 */ 9999,
/* 4849 */ 1068017,
/* 4850 */ 9999,
/* 4851 */ 9999,
/* 4852 */ 9999,
/* 4853 */ 1013016,
/* 4854 */ 9999,
/* 4855 */ 9999,
/* 4856 */ 1013016,
/* 4857 */ 9999,
/* 4858 */ 9999,
/* 4859 */ 9999,
/* 4860 */ 1014010,
/* 4861 */ 9999,
/* 4862 */ 9999,
/* 4863 */ 1014010,
/* 4864 */ 9999,
/* 4865 */ 9999,
/* 4866 */ 9999,
/* 4867 */ 1015012,
/* 4868 */ 9999,
/* 4869 */ 9999,
/* 4870 */ 1015012,
/* 4871 */ 9999,
/* 4872 */ 9999,
/* 4873 */ 9999,
/* 4874 */ 1017015,
/* 4875 */ 9999,
/* 4876 */ 9999,
/* 4877 */ 9999,
/* 4878 */ 1017025,
/* 4879 */ 9999,
/* 4880 */ 9999,
/* 4881 */ 9999,
/* 4882 */ 1017029,
/* 4883 */ 9999,
/* 4884 */ 9999,
/* 4885 */ 1017029,
/* 4886 */ 9999,
/* 4887 */ 9999,
/* 4888 */ 9999,
/* 4889 */ 1018014,
/* 4890 */ 9999,
/* 4891 */ 9999,
/* 4892 */ 1018014,
/* 4893 */ 9999,
/* 4894 */ 9999,
/* 4895 */ 9999,
/* 4896 */ 1019014,
/* 4897 */ 9999,
/* 4898 */ 9999,
/* 4899 */ 1019014,
/* 4900 */ 9999,
/* 4901 */ 9999,
/* 4902 */ 9999,
/* 4903 */ 1020014,
/* 4904 */ 9999,
/* 4905 */ 9999,
/* 4906 */ 1020014,
/* 4907 */ 9999,
/* 4908 */ 9999,
/* 4909 */ 9999,
/* 4910 */ 1021014,
/* 4911 */ 9999,
/* 4912 */ 9999,
/* 4913 */ 1021014,
/* 4914 */ 9999,
/* 4915 */ 9999,
/* 4916 */ 9999,
/* 4917 */ 1022014,
/* 4918 */ 9999,
/* 4919 */ 9999,
/* 4920 */ 1022014,
/* 4921 */ 9999,
/* 4922 */ 9999,
/* 4923 */ 9999,
/* 4924 */ 1023014,
/* 4925 */ 9999,
/* 4926 */ 9999,
/* 4927 */ 1023014,
/* 4928 */ 9999,
/* 4929 */ 9999,
/* 4930 */ 9999,
/* 4931 */ 1024025,
/* 4932 */ 9999,
/* 4933 */ 9999,
/* 4934 */ 1024025,
/* 4935 */ 9999,
/* 4936 */ 9999,
/* 4937 */ 9999,
/* 4938 */ 1025014,
/* 4939 */ 9999,
/* 4940 */ 9999,
/* 4941 */ 1025014,
/* 4942 */ 9999,
/* 4943 */ 9999,
/* 4944 */ 9999,
/* 4945 */ 1026014,
/* 4946 */ 9999,
/* 4947 */ 9999,
/* 4948 */ 1026014,
/* 4949 */ 9999,
/* 4950 */ 9999,
/* 4951 */ 9999,
/* 4952 */ 1027014,
/* 4953 */ 9999,
/* 4954 */ 9999,
/* 4955 */ 1027014,
/* 4956 */ 9999,
/* 4957 */ 9999,
/* 4958 */ 9999,
/* 4959 */ 1028020,
/* 4960 */ 9999,
/* 4961 */ 9999,
/* 4962 */ 1028020,
/* 4963 */ 9999,
/* 4964 */ 9999,
/* 4965 */ 9999,
/* 4966 */ 1028036,
/* 4967 */ 9999,
/* 4968 */ 9999,
/* 4969 */ 9999,
/* 4970 */ 1028040,
/* 4971 */ 9999,
/* 4972 */ 9999,
/* 4973 */ 9999,
/* 4974 */ 1029013,
/* 4975 */ 9999,
/* 4976 */ 9999,
/* 4977 */ 1029013,
/* 4978 */ 9999,
/* 4979 */ 9999,
/* 4980 */ 1029042,
/* 4981 */ 1029038,
/* 4982 */ 9999,
/* 4983 */ 1029038,
/* 4984 */ 9999,
/* 4985 */ 9999,
/* 4986 */ 1029038,
/* 4987 */ 9999,
/* 4988 */ 9999,
/* 4989 */ 9999,
/* 4990 */ 1042014,
/* 4991 */ 9999,
/* 4992 */ 9999,
/* 4993 */ 1042014,
/* 4994 */ 9999,
/* 4995 */ 1042021,
/* 4996 */ 1044007,
/* 4997 */ 1042021,
/* 4998 */ 9999,
/* 4999 */ 1042021,
/* 5000 */ 9999,
/* 5001 */ 9999,
/* 5002 */ 1042021,
/* 5003 */ 9999,
/* 5004 */ 1042023,
/* 5005 */ 9999,
/* 5006 */ 1042028,
/* 5007 */ 9999,
/* 5008 */ 1043015,
/* 5009 */ 1043025,
/* 5010 */ 9999,
/* 5011 */ 1043015,
/* 5012 */ 9999,
/* 5013 */ 9999,
/* 5014 */ 9999,
/* 5015 */ 1043016,
/* 5016 */ 9999,
/* 5017 */ 9999,
/* 5018 */ 9999,
/* 5019 */ 1043020,
/* 5020 */ 9999,
/* 5021 */ 9999,
/* 5022 */ 9999,
/* 5023 */ 1044008,
/* 5024 */ 9999,
/* 5025 */ 9999,
/* 5026 */ 1044008,
/* 5027 */ 9999,
/* 5028 */ 9999,
/* 5029 */ 9999,
/* 5030 */ 1045014,
/* 5031 */ 9999,
/* 5032 */ 9999,
/* 5033 */ 1045014,
/* 5034 */ 9999,
/* 5035 */ 1045021,
/* 5036 */ 1047007,
/* 5037 */ 1045021,
/* 5038 */ 9999,
/* 5039 */ 1045021,
/* 5040 */ 9999,
/* 5041 */ 9999,
/* 5042 */ 1045021,
/* 5043 */ 9999,
/* 5044 */ 1045023,
/* 5045 */ 9999,
/* 5046 */ 1045028,
/* 5047 */ 9999,
/* 5048 */ 1046015,
/* 5049 */ 1046025,
/* 5050 */ 9999,
/* 5051 */ 1046015,
/* 5052 */ 9999,
/* 5053 */ 9999,
/* 5054 */ 9999,
/* 5055 */ 1046016,
/* 5056 */ 9999,
/* 5057 */ 9999,
/* 5058 */ 9999,
/* 5059 */ 1046020,
/* 5060 */ 9999,
/* 5061 */ 9999,
/* 5062 */ 9999,
/* 5063 */ 1047008,
/* 5064 */ 9999,
/* 5065 */ 9999,
/* 5066 */ 1047008,
/* 5067 */ 9999,
/* 5068 */ 9999,
/* 5069 */ 9999,
/* 5070 */ 1048012,
/* 5071 */ 9999,
/* 5072 */ 9999,
/* 5073 */ 1048012,
/* 5074 */ 9999,
/* 5075 */ 1048019,
/* 5076 */ 9999,
/* 5077 */ 1048024,
/* 5078 */ 9999,
/* 5079 */ 1048024,
/* 5080 */ 9999,
/* 5081 */ 9999,
/* 5082 */ 1048024,
/* 5083 */ 9999,
/* 5084 */ 9999,
/* 5085 */ 9999,
/* 5086 */ 1050013,
/* 5087 */ 9999,
/* 5088 */ 9999,
/* 5089 */ 1050013,
/* 5090 */ 9999,
/* 5091 */ 1050021,
/* 5092 */ 1052006,
/* 5093 */ 1050027,
/* 5094 */ 9999,
/* 5095 */ 1050027,
/* 5096 */ 9999,
/* 5097 */ 9999,
/* 5098 */ 1050027,
/* 5099 */ 9999,
/* 5100 */ 9999,
/* 5101 */ 9999,
/* 5102 */ 1052007,
/* 5103 */ 9999,
/* 5104 */ 9999,
/* 5105 */ 1052007,
/* 5106 */ 9999,
/* 5107 */ 9999,
/* 5108 */ 9999,
/* 5109 */ 1053014,
/* 5110 */ 9999,
/* 5111 */ 9999,
/* 5112 */ 1053014,
/* 5113 */ 9999,
/* 5114 */ 1053022,
/* 5115 */ 1054006,
/* 5116 */ 1053028,
/* 5117 */ 9999,
/* 5118 */ 1053028,
/* 5119 */ 9999,
/* 5120 */ 9999,
/* 5121 */ 1053028,
/* 5122 */ 9999,
/* 5123 */ 9999,
/* 5124 */ 9999,
/* 5125 */ 1054007,
/* 5126 */ 9999,
/* 5127 */ 9999,
/* 5128 */ 1054007,
/* 5129 */ 9999,
/* 5130 */ 9999,
/* 5131 */ 9999,
/* 5132 */ 1055013,
/* 5133 */ 9999,
/* 5134 */ 9999,
/* 5135 */ 1055013,
/* 5136 */ 9999,
/* 5137 */ 1055021,
/* 5138 */ 1056006,
/* 5139 */ 1055027,
/* 5140 */ 9999,
/* 5141 */ 1055027,
/* 5142 */ 9999,
/* 5143 */ 9999,
/* 5144 */ 1055027,
/* 5145 */ 9999,
/* 5146 */ 9999,
/* 5147 */ 9999,
/* 5148 */ 1056007,
/* 5149 */ 9999,
/* 5150 */ 9999,
/* 5151 */ 1056007,
/* 5152 */ 9999,
/* 5153 */ 9999,
/* 5154 */ 9999,
/* 5155 */ 1057012,
/* 5156 */ 9999,
/* 5157 */ 9999,
/* 5158 */ 1057012,
/* 5159 */ 9999,
/* 5160 */ 1057020,
/* 5161 */ 1064006,
/* 5162 */ 1057020,
/* 5163 */ 9999,
/* 5164 */ 1057020,
/* 5165 */ 9999,
/* 5166 */ 9999,
/* 5167 */ 1057020,
/* 5168 */ 9999,
/* 5169 */ 1058006,
/* 5170 */ 9999,
/* 5171 */ 1058011,
/* 5172 */ 9999,
/* 5173 */ 1063008,
/* 5174 */ 1063018,
/* 5175 */ 9999,
/* 5176 */ 1063008,
/* 5177 */ 9999,
/* 5178 */ 9999,
/* 5179 */ 9999,
/* 5180 */ 1063009,
/* 5181 */ 9999,
/* 5182 */ 9999,
/* 5183 */ 9999,
/* 5184 */ 1063013,
/* 5185 */ 9999,
/* 5186 */ 9999,
/* 5187 */ 9999,
/* 5188 */ 1064007,
/* 5189 */ 9999,
/* 5190 */ 9999,
/* 5191 */ 9999,
/* 5192 */ 1064011,
/* 5193 */ 9999,
/* 5194 */ 1073003,
/* 5195 */ 9999,
/* 5196 */ 1074005,
/* 5197 */ 1076003,
/* 5198 */ 1078003,
/* 5199 */ 1079005,
/* 5200 */ 1091003,
/* 5201 */ 9999,
/* 5202 */ 1071032,
/* 5203 */ 9999,
/* 5204 */ 1104005,
/* 5205 */ 9999,
/* 5206 */ 1104020,
/* 5207 */ 1106019,
/* 5208 */ 9999,
/* 5209 */ 1104010,
/* 5210 */ 9999,
/* 5211 */ 1074007,
/* 5212 */ 9999,
/* 5213 */ 1074015,
/* 5214 */ 9999,
/* 5215 */ 1075009,
/* 5216 */ 9999,
/* 5217 */ 9999,
/* 5218 */ 1075014,
/* 5219 */ 9999,
/* 5220 */ 9999,
/* 5221 */ 1075014,
/* 5222 */ 9999,
/* 5223 */ 1076004,
/* 5224 */ 9999,
/* 5225 */ 1076009,
/* 5226 */ 9999,
/* 5227 */ 9999,
/* 5228 */ 1076009,
/* 5229 */ 9999,
/* 5230 */ 9999,
/* 5231 */ 9999,
/* 5232 */ 1078006,
/* 5233 */ 9999,
/* 5234 */ 9999,
/* 5235 */ 1078006,
/* 5236 */ 9999,
/* 5237 */ 9999,
/* 5238 */ 1079011,
/* 5239 */ 1079040,
/* 5240 */ 9999,
/* 5241 */ 9999,
/* 5242 */ 1079007,
/* 5243 */ 9999,
/* 5244 */ 9999,
/* 5245 */ 1079007,
/* 5246 */ 9999,
/* 5247 */ 9999,
/* 5248 */ 9999,
/* 5249 */ 1079018,
/* 5250 */ 9999,
/* 5251 */ 9999,
/* 5252 */ 9999,
/* 5253 */ 1079026,
/* 5254 */ 9999,
/* 5255 */ 9999,
/* 5256 */ 9999,
/* 5257 */ 1079037,
/* 5258 */ 9999,
/* 5259 */ 9999,
/* 5260 */ 1092005,
/* 5261 */ 1093005,
/* 5262 */ 1094005,
/* 5263 */ 1095005,
/* 5264 */ 1091006,
/* 5265 */ 9999,
/* 5266 */ 1091006,
/* 5267 */ 9999,
/* 5268 */ 9999,
/* 5269 */ 1091006,
/* 5270 */ 9999,
/* 5271 */ 1092007,
/* 5272 */ 9999,
/* 5273 */ 1092015,
/* 5274 */ 9999,
/* 5275 */ 9999,
/* 5276 */ 1092015,
/* 5277 */ 9999,
/* 5278 */ 1093006,
/* 5279 */ 9999,
/* 5280 */ 1093011,
/* 5281 */ 9999,
/* 5282 */ 9999,
/* 5283 */ 1093011,
/* 5284 */ 9999,
/* 5285 */ 9999,
/* 5286 */ 9999,
/* 5287 */ 1094008,
/* 5288 */ 9999,
/* 5289 */ 9999,
/* 5290 */ 1094008,
/* 5291 */ 9999,
/* 5292 */ 9999,
/* 5293 */ 1095011,
/* 5294 */ 1095040,
/* 5295 */ 9999,
/* 5296 */ 9999,
/* 5297 */ 1095007,
/* 5298 */ 9999,
/* 5299 */ 9999,
/* 5300 */ 1095007,
/* 5301 */ 9999,
/* 5302 */ 9999,
/* 5303 */ 9999,
/* 5304 */ 1095018,
/* 5305 */ 9999,
/* 5306 */ 9999,
/* 5307 */ 9999,
/* 5308 */ 1095026,
/* 5309 */ 9999,
/* 5310 */ 9999,
/* 5311 */ 9999,
/* 5312 */ 1095037,
/* 5313 */ 9999,
/* 5314 */ 1106021,
/* 5315 */ 9999,
/* 5316 */ 1106026,
/* 5317 */ 9999,
/* 5318 */ 9999,
/* 5319 */ 1106026,
/* 5320 */ 9999,
/* 5321 */ 9999,
/* 5322 */ 9999,
/* 5323 */ 1110018,
/* 5324 */ 9999,
/* 5325 */ 9999,
/* 5326 */ 9999,
/* 5327 */ 9999,
/* 5328 */ 1113028,
/* 5329 */ 9999,
/* 5330 */ 1119007,
/* 5331 */ 9999,
/* 5332 */ 9999,
/* 5333 */ 1113008,
/* 5334 */ 9999,
/* 5335 */ 1119008,
/* 5336 */ 1119019,
/* 5337 */ 9999,
/* 5338 */ 1119019,
/* 5339 */ 9999,
/* 5340 */ 9999,
/* 5341 */ 1119019,
/* 5342 */ 9999,
/* 5343 */ 9999,
/* 5344 */ 1123016,
/* 5345 */ 9999,
/* 5346 */ 9999,
/* 5347 */ 9999,
/* 5348 */ 1123013,
/* 5349 */ 9999,
/* 5350 */ 9999,
/* 5351 */ 1124016,
/* 5352 */ 1124023,
/* 5353 */ 9999,
/* 5354 */ 9999,
/* 5355 */ 1124012,
/* 5356 */ 9999,
/* 5357 */ 9999,
/* 5358 */ 1125016,
/* 5359 */ 1125023,
/* 5360 */ 9999,
/* 5361 */ 9999,
/* 5362 */ 1125014,
/* 5363 */ 9999,
/* 5364 */ 9999,
/* 5365 */ 1126016,
/* 5366 */ 1126023,
/* 5367 */ 9999,
/* 5368 */ 9999,
/* 5369 */ 9999,
/* 5370 */ 1126010,
/* 5371 */ 9999,
/* 5372 */ 9999,
/* 5373 */ 9999,
/* 5374 */ 1123017,
/* 5375 */ 9999,
/* 5376 */ 9999,
/* 5377 */ 1123017,
/* 5378 */ 9999,
/* 5379 */ 9999,
/* 5380 */ 9999,
/* 5381 */ 1124017,
/* 5382 */ 9999,
/* 5383 */ 9999,
/* 5384 */ 1124017,
/* 5385 */ 9999,
/* 5386 */ 9999,
/* 5387 */ 9999,
/* 5388 */ 1125017,
/* 5389 */ 9999,
/* 5390 */ 9999,
/* 5391 */ 1125017,
/* 5392 */ 9999,
/* 5393 */ 9999,
/* 5394 */ 9999,
/* 5395 */ 1126017,
/* 5396 */ 9999,
/* 5397 */ 9999,
/* 5398 */ 1126017,
/* 5399 */ 9999,
/* 5400 */ 9999,
/* 5401 */ 9999,
/* 5402 */ 1134005,
/* 5403 */ 9999,
/* 5404 */ 1136009,
/* 5405 */ 9999,
/* 5406 */ 9999,
/* 5407 */ 1131016,
/* 5408 */ 9999,
/* 5409 */ 9999,
/* 5410 */ 1134011,
/* 5411 */ 9999,
/* 5412 */ 1134007,
/* 5413 */ 9999,
/* 5414 */ 9999,
/* 5415 */ 1134007,
/* 5416 */ 9999,
/* 5417 */ 1136010,
/* 5418 */ 1136024,
/* 5419 */ 9999,
/* 5420 */ 1136024,
/* 5421 */ 9999,
/* 5422 */ 9999,
/* 5423 */ 1136024,
/* 5424 */ 9999,
/* 5425 */ 9999,
/* 5426 */ 9999,
/* 5427 */ 1140017,
/* 5428 */ 9999,
/* 5429 */ 9999,
/* 5430 */ 1140011,
/* 5431 */ 9999,
/* 5432 */ 9999,
/* 5433 */ 9999,
/* 5434 */ 1141015,
/* 5435 */ 9999,
/* 5436 */ 9999,
/* 5437 */ 1141009,
/* 5438 */ 9999,
/* 5439 */ 9999,
/* 5440 */ 9999,
/* 5441 */ 1142016,
/* 5442 */ 9999,
/* 5443 */ 9999,
/* 5444 */ 1142010,
/* 5445 */ 9999,
/* 5446 */ 9999,
/* 5447 */ 9999,
/* 5448 */ 1143017,
/* 5449 */ 9999,
/* 5450 */ 9999,
/* 5451 */ 1143011,
/* 5452 */ 9999,
/* 5453 */ 9999,
/* 5454 */ 9999,
/* 5455 */ 1144017,
/* 5456 */ 9999,
/* 5457 */ 9999,
/* 5458 */ 9999,
/* 5459 */ 1144011,
/* 5460 */ 9999,
/* 5461 */ 9999,
/* 5462 */ 9999,
/* 5463 */ 1145017,
/* 5464 */ 1145027,
/* 5465 */ 1146030,
/* 5466 */ 9999,
/* 5467 */ 9999,
/* 5468 */ 1145011,
/* 5469 */ 9999,
/* 5470 */ 9999,
/* 5471 */ 9999,
/* 5472 */ 1147015,
/* 5473 */ 9999,
/* 5474 */ 9999,
/* 5475 */ 1147009,
/* 5476 */ 9999,
/* 5477 */ 9999,
/* 5478 */ 9999,
/* 5479 */ 1149006,
/* 5480 */ 9999,
/* 5481 */ 9999,
/* 5482 */ 1148009,
/* 5483 */ 9999,
/* 5484 */ 9999,
/* 5485 */ 1151011,
/* 5486 */ 1151018,
/* 5487 */ 9999,
/* 5488 */ 9999,
/* 5489 */ 1151009,
/* 5490 */ 9999,
/* 5491 */ 1153005,
/* 5492 */ 9999,
/* 5493 */ 1153013,
/* 5494 */ 9999,
/* 5495 */ 1154005,
/* 5496 */ 9999,
/* 5497 */ 1154014,
/* 5498 */ 9999,
/* 5499 */ 1155005,
/* 5500 */ 9999,
/* 5501 */ 1155016,
/* 5502 */ 9999,
/* 5503 */ 1156005,
/* 5504 */ 9999,
/* 5505 */ 1156017,
/* 5506 */ 9999,
/* 5507 */ 9999,
/* 5508 */ 9999,
/* 5509 */ 1145029,
/* 5510 */ 9999,
/* 5511 */ 9999,
/* 5512 */ 9999,
/* 5513 */ 1145031,
/* 5514 */ 9999,
/* 5515 */ 1149007,
/* 5516 */ 1149020,
/* 5517 */ 9999,
/* 5518 */ 1149020,
/* 5519 */ 9999,
/* 5520 */ 9999,
/* 5521 */ 1149020,
/* 5522 */ 9999,
/* 5523 */ 9999,
/* 5524 */ 9999,
/* 5525 */ 1151012,
/* 5526 */ 9999,
/* 5527 */ 9999,
/* 5528 */ 1151012,
/* 5529 */ 9999,
/* 5530 */ 1151019,
/* 5531 */ 1152005,
/* 5532 */ 1151024,
/* 5533 */ 9999,
/* 5534 */ 1151024,
/* 5535 */ 9999,
/* 5536 */ 9999,
/* 5537 */ 1151024,
/* 5538 */ 9999,
/* 5539 */ 9999,
/* 5540 */ 9999,
/* 5541 */ 1152006,
/* 5542 */ 9999,
/* 5543 */ 9999,
/* 5544 */ 9999,
/* 5545 */ 1152012,
/* 5546 */ 9999,
/* 5547 */ 9999,
/* 5548 */ 1152012,
/* 5549 */ 9999,
/* 5550 */ 1161003,
/* 5551 */ 1166003,
/* 5552 */ 9999,
/* 5553 */ 1186003,
/* 5554 */ 1192003,
/* 5555 */ 1198003,
/* 5556 */ 9999,
/* 5557 */ 1160003,
/* 5558 */ 9999,
/* 5559 */ 9999,
/* 5560 */ 9999,
/* 5561 */ 1161007,
/* 5562 */ 9999,
/* 5563 */ 9999,
/* 5564 */ 9999,
/* 5565 */ 1162010,
/* 5566 */ 9999,
/* 5567 */ 9999,
/* 5568 */ 9999,
/* 5569 */ 1163007,
/* 5570 */ 9999,
/* 5571 */ 1167007,
/* 5572 */ 9999,
/* 5573 */ 1167012,
/* 5574 */ 9999,
/* 5575 */ 1167012,
/* 5576 */ 9999,
/* 5577 */ 9999,
/* 5578 */ 9999,
/* 5579 */ 1168019,
/* 5580 */ 9999,
/* 5581 */ 9999,
/* 5582 */ 1168007,
/* 5583 */ 9999,
/* 5584 */ 1168007,
/* 5585 */ 9999,
/* 5586 */ 9999,
/* 5587 */ 1169011,
/* 5588 */ 9999,
/* 5589 */ 1169007,
/* 5590 */ 9999,
/* 5591 */ 1169007,
/* 5592 */ 9999,
/* 5593 */ 9999,
/* 5594 */ 1170011,
/* 5595 */ 9999,
/* 5596 */ 1170007,
/* 5597 */ 9999,
/* 5598 */ 1170007,
/* 5599 */ 9999,
/* 5600 */ 1171007,
/* 5601 */ 1171017,
/* 5602 */ 1171026,
/* 5603 */ 1171012,
/* 5604 */ 9999,
/* 5605 */ 1171012,
/* 5606 */ 9999,
/* 5607 */ 1174007,
/* 5608 */ 1174012,
/* 5609 */ 9999,
/* 5610 */ 1174012,
/* 5611 */ 9999,
/* 5612 */ 1177007,
/* 5613 */ 9999,
/* 5614 */ 1177012,
/* 5615 */ 9999,
/* 5616 */ 1177012,
/* 5617 */ 9999,
/* 5618 */ 1180007,
/* 5619 */ 1180015,
/* 5620 */ 9999,
/* 5621 */ 1180015,
/* 5622 */ 9999,
/* 5623 */ 9999,
/* 5624 */ 1180015,
/* 5625 */ 9999,
/* 5626 */ 9999,
/* 5627 */ 9999,
/* 5628 */ 1171018,
/* 5629 */ 9999,
/* 5630 */ 9999,
/* 5631 */ 9999,
/* 5632 */ 1171023,
/* 5633 */ 9999,
/* 5634 */ 1186007,
/* 5635 */ 9999,
/* 5636 */ 1186013,
/* 5637 */ 9999,
/* 5638 */ 1187007,
/* 5639 */ 9999,
/* 5640 */ 1187012,
/* 5641 */ 9999,
/* 5642 */ 9999,
/* 5643 */ 1188016,
/* 5644 */ 9999,
/* 5645 */ 1188014,
/* 5646 */ 9999,
/* 5647 */ 1192007,
/* 5648 */ 9999,
/* 5649 */ 1192013,
/* 5650 */ 9999,
/* 5651 */ 1193007,
/* 5652 */ 9999,
/* 5653 */ 1193012,
/* 5654 */ 9999,
/* 5655 */ 9999,
/* 5656 */ 1194016,
/* 5657 */ 9999,
/* 5658 */ 1194014,
/* 5659 */ 9999,
/* 5660 */ 9999,
/* 5661 */ 1194014,
/* 5662 */ 9999,
/* 5663 */ 9999,
/* 5664 */ 1198009,
/* 5665 */ 1199006,
/* 5666 */ 9999,
/* 5667 */ 9999,
/* 5668 */ 1198005,
/* 5669 */ 9999,
/* 5670 */ 9999,
/* 5671 */ 1198005,
/* 5672 */ 9999,
/* 5673 */ 1199008,
/* 5674 */ 1199018,
/* 5675 */ 1199008,
/* 5676 */ 9999,
/* 5677 */ 1199008,
/* 5678 */ 9999,
/* 5679 */ 9999,
/* 5680 */ 1199008,
/* 5681 */ 9999,
/* 5682 */ 9999,
/* 5683 */ 9999,
/* 5684 */ 1199009,
/* 5685 */ 9999,
/* 5686 */ 9999,
/* 5687 */ 9999,
/* 5688 */ 1199013,
/* 5689 */ 9999,
/* 5690 */ 9999,
/* 5691 */ 9999,
/* 5692 */ 9999,
/* 5693 */ 1206031,
/* 5694 */ 9999,
/* 5695 */ 1209007,
/* 5696 */ 9999,
/* 5697 */ 9999,
/* 5698 */ 1206011,
/* 5699 */ 9999,
/* 5700 */ 1209008,
/* 5701 */ 1209022,
/* 5702 */ 9999,
/* 5703 */ 1209022,
/* 5704 */ 9999,
/* 5705 */ 9999,
/* 5706 */ 1209022,
/* 5707 */ 9999,
/* 5708 */ 9999,
/* 5709 */ 1213012,
/* 5710 */ 1213019,
/* 5711 */ 9999,
/* 5712 */ 9999,
/* 5713 */ 1213009,
/* 5714 */ 9999,
/* 5715 */ 9999,
/* 5716 */ 1214011,
/* 5717 */ 1214019,
/* 5718 */ 9999,
/* 5719 */ 9999,
/* 5720 */ 1214009,
/* 5721 */ 9999,
/* 5722 */ 1215005,
/* 5723 */ 9999,
/* 5724 */ 1215014,
/* 5725 */ 9999,
/* 5726 */ 1216005,
/* 5727 */ 9999,
/* 5728 */ 1216016,
/* 5729 */ 9999,
/* 5730 */ 1217005,
/* 5731 */ 9999,
/* 5732 */ 1217017,
/* 5733 */ 9999,
/* 5734 */ 9999,
/* 5735 */ 9999,
/* 5736 */ 1213013,
/* 5737 */ 9999,
/* 5738 */ 9999,
/* 5739 */ 1213013,
/* 5740 */ 9999,
/* 5741 */ 9999,
/* 5742 */ 9999,
/* 5743 */ 1214012,
/* 5744 */ 9999,
/* 5745 */ 9999,
/* 5746 */ 1214012,
/* 5747 */ 9999,
/* 5748 */ 1214020,
/* 5749 */ 1214030,
/* 5750 */ 1214025,
/* 5751 */ 9999,
/* 5752 */ 1214025,
/* 5753 */ 9999,
/* 5754 */ 9999,
/* 5755 */ 1214025,
/* 5756 */ 9999,
/* 5757 */ 9999,
/* 5758 */ 9999,
/* 5759 */ 1214031,
/* 5760 */ 9999,
/* 5761 */ 9999,
/* 5762 */ 9999,
/* 5763 */ 1214037,
/* 5764 */ 9999,
/* 5765 */ 9999,
/* 5766 */ 1214037,
/* 5767 */ 9999,
/* 5768 */ 9999,
/* 5769 */ 9999,
/* 5770 */ 1224004,
/* 5771 */ 9999,
/* 5772 */ 9999,
/* 5773 */ 1221008,
/* 5774 */ 9999,
/* 5775 */ 9999,
/* 5776 */ 1224013,
/* 5777 */ 9999,
/* 5778 */ 1224013,
/* 5779 */ 9999,
/* 5780 */ 1225007,
/* 5781 */ 1225013,
/* 5782 */ 9999,
/* 5783 */ 1225013,
/* 5784 */ 9999,
/* 5785 */ 9999,
/* 5786 */ 1226007,
/* 5787 */ 9999,
/* 5788 */ 1226007,
/* 5789 */ 9999,
/* 5790 */ 9999,
/* 5791 */ 1227007,
/* 5792 */ 9999,
/* 5793 */ 1227007,
/* 5794 */ 9999,
/* 5795 */ 9999,
/* 5796 */ 1228007,
/* 5797 */ 9999,
/* 5798 */ 1228007,
/* 5799 */ 9999,
/* 5800 */ 9999,
/* 5801 */ 1229007,
/* 5802 */ 9999,
/* 5803 */ 1229007,
/* 5804 */ 9999,
/* 5805 */ 9999,
/* 5806 */ 1230011,
/* 5807 */ 9999,
/* 5808 */ 9999,
/* 5809 */ 1230007,
/* 5810 */ 9999,
/* 5811 */ 1230007,
/* 5812 */ 9999,
/* 5813 */ 9999,
/* 5814 */ 1233011,
/* 5815 */ 9999,
/* 5816 */ 1233007,
/* 5817 */ 9999,
/* 5818 */ 1233007,
/* 5819 */ 9999,
/* 5820 */ 9999,
/* 5821 */ 1233007,
/* 5822 */ 9999,
/* 5823 */ 9999,
/* 5824 */ 9999,
/* 5825 */ 1241018,
/* 5826 */ 9999,
/* 5827 */ 9999,
/* 5828 */ 9999,
/* 5829 */ 1244008,
/* 5830 */ 9999,
/* 5831 */ 9999,
/* 5832 */ 9999,
/* 5833 */ 1245008,
/* 5834 */ 9999,
/* 5835 */ 9999,
/* 5836 */ 9999,
/* 5837 */ 1246009,
/* 5838 */ 9999,
/* 5839 */ 9999,
/* 5840 */ 9999,
/* 5841 */ 1247008,
/* 5842 */ 9999,
/* 5843 */ 9999,
/* 5844 */ 9999,
/* 5845 */ 1248009,
/* 5846 */ 9999,
/* 5847 */ 9999,
/* 5848 */ 9999,
/* 5849 */ 1249009,
/* 5850 */ 9999,
/* 5851 */ 9999,
/* 5852 */ 9999,
/* 5853 */ 1250008,
/* 5854 */ 9999,
/* 5855 */ 9999,
/* 5856 */ 9999,
/* 5857 */ 1251009,
/* 5858 */ 9999,
/* 5859 */ 9999,
/* 5860 */ 9999,
/* 5861 */ 1252007,
/* 5862 */ 9999,
/* 5863 */ 9999,
/* 5864 */ 9999,
/* 5865 */ 1253010,
/* 5866 */ 9999,
/* 5867 */ 9999,
/* 5868 */ 9999,
/* 5869 */ 1254013,
/* 5870 */ 9999,
/* 5871 */ 9999,
/* 5872 */ 1259010,
/* 5873 */ 1259024,
/* 5874 */ 9999,
/* 5875 */ 9999,
/* 5876 */ 9999,
/* 5877 */ 1276003,
/* 5878 */ 1284004,
/* 5879 */ 9999,
/* 5880 */ 9999,
/* 5881 */ 9999,
/* 5882 */ 1258015,
/* 5883 */ 9999,
/* 5884 */ 1276005,
/* 5885 */ 1276021,
/* 5886 */ 1278005,
/* 5887 */ 1278025,
/* 5888 */ 1280005,
/* 5889 */ 9999,
/* 5890 */ 1276013,
/* 5891 */ 9999,
/* 5892 */ 9999,
/* 5893 */ 1276013,
/* 5894 */ 9999,
/* 5895 */ 9999,
/* 5896 */ 1276026,
/* 5897 */ 9999,
/* 5898 */ 1276022,
/* 5899 */ 9999,
/* 5900 */ 9999,
/* 5901 */ 1276022,
/* 5902 */ 9999,
/* 5903 */ 9999,
/* 5904 */ 9999,
/* 5905 */ 1278010,
/* 5906 */ 9999,
/* 5907 */ 9999,
/* 5908 */ 1278010,
/* 5909 */ 9999,
/* 5910 */ 9999,
/* 5911 */ 1278031,
/* 5912 */ 9999,
/* 5913 */ 1278027,
/* 5914 */ 9999,
/* 5915 */ 9999,
/* 5916 */ 1278027,
/* 5917 */ 9999,
/* 5918 */ 9999,
/* 5919 */ 1280010,
/* 5920 */ 1280026,
/* 5921 */ 1282007,
/* 5922 */ 1282027,
/* 5923 */ 1280006,
/* 5924 */ 9999,
/* 5925 */ 1280006,
/* 5926 */ 9999,
/* 5927 */ 9999,
/* 5928 */ 1280006,
/* 5929 */ 9999,
/* 5930 */ 9999,
/* 5931 */ 1280031,
/* 5932 */ 9999,
/* 5933 */ 1280027,
/* 5934 */ 9999,
/* 5935 */ 9999,
/* 5936 */ 1280027,
/* 5937 */ 9999,
/* 5938 */ 9999,
/* 5939 */ 9999,
/* 5940 */ 1282012,
/* 5941 */ 9999,
/* 5942 */ 9999,
/* 5943 */ 1282012,
/* 5944 */ 9999,
/* 5945 */ 9999,
/* 5946 */ 1282033,
/* 5947 */ 9999,
/* 5948 */ 1282029,
/* 5949 */ 9999,
/* 5950 */ 9999,
/* 5951 */ 1282029,
/* 5952 */ 9999,
/* 5953 */ 9999,
/* 5954 */ 9999,
/* 5955 */ 9999,
/* 5956 */ 1284005,
/* 5957 */ 9999,
/* 5958 */ 9999,
/* 5959 */ 1284005,
/* 5960 */ 9999,
/* 5961 */ 9999,
/* 5962 */ 1288010,
/* 5963 */ 9999,
/* 5964 */ 9999,
/* 5965 */ 1295003,
/* 5966 */ 1303004,
/* 5967 */ 9999,
/* 5968 */ 9999,
/* 5969 */ 9999,
/* 5970 */ 1287016,
/* 5971 */ 9999,
/* 5972 */ 1295005,
/* 5973 */ 1295021,
/* 5974 */ 1297005,
/* 5975 */ 1297025,
/* 5976 */ 1299005,
/* 5977 */ 9999,
/* 5978 */ 1295013,
/* 5979 */ 9999,
/* 5980 */ 9999,
/* 5981 */ 1295013,
/* 5982 */ 9999,
/* 5983 */ 9999,
/* 5984 */ 1295026,
/* 5985 */ 9999,
/* 5986 */ 1295022,
/* 5987 */ 9999,
/* 5988 */ 9999,
/* 5989 */ 1295022,
/* 5990 */ 9999,
/* 5991 */ 9999,
/* 5992 */ 9999,
/* 5993 */ 1297010,
/* 5994 */ 9999,
/* 5995 */ 9999,
/* 5996 */ 1297010,
/* 5997 */ 9999,
/* 5998 */ 9999,
/* 5999 */ 1297031,
/* 6000 */ 9999,
/* 6001 */ 1297027,
/* 6002 */ 9999,
/* 6003 */ 9999,
/* 6004 */ 1297027,
/* 6005 */ 9999,
/* 6006 */ 9999,
/* 6007 */ 1299010,
/* 6008 */ 1299026,
/* 6009 */ 1301007,
/* 6010 */ 1301027,
/* 6011 */ 1299006,
/* 6012 */ 9999,
/* 6013 */ 1299006,
/* 6014 */ 9999,
/* 6015 */ 9999,
/* 6016 */ 1299006,
/* 6017 */ 9999,
/* 6018 */ 9999,
/* 6019 */ 1299031,
/* 6020 */ 9999,
/* 6021 */ 1299027,
/* 6022 */ 9999,
/* 6023 */ 9999,
/* 6024 */ 1299027,
/* 6025 */ 9999,
/* 6026 */ 9999,
/* 6027 */ 9999,
/* 6028 */ 1301012,
/* 6029 */ 9999,
/* 6030 */ 9999,
/* 6031 */ 1301012,
/* 6032 */ 9999,
/* 6033 */ 9999,
/* 6034 */ 1301033,
/* 6035 */ 9999,
/* 6036 */ 1301029,
/* 6037 */ 9999,
/* 6038 */ 9999,
/* 6039 */ 1301029,
/* 6040 */ 9999,
/* 6041 */ 9999,
/* 6042 */ 9999,
/* 6043 */ 9999,
/* 6044 */ 1303005,
/* 6045 */ 9999,
/* 6046 */ 9999,
/* 6047 */ 1303005,
/* 6048 */ 9999,
/* 6049 */ 9999,
/* 6050 */ 1307010,
/* 6051 */ 1307024,
/* 6052 */ 9999,
/* 6053 */ 9999,
/* 6054 */ 9999,
/* 6055 */ 9999,
/* 6056 */ 1306016,
/* 6057 */ 9999,
/* 6058 */ 9999,
/* 6059 */ 1340010,
/* 6060 */ 9999,
/* 6061 */ 9999,
/* 6062 */ 1340040,
/* 6063 */ 9999,
/* 6064 */ 9999,
/* 6065 */ 1339016,
/* 6066 */ 9999,
/* 6067 */ 1356003,
/* 6068 */ 9999,
/* 6069 */ 1371003,
/* 6070 */ 9999,
/* 6071 */ 9999,
/* 6072 */ 1355016,
/* 6073 */ 9999,
/* 6074 */ 9999,
/* 6075 */ 9999,
/* 6076 */ 1371004,
/* 6077 */ 9999,
/* 6078 */ 1371004,
/* 6079 */ 9999,
/* 6080 */ 9999,
/* 6081 */ 1371004,
/* 6082 */ 9999,
/* 6083 */ 1386003,
/* 6084 */ 9999,
/* 6085 */ 1403003,
/* 6086 */ 9999,
/* 6087 */ 9999,
/* 6088 */ 1385029,
/* 6089 */ 9999,
/* 6090 */ 9999,
/* 6091 */ 9999,
/* 6092 */ 1403004,
/* 6093 */ 9999,
/* 6094 */ 1403004,
/* 6095 */ 9999,
/* 6096 */ 9999,
/* 6097 */ 1403004,
/* 6098 */ 9999,
/* 6099 */ 1423003,
/* 6100 */ 9999,
/* 6101 */ 1435003,
/* 6102 */ 9999,
/* 6103 */ 9999,
/* 6104 */ 1422016,
/* 6105 */ 9999,
/* 6106 */ 9999,
/* 6107 */ 9999,
/* 6108 */ 1435004,
/* 6109 */ 9999,
/* 6110 */ 1435004,
/* 6111 */ 9999,
/* 6112 */ 9999,
/* 6113 */ 1435004,
/* 6114 */ 9999,
/* 6115 */ 1449003,
/* 6116 */ 9999,
/* 6117 */ 9999,
/* 6118 */ 1449006,
/* 6119 */ 9999,
/* 6120 */ 9999,
/* 6121 */ 9999,
/* 6122 */ 1460012,
/* 6123 */ 9999,
/* 6124 */ 1460025,
/* 6125 */ 9999,
/* 6126 */ 1460045,
/* 6127 */ 9999,
/* 6128 */ 1460006,
/* 6129 */ 9999,
/* 6130 */ 9999,
/* 6131 */ 9999,
/* 6132 */ 1475012,
/* 6133 */ 9999,
/* 6134 */ 1475025,
/* 6135 */ 9999,
/* 6136 */ 1475006,
/* 6137 */ 9999,
/* 6138 */ 9999,
/* 6139 */ 9999,
/* 6140 */ 1484013,
/* 6141 */ 9999,
/* 6142 */ 1484027,
/* 6143 */ 9999,
/* 6144 */ 1484041,
/* 6145 */ 9999,
/* 6146 */ 1485007,
/* 6147 */ 9999,
/* 6148 */ 1484007,
/* 6149 */ 9999,
/* 6150 */ 9999,
/* 6151 */ 9999,
/* 6152 */ 1511016,
/* 6153 */ 9999,
/* 6154 */ 9999,
/* 6155 */ 1521005,
/* 6156 */ 1532005,
/* 6157 */ 9999,
/* 6158 */ 9999,
/* 6159 */ 1511010,
/* 6160 */ 9999,
/* 6161 */ 9999,
/* 6162 */ 9999,
/* 6163 */ 9999,
/* 6164 */ 1581009,
/* 6165 */ 9999,
/* 6166 */ 9999,
/* 6167 */ 9999,
/* 6168 */ 9999,
/* 6169 */ 1585012,
/* 6170 */ 9999,
/* 6171 */ 9999,
/* 6172 */ 1589008,
/* 6173 */ 9999,
/* 6174 */ 9999,
/* 6175 */ 1589033,
/* 6176 */ 9999,
/* 6177 */ 9999,
/* 6178 */ 9999,
/* 6179 */ 1589006,
/* 6180 */ 9999,
/* 6181 */ 9999,
/* 6182 */ 9999,
/* 6183 */ 1598015,
/* 6184 */ 9999,
/* 6185 */ 1598028,
/* 6186 */ 9999,
/* 6187 */ 1598009,
/* 6188 */ 9999,
/* 6189 */ 1609005,
/* 6190 */ 9999,
/* 6191 */ 1609009,
/* 6192 */ 9999,
/* 6193 */ 9999,
/* 6194 */ 1613012,
/* 6195 */ 9999,
/* 6196 */ 9999,
/* 6197 */ 1613010,
/* 6198 */ 9999,
/* 6199 */ 9999,
/* 6200 */ 9999,
/* 6201 */ 9999,
/* 6202 */ 1626010,
/* 6203 */ 9999,
/* 6204 */ 9999,
/* 6205 */ 1521012,
/* 6206 */ 9999,
/* 6207 */ 1523007,
/* 6208 */ 1521010,
/* 6209 */ 9999,
/* 6210 */ 1521010,
/* 6211 */ 9999,
/* 6212 */ 9999,
/* 6213 */ 1521010,
/* 6214 */ 9999,
/* 6215 */ 1521014,
/* 6216 */ 9999,
/* 6217 */ 1521018,
/* 6218 */ 9999,
/* 6219 */ 1521027,
/* 6220 */ 9999,
/* 6221 */ 1521033,
/* 6222 */ 9999,
/* 6223 */ 1523027,
/* 6224 */ 1523009,
/* 6225 */ 9999,
/* 6226 */ 1523009,
/* 6227 */ 9999,
/* 6228 */ 9999,
/* 6229 */ 1523009,
/* 6230 */ 9999,
/* 6231 */ 9999,
/* 6232 */ 9999,
/* 6233 */ 1533007,
/* 6234 */ 9999,
/* 6235 */ 1532013,
/* 6236 */ 9999,
/* 6237 */ 9999,
/* 6238 */ 1532013,
/* 6239 */ 9999,
/* 6240 */ 1533027,
/* 6241 */ 1533009,
/* 6242 */ 9999,
/* 6243 */ 1533009,
/* 6244 */ 9999,
/* 6245 */ 9999,
/* 6246 */ 1533009,
/* 6247 */ 9999,
/* 6248 */ 9999,
/* 6249 */ 1635007,
/* 6250 */ 9999,
/* 6251 */ 9999,
/* 6252 */ 1635003,
/* 6253 */ 9999,
/* 6254 */ 1635025,
/* 6255 */ 1635009,
/* 6256 */ 9999,
/* 6257 */ 1635009,
/* 6258 */ 9999,
/* 6259 */ 9999,
/* 6260 */ 1635009,
/* 6261 */ 9999,
/* 6262 */ 9999,
/* 6263 */ 1640007,
/* 6264 */ 1645007,
/* 6265 */ 9999,
/* 6266 */ 9999,
/* 6267 */ 1639004,
/* 6268 */ 9999,
/* 6269 */ 1640027,
/* 6270 */ 1640009,
/* 6271 */ 9999,
/* 6272 */ 1640009,
/* 6273 */ 9999,
/* 6274 */ 9999,
/* 6275 */ 1641018,
/* 6276 */ 1641016,
/* 6277 */ 9999,
/* 6278 */ 1641016,
/* 6279 */ 9999,
/* 6280 */ 9999,
/* 6281 */ 1641016,
/* 6282 */ 9999,
/* 6283 */ 1645025,
/* 6284 */ 1645009,
/* 6285 */ 9999,
/* 6286 */ 1645009,
/* 6287 */ 9999,
/* 6288 */ 9999,
/* 6289 */ 1645009,
/* 6290 */ 9999,
/* 6291 */ 9999,
/* 6292 */ 9999,
/* 6293 */ 1681010,
/* 6294 */ 9999,
/* 6295 */ 9999,
/* 6296 */ 9999,
/* 6297 */ 1683014,
/* 6298 */ 9999,
/* 6299 */ 9999,
/* 6300 */ 9999,
/* 6301 */ 1685011,
/* 6302 */ 9999,
/* 6303 */ 9999,
/* 6304 */ 9999,
/* 6305 */ 1687013,
/* 6306 */ 9999,
/* 6307 */ 9999,
/* 6308 */ 9999,
/* 6309 */ 1689014,
/* 6310 */ 9999,
/* 6311 */ 9999,
/* 6312 */ 9999,
/* 6313 */ 1691013,
/* 6314 */ 9999,
/* 6315 */ 1693005,
/* 6316 */ 9999,
/* 6317 */ 1693011,
/* 6318 */ 9999,
/* 6319 */ 9999,
/* 6320 */ 1695009,
/* 6321 */ 9999,
/* 6322 */ 9999,
/* 6323 */ 1695005,
/* 6324 */ 9999,
/* 6325 */ 9999,
/* 6326 */ 9999,
/* 6327 */ 1698009,
/* 6328 */ 9999,
/* 6329 */ 1739005,
/* 6330 */ 1739020,
/* 6331 */ 9999,
/* 6332 */ 9999,
/* 6333 */ 1739013,
/* 6334 */ 9999,
/* 6335 */ 1796005,
/* 6336 */ 1796020,
/* 6337 */ 9999,
/* 6338 */ 9999,
/* 6339 */ 1823009,
/* 6340 */ 9999,
/* 6341 */ 9999,
/* 6342 */ 1796013,
/* 6343 */ 9999,
/* 6344 */ 9999,
/* 6345 */ 9999,
/* 6346 */ 1858009,
/* 6347 */ 9999,
/* 6348 */ 9999,
/* 6349 */ 1851009,
/* 6350 */ 9999,
/* 6351 */ 1877005,
/* 6352 */ 9999,
/* 6353 */ 9999,
/* 6354 */ 9999,
/* 6355 */ 1877013,
/* 6356 */ 9999,
/* 6357 */ 9999,
/* 6358 */ 9999,
/* 6359 */ 1739021,
/* 6360 */ 9999,
/* 6361 */ 9999,
/* 6362 */ 9999,
/* 6363 */ 1739033,
/* 6364 */ 9999,
/* 6365 */ 9999,
/* 6366 */ 9999,
/* 6367 */ 1796021,
/* 6368 */ 9999,
/* 6369 */ 9999,
/* 6370 */ 9999,
/* 6371 */ 1796033,
/* 6372 */ 9999,
/* 6373 */ 9999,
/* 6374 */ 9999,
/* 6375 */ 1887007,
/* 6376 */ 9999,
/* 6377 */ 1932005,
/* 6378 */ 9999,
/* 6379 */ 9999,
/* 6380 */ 9999,
/* 6381 */ 1932010,
/* 6382 */ 9999,
/* 6383 */ 1938005,
/* 6384 */ 1938020,
/* 6385 */ 9999,
/* 6386 */ 9999,
/* 6387 */ 1938013,
/* 6388 */ 9999,
/* 6389 */ 9999,
/* 6390 */ 9999,
/* 6391 */ 1938021,
/* 6392 */ 9999,
/* 6393 */ 9999,
/* 6394 */ 9999,
/* 6395 */ 1938033,
/* 6396 */ 9999,
/* 6397 */ 1981005,
/* 6398 */ 9999,
/* 6399 */ 9999,
/* 6400 */ 1981010,
/* 6401 */ 9999,
/* 6402 */ 1983005,
/* 6403 */ 9999,
/* 6404 */ 9999,
/* 6405 */ 1983010,
/* 6406 */ 9999,
/* 6407 */ 9999,
/* 6408 */ 9999,
/* 6409 */ 1985018,
/* 6410 */ 9999,
/* 6411 */ 1985035,
/* 6412 */ 9999,
/* 6413 */ 9999,
/* 6414 */ 1985012,
/* 6415 */ 9999,
/* 6416 */ 9999,
/* 6417 */ 9999,
/* 6418 */ 1987018,
/* 6419 */ 9999,
/* 6420 */ 1987035,
/* 6421 */ 9999,
/* 6422 */ 9999,
/* 6423 */ 1987012,
/* 6424 */ 9999,
/* 6425 */ 1989005,
/* 6426 */ 9999,
/* 6427 */ 1989013,
/* 6428 */ 9999,
/* 6429 */ 9999,
/* 6430 */ 1994018,
/* 6431 */ 9999,
/* 6432 */ 1994016,
/* 6433 */ 9999,
/* 6434 */ 9999,
/* 6435 */ 1996018,
/* 6436 */ 9999,
/* 6437 */ 1996016,
/* 6438 */ 9999,
/* 6439 */ 9999,
/* 6440 */ 1998009,
/* 6441 */ 9999,
/* 6442 */ 1998005,
/* 6443 */ 9999,
/* 6444 */ 9999,
/* 6445 */ 2000009,
/* 6446 */ 9999,
/* 6447 */ 2000005,
/* 6448 */ 9999,
/* 6449 */ 9999,
/* 6450 */ 2002009,
/* 6451 */ 9999,
/* 6452 */ 2002005,
/* 6453 */ 9999,
/* 6454 */ 9999,
/* 6455 */ 2004009,
/* 6456 */ 9999,
/* 6457 */ 2004005,
/* 6458 */ 9999,
/* 6459 */ 9999,
/* 6460 */ 2006009,
/* 6461 */ 9999,
/* 6462 */ 9999,
/* 6463 */ 2006005,
/* 6464 */ 9999,
/* 6465 */ 2008005,
/* 6466 */ 9999,
/* 6467 */ 2008010,
/* 6468 */ 9999,
/* 6469 */ 2013005,
/* 6470 */ 9999,
/* 6471 */ 2013021,
/* 6472 */ 9999,
/* 6473 */ 2013010,
/* 6474 */ 9999,
/* 6475 */ 2015005,
/* 6476 */ 9999,
/* 6477 */ 2015010,
/* 6478 */ 9999,
/* 6479 */ 9999,
/* 6480 */ 2020009,
/* 6481 */ 9999,
/* 6482 */ 2020027,
/* 6483 */ 9999,
/* 6484 */ 2020005,
/* 6485 */ 9999,
/* 6486 */ 2022005,
/* 6487 */ 9999,
/* 6488 */ 2022010,
/* 6489 */ 9999,
/* 6490 */ 2028005,
/* 6491 */ 9999,
/* 6492 */ 2028021,
/* 6493 */ 9999,
/* 6494 */ 2028010,
/* 6495 */ 9999,
/* 6496 */ 2030005,
/* 6497 */ 9999,
/* 6498 */ 2030021,
/* 6499 */ 9999,
/* 6500 */ 2030010,
/* 6501 */ 9999,
/* 6502 */ 2032005,
/* 6503 */ 9999,
/* 6504 */ 2032021,
/* 6505 */ 9999,
/* 6506 */ 2032010,
/* 6507 */ 9999,
/* 6508 */ 2034005,
/* 6509 */ 9999,
/* 6510 */ 2034010,
/* 6511 */ 9999,
/* 6512 */ 2039005,
/* 6513 */ 9999,
/* 6514 */ 2039021,
/* 6515 */ 9999,
/* 6516 */ 2039010,
/* 6517 */ 9999,
/* 6518 */ 2041005,
/* 6519 */ 9999,
/* 6520 */ 2041021,
/* 6521 */ 9999,
/* 6522 */ 2041010,
/* 6523 */ 9999,
/* 6524 */ 2043005,
/* 6525 */ 9999,
/* 6526 */ 2043010,
/* 6527 */ 9999,
/* 6528 */ 2048005,
/* 6529 */ 9999,
/* 6530 */ 2048027,
/* 6531 */ 9999,
/* 6532 */ 2048010,
/* 6533 */ 9999,
/* 6534 */ 2050005,
/* 6535 */ 9999,
/* 6536 */ 2050027,
/* 6537 */ 9999,
/* 6538 */ 2050010,
/* 6539 */ 9999,
/* 6540 */ 2052005,
/* 6541 */ 9999,
/* 6542 */ 2052010,
/* 6543 */ 9999,
/* 6544 */ 2061005,
/* 6545 */ 9999,
/* 6546 */ 2061021,
/* 6547 */ 9999,
/* 6548 */ 2061010,
/* 6549 */ 9999,
/* 6550 */ 2063005,
/* 6551 */ 9999,
/* 6552 */ 2063021,
/* 6553 */ 9999,
/* 6554 */ 2063010,
/* 6555 */ 9999,
/* 6556 */ 2065005,
/* 6557 */ 9999,
/* 6558 */ 2065023,
/* 6559 */ 9999,
/* 6560 */ 2065010,
/* 6561 */ 9999,
/* 6562 */ 2067005,
/* 6563 */ 9999,
/* 6564 */ 2067023,
/* 6565 */ 9999,
/* 6566 */ 2067010,
/* 6567 */ 9999,
/* 6568 */ 2069005,
/* 6569 */ 9999,
/* 6570 */ 2069023,
/* 6571 */ 9999,
/* 6572 */ 2069010,
/* 6573 */ 9999,
/* 6574 */ 2071005,
/* 6575 */ 9999,
/* 6576 */ 2071023,
/* 6577 */ 9999,
/* 6578 */ 2071010,
/* 6579 */ 9999,
/* 6580 */ 2073005,
/* 6581 */ 9999,
/* 6582 */ 2073010,
/* 6583 */ 9999,
/* 6584 */ 2078005,
/* 6585 */ 9999,
/* 6586 */ 2078021,
/* 6587 */ 9999,
/* 6588 */ 2078010,
/* 6589 */ 9999,
/* 6590 */ 2080005,
/* 6591 */ 9999,
/* 6592 */ 2080010,
/* 6593 */ 9999,
/* 6594 */ 2085005,
/* 6595 */ 9999,
/* 6596 */ 2085022,
/* 6597 */ 9999,
/* 6598 */ 2085011,
/* 6599 */ 9999,
/* 6600 */ 2087005,
/* 6601 */ 9999,
/* 6602 */ 2087010,
/* 6603 */ 9999,
/* 6604 */ 2092005,
/* 6605 */ 9999,
/* 6606 */ 2092022,
/* 6607 */ 9999,
/* 6608 */ 2092011,
/* 6609 */ 9999,
/* 6610 */ 2094005,
/* 6611 */ 9999,
/* 6612 */ 2094011,
/* 6613 */ 9999,
/* 6614 */ 2099005,
/* 6615 */ 9999,
/* 6616 */ 2099025,
/* 6617 */ 9999,
/* 6618 */ 2099011,
/* 6619 */ 9999,
/* 6620 */ 2101005,
/* 6621 */ 9999,
/* 6622 */ 2101011,
/* 6623 */ 9999,
/* 6624 */ 2106005,
/* 6625 */ 9999,
/* 6626 */ 2106024,
/* 6627 */ 9999,
/* 6628 */ 2106011,
/* 6629 */ 9999,
/* 6630 */ 2108005,
/* 6631 */ 9999,
/* 6632 */ 2108011,
/* 6633 */ 9999,
/* 6634 */ 2113003,
/* 6635 */ 9999,
/* 6636 */ 2113020,
/* 6637 */ 9999,
/* 6638 */ 2113034,
/* 6639 */ 9999,
/* 6640 */ 2113009,
/* 6641 */ 9999,
/* 6642 */ 2163005,
/* 6643 */ 9999,
/* 6644 */ 2163011,
/* 6645 */ 9999,
/* 6646 */ 2171005,
/* 6647 */ 9999,
/* 6648 */ 2171020,
/* 6649 */ 9999,
/* 6650 */ 2171010,
/* 6651 */ 9999,
/* 6652 */ 2173005,
/* 6653 */ 9999,
/* 6654 */ 2173027,
/* 6655 */ 9999,
/* 6656 */ 2173010,
/* 6657 */ 9999,
/* 6658 */ 2175005,
/* 6659 */ 9999,
/* 6660 */ 2175027,
/* 6661 */ 9999,
/* 6662 */ 2175010,
/* 6663 */ 9999,
/* 6664 */ 2177005,
/* 6665 */ 9999,
/* 6666 */ 2177028,
/* 6667 */ 9999,
/* 6668 */ 2177010,
/* 6669 */ 9999,
/* 6670 */ 2179005,
/* 6671 */ 9999,
/* 6672 */ 2179027,
/* 6673 */ 9999,
/* 6674 */ 2179010,
/* 6675 */ 9999,
/* 6676 */ 2181005,
/* 6677 */ 9999,
/* 6678 */ 2181027,
/* 6679 */ 9999,
/* 6680 */ 2181010,
/* 6681 */ 9999,
/* 6682 */ 2183005,
/* 6683 */ 9999,
/* 6684 */ 2183027,
/* 6685 */ 9999,
/* 6686 */ 2183010,
/* 6687 */ 9999,
/* 6688 */ 2185005,
/* 6689 */ 9999,
/* 6690 */ 2185026,
/* 6691 */ 9999,
/* 6692 */ 2185010,
/* 6693 */ 9999,
/* 6694 */ 2187005,
/* 6695 */ 9999,
/* 6696 */ 2187030,
/* 6697 */ 9999,
/* 6698 */ 2187010,
/* 6699 */ 9999,
/* 6700 */ 2189005,
/* 6701 */ 9999,
/* 6702 */ 2189030,
/* 6703 */ 9999,
/* 6704 */ 2189010,
/* 6705 */ 9999,
/* 6706 */ 2191005,
/* 6707 */ 9999,
/* 6708 */ 2191027,
/* 6709 */ 9999,
/* 6710 */ 2191010,
/* 6711 */ 9999,
/* 6712 */ 2193005,
/* 6713 */ 9999,
/* 6714 */ 2193011,
/* 6715 */ 9999,
/* 6716 */ 2198003,
/* 6717 */ 9999,
/* 6718 */ 2198017,
/* 6719 */ 9999,
/* 6720 */ 2198006,
/* 6721 */ 9999,
/* 6722 */ 2211005,
/* 6723 */ 9999,
/* 6724 */ 2211011,
/* 6725 */ 9999,
/* 6726 */ 2222003,
/* 6727 */ 9999,
/* 6728 */ 2220003,
/* 6729 */ 9999,
/* 6730 */ 2222005,
/* 6731 */ 2249003,
/* 6732 */ 9999,
/* 6733 */ 2222011,
/* 6734 */ 9999,
/* 6735 */ 9999,
/* 6736 */ 2222011,
/* 6737 */ 9999,
/* 6738 */ 9999,
/* 6739 */ 2249009,
/* 6740 */ 2249005,
/* 6741 */ 9999,
/* 6742 */ 2249005,
/* 6743 */ 9999,
/* 6744 */ 9999,
/* 6745 */ 2249005,
0
};
/* only for BIGHASH (see art.c)
extern int DHITS[];
int DHITS[6747];
*/
int TABLE[1425][497];
void init_dirsets() {
TABLE[478][0] = 1;
TABLE[478][380] = 1;
TABLE[478][433] = 1;
TABLE[478][352] = 1;
TABLE[478][350] = 1;
TABLE[478][451] = 1;
TABLE[478][388] = 1;
TABLE[478][372] = 1;
TABLE[478][348] = 1;
TABLE[478][293] = 1;
TABLE[478][332] = 1;
TABLE[478][261] = 1;
TABLE[478][418] = 1;
TABLE[478][309] = 1;
TABLE[478][358] = 1;
TABLE[478][328] = 1;
TABLE[478][422] = 1;
TABLE[478][450] = 1;
TABLE[478][449] = 1;
TABLE[478][341] = 1;
TABLE[478][299] = 1;
TABLE[478][336] = 1;
TABLE[478][447] = 1;
TABLE[478][264] = 1;
TABLE[478][274] = 1;
TABLE[478][268] = 1;
TABLE[478][265] = 1;
TABLE[478][266] = 1;
TABLE[478][267] = 1;
TABLE[478][263] = 1;
TABLE[478][271] = 1;
TABLE[478][272] = 1;
TABLE[478][273] = 1;
TABLE[478][474] = 1;
TABLE[478][436] = 1;
TABLE[478][334] = 1;
TABLE[478][409] = 1;
TABLE[478][330] = 1;
TABLE[478][45] = 1;
TABLE[478][43] = 1;
TABLE[478][40] = 1;
TABLE[478][256] = 1;
TABLE[478][458] = 1;
TABLE[478][345] = 1;
TABLE[478][259] = 1;
TABLE[1][0] = 1;
TABLE[1][451] = 1;
TABLE[1][350] = 1;
TABLE[1][352] = 1;
TABLE[1][433] = 1;
TABLE[1][380] = 1;
TABLE[1][474] = 1;
TABLE[1][273] = 1;
TABLE[1][272] = 1;
TABLE[1][271] = 1;
TABLE[1][263] = 1;
TABLE[1][267] = 1;
TABLE[1][266] = 1;
TABLE[1][265] = 1;
TABLE[1][268] = 1;
TABLE[1][274] = 1;
TABLE[1][264] = 1;
TABLE[1][447] = 1;
TABLE[1][336] = 1;
TABLE[1][299] = 1;
TABLE[1][341] = 1;
TABLE[1][449] = 1;
TABLE[1][450] = 1;
TABLE[1][422] = 1;
TABLE[1][328] = 1;
TABLE[1][358] = 1;
TABLE[1][309] = 1;
TABLE[1][418] = 1;
TABLE[1][261] = 1;
TABLE[1][332] = 1;
TABLE[1][293] = 1;
TABLE[1][348] = 1;
TABLE[1][372] = 1;
TABLE[1][388] = 1;
TABLE[1][43] = 1;
TABLE[1][45] = 1;
TABLE[1][330] = 1;
TABLE[1][409] = 1;
TABLE[1][334] = 1;
TABLE[1][436] = 1;
TABLE[1][259] = 1;
TABLE[1][345] = 1;
TABLE[1][458] = 1;
TABLE[1][256] = 1;
TABLE[1][40] = 1;
TABLE[479][0] = 1;
TABLE[479][380] = 1;
TABLE[479][433] = 1;
TABLE[479][352] = 1;
TABLE[479][350] = 1;
TABLE[479][451] = 1;
TABLE[479][388] = 1;
TABLE[479][372] = 1;
TABLE[479][348] = 1;
TABLE[479][293] = 1;
TABLE[479][332] = 1;
TABLE[479][261] = 1;
TABLE[479][418] = 1;
TABLE[479][309] = 1;
TABLE[479][358] = 1;
TABLE[479][328] = 1;
TABLE[479][422] = 1;
TABLE[479][450] = 1;
TABLE[479][449] = 1;
TABLE[479][341] = 1;
TABLE[479][299] = 1;
TABLE[479][336] = 1;
TABLE[479][447] = 1;
TABLE[479][264] = 1;
TABLE[479][274] = 1;
TABLE[479][268] = 1;
TABLE[479][265] = 1;
TABLE[479][266] = 1;
TABLE[479][267] = 1;
TABLE[479][263] = 1;
TABLE[479][271] = 1;
TABLE[479][272] = 1;
TABLE[479][273] = 1;
TABLE[479][474] = 1;
TABLE[479][436] = 1;
TABLE[479][334] = 1;
TABLE[479][409] = 1;
TABLE[479][330] = 1;
TABLE[479][45] = 1;
TABLE[479][43] = 1;
TABLE[479][40] = 1;
TABLE[479][256] = 1;
TABLE[479][458] = 1;
TABLE[479][345] = 1;
TABLE[479][259] = 1;
TABLE[1023][0] = 1;
TABLE[2][380] = 1;
TABLE[3][433] = 1;
TABLE[4][352] = 1;
TABLE[5][388] = 1;
TABLE[6][372] = 1;
TABLE[7][348] = 1;
TABLE[8][293] = 1;
TABLE[9][332] = 1;
TABLE[10][261] = 1;
TABLE[11][451] = 1;
TABLE[11][350] = 1;
TABLE[11][352] = 1;
TABLE[11][433] = 1;
TABLE[11][380] = 1;
TABLE[11][474] = 1;
TABLE[11][273] = 1;
TABLE[11][272] = 1;
TABLE[11][271] = 1;
TABLE[11][263] = 1;
TABLE[11][267] = 1;
TABLE[11][266] = 1;
TABLE[11][265] = 1;
TABLE[11][268] = 1;
TABLE[11][274] = 1;
TABLE[11][264] = 1;
TABLE[11][447] = 1;
TABLE[11][336] = 1;
TABLE[11][299] = 1;
TABLE[11][341] = 1;
TABLE[11][449] = 1;
TABLE[11][450] = 1;
TABLE[11][422] = 1;
TABLE[11][328] = 1;
TABLE[11][358] = 1;
TABLE[11][309] = 1;
TABLE[11][418] = 1;
TABLE[11][261] = 1;
TABLE[11][332] = 1;
TABLE[11][293] = 1;
TABLE[11][348] = 1;
TABLE[11][372] = 1;
TABLE[11][388] = 1;
TABLE[11][0] = 1;
TABLE[11][43] = 1;
TABLE[11][45] = 1;
TABLE[11][330] = 1;
TABLE[11][409] = 1;
TABLE[11][334] = 1;
TABLE[11][436] = 1;
TABLE[11][259] = 1;
TABLE[11][345] = 1;
TABLE[11][458] = 1;
TABLE[11][256] = 1;
TABLE[11][40] = 1;
TABLE[12][418] = 1;
TABLE[13][309] = 1;
TABLE[14][358] = 1;
TABLE[15][328] = 1;
TABLE[16][422] = 1;
TABLE[17][450] = 1;
TABLE[18][449] = 1;
TABLE[19][341] = 1;
TABLE[20][299] = 1;
TABLE[21][336] = 1;
TABLE[22][261] = 1;
TABLE[23][447] = 1;
TABLE[24][447] = 1;
TABLE[25][474] = 1;
TABLE[25][273] = 1;
TABLE[25][272] = 1;
TABLE[25][271] = 1;
TABLE[25][263] = 1;
TABLE[25][267] = 1;
TABLE[25][266] = 1;
TABLE[25][265] = 1;
TABLE[25][268] = 1;
TABLE[25][274] = 1;
TABLE[25][264] = 1;
TABLE[26][447] = 1;
TABLE[27][447] = 1;
TABLE[28][350] = 1;
TABLE[29][451] = 1;
TABLE[480][58] = 1;
TABLE[481][326] = 1;
TABLE[1024][257] = 1;
TABLE[482][59] = 1;
TABLE[1025][125] = 1;
TABLE[30][36] = 1;
TABLE[31][36] = 1;
TABLE[32][43] = 1;
TABLE[32][45] = 1;
TABLE[32][259] = 1;
TABLE[32][345] = 1;
TABLE[32][458] = 1;
TABLE[32][256] = 1;
TABLE[32][40] = 1;
TABLE[33][43] = 1;
TABLE[33][45] = 1;
TABLE[33][259] = 1;
TABLE[33][345] = 1;
TABLE[33][458] = 1;
TABLE[33][256] = 1;
TABLE[33][40] = 1;
TABLE[34][45] = 1;
TABLE[34][43] = 1;
TABLE[34][40] = 1;
TABLE[34][256] = 1;
TABLE[34][458] = 1;
TABLE[34][345] = 1;
TABLE[34][259] = 1;
TABLE[35][45] = 1;
TABLE[35][43] = 1;
TABLE[35][40] = 1;
TABLE[35][256] = 1;
TABLE[35][458] = 1;
TABLE[35][345] = 1;
TABLE[35][259] = 1;
TABLE[36][45] = 1;
TABLE[36][43] = 1;
TABLE[36][40] = 1;
TABLE[36][256] = 1;
TABLE[36][458] = 1;
TABLE[36][345] = 1;
TABLE[36][259] = 1;
TABLE[37][45] = 1;
TABLE[37][43] = 1;
TABLE[37][40] = 1;
TABLE[37][256] = 1;
TABLE[37][458] = 1;
TABLE[37][345] = 1;
TABLE[37][259] = 1;
TABLE[38][45] = 1;
TABLE[38][43] = 1;
TABLE[38][40] = 1;
TABLE[38][256] = 1;
TABLE[38][458] = 1;
TABLE[38][345] = 1;
TABLE[38][259] = 1;
TABLE[39][45] = 1;
TABLE[39][43] = 1;
TABLE[39][40] = 1;
TABLE[39][256] = 1;
TABLE[39][458] = 1;
TABLE[39][345] = 1;
TABLE[39][259] = 1;
TABLE[40][43] = 1;
TABLE[40][45] = 1;
TABLE[40][259] = 1;
TABLE[40][345] = 1;
TABLE[40][458] = 1;
TABLE[40][256] = 1;
TABLE[40][40] = 1;
TABLE[41][43] = 1;
TABLE[42][45] = 1;
TABLE[43][40] = 1;
TABLE[43][256] = 1;
TABLE[43][458] = 1;
TABLE[43][345] = 1;
TABLE[43][259] = 1;
TABLE[44][40] = 1;
TABLE[45][256] = 1;
TABLE[46][458] = 1;
TABLE[47][345] = 1;
TABLE[48][259] = 1;
TABLE[49][40] = 1;
TABLE[49][256] = 1;
TABLE[49][458] = 1;
TABLE[49][345] = 1;
TABLE[49][259] = 1;
TABLE[50][40] = 1;
TABLE[50][256] = 1;
TABLE[50][458] = 1;
TABLE[50][345] = 1;
TABLE[50][259] = 1;
TABLE[51][40] = 1;
TABLE[51][256] = 1;
TABLE[51][458] = 1;
TABLE[51][345] = 1;
TABLE[51][259] = 1;
TABLE[52][43] = 1;
TABLE[52][45] = 1;
TABLE[52][259] = 1;
TABLE[52][345] = 1;
TABLE[52][458] = 1;
TABLE[52][256] = 1;
TABLE[52][40] = 1;
TABLE[53][40] = 1;
TABLE[53][256] = 1;
TABLE[53][458] = 1;
TABLE[53][345] = 1;
TABLE[53][259] = 1;
TABLE[54][40] = 1;
TABLE[54][256] = 1;
TABLE[54][458] = 1;
TABLE[54][345] = 1;
TABLE[54][259] = 1;
TABLE[55][36] = 1;
TABLE[55][419] = 1;
TABLE[55][317] = 1;
TABLE[55][300] = 1;
TABLE[55][40] = 1;
TABLE[55][458] = 1;
TABLE[55][345] = 1;
TABLE[55][259] = 1;
TABLE[55][256] = 1;
TABLE[55][367] = 1;
TABLE[483][367] = 1;
TABLE[1026][40] = 1;
TABLE[1026][256] = 1;
TABLE[1026][458] = 1;
TABLE[1026][345] = 1;
TABLE[1026][259] = 1;
TABLE[1026][317] = 1;
TABLE[1026][300] = 1;
TABLE[1026][36] = 1;
TABLE[1026][419] = 1;
TABLE[484][256] = 1;
TABLE[1027][40] = 1;
TABLE[1027][256] = 1;
TABLE[1027][458] = 1;
TABLE[1027][345] = 1;
TABLE[1027][259] = 1;
TABLE[1027][317] = 1;
TABLE[1027][300] = 1;
TABLE[1027][36] = 1;
TABLE[1027][419] = 1;
TABLE[485][259] = 1;
TABLE[485][345] = 1;
TABLE[485][458] = 1;
TABLE[485][256] = 1;
TABLE[485][40] = 1;
TABLE[1028][317] = 1;
TABLE[1028][300] = 1;
TABLE[1028][36] = 1;
TABLE[1028][419] = 1;
TABLE[486][317] = 1;
TABLE[487][300] = 1;
TABLE[1029][36] = 1;
TABLE[1029][419] = 1;
TABLE[488][36] = 1;
TABLE[489][419] = 1;
TABLE[490][43] = 1;
TABLE[491][45] = 1;
TABLE[1030][471] = 1;
TABLE[1030][399] = 1;
TABLE[1030][295] = 1;
TABLE[1030][59] = 1;
TABLE[1030][44] = 1;
TABLE[1030][335] = 1;
TABLE[494][471] = 1;
TABLE[495][399] = 1;
TABLE[496][399] = 1;
TABLE[497][399] = 1;
TABLE[498][399] = 1;
TABLE[499][295] = 1;
TABLE[1033][59] = 1;
TABLE[1033][44] = 1;
TABLE[1033][335] = 1;
TABLE[493][43] = 1;
TABLE[493][45] = 1;
TABLE[493][259] = 1;
TABLE[493][345] = 1;
TABLE[493][458] = 1;
TABLE[493][256] = 1;
TABLE[493][40] = 1;
TABLE[1032][93] = 1;
TABLE[492][44] = 1;
TABLE[1031][45] = 1;
TABLE[1031][43] = 1;
TABLE[1031][40] = 1;
TABLE[1031][256] = 1;
TABLE[1031][458] = 1;
TABLE[1031][345] = 1;
TABLE[1031][259] = 1;
TABLE[1031][93] = 1;
TABLE[56][356] = 1;
TABLE[500][58] = 1;
TABLE[503][44] = 1;
TABLE[503][335] = 1;
TABLE[503][367] = 1;
TABLE[503][256] = 1;
TABLE[503][259] = 1;
TABLE[503][345] = 1;
TABLE[503][458] = 1;
TABLE[503][40] = 1;
TABLE[503][300] = 1;
TABLE[503][317] = 1;
TABLE[503][419] = 1;
TABLE[503][36] = 1;
TABLE[1034][367] = 1;
TABLE[1034][256] = 1;
TABLE[1034][259] = 1;
TABLE[1034][345] = 1;
TABLE[1034][458] = 1;
TABLE[1034][40] = 1;
TABLE[1034][300] = 1;
TABLE[1034][317] = 1;
TABLE[1034][419] = 1;
TABLE[1034][36] = 1;
TABLE[1034][59] = 1;
TABLE[501][44] = 1;
TABLE[502][335] = 1;
TABLE[57][368] = 1;
TABLE[57][357] = 1;
TABLE[504][368] = 1;
TABLE[505][357] = 1;
TABLE[506][58] = 1;
TABLE[1035][320] = 1;
TABLE[1035][376] = 1;
TABLE[1035][303] = 1;
TABLE[1035][275] = 1;
TABLE[1035][259] = 1;
TABLE[1035][256] = 1;
TABLE[507][275] = 1;
TABLE[507][303] = 1;
TABLE[507][376] = 1;
TABLE[507][320] = 1;
TABLE[1036][259] = 1;
TABLE[1036][256] = 1;
TABLE[508][259] = 1;
TABLE[509][256] = 1;
TABLE[510][400] = 1;
TABLE[1037][59] = 1;
TABLE[58][259] = 1;
TABLE[58][256] = 1;
TABLE[58][320] = 1;
TABLE[58][376] = 1;
TABLE[58][303] = 1;
TABLE[58][275] = 1;
TABLE[511][275] = 1;
TABLE[511][303] = 1;
TABLE[511][376] = 1;
TABLE[511][320] = 1;
TABLE[1038][259] = 1;
TABLE[1038][256] = 1;
TABLE[512][259] = 1;
TABLE[513][256] = 1;
TABLE[514][400] = 1;
TABLE[1039][91] = 1;
TABLE[1039][471] = 1;
TABLE[1039][294] = 1;
TABLE[1039][44] = 1;
TABLE[1039][259] = 1;
TABLE[1039][59] = 1;
TABLE[59][400] = 1;
TABLE[515][44] = 1;
TABLE[1040][59] = 1;
TABLE[60][43] = 1;
TABLE[60][45] = 1;
TABLE[60][259] = 1;
TABLE[60][345] = 1;
TABLE[60][458] = 1;
TABLE[60][256] = 1;
TABLE[60][40] = 1;
TABLE[516][44] = 1;
TABLE[1041][257] = 1;
TABLE[1041][123] = 1;
TABLE[518][257] = 1;
TABLE[519][123] = 1;
TABLE[517][47] = 1;
TABLE[1042][44] = 1;
TABLE[1042][59] = 1;
TABLE[61][475] = 1;
TABLE[61][476] = 1;
TABLE[61][43] = 1;
TABLE[61][45] = 1;
TABLE[61][33] = 1;
TABLE[61][126] = 1;
TABLE[61][58] = 1;
TABLE[61][384] = 1;
TABLE[61][387] = 1;
TABLE[61][258] = 1;
TABLE[61][40] = 1;
TABLE[61][36] = 1;
TABLE[61][395] = 1;
TABLE[61][345] = 1;
TABLE[61][458] = 1;
TABLE[61][257] = 1;
TABLE[61][259] = 1;
TABLE[61][256] = 1;
TABLE[62][367] = 1;
TABLE[62][353] = 1;
TABLE[62][446] = 1;
TABLE[62][302] = 1;
TABLE[62][314] = 1;
TABLE[62][331] = 1;
TABLE[62][469] = 1;
TABLE[62][425] = 1;
TABLE[62][256] = 1;
TABLE[62][259] = 1;
TABLE[62][257] = 1;
TABLE[62][458] = 1;
TABLE[62][345] = 1;
TABLE[62][395] = 1;
TABLE[62][36] = 1;
TABLE[62][40] = 1;
TABLE[62][258] = 1;
TABLE[62][387] = 1;
TABLE[62][384] = 1;
TABLE[62][58] = 1;
TABLE[62][126] = 1;
TABLE[62][33] = 1;
TABLE[62][45] = 1;
TABLE[62][43] = 1;
TABLE[62][476] = 1;
TABLE[62][475] = 1;
TABLE[62][123] = 1;
TABLE[63][59] = 1;
TABLE[63][44] = 1;
TABLE[63][259] = 1;
TABLE[63][345] = 1;
TABLE[63][458] = 1;
TABLE[63][256] = 1;
TABLE[63][40] = 1;
TABLE[63][45] = 1;
TABLE[63][61] = 1;
TABLE[63][43] = 1;
TABLE[521][40] = 1;
TABLE[521][256] = 1;
TABLE[521][458] = 1;
TABLE[521][345] = 1;
TABLE[521][259] = 1;
TABLE[521][43] = 1;
TABLE[522][45] = 1;
TABLE[523][61] = 1;
TABLE[524][43] = 1;
TABLE[524][45] = 1;
TABLE[524][259] = 1;
TABLE[524][345] = 1;
TABLE[524][458] = 1;
TABLE[524][256] = 1;
TABLE[524][40] = 1;
TABLE[520][43] = 1;
TABLE[1043][40] = 1;
TABLE[1043][256] = 1;
TABLE[1043][458] = 1;
TABLE[1043][345] = 1;
TABLE[1043][259] = 1;
TABLE[528][40] = 1;
TABLE[1044][59] = 1;
TABLE[1044][44] = 1;
TABLE[525][387] = 1;
TABLE[526][384] = 1;
TABLE[527][391] = 1;
TABLE[64][388] = 1;
TABLE[530][44] = 1;
TABLE[1046][123] = 1;
TABLE[529][44] = 1;
TABLE[1045][123] = 1;
TABLE[531][125] = 1;
TABLE[531][326] = 1;
TABLE[531][325] = 1;
TABLE[531][450] = 1;
TABLE[531][364] = 1;
TABLE[531][365] = 1;
TABLE[531][366] = 1;
TABLE[531][440] = 1;
TABLE[531][424] = 1;
TABLE[531][369] = 1;
TABLE[531][296] = 1;
TABLE[531][294] = 1;
TABLE[531][323] = 1;
TABLE[531][292] = 1;
TABLE[531][381] = 1;
TABLE[531][346] = 1;
TABLE[531][350] = 1;
TABLE[531][431] = 1;
TABLE[531][435] = 1;
TABLE[531][389] = 1;
TABLE[531][437] = 1;
TABLE[531][330] = 1;
TABLE[531][409] = 1;
TABLE[531][356] = 1;
TABLE[531][400] = 1;
TABLE[531][307] = 1;
TABLE[531][374] = 1;
TABLE[531][43] = 1;
TABLE[531][368] = 1;
TABLE[531][357] = 1;
TABLE[531][277] = 1;
TABLE[531][434] = 1;
TABLE[531][377] = 1;
TABLE[531][311] = 1;
TABLE[531][264] = 1;
TABLE[531][274] = 1;
TABLE[531][268] = 1;
TABLE[531][265] = 1;
TABLE[531][266] = 1;
TABLE[531][267] = 1;
TABLE[531][263] = 1;
TABLE[531][271] = 1;
TABLE[531][272] = 1;
TABLE[531][273] = 1;
TABLE[531][474] = 1;
TABLE[1047][125] = 1;
TABLE[65][326] = 1;
TABLE[66][325] = 1;
TABLE[67][374] = 1;
TABLE[67][307] = 1;
TABLE[68][374] = 1;
TABLE[68][307] = 1;
TABLE[69][450] = 1;
TABLE[70][364] = 1;
TABLE[71][365] = 1;
TABLE[72][366] = 1;
TABLE[72][43] = 1;
TABLE[73][357] = 1;
TABLE[73][368] = 1;
TABLE[74][440] = 1;
TABLE[75][440] = 1;
TABLE[76][440] = 1;
TABLE[77][440] = 1;
TABLE[78][424] = 1;
TABLE[79][369] = 1;
TABLE[80][296] = 1;
TABLE[81][294] = 1;
TABLE[82][294] = 1;
TABLE[83][323] = 1;
TABLE[84][292] = 1;
TABLE[85][381] = 1;
TABLE[86][346] = 1;
TABLE[87][350] = 1;
TABLE[88][277] = 1;
TABLE[89][431] = 1;
TABLE[90][435] = 1;
TABLE[91][389] = 1;
TABLE[92][437] = 1;
TABLE[93][330] = 1;
TABLE[94][409] = 1;
TABLE[95][356] = 1;
TABLE[96][400] = 1;
TABLE[97][311] = 1;
TABLE[97][377] = 1;
TABLE[97][434] = 1;
TABLE[98][474] = 1;
TABLE[98][273] = 1;
TABLE[98][272] = 1;
TABLE[98][271] = 1;
TABLE[98][263] = 1;
TABLE[98][267] = 1;
TABLE[98][266] = 1;
TABLE[98][265] = 1;
TABLE[98][268] = 1;
TABLE[98][274] = 1;
TABLE[98][264] = 1;
TABLE[532][58] = 1;
TABLE[1048][257] = 1;
TABLE[533][58] = 1;
TABLE[1049][43] = 1;
TABLE[1049][45] = 1;
TABLE[1049][259] = 1;
TABLE[1049][345] = 1;
TABLE[1049][458] = 1;
TABLE[1049][256] = 1;
TABLE[1049][40] = 1;
TABLE[534][374] = 1;
TABLE[535][307] = 1;
TABLE[536][58] = 1;
TABLE[1050][43] = 1;
TABLE[1050][45] = 1;
TABLE[1050][259] = 1;
TABLE[1050][345] = 1;
TABLE[1050][458] = 1;
TABLE[1050][256] = 1;
TABLE[1050][40] = 1;
TABLE[537][374] = 1;
TABLE[538][307] = 1;
TABLE[539][58] = 1;
TABLE[1051][43] = 1;
TABLE[1051][45] = 1;
TABLE[1051][259] = 1;
TABLE[1051][345] = 1;
TABLE[1051][458] = 1;
TABLE[1051][256] = 1;
TABLE[1051][40] = 1;
TABLE[540][58] = 1;
TABLE[1052][43] = 1;
TABLE[1052][45] = 1;
TABLE[1052][259] = 1;
TABLE[1052][345] = 1;
TABLE[1052][458] = 1;
TABLE[1052][256] = 1;
TABLE[1052][40] = 1;
TABLE[541][44] = 1;
TABLE[1053][59] = 1;
TABLE[542][58] = 1;
TABLE[1054][43] = 1;
TABLE[1054][45] = 1;
TABLE[1054][259] = 1;
TABLE[1054][345] = 1;
TABLE[1054][458] = 1;
TABLE[1054][256] = 1;
TABLE[1054][40] = 1;
TABLE[543][58] = 1;
TABLE[1055][43] = 1;
TABLE[1055][45] = 1;
TABLE[1055][259] = 1;
TABLE[1055][345] = 1;
TABLE[1055][458] = 1;
TABLE[1055][256] = 1;
TABLE[1055][40] = 1;
TABLE[544][43] = 1;
TABLE[1056][366] = 1;
TABLE[545][58] = 1;
TABLE[1057][43] = 1;
TABLE[1057][45] = 1;
TABLE[1057][259] = 1;
TABLE[1057][345] = 1;
TABLE[1057][458] = 1;
TABLE[1057][256] = 1;
TABLE[1057][40] = 1;
TABLE[546][58] = 1;
TABLE[1058][45] = 1;
TABLE[1058][43] = 1;
TABLE[1058][40] = 1;
TABLE[1058][256] = 1;
TABLE[1058][458] = 1;
TABLE[1058][345] = 1;
TABLE[1058][259] = 1;
TABLE[1058][59] = 1;
TABLE[548][43] = 1;
TABLE[548][45] = 1;
TABLE[548][259] = 1;
TABLE[548][345] = 1;
TABLE[548][458] = 1;
TABLE[548][256] = 1;
TABLE[548][40] = 1;
TABLE[1060][59] = 1;
TABLE[547][44] = 1;
TABLE[1059][45] = 1;
TABLE[1059][43] = 1;
TABLE[1059][40] = 1;
TABLE[1059][256] = 1;
TABLE[1059][458] = 1;
TABLE[1059][345] = 1;
TABLE[1059][259] = 1;
TABLE[1059][59] = 1;
TABLE[549][58] = 1;
TABLE[1061][45] = 1;
TABLE[1061][43] = 1;
TABLE[1061][40] = 1;
TABLE[1061][256] = 1;
TABLE[1061][458] = 1;
TABLE[1061][345] = 1;
TABLE[1061][259] = 1;
TABLE[1061][59] = 1;
TABLE[551][43] = 1;
TABLE[551][45] = 1;
TABLE[551][259] = 1;
TABLE[551][345] = 1;
TABLE[551][458] = 1;
TABLE[551][256] = 1;
TABLE[551][40] = 1;
TABLE[1063][59] = 1;
TABLE[550][44] = 1;
TABLE[1062][45] = 1;
TABLE[1062][43] = 1;
TABLE[1062][40] = 1;
TABLE[1062][256] = 1;
TABLE[1062][458] = 1;
TABLE[1062][345] = 1;
TABLE[1062][259] = 1;
TABLE[1062][59] = 1;
TABLE[552][58] = 1;
TABLE[1064][45] = 1;
TABLE[1064][43] = 1;
TABLE[1064][40] = 1;
TABLE[1064][256] = 1;
TABLE[1064][458] = 1;
TABLE[1064][345] = 1;
TABLE[1064][259] = 1;
TABLE[553][44] = 1;
TABLE[1065][59] = 1;
TABLE[554][58] = 1;
TABLE[1066][43] = 1;
TABLE[1066][45] = 1;
TABLE[1066][259] = 1;
TABLE[1066][345] = 1;
TABLE[1066][458] = 1;
TABLE[1066][256] = 1;
TABLE[1066][40] = 1;
TABLE[555][58] = 1;
TABLE[1067][43] = 1;
TABLE[1067][45] = 1;
TABLE[1067][259] = 1;
TABLE[1067][345] = 1;
TABLE[1067][458] = 1;
TABLE[1067][256] = 1;
TABLE[1067][40] = 1;
TABLE[556][58] = 1;
TABLE[1068][43] = 1;
TABLE[1068][45] = 1;
TABLE[1068][259] = 1;
TABLE[1068][345] = 1;
TABLE[1068][458] = 1;
TABLE[1068][256] = 1;
TABLE[1068][40] = 1;
TABLE[557][58] = 1;
TABLE[1069][43] = 1;
TABLE[1069][45] = 1;
TABLE[1069][259] = 1;
TABLE[1069][345] = 1;
TABLE[1069][458] = 1;
TABLE[1069][256] = 1;
TABLE[1069][40] = 1;
TABLE[558][43] = 1;
TABLE[558][45] = 1;
TABLE[558][259] = 1;
TABLE[558][345] = 1;
TABLE[558][458] = 1;
TABLE[558][256] = 1;
TABLE[558][40] = 1;
TABLE[1070][43] = 1;
TABLE[1070][45] = 1;
TABLE[1070][259] = 1;
TABLE[1070][345] = 1;
TABLE[1070][458] = 1;
TABLE[1070][256] = 1;
TABLE[1070][40] = 1;
TABLE[559][58] = 1;
TABLE[1071][126] = 1;
TABLE[1071][43] = 1;
TABLE[1071][45] = 1;
TABLE[1071][259] = 1;
TABLE[1071][345] = 1;
TABLE[1071][458] = 1;
TABLE[1071][256] = 1;
TABLE[1071][40] = 1;
TABLE[1071][59] = 1;
TABLE[560][45] = 1;
TABLE[560][43] = 1;
TABLE[560][126] = 1;
TABLE[560][40] = 1;
TABLE[560][256] = 1;
TABLE[560][458] = 1;
TABLE[560][345] = 1;
TABLE[560][259] = 1;
TABLE[1072][59] = 1;
TABLE[561][44] = 1;
TABLE[1073][59] = 1;
TABLE[562][58] = 1;
TABLE[1074][43] = 1;
TABLE[1074][45] = 1;
TABLE[1074][259] = 1;
TABLE[1074][345] = 1;
TABLE[1074][458] = 1;
TABLE[1074][256] = 1;
TABLE[1074][40] = 1;
TABLE[563][58] = 1;
TABLE[1075][43] = 1;
TABLE[1075][45] = 1;
TABLE[1075][259] = 1;
TABLE[1075][345] = 1;
TABLE[1075][458] = 1;
TABLE[1075][256] = 1;
TABLE[1075][40] = 1;
TABLE[564][37] = 1;
TABLE[1076][59] = 1;
TABLE[565][58] = 1;
TABLE[1077][43] = 1;
TABLE[1077][45] = 1;
TABLE[1077][259] = 1;
TABLE[1077][345] = 1;
TABLE[1077][458] = 1;
TABLE[1077][256] = 1;
TABLE[1077][40] = 1;
TABLE[566][37] = 1;
TABLE[1078][59] = 1;
TABLE[567][58] = 1;
TABLE[1079][36] = 1;
TABLE[1079][59] = 1;
TABLE[569][36] = 1;
TABLE[1081][59] = 1;
TABLE[568][44] = 1;
TABLE[1080][36] = 1;
TABLE[1080][59] = 1;
TABLE[570][58] = 1;
TABLE[1082][36] = 1;
TABLE[1082][59] = 1;
TABLE[572][36] = 1;
TABLE[1084][59] = 1;
TABLE[571][44] = 1;
TABLE[1083][36] = 1;
TABLE[1083][59] = 1;
TABLE[573][58] = 1;
TABLE[1085][36] = 1;
TABLE[1085][59] = 1;
TABLE[575][36] = 1;
TABLE[1087][59] = 1;
TABLE[574][44] = 1;
TABLE[1086][36] = 1;
TABLE[1086][59] = 1;
TABLE[99][275] = 1;
TABLE[100][303] = 1;
TABLE[101][376] = 1;
TABLE[102][320] = 1;
TABLE[103][43] = 1;
TABLE[103][45] = 1;
TABLE[103][259] = 1;
TABLE[103][345] = 1;
TABLE[103][458] = 1;
TABLE[103][256] = 1;
TABLE[103][40] = 1;
TABLE[104][43] = 1;
TABLE[104][45] = 1;
TABLE[104][259] = 1;
TABLE[104][345] = 1;
TABLE[104][458] = 1;
TABLE[104][256] = 1;
TABLE[104][40] = 1;
TABLE[576][259] = 1;
TABLE[576][345] = 1;
TABLE[576][458] = 1;
TABLE[576][256] = 1;
TABLE[576][40] = 1;
TABLE[577][40] = 1;
TABLE[577][256] = 1;
TABLE[577][458] = 1;
TABLE[577][345] = 1;
TABLE[577][259] = 1;
TABLE[1088][259] = 1;
TABLE[1088][345] = 1;
TABLE[1088][458] = 1;
TABLE[1088][256] = 1;
TABLE[1088][319] = 1;
TABLE[1088][40] = 1;
TABLE[1088][288] = 1;
TABLE[1088][44] = 1;
TABLE[1088][59] = 1;
TABLE[578][40] = 1;
TABLE[578][256] = 1;
TABLE[578][458] = 1;
TABLE[578][345] = 1;
TABLE[578][259] = 1;
TABLE[1089][319] = 1;
TABLE[1089][40] = 1;
TABLE[1089][288] = 1;
TABLE[1089][44] = 1;
TABLE[1089][59] = 1;
TABLE[579][319] = 1;
TABLE[1090][40] = 1;
TABLE[1090][288] = 1;
TABLE[1090][44] = 1;
TABLE[1090][59] = 1;
TABLE[583][40] = 1;
TABLE[1091][288] = 1;
TABLE[1091][44] = 1;
TABLE[1091][59] = 1;
TABLE[580][322] = 1;
TABLE[581][307] = 1;
TABLE[582][374] = 1;
TABLE[591][288] = 1;
TABLE[1096][44] = 1;
TABLE[1096][59] = 1;
TABLE[584][259] = 1;
TABLE[584][345] = 1;
TABLE[584][458] = 1;
TABLE[584][256] = 1;
TABLE[584][40] = 1;
TABLE[1092][45] = 1;
TABLE[1092][43] = 1;
TABLE[1092][256] = 1;
TABLE[1092][458] = 1;
TABLE[1092][345] = 1;
TABLE[1092][259] = 1;
TABLE[1092][319] = 1;
TABLE[1092][40] = 1;
TABLE[1092][288] = 1;
TABLE[1092][44] = 1;
TABLE[1092][59] = 1;
TABLE[585][43] = 1;
TABLE[585][45] = 1;
TABLE[585][259] = 1;
TABLE[585][345] = 1;
TABLE[585][458] = 1;
TABLE[585][256] = 1;
TABLE[585][40] = 1;
TABLE[1093][319] = 1;
TABLE[1093][40] = 1;
TABLE[1093][288] = 1;
TABLE[1093][44] = 1;
TABLE[1093][59] = 1;
TABLE[586][319] = 1;
TABLE[1094][40] = 1;
TABLE[1094][288] = 1;
TABLE[1094][44] = 1;
TABLE[1094][59] = 1;
TABLE[590][40] = 1;
TABLE[1095][288] = 1;
TABLE[1095][44] = 1;
TABLE[1095][59] = 1;
TABLE[587][322] = 1;
TABLE[588][307] = 1;
TABLE[589][374] = 1;
TABLE[592][43] = 1;
TABLE[592][45] = 1;
TABLE[592][259] = 1;
TABLE[592][345] = 1;
TABLE[592][458] = 1;
TABLE[592][256] = 1;
TABLE[592][40] = 1;
TABLE[1097][44] = 1;
TABLE[1097][59] = 1;
TABLE[105][43] = 1;
TABLE[105][45] = 1;
TABLE[105][259] = 1;
TABLE[105][345] = 1;
TABLE[105][458] = 1;
TABLE[105][256] = 1;
TABLE[105][40] = 1;
TABLE[106][126] = 1;
TABLE[593][44] = 1;
TABLE[1098][126] = 1;
TABLE[1098][43] = 1;
TABLE[1098][45] = 1;
TABLE[1098][259] = 1;
TABLE[1098][345] = 1;
TABLE[1098][458] = 1;
TABLE[1098][256] = 1;
TABLE[1098][40] = 1;
TABLE[1098][59] = 1;
TABLE[594][44] = 1;
TABLE[1099][126] = 1;
TABLE[1099][43] = 1;
TABLE[1099][45] = 1;
TABLE[1099][259] = 1;
TABLE[1099][345] = 1;
TABLE[1099][458] = 1;
TABLE[1099][256] = 1;
TABLE[1099][40] = 1;
TABLE[1099][59] = 1;
TABLE[107][277] = 1;
TABLE[595][58] = 1;
TABLE[1100][43] = 1;
TABLE[1100][45] = 1;
TABLE[1100][259] = 1;
TABLE[1100][345] = 1;
TABLE[1100][458] = 1;
TABLE[1100][256] = 1;
TABLE[1100][40] = 1;
TABLE[108][372] = 1;
TABLE[596][326] = 1;
TABLE[596][374] = 1;
TABLE[596][430] = 1;
TABLE[596][373] = 1;
TABLE[596][316] = 1;
TABLE[596][452] = 1;
TABLE[596][284] = 1;
TABLE[596][435] = 1;
TABLE[596][404] = 1;
TABLE[596][420] = 1;
TABLE[596][383] = 1;
TABLE[596][315] = 1;
TABLE[596][466] = 1;
TABLE[596][431] = 1;
TABLE[596][396] = 1;
TABLE[596][323] = 1;
TABLE[596][312] = 1;
TABLE[596][407] = 1;
TABLE[596][292] = 1;
TABLE[596][365] = 1;
TABLE[596][437] = 1;
TABLE[596][349] = 1;
TABLE[596][350] = 1;
TABLE[596][361] = 1;
TABLE[596][304] = 1;
TABLE[596][468] = 1;
TABLE[596][467] = 1;
TABLE[596][379] = 1;
TABLE[596][456] = 1;
TABLE[596][313] = 1;
TABLE[596][376] = 1;
TABLE[596][375] = 1;
TABLE[596][355] = 1;
TABLE[596][400] = 1;
TABLE[596][368] = 1;
TABLE[596][357] = 1;
TABLE[596][434] = 1;
TABLE[596][377] = 1;
TABLE[596][311] = 1;
TABLE[596][264] = 1;
TABLE[596][274] = 1;
TABLE[596][268] = 1;
TABLE[596][265] = 1;
TABLE[596][266] = 1;
TABLE[596][267] = 1;
TABLE[596][263] = 1;
TABLE[596][271] = 1;
TABLE[596][272] = 1;
TABLE[596][273] = 1;
TABLE[596][474] = 1;
TABLE[1101][125] = 1;
TABLE[109][326] = 1;
TABLE[110][374] = 1;
TABLE[111][357] = 1;
TABLE[111][368] = 1;
TABLE[112][430] = 1;
TABLE[113][373] = 1;
TABLE[114][316] = 1;
TABLE[115][452] = 1;
TABLE[116][284] = 1;
TABLE[117][435] = 1;
TABLE[118][435] = 1;
TABLE[119][435] = 1;
TABLE[120][404] = 1;
TABLE[121][420] = 1;
TABLE[122][383] = 1;
TABLE[123][315] = 1;
TABLE[124][466] = 1;
TABLE[125][431] = 1;
TABLE[126][396] = 1;
TABLE[127][323] = 1;
TABLE[128][312] = 1;
TABLE[129][407] = 1;
TABLE[130][292] = 1;
TABLE[131][365] = 1;
TABLE[132][437] = 1;
TABLE[133][349] = 1;
TABLE[134][350] = 1;
TABLE[135][361] = 1;
TABLE[136][304] = 1;
TABLE[137][468] = 1;
TABLE[138][467] = 1;
TABLE[139][379] = 1;
TABLE[140][456] = 1;
TABLE[141][313] = 1;
TABLE[142][376] = 1;
TABLE[143][375] = 1;
TABLE[144][355] = 1;
TABLE[145][311] = 1;
TABLE[145][377] = 1;
TABLE[145][434] = 1;
TABLE[146][400] = 1;
TABLE[147][474] = 1;
TABLE[147][273] = 1;
TABLE[147][272] = 1;
TABLE[147][271] = 1;
TABLE[147][263] = 1;
TABLE[147][267] = 1;
TABLE[147][266] = 1;
TABLE[147][265] = 1;
TABLE[147][268] = 1;
TABLE[147][274] = 1;
TABLE[147][264] = 1;
TABLE[597][58] = 1;
TABLE[1102][257] = 1;
TABLE[598][58] = 1;
TABLE[1103][43] = 1;
TABLE[1103][45] = 1;
TABLE[1103][259] = 1;
TABLE[1103][345] = 1;
TABLE[1103][458] = 1;
TABLE[1103][256] = 1;
TABLE[1103][40] = 1;
TABLE[599][58] = 1;
TABLE[1104][259] = 1;
TABLE[1104][345] = 1;
TABLE[1104][458] = 1;
TABLE[1104][256] = 1;
TABLE[1104][40] = 1;
TABLE[1104][45] = 1;
TABLE[1104][43] = 1;
TABLE[600][58] = 1;
TABLE[1105][259] = 1;
TABLE[1105][345] = 1;
TABLE[1105][458] = 1;
TABLE[1105][256] = 1;
TABLE[1105][40] = 1;
TABLE[1105][45] = 1;
TABLE[1105][43] = 1;
TABLE[601][58] = 1;
TABLE[1106][43] = 1;
TABLE[1106][45] = 1;
TABLE[1106][259] = 1;
TABLE[1106][345] = 1;
TABLE[1106][458] = 1;
TABLE[1106][256] = 1;
TABLE[1106][40] = 1;
TABLE[602][58] = 1;
TABLE[1107][43] = 1;
TABLE[1107][45] = 1;
TABLE[1107][259] = 1;
TABLE[1107][345] = 1;
TABLE[1107][458] = 1;
TABLE[1107][256] = 1;
TABLE[1107][40] = 1;
TABLE[603][58] = 1;
TABLE[1108][43] = 1;
TABLE[1108][45] = 1;
TABLE[1108][259] = 1;
TABLE[1108][345] = 1;
TABLE[1108][458] = 1;
TABLE[1108][256] = 1;
TABLE[1108][40] = 1;
TABLE[604][58] = 1;
TABLE[1109][43] = 1;
TABLE[1109][45] = 1;
TABLE[1109][259] = 1;
TABLE[1109][345] = 1;
TABLE[1109][458] = 1;
TABLE[1109][256] = 1;
TABLE[1109][40] = 1;
TABLE[605][58] = 1;
TABLE[1110][40] = 1;
TABLE[1110][256] = 1;
TABLE[1110][458] = 1;
TABLE[1110][345] = 1;
TABLE[1110][259] = 1;
TABLE[606][58] = 1;
TABLE[1111][43] = 1;
TABLE[607][58] = 1;
TABLE[1112][43] = 1;
TABLE[1112][45] = 1;
TABLE[1112][259] = 1;
TABLE[1112][345] = 1;
TABLE[1112][458] = 1;
TABLE[1112][256] = 1;
TABLE[1112][40] = 1;
TABLE[608][58] = 1;
TABLE[1113][43] = 1;
TABLE[1113][45] = 1;
TABLE[1113][259] = 1;
TABLE[1113][345] = 1;
TABLE[1113][458] = 1;
TABLE[1113][256] = 1;
TABLE[1113][40] = 1;
TABLE[609][58] = 1;
TABLE[1114][43] = 1;
TABLE[1114][45] = 1;
TABLE[1114][259] = 1;
TABLE[1114][345] = 1;
TABLE[1114][458] = 1;
TABLE[1114][256] = 1;
TABLE[1114][40] = 1;
TABLE[610][58] = 1;
TABLE[1115][43] = 1;
TABLE[1115][45] = 1;
TABLE[1115][259] = 1;
TABLE[1115][345] = 1;
TABLE[1115][458] = 1;
TABLE[1115][256] = 1;
TABLE[1115][40] = 1;
TABLE[611][43] = 1;
TABLE[611][45] = 1;
TABLE[611][259] = 1;
TABLE[611][345] = 1;
TABLE[611][458] = 1;
TABLE[611][256] = 1;
TABLE[611][40] = 1;
TABLE[1116][59] = 1;
TABLE[612][58] = 1;
TABLE[1117][43] = 1;
TABLE[1117][45] = 1;
TABLE[1117][259] = 1;
TABLE[1117][345] = 1;
TABLE[1117][458] = 1;
TABLE[1117][256] = 1;
TABLE[1117][40] = 1;
TABLE[613][58] = 1;
TABLE[1118][43] = 1;
TABLE[1118][45] = 1;
TABLE[1118][259] = 1;
TABLE[1118][345] = 1;
TABLE[1118][458] = 1;
TABLE[1118][256] = 1;
TABLE[1118][40] = 1;
TABLE[614][58] = 1;
TABLE[1119][43] = 1;
TABLE[1119][45] = 1;
TABLE[1119][259] = 1;
TABLE[1119][345] = 1;
TABLE[1119][458] = 1;
TABLE[1119][256] = 1;
TABLE[1119][40] = 1;
TABLE[615][58] = 1;
TABLE[1120][43] = 1;
TABLE[1120][45] = 1;
TABLE[1120][259] = 1;
TABLE[1120][345] = 1;
TABLE[1120][458] = 1;
TABLE[1120][256] = 1;
TABLE[1120][40] = 1;
TABLE[616][58] = 1;
TABLE[1121][43] = 1;
TABLE[1121][45] = 1;
TABLE[1121][259] = 1;
TABLE[1121][345] = 1;
TABLE[1121][458] = 1;
TABLE[1121][256] = 1;
TABLE[1121][40] = 1;
TABLE[617][58] = 1;
TABLE[1122][43] = 1;
TABLE[1122][45] = 1;
TABLE[1122][259] = 1;
TABLE[1122][345] = 1;
TABLE[1122][458] = 1;
TABLE[1122][256] = 1;
TABLE[1122][40] = 1;
TABLE[618][58] = 1;
TABLE[1123][276] = 1;
TABLE[1123][43] = 1;
TABLE[1123][45] = 1;
TABLE[1123][259] = 1;
TABLE[1123][345] = 1;
TABLE[1123][458] = 1;
TABLE[1123][256] = 1;
TABLE[1123][40] = 1;
TABLE[1123][59] = 1;
TABLE[619][45] = 1;
TABLE[619][43] = 1;
TABLE[619][276] = 1;
TABLE[619][40] = 1;
TABLE[619][256] = 1;
TABLE[619][458] = 1;
TABLE[619][345] = 1;
TABLE[619][259] = 1;
TABLE[1124][59] = 1;
TABLE[620][58] = 1;
TABLE[1125][43] = 1;
TABLE[1125][45] = 1;
TABLE[1125][259] = 1;
TABLE[1125][345] = 1;
TABLE[1125][458] = 1;
TABLE[1125][256] = 1;
TABLE[1125][40] = 1;
TABLE[621][58] = 1;
TABLE[1126][36] = 1;
TABLE[1126][59] = 1;
TABLE[623][36] = 1;
TABLE[1128][59] = 1;
TABLE[622][44] = 1;
TABLE[1127][36] = 1;
TABLE[1127][59] = 1;
TABLE[624][58] = 1;
TABLE[1129][36] = 1;
TABLE[1129][59] = 1;
TABLE[626][36] = 1;
TABLE[1131][59] = 1;
TABLE[625][44] = 1;
TABLE[1130][36] = 1;
TABLE[1130][59] = 1;
TABLE[627][58] = 1;
TABLE[1132][126] = 1;
TABLE[1132][43] = 1;
TABLE[1132][45] = 1;
TABLE[1132][259] = 1;
TABLE[1132][345] = 1;
TABLE[1132][458] = 1;
TABLE[1132][256] = 1;
TABLE[1132][40] = 1;
TABLE[1132][59] = 1;
TABLE[628][45] = 1;
TABLE[628][43] = 1;
TABLE[628][126] = 1;
TABLE[628][40] = 1;
TABLE[628][256] = 1;
TABLE[628][458] = 1;
TABLE[628][345] = 1;
TABLE[628][259] = 1;
TABLE[1133][59] = 1;
TABLE[629][58] = 1;
TABLE[1134][45] = 1;
TABLE[1134][43] = 1;
TABLE[1134][40] = 1;
TABLE[1134][256] = 1;
TABLE[1134][458] = 1;
TABLE[1134][345] = 1;
TABLE[1134][259] = 1;
TABLE[1134][59] = 1;
TABLE[632][43] = 1;
TABLE[632][45] = 1;
TABLE[632][259] = 1;
TABLE[632][345] = 1;
TABLE[632][458] = 1;
TABLE[632][256] = 1;
TABLE[632][40] = 1;
TABLE[1136][59] = 1;
TABLE[630][44] = 1;
TABLE[631][124] = 1;
TABLE[1135][45] = 1;
TABLE[1135][43] = 1;
TABLE[1135][40] = 1;
TABLE[1135][256] = 1;
TABLE[1135][458] = 1;
TABLE[1135][345] = 1;
TABLE[1135][259] = 1;
TABLE[1135][59] = 1;
TABLE[633][58] = 1;
TABLE[1137][43] = 1;
TABLE[1137][45] = 1;
TABLE[1137][259] = 1;
TABLE[1137][345] = 1;
TABLE[1137][458] = 1;
TABLE[1137][256] = 1;
TABLE[1137][40] = 1;
TABLE[634][58] = 1;
TABLE[1138][43] = 1;
TABLE[1138][45] = 1;
TABLE[1138][259] = 1;
TABLE[1138][345] = 1;
TABLE[1138][458] = 1;
TABLE[1138][256] = 1;
TABLE[1138][40] = 1;
TABLE[635][58] = 1;
TABLE[1139][43] = 1;
TABLE[1139][45] = 1;
TABLE[1139][259] = 1;
TABLE[1139][345] = 1;
TABLE[1139][458] = 1;
TABLE[1139][256] = 1;
TABLE[1139][40] = 1;
TABLE[636][58] = 1;
TABLE[1140][43] = 1;
TABLE[1140][45] = 1;
TABLE[1140][259] = 1;
TABLE[1140][345] = 1;
TABLE[1140][458] = 1;
TABLE[1140][256] = 1;
TABLE[1140][40] = 1;
TABLE[637][58] = 1;
TABLE[1141][43] = 1;
TABLE[1141][45] = 1;
TABLE[1141][259] = 1;
TABLE[1141][345] = 1;
TABLE[1141][458] = 1;
TABLE[1141][256] = 1;
TABLE[1141][40] = 1;
TABLE[638][58] = 1;
TABLE[1142][43] = 1;
TABLE[1142][45] = 1;
TABLE[1142][259] = 1;
TABLE[1142][345] = 1;
TABLE[1142][458] = 1;
TABLE[1142][256] = 1;
TABLE[1142][40] = 1;
TABLE[639][58] = 1;
TABLE[1143][43] = 1;
TABLE[1143][45] = 1;
TABLE[1143][259] = 1;
TABLE[1143][345] = 1;
TABLE[1143][458] = 1;
TABLE[1143][256] = 1;
TABLE[1143][40] = 1;
TABLE[640][58] = 1;
TABLE[1144][43] = 1;
TABLE[1144][45] = 1;
TABLE[1144][259] = 1;
TABLE[1144][345] = 1;
TABLE[1144][458] = 1;
TABLE[1144][256] = 1;
TABLE[1144][40] = 1;
TABLE[641][58] = 1;
TABLE[1145][36] = 1;
TABLE[148][43] = 1;
TABLE[148][45] = 1;
TABLE[148][259] = 1;
TABLE[148][345] = 1;
TABLE[148][458] = 1;
TABLE[148][256] = 1;
TABLE[148][40] = 1;
TABLE[149][276] = 1;
TABLE[642][44] = 1;
TABLE[1146][276] = 1;
TABLE[1146][43] = 1;
TABLE[1146][45] = 1;
TABLE[1146][259] = 1;
TABLE[1146][345] = 1;
TABLE[1146][458] = 1;
TABLE[1146][256] = 1;
TABLE[1146][40] = 1;
TABLE[1146][59] = 1;
TABLE[643][44] = 1;
TABLE[1147][276] = 1;
TABLE[1147][43] = 1;
TABLE[1147][45] = 1;
TABLE[1147][259] = 1;
TABLE[1147][345] = 1;
TABLE[1147][458] = 1;
TABLE[1147][256] = 1;
TABLE[1147][40] = 1;
TABLE[1147][59] = 1;
TABLE[150][43] = 1;
TABLE[150][45] = 1;
TABLE[150][259] = 1;
TABLE[150][345] = 1;
TABLE[150][458] = 1;
TABLE[150][256] = 1;
TABLE[150][40] = 1;
TABLE[151][126] = 1;
TABLE[644][44] = 1;
TABLE[1148][126] = 1;
TABLE[1148][43] = 1;
TABLE[1148][45] = 1;
TABLE[1148][259] = 1;
TABLE[1148][345] = 1;
TABLE[1148][458] = 1;
TABLE[1148][256] = 1;
TABLE[1148][40] = 1;
TABLE[1148][59] = 1;
TABLE[645][44] = 1;
TABLE[1149][126] = 1;
TABLE[1149][43] = 1;
TABLE[1149][45] = 1;
TABLE[1149][259] = 1;
TABLE[1149][345] = 1;
TABLE[1149][458] = 1;
TABLE[1149][256] = 1;
TABLE[1149][40] = 1;
TABLE[1149][59] = 1;
TABLE[152][348] = 1;
TABLE[646][326] = 1;
TABLE[646][383] = 1;
TABLE[646][389] = 1;
TABLE[646][365] = 1;
TABLE[646][350] = 1;
TABLE[646][472] = 1;
TABLE[646][344] = 1;
TABLE[646][400] = 1;
TABLE[646][368] = 1;
TABLE[646][357] = 1;
TABLE[646][334] = 1;
TABLE[646][448] = 1;
TABLE[646][434] = 1;
TABLE[646][377] = 1;
TABLE[646][311] = 1;
TABLE[646][264] = 1;
TABLE[646][274] = 1;
TABLE[646][268] = 1;
TABLE[646][265] = 1;
TABLE[646][266] = 1;
TABLE[646][267] = 1;
TABLE[646][263] = 1;
TABLE[646][271] = 1;
TABLE[646][272] = 1;
TABLE[646][273] = 1;
TABLE[646][474] = 1;
TABLE[1150][125] = 1;
TABLE[153][326] = 1;
TABLE[154][383] = 1;
TABLE[155][357] = 1;
TABLE[155][368] = 1;
TABLE[156][389] = 1;
TABLE[157][365] = 1;
TABLE[158][350] = 1;
TABLE[159][448] = 1;
TABLE[159][334] = 1;
TABLE[160][472] = 1;
TABLE[161][344] = 1;
TABLE[162][400] = 1;
TABLE[163][311] = 1;
TABLE[163][377] = 1;
TABLE[163][434] = 1;
TABLE[164][474] = 1;
TABLE[164][273] = 1;
TABLE[164][272] = 1;
TABLE[164][271] = 1;
TABLE[164][263] = 1;
TABLE[164][267] = 1;
TABLE[164][266] = 1;
TABLE[164][265] = 1;
TABLE[164][268] = 1;
TABLE[164][274] = 1;
TABLE[164][264] = 1;
TABLE[647][58] = 1;
TABLE[1151][257] = 1;
TABLE[648][58] = 1;
TABLE[1152][43] = 1;
TABLE[1152][45] = 1;
TABLE[1152][259] = 1;
TABLE[1152][345] = 1;
TABLE[1152][458] = 1;
TABLE[1152][256] = 1;
TABLE[1152][40] = 1;
TABLE[649][58] = 1;
TABLE[1153][43] = 1;
TABLE[1153][45] = 1;
TABLE[1153][259] = 1;
TABLE[1153][345] = 1;
TABLE[1153][458] = 1;
TABLE[1153][256] = 1;
TABLE[1153][40] = 1;
TABLE[650][58] = 1;
TABLE[1154][43] = 1;
TABLE[1154][45] = 1;
TABLE[1154][259] = 1;
TABLE[1154][345] = 1;
TABLE[1154][458] = 1;
TABLE[1154][256] = 1;
TABLE[1154][40] = 1;
TABLE[651][58] = 1;
TABLE[1155][45] = 1;
TABLE[1155][43] = 1;
TABLE[1155][40] = 1;
TABLE[1155][256] = 1;
TABLE[1155][458] = 1;
TABLE[1155][345] = 1;
TABLE[1155][259] = 1;
TABLE[1155][59] = 1;
TABLE[654][43] = 1;
TABLE[654][45] = 1;
TABLE[654][259] = 1;
TABLE[654][345] = 1;
TABLE[654][458] = 1;
TABLE[654][256] = 1;
TABLE[654][40] = 1;
TABLE[1157][59] = 1;
TABLE[652][124] = 1;
TABLE[653][44] = 1;
TABLE[1156][45] = 1;
TABLE[1156][43] = 1;
TABLE[1156][40] = 1;
TABLE[1156][256] = 1;
TABLE[1156][458] = 1;
TABLE[1156][345] = 1;
TABLE[1156][259] = 1;
TABLE[1156][59] = 1;
TABLE[655][448] = 1;
TABLE[656][334] = 1;
TABLE[657][58] = 1;
TABLE[1158][36] = 1;
TABLE[658][58] = 1;
TABLE[1159][43] = 1;
TABLE[1159][45] = 1;
TABLE[1159][259] = 1;
TABLE[1159][345] = 1;
TABLE[1159][458] = 1;
TABLE[1159][256] = 1;
TABLE[1159][40] = 1;
TABLE[659][58] = 1;
TABLE[1160][259] = 1;
TABLE[1160][345] = 1;
TABLE[1160][458] = 1;
TABLE[1160][256] = 1;
TABLE[1160][40] = 1;
TABLE[1160][45] = 1;
TABLE[1160][43] = 1;
TABLE[165][436] = 1;
TABLE[165][334] = 1;
TABLE[165][409] = 1;
TABLE[165][330] = 1;
TABLE[165][45] = 1;
TABLE[165][43] = 1;
TABLE[165][40] = 1;
TABLE[165][256] = 1;
TABLE[165][458] = 1;
TABLE[165][345] = 1;
TABLE[165][259] = 1;
TABLE[661][43] = 1;
TABLE[661][45] = 1;
TABLE[661][259] = 1;
TABLE[661][345] = 1;
TABLE[661][458] = 1;
TABLE[661][256] = 1;
TABLE[661][40] = 1;
TABLE[1162][436] = 1;
TABLE[1162][334] = 1;
TABLE[1162][409] = 1;
TABLE[1162][330] = 1;
TABLE[660][47] = 1;
TABLE[1161][45] = 1;
TABLE[1161][43] = 1;
TABLE[1161][40] = 1;
TABLE[1161][256] = 1;
TABLE[1161][458] = 1;
TABLE[1161][345] = 1;
TABLE[1161][259] = 1;
TABLE[1161][436] = 1;
TABLE[1161][334] = 1;
TABLE[1161][409] = 1;
TABLE[1161][330] = 1;
TABLE[662][436] = 1;
TABLE[663][334] = 1;
TABLE[664][409] = 1;
TABLE[665][330] = 1;
TABLE[666][326] = 1;
TABLE[666][374] = 1;
TABLE[666][315] = 1;
TABLE[666][298] = 1;
TABLE[666][413] = 1;
TABLE[666][445] = 1;
TABLE[666][417] = 1;
TABLE[666][472] = 1;
TABLE[666][473] = 1;
TABLE[666][333] = 1;
TABLE[666][414] = 1;
TABLE[666][297] = 1;
TABLE[666][378] = 1;
TABLE[666][461] = 1;
TABLE[666][428] = 1;
TABLE[666][318] = 1;
TABLE[666][350] = 1;
TABLE[666][400] = 1;
TABLE[666][264] = 1;
TABLE[666][274] = 1;
TABLE[666][268] = 1;
TABLE[666][265] = 1;
TABLE[666][266] = 1;
TABLE[666][267] = 1;
TABLE[666][263] = 1;
TABLE[666][271] = 1;
TABLE[666][272] = 1;
TABLE[666][273] = 1;
TABLE[666][474] = 1;
TABLE[666][434] = 1;
TABLE[666][377] = 1;
TABLE[666][311] = 1;
TABLE[666][45] = 1;
TABLE[666][43] = 1;
TABLE[666][40] = 1;
TABLE[666][256] = 1;
TABLE[666][458] = 1;
TABLE[666][345] = 1;
TABLE[666][259] = 1;
TABLE[1163][125] = 1;
TABLE[668][288] = 1;
TABLE[1165][451] = 1;
TABLE[1165][350] = 1;
TABLE[1165][352] = 1;
TABLE[1165][433] = 1;
TABLE[1165][380] = 1;
TABLE[1165][474] = 1;
TABLE[1165][273] = 1;
TABLE[1165][272] = 1;
TABLE[1165][271] = 1;
TABLE[1165][263] = 1;
TABLE[1165][267] = 1;
TABLE[1165][266] = 1;
TABLE[1165][265] = 1;
TABLE[1165][268] = 1;
TABLE[1165][274] = 1;
TABLE[1165][264] = 1;
TABLE[1165][447] = 1;
TABLE[1165][336] = 1;
TABLE[1165][299] = 1;
TABLE[1165][341] = 1;
TABLE[1165][449] = 1;
TABLE[1165][450] = 1;
TABLE[1165][422] = 1;
TABLE[1165][328] = 1;
TABLE[1165][358] = 1;
TABLE[1165][309] = 1;
TABLE[1165][418] = 1;
TABLE[1165][261] = 1;
TABLE[1165][332] = 1;
TABLE[1165][293] = 1;
TABLE[1165][348] = 1;
TABLE[1165][372] = 1;
TABLE[1165][388] = 1;
TABLE[1165][43] = 1;
TABLE[1165][45] = 1;
TABLE[1165][330] = 1;
TABLE[1165][409] = 1;
TABLE[1165][334] = 1;
TABLE[1165][436] = 1;
TABLE[1165][259] = 1;
TABLE[1165][345] = 1;
TABLE[1165][458] = 1;
TABLE[1165][256] = 1;
TABLE[1165][40] = 1;
TABLE[1165][0] = 1;
TABLE[667][326] = 1;
TABLE[667][374] = 1;
TABLE[667][315] = 1;
TABLE[667][298] = 1;
TABLE[667][413] = 1;
TABLE[667][445] = 1;
TABLE[667][417] = 1;
TABLE[667][472] = 1;
TABLE[667][473] = 1;
TABLE[667][333] = 1;
TABLE[667][414] = 1;
TABLE[667][297] = 1;
TABLE[667][378] = 1;
TABLE[667][461] = 1;
TABLE[667][428] = 1;
TABLE[667][318] = 1;
TABLE[667][350] = 1;
TABLE[667][400] = 1;
TABLE[667][264] = 1;
TABLE[667][274] = 1;
TABLE[667][268] = 1;
TABLE[667][265] = 1;
TABLE[667][266] = 1;
TABLE[667][267] = 1;
TABLE[667][263] = 1;
TABLE[667][271] = 1;
TABLE[667][272] = 1;
TABLE[667][273] = 1;
TABLE[667][474] = 1;
TABLE[667][434] = 1;
TABLE[667][377] = 1;
TABLE[667][311] = 1;
TABLE[667][45] = 1;
TABLE[667][43] = 1;
TABLE[667][40] = 1;
TABLE[667][256] = 1;
TABLE[667][458] = 1;
TABLE[667][345] = 1;
TABLE[667][259] = 1;
TABLE[1164][125] = 1;
TABLE[166][326] = 1;
TABLE[167][43] = 1;
TABLE[167][45] = 1;
TABLE[167][259] = 1;
TABLE[167][345] = 1;
TABLE[167][458] = 1;
TABLE[167][256] = 1;
TABLE[167][40] = 1;
TABLE[168][374] = 1;
TABLE[169][315] = 1;
TABLE[170][298] = 1;
TABLE[171][413] = 1;
TABLE[172][445] = 1;
TABLE[173][417] = 1;
TABLE[174][472] = 1;
TABLE[175][473] = 1;
TABLE[176][333] = 1;
TABLE[177][414] = 1;
TABLE[178][297] = 1;
TABLE[179][378] = 1;
TABLE[180][461] = 1;
TABLE[181][428] = 1;
TABLE[182][428] = 1;
TABLE[183][318] = 1;
TABLE[184][318] = 1;
TABLE[185][350] = 1;
TABLE[186][400] = 1;
TABLE[187][474] = 1;
TABLE[187][273] = 1;
TABLE[187][272] = 1;
TABLE[187][271] = 1;
TABLE[187][263] = 1;
TABLE[187][267] = 1;
TABLE[187][266] = 1;
TABLE[187][265] = 1;
TABLE[187][268] = 1;
TABLE[187][274] = 1;
TABLE[187][264] = 1;
TABLE[188][311] = 1;
TABLE[188][377] = 1;
TABLE[188][434] = 1;
TABLE[669][58] = 1;
TABLE[1166][257] = 1;
TABLE[670][58] = 1;
TABLE[1167][43] = 1;
TABLE[1167][45] = 1;
TABLE[1167][259] = 1;
TABLE[1167][345] = 1;
TABLE[1167][458] = 1;
TABLE[1167][256] = 1;
TABLE[1167][40] = 1;
TABLE[671][58] = 1;
TABLE[1168][43] = 1;
TABLE[1168][45] = 1;
TABLE[1168][259] = 1;
TABLE[1168][345] = 1;
TABLE[1168][458] = 1;
TABLE[1168][256] = 1;
TABLE[1168][40] = 1;
TABLE[672][308] = 1;
TABLE[1169][58] = 1;
TABLE[1169][43] = 1;
TABLE[1169][45] = 1;
TABLE[1169][259] = 1;
TABLE[1169][345] = 1;
TABLE[1169][458] = 1;
TABLE[1169][256] = 1;
TABLE[1169][40] = 1;
TABLE[673][58] = 1;
TABLE[1170][43] = 1;
TABLE[1170][45] = 1;
TABLE[1170][259] = 1;
TABLE[1170][345] = 1;
TABLE[1170][458] = 1;
TABLE[1170][256] = 1;
TABLE[1170][40] = 1;
TABLE[674][37] = 1;
TABLE[1171][59] = 1;
TABLE[675][58] = 1;
TABLE[1172][43] = 1;
TABLE[1172][45] = 1;
TABLE[1172][259] = 1;
TABLE[1172][345] = 1;
TABLE[1172][458] = 1;
TABLE[1172][256] = 1;
TABLE[1172][40] = 1;
TABLE[676][40] = 1;
TABLE[1173][59] = 1;
TABLE[677][58] = 1;
TABLE[1174][43] = 1;
TABLE[1174][45] = 1;
TABLE[1174][259] = 1;
TABLE[1174][345] = 1;
TABLE[1174][458] = 1;
TABLE[1174][256] = 1;
TABLE[1174][40] = 1;
TABLE[678][405] = 1;
TABLE[1175][59] = 1;
TABLE[679][58] = 1;
TABLE[1176][43] = 1;
TABLE[1176][45] = 1;
TABLE[1176][259] = 1;
TABLE[1176][345] = 1;
TABLE[1176][458] = 1;
TABLE[1176][256] = 1;
TABLE[1176][40] = 1;
TABLE[680][58] = 1;
TABLE[1177][43] = 1;
TABLE[1177][45] = 1;
TABLE[1177][259] = 1;
TABLE[1177][345] = 1;
TABLE[1177][458] = 1;
TABLE[1177][256] = 1;
TABLE[1177][40] = 1;
TABLE[681][58] = 1;
TABLE[1178][43] = 1;
TABLE[1178][45] = 1;
TABLE[1178][259] = 1;
TABLE[1178][345] = 1;
TABLE[1178][458] = 1;
TABLE[1178][256] = 1;
TABLE[1178][40] = 1;
TABLE[682][58] = 1;
TABLE[1179][43] = 1;
TABLE[1179][45] = 1;
TABLE[1179][259] = 1;
TABLE[1179][345] = 1;
TABLE[1179][458] = 1;
TABLE[1179][256] = 1;
TABLE[1179][40] = 1;
TABLE[683][58] = 1;
TABLE[1180][259] = 1;
TABLE[1180][345] = 1;
TABLE[1180][458] = 1;
TABLE[1180][256] = 1;
TABLE[1180][40] = 1;
TABLE[1180][45] = 1;
TABLE[1180][43] = 1;
TABLE[684][58] = 1;
TABLE[1181][43] = 1;
TABLE[1181][45] = 1;
TABLE[1181][259] = 1;
TABLE[1181][345] = 1;
TABLE[1181][458] = 1;
TABLE[1181][256] = 1;
TABLE[1181][40] = 1;
TABLE[685][58] = 1;
TABLE[1182][43] = 1;
TABLE[1182][45] = 1;
TABLE[1182][259] = 1;
TABLE[1182][345] = 1;
TABLE[1182][458] = 1;
TABLE[1182][256] = 1;
TABLE[1182][40] = 1;
TABLE[686][58] = 1;
TABLE[1183][43] = 1;
TABLE[1183][45] = 1;
TABLE[1183][259] = 1;
TABLE[1183][345] = 1;
TABLE[1183][458] = 1;
TABLE[1183][256] = 1;
TABLE[1183][40] = 1;
TABLE[687][58] = 1;
TABLE[1184][36] = 1;
TABLE[688][58] = 1;
TABLE[1185][43] = 1;
TABLE[1185][45] = 1;
TABLE[1185][259] = 1;
TABLE[1185][345] = 1;
TABLE[1185][458] = 1;
TABLE[1185][256] = 1;
TABLE[1185][40] = 1;
TABLE[689][58] = 1;
TABLE[1186][43] = 1;
TABLE[1186][45] = 1;
TABLE[1186][259] = 1;
TABLE[1186][345] = 1;
TABLE[1186][458] = 1;
TABLE[1186][256] = 1;
TABLE[1186][40] = 1;
TABLE[690][58] = 1;
TABLE[1187][275] = 1;
TABLE[691][58] = 1;
TABLE[1188][45] = 1;
TABLE[1188][43] = 1;
TABLE[1188][40] = 1;
TABLE[1188][256] = 1;
TABLE[1188][458] = 1;
TABLE[1188][345] = 1;
TABLE[1188][259] = 1;
TABLE[1188][59] = 1;
TABLE[694][43] = 1;
TABLE[694][45] = 1;
TABLE[694][259] = 1;
TABLE[694][345] = 1;
TABLE[694][458] = 1;
TABLE[694][256] = 1;
TABLE[694][40] = 1;
TABLE[1190][59] = 1;
TABLE[692][44] = 1;
TABLE[693][124] = 1;
TABLE[1189][45] = 1;
TABLE[1189][43] = 1;
TABLE[1189][40] = 1;
TABLE[1189][256] = 1;
TABLE[1189][458] = 1;
TABLE[1189][345] = 1;
TABLE[1189][259] = 1;
TABLE[1189][59] = 1;
TABLE[189][45] = 1;
TABLE[189][43] = 1;
TABLE[189][40] = 1;
TABLE[189][256] = 1;
TABLE[189][458] = 1;
TABLE[189][345] = 1;
TABLE[189][259] = 1;
TABLE[697][43] = 1;
TABLE[697][45] = 1;
TABLE[697][259] = 1;
TABLE[697][345] = 1;
TABLE[697][458] = 1;
TABLE[697][256] = 1;
TABLE[697][40] = 1;
TABLE[695][44] = 1;
TABLE[696][124] = 1;
TABLE[1191][45] = 1;
TABLE[1191][43] = 1;
TABLE[1191][40] = 1;
TABLE[1191][256] = 1;
TABLE[1191][458] = 1;
TABLE[1191][345] = 1;
TABLE[1191][259] = 1;
TABLE[190][261] = 1;
TABLE[191][418] = 1;
TABLE[698][58] = 1;
TABLE[1193][123] = 1;
TABLE[699][125] = 1;
TABLE[699][326] = 1;
TABLE[699][59] = 1;
TABLE[699][359] = 1;
TABLE[699][388] = 1;
TABLE[699][340] = 1;
TABLE[699][432] = 1;
TABLE[699][45] = 1;
TABLE[699][43] = 1;
TABLE[699][350] = 1;
TABLE[699][356] = 1;
TABLE[699][400] = 1;
TABLE[699][277] = 1;
TABLE[699][40] = 1;
TABLE[699][256] = 1;
TABLE[699][458] = 1;
TABLE[699][345] = 1;
TABLE[699][259] = 1;
TABLE[699][434] = 1;
TABLE[699][377] = 1;
TABLE[699][311] = 1;
TABLE[699][264] = 1;
TABLE[699][274] = 1;
TABLE[699][268] = 1;
TABLE[699][265] = 1;
TABLE[699][266] = 1;
TABLE[699][267] = 1;
TABLE[699][263] = 1;
TABLE[699][271] = 1;
TABLE[699][272] = 1;
TABLE[699][273] = 1;
TABLE[699][474] = 1;
TABLE[1194][125] = 1;
TABLE[192][326] = 1;
TABLE[193][59] = 1;
TABLE[193][277] = 1;
TABLE[194][359] = 1;
TABLE[195][388] = 1;
TABLE[196][340] = 1;
TABLE[197][340] = 1;
TABLE[198][432] = 1;
TABLE[199][43] = 1;
TABLE[199][45] = 1;
TABLE[199][259] = 1;
TABLE[199][345] = 1;
TABLE[199][458] = 1;
TABLE[199][256] = 1;
TABLE[199][40] = 1;
TABLE[200][350] = 1;
TABLE[201][356] = 1;
TABLE[202][311] = 1;
TABLE[202][377] = 1;
TABLE[202][434] = 1;
TABLE[203][400] = 1;
TABLE[204][474] = 1;
TABLE[204][273] = 1;
TABLE[204][272] = 1;
TABLE[204][271] = 1;
TABLE[204][263] = 1;
TABLE[204][267] = 1;
TABLE[204][266] = 1;
TABLE[204][265] = 1;
TABLE[204][268] = 1;
TABLE[204][274] = 1;
TABLE[204][264] = 1;
TABLE[700][58] = 1;
TABLE[1195][257] = 1;
TABLE[703][277] = 1;
TABLE[1198][59] = 1;
TABLE[701][58] = 1;
TABLE[1196][43] = 1;
TABLE[1196][45] = 1;
TABLE[1196][259] = 1;
TABLE[1196][345] = 1;
TABLE[1196][458] = 1;
TABLE[1196][256] = 1;
TABLE[1196][40] = 1;
TABLE[702][44] = 1;
TABLE[1197][277] = 1;
TABLE[1197][59] = 1;
TABLE[704][58] = 1;
TABLE[1199][347] = 1;
TABLE[1199][440] = 1;
TABLE[1199][282] = 1;
TABLE[1199][59] = 1;
TABLE[706][282] = 1;
TABLE[706][440] = 1;
TABLE[706][347] = 1;
TABLE[1201][59] = 1;
TABLE[705][44] = 1;
TABLE[1200][347] = 1;
TABLE[1200][440] = 1;
TABLE[1200][282] = 1;
TABLE[1200][59] = 1;
TABLE[707][58] = 1;
TABLE[1202][36] = 1;
TABLE[708][309] = 1;
TABLE[1203][58] = 1;
TABLE[1203][36] = 1;
TABLE[709][58] = 1;
TABLE[1204][36] = 1;
TABLE[710][309] = 1;
TABLE[1205][58] = 1;
TABLE[1205][36] = 1;
TABLE[711][58] = 1;
TABLE[1206][36] = 1;
TABLE[712][58] = 1;
TABLE[1207][45] = 1;
TABLE[1207][43] = 1;
TABLE[1207][40] = 1;
TABLE[1207][256] = 1;
TABLE[1207][458] = 1;
TABLE[1207][345] = 1;
TABLE[1207][259] = 1;
TABLE[1207][59] = 1;
TABLE[714][43] = 1;
TABLE[714][45] = 1;
TABLE[714][259] = 1;
TABLE[714][345] = 1;
TABLE[714][458] = 1;
TABLE[714][256] = 1;
TABLE[714][40] = 1;
TABLE[1209][59] = 1;
TABLE[713][44] = 1;
TABLE[1208][45] = 1;
TABLE[1208][43] = 1;
TABLE[1208][40] = 1;
TABLE[1208][256] = 1;
TABLE[1208][458] = 1;
TABLE[1208][345] = 1;
TABLE[1208][259] = 1;
TABLE[1208][59] = 1;
TABLE[715][58] = 1;
TABLE[1210][257] = 1;
TABLE[716][58] = 1;
TABLE[1211][45] = 1;
TABLE[1211][43] = 1;
TABLE[1211][40] = 1;
TABLE[1211][256] = 1;
TABLE[1211][458] = 1;
TABLE[1211][345] = 1;
TABLE[1211][259] = 1;
TABLE[1211][59] = 1;
TABLE[719][43] = 1;
TABLE[719][45] = 1;
TABLE[719][259] = 1;
TABLE[719][345] = 1;
TABLE[719][458] = 1;
TABLE[719][256] = 1;
TABLE[719][40] = 1;
TABLE[1213][59] = 1;
TABLE[717][124] = 1;
TABLE[718][44] = 1;
TABLE[1212][45] = 1;
TABLE[1212][43] = 1;
TABLE[1212][40] = 1;
TABLE[1212][256] = 1;
TABLE[1212][458] = 1;
TABLE[1212][345] = 1;
TABLE[1212][259] = 1;
TABLE[1212][59] = 1;
TABLE[205][309] = 1;
TABLE[720][326] = 1;
TABLE[720][364] = 1;
TABLE[720][381] = 1;
TABLE[720][323] = 1;
TABLE[720][296] = 1;
TABLE[720][429] = 1;
TABLE[720][359] = 1;
TABLE[720][432] = 1;
TABLE[720][411] = 1;
TABLE[720][350] = 1;
TABLE[720][356] = 1;
TABLE[720][400] = 1;
TABLE[720][434] = 1;
TABLE[720][377] = 1;
TABLE[720][311] = 1;
TABLE[720][264] = 1;
TABLE[720][274] = 1;
TABLE[720][268] = 1;
TABLE[720][265] = 1;
TABLE[720][266] = 1;
TABLE[720][267] = 1;
TABLE[720][263] = 1;
TABLE[720][271] = 1;
TABLE[720][272] = 1;
TABLE[720][273] = 1;
TABLE[720][474] = 1;
TABLE[1214][125] = 1;
TABLE[206][326] = 1;
TABLE[207][364] = 1;
TABLE[208][381] = 1;
TABLE[209][323] = 1;
TABLE[210][296] = 1;
TABLE[211][429] = 1;
TABLE[212][359] = 1;
TABLE[213][432] = 1;
TABLE[214][411] = 1;
TABLE[215][350] = 1;
TABLE[216][356] = 1;
TABLE[217][400] = 1;
TABLE[218][311] = 1;
TABLE[218][377] = 1;
TABLE[218][434] = 1;
TABLE[219][474] = 1;
TABLE[219][273] = 1;
TABLE[219][272] = 1;
TABLE[219][271] = 1;
TABLE[219][263] = 1;
TABLE[219][267] = 1;
TABLE[219][266] = 1;
TABLE[219][265] = 1;
TABLE[219][268] = 1;
TABLE[219][274] = 1;
TABLE[219][264] = 1;
TABLE[721][58] = 1;
TABLE[1215][257] = 1;
TABLE[722][58] = 1;
TABLE[1216][100] = 1;
TABLE[1216][43] = 1;
TABLE[1216][45] = 1;
TABLE[1216][259] = 1;
TABLE[1216][345] = 1;
TABLE[1216][458] = 1;
TABLE[1216][256] = 1;
TABLE[1216][40] = 1;
TABLE[723][100] = 1;
TABLE[1217][43] = 1;
TABLE[1217][45] = 1;
TABLE[1217][259] = 1;
TABLE[1217][345] = 1;
TABLE[1217][458] = 1;
TABLE[1217][256] = 1;
TABLE[1217][40] = 1;
TABLE[724][58] = 1;
TABLE[1218][100] = 1;
TABLE[1218][43] = 1;
TABLE[1218][45] = 1;
TABLE[1218][259] = 1;
TABLE[1218][345] = 1;
TABLE[1218][458] = 1;
TABLE[1218][256] = 1;
TABLE[1218][40] = 1;
TABLE[725][100] = 1;
TABLE[1219][43] = 1;
TABLE[1219][45] = 1;
TABLE[1219][259] = 1;
TABLE[1219][345] = 1;
TABLE[1219][458] = 1;
TABLE[1219][256] = 1;
TABLE[1219][40] = 1;
TABLE[726][58] = 1;
TABLE[1220][43] = 1;
TABLE[1220][45] = 1;
TABLE[1220][259] = 1;
TABLE[1220][345] = 1;
TABLE[1220][458] = 1;
TABLE[1220][256] = 1;
TABLE[1220][40] = 1;
TABLE[727][58] = 1;
TABLE[1221][45] = 1;
TABLE[1221][43] = 1;
TABLE[1221][40] = 1;
TABLE[1221][256] = 1;
TABLE[1221][458] = 1;
TABLE[1221][345] = 1;
TABLE[1221][259] = 1;
TABLE[1221][59] = 1;
TABLE[729][43] = 1;
TABLE[729][45] = 1;
TABLE[729][259] = 1;
TABLE[729][345] = 1;
TABLE[729][458] = 1;
TABLE[729][256] = 1;
TABLE[729][40] = 1;
TABLE[1223][59] = 1;
TABLE[728][44] = 1;
TABLE[1222][45] = 1;
TABLE[1222][43] = 1;
TABLE[1222][40] = 1;
TABLE[1222][256] = 1;
TABLE[1222][458] = 1;
TABLE[1222][345] = 1;
TABLE[1222][259] = 1;
TABLE[1222][59] = 1;
TABLE[730][58] = 1;
TABLE[1224][45] = 1;
TABLE[1224][43] = 1;
TABLE[1224][40] = 1;
TABLE[1224][256] = 1;
TABLE[1224][458] = 1;
TABLE[1224][345] = 1;
TABLE[1224][259] = 1;
TABLE[1224][59] = 1;
TABLE[732][43] = 1;
TABLE[732][45] = 1;
TABLE[732][259] = 1;
TABLE[732][345] = 1;
TABLE[732][458] = 1;
TABLE[732][256] = 1;
TABLE[732][40] = 1;
TABLE[1226][59] = 1;
TABLE[731][44] = 1;
TABLE[1225][45] = 1;
TABLE[1225][43] = 1;
TABLE[1225][40] = 1;
TABLE[1225][256] = 1;
TABLE[1225][458] = 1;
TABLE[1225][345] = 1;
TABLE[1225][259] = 1;
TABLE[1225][59] = 1;
TABLE[733][58] = 1;
TABLE[1227][347] = 1;
TABLE[1227][440] = 1;
TABLE[1227][282] = 1;
TABLE[1227][59] = 1;
TABLE[735][282] = 1;
TABLE[735][440] = 1;
TABLE[735][347] = 1;
TABLE[1229][59] = 1;
TABLE[734][44] = 1;
TABLE[1228][347] = 1;
TABLE[1228][440] = 1;
TABLE[1228][282] = 1;
TABLE[1228][59] = 1;
TABLE[736][91] = 1;
TABLE[737][93] = 1;
TABLE[738][58] = 1;
TABLE[1230][45] = 1;
TABLE[1230][43] = 1;
TABLE[1230][40] = 1;
TABLE[1230][256] = 1;
TABLE[1230][458] = 1;
TABLE[1230][345] = 1;
TABLE[1230][259] = 1;
TABLE[1230][59] = 1;
TABLE[740][43] = 1;
TABLE[740][45] = 1;
TABLE[740][259] = 1;
TABLE[740][345] = 1;
TABLE[740][458] = 1;
TABLE[740][256] = 1;
TABLE[740][40] = 1;
TABLE[1232][59] = 1;
TABLE[739][44] = 1;
TABLE[1231][45] = 1;
TABLE[1231][43] = 1;
TABLE[1231][40] = 1;
TABLE[1231][256] = 1;
TABLE[1231][458] = 1;
TABLE[1231][345] = 1;
TABLE[1231][259] = 1;
TABLE[1231][59] = 1;
TABLE[741][58] = 1;
TABLE[1233][45] = 1;
TABLE[1233][43] = 1;
TABLE[1233][40] = 1;
TABLE[1233][256] = 1;
TABLE[1233][458] = 1;
TABLE[1233][345] = 1;
TABLE[1233][259] = 1;
TABLE[1233][59] = 1;
TABLE[743][43] = 1;
TABLE[743][45] = 1;
TABLE[743][259] = 1;
TABLE[743][345] = 1;
TABLE[743][458] = 1;
TABLE[743][256] = 1;
TABLE[743][40] = 1;
TABLE[1235][59] = 1;
TABLE[742][44] = 1;
TABLE[1234][45] = 1;
TABLE[1234][43] = 1;
TABLE[1234][40] = 1;
TABLE[1234][256] = 1;
TABLE[1234][458] = 1;
TABLE[1234][345] = 1;
TABLE[1234][259] = 1;
TABLE[1234][59] = 1;
TABLE[744][58] = 1;
TABLE[1236][45] = 1;
TABLE[1236][43] = 1;
TABLE[1236][40] = 1;
TABLE[1236][256] = 1;
TABLE[1236][458] = 1;
TABLE[1236][345] = 1;
TABLE[1236][259] = 1;
TABLE[1236][59] = 1;
TABLE[747][43] = 1;
TABLE[747][45] = 1;
TABLE[747][259] = 1;
TABLE[747][345] = 1;
TABLE[747][458] = 1;
TABLE[747][256] = 1;
TABLE[747][40] = 1;
TABLE[1238][59] = 1;
TABLE[745][124] = 1;
TABLE[746][44] = 1;
TABLE[1237][45] = 1;
TABLE[1237][43] = 1;
TABLE[1237][40] = 1;
TABLE[1237][256] = 1;
TABLE[1237][458] = 1;
TABLE[1237][345] = 1;
TABLE[1237][259] = 1;
TABLE[1237][59] = 1;
TABLE[220][282] = 1;
TABLE[221][440] = 1;
TABLE[222][347] = 1;
TABLE[748][44] = 1;
TABLE[1239][93] = 1;
TABLE[749][44] = 1;
TABLE[1240][93] = 1;
TABLE[750][44] = 1;
TABLE[1241][93] = 1;
TABLE[223][44] = 1;
TABLE[224][44] = 1;
TABLE[225][44] = 1;
TABLE[226][43] = 1;
TABLE[226][45] = 1;
TABLE[226][259] = 1;
TABLE[226][345] = 1;
TABLE[226][458] = 1;
TABLE[226][256] = 1;
TABLE[226][40] = 1;
TABLE[227][343] = 1;
TABLE[228][343] = 1;
TABLE[229][343] = 1;
TABLE[230][343] = 1;
TABLE[751][374] = 1;
TABLE[1242][44] = 1;
TABLE[1242][347] = 1;
TABLE[1242][440] = 1;
TABLE[1242][282] = 1;
TABLE[1242][59] = 1;
TABLE[752][374] = 1;
TABLE[1243][44] = 1;
TABLE[1243][347] = 1;
TABLE[1243][440] = 1;
TABLE[1243][282] = 1;
TABLE[1243][59] = 1;
TABLE[753][374] = 1;
TABLE[1244][44] = 1;
TABLE[1244][347] = 1;
TABLE[1244][440] = 1;
TABLE[1244][282] = 1;
TABLE[1244][59] = 1;
TABLE[231][332] = 1;
TABLE[754][326] = 1;
TABLE[754][400] = 1;
TABLE[754][434] = 1;
TABLE[754][377] = 1;
TABLE[754][311] = 1;
TABLE[754][264] = 1;
TABLE[754][274] = 1;
TABLE[754][268] = 1;
TABLE[754][265] = 1;
TABLE[754][266] = 1;
TABLE[754][267] = 1;
TABLE[754][263] = 1;
TABLE[754][271] = 1;
TABLE[754][272] = 1;
TABLE[754][273] = 1;
TABLE[754][474] = 1;
TABLE[1245][125] = 1;
TABLE[232][326] = 1;
TABLE[233][311] = 1;
TABLE[233][377] = 1;
TABLE[233][434] = 1;
TABLE[234][400] = 1;
TABLE[235][474] = 1;
TABLE[235][273] = 1;
TABLE[235][272] = 1;
TABLE[235][271] = 1;
TABLE[235][263] = 1;
TABLE[235][267] = 1;
TABLE[235][266] = 1;
TABLE[235][265] = 1;
TABLE[235][268] = 1;
TABLE[235][274] = 1;
TABLE[235][264] = 1;
TABLE[755][58] = 1;
TABLE[1246][257] = 1;
TABLE[236][311] = 1;
TABLE[237][377] = 1;
TABLE[238][434] = 1;
TABLE[756][58] = 1;
TABLE[1247][42] = 1;
TABLE[1247][59] = 1;
TABLE[758][42] = 1;
TABLE[760][42] = 1;
TABLE[1250][59] = 1;
TABLE[757][44] = 1;
TABLE[1248][42] = 1;
TABLE[1248][59] = 1;
TABLE[759][44] = 1;
TABLE[1249][42] = 1;
TABLE[1249][59] = 1;
TABLE[761][58] = 1;
TABLE[1251][42] = 1;
TABLE[1251][400] = 1;
TABLE[1251][356] = 1;
TABLE[1251][409] = 1;
TABLE[1251][330] = 1;
TABLE[1251][437] = 1;
TABLE[1251][389] = 1;
TABLE[1251][435] = 1;
TABLE[1251][431] = 1;
TABLE[1251][350] = 1;
TABLE[1251][346] = 1;
TABLE[1251][381] = 1;
TABLE[1251][292] = 1;
TABLE[1251][323] = 1;
TABLE[1251][294] = 1;
TABLE[1251][296] = 1;
TABLE[1251][369] = 1;
TABLE[1251][424] = 1;
TABLE[1251][440] = 1;
TABLE[1251][366] = 1;
TABLE[1251][365] = 1;
TABLE[1251][364] = 1;
TABLE[1251][450] = 1;
TABLE[1251][325] = 1;
TABLE[1251][326] = 1;
TABLE[1251][474] = 1;
TABLE[1251][273] = 1;
TABLE[1251][272] = 1;
TABLE[1251][271] = 1;
TABLE[1251][263] = 1;
TABLE[1251][267] = 1;
TABLE[1251][266] = 1;
TABLE[1251][265] = 1;
TABLE[1251][268] = 1;
TABLE[1251][274] = 1;
TABLE[1251][264] = 1;
TABLE[1251][311] = 1;
TABLE[1251][377] = 1;
TABLE[1251][434] = 1;
TABLE[1251][277] = 1;
TABLE[1251][357] = 1;
TABLE[1251][368] = 1;
TABLE[1251][43] = 1;
TABLE[1251][374] = 1;
TABLE[1251][307] = 1;
TABLE[1251][125] = 1;
TABLE[1251][355] = 1;
TABLE[1251][375] = 1;
TABLE[1251][376] = 1;
TABLE[1251][313] = 1;
TABLE[1251][456] = 1;
TABLE[1251][379] = 1;
TABLE[1251][467] = 1;
TABLE[1251][468] = 1;
TABLE[1251][304] = 1;
TABLE[1251][361] = 1;
TABLE[1251][349] = 1;
TABLE[1251][407] = 1;
TABLE[1251][312] = 1;
TABLE[1251][396] = 1;
TABLE[1251][466] = 1;
TABLE[1251][315] = 1;
TABLE[1251][383] = 1;
TABLE[1251][420] = 1;
TABLE[1251][404] = 1;
TABLE[1251][284] = 1;
TABLE[1251][452] = 1;
TABLE[1251][316] = 1;
TABLE[1251][373] = 1;
TABLE[1251][430] = 1;
TABLE[1251][344] = 1;
TABLE[1251][472] = 1;
TABLE[1251][448] = 1;
TABLE[1251][334] = 1;
TABLE[1251][318] = 1;
TABLE[1251][428] = 1;
TABLE[1251][461] = 1;
TABLE[1251][378] = 1;
TABLE[1251][297] = 1;
TABLE[1251][414] = 1;
TABLE[1251][333] = 1;
TABLE[1251][473] = 1;
TABLE[1251][417] = 1;
TABLE[1251][445] = 1;
TABLE[1251][413] = 1;
TABLE[1251][298] = 1;
TABLE[1251][45] = 1;
TABLE[1251][259] = 1;
TABLE[1251][345] = 1;
TABLE[1251][458] = 1;
TABLE[1251][256] = 1;
TABLE[1251][40] = 1;
TABLE[1251][432] = 1;
TABLE[1251][340] = 1;
TABLE[1251][388] = 1;
TABLE[1251][359] = 1;
TABLE[1251][59] = 1;
TABLE[1251][411] = 1;
TABLE[1251][429] = 1;
TABLE[1251][329] = 1;
TABLE[1251][398] = 1;
TABLE[1251][423] = 1;
TABLE[1251][455] = 1;
TABLE[1251][360] = 1;
TABLE[1251][426] = 1;
TABLE[1251][327] = 1;
TABLE[1251][351] = 1;
TABLE[1251][464] = 1;
TABLE[1251][306] = 1;
TABLE[1251][410] = 1;
TABLE[1251][319] = 1;
TABLE[1251][390] = 1;
TABLE[1251][353] = 1;
TABLE[1251][406] = 1;
TABLE[1251][285] = 1;
TABLE[1251][436] = 1;
TABLE[765][42] = 1;
TABLE[1253][400] = 1;
TABLE[1253][356] = 1;
TABLE[1253][409] = 1;
TABLE[1253][330] = 1;
TABLE[1253][437] = 1;
TABLE[1253][389] = 1;
TABLE[1253][435] = 1;
TABLE[1253][431] = 1;
TABLE[1253][350] = 1;
TABLE[1253][346] = 1;
TABLE[1253][381] = 1;
TABLE[1253][292] = 1;
TABLE[1253][323] = 1;
TABLE[1253][294] = 1;
TABLE[1253][296] = 1;
TABLE[1253][369] = 1;
TABLE[1253][424] = 1;
TABLE[1253][440] = 1;
TABLE[1253][366] = 1;
TABLE[1253][365] = 1;
TABLE[1253][364] = 1;
TABLE[1253][450] = 1;
TABLE[1253][325] = 1;
TABLE[1253][326] = 1;
TABLE[1253][474] = 1;
TABLE[1253][273] = 1;
TABLE[1253][272] = 1;
TABLE[1253][271] = 1;
TABLE[1253][263] = 1;
TABLE[1253][267] = 1;
TABLE[1253][266] = 1;
TABLE[1253][265] = 1;
TABLE[1253][268] = 1;
TABLE[1253][274] = 1;
TABLE[1253][264] = 1;
TABLE[1253][311] = 1;
TABLE[1253][377] = 1;
TABLE[1253][434] = 1;
TABLE[1253][277] = 1;
TABLE[1253][357] = 1;
TABLE[1253][368] = 1;
TABLE[1253][43] = 1;
TABLE[1253][374] = 1;
TABLE[1253][307] = 1;
TABLE[1253][125] = 1;
TABLE[1253][355] = 1;
TABLE[1253][375] = 1;
TABLE[1253][376] = 1;
TABLE[1253][313] = 1;
TABLE[1253][456] = 1;
TABLE[1253][379] = 1;
TABLE[1253][467] = 1;
TABLE[1253][468] = 1;
TABLE[1253][304] = 1;
TABLE[1253][361] = 1;
TABLE[1253][349] = 1;
TABLE[1253][407] = 1;
TABLE[1253][312] = 1;
TABLE[1253][396] = 1;
TABLE[1253][466] = 1;
TABLE[1253][315] = 1;
TABLE[1253][383] = 1;
TABLE[1253][420] = 1;
TABLE[1253][404] = 1;
TABLE[1253][284] = 1;
TABLE[1253][452] = 1;
TABLE[1253][316] = 1;
TABLE[1253][373] = 1;
TABLE[1253][430] = 1;
TABLE[1253][344] = 1;
TABLE[1253][472] = 1;
TABLE[1253][448] = 1;
TABLE[1253][334] = 1;
TABLE[1253][318] = 1;
TABLE[1253][428] = 1;
TABLE[1253][461] = 1;
TABLE[1253][378] = 1;
TABLE[1253][297] = 1;
TABLE[1253][414] = 1;
TABLE[1253][333] = 1;
TABLE[1253][473] = 1;
TABLE[1253][417] = 1;
TABLE[1253][445] = 1;
TABLE[1253][413] = 1;
TABLE[1253][298] = 1;
TABLE[1253][45] = 1;
TABLE[1253][259] = 1;
TABLE[1253][345] = 1;
TABLE[1253][458] = 1;
TABLE[1253][256] = 1;
TABLE[1253][40] = 1;
TABLE[1253][432] = 1;
TABLE[1253][340] = 1;
TABLE[1253][388] = 1;
TABLE[1253][359] = 1;
TABLE[1253][59] = 1;
TABLE[1253][411] = 1;
TABLE[1253][429] = 1;
TABLE[1253][329] = 1;
TABLE[1253][398] = 1;
TABLE[1253][423] = 1;
TABLE[1253][455] = 1;
TABLE[1253][360] = 1;
TABLE[1253][426] = 1;
TABLE[1253][327] = 1;
TABLE[1253][351] = 1;
TABLE[1253][464] = 1;
TABLE[1253][306] = 1;
TABLE[1253][410] = 1;
TABLE[1253][319] = 1;
TABLE[1253][390] = 1;
TABLE[1253][353] = 1;
TABLE[1253][406] = 1;
TABLE[1253][285] = 1;
TABLE[1253][436] = 1;
TABLE[762][36] = 1;
TABLE[762][259] = 1;
TABLE[762][345] = 1;
TABLE[762][458] = 1;
TABLE[762][256] = 1;
TABLE[762][40] = 1;
TABLE[762][257] = 1;
TABLE[1252][44] = 1;
TABLE[1252][59] = 1;
TABLE[763][44] = 1;
TABLE[764][59] = 1;
TABLE[766][58] = 1;
TABLE[1254][42] = 1;
TABLE[1254][59] = 1;
TABLE[772][42] = 1;
TABLE[1257][59] = 1;
TABLE[767][40] = 1;
TABLE[767][256] = 1;
TABLE[767][458] = 1;
TABLE[767][345] = 1;
TABLE[767][259] = 1;
TABLE[1255][36] = 1;
TABLE[768][36] = 1;
TABLE[769][36] = 1;
TABLE[770][36] = 1;
TABLE[771][44] = 1;
TABLE[1256][42] = 1;
TABLE[1256][59] = 1;
TABLE[239][36] = 1;
TABLE[240][40] = 1;
TABLE[240][256] = 1;
TABLE[240][458] = 1;
TABLE[240][345] = 1;
TABLE[240][259] = 1;
TABLE[241][257] = 1;
TABLE[242][293] = 1;
TABLE[773][125] = 1;
TABLE[773][326] = 1;
TABLE[773][372] = 1;
TABLE[773][59] = 1;
TABLE[773][424] = 1;
TABLE[773][444] = 1;
TABLE[773][415] = 1;
TABLE[773][338] = 1;
TABLE[773][470] = 1;
TABLE[773][366] = 1;
TABLE[773][371] = 1;
TABLE[773][400] = 1;
TABLE[773][277] = 1;
TABLE[773][264] = 1;
TABLE[773][274] = 1;
TABLE[773][268] = 1;
TABLE[773][265] = 1;
TABLE[773][266] = 1;
TABLE[773][267] = 1;
TABLE[773][263] = 1;
TABLE[773][271] = 1;
TABLE[773][272] = 1;
TABLE[773][273] = 1;
TABLE[773][474] = 1;
TABLE[1258][125] = 1;
TABLE[243][326] = 1;
TABLE[244][372] = 1;
TABLE[245][59] = 1;
TABLE[245][277] = 1;
TABLE[246][424] = 1;
TABLE[247][444] = 1;
TABLE[248][415] = 1;
TABLE[249][338] = 1;
TABLE[250][470] = 1;
TABLE[251][366] = 1;
TABLE[252][371] = 1;
TABLE[253][400] = 1;
TABLE[254][474] = 1;
TABLE[254][273] = 1;
TABLE[254][272] = 1;
TABLE[254][271] = 1;
TABLE[254][263] = 1;
TABLE[254][267] = 1;
TABLE[254][266] = 1;
TABLE[254][265] = 1;
TABLE[254][268] = 1;
TABLE[254][274] = 1;
TABLE[254][264] = 1;
TABLE[774][58] = 1;
TABLE[1259][257] = 1;
TABLE[775][58] = 1;
TABLE[1260][36] = 1;
TABLE[778][277] = 1;
TABLE[1263][59] = 1;
TABLE[776][58] = 1;
TABLE[1261][43] = 1;
TABLE[1261][45] = 1;
TABLE[1261][259] = 1;
TABLE[1261][345] = 1;
TABLE[1261][458] = 1;
TABLE[1261][256] = 1;
TABLE[1261][40] = 1;
TABLE[777][44] = 1;
TABLE[1262][277] = 1;
TABLE[1262][59] = 1;
TABLE[779][58] = 1;
TABLE[1264][45] = 1;
TABLE[1264][43] = 1;
TABLE[1264][40] = 1;
TABLE[1264][256] = 1;
TABLE[1264][458] = 1;
TABLE[1264][345] = 1;
TABLE[1264][259] = 1;
TABLE[1264][59] = 1;
TABLE[781][43] = 1;
TABLE[781][45] = 1;
TABLE[781][259] = 1;
TABLE[781][345] = 1;
TABLE[781][458] = 1;
TABLE[781][256] = 1;
TABLE[781][40] = 1;
TABLE[1266][59] = 1;
TABLE[780][44] = 1;
TABLE[1265][45] = 1;
TABLE[1265][43] = 1;
TABLE[1265][40] = 1;
TABLE[1265][256] = 1;
TABLE[1265][458] = 1;
TABLE[1265][345] = 1;
TABLE[1265][259] = 1;
TABLE[1265][59] = 1;
TABLE[782][58] = 1;
TABLE[1267][277] = 1;
TABLE[1267][59] = 1;
TABLE[784][277] = 1;
TABLE[1269][59] = 1;
TABLE[783][44] = 1;
TABLE[1268][277] = 1;
TABLE[1268][59] = 1;
TABLE[785][58] = 1;
TABLE[1270][45] = 1;
TABLE[1270][43] = 1;
TABLE[1270][40] = 1;
TABLE[1270][256] = 1;
TABLE[1270][458] = 1;
TABLE[1270][345] = 1;
TABLE[1270][259] = 1;
TABLE[1270][59] = 1;
TABLE[787][43] = 1;
TABLE[787][45] = 1;
TABLE[787][259] = 1;
TABLE[787][345] = 1;
TABLE[787][458] = 1;
TABLE[787][256] = 1;
TABLE[787][40] = 1;
TABLE[1272][59] = 1;
TABLE[786][44] = 1;
TABLE[1271][45] = 1;
TABLE[1271][43] = 1;
TABLE[1271][40] = 1;
TABLE[1271][256] = 1;
TABLE[1271][458] = 1;
TABLE[1271][345] = 1;
TABLE[1271][259] = 1;
TABLE[1271][59] = 1;
TABLE[789][440] = 1;
TABLE[789][36] = 1;
TABLE[789][370] = 1;
TABLE[789][347] = 1;
TABLE[1273][59] = 1;
TABLE[788][44] = 1;
TABLE[791][440] = 1;
TABLE[791][36] = 1;
TABLE[791][370] = 1;
TABLE[791][347] = 1;
TABLE[1274][59] = 1;
TABLE[790][44] = 1;
TABLE[793][440] = 1;
TABLE[793][36] = 1;
TABLE[793][370] = 1;
TABLE[793][347] = 1;
TABLE[1275][59] = 1;
TABLE[792][44] = 1;
TABLE[795][440] = 1;
TABLE[795][36] = 1;
TABLE[795][370] = 1;
TABLE[795][347] = 1;
TABLE[1276][59] = 1;
TABLE[794][44] = 1;
TABLE[255][440] = 1;
TABLE[256][440] = 1;
TABLE[257][36] = 1;
TABLE[258][36] = 1;
TABLE[259][36] = 1;
TABLE[260][36] = 1;
TABLE[261][370] = 1;
TABLE[262][347] = 1;
TABLE[796][295] = 1;
TABLE[1277][44] = 1;
TABLE[797][47] = 1;
TABLE[798][408] = 1;
TABLE[799][47] = 1;
TABLE[800][408] = 1;
TABLE[263][328] = 1;
TABLE[801][437] = 1;
TABLE[801][359] = 1;
TABLE[801][326] = 1;
TABLE[801][400] = 1;
TABLE[801][434] = 1;
TABLE[801][377] = 1;
TABLE[801][311] = 1;
TABLE[801][264] = 1;
TABLE[801][274] = 1;
TABLE[801][268] = 1;
TABLE[801][265] = 1;
TABLE[801][266] = 1;
TABLE[801][267] = 1;
TABLE[801][263] = 1;
TABLE[801][271] = 1;
TABLE[801][272] = 1;
TABLE[801][273] = 1;
TABLE[801][474] = 1;
TABLE[1278][125] = 1;
TABLE[264][437] = 1;
TABLE[265][359] = 1;
TABLE[266][326] = 1;
TABLE[267][400] = 1;
TABLE[268][311] = 1;
TABLE[268][377] = 1;
TABLE[268][434] = 1;
TABLE[269][474] = 1;
TABLE[269][273] = 1;
TABLE[269][272] = 1;
TABLE[269][271] = 1;
TABLE[269][263] = 1;
TABLE[269][267] = 1;
TABLE[269][266] = 1;
TABLE[269][265] = 1;
TABLE[269][268] = 1;
TABLE[269][274] = 1;
TABLE[269][264] = 1;
TABLE[802][58] = 1;
TABLE[1279][36] = 1;
TABLE[1279][59] = 1;
TABLE[804][36] = 1;
TABLE[1281][59] = 1;
TABLE[803][44] = 1;
TABLE[1280][36] = 1;
TABLE[1280][59] = 1;
TABLE[805][58] = 1;
TABLE[1282][347] = 1;
TABLE[1282][440] = 1;
TABLE[1282][282] = 1;
TABLE[1282][59] = 1;
TABLE[807][282] = 1;
TABLE[807][440] = 1;
TABLE[807][347] = 1;
TABLE[1284][59] = 1;
TABLE[806][44] = 1;
TABLE[1283][347] = 1;
TABLE[1283][440] = 1;
TABLE[1283][282] = 1;
TABLE[1283][59] = 1;
TABLE[808][58] = 1;
TABLE[1285][257] = 1;
TABLE[270][358] = 1;
TABLE[809][329] = 1;
TABLE[809][311] = 1;
TABLE[809][350] = 1;
TABLE[809][326] = 1;
TABLE[809][359] = 1;
TABLE[809][400] = 1;
TABLE[809][434] = 1;
TABLE[809][377] = 1;
TABLE[809][264] = 1;
TABLE[809][274] = 1;
TABLE[809][268] = 1;
TABLE[809][265] = 1;
TABLE[809][266] = 1;
TABLE[809][267] = 1;
TABLE[809][263] = 1;
TABLE[809][271] = 1;
TABLE[809][272] = 1;
TABLE[809][273] = 1;
TABLE[809][474] = 1;
TABLE[1286][125] = 1;
TABLE[271][329] = 1;
TABLE[272][311] = 1;
TABLE[273][350] = 1;
TABLE[274][326] = 1;
TABLE[275][359] = 1;
TABLE[276][400] = 1;
TABLE[277][311] = 1;
TABLE[277][377] = 1;
TABLE[277][434] = 1;
TABLE[278][474] = 1;
TABLE[278][273] = 1;
TABLE[278][272] = 1;
TABLE[278][271] = 1;
TABLE[278][263] = 1;
TABLE[278][267] = 1;
TABLE[278][266] = 1;
TABLE[278][265] = 1;
TABLE[278][268] = 1;
TABLE[278][274] = 1;
TABLE[278][264] = 1;
TABLE[810][58] = 1;
TABLE[1287][36] = 1;
TABLE[1287][59] = 1;
TABLE[812][36] = 1;
TABLE[1289][59] = 1;
TABLE[811][44] = 1;
TABLE[1288][36] = 1;
TABLE[1288][59] = 1;
TABLE[813][58] = 1;
TABLE[1290][45] = 1;
TABLE[1290][43] = 1;
TABLE[1290][40] = 1;
TABLE[1290][256] = 1;
TABLE[1290][458] = 1;
TABLE[1290][345] = 1;
TABLE[1290][259] = 1;
TABLE[1290][59] = 1;
TABLE[816][43] = 1;
TABLE[816][45] = 1;
TABLE[816][259] = 1;
TABLE[816][345] = 1;
TABLE[816][458] = 1;
TABLE[816][256] = 1;
TABLE[816][40] = 1;
TABLE[1292][59] = 1;
TABLE[814][124] = 1;
TABLE[815][44] = 1;
TABLE[1291][45] = 1;
TABLE[1291][43] = 1;
TABLE[1291][40] = 1;
TABLE[1291][256] = 1;
TABLE[1291][458] = 1;
TABLE[1291][345] = 1;
TABLE[1291][259] = 1;
TABLE[1291][59] = 1;
TABLE[817][58] = 1;
TABLE[1293][257] = 1;
TABLE[818][58] = 1;
TABLE[1294][347] = 1;
TABLE[1294][440] = 1;
TABLE[1294][282] = 1;
TABLE[1294][59] = 1;
TABLE[820][282] = 1;
TABLE[820][440] = 1;
TABLE[820][347] = 1;
TABLE[1296][59] = 1;
TABLE[819][44] = 1;
TABLE[1295][347] = 1;
TABLE[1295][440] = 1;
TABLE[1295][282] = 1;
TABLE[1295][59] = 1;
TABLE[279][450] = 1;
TABLE[821][326] = 1;
TABLE[821][389] = 1;
TABLE[821][407] = 1;
TABLE[821][383] = 1;
TABLE[821][350] = 1;
TABLE[821][434] = 1;
TABLE[821][377] = 1;
TABLE[821][311] = 1;
TABLE[821][400] = 1;
TABLE[821][368] = 1;
TABLE[821][357] = 1;
TABLE[821][264] = 1;
TABLE[821][274] = 1;
TABLE[821][268] = 1;
TABLE[821][265] = 1;
TABLE[821][266] = 1;
TABLE[821][267] = 1;
TABLE[821][263] = 1;
TABLE[821][271] = 1;
TABLE[821][272] = 1;
TABLE[821][273] = 1;
TABLE[821][474] = 1;
TABLE[1297][125] = 1;
TABLE[280][326] = 1;
TABLE[281][357] = 1;
TABLE[281][368] = 1;
TABLE[282][389] = 1;
TABLE[283][407] = 1;
TABLE[284][383] = 1;
TABLE[285][350] = 1;
TABLE[286][311] = 1;
TABLE[286][377] = 1;
TABLE[286][434] = 1;
TABLE[287][400] = 1;
TABLE[288][474] = 1;
TABLE[288][273] = 1;
TABLE[288][272] = 1;
TABLE[288][271] = 1;
TABLE[288][263] = 1;
TABLE[288][267] = 1;
TABLE[288][266] = 1;
TABLE[288][265] = 1;
TABLE[288][268] = 1;
TABLE[288][274] = 1;
TABLE[288][264] = 1;
TABLE[822][58] = 1;
TABLE[1298][257] = 1;
TABLE[823][58] = 1;
TABLE[1299][43] = 1;
TABLE[1299][45] = 1;
TABLE[1299][259] = 1;
TABLE[1299][345] = 1;
TABLE[1299][458] = 1;
TABLE[1299][256] = 1;
TABLE[1299][40] = 1;
TABLE[824][58] = 1;
TABLE[1300][43] = 1;
TABLE[1300][45] = 1;
TABLE[1300][259] = 1;
TABLE[1300][345] = 1;
TABLE[1300][458] = 1;
TABLE[1300][256] = 1;
TABLE[1300][40] = 1;
TABLE[825][58] = 1;
TABLE[1301][43] = 1;
TABLE[1301][45] = 1;
TABLE[1301][259] = 1;
TABLE[1301][345] = 1;
TABLE[1301][458] = 1;
TABLE[1301][256] = 1;
TABLE[1301][40] = 1;
TABLE[826][58] = 1;
TABLE[1302][45] = 1;
TABLE[1302][43] = 1;
TABLE[1302][40] = 1;
TABLE[1302][256] = 1;
TABLE[1302][458] = 1;
TABLE[1302][345] = 1;
TABLE[1302][259] = 1;
TABLE[1302][59] = 1;
TABLE[829][43] = 1;
TABLE[829][45] = 1;
TABLE[829][259] = 1;
TABLE[829][345] = 1;
TABLE[829][458] = 1;
TABLE[829][256] = 1;
TABLE[829][40] = 1;
TABLE[1304][59] = 1;
TABLE[827][124] = 1;
TABLE[828][44] = 1;
TABLE[1303][45] = 1;
TABLE[1303][43] = 1;
TABLE[1303][40] = 1;
TABLE[1303][256] = 1;
TABLE[1303][458] = 1;
TABLE[1303][345] = 1;
TABLE[1303][259] = 1;
TABLE[1303][59] = 1;
TABLE[289][422] = 1;
TABLE[830][326] = 1;
TABLE[830][464] = 1;
TABLE[830][351] = 1;
TABLE[830][327] = 1;
TABLE[830][325] = 1;
TABLE[830][431] = 1;
TABLE[830][426] = 1;
TABLE[830][350] = 1;
TABLE[830][360] = 1;
TABLE[830][455] = 1;
TABLE[830][423] = 1;
TABLE[830][398] = 1;
TABLE[830][434] = 1;
TABLE[830][377] = 1;
TABLE[830][311] = 1;
TABLE[830][400] = 1;
TABLE[830][264] = 1;
TABLE[830][274] = 1;
TABLE[830][268] = 1;
TABLE[830][265] = 1;
TABLE[830][266] = 1;
TABLE[830][267] = 1;
TABLE[830][263] = 1;
TABLE[830][271] = 1;
TABLE[830][272] = 1;
TABLE[830][273] = 1;
TABLE[830][474] = 1;
TABLE[1305][125] = 1;
TABLE[290][326] = 1;
TABLE[291][464] = 1;
TABLE[292][351] = 1;
TABLE[293][327] = 1;
TABLE[294][325] = 1;
TABLE[295][431] = 1;
TABLE[296][426] = 1;
TABLE[297][350] = 1;
TABLE[298][360] = 1;
TABLE[299][455] = 1;
TABLE[300][423] = 1;
TABLE[301][398] = 1;
TABLE[302][311] = 1;
TABLE[302][377] = 1;
TABLE[302][434] = 1;
TABLE[303][400] = 1;
TABLE[304][474] = 1;
TABLE[304][273] = 1;
TABLE[304][272] = 1;
TABLE[304][271] = 1;
TABLE[304][263] = 1;
TABLE[304][267] = 1;
TABLE[304][266] = 1;
TABLE[304][265] = 1;
TABLE[304][268] = 1;
TABLE[304][274] = 1;
TABLE[304][264] = 1;
TABLE[831][58] = 1;
TABLE[1306][257] = 1;
TABLE[832][58] = 1;
TABLE[1307][36] = 1;
TABLE[833][58] = 1;
TABLE[1308][36] = 1;
TABLE[834][58] = 1;
TABLE[1309][36] = 1;
TABLE[835][58] = 1;
TABLE[1310][43] = 1;
TABLE[1310][45] = 1;
TABLE[1310][259] = 1;
TABLE[1310][345] = 1;
TABLE[1310][458] = 1;
TABLE[1310][256] = 1;
TABLE[1310][40] = 1;
TABLE[836][58] = 1;
TABLE[1311][43] = 1;
TABLE[1311][45] = 1;
TABLE[1311][259] = 1;
TABLE[1311][345] = 1;
TABLE[1311][458] = 1;
TABLE[1311][256] = 1;
TABLE[1311][40] = 1;
TABLE[837][58] = 1;
TABLE[1312][45] = 1;
TABLE[1312][43] = 1;
TABLE[1312][40] = 1;
TABLE[1312][256] = 1;
TABLE[1312][458] = 1;
TABLE[1312][345] = 1;
TABLE[1312][259] = 1;
TABLE[1312][59] = 1;
TABLE[840][43] = 1;
TABLE[840][45] = 1;
TABLE[840][259] = 1;
TABLE[840][345] = 1;
TABLE[840][458] = 1;
TABLE[840][256] = 1;
TABLE[840][40] = 1;
TABLE[1314][59] = 1;
TABLE[838][44] = 1;
TABLE[839][124] = 1;
TABLE[1313][45] = 1;
TABLE[1313][43] = 1;
TABLE[1313][40] = 1;
TABLE[1313][256] = 1;
TABLE[1313][458] = 1;
TABLE[1313][345] = 1;
TABLE[1313][259] = 1;
TABLE[1313][59] = 1;
TABLE[841][58] = 1;
TABLE[1315][45] = 1;
TABLE[1315][43] = 1;
TABLE[1315][40] = 1;
TABLE[1315][256] = 1;
TABLE[1315][458] = 1;
TABLE[1315][345] = 1;
TABLE[1315][259] = 1;
TABLE[1315][59] = 1;
TABLE[844][43] = 1;
TABLE[844][45] = 1;
TABLE[844][259] = 1;
TABLE[844][345] = 1;
TABLE[844][458] = 1;
TABLE[844][256] = 1;
TABLE[844][40] = 1;
TABLE[1317][59] = 1;
TABLE[842][124] = 1;
TABLE[843][44] = 1;
TABLE[1316][45] = 1;
TABLE[1316][43] = 1;
TABLE[1316][40] = 1;
TABLE[1316][256] = 1;
TABLE[1316][458] = 1;
TABLE[1316][345] = 1;
TABLE[1316][259] = 1;
TABLE[1316][59] = 1;
TABLE[857][259] = 1;
TABLE[1323][59] = 1;
TABLE[845][91] = 1;
TABLE[854][471] = 1;
TABLE[855][294] = 1;
TABLE[1321][44] = 1;
TABLE[1321][259] = 1;
TABLE[1321][59] = 1;
TABLE[846][36] = 1;
TABLE[848][419] = 1;
TABLE[850][419] = 1;
TABLE[847][43] = 1;
TABLE[847][45] = 1;
TABLE[847][259] = 1;
TABLE[847][345] = 1;
TABLE[847][458] = 1;
TABLE[847][256] = 1;
TABLE[847][40] = 1;
TABLE[1318][399] = 1;
TABLE[1318][91] = 1;
TABLE[1318][471] = 1;
TABLE[1318][294] = 1;
TABLE[1318][44] = 1;
TABLE[1318][259] = 1;
TABLE[1318][59] = 1;
TABLE[849][43] = 1;
TABLE[849][45] = 1;
TABLE[849][259] = 1;
TABLE[849][345] = 1;
TABLE[849][458] = 1;
TABLE[849][256] = 1;
TABLE[849][40] = 1;
TABLE[1319][399] = 1;
TABLE[1319][91] = 1;
TABLE[1319][471] = 1;
TABLE[1319][294] = 1;
TABLE[1319][44] = 1;
TABLE[1319][259] = 1;
TABLE[1319][59] = 1;
TABLE[851][399] = 1;
TABLE[852][399] = 1;
TABLE[853][399] = 1;
TABLE[1320][91] = 1;
TABLE[1320][471] = 1;
TABLE[1320][294] = 1;
TABLE[1320][44] = 1;
TABLE[1320][259] = 1;
TABLE[1320][59] = 1;
TABLE[856][44] = 1;
TABLE[1322][259] = 1;
TABLE[1322][59] = 1;
TABLE[858][326] = 1;
TABLE[858][464] = 1;
TABLE[858][351] = 1;
TABLE[858][327] = 1;
TABLE[858][325] = 1;
TABLE[858][431] = 1;
TABLE[858][426] = 1;
TABLE[858][350] = 1;
TABLE[858][360] = 1;
TABLE[858][455] = 1;
TABLE[858][423] = 1;
TABLE[858][398] = 1;
TABLE[858][434] = 1;
TABLE[858][377] = 1;
TABLE[858][311] = 1;
TABLE[858][400] = 1;
TABLE[858][264] = 1;
TABLE[858][274] = 1;
TABLE[858][268] = 1;
TABLE[858][265] = 1;
TABLE[858][266] = 1;
TABLE[858][267] = 1;
TABLE[858][263] = 1;
TABLE[858][271] = 1;
TABLE[858][272] = 1;
TABLE[858][273] = 1;
TABLE[858][474] = 1;
TABLE[1324][400] = 1;
TABLE[1324][311] = 1;
TABLE[1324][377] = 1;
TABLE[1324][434] = 1;
TABLE[1324][398] = 1;
TABLE[1324][423] = 1;
TABLE[1324][455] = 1;
TABLE[1324][360] = 1;
TABLE[1324][350] = 1;
TABLE[1324][426] = 1;
TABLE[1324][431] = 1;
TABLE[1324][325] = 1;
TABLE[1324][327] = 1;
TABLE[1324][351] = 1;
TABLE[1324][464] = 1;
TABLE[1324][326] = 1;
TABLE[1324][474] = 1;
TABLE[1324][273] = 1;
TABLE[1324][272] = 1;
TABLE[1324][271] = 1;
TABLE[1324][263] = 1;
TABLE[1324][267] = 1;
TABLE[1324][266] = 1;
TABLE[1324][265] = 1;
TABLE[1324][268] = 1;
TABLE[1324][274] = 1;
TABLE[1324][264] = 1;
TABLE[1324][125] = 1;
TABLE[859][261] = 1;
TABLE[1325][400] = 1;
TABLE[1325][311] = 1;
TABLE[1325][377] = 1;
TABLE[1325][434] = 1;
TABLE[1325][398] = 1;
TABLE[1325][423] = 1;
TABLE[1325][455] = 1;
TABLE[1325][360] = 1;
TABLE[1325][350] = 1;
TABLE[1325][426] = 1;
TABLE[1325][431] = 1;
TABLE[1325][325] = 1;
TABLE[1325][327] = 1;
TABLE[1325][351] = 1;
TABLE[1325][464] = 1;
TABLE[1325][326] = 1;
TABLE[1325][474] = 1;
TABLE[1325][273] = 1;
TABLE[1325][272] = 1;
TABLE[1325][271] = 1;
TABLE[1325][263] = 1;
TABLE[1325][267] = 1;
TABLE[1325][266] = 1;
TABLE[1325][265] = 1;
TABLE[1325][268] = 1;
TABLE[1325][274] = 1;
TABLE[1325][264] = 1;
TABLE[1325][125] = 1;
TABLE[305][449] = 1;
TABLE[860][353] = 1;
TABLE[1326][123] = 1;
TABLE[861][326] = 1;
TABLE[861][353] = 1;
TABLE[861][390] = 1;
TABLE[861][364] = 1;
TABLE[861][366] = 1;
TABLE[861][323] = 1;
TABLE[861][292] = 1;
TABLE[861][319] = 1;
TABLE[861][410] = 1;
TABLE[861][306] = 1;
TABLE[861][431] = 1;
TABLE[861][435] = 1;
TABLE[861][389] = 1;
TABLE[861][277] = 1;
TABLE[861][296] = 1;
TABLE[861][440] = 1;
TABLE[861][424] = 1;
TABLE[861][369] = 1;
TABLE[861][346] = 1;
TABLE[861][437] = 1;
TABLE[861][330] = 1;
TABLE[861][409] = 1;
TABLE[861][350] = 1;
TABLE[861][356] = 1;
TABLE[861][400] = 1;
TABLE[861][434] = 1;
TABLE[861][377] = 1;
TABLE[861][311] = 1;
TABLE[861][368] = 1;
TABLE[861][357] = 1;
TABLE[861][307] = 1;
TABLE[861][374] = 1;
TABLE[861][264] = 1;
TABLE[861][274] = 1;
TABLE[861][268] = 1;
TABLE[861][265] = 1;
TABLE[861][266] = 1;
TABLE[861][267] = 1;
TABLE[861][263] = 1;
TABLE[861][271] = 1;
TABLE[861][272] = 1;
TABLE[861][273] = 1;
TABLE[861][474] = 1;
TABLE[1327][125] = 1;
TABLE[306][326] = 1;
TABLE[307][353] = 1;
TABLE[308][390] = 1;
TABLE[309][357] = 1;
TABLE[309][368] = 1;
TABLE[310][374] = 1;
TABLE[310][307] = 1;
TABLE[311][364] = 1;
TABLE[312][366] = 1;
TABLE[313][323] = 1;
TABLE[314][292] = 1;
TABLE[315][319] = 1;
TABLE[316][410] = 1;
TABLE[317][306] = 1;
TABLE[318][431] = 1;
TABLE[319][435] = 1;
TABLE[320][389] = 1;
TABLE[321][277] = 1;
TABLE[322][296] = 1;
TABLE[323][440] = 1;
TABLE[324][440] = 1;
TABLE[325][440] = 1;
TABLE[326][440] = 1;
TABLE[327][424] = 1;
TABLE[328][369] = 1;
TABLE[329][346] = 1;
TABLE[330][437] = 1;
TABLE[331][330] = 1;
TABLE[332][409] = 1;
TABLE[333][350] = 1;
TABLE[334][356] = 1;
TABLE[335][400] = 1;
TABLE[336][311] = 1;
TABLE[336][377] = 1;
TABLE[336][434] = 1;
TABLE[337][474] = 1;
TABLE[337][273] = 1;
TABLE[337][272] = 1;
TABLE[337][271] = 1;
TABLE[337][263] = 1;
TABLE[337][267] = 1;
TABLE[337][266] = 1;
TABLE[337][265] = 1;
TABLE[337][268] = 1;
TABLE[337][274] = 1;
TABLE[337][264] = 1;
TABLE[862][58] = 1;
TABLE[1328][257] = 1;
TABLE[863][58] = 1;
TABLE[1329][43] = 1;
TABLE[1329][45] = 1;
TABLE[1329][259] = 1;
TABLE[1329][345] = 1;
TABLE[1329][458] = 1;
TABLE[1329][256] = 1;
TABLE[1329][40] = 1;
TABLE[864][58] = 1;
TABLE[1330][43] = 1;
TABLE[1330][45] = 1;
TABLE[1330][259] = 1;
TABLE[1330][345] = 1;
TABLE[1330][458] = 1;
TABLE[1330][256] = 1;
TABLE[1330][40] = 1;
TABLE[865][374] = 1;
TABLE[866][307] = 1;
TABLE[867][58] = 1;
TABLE[1331][43] = 1;
TABLE[1331][61] = 1;
TABLE[1331][45] = 1;
TABLE[1331][40] = 1;
TABLE[1331][256] = 1;
TABLE[1331][458] = 1;
TABLE[1331][345] = 1;
TABLE[1331][259] = 1;
TABLE[1331][59] = 1;
TABLE[868][58] = 1;
TABLE[1332][43] = 1;
TABLE[1332][61] = 1;
TABLE[1332][45] = 1;
TABLE[1332][40] = 1;
TABLE[1332][256] = 1;
TABLE[1332][458] = 1;
TABLE[1332][345] = 1;
TABLE[1332][259] = 1;
TABLE[1332][59] = 1;
TABLE[869][58] = 1;
TABLE[1333][43] = 1;
TABLE[1333][61] = 1;
TABLE[1333][45] = 1;
TABLE[1333][40] = 1;
TABLE[1333][256] = 1;
TABLE[1333][458] = 1;
TABLE[1333][345] = 1;
TABLE[1333][259] = 1;
TABLE[1333][59] = 1;
TABLE[870][58] = 1;
TABLE[1334][43] = 1;
TABLE[1334][61] = 1;
TABLE[1334][45] = 1;
TABLE[1334][40] = 1;
TABLE[1334][256] = 1;
TABLE[1334][458] = 1;
TABLE[1334][345] = 1;
TABLE[1334][259] = 1;
TABLE[1334][59] = 1;
TABLE[871][58] = 1;
TABLE[1335][43] = 1;
TABLE[1335][61] = 1;
TABLE[1335][45] = 1;
TABLE[1335][40] = 1;
TABLE[1335][256] = 1;
TABLE[1335][458] = 1;
TABLE[1335][345] = 1;
TABLE[1335][259] = 1;
TABLE[1335][59] = 1;
TABLE[872][58] = 1;
TABLE[1336][43] = 1;
TABLE[1336][45] = 1;
TABLE[1336][259] = 1;
TABLE[1336][345] = 1;
TABLE[1336][458] = 1;
TABLE[1336][256] = 1;
TABLE[1336][40] = 1;
TABLE[873][58] = 1;
TABLE[1337][43] = 1;
TABLE[1337][61] = 1;
TABLE[1337][45] = 1;
TABLE[1337][40] = 1;
TABLE[1337][256] = 1;
TABLE[1337][458] = 1;
TABLE[1337][345] = 1;
TABLE[1337][259] = 1;
TABLE[1337][59] = 1;
TABLE[874][58] = 1;
TABLE[1338][43] = 1;
TABLE[1338][61] = 1;
TABLE[1338][45] = 1;
TABLE[1338][40] = 1;
TABLE[1338][256] = 1;
TABLE[1338][458] = 1;
TABLE[1338][345] = 1;
TABLE[1338][259] = 1;
TABLE[1338][59] = 1;
TABLE[875][58] = 1;
TABLE[1339][43] = 1;
TABLE[1339][61] = 1;
TABLE[1339][45] = 1;
TABLE[1339][40] = 1;
TABLE[1339][256] = 1;
TABLE[1339][458] = 1;
TABLE[1339][345] = 1;
TABLE[1339][259] = 1;
TABLE[1339][59] = 1;
TABLE[876][58] = 1;
TABLE[1340][43] = 1;
TABLE[1340][61] = 1;
TABLE[1340][45] = 1;
TABLE[1340][40] = 1;
TABLE[1340][256] = 1;
TABLE[1340][458] = 1;
TABLE[1340][345] = 1;
TABLE[1340][259] = 1;
TABLE[1340][59] = 1;
TABLE[877][58] = 1;
TABLE[1341][43] = 1;
TABLE[1341][61] = 1;
TABLE[1341][45] = 1;
TABLE[1341][40] = 1;
TABLE[1341][256] = 1;
TABLE[1341][458] = 1;
TABLE[1341][345] = 1;
TABLE[1341][259] = 1;
TABLE[1341][59] = 1;
TABLE[878][58] = 1;
TABLE[1342][43] = 1;
TABLE[1342][61] = 1;
TABLE[1342][45] = 1;
TABLE[1342][40] = 1;
TABLE[1342][256] = 1;
TABLE[1342][458] = 1;
TABLE[1342][345] = 1;
TABLE[1342][259] = 1;
TABLE[1342][44] = 1;
TABLE[1342][59] = 1;
TABLE[879][44] = 1;
TABLE[880][59] = 1;
TABLE[881][58] = 1;
TABLE[1343][45] = 1;
TABLE[1343][43] = 1;
TABLE[1343][40] = 1;
TABLE[1343][256] = 1;
TABLE[1343][458] = 1;
TABLE[1343][345] = 1;
TABLE[1343][259] = 1;
TABLE[882][44] = 1;
TABLE[1344][59] = 1;
TABLE[883][58] = 1;
TABLE[1345][43] = 1;
TABLE[1345][45] = 1;
TABLE[1345][126] = 1;
TABLE[1345][33] = 1;
TABLE[1345][259] = 1;
TABLE[1345][345] = 1;
TABLE[1345][458] = 1;
TABLE[1345][256] = 1;
TABLE[1345][40] = 1;
TABLE[1345][59] = 1;
TABLE[889][45] = 1;
TABLE[889][43] = 1;
TABLE[889][40] = 1;
TABLE[889][256] = 1;
TABLE[889][458] = 1;
TABLE[889][345] = 1;
TABLE[889][259] = 1;
TABLE[889][33] = 1;
TABLE[889][126] = 1;
TABLE[1347][59] = 1;
TABLE[884][43] = 1;
TABLE[884][45] = 1;
TABLE[884][259] = 1;
TABLE[884][345] = 1;
TABLE[884][458] = 1;
TABLE[884][256] = 1;
TABLE[884][40] = 1;
TABLE[887][126] = 1;
TABLE[887][33] = 1;
TABLE[885][126] = 1;
TABLE[886][33] = 1;
TABLE[888][44] = 1;
TABLE[1346][43] = 1;
TABLE[1346][45] = 1;
TABLE[1346][126] = 1;
TABLE[1346][33] = 1;
TABLE[1346][259] = 1;
TABLE[1346][345] = 1;
TABLE[1346][458] = 1;
TABLE[1346][256] = 1;
TABLE[1346][40] = 1;
TABLE[1346][59] = 1;
TABLE[890][58] = 1;
TABLE[1348][43] = 1;
TABLE[1348][45] = 1;
TABLE[1348][126] = 1;
TABLE[1348][33] = 1;
TABLE[1348][259] = 1;
TABLE[1348][345] = 1;
TABLE[1348][458] = 1;
TABLE[1348][256] = 1;
TABLE[1348][40] = 1;
TABLE[1348][59] = 1;
TABLE[896][45] = 1;
TABLE[896][43] = 1;
TABLE[896][40] = 1;
TABLE[896][256] = 1;
TABLE[896][458] = 1;
TABLE[896][345] = 1;
TABLE[896][259] = 1;
TABLE[896][33] = 1;
TABLE[896][126] = 1;
TABLE[1350][59] = 1;
TABLE[891][43] = 1;
TABLE[891][45] = 1;
TABLE[891][259] = 1;
TABLE[891][345] = 1;
TABLE[891][458] = 1;
TABLE[891][256] = 1;
TABLE[891][40] = 1;
TABLE[894][126] = 1;
TABLE[894][33] = 1;
TABLE[892][126] = 1;
TABLE[893][33] = 1;
TABLE[895][44] = 1;
TABLE[1349][43] = 1;
TABLE[1349][45] = 1;
TABLE[1349][126] = 1;
TABLE[1349][33] = 1;
TABLE[1349][259] = 1;
TABLE[1349][345] = 1;
TABLE[1349][458] = 1;
TABLE[1349][256] = 1;
TABLE[1349][40] = 1;
TABLE[1349][59] = 1;
TABLE[897][58] = 1;
TABLE[1351][43] = 1;
TABLE[1351][45] = 1;
TABLE[1351][259] = 1;
TABLE[1351][345] = 1;
TABLE[1351][458] = 1;
TABLE[1351][256] = 1;
TABLE[1351][40] = 1;
TABLE[898][43] = 1;
TABLE[898][45] = 1;
TABLE[898][259] = 1;
TABLE[898][345] = 1;
TABLE[898][458] = 1;
TABLE[898][256] = 1;
TABLE[898][40] = 1;
TABLE[1352][43] = 1;
TABLE[1352][45] = 1;
TABLE[1352][259] = 1;
TABLE[1352][345] = 1;
TABLE[1352][458] = 1;
TABLE[1352][256] = 1;
TABLE[1352][40] = 1;
TABLE[899][58] = 1;
TABLE[1353][36] = 1;
TABLE[1353][59] = 1;
TABLE[901][36] = 1;
TABLE[1355][59] = 1;
TABLE[900][44] = 1;
TABLE[1354][36] = 1;
TABLE[1354][59] = 1;
TABLE[902][58] = 1;
TABLE[1356][36] = 1;
TABLE[1356][59] = 1;
TABLE[904][36] = 1;
TABLE[1358][59] = 1;
TABLE[903][44] = 1;
TABLE[1357][36] = 1;
TABLE[1357][59] = 1;
TABLE[905][58] = 1;
TABLE[1359][36] = 1;
TABLE[1359][59] = 1;
TABLE[907][36] = 1;
TABLE[1361][59] = 1;
TABLE[906][44] = 1;
TABLE[1360][36] = 1;
TABLE[1360][59] = 1;
TABLE[908][58] = 1;
TABLE[1362][43] = 1;
TABLE[1362][45] = 1;
TABLE[1362][126] = 1;
TABLE[1362][33] = 1;
TABLE[1362][259] = 1;
TABLE[1362][345] = 1;
TABLE[1362][458] = 1;
TABLE[1362][256] = 1;
TABLE[1362][40] = 1;
TABLE[1362][311] = 1;
TABLE[1362][377] = 1;
TABLE[1362][434] = 1;
TABLE[1362][400] = 1;
TABLE[1362][356] = 1;
TABLE[1362][350] = 1;
TABLE[1362][409] = 1;
TABLE[1362][330] = 1;
TABLE[1362][437] = 1;
TABLE[1362][346] = 1;
TABLE[1362][369] = 1;
TABLE[1362][424] = 1;
TABLE[1362][440] = 1;
TABLE[1362][296] = 1;
TABLE[1362][277] = 1;
TABLE[1362][389] = 1;
TABLE[1362][435] = 1;
TABLE[1362][431] = 1;
TABLE[1362][306] = 1;
TABLE[1362][410] = 1;
TABLE[1362][319] = 1;
TABLE[1362][292] = 1;
TABLE[1362][323] = 1;
TABLE[1362][366] = 1;
TABLE[1362][364] = 1;
TABLE[1362][390] = 1;
TABLE[1362][353] = 1;
TABLE[1362][326] = 1;
TABLE[1362][474] = 1;
TABLE[1362][273] = 1;
TABLE[1362][272] = 1;
TABLE[1362][271] = 1;
TABLE[1362][263] = 1;
TABLE[1362][267] = 1;
TABLE[1362][266] = 1;
TABLE[1362][265] = 1;
TABLE[1362][268] = 1;
TABLE[1362][274] = 1;
TABLE[1362][264] = 1;
TABLE[1362][374] = 1;
TABLE[1362][307] = 1;
TABLE[1362][357] = 1;
TABLE[1362][368] = 1;
TABLE[1362][125] = 1;
TABLE[915][45] = 1;
TABLE[915][43] = 1;
TABLE[915][40] = 1;
TABLE[915][256] = 1;
TABLE[915][458] = 1;
TABLE[915][345] = 1;
TABLE[915][259] = 1;
TABLE[915][33] = 1;
TABLE[915][126] = 1;
TABLE[1363][311] = 1;
TABLE[1363][377] = 1;
TABLE[1363][434] = 1;
TABLE[1363][400] = 1;
TABLE[1363][356] = 1;
TABLE[1363][350] = 1;
TABLE[1363][409] = 1;
TABLE[1363][330] = 1;
TABLE[1363][437] = 1;
TABLE[1363][346] = 1;
TABLE[1363][369] = 1;
TABLE[1363][424] = 1;
TABLE[1363][440] = 1;
TABLE[1363][296] = 1;
TABLE[1363][277] = 1;
TABLE[1363][389] = 1;
TABLE[1363][435] = 1;
TABLE[1363][431] = 1;
TABLE[1363][306] = 1;
TABLE[1363][410] = 1;
TABLE[1363][319] = 1;
TABLE[1363][292] = 1;
TABLE[1363][323] = 1;
TABLE[1363][366] = 1;
TABLE[1363][364] = 1;
TABLE[1363][390] = 1;
TABLE[1363][353] = 1;
TABLE[1363][326] = 1;
TABLE[1363][474] = 1;
TABLE[1363][273] = 1;
TABLE[1363][272] = 1;
TABLE[1363][271] = 1;
TABLE[1363][263] = 1;
TABLE[1363][267] = 1;
TABLE[1363][266] = 1;
TABLE[1363][265] = 1;
TABLE[1363][268] = 1;
TABLE[1363][274] = 1;
TABLE[1363][264] = 1;
TABLE[1363][374] = 1;
TABLE[1363][307] = 1;
TABLE[1363][357] = 1;
TABLE[1363][368] = 1;
TABLE[1363][125] = 1;
TABLE[909][43] = 1;
TABLE[909][45] = 1;
TABLE[909][259] = 1;
TABLE[909][345] = 1;
TABLE[909][458] = 1;
TABLE[909][256] = 1;
TABLE[909][40] = 1;
TABLE[912][126] = 1;
TABLE[912][33] = 1;
TABLE[910][126] = 1;
TABLE[911][33] = 1;
TABLE[913][44] = 1;
TABLE[914][59] = 1;
TABLE[338][43] = 1;
TABLE[338][45] = 1;
TABLE[338][259] = 1;
TABLE[338][345] = 1;
TABLE[338][458] = 1;
TABLE[338][256] = 1;
TABLE[338][40] = 1;
TABLE[339][43] = 1;
TABLE[339][45] = 1;
TABLE[339][259] = 1;
TABLE[339][345] = 1;
TABLE[339][458] = 1;
TABLE[339][256] = 1;
TABLE[339][40] = 1;
TABLE[916][259] = 1;
TABLE[916][345] = 1;
TABLE[916][458] = 1;
TABLE[916][256] = 1;
TABLE[916][40] = 1;
TABLE[917][43] = 1;
TABLE[917][45] = 1;
TABLE[917][259] = 1;
TABLE[917][345] = 1;
TABLE[917][458] = 1;
TABLE[917][256] = 1;
TABLE[917][40] = 1;
TABLE[1364][45] = 1;
TABLE[1364][43] = 1;
TABLE[1364][256] = 1;
TABLE[1364][458] = 1;
TABLE[1364][345] = 1;
TABLE[1364][259] = 1;
TABLE[1364][319] = 1;
TABLE[1364][40] = 1;
TABLE[1364][288] = 1;
TABLE[1364][44] = 1;
TABLE[1364][59] = 1;
TABLE[918][43] = 1;
TABLE[918][45] = 1;
TABLE[918][259] = 1;
TABLE[918][345] = 1;
TABLE[918][458] = 1;
TABLE[918][256] = 1;
TABLE[918][40] = 1;
TABLE[1365][319] = 1;
TABLE[1365][40] = 1;
TABLE[1365][288] = 1;
TABLE[1365][44] = 1;
TABLE[1365][59] = 1;
TABLE[919][319] = 1;
TABLE[1366][40] = 1;
TABLE[1366][288] = 1;
TABLE[1366][44] = 1;
TABLE[1366][59] = 1;
TABLE[923][40] = 1;
TABLE[1367][288] = 1;
TABLE[1367][44] = 1;
TABLE[1367][59] = 1;
TABLE[920][322] = 1;
TABLE[921][307] = 1;
TABLE[922][374] = 1;
TABLE[931][288] = 1;
TABLE[1372][44] = 1;
TABLE[1372][59] = 1;
TABLE[924][259] = 1;
TABLE[924][345] = 1;
TABLE[924][458] = 1;
TABLE[924][256] = 1;
TABLE[924][40] = 1;
TABLE[1368][45] = 1;
TABLE[1368][43] = 1;
TABLE[1368][256] = 1;
TABLE[1368][458] = 1;
TABLE[1368][345] = 1;
TABLE[1368][259] = 1;
TABLE[1368][319] = 1;
TABLE[1368][40] = 1;
TABLE[1368][288] = 1;
TABLE[1368][44] = 1;
TABLE[1368][59] = 1;
TABLE[925][43] = 1;
TABLE[925][45] = 1;
TABLE[925][259] = 1;
TABLE[925][345] = 1;
TABLE[925][458] = 1;
TABLE[925][256] = 1;
TABLE[925][40] = 1;
TABLE[1369][319] = 1;
TABLE[1369][40] = 1;
TABLE[1369][288] = 1;
TABLE[1369][44] = 1;
TABLE[1369][59] = 1;
TABLE[926][319] = 1;
TABLE[1370][40] = 1;
TABLE[1370][288] = 1;
TABLE[1370][44] = 1;
TABLE[1370][59] = 1;
TABLE[930][40] = 1;
TABLE[1371][288] = 1;
TABLE[1371][44] = 1;
TABLE[1371][59] = 1;
TABLE[927][322] = 1;
TABLE[928][307] = 1;
TABLE[929][374] = 1;
TABLE[932][43] = 1;
TABLE[932][45] = 1;
TABLE[932][259] = 1;
TABLE[932][345] = 1;
TABLE[932][458] = 1;
TABLE[932][256] = 1;
TABLE[932][40] = 1;
TABLE[1373][44] = 1;
TABLE[1373][59] = 1;
TABLE[340][261] = 1;
TABLE[341][341] = 1;
TABLE[933][326] = 1;
TABLE[933][383] = 1;
TABLE[933][310] = 1;
TABLE[933][466] = 1;
TABLE[1374][125] = 1;
TABLE[342][326] = 1;
TABLE[343][383] = 1;
TABLE[344][310] = 1;
TABLE[345][466] = 1;
TABLE[934][58] = 1;
TABLE[1375][257] = 1;
TABLE[935][58] = 1;
TABLE[1376][43] = 1;
TABLE[1376][45] = 1;
TABLE[1376][259] = 1;
TABLE[1376][345] = 1;
TABLE[1376][458] = 1;
TABLE[1376][256] = 1;
TABLE[1376][40] = 1;
TABLE[936][58] = 1;
TABLE[1377][275] = 1;
TABLE[1377][303] = 1;
TABLE[1377][376] = 1;
TABLE[1377][320] = 1;
TABLE[937][58] = 1;
TABLE[1378][43] = 1;
TABLE[1378][45] = 1;
TABLE[1378][259] = 1;
TABLE[1378][345] = 1;
TABLE[1378][458] = 1;
TABLE[1378][256] = 1;
TABLE[1378][40] = 1;
TABLE[346][336] = 1;
TABLE[938][58] = 1;
TABLE[1379][123] = 1;
TABLE[939][450] = 1;
TABLE[939][325] = 1;
TABLE[939][466] = 1;
TABLE[939][307] = 1;
TABLE[939][285] = 1;
TABLE[939][406] = 1;
TABLE[939][350] = 1;
TABLE[939][356] = 1;
TABLE[939][400] = 1;
TABLE[939][434] = 1;
TABLE[939][377] = 1;
TABLE[939][311] = 1;
TABLE[939][264] = 1;
TABLE[939][274] = 1;
TABLE[939][268] = 1;
TABLE[939][265] = 1;
TABLE[939][266] = 1;
TABLE[939][267] = 1;
TABLE[939][263] = 1;
TABLE[939][271] = 1;
TABLE[939][272] = 1;
TABLE[939][273] = 1;
TABLE[939][474] = 1;
TABLE[1380][125] = 1;
TABLE[347][450] = 1;
TABLE[348][325] = 1;
TABLE[349][466] = 1;
TABLE[350][307] = 1;
TABLE[351][307] = 1;
TABLE[352][307] = 1;
TABLE[353][285] = 1;
TABLE[354][406] = 1;
TABLE[355][350] = 1;
TABLE[356][356] = 1;
TABLE[357][400] = 1;
TABLE[358][311] = 1;
TABLE[358][377] = 1;
TABLE[358][434] = 1;
TABLE[359][474] = 1;
TABLE[359][273] = 1;
TABLE[359][272] = 1;
TABLE[359][271] = 1;
TABLE[359][263] = 1;
TABLE[359][267] = 1;
TABLE[359][266] = 1;
TABLE[359][265] = 1;
TABLE[359][268] = 1;
TABLE[359][274] = 1;
TABLE[359][264] = 1;
TABLE[940][457] = 1;
TABLE[941][45] = 1;
TABLE[942][124] = 1;
TABLE[942][335] = 1;
TABLE[942][42] = 1;
TABLE[1381][59] = 1;
TABLE[943][58] = 1;
TABLE[1382][45] = 1;
TABLE[1382][43] = 1;
TABLE[1382][40] = 1;
TABLE[1382][256] = 1;
TABLE[1382][458] = 1;
TABLE[1382][345] = 1;
TABLE[1382][259] = 1;
TABLE[1382][59] = 1;
TABLE[946][43] = 1;
TABLE[946][45] = 1;
TABLE[946][259] = 1;
TABLE[946][345] = 1;
TABLE[946][458] = 1;
TABLE[946][256] = 1;
TABLE[946][40] = 1;
TABLE[1384][59] = 1;
TABLE[944][44] = 1;
TABLE[945][124] = 1;
TABLE[1383][45] = 1;
TABLE[1383][43] = 1;
TABLE[1383][40] = 1;
TABLE[1383][256] = 1;
TABLE[1383][458] = 1;
TABLE[1383][345] = 1;
TABLE[1383][259] = 1;
TABLE[1383][59] = 1;
TABLE[360][42] = 1;
TABLE[360][335] = 1;
TABLE[360][124] = 1;
TABLE[947][42] = 1;
TABLE[948][335] = 1;
TABLE[949][124] = 1;
TABLE[950][40] = 1;
TABLE[950][256] = 1;
TABLE[950][458] = 1;
TABLE[950][345] = 1;
TABLE[950][259] = 1;
TABLE[951][60] = 1;
TABLE[952][91] = 1;
TABLE[953][123] = 1;
TABLE[956][40] = 1;
TABLE[956][256] = 1;
TABLE[956][458] = 1;
TABLE[956][345] = 1;
TABLE[956][259] = 1;
TABLE[957][40] = 1;
TABLE[957][256] = 1;
TABLE[957][458] = 1;
TABLE[957][345] = 1;
TABLE[957][259] = 1;
TABLE[958][40] = 1;
TABLE[958][256] = 1;
TABLE[958][458] = 1;
TABLE[958][345] = 1;
TABLE[958][259] = 1;
TABLE[959][259] = 1;
TABLE[959][345] = 1;
TABLE[959][458] = 1;
TABLE[959][256] = 1;
TABLE[959][40] = 1;
TABLE[1385][399] = 1;
TABLE[954][45] = 1;
TABLE[955][457] = 1;
TABLE[960][36] = 1;
TABLE[961][43] = 1;
TABLE[961][45] = 1;
TABLE[961][259] = 1;
TABLE[961][345] = 1;
TABLE[961][458] = 1;
TABLE[961][256] = 1;
TABLE[961][40] = 1;
TABLE[962][289] = 1;
TABLE[963][36] = 1;
TABLE[964][43] = 1;
TABLE[964][45] = 1;
TABLE[964][259] = 1;
TABLE[964][345] = 1;
TABLE[964][458] = 1;
TABLE[964][256] = 1;
TABLE[964][40] = 1;
TABLE[965][289] = 1;
TABLE[1386][91] = 1;
TABLE[1386][42] = 1;
TABLE[1386][335] = 1;
TABLE[1386][124] = 1;
TABLE[1386][59] = 1;
TABLE[969][91] = 1;
TABLE[1388][42] = 1;
TABLE[1388][335] = 1;
TABLE[1388][124] = 1;
TABLE[1388][59] = 1;
TABLE[968][124] = 1;
TABLE[968][44] = 1;
TABLE[1387][93] = 1;
TABLE[966][124] = 1;
TABLE[967][44] = 1;
TABLE[361][299] = 1;
TABLE[970][436] = 1;
TABLE[970][350] = 1;
TABLE[970][400] = 1;
TABLE[970][434] = 1;
TABLE[970][377] = 1;
TABLE[970][311] = 1;
TABLE[970][264] = 1;
TABLE[970][274] = 1;
TABLE[970][268] = 1;
TABLE[970][265] = 1;
TABLE[970][266] = 1;
TABLE[970][267] = 1;
TABLE[970][263] = 1;
TABLE[970][271] = 1;
TABLE[970][272] = 1;
TABLE[970][273] = 1;
TABLE[970][474] = 1;
TABLE[1389][125] = 1;
TABLE[362][436] = 1;
TABLE[363][350] = 1;
TABLE[364][400] = 1;
TABLE[365][311] = 1;
TABLE[365][377] = 1;
TABLE[365][434] = 1;
TABLE[366][474] = 1;
TABLE[366][273] = 1;
TABLE[366][272] = 1;
TABLE[366][271] = 1;
TABLE[366][263] = 1;
TABLE[366][267] = 1;
TABLE[366][266] = 1;
TABLE[366][265] = 1;
TABLE[366][268] = 1;
TABLE[366][274] = 1;
TABLE[366][264] = 1;
TABLE[971][58] = 1;
TABLE[1390][36] = 1;
TABLE[972][58] = 1;
TABLE[1391][45] = 1;
TABLE[1391][43] = 1;
TABLE[1391][40] = 1;
TABLE[1391][256] = 1;
TABLE[1391][458] = 1;
TABLE[1391][345] = 1;
TABLE[1391][259] = 1;
TABLE[1391][59] = 1;
TABLE[975][43] = 1;
TABLE[975][45] = 1;
TABLE[975][259] = 1;
TABLE[975][345] = 1;
TABLE[975][458] = 1;
TABLE[975][256] = 1;
TABLE[975][40] = 1;
TABLE[1393][59] = 1;
TABLE[973][124] = 1;
TABLE[974][44] = 1;
TABLE[1392][45] = 1;
TABLE[1392][43] = 1;
TABLE[1392][40] = 1;
TABLE[1392][256] = 1;
TABLE[1392][458] = 1;
TABLE[1392][345] = 1;
TABLE[1392][259] = 1;
TABLE[1392][59] = 1;
TABLE[367][451] = 1;
TABLE[976][257] = 1;
TABLE[977][36] = 1;
TABLE[978][40] = 1;
TABLE[979][41] = 1;
TABLE[980][124] = 1;
TABLE[981][59] = 1;
TABLE[982][91] = 1;
TABLE[983][91] = 1;
TABLE[1394][59] = 1;
TABLE[368][261] = 1;
TABLE[369][264] = 1;
TABLE[370][274] = 1;
TABLE[371][268] = 1;
TABLE[372][265] = 1;
TABLE[373][266] = 1;
TABLE[374][267] = 1;
TABLE[375][263] = 1;
TABLE[376][271] = 1;
TABLE[377][272] = 1;
TABLE[378][273] = 1;
TABLE[379][474] = 1;
TABLE[380][447] = 1;
TABLE[991][264] = 1;
TABLE[991][274] = 1;
TABLE[991][268] = 1;
TABLE[991][265] = 1;
TABLE[991][266] = 1;
TABLE[991][267] = 1;
TABLE[991][263] = 1;
TABLE[991][271] = 1;
TABLE[991][272] = 1;
TABLE[991][273] = 1;
TABLE[991][474] = 1;
TABLE[1402][44] = 1;
TABLE[1402][41] = 1;
TABLE[984][58] = 1;
TABLE[1395][258] = 1;
TABLE[1395][61] = 1;
TABLE[1395][44] = 1;
TABLE[1395][41] = 1;
TABLE[985][258] = 1;
TABLE[1396][61] = 1;
TABLE[1396][44] = 1;
TABLE[1396][41] = 1;
TABLE[986][61] = 1;
TABLE[1397][44] = 1;
TABLE[1397][41] = 1;
TABLE[990][44] = 1;
TABLE[1401][44] = 1;
TABLE[1401][41] = 1;
TABLE[987][58] = 1;
TABLE[1398][258] = 1;
TABLE[1398][61] = 1;
TABLE[1398][44] = 1;
TABLE[1398][41] = 1;
TABLE[988][258] = 1;
TABLE[1399][61] = 1;
TABLE[1399][44] = 1;
TABLE[1399][41] = 1;
TABLE[989][61] = 1;
TABLE[1400][44] = 1;
TABLE[1400][41] = 1;
TABLE[992][44] = 1;
TABLE[1403][41] = 1;
TABLE[381][447] = 1;
TABLE[1000][264] = 1;
TABLE[1000][274] = 1;
TABLE[1000][268] = 1;
TABLE[1000][265] = 1;
TABLE[1000][266] = 1;
TABLE[1000][267] = 1;
TABLE[1000][263] = 1;
TABLE[1000][271] = 1;
TABLE[1000][272] = 1;
TABLE[1000][273] = 1;
TABLE[1000][474] = 1;
TABLE[1411][44] = 1;
TABLE[1411][41] = 1;
TABLE[993][58] = 1;
TABLE[1404][258] = 1;
TABLE[1404][61] = 1;
TABLE[1404][44] = 1;
TABLE[1404][41] = 1;
TABLE[994][258] = 1;
TABLE[1405][61] = 1;
TABLE[1405][44] = 1;
TABLE[1405][41] = 1;
TABLE[995][61] = 1;
TABLE[1406][44] = 1;
TABLE[1406][41] = 1;
TABLE[999][44] = 1;
TABLE[1410][44] = 1;
TABLE[1410][41] = 1;
TABLE[996][58] = 1;
TABLE[1407][258] = 1;
TABLE[1407][61] = 1;
TABLE[1407][44] = 1;
TABLE[1407][41] = 1;
TABLE[997][258] = 1;
TABLE[1408][61] = 1;
TABLE[1408][44] = 1;
TABLE[1408][41] = 1;
TABLE[998][61] = 1;
TABLE[1409][44] = 1;
TABLE[1409][41] = 1;
TABLE[1001][44] = 1;
TABLE[1412][41] = 1;
TABLE[382][447] = 1;
TABLE[383][447] = 1;
TABLE[384][264] = 1;
TABLE[384][274] = 1;
TABLE[384][268] = 1;
TABLE[384][265] = 1;
TABLE[384][266] = 1;
TABLE[384][267] = 1;
TABLE[384][263] = 1;
TABLE[384][271] = 1;
TABLE[384][272] = 1;
TABLE[384][273] = 1;
TABLE[384][474] = 1;
TABLE[1002][44] = 1;
TABLE[1413][59] = 1;
TABLE[385][264] = 1;
TABLE[385][274] = 1;
TABLE[385][268] = 1;
TABLE[385][265] = 1;
TABLE[385][266] = 1;
TABLE[385][267] = 1;
TABLE[385][263] = 1;
TABLE[385][271] = 1;
TABLE[385][272] = 1;
TABLE[385][273] = 1;
TABLE[385][474] = 1;
TABLE[1003][44] = 1;
TABLE[1414][59] = 1;
TABLE[386][264] = 1;
TABLE[386][274] = 1;
TABLE[386][268] = 1;
TABLE[386][265] = 1;
TABLE[386][266] = 1;
TABLE[386][267] = 1;
TABLE[386][263] = 1;
TABLE[386][271] = 1;
TABLE[386][272] = 1;
TABLE[386][273] = 1;
TABLE[386][474] = 1;
TABLE[1004][44] = 1;
TABLE[1415][59] = 1;
TABLE[387][475] = 1;
TABLE[387][476] = 1;
TABLE[387][43] = 1;
TABLE[387][45] = 1;
TABLE[387][33] = 1;
TABLE[387][126] = 1;
TABLE[387][58] = 1;
TABLE[387][384] = 1;
TABLE[387][387] = 1;
TABLE[387][258] = 1;
TABLE[387][40] = 1;
TABLE[387][36] = 1;
TABLE[387][395] = 1;
TABLE[387][345] = 1;
TABLE[387][458] = 1;
TABLE[387][257] = 1;
TABLE[387][259] = 1;
TABLE[387][256] = 1;
TABLE[388][367] = 1;
TABLE[389][367] = 1;
TABLE[390][353] = 1;
TABLE[391][446] = 1;
TABLE[392][302] = 1;
TABLE[393][314] = 1;
TABLE[394][331] = 1;
TABLE[395][469] = 1;
TABLE[396][123] = 1;
TABLE[397][425] = 1;
TABLE[398][425] = 1;
TABLE[1008][305] = 1;
TABLE[1417][324] = 1;
TABLE[1417][125] = 1;
TABLE[1005][43] = 1;
TABLE[1005][45] = 1;
TABLE[1005][259] = 1;
TABLE[1005][345] = 1;
TABLE[1005][458] = 1;
TABLE[1005][256] = 1;
TABLE[1005][40] = 1;
TABLE[1006][36] = 1;
TABLE[1007][367] = 1;
TABLE[1007][353] = 1;
TABLE[1007][446] = 1;
TABLE[1007][302] = 1;
TABLE[1007][314] = 1;
TABLE[1007][331] = 1;
TABLE[1007][469] = 1;
TABLE[1007][425] = 1;
TABLE[1007][256] = 1;
TABLE[1007][259] = 1;
TABLE[1007][257] = 1;
TABLE[1007][458] = 1;
TABLE[1007][345] = 1;
TABLE[1007][395] = 1;
TABLE[1007][36] = 1;
TABLE[1007][40] = 1;
TABLE[1007][258] = 1;
TABLE[1007][387] = 1;
TABLE[1007][384] = 1;
TABLE[1007][58] = 1;
TABLE[1007][126] = 1;
TABLE[1007][33] = 1;
TABLE[1007][45] = 1;
TABLE[1007][43] = 1;
TABLE[1007][476] = 1;
TABLE[1007][475] = 1;
TABLE[1007][123] = 1;
TABLE[1416][305] = 1;
TABLE[1416][324] = 1;
TABLE[1416][125] = 1;
TABLE[1010][324] = 1;
TABLE[1419][125] = 1;
TABLE[1009][367] = 1;
TABLE[1009][353] = 1;
TABLE[1009][446] = 1;
TABLE[1009][302] = 1;
TABLE[1009][314] = 1;
TABLE[1009][331] = 1;
TABLE[1009][469] = 1;
TABLE[1009][425] = 1;
TABLE[1009][256] = 1;
TABLE[1009][259] = 1;
TABLE[1009][257] = 1;
TABLE[1009][458] = 1;
TABLE[1009][345] = 1;
TABLE[1009][395] = 1;
TABLE[1009][36] = 1;
TABLE[1009][40] = 1;
TABLE[1009][258] = 1;
TABLE[1009][387] = 1;
TABLE[1009][384] = 1;
TABLE[1009][58] = 1;
TABLE[1009][126] = 1;
TABLE[1009][33] = 1;
TABLE[1009][45] = 1;
TABLE[1009][43] = 1;
TABLE[1009][476] = 1;
TABLE[1009][475] = 1;
TABLE[1009][123] = 1;
TABLE[1418][125] = 1;
TABLE[399][123] = 1;
TABLE[1011][367] = 1;
TABLE[1011][353] = 1;
TABLE[1011][446] = 1;
TABLE[1011][302] = 1;
TABLE[1011][314] = 1;
TABLE[1011][331] = 1;
TABLE[1011][469] = 1;
TABLE[1011][425] = 1;
TABLE[1011][256] = 1;
TABLE[1011][259] = 1;
TABLE[1011][257] = 1;
TABLE[1011][458] = 1;
TABLE[1011][345] = 1;
TABLE[1011][395] = 1;
TABLE[1011][36] = 1;
TABLE[1011][40] = 1;
TABLE[1011][258] = 1;
TABLE[1011][387] = 1;
TABLE[1011][384] = 1;
TABLE[1011][58] = 1;
TABLE[1011][126] = 1;
TABLE[1011][33] = 1;
TABLE[1011][45] = 1;
TABLE[1011][43] = 1;
TABLE[1011][476] = 1;
TABLE[1011][475] = 1;
TABLE[1011][123] = 1;
TABLE[1420][125] = 1;
TABLE[400][123] = 1;
TABLE[1012][474] = 1;
TABLE[1012][273] = 1;
TABLE[1012][272] = 1;
TABLE[1012][271] = 1;
TABLE[1012][263] = 1;
TABLE[1012][267] = 1;
TABLE[1012][266] = 1;
TABLE[1012][265] = 1;
TABLE[1012][268] = 1;
TABLE[1012][274] = 1;
TABLE[1012][264] = 1;
TABLE[1013][441] = 1;
TABLE[1421][425] = 1;
TABLE[1421][469] = 1;
TABLE[1421][331] = 1;
TABLE[1421][314] = 1;
TABLE[1421][302] = 1;
TABLE[1421][446] = 1;
TABLE[1421][353] = 1;
TABLE[1421][367] = 1;
TABLE[1421][123] = 1;
TABLE[1421][475] = 1;
TABLE[1421][476] = 1;
TABLE[1421][43] = 1;
TABLE[1421][45] = 1;
TABLE[1421][33] = 1;
TABLE[1421][126] = 1;
TABLE[1421][58] = 1;
TABLE[1421][384] = 1;
TABLE[1421][387] = 1;
TABLE[1421][258] = 1;
TABLE[1421][40] = 1;
TABLE[1421][36] = 1;
TABLE[1421][395] = 1;
TABLE[1421][345] = 1;
TABLE[1421][458] = 1;
TABLE[1421][257] = 1;
TABLE[1421][259] = 1;
TABLE[1421][256] = 1;
TABLE[1421][125] = 1;
TABLE[1014][367] = 1;
TABLE[1014][353] = 1;
TABLE[1014][446] = 1;
TABLE[1014][302] = 1;
TABLE[1014][314] = 1;
TABLE[1014][331] = 1;
TABLE[1014][469] = 1;
TABLE[1014][425] = 1;
TABLE[1014][256] = 1;
TABLE[1014][259] = 1;
TABLE[1014][257] = 1;
TABLE[1014][458] = 1;
TABLE[1014][345] = 1;
TABLE[1014][395] = 1;
TABLE[1014][36] = 1;
TABLE[1014][40] = 1;
TABLE[1014][258] = 1;
TABLE[1014][387] = 1;
TABLE[1014][384] = 1;
TABLE[1014][58] = 1;
TABLE[1014][126] = 1;
TABLE[1014][33] = 1;
TABLE[1014][45] = 1;
TABLE[1014][43] = 1;
TABLE[1014][476] = 1;
TABLE[1014][475] = 1;
TABLE[1014][123] = 1;
TABLE[1422][125] = 1;
TABLE[401][256] = 1;
TABLE[402][259] = 1;
TABLE[403][257] = 1;
TABLE[404][458] = 1;
TABLE[405][345] = 1;
TABLE[406][395] = 1;
TABLE[407][36] = 1;
TABLE[408][40] = 1;
TABLE[409][258] = 1;
TABLE[410][256] = 1;
TABLE[410][259] = 1;
TABLE[410][257] = 1;
TABLE[410][458] = 1;
TABLE[410][345] = 1;
TABLE[410][395] = 1;
TABLE[410][36] = 1;
TABLE[410][40] = 1;
TABLE[410][258] = 1;
TABLE[411][256] = 1;
TABLE[411][259] = 1;
TABLE[411][257] = 1;
TABLE[411][458] = 1;
TABLE[411][345] = 1;
TABLE[411][395] = 1;
TABLE[411][36] = 1;
TABLE[411][40] = 1;
TABLE[411][258] = 1;
TABLE[412][258] = 1;
TABLE[413][256] = 1;
TABLE[413][259] = 1;
TABLE[413][257] = 1;
TABLE[413][458] = 1;
TABLE[413][345] = 1;
TABLE[413][395] = 1;
TABLE[413][36] = 1;
TABLE[413][40] = 1;
TABLE[413][258] = 1;
TABLE[1015][46] = 1;
TABLE[1016][496] = 1;
TABLE[1017][46] = 1;
TABLE[1018][496] = 1;
TABLE[414][258] = 1;
TABLE[415][258] = 1;
TABLE[415][40] = 1;
TABLE[415][36] = 1;
TABLE[415][395] = 1;
TABLE[415][345] = 1;
TABLE[415][458] = 1;
TABLE[415][257] = 1;
TABLE[415][259] = 1;
TABLE[415][256] = 1;
TABLE[416][256] = 1;
TABLE[416][259] = 1;
TABLE[416][257] = 1;
TABLE[416][458] = 1;
TABLE[416][345] = 1;
TABLE[416][395] = 1;
TABLE[416][36] = 1;
TABLE[416][40] = 1;
TABLE[416][258] = 1;
TABLE[1019][46] = 1;
TABLE[1020][496] = 1;
TABLE[417][258] = 1;
TABLE[417][40] = 1;
TABLE[417][36] = 1;
TABLE[417][395] = 1;
TABLE[417][345] = 1;
TABLE[417][458] = 1;
TABLE[417][257] = 1;
TABLE[417][259] = 1;
TABLE[417][256] = 1;
TABLE[418][258] = 1;
TABLE[418][40] = 1;
TABLE[418][36] = 1;
TABLE[418][395] = 1;
TABLE[418][345] = 1;
TABLE[418][458] = 1;
TABLE[418][257] = 1;
TABLE[418][259] = 1;
TABLE[418][256] = 1;
TABLE[419][387] = 1;
TABLE[420][384] = 1;
TABLE[421][256] = 1;
TABLE[421][259] = 1;
TABLE[421][257] = 1;
TABLE[421][458] = 1;
TABLE[421][345] = 1;
TABLE[421][395] = 1;
TABLE[421][36] = 1;
TABLE[421][40] = 1;
TABLE[421][258] = 1;
TABLE[422][475] = 1;
TABLE[423][476] = 1;
TABLE[424][43] = 1;
TABLE[425][45] = 1;
TABLE[426][33] = 1;
TABLE[427][126] = 1;
TABLE[428][58] = 1;
TABLE[429][256] = 1;
TABLE[429][259] = 1;
TABLE[429][257] = 1;
TABLE[429][458] = 1;
TABLE[429][345] = 1;
TABLE[429][395] = 1;
TABLE[429][36] = 1;
TABLE[429][40] = 1;
TABLE[429][258] = 1;
TABLE[429][387] = 1;
TABLE[429][384] = 1;
TABLE[430][475] = 1;
TABLE[430][476] = 1;
TABLE[430][43] = 1;
TABLE[430][45] = 1;
TABLE[430][33] = 1;
TABLE[430][126] = 1;
TABLE[430][58] = 1;
TABLE[430][384] = 1;
TABLE[430][387] = 1;
TABLE[430][258] = 1;
TABLE[430][40] = 1;
TABLE[430][36] = 1;
TABLE[430][395] = 1;
TABLE[430][345] = 1;
TABLE[430][458] = 1;
TABLE[430][257] = 1;
TABLE[430][259] = 1;
TABLE[430][256] = 1;
TABLE[431][475] = 1;
TABLE[431][476] = 1;
TABLE[431][43] = 1;
TABLE[431][45] = 1;
TABLE[431][33] = 1;
TABLE[431][126] = 1;
TABLE[431][58] = 1;
TABLE[431][384] = 1;
TABLE[431][387] = 1;
TABLE[431][258] = 1;
TABLE[431][40] = 1;
TABLE[431][36] = 1;
TABLE[431][395] = 1;
TABLE[431][345] = 1;
TABLE[431][458] = 1;
TABLE[431][257] = 1;
TABLE[431][259] = 1;
TABLE[431][256] = 1;
TABLE[432][40] = 1;
TABLE[433][256] = 1;
TABLE[433][259] = 1;
TABLE[433][257] = 1;
TABLE[433][458] = 1;
TABLE[433][345] = 1;
TABLE[433][395] = 1;
TABLE[433][36] = 1;
TABLE[433][40] = 1;
TABLE[433][258] = 1;
TABLE[433][387] = 1;
TABLE[433][384] = 1;
TABLE[433][58] = 1;
TABLE[433][126] = 1;
TABLE[433][33] = 1;
TABLE[433][45] = 1;
TABLE[433][43] = 1;
TABLE[433][476] = 1;
TABLE[433][475] = 1;
TABLE[434][256] = 1;
TABLE[434][259] = 1;
TABLE[434][257] = 1;
TABLE[434][458] = 1;
TABLE[434][345] = 1;
TABLE[434][395] = 1;
TABLE[434][36] = 1;
TABLE[434][258] = 1;
TABLE[434][387] = 1;
TABLE[434][384] = 1;
TABLE[434][58] = 1;
TABLE[434][126] = 1;
TABLE[434][33] = 1;
TABLE[434][45] = 1;
TABLE[434][43] = 1;
TABLE[434][476] = 1;
TABLE[434][475] = 1;
TABLE[434][40] = 1;
TABLE[435][256] = 1;
TABLE[435][259] = 1;
TABLE[435][257] = 1;
TABLE[435][458] = 1;
TABLE[435][345] = 1;
TABLE[435][395] = 1;
TABLE[435][36] = 1;
TABLE[435][258] = 1;
TABLE[435][387] = 1;
TABLE[435][384] = 1;
TABLE[435][58] = 1;
TABLE[435][126] = 1;
TABLE[435][33] = 1;
TABLE[435][45] = 1;
TABLE[435][43] = 1;
TABLE[435][476] = 1;
TABLE[435][475] = 1;
TABLE[435][40] = 1;
TABLE[436][256] = 1;
TABLE[436][259] = 1;
TABLE[436][257] = 1;
TABLE[436][458] = 1;
TABLE[436][345] = 1;
TABLE[436][395] = 1;
TABLE[436][36] = 1;
TABLE[436][258] = 1;
TABLE[436][387] = 1;
TABLE[436][384] = 1;
TABLE[436][58] = 1;
TABLE[436][126] = 1;
TABLE[436][33] = 1;
TABLE[436][45] = 1;
TABLE[436][43] = 1;
TABLE[436][476] = 1;
TABLE[436][475] = 1;
TABLE[436][40] = 1;
TABLE[437][40] = 1;
TABLE[437][475] = 1;
TABLE[437][476] = 1;
TABLE[437][43] = 1;
TABLE[437][45] = 1;
TABLE[437][33] = 1;
TABLE[437][126] = 1;
TABLE[437][58] = 1;
TABLE[437][384] = 1;
TABLE[437][387] = 1;
TABLE[437][258] = 1;
TABLE[437][36] = 1;
TABLE[437][395] = 1;
TABLE[437][345] = 1;
TABLE[437][458] = 1;
TABLE[437][257] = 1;
TABLE[437][259] = 1;
TABLE[437][256] = 1;
TABLE[438][40] = 1;
TABLE[438][475] = 1;
TABLE[438][476] = 1;
TABLE[438][43] = 1;
TABLE[438][45] = 1;
TABLE[438][33] = 1;
TABLE[438][126] = 1;
TABLE[438][58] = 1;
TABLE[438][384] = 1;
TABLE[438][387] = 1;
TABLE[438][258] = 1;
TABLE[438][36] = 1;
TABLE[438][395] = 1;
TABLE[438][345] = 1;
TABLE[438][458] = 1;
TABLE[438][257] = 1;
TABLE[438][259] = 1;
TABLE[438][256] = 1;
TABLE[439][40] = 1;
TABLE[439][475] = 1;
TABLE[439][476] = 1;
TABLE[439][43] = 1;
TABLE[439][45] = 1;
TABLE[439][33] = 1;
TABLE[439][126] = 1;
TABLE[439][58] = 1;
TABLE[439][384] = 1;
TABLE[439][387] = 1;
TABLE[439][258] = 1;
TABLE[439][36] = 1;
TABLE[439][395] = 1;
TABLE[439][345] = 1;
TABLE[439][458] = 1;
TABLE[439][257] = 1;
TABLE[439][259] = 1;
TABLE[439][256] = 1;
TABLE[440][256] = 1;
TABLE[440][259] = 1;
TABLE[440][257] = 1;
TABLE[440][458] = 1;
TABLE[440][345] = 1;
TABLE[440][395] = 1;
TABLE[440][36] = 1;
TABLE[440][258] = 1;
TABLE[440][387] = 1;
TABLE[440][384] = 1;
TABLE[440][58] = 1;
TABLE[440][126] = 1;
TABLE[440][33] = 1;
TABLE[440][45] = 1;
TABLE[440][43] = 1;
TABLE[440][476] = 1;
TABLE[440][475] = 1;
TABLE[440][40] = 1;
TABLE[441][256] = 1;
TABLE[441][259] = 1;
TABLE[441][257] = 1;
TABLE[441][458] = 1;
TABLE[441][345] = 1;
TABLE[441][395] = 1;
TABLE[441][36] = 1;
TABLE[441][258] = 1;
TABLE[441][387] = 1;
TABLE[441][384] = 1;
TABLE[441][58] = 1;
TABLE[441][126] = 1;
TABLE[441][33] = 1;
TABLE[441][45] = 1;
TABLE[441][43] = 1;
TABLE[441][476] = 1;
TABLE[441][475] = 1;
TABLE[441][40] = 1;
TABLE[442][256] = 1;
TABLE[442][259] = 1;
TABLE[442][257] = 1;
TABLE[442][458] = 1;
TABLE[442][345] = 1;
TABLE[442][395] = 1;
TABLE[442][36] = 1;
TABLE[442][258] = 1;
TABLE[442][387] = 1;
TABLE[442][384] = 1;
TABLE[442][58] = 1;
TABLE[442][126] = 1;
TABLE[442][33] = 1;
TABLE[442][45] = 1;
TABLE[442][43] = 1;
TABLE[442][476] = 1;
TABLE[442][475] = 1;
TABLE[442][40] = 1;
TABLE[443][40] = 1;
TABLE[443][475] = 1;
TABLE[443][476] = 1;
TABLE[443][43] = 1;
TABLE[443][45] = 1;
TABLE[443][33] = 1;
TABLE[443][126] = 1;
TABLE[443][58] = 1;
TABLE[443][384] = 1;
TABLE[443][387] = 1;
TABLE[443][258] = 1;
TABLE[443][36] = 1;
TABLE[443][395] = 1;
TABLE[443][345] = 1;
TABLE[443][458] = 1;
TABLE[443][257] = 1;
TABLE[443][259] = 1;
TABLE[443][256] = 1;
TABLE[444][40] = 1;
TABLE[444][475] = 1;
TABLE[444][476] = 1;
TABLE[444][43] = 1;
TABLE[444][45] = 1;
TABLE[444][33] = 1;
TABLE[444][126] = 1;
TABLE[444][58] = 1;
TABLE[444][384] = 1;
TABLE[444][387] = 1;
TABLE[444][258] = 1;
TABLE[444][36] = 1;
TABLE[444][395] = 1;
TABLE[444][345] = 1;
TABLE[444][458] = 1;
TABLE[444][257] = 1;
TABLE[444][259] = 1;
TABLE[444][256] = 1;
TABLE[445][40] = 1;
TABLE[445][475] = 1;
TABLE[445][476] = 1;
TABLE[445][43] = 1;
TABLE[445][45] = 1;
TABLE[445][33] = 1;
TABLE[445][126] = 1;
TABLE[445][58] = 1;
TABLE[445][384] = 1;
TABLE[445][387] = 1;
TABLE[445][258] = 1;
TABLE[445][36] = 1;
TABLE[445][395] = 1;
TABLE[445][345] = 1;
TABLE[445][458] = 1;
TABLE[445][257] = 1;
TABLE[445][259] = 1;
TABLE[445][256] = 1;
TABLE[446][40] = 1;
TABLE[446][475] = 1;
TABLE[446][476] = 1;
TABLE[446][43] = 1;
TABLE[446][45] = 1;
TABLE[446][33] = 1;
TABLE[446][126] = 1;
TABLE[446][58] = 1;
TABLE[446][384] = 1;
TABLE[446][387] = 1;
TABLE[446][258] = 1;
TABLE[446][36] = 1;
TABLE[446][395] = 1;
TABLE[446][345] = 1;
TABLE[446][458] = 1;
TABLE[446][257] = 1;
TABLE[446][259] = 1;
TABLE[446][256] = 1;
TABLE[447][40] = 1;
TABLE[447][475] = 1;
TABLE[447][476] = 1;
TABLE[447][43] = 1;
TABLE[447][45] = 1;
TABLE[447][33] = 1;
TABLE[447][126] = 1;
TABLE[447][58] = 1;
TABLE[447][384] = 1;
TABLE[447][387] = 1;
TABLE[447][258] = 1;
TABLE[447][36] = 1;
TABLE[447][395] = 1;
TABLE[447][345] = 1;
TABLE[447][458] = 1;
TABLE[447][257] = 1;
TABLE[447][259] = 1;
TABLE[447][256] = 1;
TABLE[448][40] = 1;
TABLE[448][475] = 1;
TABLE[448][476] = 1;
TABLE[448][43] = 1;
TABLE[448][45] = 1;
TABLE[448][33] = 1;
TABLE[448][126] = 1;
TABLE[448][58] = 1;
TABLE[448][384] = 1;
TABLE[448][387] = 1;
TABLE[448][258] = 1;
TABLE[448][36] = 1;
TABLE[448][395] = 1;
TABLE[448][345] = 1;
TABLE[448][458] = 1;
TABLE[448][257] = 1;
TABLE[448][259] = 1;
TABLE[448][256] = 1;
TABLE[449][40] = 1;
TABLE[449][475] = 1;
TABLE[449][476] = 1;
TABLE[449][43] = 1;
TABLE[449][45] = 1;
TABLE[449][33] = 1;
TABLE[449][126] = 1;
TABLE[449][58] = 1;
TABLE[449][384] = 1;
TABLE[449][387] = 1;
TABLE[449][258] = 1;
TABLE[449][36] = 1;
TABLE[449][395] = 1;
TABLE[449][345] = 1;
TABLE[449][458] = 1;
TABLE[449][257] = 1;
TABLE[449][259] = 1;
TABLE[449][256] = 1;
TABLE[450][256] = 1;
TABLE[450][259] = 1;
TABLE[450][257] = 1;
TABLE[450][458] = 1;
TABLE[450][345] = 1;
TABLE[450][395] = 1;
TABLE[450][36] = 1;
TABLE[450][258] = 1;
TABLE[450][387] = 1;
TABLE[450][384] = 1;
TABLE[450][58] = 1;
TABLE[450][126] = 1;
TABLE[450][33] = 1;
TABLE[450][45] = 1;
TABLE[450][43] = 1;
TABLE[450][476] = 1;
TABLE[450][475] = 1;
TABLE[450][40] = 1;
TABLE[451][256] = 1;
TABLE[451][259] = 1;
TABLE[451][257] = 1;
TABLE[451][458] = 1;
TABLE[451][345] = 1;
TABLE[451][395] = 1;
TABLE[451][36] = 1;
TABLE[451][258] = 1;
TABLE[451][387] = 1;
TABLE[451][384] = 1;
TABLE[451][58] = 1;
TABLE[451][126] = 1;
TABLE[451][33] = 1;
TABLE[451][45] = 1;
TABLE[451][43] = 1;
TABLE[451][476] = 1;
TABLE[451][475] = 1;
TABLE[451][40] = 1;
TABLE[452][40] = 1;
TABLE[452][475] = 1;
TABLE[452][476] = 1;
TABLE[452][43] = 1;
TABLE[452][45] = 1;
TABLE[452][33] = 1;
TABLE[452][126] = 1;
TABLE[452][58] = 1;
TABLE[452][384] = 1;
TABLE[452][387] = 1;
TABLE[452][258] = 1;
TABLE[452][36] = 1;
TABLE[452][395] = 1;
TABLE[452][345] = 1;
TABLE[452][458] = 1;
TABLE[452][257] = 1;
TABLE[452][259] = 1;
TABLE[452][256] = 1;
TABLE[453][40] = 1;
TABLE[453][475] = 1;
TABLE[453][476] = 1;
TABLE[453][43] = 1;
TABLE[453][45] = 1;
TABLE[453][33] = 1;
TABLE[453][126] = 1;
TABLE[453][58] = 1;
TABLE[453][384] = 1;
TABLE[453][387] = 1;
TABLE[453][258] = 1;
TABLE[453][36] = 1;
TABLE[453][395] = 1;
TABLE[453][345] = 1;
TABLE[453][458] = 1;
TABLE[453][257] = 1;
TABLE[453][259] = 1;
TABLE[453][256] = 1;
TABLE[454][256] = 1;
TABLE[454][259] = 1;
TABLE[454][257] = 1;
TABLE[454][458] = 1;
TABLE[454][345] = 1;
TABLE[454][395] = 1;
TABLE[454][36] = 1;
TABLE[454][258] = 1;
TABLE[454][387] = 1;
TABLE[454][384] = 1;
TABLE[454][58] = 1;
TABLE[454][126] = 1;
TABLE[454][33] = 1;
TABLE[454][45] = 1;
TABLE[454][43] = 1;
TABLE[454][476] = 1;
TABLE[454][475] = 1;
TABLE[454][40] = 1;
TABLE[455][256] = 1;
TABLE[455][259] = 1;
TABLE[455][257] = 1;
TABLE[455][458] = 1;
TABLE[455][345] = 1;
TABLE[455][395] = 1;
TABLE[455][36] = 1;
TABLE[455][258] = 1;
TABLE[455][387] = 1;
TABLE[455][384] = 1;
TABLE[455][58] = 1;
TABLE[455][126] = 1;
TABLE[455][33] = 1;
TABLE[455][45] = 1;
TABLE[455][43] = 1;
TABLE[455][476] = 1;
TABLE[455][475] = 1;
TABLE[455][40] = 1;
TABLE[456][40] = 1;
TABLE[456][475] = 1;
TABLE[456][476] = 1;
TABLE[456][43] = 1;
TABLE[456][45] = 1;
TABLE[456][33] = 1;
TABLE[456][126] = 1;
TABLE[456][58] = 1;
TABLE[456][384] = 1;
TABLE[456][387] = 1;
TABLE[456][258] = 1;
TABLE[456][36] = 1;
TABLE[456][395] = 1;
TABLE[456][345] = 1;
TABLE[456][458] = 1;
TABLE[456][257] = 1;
TABLE[456][259] = 1;
TABLE[456][256] = 1;
TABLE[457][40] = 1;
TABLE[457][475] = 1;
TABLE[457][476] = 1;
TABLE[457][43] = 1;
TABLE[457][45] = 1;
TABLE[457][33] = 1;
TABLE[457][126] = 1;
TABLE[457][58] = 1;
TABLE[457][384] = 1;
TABLE[457][387] = 1;
TABLE[457][258] = 1;
TABLE[457][36] = 1;
TABLE[457][395] = 1;
TABLE[457][345] = 1;
TABLE[457][458] = 1;
TABLE[457][257] = 1;
TABLE[457][259] = 1;
TABLE[457][256] = 1;
TABLE[458][256] = 1;
TABLE[458][259] = 1;
TABLE[458][257] = 1;
TABLE[458][458] = 1;
TABLE[458][345] = 1;
TABLE[458][395] = 1;
TABLE[458][36] = 1;
TABLE[458][258] = 1;
TABLE[458][387] = 1;
TABLE[458][384] = 1;
TABLE[458][58] = 1;
TABLE[458][126] = 1;
TABLE[458][33] = 1;
TABLE[458][45] = 1;
TABLE[458][43] = 1;
TABLE[458][476] = 1;
TABLE[458][475] = 1;
TABLE[458][40] = 1;
TABLE[459][256] = 1;
TABLE[459][259] = 1;
TABLE[459][257] = 1;
TABLE[459][458] = 1;
TABLE[459][345] = 1;
TABLE[459][395] = 1;
TABLE[459][36] = 1;
TABLE[459][258] = 1;
TABLE[459][387] = 1;
TABLE[459][384] = 1;
TABLE[459][58] = 1;
TABLE[459][126] = 1;
TABLE[459][33] = 1;
TABLE[459][45] = 1;
TABLE[459][43] = 1;
TABLE[459][476] = 1;
TABLE[459][475] = 1;
TABLE[459][40] = 1;
TABLE[460][40] = 1;
TABLE[460][475] = 1;
TABLE[460][476] = 1;
TABLE[460][43] = 1;
TABLE[460][45] = 1;
TABLE[460][33] = 1;
TABLE[460][126] = 1;
TABLE[460][58] = 1;
TABLE[460][384] = 1;
TABLE[460][387] = 1;
TABLE[460][258] = 1;
TABLE[460][36] = 1;
TABLE[460][395] = 1;
TABLE[460][345] = 1;
TABLE[460][458] = 1;
TABLE[460][257] = 1;
TABLE[460][259] = 1;
TABLE[460][256] = 1;
TABLE[461][256] = 1;
TABLE[461][259] = 1;
TABLE[461][257] = 1;
TABLE[461][458] = 1;
TABLE[461][345] = 1;
TABLE[461][395] = 1;
TABLE[461][36] = 1;
TABLE[461][258] = 1;
TABLE[461][387] = 1;
TABLE[461][384] = 1;
TABLE[461][58] = 1;
TABLE[461][126] = 1;
TABLE[461][33] = 1;
TABLE[461][45] = 1;
TABLE[461][43] = 1;
TABLE[461][476] = 1;
TABLE[461][475] = 1;
TABLE[461][40] = 1;
TABLE[462][256] = 1;
TABLE[462][259] = 1;
TABLE[462][257] = 1;
TABLE[462][458] = 1;
TABLE[462][345] = 1;
TABLE[462][395] = 1;
TABLE[462][36] = 1;
TABLE[462][258] = 1;
TABLE[462][387] = 1;
TABLE[462][384] = 1;
TABLE[462][58] = 1;
TABLE[462][126] = 1;
TABLE[462][33] = 1;
TABLE[462][45] = 1;
TABLE[462][43] = 1;
TABLE[462][476] = 1;
TABLE[462][475] = 1;
TABLE[462][40] = 1;
TABLE[463][258] = 1;
TABLE[463][40] = 1;
TABLE[463][36] = 1;
TABLE[463][395] = 1;
TABLE[463][345] = 1;
TABLE[463][458] = 1;
TABLE[463][257] = 1;
TABLE[463][259] = 1;
TABLE[463][256] = 1;
TABLE[464][258] = 1;
TABLE[464][40] = 1;
TABLE[464][36] = 1;
TABLE[464][395] = 1;
TABLE[464][345] = 1;
TABLE[464][458] = 1;
TABLE[464][257] = 1;
TABLE[464][259] = 1;
TABLE[464][256] = 1;
TABLE[465][258] = 1;
TABLE[465][40] = 1;
TABLE[465][36] = 1;
TABLE[465][395] = 1;
TABLE[465][345] = 1;
TABLE[465][458] = 1;
TABLE[465][257] = 1;
TABLE[465][259] = 1;
TABLE[465][256] = 1;
TABLE[466][258] = 1;
TABLE[466][40] = 1;
TABLE[466][36] = 1;
TABLE[466][395] = 1;
TABLE[466][345] = 1;
TABLE[466][458] = 1;
TABLE[466][257] = 1;
TABLE[466][259] = 1;
TABLE[466][256] = 1;
TABLE[467][258] = 1;
TABLE[467][40] = 1;
TABLE[467][36] = 1;
TABLE[467][395] = 1;
TABLE[467][345] = 1;
TABLE[467][458] = 1;
TABLE[467][257] = 1;
TABLE[467][259] = 1;
TABLE[467][256] = 1;
TABLE[468][258] = 1;
TABLE[468][40] = 1;
TABLE[468][36] = 1;
TABLE[468][395] = 1;
TABLE[468][345] = 1;
TABLE[468][458] = 1;
TABLE[468][257] = 1;
TABLE[468][259] = 1;
TABLE[468][256] = 1;
TABLE[469][258] = 1;
TABLE[469][40] = 1;
TABLE[469][36] = 1;
TABLE[469][395] = 1;
TABLE[469][345] = 1;
TABLE[469][458] = 1;
TABLE[469][257] = 1;
TABLE[469][259] = 1;
TABLE[469][256] = 1;
TABLE[470][258] = 1;
TABLE[470][40] = 1;
TABLE[470][36] = 1;
TABLE[470][395] = 1;
TABLE[470][345] = 1;
TABLE[470][458] = 1;
TABLE[470][257] = 1;
TABLE[470][259] = 1;
TABLE[470][256] = 1;
TABLE[471][258] = 1;
TABLE[471][40] = 1;
TABLE[471][36] = 1;
TABLE[471][395] = 1;
TABLE[471][345] = 1;
TABLE[471][458] = 1;
TABLE[471][257] = 1;
TABLE[471][259] = 1;
TABLE[471][256] = 1;
TABLE[472][258] = 1;
TABLE[472][40] = 1;
TABLE[472][36] = 1;
TABLE[472][395] = 1;
TABLE[472][345] = 1;
TABLE[472][458] = 1;
TABLE[472][257] = 1;
TABLE[472][259] = 1;
TABLE[472][256] = 1;
TABLE[473][258] = 1;
TABLE[473][40] = 1;
TABLE[473][36] = 1;
TABLE[473][395] = 1;
TABLE[473][345] = 1;
TABLE[473][458] = 1;
TABLE[473][257] = 1;
TABLE[473][259] = 1;
TABLE[473][256] = 1;
TABLE[474][40] = 1;
TABLE[474][475] = 1;
TABLE[474][476] = 1;
TABLE[474][43] = 1;
TABLE[474][45] = 1;
TABLE[474][33] = 1;
TABLE[474][126] = 1;
TABLE[474][58] = 1;
TABLE[474][384] = 1;
TABLE[474][387] = 1;
TABLE[474][258] = 1;
TABLE[474][36] = 1;
TABLE[474][395] = 1;
TABLE[474][345] = 1;
TABLE[474][458] = 1;
TABLE[474][257] = 1;
TABLE[474][259] = 1;
TABLE[474][256] = 1;
TABLE[475][475] = 1;
TABLE[475][476] = 1;
TABLE[475][43] = 1;
TABLE[475][45] = 1;
TABLE[475][33] = 1;
TABLE[475][126] = 1;
TABLE[475][58] = 1;
TABLE[475][384] = 1;
TABLE[475][387] = 1;
TABLE[475][258] = 1;
TABLE[475][40] = 1;
TABLE[475][36] = 1;
TABLE[475][395] = 1;
TABLE[475][345] = 1;
TABLE[475][458] = 1;
TABLE[475][257] = 1;
TABLE[475][259] = 1;
TABLE[475][256] = 1;
TABLE[476][256] = 1;
TABLE[476][259] = 1;
TABLE[476][257] = 1;
TABLE[476][458] = 1;
TABLE[476][345] = 1;
TABLE[476][395] = 1;
TABLE[476][36] = 1;
TABLE[476][40] = 1;
TABLE[476][258] = 1;
TABLE[476][387] = 1;
TABLE[476][384] = 1;
TABLE[476][58] = 1;
TABLE[476][126] = 1;
TABLE[476][33] = 1;
TABLE[476][45] = 1;
TABLE[476][43] = 1;
TABLE[476][476] = 1;
TABLE[476][475] = 1;
TABLE[477][41] = 1;
TABLE[477][475] = 1;
TABLE[477][476] = 1;
TABLE[477][43] = 1;
TABLE[477][45] = 1;
TABLE[477][33] = 1;
TABLE[477][126] = 1;
TABLE[477][58] = 1;
TABLE[477][384] = 1;
TABLE[477][387] = 1;
TABLE[477][258] = 1;
TABLE[477][40] = 1;
TABLE[477][36] = 1;
TABLE[477][395] = 1;
TABLE[477][345] = 1;
TABLE[477][458] = 1;
TABLE[477][257] = 1;
TABLE[477][259] = 1;
TABLE[477][256] = 1;
TABLE[1022][256] = 1;
TABLE[1022][259] = 1;
TABLE[1022][257] = 1;
TABLE[1022][458] = 1;
TABLE[1022][345] = 1;
TABLE[1022][395] = 1;
TABLE[1022][36] = 1;
TABLE[1022][40] = 1;
TABLE[1022][258] = 1;
TABLE[1022][387] = 1;
TABLE[1022][384] = 1;
TABLE[1022][58] = 1;
TABLE[1022][126] = 1;
TABLE[1022][33] = 1;
TABLE[1022][45] = 1;
TABLE[1022][43] = 1;
TABLE[1022][476] = 1;
TABLE[1022][475] = 1;
TABLE[1424][41] = 1;
TABLE[1021][44] = 1;
TABLE[1423][41] = 1;
}

extern int yydirset();
int yydirset(int i, int j)
{
   return TABLE[i][j];
}
int yytransparent(int n)
{
   switch(n) {
      case 1: return 1; break;
      case 6: return 1; break;
      case 10: return 1; break;
      case 18: return 1; break;
      case 144: return 0; break;
      case 148: return 1; break;
      case 156: return 1; break;
      case 163: return 0; break;
      case 173: return 0; break;
      case 189: return 0; break;
      case 223: return 0; break;
      case 237: return 0; break;
      case 259: return 0; break;
      case 286: return 0; break;
      case 300: return 1; break;
      case 310: return 1; break;
      case 320: return 1; break;
      case 328: return 1; break;
      case 335: return 1; break;
      case 346: return 0; break;
      case 355: return 1; break;
      case 368: return 1; break;
      case 417: return 1; break;
      case 426: return 1; break;
      case 433: return 0; break;
      case 441: return 0; break;
      case 445: return 1; break;
      case 454: return 0; break;
      case 462: return 0; break;
      case 471: return 0; break;
      case 479: return 1; break;
      case 486: return 1; break;
      case 493: return 0; break;
      case 501: return 1; break;
      case 509: return 1; break;
      case 515: return 1; break;
      case 522: return 0; break;
      case 530: return 1; break;
      case 538: return 0; break;
      case 546: return 1; break;
      case 555: return 0; break;
      case 561: return 1; break;
      case 570: return 0; break;
      case 579: return 1; break;
      case 588: return 0; break;
      case 592: return 0; break;
      case 596: return 1; break;
      case 601: return 1; break;
      case 621: return 1; break;
      case 628: return 1; break;
      case 637: return 0; break;
      case 652: return 0; break;
      case 663: return 1; break;
      case 672: return 1; break;
      case 680: return 1; break;
      case 688: return 1; break;
      case 932: return 1; break;
      case 939: return 1; break;
      case 946: return 0; break;
      case 954: return 1; break;
      case 961: return 0; break;
      case 969: return 1; break;
      case 976: return 1; break;
      case 983: return 1; break;
      case 992: return 1; break;
      case 999: return 1; break;
      case 1006: return 1; break;
      case 1013: return 1; break;
      case 1020: return 1; break;
      case 1027: return 1; break;
      case 1036: return 1; break;
      case 1043: return 1; break;
      case 1050: return 1; break;
      case 1059: return 1; break;
      case 1066: return 1; break;
      case 1073: return 1; break;
      case 1082: return 1; break;
      case 1089: return 1; break;
      case 1096: return 1; break;
      case 1103: return 1; break;
      case 1110: return 1; break;
      case 1119: return 1; break;
      case 1126: return 1; break;
      case 1134: return 1; break;
      case 1143: return 1; break;
      case 1150: return 1; break;
      case 1157: return 1; break;
      case 1164: return 1; break;
      case 1171: return 1; break;
      case 1178: return 1; break;
      case 1185: return 1; break;
      case 1194: return 1; break;
      case 1201: return 1; break;
      case 1208: return 1; break;
      case 1217: return 1; break;
      case 1224: return 1; break;
      case 1231: return 1; break;
      case 1240: return 1; break;
      case 1247: return 0; break;
      case 1266: return 0; break;
      case 1283: return 1; break;
      case 1295: return 1; break;
      case 1302: return 1; break;
      case 1309: return 1; break;
      case 1319: return 0; break;
      case 1331: return 1; break;
      case 1343: return 1; break;
      case 1350: return 1; break;
      case 1357: return 1; break;
      case 1364: return 1; break;
      case 1374: return 0; break;
      case 1386: return 1; break;
      case 1393: return 0; break;
      case 1404: return 1; break;
      case 1411: return 1; break;
      case 1418: return 0; break;
      case 1424: return 1; break;
      case 1431: return 0; break;
      case 1441: return 1; break;
      case 1449: return 0; break;
      case 1715: return 1; break;
      case 1722: return 1; break;
      case 1729: return 1; break;
      case 1736: return 1; break;
      case 1743: return 1; break;
      case 1750: return 1; break;
      case 1757: return 1; break;
      case 1764: return 1; break;
      case 1771: return 1; break;
      case 1778: return 1; break;
      case 1785: return 1; break;
      case 1792: return 1; break;
      case 1799: return 1; break;
      case 1806: return 1; break;
      case 1813: return 1; break;
      case 1820: return 1; break;
      case 1827: return 1; break;
      case 1834: return 1; break;
      case 1841: return 1; break;
      case 1848: return 1; break;
      case 1855: return 1; break;
      case 1862: return 1; break;
      case 1869: return 1; break;
      case 1877: return 1; break;
      case 1884: return 1; break;
      case 1891: return 1; break;
      case 1900: return 1; break;
      case 1907: return 1; break;
      case 1914: return 1; break;
      case 1923: return 1; break;
      case 1930: return 1; break;
      case 1937: return 1; break;
      case 1945: return 1; break;
      case 1952: return 1; break;
      case 1961: return 1; break;
      case 1972: return 1; break;
      case 1979: return 1; break;
      case 1986: return 1; break;
      case 1993: return 1; break;
      case 2000: return 1; break;
      case 2007: return 1; break;
      case 2014: return 1; break;
      case 2021: return 1; break;
      case 2028: return 1; break;
      case 2035: return 0; break;
      case 2046: return 1; break;
      case 2053: return 1; break;
      case 2060: return 0; break;
      case 2071: return 1; break;
      case 2078: return 1; break;
      case 2085: return 0; break;
      case 2095: return 1; break;
      case 2103: return 0; break;
      case 2176: return 1; break;
      case 2183: return 1; break;
      case 2190: return 1; break;
      case 2197: return 1; break;
      case 2204: return 1; break;
      case 2211: return 1; break;
      case 2220: return 1; break;
      case 2231: return 0; break;
      case 2239: return 1; break;
      case 2246: return 1; break;
      case 2253: return 1; break;
      case 2260: return 1; break;
      case 2272: return 1; break;
      case 2281: return 1; break;
      case 2288: return 0; break;
      case 2304: return 1; break;
      case 2312: return 1; break;
      case 2324: return 1; break;
      case 2332: return 0; break;
      case 2486: return 1; break;
      case 2493: return 1; break;
      case 2500: return 1; break;
      case 2507: return 1; break;
      case 2514: return 1; break;
      case 2521: return 1; break;
      case 2528: return 1; break;
      case 2535: return 1; break;
      case 2544: return 1; break;
      case 2551: return 1; break;
      case 2558: return 1; break;
      case 2565: return 1; break;
      case 2572: return 1; break;
      case 2579: return 1; break;
      case 2586: return 1; break;
      case 2593: return 1; break;
      case 2600: return 1; break;
      case 2607: return 1; break;
      case 2614: return 1; break;
      case 2621: return 1; break;
      case 2628: return 1; break;
      case 2635: return 1; break;
      case 2642: return 1; break;
      case 2649: return 1; break;
      case 2658: return 1; break;
      case 2669: return 1; break;
      case 2673: return 1; break;
      case 2682: return 1; break;
      case 2693: return 0; break;
      case 2697: return 0; break;
      case 2706: return 1; break;
      case 2714: return 1; break;
      case 2722: return 1; break;
      case 2807: return 1; break;
      case 2814: return 1; break;
      case 2825: return 1; break;
      case 2832: return 1; break;
      case 2839: return 1; break;
      case 2846: return 1; break;
      case 2855: return 1; break;
      case 2862: return 1; break;
      case 2869: return 1; break;
      case 2876: return 1; break;
      case 2883: return 1; break;
      case 2890: return 1; break;
      case 2897: return 1; break;
      case 2904: return 1; break;
      case 2913: return 1; break;
      case 2920: return 1; break;
      case 2927: return 1; break;
      case 2934: return 1; break;
      case 2943: return 1; break;
      case 2954: return 0; break;
      case 2962: return 1; break;
      case 2970: return 0; break;
      case 3063: return 1; break;
      case 3070: return 1; break;
      case 3077: return 1; break;
      case 3084: return 1; break;
      case 3091: return 1; break;
      case 3098: return 1; break;
      case 3105: return 1; break;
      case 3112: return 1; break;
      case 3122: return 1; break;
      case 3129: return 1; break;
      case 3136: return 1; break;
      case 3146: return 1; break;
      case 3153: return 1; break;
      case 3160: return 1; break;
      case 3169: return 1; break;
      case 3176: return 0; break;
      case 3180: return 0; break;
      case 3184: return 1; break;
      case 3191: return 1; break;
      case 3200: return 1; break;
      case 3207: return 1; break;
      case 3214: return 1; break;
      case 3223: return 1; break;
      case 3230: return 1; break;
      case 3237: return 1; break;
      case 3246: return 1; break;
      case 3257: return 0; break;
      case 3287: return 1; break;
      case 3294: return 1; break;
      case 3301: return 1; break;
      case 3308: return 0; break;
      case 3325: return 0; break;
      case 3360: return 1; break;
      case 3367: return 1; break;
      case 3374: return 1; break;
      case 3381: return 0; break;
      case 3389: return 1; break;
      case 3397: return 0; break;
      case 3416: return 1; break;
      case 3423: return 0; break;
      case 3443: return 1; break;
      case 3450: return 1; break;
      case 3469: return 1; break;
      case 3476: return 1; break;
      case 3483: return 1; break;
      case 3490: return 1; break;
      case 3501: return 1; break;
      case 3509: return 0; break;
      case 3517: return 1; break;
      case 3524: return 1; break;
      case 3535: return 1; break;
      case 3543: return 0; break;
      case 3564: return 1; break;
      case 3571: return 0; break;
      case 3583: return 0; break;
      case 3591: return 1; break;
      case 3599: return 1; break;
      case 3675: return 1; break;
      case 3682: return 1; break;
      case 3689: return 1; break;
      case 3700: return 1; break;
      case 3707: return 1; break;
      case 3714: return 1; break;
      case 3721: return 1; break;
      case 3730: return 1; break;
      case 3737: return 1; break;
      case 3744: return 1; break;
      case 3753: return 1; break;
      case 3760: return 1; break;
      case 3767: return 1; break;
      case 3776: return 1; break;
      case 3783: return 1; break;
      case 3792: return 0; break;
      case 3796: return 1; break;
      case 3805: return 0; break;
      case 3809: return 1; break;
      case 3818: return 0; break;
      case 3822: return 1; break;
      case 3831: return 0; break;
      case 3835: return 0; break;
      case 3887: return 1; break;
      case 3895: return 0; break;
      case 3903: return 0; break;
      case 3911: return 0; break;
      case 3921: return 1; break;
      case 3929: return 0; break;
      case 3962: return 1; break;
      case 3969: return 1; break;
      case 3978: return 1; break;
      case 3985: return 1; break;
      case 3992: return 1; break;
      case 4001: return 1; break;
      case 4008: return 1; break;
      case 4015: return 0; break;
      case 4023: return 1; break;
      case 4031: return 0; break;
      case 4076: return 1; break;
      case 4083: return 1; break;
      case 4092: return 1; break;
      case 4099: return 1; break;
      case 4106: return 1; break;
      case 4115: return 1; break;
      case 4126: return 1; break;
      case 4133: return 1; break;
      case 4140: return 1; break;
      case 4149: return 1; break;
      case 4156: return 0; break;
      case 4164: return 1; break;
      case 4172: return 0; break;
      case 4224: return 1; break;
      case 4231: return 1; break;
      case 4238: return 1; break;
      case 4245: return 1; break;
      case 4252: return 1; break;
      case 4259: return 1; break;
      case 4268: return 1; break;
      case 4279: return 0; break;
      case 4289: return 1; break;
      case 4297: return 0; break;
      case 4391: return 1; break;
      case 4398: return 1; break;
      case 4405: return 1; break;
      case 4412: return 1; break;
      case 4419: return 1; break;
      case 4426: return 1; break;
      case 4433: return 1; break;
      case 4440: return 1; break;
      case 4449: return 1; break;
      case 4460: return 1; break;
      case 4467: return 1; break;
      case 4476: return 1; break;
      case 4487: return 1; break;
      case 4499: return 1; break;
      case 4522: return 0; break;
      case 4538: return 1; break;
      case 4545: return 1; break;
      case 4552: return 1; break;
      case 4573: return 1; break;
      case 4580: return 1; break;
      case 4588: return 1; break;
      case 4596: return 0; break;
      case 4607: return 1; break;
      case 4615: return 1; break;
      case 4623: return 0; break;
      case 4850: return 1; break;
      case 4857: return 1; break;
      case 4864: return 1; break;
      case 4871: return 0; break;
      case 4879: return 1; break;
      case 4886: return 1; break;
      case 4893: return 1; break;
      case 4900: return 1; break;
      case 4907: return 1; break;
      case 4914: return 1; break;
      case 4921: return 1; break;
      case 4928: return 1; break;
      case 4935: return 1; break;
      case 4942: return 1; break;
      case 4949: return 1; break;
      case 4956: return 1; break;
      case 4963: return 0; break;
      case 4971: return 1; break;
      case 4978: return 1; break;
      case 4987: return 1; break;
      case 4994: return 1; break;
      case 5003: return 0; break;
      case 5012: return 0; break;
      case 5020: return 1; break;
      case 5027: return 1; break;
      case 5034: return 1; break;
      case 5043: return 0; break;
      case 5052: return 0; break;
      case 5060: return 1; break;
      case 5067: return 1; break;
      case 5074: return 1; break;
      case 5083: return 1; break;
      case 5090: return 1; break;
      case 5099: return 1; break;
      case 5106: return 1; break;
      case 5113: return 1; break;
      case 5122: return 1; break;
      case 5129: return 1; break;
      case 5136: return 1; break;
      case 5145: return 1; break;
      case 5152: return 1; break;
      case 5159: return 1; break;
      case 5168: return 0; break;
      case 5177: return 0; break;
      case 5185: return 0; break;
      case 5193: return 0; break;
      case 5210: return 1; break;
      case 5222: return 1; break;
      case 5229: return 1; break;
      case 5236: return 1; break;
      case 5246: return 0; break;
      case 5258: return 1; break;
      case 5270: return 1; break;
      case 5277: return 1; break;
      case 5284: return 1; break;
      case 5291: return 1; break;
      case 5301: return 0; break;
      case 5313: return 1; break;
      case 5320: return 0; break;
      case 5324: return 0; break;
      case 5334: return 1; break;
      case 5342: return 0; break;
      case 5371: return 1; break;
      case 5378: return 1; break;
      case 5385: return 1; break;
      case 5392: return 1; break;
      case 5399: return 0; break;
      case 5408: return 1; break;
      case 5416: return 1; break;
      case 5424: return 0; break;
      case 5506: return 0; break;
      case 5514: return 1; break;
      case 5522: return 1; break;
      case 5529: return 1; break;
      case 5538: return 1; break;
      case 5549: return 0; break;
      case 5558: return 0; break;
      case 5570: return 1; break;
      case 5625: return 0; break;
      case 5633: return 0; break;
      case 5646: return 1; break;
      case 5662: return 1; break;
      case 5672: return 1; break;
      case 5681: return 0; break;
      case 5689: return 0; break;
      case 5699: return 1; break;
      case 5707: return 0; break;
      case 5733: return 1; break;
      case 5740: return 1; break;
      case 5747: return 1; break;
      case 5756: return 1; break;
      case 5767: return 0; break;
      case 5774: return 1; break;
      case 5822: return 0; break;
      case 5826: return 0; break;
      case 5870: return 0; break;
      case 5883: return 1; break;
      case 5894: return 1; break;
      case 5902: return 1; break;
      case 5909: return 1; break;
      case 5917: return 1; break;
      case 5929: return 1; break;
      case 5937: return 1; break;
      case 5944: return 1; break;
      case 5952: return 1; break;
      case 5960: return 0; break;
      case 5971: return 1; break;
      case 5982: return 1; break;
      case 5990: return 1; break;
      case 5997: return 1; break;
      case 6005: return 1; break;
      case 6017: return 1; break;
      case 6025: return 1; break;
      case 6032: return 1; break;
      case 6040: return 1; break;
      case 6048: return 0; break;
      case 6057: return 0; break;
      case 6066: return 0; break;
      case 6073: return 1; break;
      case 6082: return 0; break;
      case 6089: return 1; break;
      case 6098: return 0; break;
      case 6105: return 1; break;
      case 6114: return 0; break;
      case 6203: return 1; break;
      case 6214: return 0; break;
      case 6222: return 1; break;
      case 6230: return 1; break;
      case 6239: return 1; break;
      case 6247: return 0; break;
      case 6253: return 1; break;
      case 6261: return 0; break;
      case 6268: return 1; break;
      case 6282: return 1; break;
      case 6290: return 0; break;
      case 6356: return 0; break;
      case 6364: return 0; break;
      case 6372: return 0; break;
      case 6388: return 0; break;
      case 6396: return 0; break;
      case 6428: return 0; break;
      case 6468: return 0; break;
      case 6478: return 0; break;
      case 6489: return 0; break;
      case 6511: return 0; break;
      case 6527: return 0; break;
      case 6543: return 0; break;
      case 6583: return 0; break;
      case 6593: return 0; break;
      case 6603: return 0; break;
      case 6613: return 0; break;
      case 6623: return 0; break;
      case 6633: return 0; break;
      case 6645: return 0; break;
      case 6715: return 0; break;
      case 6725: return 1; break;
      case 6729: return 1; break;
      case 6737: return 1; break;
   }
   return 0;
}
char * yyprintname(int n)
{
   if (n <= 50000)
      switch(n) {
         case 1: return "YYSTART"; break;
         case 6: return "file"; break;
         case 10: return "Subphrase"; break;
         case 18: return "resource"; break;
         case 144: return "Subphrase"; break;
         case 148: return "Subphrase"; break;
         case 156: return "Subphrase"; break;
         case 163: return "res_ref"; break;
         case 173: return "cexpr"; break;
         case 189: return "cexpr2"; break;
         case 223: return "cexpr3"; break;
         case 237: return "cexpr4"; break;
         case 259: return "dice_val"; break;
         case 286: return "dice_only"; break;
         case 300: return "gear_entry"; break;
         case 310: return "Subphrase"; break;
         case 320: return "Subphrase"; break;
         case 328: return "Subphrase"; break;
         case 335: return "Subphrase"; break;
         case 346: return "Subphrase"; break;
         case 355: return "Subphrase"; break;
         case 368: return "Subphrase"; break;
         case 417: return "Subphrase"; break;
         case 426: return "Subphrase"; break;
         case 433: return "gear_desc"; break;
         case 441: return "Subphrase"; break;
         case 445: return "Subphrase"; break;
         case 454: return "Subphrase"; break;
         case 462: return "glyph_entry"; break;
         case 471: return "Subphrase"; break;
         case 479: return "Subphrase"; break;
         case 486: return "Subphrase"; break;
         case 493: return "Subphrase"; break;
         case 501: return "Subphrase"; break;
         case 509: return "glyph_desc"; break;
         case 515: return "Subphrase"; break;
         case 522: return "Subphrase"; break;
         case 530: return "Subphrase"; break;
         case 538: return "event_desc"; break;
         case 546: return "Subphrase"; break;
         case 555: return "event_entry"; break;
         case 561: return "Subphrase"; break;
         case 570: return "Subphrase"; break;
         case 579: return "Subphrase"; break;
         case 588: return "sep_expr"; break;
         case 592: return "sep_stat"; break;
         case 596: return "mval"; break;
         case 601: return "Subphrase"; break;
         case 621: return "Subphrase"; break;
         case 628: return "Subphrase"; break;
         case 637: return "Subphrase"; break;
         case 652: return "monster_def"; break;
         case 663: return "Subphrase"; break;
         case 672: return "Subphrase"; break;
         case 680: return "Subphrase"; break;
         case 688: return "mon_entry"; break;
         case 932: return "Subphrase"; break;
         case 939: return "Subphrase"; break;
         case 946: return "Subphrase"; break;
         case 954: return "Subphrase"; break;
         case 961: return "Subphrase"; break;
         case 969: return "Subphrase"; break;
         case 976: return "Subphrase"; break;
         case 983: return "Subphrase"; break;
         case 992: return "Subphrase"; break;
         case 999: return "Subphrase"; break;
         case 1006: return "Subphrase"; break;
         case 1013: return "Subphrase"; break;
         case 1020: return "Subphrase"; break;
         case 1027: return "Subphrase"; break;
         case 1036: return "Subphrase"; break;
         case 1043: return "Subphrase"; break;
         case 1050: return "Subphrase"; break;
         case 1059: return "Subphrase"; break;
         case 1066: return "Subphrase"; break;
         case 1073: return "Subphrase"; break;
         case 1082: return "Subphrase"; break;
         case 1089: return "Subphrase"; break;
         case 1096: return "Subphrase"; break;
         case 1103: return "Subphrase"; break;
         case 1110: return "Subphrase"; break;
         case 1119: return "Subphrase"; break;
         case 1126: return "Subphrase"; break;
         case 1134: return "Subphrase"; break;
         case 1143: return "Subphrase"; break;
         case 1150: return "Subphrase"; break;
         case 1157: return "Subphrase"; break;
         case 1164: return "Subphrase"; break;
         case 1171: return "Subphrase"; break;
         case 1178: return "Subphrase"; break;
         case 1185: return "Subphrase"; break;
         case 1194: return "Subphrase"; break;
         case 1201: return "Subphrase"; break;
         case 1208: return "Subphrase"; break;
         case 1217: return "Subphrase"; break;
         case 1224: return "Subphrase"; break;
         case 1231: return "Subphrase"; break;
         case 1240: return "Subphrase"; break;
         case 1247: return "color"; break;
         case 1266: return "attack_entry"; break;
         case 1283: return "Subphrase"; break;
         case 1295: return "Subphrase"; break;
         case 1302: return "Subphrase"; break;
         case 1309: return "Subphrase"; break;
         case 1319: return "Subphrase"; break;
         case 1331: return "Subphrase"; break;
         case 1343: return "Subphrase"; break;
         case 1350: return "Subphrase"; break;
         case 1357: return "Subphrase"; break;
         case 1364: return "Subphrase"; break;
         case 1374: return "Subphrase"; break;
         case 1386: return "Subphrase"; break;
         case 1393: return "mflag_entry"; break;
         case 1404: return "Subphrase"; break;
         case 1411: return "Subphrase"; break;
         case 1418: return "attrib_entry"; break;
         case 1424: return "Subphrase"; break;
         case 1431: return "item_def"; break;
         case 1441: return "Subphrase"; break;
         case 1449: return "item_entry"; break;
         case 1715: return "Subphrase"; break;
         case 1722: return "Subphrase"; break;
         case 1729: return "Subphrase"; break;
         case 1736: return "Subphrase"; break;
         case 1743: return "Subphrase"; break;
         case 1750: return "Subphrase"; break;
         case 1757: return "Subphrase"; break;
         case 1764: return "Subphrase"; break;
         case 1771: return "Subphrase"; break;
         case 1778: return "Subphrase"; break;
         case 1785: return "Subphrase"; break;
         case 1792: return "Subphrase"; break;
         case 1799: return "Subphrase"; break;
         case 1806: return "Subphrase"; break;
         case 1813: return "Subphrase"; break;
         case 1820: return "Subphrase"; break;
         case 1827: return "Subphrase"; break;
         case 1834: return "Subphrase"; break;
         case 1841: return "Subphrase"; break;
         case 1848: return "Subphrase"; break;
         case 1855: return "Subphrase"; break;
         case 1862: return "Subphrase"; break;
         case 1869: return "Subphrase"; break;
         case 1877: return "Subphrase"; break;
         case 1884: return "Subphrase"; break;
         case 1891: return "Subphrase"; break;
         case 1900: return "Subphrase"; break;
         case 1907: return "Subphrase"; break;
         case 1914: return "Subphrase"; break;
         case 1923: return "Subphrase"; break;
         case 1930: return "Subphrase"; break;
         case 1937: return "Subphrase"; break;
         case 1945: return "Subphrase"; break;
         case 1952: return "Subphrase"; break;
         case 1961: return "Subphrase"; break;
         case 1972: return "Subphrase"; break;
         case 1979: return "Subphrase"; break;
         case 1986: return "Subphrase"; break;
         case 1993: return "Subphrase"; break;
         case 2000: return "Subphrase"; break;
         case 2007: return "Subphrase"; break;
         case 2014: return "Subphrase"; break;
         case 2021: return "Subphrase"; break;
         case 2028: return "Subphrase"; break;
         case 2035: return "arm_entry"; break;
         case 2046: return "Subphrase"; break;
         case 2053: return "Subphrase"; break;
         case 2060: return "iflag_entry"; break;
         case 2071: return "Subphrase"; break;
         case 2078: return "Subphrase"; break;
         case 2085: return "feature_def"; break;
         case 2095: return "Subphrase"; break;
         case 2103: return "feat_entry"; break;
         case 2176: return "Subphrase"; break;
         case 2183: return "Subphrase"; break;
         case 2190: return "Subphrase"; break;
         case 2197: return "Subphrase"; break;
         case 2204: return "Subphrase"; break;
         case 2211: return "Subphrase"; break;
         case 2220: return "Subphrase"; break;
         case 2231: return "Subphrase"; break;
         case 2239: return "Subphrase"; break;
         case 2246: return "Subphrase"; break;
         case 2253: return "Subphrase"; break;
         case 2260: return "effect_def"; break;
         case 2272: return "Subphrase"; break;
         case 2281: return "Subphrase"; break;
         case 2288: return "Subphrase"; break;
         case 2304: return "Subphrase"; break;
         case 2312: return "Subphrase"; break;
         case 2324: return "Subphrase"; break;
         case 2332: return "eff_entry"; break;
         case 2486: return "Subphrase"; break;
         case 2493: return "Subphrase"; break;
         case 2500: return "Subphrase"; break;
         case 2507: return "Subphrase"; break;
         case 2514: return "Subphrase"; break;
         case 2521: return "Subphrase"; break;
         case 2528: return "Subphrase"; break;
         case 2535: return "Subphrase"; break;
         case 2544: return "Subphrase"; break;
         case 2551: return "Subphrase"; break;
         case 2558: return "Subphrase"; break;
         case 2565: return "Subphrase"; break;
         case 2572: return "Subphrase"; break;
         case 2579: return "Subphrase"; break;
         case 2586: return "Subphrase"; break;
         case 2593: return "Subphrase"; break;
         case 2600: return "Subphrase"; break;
         case 2607: return "Subphrase"; break;
         case 2614: return "Subphrase"; break;
         case 2621: return "Subphrase"; break;
         case 2628: return "Subphrase"; break;
         case 2635: return "Subphrase"; break;
         case 2642: return "Subphrase"; break;
         case 2649: return "Subphrase"; break;
         case 2658: return "Subphrase"; break;
         case 2669: return "numlist"; break;
         case 2673: return "Subphrase"; break;
         case 2682: return "Subphrase"; break;
         case 2693: return "object_entry"; break;
         case 2697: return "race_def"; break;
         case 2706: return "Subphrase"; break;
         case 2714: return "Subphrase"; break;
         case 2722: return "race_entry"; break;
         case 2807: return "Subphrase"; break;
         case 2814: return "Subphrase"; break;
         case 2825: return "Subphrase"; break;
         case 2832: return "Subphrase"; break;
         case 2839: return "Subphrase"; break;
         case 2846: return "Subphrase"; break;
         case 2855: return "Subphrase"; break;
         case 2862: return "Subphrase"; break;
         case 2869: return "Subphrase"; break;
         case 2876: return "Subphrase"; break;
         case 2883: return "Subphrase"; break;
         case 2890: return "Subphrase"; break;
         case 2897: return "Subphrase"; break;
         case 2904: return "Subphrase"; break;
         case 2913: return "Subphrase"; break;
         case 2920: return "Subphrase"; break;
         case 2927: return "Subphrase"; break;
         case 2934: return "Subphrase"; break;
         case 2943: return "Subphrase"; break;
         case 2954: return "class_def"; break;
         case 2962: return "Subphrase"; break;
         case 2970: return "class_entry"; break;
         case 3063: return "Subphrase"; break;
         case 3070: return "Subphrase"; break;
         case 3077: return "Subphrase"; break;
         case 3084: return "Subphrase"; break;
         case 3091: return "Subphrase"; break;
         case 3098: return "Subphrase"; break;
         case 3105: return "Subphrase"; break;
         case 3112: return "Subphrase"; break;
         case 3122: return "Subphrase"; break;
         case 3129: return "Subphrase"; break;
         case 3136: return "Subphrase"; break;
         case 3146: return "Subphrase"; break;
         case 3153: return "Subphrase"; break;
         case 3160: return "Subphrase"; break;
         case 3169: return "Subphrase"; break;
         case 3176: return "Subphrase"; break;
         case 3180: return "Subphrase"; break;
         case 3184: return "Subphrase"; break;
         case 3191: return "Subphrase"; break;
         case 3200: return "Subphrase"; break;
         case 3207: return "Subphrase"; break;
         case 3214: return "Subphrase"; break;
         case 3223: return "Subphrase"; break;
         case 3230: return "Subphrase"; break;
         case 3237: return "Subphrase"; break;
         case 3246: return "Subphrase"; break;
         case 3257: return "special_ability"; break;
         case 3287: return "Subphrase"; break;
         case 3294: return "Subphrase"; break;
         case 3301: return "Subphrase"; break;
         case 3308: return "abil_param"; break;
         case 3325: return "abil_level"; break;
         case 3360: return "Subphrase"; break;
         case 3367: return "Subphrase"; break;
         case 3374: return "Subphrase"; break;
         case 3381: return "dungeon_def"; break;
         case 3389: return "Subphrase"; break;
         case 3397: return "dun_entry"; break;
         case 3416: return "Subphrase"; break;
         case 3423: return "dconst_entry"; break;
         case 3443: return "Subphrase"; break;
         case 3450: return "Subphrase"; break;
         case 3469: return "Subphrase"; break;
         case 3476: return "Subphrase"; break;
         case 3483: return "Subphrase"; break;
         case 3490: return "Subphrase"; break;
         case 3501: return "Subphrase"; break;
         case 3509: return "Subphrase"; break;
         case 3517: return "Subphrase"; break;
         case 3524: return "Subphrase"; break;
         case 3535: return "Subphrase"; break;
         case 3543: return "Subphrase"; break;
         case 3564: return "Subphrase"; break;
         case 3571: return "list_entry"; break;
         case 3583: return "artifact_def"; break;
         case 3591: return "Subphrase"; break;
         case 3599: return "arti_entry"; break;
         case 3675: return "Subphrase"; break;
         case 3682: return "Subphrase"; break;
         case 3689: return "Subphrase"; break;
         case 3700: return "Subphrase"; break;
         case 3707: return "Subphrase"; break;
         case 3714: return "Subphrase"; break;
         case 3721: return "Subphrase"; break;
         case 3730: return "Subphrase"; break;
         case 3737: return "Subphrase"; break;
         case 3744: return "Subphrase"; break;
         case 3753: return "Subphrase"; break;
         case 3760: return "Subphrase"; break;
         case 3767: return "Subphrase"; break;
         case 3776: return "Subphrase"; break;
         case 3783: return "Subphrase"; break;
         case 3792: return "Subphrase"; break;
         case 3796: return "Subphrase"; break;
         case 3805: return "Subphrase"; break;
         case 3809: return "Subphrase"; break;
         case 3818: return "Subphrase"; break;
         case 3822: return "Subphrase"; break;
         case 3831: return "Subphrase"; break;
         case 3835: return "art_power"; break;
         case 3887: return "Subphrase"; break;
         case 3895: return "Subphrase"; break;
         case 3903: return "Subphrase"; break;
         case 3911: return "domain_def"; break;
         case 3921: return "Subphrase"; break;
         case 3929: return "domain_entry"; break;
         case 3962: return "Subphrase"; break;
         case 3969: return "Subphrase"; break;
         case 3978: return "Subphrase"; break;
         case 3985: return "Subphrase"; break;
         case 3992: return "Subphrase"; break;
         case 4001: return "Subphrase"; break;
         case 4008: return "Subphrase"; break;
         case 4015: return "god_def"; break;
         case 4023: return "Subphrase"; break;
         case 4031: return "god_entry"; break;
         case 4076: return "Subphrase"; break;
         case 4083: return "Subphrase"; break;
         case 4092: return "Subphrase"; break;
         case 4099: return "Subphrase"; break;
         case 4106: return "Subphrase"; break;
         case 4115: return "Subphrase"; break;
         case 4126: return "Subphrase"; break;
         case 4133: return "Subphrase"; break;
         case 4140: return "Subphrase"; break;
         case 4149: return "Subphrase"; break;
         case 4156: return "terrain_def"; break;
         case 4164: return "Subphrase"; break;
         case 4172: return "terrain_entry"; break;
         case 4224: return "Subphrase"; break;
         case 4231: return "Subphrase"; break;
         case 4238: return "Subphrase"; break;
         case 4245: return "Subphrase"; break;
         case 4252: return "Subphrase"; break;
         case 4259: return "Subphrase"; break;
         case 4268: return "Subphrase"; break;
         case 4279: return "region_def"; break;
         case 4289: return "Subphrase"; break;
         case 4297: return "region_entry"; break;
         case 4391: return "Subphrase"; break;
         case 4398: return "Subphrase"; break;
         case 4405: return "Subphrase"; break;
         case 4412: return "Subphrase"; break;
         case 4419: return "Subphrase"; break;
         case 4426: return "Subphrase"; break;
         case 4433: return "Subphrase"; break;
         case 4440: return "Subphrase"; break;
         case 4449: return "Subphrase"; break;
         case 4460: return "Subphrase"; break;
         case 4467: return "Subphrase"; break;
         case 4476: return "Subphrase"; break;
         case 4487: return "Subphrase"; break;
         case 4499: return "Subphrase"; break;
         case 4522: return "Subphrase"; break;
         case 4538: return "Subphrase"; break;
         case 4545: return "Subphrase"; break;
         case 4552: return "Subphrase"; break;
         case 4573: return "Subphrase"; break;
         case 4580: return "Subphrase"; break;
         case 4588: return "Subphrase"; break;
         case 4596: return "template_def"; break;
         case 4607: return "Subphrase"; break;
         case 4615: return "Subphrase"; break;
         case 4623: return "template_entry"; break;
         case 4850: return "Subphrase"; break;
         case 4857: return "Subphrase"; break;
         case 4864: return "Subphrase"; break;
         case 4871: return "Subphrase"; break;
         case 4879: return "Subphrase"; break;
         case 4886: return "Subphrase"; break;
         case 4893: return "Subphrase"; break;
         case 4900: return "Subphrase"; break;
         case 4907: return "Subphrase"; break;
         case 4914: return "Subphrase"; break;
         case 4921: return "Subphrase"; break;
         case 4928: return "Subphrase"; break;
         case 4935: return "Subphrase"; break;
         case 4942: return "Subphrase"; break;
         case 4949: return "Subphrase"; break;
         case 4956: return "Subphrase"; break;
         case 4963: return "Subphrase"; break;
         case 4971: return "Subphrase"; break;
         case 4978: return "Subphrase"; break;
         case 4987: return "Subphrase"; break;
         case 4994: return "Subphrase"; break;
         case 5003: return "Subphrase"; break;
         case 5012: return "Subphrase"; break;
         case 5020: return "Subphrase"; break;
         case 5027: return "Subphrase"; break;
         case 5034: return "Subphrase"; break;
         case 5043: return "Subphrase"; break;
         case 5052: return "Subphrase"; break;
         case 5060: return "Subphrase"; break;
         case 5067: return "Subphrase"; break;
         case 5074: return "Subphrase"; break;
         case 5083: return "Subphrase"; break;
         case 5090: return "Subphrase"; break;
         case 5099: return "Subphrase"; break;
         case 5106: return "Subphrase"; break;
         case 5113: return "Subphrase"; break;
         case 5122: return "Subphrase"; break;
         case 5129: return "Subphrase"; break;
         case 5136: return "Subphrase"; break;
         case 5145: return "Subphrase"; break;
         case 5152: return "Subphrase"; break;
         case 5159: return "Subphrase"; break;
         case 5168: return "Subphrase"; break;
         case 5177: return "Subphrase"; break;
         case 5185: return "Subphrase"; break;
         case 5193: return "temp_attack_entry"; break;
         case 5210: return "Subphrase"; break;
         case 5222: return "Subphrase"; break;
         case 5229: return "Subphrase"; break;
         case 5236: return "Subphrase"; break;
         case 5246: return "Subphrase"; break;
         case 5258: return "Subphrase"; break;
         case 5270: return "Subphrase"; break;
         case 5277: return "Subphrase"; break;
         case 5284: return "Subphrase"; break;
         case 5291: return "Subphrase"; break;
         case 5301: return "Subphrase"; break;
         case 5313: return "Subphrase"; break;
         case 5320: return "quest_def"; break;
         case 5324: return "flavor_def"; break;
         case 5334: return "Subphrase"; break;
         case 5342: return "flavor_entry"; break;
         case 5371: return "Subphrase"; break;
         case 5378: return "Subphrase"; break;
         case 5385: return "Subphrase"; break;
         case 5392: return "Subphrase"; break;
         case 5399: return "encounter_def"; break;
         case 5408: return "Subphrase"; break;
         case 5416: return "Subphrase"; break;
         case 5424: return "encounter_entry"; break;
         case 5506: return "Subphrase"; break;
         case 5514: return "Subphrase"; break;
         case 5522: return "Subphrase"; break;
         case 5529: return "Subphrase"; break;
         case 5538: return "Subphrase"; break;
         case 5549: return "encounter_part"; break;
         case 5558: return "Subphrase"; break;
         case 5570: return "Subphrase"; break;
         case 5625: return "Subphrase"; break;
         case 5633: return "Subphrase"; break;
         case 5646: return "Subphrase"; break;
         case 5662: return "Subphrase"; break;
         case 5672: return "Subphrase"; break;
         case 5681: return "Subphrase"; break;
         case 5689: return "behaviour_def"; break;
         case 5699: return "Subphrase"; break;
         case 5707: return "behaviour_entry"; break;
         case 5733: return "Subphrase"; break;
         case 5740: return "Subphrase"; break;
         case 5747: return "Subphrase"; break;
         case 5756: return "Subphrase"; break;
         case 5767: return "text_def"; break;
         case 5774: return "Subphrase"; break;
         case 5822: return "prototype"; break;
         case 5826: return "type_name"; break;
         case 5870: return "s_prototype"; break;
         case 5883: return "Subphrase"; break;
         case 5894: return "Subphrase"; break;
         case 5902: return "Subphrase"; break;
         case 5909: return "Subphrase"; break;
         case 5917: return "Subphrase"; break;
         case 5929: return "Subphrase"; break;
         case 5937: return "Subphrase"; break;
         case 5944: return "Subphrase"; break;
         case 5952: return "Subphrase"; break;
         case 5960: return "sg_prototype"; break;
         case 5971: return "Subphrase"; break;
         case 5982: return "Subphrase"; break;
         case 5990: return "Subphrase"; break;
         case 5997: return "Subphrase"; break;
         case 6005: return "Subphrase"; break;
         case 6017: return "Subphrase"; break;
         case 6025: return "Subphrase"; break;
         case 6032: return "Subphrase"; break;
         case 6040: return "Subphrase"; break;
         case 6048: return "s_declaration"; break;
         case 6057: return "s_object_decl"; break;
         case 6066: return "g_declaration"; break;
         case 6073: return "Subphrase"; break;
         case 6082: return "l_declaration"; break;
         case 6089: return "Subphrase"; break;
         case 6098: return "r_declaration"; break;
         case 6105: return "Subphrase"; break;
         case 6114: return "statement"; break;
         case 6203: return "Subphrase"; break;
         case 6214: return "Subphrase"; break;
         case 6222: return "Subphrase"; break;
         case 6230: return "Subphrase"; break;
         case 6239: return "Subphrase"; break;
         case 6247: return "block"; break;
         case 6253: return "Subphrase"; break;
         case 6261: return "routine"; break;
         case 6268: return "Subphrase"; break;
         case 6282: return "Subphrase"; break;
         case 6290: return "fund_expr"; break;
         case 6356: return "Subphrase"; break;
         case 6364: return "Subphrase"; break;
         case 6372: return "lvalue"; break;
         case 6388: return "Subphrase"; break;
         case 6396: return "expr10"; break;
         case 6428: return "expr20"; break;
         case 6468: return "expr30"; break;
         case 6478: return "expr40"; break;
         case 6489: return "expr50"; break;
         case 6511: return "expr60"; break;
         case 6527: return "expr70"; break;
         case 6543: return "expr80"; break;
         case 6583: return "expr90"; break;
         case 6593: return "expr100"; break;
         case 6603: return "expr110"; break;
         case 6613: return "expr120"; break;
         case 6623: return "expr130"; break;
         case 6633: return "expr140"; break;
         case 6645: return "expr150"; break;
         case 6715: return "expr"; break;
         case 6725: return "param_list"; break;
         case 6729: return "Subphrase"; break;
         case 6737: return "Subphrase"; break;
   }
   else 
      switch(n-50000) {
         case 496: return "MEMBER_OP"; break;
         case 495: return "ELLIPSIS"; break;
         case 494: return "ASSIGN_SUB"; break;
         case 493: return "ASSIGN_RSHIFT"; break;
         case 492: return "ASSIGN_LSHIFT"; break;
         case 491: return "ASSIGN_NEG"; break;
         case 490: return "ASSIGN_OR"; break;
         case 489: return "ASSIGN_AND"; break;
         case 488: return "ASSIGN_MOD"; break;
         case 487: return "ASSIGN_DIV"; break;
         case 486: return "ASSIGN_ADD"; break;
         case 485: return "ASSIGN_MULT"; break;
         case 484: return "OR_OP"; break;
         case 483: return "AND_OP"; break;
         case 482: return "NE_OP"; break;
         case 481: return "EQ_OP"; break;
         case 480: return "LE_OP"; break;
         case 479: return "GE_OP"; break;
         case 478: return "RSHIFT_OP"; break;
         case 477: return "LSHIFT_OP"; break;
         case 476: return "DECREMENT_OP"; break;
         case 475: return "INCREMENT_OP"; break;
         case 474: return "WORD_RECT"; break;
         case 473: return "YVAL"; break;
         case 472: return "XVAL"; break;
         case 471: return "WITH"; break;
         case 470: return "WIELD"; break;
         case 469: return "WHILE"; break;
         case 468: return "WEIGHT_LIM"; break;
         case 467: return "WEIGHT_MOD"; break;
         case 466: return "WEIGHT"; break;
         case 465: return "WEEK"; break;
         case 464: return "WALLS"; break;
         case 463: return "VAR"; break;
         case 462: return "VALUE"; break;
         case 461: return "TVAL"; break;
         case 460: return "TYPE"; break;
         case 459: return "TURNS"; break;
         case 458: return "WORD_TRUE"; break;
         case 457: return "TO"; break;
         case 456: return "TIMEOUT"; break;
         case 455: return "TILES"; break;
         case 454: return "TILE"; break;
         case 453: return "THROWING"; break;
         case 452: return "THREAT"; break;
         case 451: return "TEXT"; break;
         case 450: return "TERRAIN"; break;
         case 449: return "WORD_TEMPLATE"; break;
         case 448: return "WORD_TARGET"; break;
         case 447: return "SYSTEM"; break;
         case 446: return "SWITCH"; break;
         case 445: return "SVAL"; break;
         case 444: return "SUSTAINS"; break;
         case 443: return "STORE"; break;
         case 442: return "STOCK"; break;
         case 441: return "STATIC"; break;
         case 440: return "STATI"; break;
         case 439: return "STARTING"; break;
         case 438: return "START"; break;
         case 437: return "SPELLS"; break;
         case 436: return "SPELL"; break;
         case 435: return "SPEED"; break;
         case 434: return "SPECIALS"; break;
         case 433: return "SLOT"; break;
         case 432: return "SKILLS"; break;
         case 431: return "SIZE"; break;
         case 430: return "SDMG"; break;
         case 429: return "SAVES"; break;
         case 428: return "RVAL"; break;
         case 427: return "ROUTINE"; break;
         case 426: return "ROOMTYPES"; break;
         case 425: return "RETURN"; break;
         case 424: return "RESISTS"; break;
         case 423: return "REGIONS"; break;
         case 422: return "REGION"; break;
         case 421: return "RATING"; break;
         case 420: return "RANGE"; break;
         case 419: return "RANDOM"; break;
         case 418: return "RACE"; break;
         case 417: return "QVAL"; break;
         case 416: return "QUEST"; break;
         case 415: return "QUALITIES"; break;
         case 414: return "PVAL"; break;
         case 413: return "PURPOSE"; break;
         case 412: return "PROP"; break;
         case 411: return "PROFICIENCIES"; break;
         case 410: return "POWER"; break;
         case 409: return "POISON"; break;
         case 408: return "PER"; break;
         case 407: return "PENALTY"; break;
         case 406: return "PARTS"; break;
         case 405: return "PARTIAL"; break;
         case 404: return "PARRY"; break;
         case 403: return "PARAMS"; break;
         case 402: return "OPTION"; break;
         case 401: return "OR"; break;
         case 400: return "ON"; break;
         case 399: return "OF"; break;
         case 398: return "OBJECTS"; break;
         case 397: return "OBJECT"; break;
         case 396: return "NUTRITION"; break;
         case 395: return "WORD_NULL"; break;
         case 394: return "NPC"; break;
         case 393: return "NORMAL"; break;
         case 392: return "NONE"; break;
         case 391: return "NEAR"; break;
         case 390: return "MTYPE"; break;
         case 389: return "MOVE"; break;
         case 388: return "MONSTER"; break;
         case 387: return "WORD_MIN"; break;
         case 386: return "MEMBER"; break;
         case 385: return "MELEE"; break;
         case 384: return "WORD_MAX"; break;
         case 383: return "MATERIAL"; break;
         case 382: return "MAP"; break;
         case 381: return "MANA"; break;
         case 380: return "MODULE"; break;
         case 379: return "MAX_SIZE"; break;
         case 378: return "LVAL"; break;
         case 377: return "LISTS"; break;
         case 376: return "LIGHT"; break;
         case 375: return "LIFESPAN"; break;
         case 374: return "WORD_LEVEL"; break;
         case 373: return "LDMG"; break;
         case 372: return "ITEM"; break;
         case 371: return "INVOKE"; break;
         case 370: return "INNATE"; break;
         case 369: return "IMMUNE"; break;
         case 368: return "IMAGE"; break;
         case 367: return "IF"; break;
         case 366: return "HIT"; break;
         case 365: return "HITPOINTS"; break;
         case 364: return "HITDICE"; break;
         case 363: return "HEAVY"; break;
         case 362: return "GUILD"; break;
         case 361: return "GROUP"; break;
         case 360: return "GRID"; break;
         case 359: return "GRANTS"; break;
         case 358: return "GOD"; break;
         case 357: return "GLYPH"; break;
         case 356: return "GEAR"; break;
         case 355: return "FUEL"; break;
         case 354: return "FROM"; break;
         case 353: return "FOR"; break;
         case 352: return "WORD_FILE"; break;
         case 351: return "FLOOR"; break;
         case 350: return "FLAGS"; break;
         case 349: return "FIRES"; break;
         case 348: return "FEATURE"; break;
         case 347: return "FEAT"; break;
         case 346: return "FEATS"; break;
         case 345: return "WORD_FALSE"; break;
         case 344: return "FACTOR"; break;
         case 343: return "EVERY"; break;
         case 342: return "EVENT"; break;
         case 341: return "FLAVOR"; break;
         case 340: return "FAVORED"; break;
         case 339: return "EVAL"; break;
         case 338: return "EQUIP"; break;
         case 337: return "EXPORT"; break;
         case 336: return "WORD_ENCOUNTER"; break;
         case 335: return "ELSE"; break;
         case 334: return "EFFECT"; break;
         case 333: return "DVAL"; break;
         case 332: return "DUNGEON"; break;
         case 331: return "DO"; break;
         case 330: return "DISEASE"; break;
         case 329: return "DOMAINS"; break;
         case 328: return "DOMAIN"; break;
         case 327: return "DOOR"; break;
         case 326: return "WORD_DESC"; break;
         case 325: return "DEPTH"; break;
         case 324: return "DEFAULT"; break;
         case 323: return "DEF"; break;
         case 322: return "WORD_DC"; break;
         case 321: return "DAY"; break;
         case 320: return "DARK"; break;
         case 319: return "DMG"; break;
         case 318: return "CVAL"; break;
         case 317: return "CURSED"; break;
         case 316: return "CRIT"; break;
         case 315: return "COST"; break;
         case 314: return "CONTINUE"; break;
         case 313: return "CTYPE"; break;
         case 312: return "COVERAGE"; break;
         case 311: return "CONSTANTS"; break;
         case 310: return "WORD_COLOR"; break;
         case 309: return "CLASS"; break;
         case 308: return "CHANCE"; break;
         case 307: return "WORD_CR"; break;
         case 306: return "CASTER"; break;
         case 305: return "CASE"; break;
         case 304: return "CAPACITY"; break;
         case 303: return "BRIGHT"; break;
         case 302: return "BREAK"; break;
         case 301: return "BRAWL"; break;
         case 300: return "BLESSED"; break;
         case 299: return "BEHAVIOUR"; break;
         case 298: return "BASE"; break;
         case 297: return "AVAL"; break;
         case 296: return "ATTACK"; break;
         case 295: return "AT"; break;
         case 294: return "AS"; break;
         case 293: return "ARTIFACT"; break;
         case 292: return "ARM"; break;
         case 291: return "ARCHERY"; break;
         case 290: return "APTITUDES"; break;
         case 289: return "WORD_ANY"; break;
         case 288: return "AND"; break;
         case 287: return "ALSO"; break;
         case 286: return "ALL"; break;
         case 285: return "ALIGN"; break;
         case 284: return "ACC"; break;
         case 283: return "ABS"; break;
         case 282: return "ABILITY"; break;
         case 281: return "REGNAME"; break;
         case 280: return "OPCODE"; break;
         case 279: return "STYPE"; break;
         case 278: return "DIRECTION"; break;
         case 277: return "ATTRIBUTE"; break;
         case 276: return "WEP_TYPE"; break;
         case 275: return "COLOR"; break;
         case 274: return "BOOL"; break;
         case 273: return "STRING"; break;
         case 272: return "RID"; break;
         case 271: return "HTEXT"; break;
         case 270: return "UINT32"; break;
         case 269: return "UINT16"; break;
         case 268: return "UINT8"; break;
         case 267: return "INT32"; break;
         case 266: return "INT16"; break;
         case 265: return "INT8"; break;
         case 264: return "VOID"; break;
         case 263: return "HOBJ"; break;
         case 262: return "SCOPE_OPER"; break;
         case 261: return "NOTDONE"; break;
         case 260: return "MAPAREA"; break;
         case 259: return "CHAR_CONST"; break;
         case 258: return "IDENT"; break;
         case 257: return "LITERAL"; break;
         case 256: return "NUMBER"; break;
      }
   return "special_character";
}
