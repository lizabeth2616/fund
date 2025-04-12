#include <iostream>
#include <cmath>

class Complex {
private:
    double real;
    double imag;    

public:
    Complex(double r = 0, double i = 0) {
        real = r;
        imag = i;
    }//lego


    Complex add(const Complex &other) const {
        return Complex(real + other.real, imag + other.imag);
    }

 
    Complex subtract(const Complex &other) const {
        return Complex(real - other.real, imag - other.imag);
    }


    //Complex c = a - b
    Complex operator-(const Complex &other) const {
        return (*this).subtract(other);
    }

    Complex multiply(const Complex &other) const {
        return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
    }    
    
 
    Complex share(const Complex &other) const {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) {
            throw std::string{"Division into 0"};
        }                                                 
        double r = (real * other.real + imag * other.imag) / denominator;
        double i = (imag * other.real - real * other.imag) / denominator;
        return Complex(r, i);
    }

    double modul() const {
        return std::sqrt(real * real + imag * imag);
    }

    double argument() const {
        return std::atan2(imag, real);
    }

    void display() const {
        std::cout << real;
        if (imag >= 0) {
            std::cout << " + " << imag << "i";
        } else {
            std::cout << " - " << -imag << "i";
        }
        std::cout << std::endl;
    }
};

int main() {
    Complex c1(3, 4);     // 3 + 4i
    Complex c2(-2, 1);
    
    Complex a (3, 4);
    Complex b (1, 0);// 1 - 2i
    Complex c = a - b;
    c = a.operator-(b);

    std::cout << "The first complex number: ";
    c1.display();

    std::cout << "The second complex number: ";
    c2.display();

    Complex sum = c1.add(c2);
    std::cout << "Addition: ";
    sum.display();

    Complex diff = c1.subtract(c2);
    std::cout << "Subtraction: ";
    diff.display();


    Complex product = c1.multiply(c2);
    std::cout << "Multiplication: ";
    product.display();
    try {
        Complex quotient = c1.share(c2);
        std::cout << "Division: ";
        quotient.display();
    } catch (const std::string &err_mes) {
        std::cout << err_mes << std::endl;
    }

    std::cout << "The module of the first number: " << c1.modul() << std::endl;

    std::cout << "First number argument: " << c1.argument() << std::endl;

    return 0;
}
