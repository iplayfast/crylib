// big num factor'r
#include <stdio.h>

//                n3,n2,n1          n1,n2,n3
//                d3,d2,d1          d1,d2,d3
//          --------------          ---------------------
//          n3d1,n2d1,n1d1          n1d1,n2d1,n3d1
//     n3d2,n2d2,n1d2,0             0   ,n1d2,n2d2,n3d2
//n3d3,n2d3,n1d3,0   ,0             0   ,0   ,n1d3,n2d3,n3d3
//---------------------------
//               n1n2d11d1m

bool sprintCalcLineRowfs(char *formatstr,int Row,int Col,int MaxRow)
{
    if (Col>Row || (Row-Col)>=MaxRow)
    {
        sprintf(formatstr,"0");
        return false;
    }
    else
    {
        Row -= Col;
        sprintf(formatstr,"n%dd%d",Row+1,Col+1);
        return true;
    }
}
bool sprintCalcLineRowVar(char *Vformatstr,int Row,int Col,int MaxRow)
{
    if (Col>Row || (Row-Col)>=MaxRow)
    {
        sprintf(Vformatstr,"0");
        return false;
    }
    else
    {
        Row -= Col;
        sprintf(Vformatstr,"Numerators[%d]*Denominators[%d]",Row+1,Col+1);
        return true;
    }
}
bool sprintCalcLineRowVars(bool lookup,char *VformatstrRow,char *VformatstrCol,int Row,int Col,int MaxRow)
{
    if (Col>Row || (Row-Col)>=MaxRow)
    {
        sprintf(VformatstrRow,"0");
        sprintf(VformatstrCol,"0");
        return false;
    }
    else
    {
        Row -= Col;
        if (lookup)
        {
        sprintf(VformatstrRow,"Numerators[%d]",Row);
        sprintf(VformatstrCol,"Denominators[%d]",Col);
        }
        else
        {
        sprintf(VformatstrRow,"Numerators%d",Row+1);
        sprintf(VformatstrCol,"Denominators%d",Col+1);
        }
        return true;
    }
}
void PrintComment(char *s)
{
    printf("\n\n//\n//\n//%s\n//\n//\n\n",s);
}
void BuildRowColFunc(int MaxNum,int MaxDen)
{
int MaxDigit = 6;//617;
// define glob vars
    PrintComment("Includes");
    printf("#include <stdio.h>\n");
    printf("#include <string.h>\n\n");
    PrintComment("Target");
/*  printf("char *Target = \"2519590847565789349402718324004839857142928212620403202777713783604366202070\\\n");
    printf("7595556264018525880784406918290641249515082189298559149176184502808489120072\\\n");
    printf("8449926873928072877767359714183472702618963750149718246911650776133798590957\\\n");
    printf("0009733045974880842840179742910064245869181719511874612151517265463228221686\\\n");
    printf("9987549182422433637259085141865462043576798423387184774447920739934236584823\\\n");
    printf("8242811981638150106748104516603773060562016196762561338441436038339044149526\\\n");
    printf("3443219011465754445417842402092461651572335077870774981712577246796292638635\\\n");
    printf("6373289912154831438167899885040445364023527381951378636564391212010397122822\\\n");
    printf("120720357\";\n\n");
*/
    printf("char *Target = \"1591\";\n\n");
    PrintComment("Global Variables");
    printf("//Numerators\nint Numerators[%d];\n",MaxNum);
    printf("\n\n//Denominators\nint Denominators[%d];\n",MaxNum);

    printf("\n\n//Results\nint Results[%d];\n",MaxDigit);
    printf("// Lookup instead of multiply\nint lookup[10][10];");
    printf("// Array of digit test functions\nbool (*TestArray[%d])();\n",MaxDigit);
    printf("// Array of Carry calculate functions\nint (*CarryArray[%d])();\n",MaxDigit);

    printf("\n\n");
// Row Col functions for each numerator and denomitator
    PrintComment("Row Col functions for each numerator and denominator");
    for(int i=0;i<MaxNum;i++)
    {
        for(int j =0;j<MaxDigit;j++)
        {
        char FormatStr[30];
        char FormatStrr[30],FormatStrc[30];
            if (sprintCalcLineRowfs(FormatStr,j,i,MaxDen))
            {
                sprintCalcLineRowVars(false,FormatStrr,FormatStrc,j,i,MaxDen);
                printf("\nint RowCol%s(int %s,int %s)",FormatStr,FormatStrr,FormatStrc);
                printf("\n{\n\treturn %s * %s;\n}",FormatStrr,FormatStrc);
            }
        }
    }
// Sum Functions for each Row Col result
    PrintComment("Sum functions for each Col");
    for(int i=0;i<MaxDigit;i++)
    {
    bool p = false;
        printf("\nint Sum%d()\n{\n",i+1);
        if (i==MaxDigit-1)
            printf("\n\treturn 0");
        else
            printf("\n\treturn ");
        for(int j =0;j<MaxDen;j++)
        {
        char FormatStr[30];
        char FormatStrr[30],FormatStrc[30];
            if (sprintCalcLineRowfs(FormatStr,i,j,MaxDen))
            {
                sprintCalcLineRowVars(true,FormatStrr,FormatStrc,i,j,MaxDen);
                if (p) printf("+");
                if ((j+1) % 3==0) printf("\n\t\t");
                printf("RowCol%s(%s,%s)",FormatStr,FormatStrr,FormatStrc);
                p = true;
            }
        }
        printf(";\n}\n");
    }
    PrintComment("Carry Functions for each col");
    printf("\nint Carry1()\n{\n\treturn 0;\n}\n");
    for(int i=1;i<MaxDigit+1;i++)
    {
        printf("\nint Carry%d",i+1);
        printf("()\n{\n\treturn Sum%d() / 10",i);
        printf(";\n}\n");
    }
    PrintComment("Digit Functions for each col");
    for(int i=0;i<MaxDigit;i++)
    {
        printf("\nint Digit%d()\n{\n",i+1);
        if (i)
            printf("return (Carry%d() + Sum%d()) % 10;\n}\n",i+1,i+1);
        else
            printf("return Sum%d() % 10;\n}\n",i+1);
    }
    PrintComment("Test Functions for each col");
    for(int i=0;i<MaxDigit;i++)
    {
        printf("bool Test%d()",i+1);
        printf("\n{\n return Digit%d()==Results[%d];\n}\n",i+1,i);
    }
    PrintComment("Setup");
    printf("void Setup()\n");
    printf("{\n");
    printf("for(int c=0,i=0;i<%d;i++)\n{\n",MaxDigit);
    printf("\tResults[i] = 0;\n");\
    printf("\tif (!(strlen(Target)<%d-i))\n",MaxDigit);
    printf("\t\tResults[c++] = Target[%d-i]-'0';\n",MaxDigit-1);
    printf("}\n");
    for(int i=0;i<MaxDigit;i++)
    {
        printf("\tTestArray[%d] = Test%d;\n",i,i+1);
        printf("\tCarryArray[%d] = Carry%d;\n",i,i+1);
    }
    printf("for(int i=0;i<9;i++)\n\tfor(int j=0;j<9;j++)\n\t\tlookup[i][j] = i * j % 10;\n");

    printf("\tNumerators[0] = 1; Denominators[0] = 1;\n}\n\n");

    PrintComment("Main");
    printf("int main(int argv,char *argc[])\n");
    printf("{\n\tSetup();\n");
    printf("bool (*Digit)();\nint (*Carry)();\nint rindex;\n");
    printf("bool recalc = false,done = false;\nint n,d,r;\n");
    printf("\trindex = 0;\n");
    printf("\twhile(!done)\n\t{\n");
    printf("\tDigit = TestArray[rindex];\n\tCarry = CarryArray[rindex];\n");
    printf("		r = (*Digit)();\n\
		if (recalc || (r!=Results[rindex]))\n\
		{\n\
            recalc = false;\n\
            r = Results[rindex] - (*Carry)();\n\
            n = Numerators[rindex];\n\
            d = Denominators[rindex];\n\
			do\n\
            {\n\
                n++;\n\
                if (n>9)\n\
                {\n\
                    n = 0; d++;\n\
                }\n\
                if (d>9)\n\
                {\n\
                    Numerators[rindex] = 0;\n\
                    Denominators[rindex] = 0;\n\
                    rindex--;\n\
                    recalc = true;\n\
                    break;\n\
                }\n\
                Numerators[rindex] = n;\n\
                Denominators[rindex] = d;\n\
            }while(r!= lookup[n][d]);\n\
		}\n\
        else\n\
        {\n\
            rindex++;\n\
            if (rindex>5)\n\
            {\n\
                printf(\"done\\n\\t\");\n\
                for(int i=0;i<6;i++)\n\
                    printf(\"%d\",Numerators[i]);\n\
                printf(\" X \");\n\
                for(int i=0;i<6;i++)\n\
                    printf(\"%d\",Denominators[i]);\n\
                printf(\" = \\n\");\n\
                for(int i=0;i<6;i++)\n\
                    printf(\"%d\",Results[i]);\n\
\t            }\n\
\t}\n}\n}\n");
}

void PrintCalcLineRow(int MaxNum,int MaxDen)
{
int MaxDigit = MaxNum *2;
    printf("\n");
for(int i=0;i<MaxNum;i++)
{
    printf("printf(\"");
    for(int j =0;j<MaxDigit;j++)
    {
    char FormatStr[30];
        sprintCalcLineRowfs(FormatStr,j,i,MaxDen);
        printf("%s\t",FormatStr);
    }
    printf("\",");
    for(int j =0;j<MaxDigit;j++)
    {
    char FormatStr[30];
        sprintCalcLineRowVar(FormatStr,j,i,MaxDen);
        printf("%s,",FormatStr);
    }
    printf(")\n");
}
}

int BigNumFactorMain(int argv,char *argc[])
{
    BuildRowColFunc(3,3);
    return 0;
//  BuildRowColFunc(25,25);

//    PrintCalcLineRow(3,3);
}
