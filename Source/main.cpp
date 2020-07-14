#include <iostream>
#include <math.h>
#include "ISet.h"



void test() {
    std::cout << "Set test, good luck!" << std::endl;

    const size_t dim2 = 2;
    const size_t dim3 = 3;

    double a1[dim3], a3[dim3];

    for (size_t i = 0; i < dim3; ++i) {
        a1[i] = i * 2;
        a3[i] = i * 3;
    }

    double a2[dim2] = { 1, 2 };

    ILogger* logger = ILogger::createLogger(nullptr);
    if (!logger) {
        std::cout << "SET: not enough memory";
        return;
    }
    logger->setLogFile("setTestingLogger.txt");

    double tolerance = 1e-3;

    RESULT_CODE rc1, rc2, rc3 = RESULT_CODE::SUCCESS;

    IVector* vec1 = IVector::createVector(dim3, a1, logger);

    IVector* vec2 = IVector::createVector(dim2, a2, logger);

    IVector* vec3 = IVector::createVector(dim3, a3, logger);


    //nullptr
    IVector* vec4 = nullptr;

    ISet* s1 = ISet::createSet(logger);
    rc1 = s1->insert(vec1, IVector::NORM::NORM_1, tolerance);
    ISet* s3 = ISet::createSet(logger);
    rc2 = s3->insert(vec3, IVector::NORM::NORM_1, tolerance);

    ISet* s2 = ISet::createSet(logger);
    rc3 = s2->insert(vec2, IVector::NORM::NORM_1, tolerance);


    std::cout << "insert(+)";
    if (rc1 == RESULT_CODE::SUCCESS && rc2 == RESULT_CODE::SUCCESS && rc3 == RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "insert(wrong dim)";
    rc3 = s3->insert(vec2, IVector::NORM::NORM_1, tolerance);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "insert(nullptr)";
    rc3 = s3->insert(nullptr, IVector::NORM::NORM_1, tolerance);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "insert(tolerance = NAN)";
    rc3 = s3->insert(vec1, IVector::NORM::NORM_1, NAN);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "insert(tolerance < 0)";
    rc3 = s3->insert(vec1, IVector::NORM::NORM_1, -tolerance);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "get(index)(+)";
    rc1 = s1->get(vec4, s1->getSize() - 1);
    if (rc1 == RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(index)(-)";
    rc1 = s1->get(vec4, s1->getSize());
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(+)";
    rc1 = s1->get(vec4, vec1, IVector::NORM::NORM_1, tolerance);
    if (rc1 == RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(-)";
    rc1 = s1->get(vec4, vec3, IVector::NORM::NORM_1, tolerance);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(nullptr)";
    rc1 = s1->get(vec4, nullptr, IVector::NORM::NORM_1, tolerance);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(wrong dim)";
    rc1 = s1->get(vec4, vec2, IVector::NORM::NORM_1, tolerance);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(tolerance = NAN)";
    rc1 = s1->get(vec4, vec1, IVector::NORM::NORM_1, NAN);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "get(tolerance < 0)";
    rc1 = s1->get(vec4, vec1, IVector::NORM::NORM_1, -tolerance);
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   " << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "erase(index)(+)";
    rc1 = s1->erase(s1->getSize() - 1);
    if (rc1 == RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    rc1 = s1->insert(vec1, IVector::NORM::NORM_1, tolerance);

    std::cout << "erase(index)(-)";
    rc1 = s1->erase(s1->getSize());
    if (rc1 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "erase(+)";
    rc3 = s3->erase(vec3, IVector::NORM::NORM_1, tolerance);
    if (rc3 == RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    s3->insert(vec3, IVector::NORM::NORM_1, tolerance);

    std::cout << "erase(-)";
    rc3 = s3->erase(vec1, IVector::NORM::NORM_1, tolerance);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "erase(wrong dim)";
    rc3 = s3->erase(vec2, IVector::NORM::NORM_1, tolerance);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "erase(tolerance = NAN)";
    rc3 = s3->erase(vec3, IVector::NORM::NORM_1, NAN);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "erase(tolerance < 0)";
    rc3 = s3->erase(vec3, IVector::NORM::NORM_1, -tolerance);
    if (rc3 != RESULT_CODE::SUCCESS)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "clone(+)";
    ISet* s = s1->clone();
    if (s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete s;

    std::cout << "add(+)";
    s = ISet::add(s1, s3, IVector::NORM::NORM_1, tolerance, logger);
    if (s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete s;

    std::cout << "add(wrong dim)";
    s = ISet::add(s1, s2, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(nullptr)";
    s = ISet::add(s1, nullptr, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(tolerance = NAN)";
    s = ISet::add(s1, s3, IVector::NORM::NORM_1, NAN, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "add(tolerance < 0)";
    s = ISet::add(s1, s3, IVector::NORM::NORM_1, -tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "sub(+)";
    s = ISet::sub(s1, s3, IVector::NORM::NORM_1, tolerance, logger);
    if (s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete s;

    std::cout << "sub(wrong dim)";
    s = ISet::sub(s1, s2, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "sub(nullptr)";
    s = ISet::sub(s1, nullptr, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "sub(tolerance = NAN)";
    s = ISet::sub(s1, s3, IVector::NORM::NORM_1, NAN, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "sub(tolerance < 0)";
    s = ISet::sub(s1, s3, IVector::NORM::NORM_1, -tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "symSub(wrong dim)";
    s = ISet::symSub(s1, s2, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "symSub(nullptr)";
    s = ISet::symSub(s1, nullptr, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "symSub(tolerance = NAN)";
    s = ISet::symSub(s1, s3, IVector::NORM::NORM_1, NAN, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "symSub(tolerance < 0)";
    s = ISet::symSub(s1, s3, IVector::NORM::NORM_1, -tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;


    std::cout << "intersect(+)";
    s = ISet::intersect(s1, s1, IVector::NORM::NORM_1, tolerance, logger);
    if (s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    delete s;

    std::cout << "intersect(wrong dim)";
    s = ISet::intersect(s1, s2, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "intersect(nullptr)";
    s = ISet::intersect(s1, nullptr, IVector::NORM::NORM_1, tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "intersect(tolerance = NAN)";
    s = ISet::symSub(s1, s3, IVector::NORM::NORM_1, NAN, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;

    std::cout << "intersect(tolerance < 0)";
    s = ISet::symSub(s1, s3, IVector::NORM::NORM_1, -tolerance, logger);
    if (!s)
        std::cout << "   Good job!" << std::endl;
    else
        std::cout << "   You are loser!" << std::endl;



    delete s1;
    delete s2;
    delete s3;
    delete vec1;
    delete vec2;
    delete vec3;

    logger->destroyLogger(nullptr);
}

int main()
{
    test();
    return 0;
}
