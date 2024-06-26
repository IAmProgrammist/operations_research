/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Daniel Dorndorf <dorndorf@featdd.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <string>
#include <iostream>

/**
 * Fraction class
 */
class Fraction {
	private:
		// Fraction parts
		long numerator, denominator;
		// Euclidean algorithm for greatest common divisor
		long euclidean(long a, long b);
	public:
		// Constructors
		Fraction(void);
		Fraction(double Number);
		Fraction(std::string FractionString);
		// Destructur
		~Fraction(void);

		// Getter and Setter
		long getNumerator(void);
		long getDenominator(void);
		void setNumerator(long Numerator);
		void setDenominator(long Denominator);

		// Fraction functions
		bool reduce(void);
		void convertDoubleToFraction(double Number);
		double convertFractionToDouble(void);
		bool convertStringToFraction(std::string FractionString);
		bool isInt();
		Fraction getFrac();

		// Operator overloading functions
		bool operator<(Fraction fraction);
		bool operator<=(Fraction fraction);
		bool operator>(Fraction fraction);
		bool operator>=(Fraction fraction);
		bool operator==(Fraction fraction);
		bool operator!=(Fraction fraction);
		long operator%(Fraction fraction);
		operator double();
		operator float();
		operator long();
		operator std::string();
		Fraction operator+(Fraction fraction);
		Fraction operator+=(Fraction fraction);
		Fraction operator-=(Fraction fraction);
		Fraction operator-(Fraction fraction);
		Fraction operator*(Fraction fraction);
		Fraction operator*=(Fraction fraction);
		Fraction operator/(Fraction fraction);
		Fraction operator/=(Fraction fraction);
		Fraction operator~(void);
		Fraction operator++(void);
		Fraction operator--(void);

        // More functions
        friend Fraction abs(const Fraction &f) {
            Fraction fraction;
            fraction.setNumerator(f.numerator);
            fraction.setDenominator(f.denominator);

            if (fraction.getNumerator() < 0) fraction.setNumerator(-fraction.getNumerator());
            if (fraction.getDenominator() < 0) fraction.setDenominator(-fraction.getDenominator());

            return fraction;
        }
};

/**
 * FractionInputFailException class
 *
 * Exception object extending the c++ standard exceptions
 * Thrown exception can be handled like standard exceptions
*/
class FractionInputFailException: public std::exception {
	public: virtual const char* what() const throw() { return "Incorrect Input"; }
};

/** Left Shift Operator overloading functions (need to be declared global) */
std::ostream& operator<<(std::ostream &out, Fraction &Fraction);
/** Right Shift Operator overloading functions (need to be declared global) */
std::istream& operator>>(std::istream &in, Fraction &Fraction);

#endif