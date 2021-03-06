#ifndef STRINGFORMAT_H
#define STRINGFORMAT_H

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace glw
{
  namespace util
  {
    class StringFormat
    {
    public:
      StringFormat
      arg(std::string str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }
      StringFormat
      arg(float str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }
      StringFormat
      arg(int str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }
      StringFormat
      arg(unsigned int str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }
      StringFormat
      arg(long str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }
      StringFormat
      arg(size_t str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }
      StringFormat
      arg(unsigned char *str)
      {
        m_args.push_back(ARG(str));
        return *this;
      }

      std::string
      str()
      {
        std::stringstream ss;
        int pos = 0;
        if (m_format.empty())
        {
          for (int i = 0; i < m_args.size(); ++i)
          {
            ss << "%" << i << " ";
          }
          m_format = ss.str();
          ss       = std::stringstream("");
        }

        for (int i = 0; i < m_format.size(); ++i)
        {
          switch (m_format[i])
          {
          case '\\':
            i++;
            if (m_format[i] == 'n')
              ss << '\n';
            else
              ss << m_format[i];
            break;
          case '%':
            i++;
            pos = m_format[i] - 48;
            if (pos >= 0 && pos < 10 && pos < m_args.size())
            {
              switch (m_args[pos].TYPE)
              {
              case ARG::ARG_STRING:
                ss << m_args[pos].data_s;
                break;
              case ARG::ARG_FLOAT:
                ss << m_args[pos].data_f;
                break;
              case ARG::ARG_INT:
                ss << m_args[pos].data_i;
                break;
              case ARG::ARG_UINT:
                ss << m_args[pos].data_u;
                break;
              case ARG::ARG_LONG:
                ss << m_args[pos].data_l;
                break;
              case ARG::ARG_SIZE_T:
                ss << m_args[pos].data_st;
                break;
              case ARG::ARG_CHAR:
                ss << m_args[pos].data_c;
                break;
              }
            }
            break;
          default:
            ss << m_format[i];
          }
        }
        return ss.str();
      }

      StringFormat() {}
      StringFormat(std::string format)
      {
        m_format = format;
      }

    private:
      struct ARG
      {
        enum
        {
          ARG_NULL,
          ARG_STRING,
          ARG_FLOAT,
          ARG_INT,
          ARG_UINT,
          ARG_LONG,
          ARG_SIZE_T,
          ARG_CHAR
        };

        int TYPE = ARG_NULL;

        std::string data_s;
        float data_f;
        int data_i;
        unsigned int data_u;
        long data_l;
        size_t data_st;
        unsigned char *data_c;

        ARG(std::string data)
        {
          TYPE   = ARG_STRING;
          data_s = data;
        }
        ARG(float data)
        {
          TYPE   = ARG_FLOAT;
          data_f = data;
        }
        ARG(int data)
        {
          TYPE   = ARG_INT;
          data_i = data;
        }
        ARG(unsigned int data)
        {
          TYPE   = ARG_UINT;
          data_u = data;
        }
        ARG(long data)
        {
          TYPE   = ARG_LONG;
          data_l = data;
        }
        ARG(size_t data)
        {
          TYPE    = ARG_SIZE_T;
          data_st = data;
        }
        ARG(unsigned char *data)
        {
          TYPE   = ARG_CHAR;
          data_c = data;
        }
      };

      std::string m_str;
      std::string m_format;
      std::vector<ARG> m_args;
    };
  } // namespace util
} // namespace glw

#define PRINTVAL(val) glw::util::StringFormat("%0").arg(val).str()

#endif // STRINGFORMAT_H
