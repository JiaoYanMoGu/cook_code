#include "cmdline.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace unicorn;

int main(int argc, char** argv)
{
//    Switch: 不需要参数
//    Value<T>: 给定参数值，可以有默认值
//    Implicit<T>: 默认值

    // 简单用法
    float f;
    int m, i;
    bool v;

    OptionParser op("Allowed options");
    auto help_option = op.add<Switch>("h","help","produce help message");
    auto groff_option = op.add<Switch>("","groff","produce groff formatted help message");
    auto bash_option = op.add<Switch>("","bash","produce bash completion script");
    auto verbose_option = op.add<Switch,Attribute::optional>("v","verbose","be verbose",&v);
    auto hidden_option = op.add<Switch,Attribute::hidden>("x","","hidden option");
    auto double_option = op.add<Value<double>>("d","double","test for double value",3.1415926);
    auto float_option = op.add<Value<float>>("f","float","test for float values",2.718281828,&f);
    auto int_option = op.add<Value<int>>("i","int","test for int values",12306);
    auto string_option = op.add<Value<string>>("s","string","test for string value","fuck u");
    auto implicit_int_option = op.add<Implicit<int>>("m","implicit","implicit test",42);
    auto advanced_option = op.add<Switch,Attribute::advanced>("","advanced","advanced option");

    op.parse(argc,argv);
    if (help_option->count() == 1)
        cout << op << "\n";
    else if (help_option->count() == 2)
        cout << op.help(Attribute::advanced) << "\n";
    else if (help_option->count() > 2)
        cout << op.help(Attribute::expert) << "\n";

    if (groff_option->is_set())
    {
        GroffOptionPrinter option_printer(&op);
        cout << option_printer.print();
    }

    if (bash_option->is_set())
    {
        BashCompletionOptionPrinter option_printer(&op, "popl_example");
        cout << option_printer.print();
    }

    // show all non option arguments (those without "-o" or "--option")
    for (const auto& non_option_arg: op.non_option_args())
        cout << "non_option_args: " << non_option_arg << "\n";

    // show unknown options (undefined ones, like "-u" or "--undefined")
    for (const auto& unknown_option: op.unknown_options())
        cout << "unknown_options: " << unknown_option << "\n";

    // print all the configured values
    cout << "verbose_option  - is_set: " << verbose_option->is_set() << ", count: " << verbose_option->count() << ", reference: " << v << "\n";
    cout << "hidden_option   - is_set: " << hidden_option->is_set() << ", count: " << hidden_option->count() << "\n";
    cout << "double_option   - is_set: " << double_option->is_set() << ", count: " << double_option->count() << ", value: " << double_option->value() << "\n";
    cout << "string_option   - is_set: " << string_option->is_set() << ", count: " << string_option->count() << "\n";


    return 0;
}
