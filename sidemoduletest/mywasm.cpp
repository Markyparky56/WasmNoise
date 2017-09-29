enum class Type
{
    Cat, Dog
};

Type type;

extern "C"
{
int doubler(int x)
{
    return 2*x;
}

int tripler(int x)
{
    return 3*x;
}

class MyClass
{
public:
    int myFunc();
    static int myStaticFunc();
    Type myType();
} myClass;

int MyClass::myFunc()
{
    return 42;
}

int MyClass::myStaticFunc()
{
    return 4242;
}

Type MyClass::myType()
{
    return Type::Cat;
}

int myClassMyFunc()
{
    return myClass.myFunc();
}

double eval2(double x, double y)
{
    return x*y;
}

double eval3(double x, double y, double z)
{
    return x*y*z;
}

double eval4(double x, double y, double z, double w)
{
    return x*y*z*w;
}
}
