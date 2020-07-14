#include <iostream>
#include <math.h>
#include "ICompact.h"
#include "IVector.h"



void test() {
    std::cout << "Compact test, good luck!" << std::endl;

    bool result = false;
    const size_t dim2 = 2;
    const size_t dim3 = 3;

    double a1[dim3], a3[dim3];

    for (size_t i = 0; i < dim3; ++i) {
        a1[i] = (i + 1) * 2;
        a3[i] = (i + 1) * 3;
    }

    double a2[dim2] = { 1, 2 };
    double a4[dim2] = { 4, 6 };

    double a5[dim2] = { 2, 3 };
    double a6[dim2] = { 5, 7 };

    double a7[dim2] = { 100, 110 };
    double a8[dim2] = { 200, 210 };

    ILogger* logger = ILogger::createLogger(nullptr);
    if (!logger) {
        std::cout << "COMPACT: not enough memory";
        return;
    }
    logger->setLogFile("compactTestingLogger.txt");

    RESULT_CODE rc1, rc2 = RESULT_CODE::SUCCESS;

    IVector* vec1 = IVector::createVector(dim3, a1, logger);

    IVector* vec2 = IVector::createVector(dim2, a2, logger);

    IVector* vec3 = IVector::createVector(dim3, a3, logger);

    IVector* vec4 = IVector::createVector(dim2, a4, logger);

    IVector* vec5 = IVector::createVector(dim2, a5, logger);

    IVector* vec6 = IVector::createVector(dim2, a6, logger);

    IVector* vec7 = IVector::createVector(dim2, a7, logger);

    IVector* vec8 = IVector::createVector(dim2, a8, logger);

    ICompact* c1 = ICompact::createCompact(vec1, vec3, logger);
    ICompact* c2 = ICompact::createCompact(vec2, vec4, logger);
    ICompact* c4 = ICompact::createCompact(vec5, vec6, logger);
    ICompact* c6 = ICompact::createCompact(vec7, vec8, logger);

    std::cout << "createCompact(+)";
    if (c1 && c2)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;
    
    std::cout << "createCompact(left > right)";
    ICompact* c3 = ICompact::createCompact(vec4, vec2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "createCompact(nullptr)";
    c3 = ICompact::createCompact(nullptr, vec2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "createCompact(wrong dim)";
    c3 = ICompact::createCompact(vec1, vec2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "intersection(+)";
    c3 = ICompact::intersection(c2, c4, logger);
    if (c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete c3;

    std::cout << "intersection(intersection is empty)";
    c3 = ICompact::intersection(c2, c6, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "intersection(wrong dim)";
    c3 = ICompact::intersection(c1, c2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "intersection(nullptr)";
    c3 = ICompact::intersection(nullptr, c2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "add(-)";
    c3 = ICompact::add(c2, c4, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(intersection is empty)";
    c3 = ICompact::add(c4, c6, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(nullptr)";
    c3 = ICompact::add(nullptr, c2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(wrong dim)";
    c3 = ICompact::add(c1, c4, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "makeConvex(+)";
    c3 = ICompact::makeConvex(c2, c4, logger);
    if (c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete c3;

    std::cout << "makeConvex(nullptr)";
    c3 = ICompact::makeConvex(nullptr, c2, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "makeConvex(wrong dim)";
    c3 = ICompact::makeConvex(c1, c4, logger);
    if (!c3)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "isContains(+)";
    double a[dim2] = { 150, 160 };
    IVector* vec = IVector::createVector(dim2, a, logger);
    rc1 = c6->isContains(vec, result);
    if (rc1 == RESULT_CODE::SUCCESS && result)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete vec;

    std::cout << "isContains(-)";
    rc1 = c6->isContains(vec2, result);
    if (rc1 == RESULT_CODE::SUCCESS && !result)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isContains(wrong dim)";
    rc1 = c6->isContains(vec1, result);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isContains(nullptr)";
    rc1 = c6->isContains(nullptr, result);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "isSubSet(+)";

    double a9[dim2] = { 150, 160 };
    double a10[dim2] = { 170, 180 };
    IVector* v1 = IVector::createVector(dim2, a9, logger);
    IVector* v2 = IVector::createVector(dim2, a10, logger);
    ICompact* c7 = ICompact::createCompact(v1, v2, logger);

    delete v1;
    delete v2;

    rc1 = c6->isSubSet(c7, result);
    if (rc1 == RESULT_CODE::SUCCESS && result)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isSubSet(-)";
    rc1 = c6->isSubSet(c4, result);
    if (rc1 == RESULT_CODE::SUCCESS && !result)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isSubSet(nullptr)";
    rc1 = c6->isSubSet(nullptr, result);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isSubSet(wrong dim)";
    rc1 = c6->isSubSet(c1, result);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "isIntersects(+)";
    rc1 = c6->isIntersects(c7, result);
    if (rc1 == RESULT_CODE::SUCCESS && result)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isIntersects(-)";
    rc1 = c6->isIntersects(c4, result);
    if (rc1 == RESULT_CODE::SUCCESS && !result)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isIntersects(nullptr)";
    rc1 = c6->isIntersects(nullptr, result);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "isIntersects(wrong dim)";
    rc1 = c6->isIntersects(c1, result);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;



    std::cout << "clone(+)";
    ICompact* c_cl = c6->clone();
    if (c_cl)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete c1;
    delete c2;
    delete c4;
    delete c6;
    delete c7;
    delete c_cl;

    delete vec1;
    delete vec2;
    delete vec3;
    delete vec4;
    delete vec5;
    delete vec6;
    delete vec7;
    delete vec8;

    logger->destroyLogger(nullptr);
}

int main()
{
    test();
    return 0;
}
