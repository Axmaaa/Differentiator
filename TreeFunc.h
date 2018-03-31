#define FIRST 1
#define SECOND 0

#define NUMBER 1
#define VARIABLE 2
#define OPERATOR 3
#define PLUS 1
#define MINUS 2
#define MUL 3
#define DIVIDE 4
#define LOGARITHM 5
#define EXPONENT 6
#define SINUS 7
#define COSINUS 8
#define TANGENT 9
#define COTANGENT 10
#define HYP_SIN 11
#define HYP_COS 12

#define _NUMBER(number) NodeCreate(NUMBER, number, NULL, NULL)
#define _VARIABLE(variable) NodeCreate(VARIABLE, variable, NULL, NULL)
#define _PLUS(NodePointer1, NodePointer2) NodeCreate(OPERATOR, PLUS, NodePointer1, NodePointer2)
#define _MINUS(NodePointer1, NodePointer2) NodeCreate(OPERATOR, MINUS, NodePointer1, NodePointer2)
#define _MUL(NodePointer1, NodePointer2) NodeCreate(OPERATOR, MUL, NodePointer1, NodePointer2)
#define _DIVIDE(NodePointer1, NodePointer2) NodeCreate(OPERATOR, DIVIDE, NodePointer1, NodePointer2)
#define _LOGARITHM(NodePointer) NodeCreate(OPERATOR, LOGARITHM, NULL, NodePointer)
#define _EXPONENT(NodePointer1, NodePointer2) NodeCreate(OPERATOR, EXPONENT, NodePointer1, NodePointer2)
#define _SINUS(NodePointer) NodeCreate(OPERATOR, SINUS, NULL, NodePointer)
#define _COSINUS(NodePointer) NodeCreate(OPERATOR, COSINUS, NULL, NodePointer)
#define _TANGENT(NodePointer) NodeCreate(OPERATOR, TANGENT, NULL, NodePointer)
#define _COTANGENT(NodePointer) NodeCreate(OPERATOR, COTANGENT, NULL, NodePointer)
#define _HYP_SIN(NodePointer) NodeCreate(OPERATOR, HYP_SIN, NULL, NodePointer)
#define _HYP_COS(NodePointer) NodeCreate(OPERATOR, HYP_COS, NULL, NodePointer)

char mainvar = 'x';

struct Node * NodeCreate(int type, int value, struct Node * l, struct Node * r);
void TreeDelete(struct Node * n);
struct Node * TreeCopy(struct Node * n);
struct Node * TreeDiff(struct Node * n, FILE * output);
struct Node * TreeDiffPrint(struct Node * n, FILE * output, int FirstFlag); // FirstFlag = 0, if it's not first undertree I need diff, else it isn't 0
int TreeValidation(struct Node * n);
void TreeOptimization(struct Node * n);
void TreeSimplification(struct Node * n, int * pcount);
void TreePrintTex(struct Node * n, FILE * output);
void TreePrintExpression(struct Node * n, FILE * output);
int GCD(int a, int b);
int TreePrintDot(struct Node * n);
int TreePrintRec(struct Node * n, FILE * output);

struct Node
{
    int type;
    int value;
    struct Node * left;
    struct Node * right;
};

struct Node * NodeCreate(int t, int v, struct Node * l, struct Node * r)
{
    struct Node * ptr = (struct Node *)calloc(1, sizeof(struct Node));
    ptr->type = t;
    ptr->value = v;
    ptr->left = l;
    ptr->right = r;
    return ptr;
}

void TreeDelete(struct Node * n)
{
    if (n->left != NULL)
        TreeDelete(n->left);
    if (n->right != NULL)
        TreeDelete(n->right);
    free(n);
}

struct Node * TreeCopy(struct Node * n)
{
    struct Node * ptr;
    ptr = (struct Node *)calloc(1, sizeof(struct Node));
    ptr->type = n->type;
    ptr->value = n->value;
    if (n->left != NULL)
        ptr->left = TreeCopy(n->left);
    if (n->right != NULL)
        ptr->right = TreeCopy(n->right);
    return ptr;
}

struct Node * TreeDiff(struct Node * n, FILE * output)
{
    switch (n->type)
    {
        case NUMBER:
        {
            fprintf(output, "Производная числа всегда равна нулю\\\\");
            return _NUMBER(0);
        }
        case VARIABLE:
        {
            if (n->value == mainvar)
            {
                fprintf(output, "$%c$ - главная переменная\\\\", n->value);
                return _NUMBER(1);
            }
            else
            {
                fprintf(output, "$%c$ - не главная переменная\\\\", n->value);
                return _NUMBER(0);
            }
        }
        case OPERATOR:
            switch (n->value)
            {
                case PLUS: // (f + g)' = f' + g'
                {
                    struct Node * f1, * g1;
                    fprintf(output, "По правилу производной суммы $\\left(f + g\\right)' = f' + g'$, где $f = ");
                    TreePrintExpression(n->left, output);
                    fprintf(output, "$, а $g = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f1 = TreeDiffPrint(n->left, output, FIRST);
                    g1 = TreeDiffPrint(n->right, output, SECOND);
                    return _PLUS(f1, g1);
                }
                case MINUS: // (f - g)' = f' - g'
                {
                    struct Node * f1, * g1;
                    fprintf(output, "По правилу производной разности $\\left(f - g\\right)' = f' - g'$, где $f = ");
                    TreePrintExpression(n->left, output);
                    fprintf(output, "$, а $g = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f1 = TreeDiffPrint(n->left, output, FIRST);
                    g1 = TreeDiffPrint(n->right, output, SECOND);
                    return _MINUS(f1, g1);
                }
                case MUL: // (f * g)' = f' * g + f * g'; f' = f1, g' = g1;
                {
                    struct Node * u, * v, * u1, * v1;
                    fprintf(output, "По правилу производной произведения $\\left(f * g\\right)' = f' * g + f * g'$, где $f = ");
                    TreePrintExpression(n->left, output);
                    fprintf(output, "$, а $g = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    u = TreeCopy(n->left);
                    v = TreeCopy(n->right);
                    u1 = TreeDiffPrint(n->left, output, FIRST);
                    v1 = TreeDiffPrint(n->right, output, SECOND);
                    return _PLUS(_MUL(u1, v), _MUL(u, v1));
                }
                case DIVIDE: // (f / g)' = (f' * g - f * g') / (g ^ 2)
                {
                    struct Node * f, * g, * f1, * g1;
                    fprintf(output, "По правилу производной частного $\\left(\\frac{f}{g}\\right)' = \\frac{f' * g - f * g'}{g ^ {2}}$, где $f = ");
                    TreePrintExpression(n->left, output);
                    fprintf(output, "$, а $g = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->left);
                    g = TreeCopy(n->right);
                    f1 = TreeDiffPrint(n->left, output, FIRST);
                    g1 = TreeDiffPrint(n->right, output, SECOND);
                    return _DIVIDE(_MINUS(_MUL(f1, g), _MUL(f, g1)), _EXPONENT(TreeCopy(g), _NUMBER(2)));
                }
                case LOGARITHM: // ln(f)' = f' / f
                {
                    struct Node * f, * f1;
                    fprintf(output, "По правилу производной логарифма $\\left(ln\\left(f\\right)\\right)' = \\frac{f'}{f}$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    f1 = TreeDiffPrint(n->right, output, FIRST);
                    return _DIVIDE(f1, f);
                }
                case EXPONENT:
                {
                    if (n->right->type == NUMBER || (n->right->type == VARIABLE && n->right->value != mainvar)) // (f ^ a)' == a * f ^ (a - 1) * f'
                    {
                        struct Node * a, * aa, * f, * f1;
                        fprintf(output, "По правилу производной показательной функции $\\left(f ^ {\\alpha}\\right)' = \\alpha * f ^ {\\alpha - 1} * f'$, где $f = ");
                        TreePrintExpression(n->left, output);
                        fprintf(output, "$, а $\\alpha = ");
                        TreePrintExpression(n->right, output);
                        fprintf(output, "$\\\\\n");
                        f = TreeCopy(n->left);
                        a = TreeCopy(n->right);
                        aa = TreeCopy(a);
                        f1 = TreeDiffPrint(n->left, output, FIRST);
                        return _MUL(_MUL(a, _EXPONENT(f, _MINUS(aa, _NUMBER(1)))), f1);
                    }
                    else if (n->left->type == NUMBER || (n->left->type == VARIABLE && n->left->value != mainvar)) // (a ^ f)' = a ^ f * ln(a) * f'
                    {
                        struct Node *f, * f1, * a, * lna;
                        fprintf(output, "По правилу производной экспоненты $\\left(\\alpha ^ {f}\\right)' = \\alpha ^ {f} * ln\\left(\\alpha\\right) * f'$, где $\\alpha = ");
                        TreePrintExpression(n->left, output);
                        fprintf(output, "$, а $f = ");
                        TreePrintExpression(n->right, output);
                        fprintf(output, "$\\\\\n");
                        f1 = TreeDiffPrint(n->right, output, FIRST);
                        a = TreeCopy(n->left);
                        lna = _LOGARITHM(TreeCopy(a));
                        f = TreeCopy(n->right);
                        return _MUL(_MUL(_EXPONENT(a, f), lna), f1);
                    }
                    else  // (f ^ g)' = (e ^ (ln(f) * g))' = e ^ (ln(f) * g) * (ln(f) * g)'
                    {
                        struct Node * e, * f, * g, *gg, * g1, * lnf, *lnf1, * lnf_copy;
                        fprintf(output, "По правилу такой дичи, как $f ^ {g}$: $\\left(f ^ {g}\\right)' = (e ^ {ln\\left(f\\right) * g})' = e ^ {ln\\left(f\\right) * g} * \\left(ln\\left(f\\right) * g\\right)'$, где $\\alpha = ");
                        TreePrintExpression(n->left, output);
                        fprintf(output, "$, а $f = ");
                        TreePrintExpression(n->right, output);
                        fprintf(output, "$\\\\\n");
                        e = _VARIABLE('e');
                        f = TreeCopy(n->left);
                        lnf = _LOGARITHM(f);
                        lnf1 = TreeDiffPrint(lnf, output, FIRST);
                        lnf_copy = TreeCopy(lnf);
                        g = TreeCopy(n->right);
                        gg = TreeCopy(g);
                        g1 = TreeDiffPrint(g, output, SECOND);
                        return _MUL(_EXPONENT(e, _MUL(g, lnf)), _PLUS(_MUL(g1, lnf_copy), _MUL(gg, lnf1)));
                    }
                }
                case SINUS: // (sin(x))' = cos(x) * x'
                {
                    struct Node * f, * f1;
                    fprintf(output, "По правилу производной синуса $\\left(sin\\left(f\\right)\\right)' = cos\\left(f\\right) * f'$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    f1 = TreeDiffPrint(f, output, FIRST);
                    return _MUL(_COSINUS(f), f1);
                }
                case COSINUS: // (cos(f)' = 0 - sin(f) * f'
                {
                    struct Node * f, * f1;
                    fprintf(output, "По правилу производной косинуса $\\left(cos\\left(f\\right)\\right)' = -sin\\left(f\\right) * f'$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    f1 = TreeDiffPrint(f, output, FIRST);
                    return _MINUS(_NUMBER(0), _MUL(_SINUS(f), f1));
                }
                case TANGENT: // (tg(f))' = (1 / (cos(f)) ^ 2) * f'
                {
                    struct Node * f, * cosf, * f1;
                    fprintf(output, "По правилу производной тангенса $\\left(tg\\left(f\\right)\\right)' = \\frac{f'}{cos\\left(f\\right) ^ {2}}$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    cosf = _COSINUS(f);
                    f1 = TreeDiffPrint(f, output, FIRST);
                    return _DIVIDE(f1, _EXPONENT(cosf, _NUMBER(2)));
                }
                case COTANGENT: // (ctg(f))' = 0 - (f' / (sin(f)) ^ 2)
                {
                    struct Node * f, * sinf, * f1;
                    fprintf(output, "По правилу производной котангенса $\\left(ctg\\left(f\\right)\\right)' = -\\frac{f'}{sin\\left(f\\right) ^ {2}}$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    sinf = _SINUS(f);
                    f1 = TreeDiffPrint(f, output, FIRST);
                    return _MINUS(_NUMBER(0), _DIVIDE(f1, _EXPONENT(sinf, _NUMBER(2))));
                }
                case HYP_SIN: // (sinh(f))' = cosh(f) * f'
                {
                    struct Node * f, * f1;
                    fprintf(output, "По правилу производной гиперболического синуса $\\left(sh\\left(f\\right)\\right)' = ch\\left(f\\right) * f'$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    f1 = TreeDiff(f, output);
                    return _MUL(_HYP_COS(f), f1);
                }
                case HYP_COS: // (cosh(f))' = sinh(f) * f'
                {
                    struct Node * f, * f1;
                    fprintf(output, "По правилу производной гиперболического косинуса $\\left(ch\\left(f\\right)\\right)' = sh\\left(f\\right) * f'$, где $f = ");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "$\\\\\n");
                    f = TreeCopy(n->right);
                    f1 = TreeDiff(f, output);
                    return _MUL(_HYP_SIN(f), f1);
                }
                default: printf("Что-то здесь не то, я не знаю такой функции\n"); break;
            }
        default: printf("Что-то здесь не то, я не знаю такого типа\n"); break;
    }
    return NULL;
}

struct Node * TreeDiffPrint(struct Node * n, FILE * output, int FirstFlag) // FirstFlag = 0, if it's not first undertree I need diff, else it isn't 0
{
    struct Node * n1;
    if (FirstFlag != 0)
        fprintf(output, "Продифференцируем $f = ");
    else
        fprintf(output, "\nТеперь продифференцируем $g = ");
    TreePrintExpression(n, output);
    fprintf(output, "$:\\\\");
    n1 = TreeDiff(n, output);
    fprintf(output, "$\\left(");
    TreePrintExpression(n, output);
    fprintf(output, "\\right)' = ");
    TreePrintExpression(n1, output);
    fprintf(output, "$\\\\");
    return n1;
}

int TreeValidation(struct Node * n)
{
    if (n->type == NUMBER)
    {
        if (n->left != NULL)
        {
            fprintf(stderr, "Error, node on pointer %p with type NUMBER has left undertree.\n", n);
            exit(1);
        }
        if (n->right != NULL)
        {
            fprintf(stderr, "Error, node on pointer %p with type NUMBER has right undertree.\n", n);
            exit(2);
        }
    }
    else if (n->type == VARIABLE)
    {
        if (n->left != NULL)
        {
            fprintf(stderr, "Error, node on pointer %p with type VARIABLE has left undertree.\n", n);
            exit(3);
        }
        if (n->right != NULL)
        {
            fprintf(stderr, "Error, node on pointer %p with type VARIABLE has right undertree.\n", n);
            exit(4);
        }
    }
    else if (n->type == OPERATOR)
    {
        if (n->value == PLUS)
        {
            if (n->left == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR PLUS hasn't left undertree.\n", n);
                exit(5);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR PLUS hasn't right undertree.\n", n);
                exit(6);
            }
        }
        else if (n->value == MINUS)
        {
            if (n->left == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR MINUS hasn't left undertree.\n", n);
                exit(7);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR MINUS hasn't right undertree.\n", n);
                exit(8);
            }
        }
        else if (n->value == MUL)
        {
            if (n->left == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR MUL hasn't left undertree\n", n);
                exit(9);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR MUL hasn't right undertree\n", n);
                exit(10);
            }
        }
        else if (n->value == DIVIDE)
        {
            if (n->left == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR DIVIDE hasn't a numerator\n", n);
                exit(11);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR DIVIDE hasn't a denominator", n);
                exit(12);
            }
            if (n->right->type == NUMBER && n->right->value == 0)
            {
                fprintf(stderr, "Error, there is division by zero at node on pointer %p\n", n);
                exit(13);
            }
        }
        else if (n->value == LOGARITHM)
        {
            if (n->left != NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR LOGARITHM has left undertree\n", n);
                exit(14);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR LOGARITHM hasn't right undertree\n", n);
                exit(15);
            }
            if (n->right->type == NUMBER && n->right->value <= 0)
            {
                fprintf(stderr, "Error, the argument of the logarithm at node on pointer %p is less than zero\n", n);
                exit(16);
            }
        }
        else if (n->value == EXPONENT)
        {
            if (n->left == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR EXPONENT hasn't foundation\n", n);
                exit(17);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p wih\th type OPERATOR EXPONENT hasn't exponent\n", n);
                exit(18);
            }
            if (n->left->type == NUMBER && n->left->value == 0 && n->right->type == NUMBER && n->right->value == 0)
            {
                fprintf(stderr, "Error, at node on pointer %p with type OPERATOR EXPONENT the foundation and the exponent are zero\n", n);
                exit(19);
            }
        }
        else if (n->value == SINUS)
        {
            if (n->left != NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR SINUS has left undertree\n", n);
                exit(20);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR SINUS hasn't right undertree\n", n);
                exit(21);
            }
        }
        else if (n->value == COSINUS)
        {
            if (n->left != NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR COSINUS has left undertree\n", n);
                exit(22);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR COSINUS hasn't right undertree\n", n);
                exit(23);
            }
        }
        else if (n->value == COTANGENT)
        {
            if (n->left != NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR COTANGENT has left undertree\n", n);
                exit(24);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR COTANGENT hasn't right undertree\n", n);
                exit(25);
            }
            if (n->right->type == NUMBER && n->right->value == 0)
            {
                fprintf(stderr, "Error, the argument of cotangent at node with pointer %p is zero\n", n);
            }
        }
        else if (n->value == HYP_SIN)
        {
            if (n->left != NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR HYPERBOLIC SINUS has left undertree\n", n);
                exit(26);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR HYPERBOLIC SINUS hasn't right undertree\n", n);
                exit(27);
            }
        }
        else if (n->value == HYP_COS)
        {
            if (n->left != NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR HYPERBOLIC COSINUS has left undertree\n", n);
                exit(28);
            }
            if (n->right == NULL)
            {
                fprintf(stderr, "Error, node on pointer %p with type OPERATOR HYPERBOLIC COSINUS hasn't right undertree\n", n);
                exit(29);
            }
        }
    }
    if (n->left != NULL)
        TreeValidation(n->left);
    if (n->right != NULL)
        TreeValidation(n->right);
    return 0;
}

void TreeOptimization(struct Node * n)
{
    int changes = 0;
    do
    {
        changes = 0;
        TreeSimplification(n, &changes);
    } while (changes != 0);
}

void TreeSimplification(struct Node * n, int * pcount)
{
    struct Node * tmp;
    if (n->type == OPERATOR && n->value == PLUS)
    {
        if (n->left->type == NUMBER && n->right->type == NUMBER)
        {
            n->type = NUMBER;
            n->value = n->left->value + n->right->value;
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->left->value == 0)
        {
            // Скопировать правое поддерево в узел
            tmp = TreeCopy(n->right);
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
        else if (n->right->type == NUMBER && n->right->value == 0)
        {
            // Скопировать левое поддерево в узел
            tmp = TreeCopy(n->left);
            TreeDelete(n->right);
            TreeDelete(n->left);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == MINUS)
    {
        if (n->left->type == NUMBER && n->right->type == NUMBER)
        {
            n->type = NUMBER;
            n->value = n->left->value - n->right->value;
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->right->type == NUMBER && n->right->value == 0)
        {
            tmp = TreeCopy(n->left);
            TreeDelete(n->right);
            TreeDelete(n->left);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == MUL)
    {
        if (n->left->type == NUMBER && n->right->type == NUMBER)
        {
            n->type = NUMBER;
            n->value = n->left->value * n->right->value;
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if ((n->left->type == NUMBER && n->left->value == 0) || (n->right->type == NUMBER && n->right->value == 0))
        {
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->left->value == 1)
        {
            tmp = TreeCopy(n->right);
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
        else if (n->right->type == NUMBER && n->right->value == 1)
        {
            tmp = TreeCopy(n->left);
            TreeDelete(n->right);
            TreeDelete(n->left);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->left->value == -1)
        {
            n->left->value = 0;
            n->value = MINUS;
        }
        else if (n->right->type == NUMBER && n->right->value == -1)
        {
            tmp = TreeCopy(n->left);
            TreeDelete(n->left);
            n->right->type = tmp->type;
            n->right->value = tmp->value;
            n->right->left = tmp->left;
            n->right->right = tmp->right;
            n->left = _NUMBER(0);
            n->value = MINUS;
            free(tmp);
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == DIVIDE)
    {
        if (n->left->type == NUMBER && n->right->type == NUMBER && ((n->left->value % n->right->value) == 0))
        {
            n->type = NUMBER;
            n->value = n->left->value / n->right->value;
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->right->type == NUMBER && (GCD(n->left->value, n->right->value != 1)))
        {
            n->left->value /= GCD(n->left->value, n->right->value != 1);
            n->right->value /= GCD(n->left->value, n->right->value != 1);
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->left->value == 0)
        {
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->right->type == NUMBER && n->right->value == 1)
        {
            tmp = TreeCopy(n->left);
            TreeDelete(n->right);
            TreeDelete(n->left);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
        else if (n->right->type == OPERATOR && n->right->type == DIVIDE)
        {
            n->value = MUL;
            tmp = n->right->left;
            n->right->left = n->right->right;
            n->right->right = tmp;
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == EXPONENT)
    {
        if (n->left->type == NUMBER && n->right->type == NUMBER && (n->left->value != 0 || n->right->value != 0))
        {
            n->type = NUMBER;
            n->value = pow(n->left->value, n->right->value);
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->right->type == NUMBER && n->right->value == 0)
        {
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 1;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->right->type == NUMBER && n->right->value == 1)
        {
            tmp = TreeCopy(n->left);
            TreeDelete(n->right);
            TreeDelete(n->left);
            n->type = tmp->type;
            n->value = tmp->value;
            n->left = tmp->left;
            n->right = tmp->right;
            free(tmp);
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->left->value == 0)
        {
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->left->type == NUMBER && n->left->value == 1)
        {
            TreeDelete(n->left);
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 1;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == LOGARITHM)
    {
        if (n->right->type == NUMBER && n->right->value == 1)
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
        else if (n->right->type == VARIABLE && n->right->value == 'e')
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 1;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }

    }
    else if (n->type == OPERATOR && n->value == SINUS)
    {
        if (n->right->type == NUMBER && n->right->value == 0)
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == COSINUS)
    {
        if (n->right->type == NUMBER && n->right->value == 0)
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 1;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == TANGENT)
    {
        if (n->right->type == NUMBER && n->right->value == 0)
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == HYP_SIN)
    {
        if (n->right->type == NUMBER && n->right->value == 0)
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 0;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
    }
    else if (n->type == OPERATOR && n->value == HYP_COS)
    {
        if (n->right->type == NUMBER && n->right->value == 0)
        {
            TreeDelete(n->right);
            n->type = NUMBER;
            n->value = 1;
            n->left = NULL;
            n->right = NULL;
            (*pcount) += 1;
        }
    }
    if (*pcount != 0)
        return;
    if (n->left != NULL)
        TreeSimplification(n->left, pcount);
    if (n->right != NULL)
        TreeSimplification(n->right, pcount);
}

void TreePrintTex(struct Node * n, FILE * output)
{
    fprintf(output, "  $$");
    TreePrintExpression(n, output);
    fprintf(output, "$$\n");
}

void TreePrintExpression(struct Node * n, FILE * output)
{
    if (n->left == NULL && n->right == NULL)
        switch (n->type)
        {
            case NUMBER: fprintf(output, "%d", n->value); break;
            case VARIABLE: fprintf(output, "%c", n->value); break;
        }
    else
    {
        if (n->value == PLUS)
        {

            TreePrintExpression(n->left, output);
            fprintf(output, " + ");
            TreePrintExpression(n->right, output);
        }
        if (n->value == MINUS)
        {
            if (n->left->type == NUMBER && n->left->value == 0)
                fprintf(output, "\\left(");
            else
                TreePrintExpression(n->left, output);
            fprintf(output, " - ");
            if (n->right->type == OPERATOR && (n->right->value == PLUS || n->right->value == MINUS))
                {
                    fprintf(output, "\\left(");
                    TreePrintExpression(n->right, output);
                    fprintf(output, "\\right)");
                }
            else
                TreePrintExpression(n->right, output);
            if (n->left->type == NUMBER && n->left->value == 0)
                fprintf(output, "\\right)");
        }
        if (n->value == MUL)
        {
            if (n->left->type == OPERATOR && (n->left->value == PLUS || n->left->value == MINUS))
            {
                fprintf(output, "\\left(");
                TreePrintExpression(n->left, output);
                fprintf(output, "\\right)");
            }
            else
                TreePrintExpression(n->left, output);
            fprintf(output, " \\cdot ");
            if (n->right->type == OPERATOR && (n->right->value == PLUS || n->right->value == MINUS))
            {
                fprintf(output, "\\left(");
                TreePrintExpression(n->right, output);
                fprintf(output, "\\right)");
            }
            else
                TreePrintExpression(n->right, output);
        }
        if (n->value == DIVIDE)
        {
            fprintf(output, "\\frac{");
            TreePrintExpression(n->left, output);
            fprintf(output, "}{");
            TreePrintExpression(n->right, output);
            fprintf(output, "}");
        }
        if (n->value == LOGARITHM)
        {
            fprintf(output, "ln\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
        if (n->value == EXPONENT)
        {
            if (n->left->type == OPERATOR && (n->left->value == PLUS || n->left->value == MINUS || n->left->value == MUL || n->left->value == DIVIDE))
            {
                fprintf(output, "\\left(");
                TreePrintExpression(n->left,output);
                fprintf(output, "\\right)");
            }
            else
            {
                TreePrintExpression(n->left, output);
            }
            fprintf(output, "^{");
            TreePrintExpression(n->right, output);
            fprintf(output, "}");
        }
        if (n->value == SINUS)
        {
            fprintf(output, "sin\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
        if (n->value == COSINUS)
        {
            fprintf(output, "cos\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
        if (n->value == TANGENT)
        {
            fprintf(output, "tg\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
        if (n->value == COTANGENT)
        {
            fprintf(output, "ctg\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
        if (n->value == HYP_SIN)
        {
            fprintf(output, "sh\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
        if (n->value == HYP_COS)
        {
            fprintf(output, "ch\\left(");
            TreePrintExpression(n->right, output);
            fprintf(output, "\\right)");
        }
    }
}

int GCD(int a, int b)
{
    while (a > 0 && b > 0)
    {
        if (b < a)
        {
            a = a - b * (a / b);
        }
        else
        {
            b = b - a * (b / a);
        }
    }
    if (a == 0)
        return b;
    else
        return a;
}

int TreePrintDot(struct Node * n)
{
    FILE * output = fopen("dump.dot", "w");
    fprintf(output, "digraph dump {\n node [shape = record];\n");
    TreePrintRec(n, output);
    fprintf(output, "}");
    fclose(output);
    system("dot -Tps dump.dot -o dumpimg.ps");
    system("xdg-open dumpimg.ps");
    return 0;
}

int TreePrintRec(struct Node * n, FILE * output)
{
    if (n->type == VARIABLE)
        fprintf(output, "%ld [shape = record, label = \"{ptr: %p|type: %d|value: %c|left: %p|right: %p}\"];", (long int)n, n, n->type, n->value, n->left, n->right);
    else
        fprintf(output, "%ld [shape = record, label = \"{ptr: %p|type: %d|value: %d|left: %p|right: %p}\"];", (long int)n, n, n->type, n->value, n->left, n->right);
    if(n -> left != NULL)
    {
        fprintf(output, "%ld -> %ld\n", (long int)n, (long int)n->left);
        TreePrintRec(n -> left, output);
    }
    if(n -> right != NULL)
    {
        fprintf(output, "%ld -> %ld\n", (long int)n, (long int)n->right);
        TreePrintRec(n -> right, output);
    }
    return 0;
}

