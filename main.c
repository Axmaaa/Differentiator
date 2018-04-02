/**

Добавить оператор
Добавить операцию
Добавить дифференцирование и промежуточный вывод
Добавить упрощение
Добавить проверку корректности
Добавить печать

**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include "TreeFunc.h"
#include "RDP.h"


int main()
{
    FILE * result, * input;
    struct Node * tree, * tree_simple, * der, * der_simple;
    char * cap = "\\documentclass[12pt]{article}\n\\usepackage{ucs}\n\\usepackage[utf8x]{inputenc}\n\\usepackage[english,russian]{babel}\n\\pagestyle{empty}\n\\usepackage{amsmath}\n\n\\begin{document}\n";
    //mainvar = 'x';

    /*tree = _MINUS(NULL, NULL);
    tree->left = _PLUS(_VARIABLE('x'), _VARIABLE('a'));
    tree->right = _MINUS(_NUMBER(5), _VARIABLE('x'));*/

    /*tree = _HYP_COS(NULL);
    tree->right = _EXPONENT(NULL, NULL);
    tree->right->left = _VARIABLE('x');
    tree->right->right = _VARIABLE('a');*/

    /*tree = _EXPONENT(NULL, NULL);
    tree->left = _VARIABLE('y');
    tree->right = _SINUS(NULL);
    tree->right->right = _VARIABLE('x');*/

    /*tree = NodeCreate(OPERATOR, LOGARITHM, NULL, NULL);
    tree->right = NodeCreate(OPERATOR, DIVIDE, NULL, NULL);
    tree->right->left = _NUMBER(1);
    tree->right->right = NodeCreate(OPERATOR, MUL, NULL, NULL);
    tree->right->right->left = _VARIABLE('x');
    tree->right->right->right = NodeCreate(OPERATOR, SINUS, NULL, NULL);
    tree->right->right->right->right = NodeCreate(OPERATOR, EXPONENT, NULL, NULL);
    tree->right->right->right->right->left = _VARIABLE('x');
    tree->right->right->right->right->right = _NUMBER(2);*/


    result = fopen("result.tex", "w");
    fprintf(result, "%s", cap);
    input = fopen("input.txt", "r");
    fgets(expression, 100, input);
    fscanf(input, "%c", &mainvar);
    tree = GetG0();
    fclose(input);
    TreeValidation(tree);
    tree_simple = TreeCopy(tree);
    TreeOptimization(tree_simple);
    TreePrintTex(tree, result);
    fprintf(result, "После упрощения получаем\n  ");
    TreePrintTex(tree_simple, result);
    der = TreeDiff(tree, result);
    TreeValidation(der);
    der_simple = TreeCopy(der);
    TreeOptimization(der_simple);
    //TreePrintDot(tree);
    TreePrintDot(tree_simple);
    //TreePrintDot(der);
    //TreePrintDot(der_simple);


    fprintf(result, "Итак, производная равна\n  ");
    TreePrintTex(der, result);
    fprintf(result, "После упрощения получаем\n  ");
    TreePrintTex(der_simple, result);
    fprintf(result, "\\end{document}");
    fclose(result);

    TreeDelete(tree);
    TreeDelete(der);
    system("pdflatex result.tex");
    system("xdg-open result.pdf");
    return 0;
}
