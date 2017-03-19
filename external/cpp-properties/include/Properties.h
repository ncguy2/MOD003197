//
// Created by nick on 13/03/17.
//

#ifndef FIRESIM_PROPERTIES_H
#define FIRESIM_PROPERTIES_H

#include <string>
#include <vector>
#include <map>

    class PropertiesException : public std::exception {
    public:
        PropertiesException(const char* msg) throw() : message(msg) {}
        virtual ~PropertiesException() throw() {}
        virtual const char* what() const throw() { return message; }
    private:
        const char* message;
    };
    class PropertyNotFoundException : public std::exception {
    public:
        PropertyNotFoundException(const char* msg) throw() : message(msg) {}
        ~PropertyNotFoundException() throw() {}
        virtual const char* what() const throw() { return message; }

    private:
        const char* message;
    };

    class Properties {
    public:
        static Properties& Instance() {
            static Properties instance;
            return instance;
        }
        std::string GetProperty(std::string key) const;
        std::string GetProperty(std::string key, const std::string& defaultValue) const;
        std::vector<std::string> GetPropertyNames() const;
        void AddProperty(std::string key, const std::string& value);
        void RemoveProperty(std::string key);

        int GetIntProperty(std::string key);
        int GetIntProperty(std::string key, int defVal);

        float GetFloatProperty(std::string key);
        float GetFloatProperty(std::string key, float defVal);

        bool GetBoolProperty(std::string key);
        bool GetBoolProperty(std::string key, bool defVal);

    private:
        Properties();
        std::vector<std::string> keys;
        std::map<std::string, std::string> properties;
    };


#endif //FIRESIM_PROPERTIES_H
