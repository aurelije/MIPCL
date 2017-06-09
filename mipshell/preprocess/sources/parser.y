/**
 * \file parser.y __MIPCL__ parser
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
 
%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YYERROR_VERBOSE
#define MAX_STR_LEN 1024 ///< maximum length of input line
#define SUM_PREFIX_STR_LEN 24

void yyerror(const char *);
int yylex();
extern int iLineNo, iFlags;
/* 0x1 - SU, 0x2  - obj, 0x4 - forall, 0x8 - sum, 0x10 -SumInEq */
extern FILE *yyout, *yyin;
int iCurLinSum=9;
char *InFile;
%}

%union {
	int iVal;
	char strVal[1024];
};

%token <iVal> IN DVARIN FORALL SUM TWO_POINTS
%token <iVal> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <iVal> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <iVal> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <iVal> XOR_ASSIGN OR_ASSIGN
%token <iVal> VOID CHAR	SHORT INT UNSIGNED LONG	FLOAT DOUBLE BOOL
%token <iVal> IF ELSE WHILE FOR
 
%token <strVal> IDENTIFIER CONSTANT STRING

%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%type <strVal> primary_expr postfix_expr arg_expr_list unary_expr unary_operator
%type <strVal> cast_expr add_expr mult_expr shift_expr rel_expr equality_expr expr
%type <strVal> and_expr xor_expr or_expr logic_and_expr logic_or_expr
%type <strVal> cond_expr assign_expr assign_operator expr_statement
%type <strVal> type_specifier type_name 
%type <strVal> forall ineq_expr cond_incl incl_list incl sum_expr comp_sign dvarincl
%type <strVal> if_operator while_operator for_operator

%%
system:
	 system forall			{fprintf(yyout,"%s",$2);}
	| system if_operator	{fprintf(yyout,"%s",$2);}
	| system for_operator	{fprintf(yyout,"%s",$2);}
	| system while_operator	{fprintf(yyout,"%s",$2);}
	| system expr_statement	{fprintf(yyout,"%s",$2);}
	| system error ';'
	| system error '}'
	|
	;
	
ineq_expr:
	add_expr comp_sign add_expr ';' {
		sprintf($$,"addCtr(%s %s %s);",$1,$2,$3);
		if (iFlags & 0x10) {
			iFlags&=~0x10;
			strcat($$,"\n}");
		}
	}
	| add_expr comp_sign add_expr comp_sign add_expr ';' {
		sprintf($$,"addCtr(%s %s %s %s %s);",$1,$2,$3,$4,$5);
		if (iFlags & 0x10) {
			iFlags&=~0x10;
			strcat($$,"\n}");
		}
	}
	| '(' add_expr comp_sign add_expr ')' '.' add_expr ';' {
		sprintf($$,"addCtr(%s %s %s).%s;",$2,$3,$4,$7);
		if (iFlags & 0x10) {
			iFlags&=~0x10;
			sprintf($$,"\n}");
		}
	}
	| '(' add_expr comp_sign add_expr comp_sign add_expr ')' '.' add_expr ';' {
		sprintf($$,"addCtr(%s %s %s %s %s).%s;",$2,$3,$4,$5,$6,$9);
		if (iFlags & 0x10) {
			iFlags&=~0x10;
			sprintf($$,"\n}");
		}
	}
;

comp_sign:
	LE_OP				{$$[0]='<'; $$[1]='='; $$[2]=0;}
	| GE_OP				{$$[0]='>'; $$[1]='='; $$[2]=0;}
	| EQ_OP				{$$[0]='='; $$[1]='='; $$[2]=0;}
	| NE_OP				{$$[0]='!'; $$[1]='='; $$[2]=0;}
	;

forall:
	FORALL '(' cond_incl ')'	{strcpy($$,$3); iFlags&=~0x4;}
	;

cond_incl:
	incl_list
	| incl_list ':' expr	{sprintf($$,"%s\n#line %d\n\tif (%s)",$1,iLineNo,$3);}
	;

incl_list:
	  incl
	| incl ',' incl_list	{sprintf($$,"%s\n#line %d\n%s",$1,iLineNo,$3);}
	;

incl:
	IDENTIFIER IN add_expr
	{
		sprintf($$,"for ((%s).initIt(); (%s).getNext(%s);)",$3,$3,$1);
	}
	|
	type_name IDENTIFIER IN add_expr
	{
		sprintf($$,"(%s).initIt();\n#line %d\nfor (%s %s; (%s).getNext(%s);)",$4,iLineNo,$1,$2,$4,$2);
	}
	|
	IDENTIFIER IN add_expr TWO_POINTS add_expr
	{
		sprintf($$,"for (%s=%s; %s <= %s; ++%s)",$1,$3,$1,$5,$1);
	}
	|
	IDENTIFIER IN '[' add_expr ',' add_expr ']'
	{
		sprintf($$,"for (%s=%s; %s <= %s; ++%s)",$1,$4,$1,$6,$1);
	}
	|
	IDENTIFIER IN '[' add_expr ',' add_expr ')'
	{
		sprintf($$,"for (%s=%s; %s < %s; ++%s)",$1,$4,$1,$6,$1);
	}
	|
	type_name IDENTIFIER IN '[' add_expr ',' add_expr ']'
	{
		sprintf($$,"for (%s %s=%s; %s <= %s; ++%s)",$1,$2,$5,$2,$7,$2);
	}
	|
	type_name IDENTIFIER IN '[' add_expr ',' add_expr ')'
	{
		sprintf($$,"for (%s %s=%s; %s < %s; ++%s)",$1,$2,$5,$2,$7,$2);
	}
	;

dvarincl:
	IDENTIFIER DVARIN IDENTIFIER
	{
		sprintf($$,"addDVar(new DVAR(%s,%s))",$1,$3);
	}
	|
	IDENTIFIER DVARIN STRING
	{
		sprintf($$,"addDVar(new DVAR(%s,%s))",$1,$3);
	}
;

primary_expr:
	IDENTIFIER
	| CONSTANT
	| STRING
	| '(' expr ')'	{sprintf($$,"(%s)",$2);}
	;

postfix_expr:
	primary_expr
	| postfix_expr '[' expr ']'				{sprintf($$,"%s[%s]",$1,$3);}
	| postfix_expr '(' ')'					{sprintf($$,"%s()",$1);}
	| postfix_expr '(' arg_expr_list ')'	{sprintf($$,"%s(%s)",$1,$3);}
	| postfix_expr '.' expr					{sprintf($$,"%s.%s",$1,$3);}
	| postfix_expr PTR_OP expr				{sprintf($$,"%s->%s",$1,$3);}
	| postfix_expr INC_OP					{sprintf($$,"%s++",$1);}
	| postfix_expr DEC_OP					{sprintf($$,"%s--",$1);}
	;

arg_expr_list:
	assign_expr
	|  assign_expr ',' arg_expr_list		{sprintf($$,"%s,%s",$1,$3);}
	;
	
unary_expr:
	postfix_expr
	| INC_OP unary_expr			%prec UMINUS	{sprintf($$,"++%s",$2);}
	| DEC_OP unary_expr			%prec UMINUS	{sprintf($$,"--%s",$2);}
	| unary_operator cast_expr	%prec UMINUS	{sprintf($$,"%s%s",$1,$2);}
	;

unary_operator:
	'&'					{$$[0]='&'; $$[1]=0;}
	| '*'				{$$[0]='*'; $$[1]=0;}
	| '+'				{$$[0]='+'; $$[1]=0;}
	| '-'				{$$[0]='-'; $$[1]=0;}
	| '~'				{$$[0]='~'; $$[1]=0;}
	| '!'				{$$[0]='!'; $$[1]=0;}
	;

cast_expr:
	unary_expr
	| sum_expr
	| '(' type_name ')' cast_expr		{sprintf($$,"(%s)%s",$2,$4);}
	;

type_name:
	type_specifier
	| type_name '*'			{sprintf($$,"%s*",$1);}
	;

type_specifier:
	VOID				{strcpy($$,"void");}
	| CHAR				{strcpy($$,"char");}
	| SHORT				{strcpy($$,"short");}
	| INT				{strcpy($$,"int");}
	| UNSIGNED			{strcpy($$,"unsigned");}
	| LONG				{strcpy($$,"long");}
	| FLOAT				{strcpy($$,"float");}
	| DOUBLE			{strcpy($$,"double");}
	| BOOL				{strcpy($$,"bool");}
	;

mult_expr:
	 cast_expr
	| cast_expr '*' mult_expr		{sprintf($$,"%s*%s",$1,$3);}
	| cast_expr '/' mult_expr		{sprintf($$,"%s/%s",$1,$3);}
	| cast_expr '%' mult_expr		{sprintf($$,"%s%%%s",$1,$3);}
	;

sum_expr:
	SUM '(' cond_incl ')' mult_expr	{
		iFlags&=~0x8;
		iCurLinSum=(iCurLinSum+1) % 10;
		if (!(iFlags & 0x12)) {
			iFlags|=0x10;
			fprintf(yyout,"{\n");
		}
		fprintf(yyout,"#line %d\ngetSum(%d).reset();\n",iLineNo,iCurLinSum);
		fprintf(yyout,"#line %d\n%s getSum(%d)+=(%s);\n",iLineNo,$3,iCurLinSum,$5);
		sprintf($$,"getSum(%d)",iCurLinSum);
	}
	;

add_expr:
	mult_expr
	| mult_expr '+' add_expr		{sprintf($$,"%s+%s",$1,$3);}
	| mult_expr '-' add_expr		{sprintf($$,"%s-%s",$1,$3);}
	;

shift_expr:
	add_expr
	| add_expr LEFT_OP shift_expr		{sprintf($$,"%s << %s",$1,$3);}
	| add_expr RIGHT_OP shift_expr		{sprintf($$,"%s >> %s",$1,$3);}
	;

rel_expr:
	shift_expr
	| rel_expr '<' shift_expr		{sprintf($$,"%s < %s",$1,$3);}
	| rel_expr '>' shift_expr		{sprintf($$,"%s > %s",$1,$3);}
	| rel_expr LE_OP shift_expr		{sprintf($$,"%s <= %s",$1,$3);}
	| rel_expr GE_OP shift_expr		{sprintf($$,"%s >= %s",$1,$3);}
	;

equality_expr:
	rel_expr
	| rel_expr EQ_OP equality_expr	{sprintf($$,"%s == %s",$1,$3);}
	| rel_expr NE_OP equality_expr	{sprintf($$,"%s != %s",$1,$3);}
	;

and_expr:
	equality_expr
	| equality_expr '&' and_expr 	{sprintf($$,"%s & %s",$1,$3);}
	;

xor_expr:
	and_expr
	| and_expr '^' xor_expr				{sprintf($$,"%s ^ %s",$1,$3);}
	;

or_expr:
	xor_expr
	| xor_expr '|' or_expr				{sprintf($$,"%s | %s",$1,$3);}
	;

logic_and_expr:
	or_expr
	| or_expr AND_OP logic_and_expr		{sprintf($$,"%s && %s",$1,$3);}
	;

logic_or_expr:
	logic_and_expr
	| logic_and_expr OR_OP logic_or_expr	{sprintf($$,"%s || %s",$1,$3);}
	;

cond_expr:
	logic_or_expr
	| logic_or_expr '?' expr ':' cond_expr	{sprintf($$,"%s? %s: %s",$1,$3,$5);}
	;

assign_expr:
	cond_expr
	|unary_expr assign_operator assign_expr		{sprintf($$,"%s%s%s",$1,$2,$3);}
	;

assign_operator:
	'='				{$$[0]='='; $$[1]=0;}
	| MUL_ASSIGN	{$$[0]='*'; $$[1]='='; $$[2]=0;}
	| DIV_ASSIGN	{$$[0]='/'; $$[1]='='; $$[2]=0;}
	| MOD_ASSIGN	{$$[0]='%'; $$[1]='='; $$[2]=0;}
	| ADD_ASSIGN	{$$[0]='+'; $$[1]='='; $$[2]=0;}
	| SUB_ASSIGN	{$$[0]='-'; $$[1]='='; $$[2]=0;}
	| LEFT_ASSIGN	{$$[0]='<'; $$[1]='<'; $$[2]='='; $$[3]=0;}
	| RIGHT_ASSIGN	{$$[0]='>'; $$[1]='>'; $$[2]='='; $$[3]=0;}
	| AND_ASSIGN	{$$[0]='&'; $$[1]='='; $$[2]=0;}
	| XOR_ASSIGN	{$$[0]='^'; $$[1]='='; $$[2]=0;}
	| OR_ASSIGN		{$$[0]='|'; $$[1]='='; $$[2]=0;}
	;

expr:
	assign_expr
	| assign_expr ',' expr		{sprintf($$,"%s,%s",$1,$3);}
	;

expr_statement: 
	';'				{sprintf($$,";");}
	| expr ';'		{sprintf($$,"%s;",$1);}
	| dvarincl ';'	{sprintf($$,"%s;",$1);}
	| ineq_expr 	{sprintf($$,"%s",$1);}
	;


if_operator:
	IF '(' expr ')'		{sprintf($$,"if (%s)",$3);}
	| ELSE {sprintf($$,"else");}
	;

while_operator:
	WHILE '(' expr ')'		{sprintf($$,"while (%s)",$3);}
	;

for_operator:
	FOR '(' expr_statement expr_statement expr ')' {sprintf($$,"for (%s %s %s)",$3,$4,$5);}
	;

%%

void yyerror(const char* s)
{
	fprintf(stderr,"%s(%d): %s\n",InFile,iLineNo,s);
}

/*
int main(int argc, char* argv[])
{
	int i, len;
	char *str;
	str=argv[1];
	len=strlen(str)-4;
	if (strcmp(str+len,".mod")) {
		fprintf(stderr,"Input file %s is not a .mod file\n",str);
		return 1;
	}
	if ((yyin=fopen(str,"r")) == NULL) {
		fprintf(stderr,"Error opening input file %s\n",str);
		return 1;
	}
	str=argv[2]; 
	if ((yyout=fopen(str,"w")) == NULL) {
		fprintf(stderr,"Error opening output file %s\n",str);
		return 1;
	}
	
	str=argv[1];
	for (i=len-1; i >= 0; --i) {
		if (str[i] == '/' || str[i] == '\\')
			break;
	}
	InFile=str + (i+1);

	fprintf(yyout,"#line 1 \"%s\"\n",InFile);

	yyparse();
	fclose(yyin);
	fclose(yyout);
	return 0;
}
*/

int main(int argc, char* argv[])
{
	int i,j,len;
	char *str, buf[MAX_STR_LEN];
	str=argv[1];
	len=strlen(str)-4;
	if (strcmp(str+len,".mod")) {
		fprintf(stderr,"Input file %s is not a .mod file\n",str);
		return 1;
	}
	if ((yyin=fopen(str,"r")) == NULL) {
		fprintf(stderr,"Error opening input file %s\n",str);
		return 1;
	}
	str=argv[2]; 
	if ((yyout=fopen(str,"w")) == NULL) {
		fprintf(stderr,"Error opening output file %s\n",str);
		return 1;
	}
	InFile=argv[1];
	if (argc > 3) {
		if (!strcmp(argv[3],"win32")) {
			InFile=buf;
			str=argv[1];
			len+=4;
			for (i=j=0; i < len; i++) {
				if ((buf[j++]=str[i]) == '\\')
					buf[j++]='\\';
			}
			buf[j]=0;
			len-=4;
		}
	}
	fprintf(yyout,"#line 1 \"%s\"\n",InFile);
/* parsing */
	yyparse();
	fclose(yyin);
	fclose(yyout);
	return 0;
}
