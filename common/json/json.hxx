/*! \file json.hxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _JSON_HXX_
#define _JSON_HXX_

#include <vector>
#include <string>
#include <map>

namespace JSON {

  //! types of JSON values
    enum Type {
	T_OBJECT,
	T_ARRAY,
	T_NUMBER,
	T_STRING,
	T_BOOL,
	T_NULL
    };

    class Value;
    class Object;
    class Array;
    class Number;
    class String;
    class Bool;
    class Null;

  // parse a JSON file; this returns nullptr if there is a parsing error
    Value *ParseFile (std::string filename);

  // virtual base class of JSON values
    class Value {
      public:

	Type type() const { return this->_ty; }

	bool isObject() const { return (this->_ty == T_OBJECT); }
	bool isArray() const { return (this->_ty == T_ARRAY); }
	bool isNumber() const { return (this->_ty == T_NUMBER); }
	bool isString() const { return (this->_ty == T_STRING); }
	bool isBool() const { return (this->_ty == T_BOOL); }
	bool isNull() const { return (this->_ty == T_NULL); }

	const Object *asObject () const;
	const Array *asArray () const;
	const Number *asNumber () const;
	const String *asString () const;
	const Bool *asBool () const;

	virtual ~Value() = 0;
	virtual std::string toString() = 0;

      protected:

	explicit Value (Type ty) : _ty(ty) { };

	Type	_ty;
    };

    inline std::ostream& operator<< (std::ostream& s, Value *v)
    {
	return s << v->toString();
    }

  // JSON objects
    class Object : public Value {
      public:
	Object ();
	~Object ();

	int size () const { return this->_value.size(); }

	void insert (std::string key, Value *val);

	Value *operator[] (std::string key) const;

	std::string toString();

      private:
	std::map<std::string, Value *> _value;
    };

  // JSON arrays
    class Array : public Value {
      public:
	Array ();
	~Array ();

	int length () const { return static_cast<int>(this->_value.size()); }
	void add (Value *v) { this->_value.push_back(v); }

	Value *operator[] (int idx) const { return this->_value[idx]; }

	std::string toString();

      private:
	std::vector<Value *> _value;
    };

    class Number : public Value {
      public:
	Number (double v);
	~Number ();

	double value () const { return this->_value; }

	std::string toString();

      private:
	double _value;
    };

    class String : public Value {
      public:
	String (std::string v);
	~String ();

	std::string value () const { return this->_value; }

	std::string toString();

      private:
	std::string _value;
    };

    class Bool : public Value {
      public:
	Bool (bool v);
	~Bool ();

	bool value () const { return this->_value; }

	std::string toString();

      private:
	bool _value;
    };

    class Null : public Value {
      public:
	Null ();
	~Null ();

	std::string toString();

    };

}; // namespace JSON

#endif // !_JSON_HXX_
