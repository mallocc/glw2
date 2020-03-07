#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <ctime>

#define MAX_LINE 300

#define PAD2ZEROS std::setfill('0') << std::setw(2)


namespace glw
{
  namespace util
  {
    class Logger
    {
    public:

      // STATIC //

      static Logger& getInstance()
      {
        static Logger instance;
        return instance;
      }

      static void LOpenFile(const std::string& filename)
      {
        getInstance().openFile(filename.c_str());
      }

      static void LCloseFile()
      {
        getInstance().closeFile();
      }

      static void LAppendFile(const char * line)
      {
        if (getInstance().isOpen())
        {
          getInstance().appendFile(line);
        }
      }

      static std::string LDATE_TIME()
      {
        std::stringstream ss;
        std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        ss << (now->tm_year + 1900) << '-';
        ss << PAD2ZEROS << (now->tm_mon + 1);
        ss << '-';
        ss << PAD2ZEROS << now->tm_mday;
        ss << '_';
        ss << PAD2ZEROS << now->tm_hour;
        ss << ':';
        ss << PAD2ZEROS << now->tm_min;
        ss << ':';
        ss << PAD2ZEROS << now->tm_sec;
        return ss.str();
      }
      static std::string LDATE_TIME_F()
      {
        std::stringstream ss;
        std::time_t t = std::time(0);   // get time now
        std::tm* now = std::localtime(&t);
        ss << (now->tm_year + 1900);
        ss << PAD2ZEROS << (now->tm_mon + 1);
        ss << PAD2ZEROS << now->tm_mday ;
        ss << '_';
        ss << PAD2ZEROS << now->tm_hour;
        ss << PAD2ZEROS << now->tm_min;
        ss << PAD2ZEROS << now->tm_sec;
        return ss.str();
      }

      static std::string LFILENAME(std::string str)
      {
        return str.substr(str.find_last_of("/\\") + 1);
      }

      static std::string LSIGNITURE(const char * classname, const char * func)
      {
        char CLASS[MAX_LINE];
        sprintf(CLASS,
                "(%s", classname);
        char FUNC[MAX_LINE];
        sprintf(FUNC,
                "%s)", func);
        char output[MAX_LINE];
        sprintf(output, "%15.15s::%-15.15s", CLASS, FUNC);
        return std::string(output);
      }

      static void LCritical(
          const char * traceGroup, std::string str,
          const char * file, unsigned int line,
          const char * classname, const char * func)
      {
        char output[MAX_LINE];
        sprintf(output,
                "[%s] :%-5.5s [CRITICAL] %s %s <%s:%u>\n",
                LDATE_TIME().c_str(), traceGroup,
                LSIGNITURE(classname, func).c_str(),
                str.c_str(),
                LFILENAME(file).c_str(), line);
        LAppendFile(output);
        printf("%s", output);
      }

      static void LError(
          const char * traceGroup, std::string str,
          const char * file, unsigned int line,
          const char * classname, const char * func)
      {
        char output[MAX_LINE];
        sprintf(output,
                "[%s] :%-5.5s [ERROR   ] %s %s <%s:%u>\n",
                LDATE_TIME().c_str(), traceGroup,
                LSIGNITURE(classname, func).c_str(),
                str.c_str(),
                LFILENAME(file).c_str(), line);
        LAppendFile(output);
        printf("%s", output);
      }

      static void LWarning(
          const char * traceGroup, std::string str,
          const char * file, unsigned int line,
          const char * classname, const char * func)
      {
        char output[MAX_LINE];
        sprintf(output,
                "[%s] :%-5.5s [WARNING ] %s %s <%s:%u>\n",
                LDATE_TIME().c_str(), traceGroup,
                LSIGNITURE(classname, func).c_str(),
                str.c_str(),
                LFILENAME(file).c_str(), line);
        LAppendFile(output);
        printf("%s", output);
      }

      static void LDebug(
          const char * traceGroup, std::string str,
          const char * classname, const char * func)
      {
        char output[MAX_LINE];
        sprintf(output,
                "[%s] :%-5.5s [DEBUG   ] %s %s\n",
                LDATE_TIME().c_str(), traceGroup,
                LSIGNITURE(classname, func).c_str(),
                str.c_str());
        LAppendFile(output);
        printf("%s", output);
      }

      static void LInfo(const char * traceGroup, std::string str,
                        const char * classname = "", const char * func = "")
      {
        char output[MAX_LINE];
        sprintf(output,
                "[%s] :%-5.5s [INFO    ] %s %s\n",
                LDATE_TIME().c_str(), traceGroup,
                LSIGNITURE(classname, func).c_str(),
                str.c_str());
        LAppendFile(output);
        printf("%s", output);
      }

      static void LTrace()
      {
        char output[MAX_LINE];
        sprintf(output,
                "[%s] :%-5.5s [TRACE   ] %s\n",
                LDATE_TIME().c_str(), "NLL",
                "");
        LAppendFile(output);
        printf("%s", output);
      }

      // NON STATIC //

      void openFile(const std::string& filename)
      {
        char file[100];
        sprintf(file, "%s__%s.log", filename.c_str(), LDATE_TIME_F().c_str());
        m_file.open(file);
      }

      void closeFile()
      {
        m_file.close();
      }
      void appendFile(const char * line)
      {
        m_file << line;
      }

      bool isOpen()
      {
        return m_file.is_open();
      }

    private:
      Logger() {}

      std::ofstream m_file;
    };
  }
}

// GLOBAL DEFINES //

#define __XCLASSNAME__ typeid(this).name()
#define LINFO_ARGS __CLASSNAME__, __func__
#define LDEBUG_ARGS LINFO_ARGS
#define LWARNING_ARGS __FILE__, __LINE__, LDEBUG_ARGS
#define LERROR_ARGS LWARNING_ARGS
#define LCRITICAL_ARGS LERROR_ARGS

#define LDEFINECLASS(trg, classname) namespace { const char * TRG = trg; const char * __CLASSNAME__ = classname; }

#define LCRITICAL(str)  glw::util::Logger::LCritical(TRG, str, LCRITICAL_ARGS)
#define LERROR(str)     glw::util::Logger::LError(TRG, str, LERROR_ARGS)
#define LWARNING(str)   glw::util::Logger::LWarning(TRG, str, LWARNING_ARGS)
#define LDEBUG(str)     glw::util::Logger::LDebug(TRG, str, LDEBUG_ARGS)
#define LINFO(str)      glw::util::Logger::LInfo(TRG, str, LINFO_ARGS)
#define LTRACE()        glw::util::Logger::LTrace()

#define LASSERT_TRUE(b) LINFO(b ? "ASSERT_TRUE: PASSED" : "ASSERT_TRUE: FAILED")
#define LASSERT_FALSE(b) LINFO(!b ? "ASSERT_FALSE: PASSED" : "ASSERT_FALSE: FAILED")

#define LSTARTLOGGER(str) glw::util::Logger::LOpenFile(str)
#define LENDLOGGER() glw::util::Logger::LCloseFile()


#endif // LOGGER_H
