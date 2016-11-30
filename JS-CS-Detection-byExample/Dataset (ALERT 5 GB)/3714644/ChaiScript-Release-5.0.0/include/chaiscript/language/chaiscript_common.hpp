// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// and Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#ifndef CHAISCRIPT_COMMON_HPP_
#define	CHAISCRIPT_COMMON_HPP_

#include <sstream>
#include "../dispatchkit/dispatchkit.hpp"

namespace chaiscript
{
  /// Signature of module entry point that all binary loadable modules must implement.
  typedef ModulePtr (*Create_Module_Func)();


  /// Types of AST nodes available to the parser and eval
  class AST_Node_Type {
  public:
    enum Type { Error, Int, Float, Id, Char, Str, Eol, Fun_Call, Inplace_Fun_Call, Arg_List, Variable, Equation, Var_Decl,
                Comparison, Addition, Subtraction, Multiplication, Division, Modulus, Array_Call, Dot_Access, Quoted_String, Single_Quoted_String,
                Lambda, Block, Def, While, If, For, Inline_Array, Inline_Map, Return, File, Prefix, Break, Map_Pair, Value_Range,
                Inline_Range, Annotation, Try, Catch, Finally, Method, Attr_Decl, Shift, Equality, Bitwise_And, Bitwise_Xor, Bitwise_Or, 
                Logical_And, Logical_Or, Reference, Switch, Case, Default, Ternary_Cond
    };
  };

  namespace
  {
    /**
     * Helper lookup to get the name of each node type
     */
    const char *ast_node_type_to_string(int ast_node_type) {
      const char *ast_node_types[] = { "Internal Parser Error", "Int", "Float", "Id", "Char", "Str", "Eol", "Fun_Call", "Inplace_Fun_Call", "Arg_List", "Variable", "Equation", "Var_Decl",
                                    "Comparison", "Addition", "Subtraction", "Multiplication", "Division", "Modulus", "Array_Call", "Dot_Access", "Quoted_String", "Single_Quoted_String",
                                    "Lambda", "Block", "Def", "While", "If", "For", "Inline_Array", "Inline_Map", "Return", "File", "Prefix", "Break", "Map_Pair", "Value_Range",
                                    "Inline_Range", "Annotation", "Try", "Catch", "Finally", "Method", "Attr_Decl", "Shift", "Equality", "Bitwise_And", "Bitwise_Xor", "Bitwise_Or", 
                                    "Logical_And", "Logical_Or", "Reference", "Switch", "Case", "Default", "Ternary Condition"};

      return ast_node_types[ast_node_type];
    }
  }

  /// \brief Convenience type for file positions
  struct File_Position {
    int line;
    int column;

    File_Position(int t_file_line, int t_file_column)
      : line(t_file_line), column(t_file_column) { }

    File_Position() : line(0), column(0) { }
  };

  /// \brief Typedef for pointers to AST_Node objects. Used in building of the AST_Node tree
  typedef std::shared_ptr<struct AST_Node> AST_NodePtr;


  /// \brief Classes which may be thrown during error cases when ChaiScript is executing.
  namespace exception
  {
    /**
     * Errors generated during parsing or evaluation
     */
    struct eval_error : public std::runtime_error {
      std::string reason;
      File_Position start_position;
      File_Position end_position;
      std::string filename;
      std::vector<AST_NodePtr> call_stack;

      eval_error(const std::string &t_why, const File_Position &t_where, const std::string &t_fname,
          const std::vector<Boxed_Value> &t_parameters, const chaiscript::detail::Dispatch_Engine &t_ss) noexcept :
        std::runtime_error(format(t_why, t_where, t_fname, t_parameters, t_ss)),
        reason(t_why), start_position(t_where), end_position(t_where), filename(t_fname)
      {}

      eval_error(const std::string &t_why, 
           const std::vector<Boxed_Value> &t_parameters, const chaiscript::detail::Dispatch_Engine &t_ss) noexcept :
        std::runtime_error(format(t_why, t_parameters, t_ss)),
        reason(t_why)
      {}


      eval_error(const std::string &t_why, const File_Position &t_where, const std::string &t_fname) noexcept :
        std::runtime_error(format(t_why, t_where, t_fname)),
        reason(t_why), start_position(t_where), end_position(t_where), filename(t_fname)
      {}

      eval_error(const std::string &t_why) noexcept
        : std::runtime_error("Error: \"" + t_why + "\" "),
        reason(t_why) 
      {}

      virtual ~eval_error() noexcept {}

    private:
      static std::string format_why(const std::string &t_why)
      {
        return "Error: \"" + t_why + "\"";
      }

      static std::string format_parameters(const std::vector<Boxed_Value> &t_parameters,
          const chaiscript::detail::Dispatch_Engine &t_ss)
      {
        std::stringstream ss;
        ss << "With parameters: (";

        if (!t_parameters.empty())
        {
          std::string paramstr;

          for (const Boxed_Value &bv: t_parameters)
          {
            paramstr += (bv.is_const()?"const ":"");
            paramstr += t_ss.type_name(bv);
            paramstr += ", ";
          }

          ss << paramstr.substr(0, paramstr.size() - 2);
        }
        ss << ")";

        return ss.str();
      }

      static std::string format_filename(const std::string &t_fname)
      {
        std::stringstream ss;

        if (t_fname != "__EVAL__")
        {
          ss << "in '" << t_fname << "' ";
        } else {
          ss << "during evaluation ";
        }

        return ss.str();
      }

      static std::string format_location(const File_Position &t_where)
      {
        std::stringstream ss;
        ss << "at (" << t_where.line << ", " << t_where.column << ")";
        return ss.str();
      }

      static std::string format(const std::string &t_why, const File_Position &t_where, const std::string &t_fname,
          const std::vector<Boxed_Value> &t_parameters, const chaiscript::detail::Dispatch_Engine &t_ss)
      {
        std::stringstream ss;

        ss << format_why(t_why);
        ss << " ";

        ss << format_parameters(t_parameters, t_ss);
        ss << " ";

        ss << format_filename(t_fname);
        ss << " ";

        ss << format_location(t_where);

        return ss.str();
      }

      static std::string format(const std::string &t_why, 
          const std::vector<Boxed_Value> &t_parameters, const chaiscript::detail::Dispatch_Engine &t_ss)
      {
        std::stringstream ss;

        ss << format_why(t_why);
        ss << " ";

        ss << format_parameters(t_parameters, t_ss);
        ss << " ";

        return ss.str();
      }

      static std::string format(const std::string &t_why, const File_Position &t_where, const std::string &t_fname)
      {
        std::stringstream ss;

        ss << format_why(t_why);
        ss << " ";

        ss << format_filename(t_fname);
        ss << " ";

        ss << format_location(t_where);

        return ss.str();
      }
    };

    /**
     * Errors generated when loading a file
     */
    struct file_not_found_error : public std::runtime_error {
      file_not_found_error(const std::string &t_filename) noexcept
        : std::runtime_error("File Not Found: " + t_filename)
      { }

      virtual ~file_not_found_error() noexcept {}
    };

  }

 
  /// \brief Struct that doubles as both a parser ast_node and an AST node.
  struct AST_Node : std::enable_shared_from_this<AST_Node> {
    public:
      const std::string text;
      const int identifier; //< \todo shouldn't this be a strongly typed enum value?
      std::shared_ptr<const std::string> filename;
      File_Position start, end;
      std::vector<AST_NodePtr> children;
      AST_NodePtr annotation;

      /**
       * Prints the contents of an AST node, including its children, recursively
       */
      std::string to_string(std::string t_prepend = "") {
        std::ostringstream oss;

        oss << t_prepend << "(" << ast_node_type_to_string(this->identifier) << ") "
            << this->text << " : " << this->start.line << ", " << this->start.column << std::endl;
        
        for (unsigned int j = 0; j < this->children.size(); ++j) {
          oss << this->children[j]->to_string(t_prepend + "  ");
        }
        return oss.str();
      }

      std::string internal_to_string() {
        return to_string();
      }

      Boxed_Value eval(chaiscript::detail::Dispatch_Engine &t_e) 
      {
        try {
          return eval_internal(t_e);
        } catch (exception::eval_error &ee) {
          ee.call_stack.push_back(shared_from_this());
          throw ee;
        }
      }


      void replace_child(const AST_NodePtr &t_child, const AST_NodePtr &t_new_child)
      {
        std::replace(children.begin(), children.end(), t_child, t_new_child);
      }

    protected:
      AST_Node(const std::string &t_ast_node_text, int t_id, const std::shared_ptr<std::string> &t_fname, 
          int t_start_line, int t_start_col, int t_end_line, int t_end_col) :
        text(t_ast_node_text), identifier(t_id), filename(t_fname),
        start(t_start_line, t_start_col), end(t_end_line, t_end_col)
      {
      }

      AST_Node(const std::string &t_ast_node_text, int t_id, const std::shared_ptr<std::string> &t_fname) :
        text(t_ast_node_text), identifier(t_id), filename(t_fname) {}

      virtual ~AST_Node() {}

      virtual Boxed_Value eval_internal(chaiscript::detail::Dispatch_Engine &)
      {
        throw std::runtime_error("Undispatched ast_node (internal error)");
      }

    private:
      // Copy and assignment explicitly unimplemented
      AST_Node(const AST_Node &);
      AST_Node& operator=(const AST_Node &);
  };


  namespace eval
  {
    namespace detail
    {
      /**
       * Special type for returned values
       */
      struct Return_Value {
        Boxed_Value retval;

        Return_Value(const Boxed_Value &t_return_value) : retval(t_return_value) { }
      };

      /**
       * Special type indicating a call to 'break'
       */
      struct Break_Loop {
        Break_Loop() { }
      };

      /// Creates a new scope then pops it on destruction
      struct Scope_Push_Pop
      {
        Scope_Push_Pop(chaiscript::detail::Dispatch_Engine &t_de)
          : m_de(t_de)
        {
          m_de.new_scope();
        }

        ~Scope_Push_Pop()
        {
          m_de.pop_scope();
        }


        private:
        // explicitly unimplemented copy and assignment
        Scope_Push_Pop(const Scope_Push_Pop &);
        Scope_Push_Pop& operator=(const Scope_Push_Pop &);

        chaiscript::detail::Dispatch_Engine &m_de;
      };

      /// Creates a new functon call and pops it on destruction
      struct Function_Push_Pop
      {
        Function_Push_Pop(chaiscript::detail::Dispatch_Engine &t_de)
          : m_de(t_de)
        {
          m_de.new_function_call();
        }

        ~Function_Push_Pop()
        {
          m_de.pop_function_call();
        }

        void save_params(const std::vector<Boxed_Value> &t_params)
        {
          m_de.save_function_params(t_params);
        }


        private:
        // explicitly unimplemented copy and assignment
        Function_Push_Pop(const Function_Push_Pop &);
        Function_Push_Pop& operator=(const Function_Push_Pop &);

        chaiscript::detail::Dispatch_Engine &m_de;
      };

      /// Creates a new scope then pops it on destruction
      struct Stack_Push_Pop
      {
        Stack_Push_Pop(chaiscript::detail::Dispatch_Engine &t_de)
          : m_de(t_de)
        {
          m_de.new_stack();
        }

        ~Stack_Push_Pop()
        {
          m_de.pop_stack();
        }


        private:
        // explicitly unimplemented copy and assignment
        Stack_Push_Pop(const Stack_Push_Pop &);
        Stack_Push_Pop& operator=(const Stack_Push_Pop &);

        chaiscript::detail::Dispatch_Engine &m_de;
      };
    }
  }
}

#endif	/* _CHAISCRIPT_COMMON_HPP */
