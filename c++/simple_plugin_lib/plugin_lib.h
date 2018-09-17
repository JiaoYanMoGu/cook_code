#ifndef PLUGIN_LIB_H
#define PLUGIN_LIB_H

#include <map>
#include <string>
#include <iostream>

typedef void* (*PtrCreateObject)(void);

class plugin_lib
{
public:
    plugin_lib(){}
    ~plugin_lib(){}
    inline static plugin_lib& getInstance()
    {
        static plugin_lib plugins;
        return plugins;
    }
    inline void register_class(std::string name,PtrCreateObject func)
    {
        class_map.insert(std::pair<std::string,PtrCreateObject>(name,func));
    }
    inline void Print()
    {
        std::cout<<"plugin lib members: \n";
        for(auto i: class_map)
        {
           std::cout<<i.first<<std::endl;
        }
    }
    void* getClassByName(std::string name)
    {
        for(auto i:class_map)
        {
            if(i.first == name)
            {
                return i.second();
            }
        }
        return nullptr;
    }

private:
    plugin_lib(const plugin_lib &p);
    std::map<std::string,PtrCreateObject> class_map;
};

class RegisterAction{
public:
    RegisterAction(std::string class_name, PtrCreateObject obj)
    {
        plugin_lib::getInstance().register_class(class_name,obj);
    }
};

//使用宏定义来简化操作
#define REGISTER(className)                                             \
    className* objectCreator##className(){                              \
        return new className;                                           \
    }                                                                   \
    RegisterAction g_creatorRegister##className(                        \
        #className,(PtrCreateObject)objectCreator##className)           \

#endif // PLUGIN_LIB_H
