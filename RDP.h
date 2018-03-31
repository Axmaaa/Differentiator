struct Node * GetG0();
struct Node * GetN();
struct Node * GetV();
struct Node * GetT();
struct Node * GetE();
struct Node * GetD();
struct Node * GetP();
struct Node * GetR();

int IsNumeral(char c);
int IsLetter(char c);

char expression[100] = {};
int current = 0;

struct Node * GetG0()
{
    printf("Вызвана функция G0\n");
    printf("%c\n", expression[current]);
    struct Node * n = NULL;
    if (expression[current] != '\n')
        n = GetT();
    printf("Закончилась функция G0\n");
    return n;
}

struct Node * GetN()
{
    printf("Вызвана функция N\n");
    printf("%c\n", expression[current]);
    int num = 0, rat = 0, k = 0;
    struct Node * n;
    if (!IsNumeral(expression[current]))
    {
        fprintf(stderr, "Какая-то фигня на символе %c с номером %d.\n", expression[current], current);
        exit(1);
    }
    while (IsNumeral(expression[current]))
    {
        num *= 10;
        num += expression[current] - '0';
        current++;
    }
    if (expression[current] == '.')
    {
        current++;
        while (IsNumeral(expression[current]))
        {
            rat *= 10;
            rat += expression[current] - '0';
            current++;
            k++;
        }
    }
    if (k == 0)
    {
        n = _NUMBER(num);
    }
    else
    {
        n = _PLUS(NULL, NULL);
        n->left = _NUMBER(num);
        n->right = _DIVIDE(NULL, NULL);
        n->right->left = _NUMBER(rat);
        n->right->right = _NUMBER(pow(10, k));
    }
    printf("Закончилась функция N\n");
    return n;
}

struct Node * GetV()
{
    printf("Вызвана функция V\n");
    printf("%c\n", expression[current]);
    char v;
    struct Node * n;
    v = expression[current];
    current++;
    n = _VARIABLE(v);
    printf("Закончилась функция V\n");
    return n;
}

struct Node * GetT()
{
    printf("Вызвана функция T\n\n");
    printf("%c\n", expression[current]);
    struct Node * n, * l, * r;
    n = GetE();
    while (expression[current] == '+' || expression[current] == '-')
    {
        printf("plus\n");
        l = TreeCopy(n);
        TreeDelete(n);
        if (expression[current] == '+')
            n = _PLUS(NULL, NULL);
        else if (expression[current] == '-')
            n = _MINUS(NULL, NULL);
        else
        {
            fprintf(stderr, "Какая-то фигня на символе %c с номером %d.\n", expression[current], current);
            exit(2);
        }
        current++;
        n->left = l;
        r = GetE();
        n->right = r;
    }
    printf("Закончилась функция T\n");
    return n;
}

struct Node * GetE()
{
    printf("Вызвана функция E\n");
    printf("%c\n", expression[current]);
    struct Node * n, * l, * r;
    n = GetD();
    while (expression[current] == '*' || expression[current] == '/')
    {
        l = TreeCopy(n);
        TreeDelete(n);
        if (expression[current] == '*')
            n = _MUL(NULL, NULL);
        else if (expression[current] == '/')
            n = _DIVIDE(NULL, NULL);
        else
        {
            fprintf(stderr, "Какая-то фигня на символе %c с номером %d.\n", expression[current], current);
            exit(3);
        }
        current++;
        n->left = l;
        r = GetD();
        n->right = r;
    }
    printf("Закончилась функция E\n");
    return n;
}

struct Node * GetD()
{
    printf("Вызвана функция D\n");
    printf("%c\n", expression[current]);
    struct Node * n, * l, * r;
    n = GetP();
    while (expression[current] == '^')
    {
        l = TreeCopy(n);
        TreeDelete(n);
        if (expression[current] == '^')
            n = _EXPONENT(NULL, NULL);
        else
        {
            fprintf(stderr, "Какая-то фигня на символе %c с номером %d.\n", expression[current], current);
            exit(4);
        }
        current++;
        n->left = l;
        r = GetP();
        n->right = r;
    }
    printf("Закончилась функция D\n");
    return n;
}

struct Node * GetP()
{
    printf("Вызвана функция P\n");
    printf("%c\n", expression[current]);
    struct Node * n;
    if (expression[current] == '(')
    {
        current++;
        n = GetT();
        current++;
    }
    else if (IsNumeral(expression[current]))
    {
        n = GetN();
    }
    else if (IsLetter(expression[current + 1]))
    {
        printf("aaaaaaaaaaaaaaaa\n");
        n = GetR();
    }
    else if (IsLetter(expression[current]))
    {
        n = GetV();
    }
    printf("lalka %p\n", n);
    printf("Закончилась функция P\n");
    return n;
}

struct Node * GetR()
{
    printf("Вызвана функция R\n");
    printf("%c\n", expression[current]);
    struct Node * n, * r;
    // else if (expression[current] == '' && expression[current + 1] == '')
    // else if (expression[current] == '' && expression[current + 1] == '' && expression[current + 2] == '')
    if (expression[current] == 'l' && expression[current + 1] == 'n')
    {
        current += 2;
        n = _LOGARITHM(NULL);
        r = GetP();
        n->right = r;
    }
    else if (expression[current] == 's' && expression[current + 1] == 'i' && expression[current + 2] == 'n')
    {
        current += 3;
        n = _SINUS(NULL);
        r = GetP();
        n->right = r;
    }
    else if (expression[current] == 'c' && expression[current + 1] == 'o' && expression[current + 2] == 's')
    {
        current += 3;
        n = _COSINUS(NULL);
        r = GetP();
        n->right = r;
    }
    if (expression[current] == 't' && expression[current + 1] == 'g')
    {
        current += 2;
        n = _TANGENT(NULL);
        r = GetP();
        n->right = r;
    }
    else if (expression[current] == 'c' && expression[current + 1] == 't' && expression[current + 2] == 'g')
    {
        current += 3;
        n = _COTANGENT(NULL);
        r = GetP();
        n->right = r;
    }
    if (expression[current] == 's' && expression[current + 1] == 'h')
    {
        current += 2;
        n = _HYP_SIN(NULL);
        r = GetP();
        n->right = r;
    }
    if (expression[current] == 'c' && expression[current + 1] == 'h')
    {
        current += 2;
        n = _HYP_COS(NULL);
        r = GetP();
        n->right = r;
    }
    printf("meow %p;\n", n);
    printf("Закончилась функция R\n");
    return n;
}

int IsNumeral(char c)
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
        return 1;
    else
        return 0;
}

int IsLetter(char c)
{
    if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z')
        return 1;
    else
        return 0;
}
