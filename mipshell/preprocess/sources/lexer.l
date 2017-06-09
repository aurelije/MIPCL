/**
 * \file lexer.l __MIPCL__ lexer
 * |  __Author__  | N.N. Pisaruk                              |
 * |-------------:|:------------------------------------------|
 * | __Address__  | Belarus, 220020 Minsk, L.Ukrainky, 8, 166 |
 * |    __Tel.__  | (00 375 17)250-08-32                      |
 * |  __Mobile__  | +37529 2761930                            |
 * |  __e-mail__  | pisaruk@yandex.ru                         |
 * | __home page__| http://pisaruk.narod.ru                   |
 *
 *   \copyright  __N.N. Pisaruk,      1995 - 2013__
 */

%START SU COM CPPCOM VT
digit		[0-9]
letter		[A-Za-z_]
integer		{digit}+
real		{digit}+'.'{digit}+([DEde][-+]?{digit}+)?
const		{integer}|{real}|\'.\'
identifier	{letter}({letter}|{digit})*
vartype		(VAR|CVar|VAR_VECTOR|CVarVector|VAR_ARRAY|CVarArray)

%{
#include <string.h>
#include <stdio.h>
#include "parser.tab.h"

int iLineNo=1;
int iFlags=0;
/* 0x1 - SU, 0x2  - obj, 0x4 - forall, 0x8 - sum */
extern char *InFile;
%}

%%
{vartype}				{BEGIN VT; ECHO;}
<VT>\(					{fprintf(yyout,"(this,");}
<VT>;					{ECHO; BEGIN 0;}
minimize|maximize		{BEGIN SU; iFlags|=3; strcpy(yylval.strVal,yytext); return IDENTIFIER;}
<SU>return				{BEGIN 0; ECHO;}
forall					{BEGIN SU; iFlags|=0x4; return yylval.iVal=FORALL;}
<SU>void				{return yylval.iVal=VOID;}
<SU>char				{return yylval.iVal=CHAR;}
<SU>short				{return yylval.iVal=SHORT;}
<SU>int					{return yylval.iVal=INT;}
<SU>unsigned			{return yylval.iVal=UNSIGNED;}
<SU>long				{return yylval.iVal=LONG;}
<SU>float				{return yylval.iVal=FLOAT;}
<SU>double				{return yylval.iVal=DOUBLE;}
<SU>bool				{return yylval.iVal=BOOL;}
sum						{BEGIN SU; iFlags|=8; return yylval.iVal=SUM;}
<SU>if					{return yylval.iVal=IF;}
<SU>else				{return yylval.iVal=ELSE;}
<SU>for					{return yylval.iVal=FOR;}
<SU>while				{return yylval.iVal=WHILE;}
<SU>in					{
							if (iFlags & 0xC) return yylval.iVal=IN;
						 	else return yylval.iVal=DVARIN;
						}
<SU>{const}				{strcpy(yylval.strVal,yytext); return CONSTANT;}
<SU>{identifier}		{strcpy(yylval.strVal,yytext); return IDENTIFIER;}
<SU>->					{return yylval.iVal=PTR_OP;}
<SU>\+\+				{return yylval.iVal=INC_OP;}
<SU>--					{return yylval.iVal=DEC_OP;}
<SU><<					{return yylval.iVal=LEFT_OP;}
<SU>>>					{return yylval.iVal=RIGHT_OP;}
<SU><=					{return yylval.iVal=LE_OP;}
<SU>>=					{return yylval.iVal=GE_OP;}
<SU>==					{return yylval.iVal=EQ_OP;}
<SU>!=					{return yylval.iVal=NE_OP;}
<SU>&&					{return yylval.iVal=AND_OP;}
<SU>\|\|				{return yylval.iVal=OR_OP;}
<SU>\*=					{return yylval.iVal=MUL_ASSIGN;}
<SU>\/=					{return yylval.iVal=DIV_ASSIGN;}
<SU>%=					{return yylval.iVal=MOD_ASSIGN;}
<SU>\+=					{return yylval.iVal=ADD_ASSIGN;}
<SU>-=					{return yylval.iVal=SUB_ASSIGN;}
<SU><<=					{return yylval.iVal=LEFT_ASSIGN;}
<SU>>>=					{return yylval.iVal=RIGHT_ASSIGN;}
<SU>&=					{return yylval.iVal=AND_ASSIGN;}
<SU>\^=					{return yylval.iVal=XOR_ASSIGN;}
<SU>\|=					{return yylval.iVal=OR_ASSIGN;}
<SU>\.\.				{return yylval.iVal=TWO_POINTS;}
<SU>[-+*%/!&|?:<>=(),.\[\]]	{return yylval.iVal=*yytext;}
<SU>\n					{fprintf(yyout,"\n#line %d \"%s\"\n",++iLineNo,InFile);}
<SU>;					{iFlags&=~0xE; return yylval.iVal=';';}
<SU>\"[^"]*\"			{
							if (yytext[yyleng-2] == '\\')
								yymore();
							else {
								strcpy(yylval.strVal,yytext); return STRING;
							}
						}
<SU>\/\/				{BEGIN CPPCOM;}
<CPPCOM>\n				{
							fprintf(yyout,"\n#line %d \"%s\"\n",++iLineNo,InFile);
							if (iFlags & 0x1) {BEGIN SU;} else {BEGIN 0;}	
						}	
<CPPCOM>.				{}	
\/\*					{ECHO; BEGIN COM;}
<COM>.					{}
<COM>\*\/				{ECHO; if (iFlags & 0x1) {BEGIN SU;} else {BEGIN 0;}}
.						{ECHO;}
\n						{fprintf(yyout,"\n#line %d \"%s\"\n",++iLineNo,InFile);}
%%

int yywrap()
{
	return 1;
}
