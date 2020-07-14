#include <iostream>
#include <math.h>
#include "IVector.h"



void test() {
    std::cout << "Vector test, good luck!" << std::endl;

    const size_t dim2 = 2;
    const size_t dim3 = 3;

    double a1[dim3], a3[dim3];

    for (size_t i = 0; i < dim3; ++i) {
        a1[i] = i * 2;
        a3[i] = i * 3;
    }

    double a2[dim2] = { 1, 2 };
    double a4[dim3] = { NAN, NAN, NAN };

    ILogger* logger = ILogger::createLogger(nullptr);
    if (!logger) {
        std::cout << "VECTOR: not enough memory";
        return;
    }
    logger->setLogFile("vectorTestingLogger.txt");

    double tolerance = 1e-3;
    bool* result = new bool(false);
    RESULT_CODE rc = RESULT_CODE::SUCCESS;

    IVector* vec1 = IVector::createVector(dim3, a1, logger);
    IVector* vec2 = IVector::createVector(dim2, a2, logger);
    IVector* vec3 = IVector::createVector(dim3, a3, logger);

    //nullptr
    IVector* vec4 = IVector::createVector(dim3, a4, logger);
    IVector* vec5 = IVector::createVector(dim3, nullptr, logger);


    std::cout << "createVector(+)";
    if (vec1 && vec2 && vec3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "createVector(NAN)";
    if (!vec4)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "createVector(nullptr)";
    if (!vec5)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "clone(+)";
    IVector* vec_cl = vec1->clone();
    rc = IVector::equals(vec_cl, vec1, IVector::NORM::NORM_1, tolerance, result, logger);
    if (rc == RESULT_CODE::SUCCESS && *result)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "add(+)";
    IVector* a = IVector::add(vec1, vec3, logger);
    if (a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete a;

    std::cout << "add(nullptr)";
    a = IVector::add(vec4, vec1, logger);
    if (!a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(wrong dim)";
    a = IVector::add(vec1, vec2, logger);
    if (!a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "sub(+)";
    a = IVector::sub(vec1, vec3, logger);
    if (a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete a;

    std::cout << "sub(nullptr)";
    a = IVector::sub(vec4, vec3, logger);
    if (!a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(wrong dim)";
    a = IVector::sub(vec1, vec2, logger);
    if (!a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "mul(scaleParam)(+)";
    double scaleParam = 2;
    a = IVector::mul(vec1, scaleParam, logger);
    if (a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete a;

    std::cout << "mul(scaleParam)(nullptr)";
    a = IVector::mul(vec4, scaleParam, logger);
    if (!a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "mul(scaleParam = NAN)";
    a = IVector::mul(vec1, NAN, logger);
    if (!a)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "mul(+)";
    scaleParam = IVector::mul(vec1, vec3, logger);
    if (!isnan(scaleParam))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "mul(nullptr)";
    scaleParam = IVector::mul(vec4, vec1, logger);
    if (isnan(scaleParam))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "mul(wrong dim)";
    scaleParam = IVector::mul(vec1, vec2, logger);
    if (isnan(scaleParam))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "equals(+)";
    rc = IVector::equals(vec1, vec3, IVector::NORM::NORM_1, tolerance, result, logger);
    if (rc == RESULT_CODE::SUCCESS && !*result)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "equals(+)";
    rc = IVector::equals(vec1, vec1, IVector::NORM::NORM_1, tolerance, result, logger);
    if (rc == RESULT_CODE::SUCCESS && *result)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "equals(nullptr)";
    rc = IVector::equals(vec4, vec1, IVector::NORM::NORM_1, tolerance, result, logger);
    if (rc != RESULT_CODE::SUCCESS)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "equals(wrong dim)";
    rc = IVector::equals(vec1, vec2, IVector::NORM::NORM_1, tolerance, result, logger);
    if (rc != RESULT_CODE::SUCCESS)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "equals(tolerance = NAN)";
    rc = IVector::equals(vec1, vec3, IVector::NORM::NORM_1, NAN, result, logger);
    if (rc != RESULT_CODE::SUCCESS)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "getCoord(+)";
    double coord = vec1->getCoord(vec1->getDim() - 1);
    if (!isnan(coord))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "getCoord(-)";
    coord = vec1->getCoord(vec1->getDim());
    if (isnan(coord))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    scaleParam = 2;
    std::cout << "setCoord(+)";
    rc = vec1->setCoord(vec1->getDim() - 1, scaleParam);
    if (rc == RESULT_CODE::SUCCESS)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "setCoord(-)";
    rc = vec1->setCoord(vec1->getDim(), scaleParam);
    if (rc != RESULT_CODE::SUCCESS)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "setCoord(value = NAN)";
    rc = vec1->setCoord(vec1->getDim() - 1, NAN);
    if (rc != RESULT_CODE::SUCCESS)
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "NORM1(+)";
    coord = vec1->norm(IVector::NORM::NORM_1);
    if (!isnan(coord))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "NORM2(+)";
    coord = vec1->norm(IVector::NORM::NORM_2);
    if (!isnan(coord))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "NORM_INF(+)";
    coord = vec1->norm(IVector::NORM::NORM_INF);
    if (!isnan(coord))
        std::cout << "  Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    delete vec1;
    delete vec2;
    delete vec3;
    delete vec_cl;

    logger->destroyLogger(nullptr);
}

int main()
{
    test();
    return 0;
}
