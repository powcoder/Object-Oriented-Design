https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
/* Compile normally, without any macros to get the solution to the simple
 * task. Use macro 'EXTRA' to get the solution to the simple task AND the extra
 * challenge (i.e. -DEXTRA). */

#include <iostream>

using namespace std;

/* The precedence ranking of the expressions are defined below. By assigning
 * 10000 to non-negative integers, we have reserved enough sub-rankings between
 * each two levels we currently use while making our program future-proof to a
 * certain extent. */

#ifdef EXTRA
static const unsigned int PREC_NUMBER_POS = 10000;
static const unsigned int PREC_NUMBER_NEG = 10;
static const unsigned int PREC_ADDITION   = 10;
static const unsigned int PREC_MULTI      = 20;
#endif

class Expression {
	public:
		virtual int value() const = 0;
		virtual void print(ostream&) const = 0;
#ifdef EXTRA
		virtual int prec() const = 0;
#endif
};

ostream& operator <<(ostream& o, const Expression& e) {
	e.print(o);
	return o << " = " << e.value();
}

class Number : public Expression {
	private:
		int number_value;
	public:
		Number(int value) : number_value(value) {
		}
		virtual int value() const {
			return number_value;
		}
		virtual void print(ostream& o) const {
			o << number_value;
		}
#ifdef EXTRA
		virtual int prec() const {
				return number_value >= 0 ? PREC_NUMBER_POS : PREC_NUMBER_NEG;
		}
#endif
};

class LBinary : public Expression {
	private:
		Expression& left_expr;
		Expression& right_expr;
	protected:
		virtual int calculate(int v1, int v2) const = 0;
	public:
		LBinary(Expression& left, Expression& right)
			: left_expr(left), right_expr(right) {
		}
		Expression& left() const {
			return left_expr;
		}
		Expression& right() const {
			return right_expr;
		}
		virtual const char* symbol() const = 0;
		virtual void print(ostream& o) const {
#ifdef EXTRA
			bool left_lower = left_expr.prec() < prec();
#else
			bool left_lower = true;
#endif
			if(left_lower) {
				o << "(";
			}
			left_expr.print(o);
			if(left_lower) {
				o << ")";
			}
			o << symbol();
#ifdef EXTRA
			bool right_not_higher = right_expr.prec() <= prec();
#else
			bool right_not_higher = true;
#endif
			if(right_not_higher) {
				o << "(";
			}
			right_expr.print(o);
			if(right_not_higher) {
				o << ")";
			}
		}
		virtual int value() const {
			int left_value = left_expr.value();
			int right_value = right_expr.value();
			return calculate(left_value, right_value);
		}
};

class Addition : public LBinary {
	protected:
		virtual int calculate(int v1, int v2) const {
			return v1 + v2;
		}
	public:
		Addition(Expression& left, Expression& right) : LBinary(left, right) {}
		virtual const char* symbol() const {
			return "+";
		}
#ifdef EXTRA
		virtual int prec() const {
			return PREC_ADDITION;
		}
#endif
};

class Multiplication : public LBinary {
	protected:
		virtual int calculate(int v1, int v2) const {
			return v1 * v2;
		}
	public:
		Multiplication(Expression& left, Expression& right)
			: LBinary(left, right) {
		}
		virtual const char* symbol() const {
			return "*";
		}
#ifdef EXTRA
		virtual int prec() const {
			return PREC_MULTI;
		}
#endif
};
