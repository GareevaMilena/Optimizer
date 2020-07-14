#include "ICompact.h"
#include "IVector.h"
#include<string.h>
#include<new>
#include<algorithm>
#include <vector>

#define eps 1e-3

namespace {

    class MiCompact : public ICompact {

    private:

        ILogger* logger;

        IVector* begin_vec;
        IVector* end_vec;

    public:
        
        MiCompact(IVector* begin, IVector* end, ILogger* logger) :
            begin_vec(begin),
            end_vec(end),
            logger(logger)
        {};

        static ICompact* createCompact(IVector const* const begin, IVector const* const end, ILogger* logger);

        IVector* getBegin() const;
        IVector* getEnd() const;

        iterator* end(IVector const* const step = 0);
        iterator* begin(IVector const* const step = 0);

        RESULT_CODE isContains(IVector const* const vec, bool& result) const;
        RESULT_CODE isSubSet(ICompact const* const other, bool& result) const;
        RESULT_CODE isIntersects(ICompact const* const other, bool& result) const;


        size_t getDim() const;
        ICompact* clone() const;

        ~MiCompact()
        {
            delete begin_vec;
            delete end_vec;
        }

        class MiIterator : public iterator
        {

        public:
            //adds step to current value in iterator
            //+step

            MiIterator(IVector* start, IVector* finish, IVector* step) :
                currentPoint(start->clone()),
                start(start),
                finish(finish),
                step(step),
                dir(nullptr)
            {};

            RESULT_CODE doStep();

            IVector* getPoint() const;

            //change order of step
            RESULT_CODE setDirection(IVector const* const dir);

            ~MiIterator()
            {
                delete currentPoint;
                delete start;
                delete finish;
                delete dir;
                delete step;
            }

        private:

            IVector* currentPoint;
            IVector* start;
            IVector* finish;
            IVector* step;
            IVector* dir;
        };
    };
}

RESULT_CODE MiCompact::MiIterator::doStep()
{
    bool res;
    if (this->step->getCoord(0) > 0)
        res = true;

    size_t i = 0;

    for (i = 0; i < this->start->getDim(); ++i) {
        size_t dim = i;

        if (this->dir)
            dim = this->dir->getCoord(i);

        if (res && this->currentPoint->getCoord(dim) + this->step->getCoord(dim) <= this->finish->getCoord(dim)) {
            this->currentPoint->setCoord(dim, this->currentPoint->getCoord(dim) + this->step->getCoord(dim));
            break;
        }

        else if (!res && this->currentPoint->getCoord(dim) + this->step->getCoord(dim) >= this->finish->getCoord(dim)) {
            this->currentPoint->setCoord(dim, this->currentPoint->getCoord(dim) + this->step->getCoord(dim));
            break;
        }

        this->currentPoint->setCoord(dim, this->start->getCoord(dim));
    }

    if (i == this->start->getDim())
        return RESULT_CODE::OUT_OF_BOUNDS;

    return RESULT_CODE::SUCCESS;
}

IVector* MiCompact::MiIterator::getPoint() const
{
    return currentPoint->clone();
}

RESULT_CODE MiCompact::MiIterator::setDirection(IVector const* const dir)
{
    if (!dir)
        return RESULT_CODE::NAN_VALUE;

    if (dir->getDim() != this->start->getDim())
        return RESULT_CODE::WRONG_DIM;
    delete currentPoint;
    this->currentPoint = this->start->clone();
    std::vector <double> a;
    for (size_t i = 0; i < dir->getDim(); i++)
        a.push_back(dir->getCoord(i));

    sort(a.begin(), a.end());

    if (a.front() != 0 || a.back() != this->start->getDim() - 1)
        return RESULT_CODE::WRONG_ARGUMENT;

    for (size_t i = 0; i < dir->getDim() - 1; i++) {
        if (a.at(i) == a.at(i + 1))
            return RESULT_CODE::WRONG_ARGUMENT;
    }

    this->dir = dir->clone();
    return RESULT_CODE::SUCCESS;
}

    ICompact* ICompact::createCompact(IVector const* const begin, IVector const* const end, ILogger* logger)
    {
        return MiCompact::createCompact(begin, end, logger);
    }

    ICompact* MiCompact::createCompact(IVector const* const begin, IVector const* const end, ILogger* logger)
    {
        if (!begin || !end) {
            if (logger)
                logger->log("COMPACT: need to set begin/end", RESULT_CODE::WRONG_ARGUMENT);
            return nullptr;
        }

        if (begin->getDim() != end->getDim()) {
            if (logger)
                logger->log("COMPACT: begin and end must have the same dimension", RESULT_CODE::WRONG_DIM);
            return nullptr;
        }

        for (size_t i = 0; i < begin->getDim(); ++i) {
            if (begin->getCoord(i) > end->getCoord(i)) {
                if (logger)
                    logger->log("COMPACT: start values ​​must be less than end", RESULT_CODE::WRONG_ARGUMENT);
                return nullptr;
            }

        }

        IVector* b = begin->clone();
        IVector* e = end->clone();

        if (!b)
        {
            if (logger)
                logger->log("COMPACT: begin-end vectors not created", RESULT_CODE::OUT_OF_MEMORY);
            if (e)
                delete e;
            return nullptr;
        }
        if (!e)
        {
            if (logger)
                logger->log("COMPACT: begin-end vectors not created", RESULT_CODE::OUT_OF_MEMORY);
            delete b;
            return nullptr;
        }
        return new MiCompact(b, e, logger);
    }

    ICompact* ICompact::intersection(ICompact const* const left, ICompact const* const right, ILogger* logger)
    {
        if (!left || !right) {
            if (logger)
                logger->log("COMPACT: no compacts are defined for the intersection", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }

        if (left->getDim() != right->getDim()) {
            if (logger)
                logger->log("COMPACT: compacts must have the same dimension", RESULT_CODE::WRONG_DIM);
            return nullptr;
        }

        IVector* b1 = left->getBegin();
        IVector* b2 = right->getBegin();
        IVector* e1 = left->getEnd();
        IVector* e2 = right->getEnd();

        double MAX, MIN;
        for (size_t i = 0; i < left->getDim(); i++) {
            MAX = std::max(b1->getCoord(i), b2->getCoord(i));
            MIN = std::min(e1->getCoord(i), e2->getCoord(i));
            if (MAX > MIN) {
                if (logger)
                    logger->log("COMPACT: intersection is empty", RESULT_CODE::WRONG_ARGUMENT);
                delete b1;
                delete b2;
                delete e1;
                delete e2;
                return nullptr;
            }

            b1->setCoord(i, MAX);
            e1->setCoord(i, MIN);
        }

        delete b2;
        delete e2;

        return new MiCompact(b1, e1, logger);
    }

    ICompact* ICompact::add(ICompact const* const left, ICompact const* const right, ILogger* logger)
    {
        if (!left || !right) {
            if (logger)
                logger->log("COMPACT: no compacts are defined for the union", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }

        if (left->getDim() != right->getDim()) {
            if (logger)
                logger->log("COMPACT: compacts must have the same dimension", RESULT_CODE::WRONG_DIM);
            return nullptr;
        }

        bool result = false;
        left->isIntersects(right, result);
        if (result == false)
        {
            if (logger)
                logger->log("COMPACT: impossible to union compacts", RESULT_CODE::WRONG_ARGUMENT);
            return nullptr;
        }

        left->isSubSet(right, result);
        if (result == true)
            return left->clone();

        right->isSubSet(left, result);
        if (result == true)
            return right->clone();

        IVector* b1 = left->getBegin();
        IVector* b2 = right->getBegin();
        IVector* e1 = left->getEnd();
        IVector* e2 = right->getEnd();

        size_t count = 0;
        for (size_t i = 0; i < left->getDim(); ++i) {
            if (fabs(b1->getCoord(i) - b2->getCoord(i)) < eps && fabs(e1->getCoord(i) - e2->getCoord(i)) < eps)
                count++;
        }

        delete b1;
        delete e1;
        delete b2;
        delete e2;

        if (count < left->getDim() - 1) {
            if (logger)
                logger->log("COMPACT: impossible to union compacts", RESULT_CODE::WRONG_ARGUMENT);
            return nullptr;
        }

        return makeConvex(left, right, logger);
    }

    ICompact* ICompact::makeConvex(ICompact const* const left, ICompact const* const right, ILogger* logger)
    {
        if (!left || !right) {
            if (logger)
                logger->log("COMPACT: no compacts are defined for the intersection", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }

        if (left->getDim() != right->getDim()) {
            if (logger)
                logger->log("COMPACT: compacts must have the same dimension", RESULT_CODE::WRONG_DIM);
            return nullptr;
        }

        IVector* b1 = left->getBegin();
        IVector* b2 = right->getBegin();
        IVector* e1 = left->getEnd();
        IVector* e2 = right->getEnd();

        double MAX, MIN;

        for (size_t i = 0; i < left->getDim(); i++) {
            MIN = std::min(b1->getCoord(i), b2->getCoord(i));
            MAX = std::max(e1->getCoord(i), e2->getCoord(i));

            b1->setCoord(i, MIN);
            e1->setCoord(i, MAX);
        }

        delete b2;
        delete e2;

        return new MiCompact(b1, e1, logger);
    }

    IVector* MiCompact::getBegin() const
    {
        return this->begin_vec->clone();
    }

    IVector* MiCompact::getEnd() const
    {
        return this->end_vec->clone();
    }

    MiCompact::iterator* MiCompact::end(IVector const * const step)
    {
        if (!step)
        {
            if (logger)
                this->logger->log("COMPACT: need to set step", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }

        size_t count = 0;

        for (size_t i = 0; i < step->getDim(); i++) {
            if (step->getCoord(i) < 0)
                count++;
        }

        if (count != step->getDim()) {
            if (logger)
                this->logger->log("COMPACT: wrong step", RESULT_CODE::WRONG_ARGUMENT);
            return nullptr;
        }

        return new MiIterator(this->getEnd(), this->getBegin(), step->clone());
    }

    MiCompact::iterator* MiCompact::begin(IVector const * const step)
    {
        if (!step)
        {
            if (logger)
                this->logger->log("COMPACT: need to set step", RESULT_CODE::NAN_VALUE);
            return nullptr;
        }

        size_t count = 0;

        for (size_t i = 0; i < step->getDim(); i++) {
            if (step->getCoord(i) > 0)
                count++;
        }

        if (count != step->getDim()) {
            if (logger)
                this->logger->log("COMPACT: wrong step", RESULT_CODE::WRONG_ARGUMENT);
            return nullptr;
        }

        return new MiIterator(this->getBegin(), this->getEnd(), step->clone());
    }

    RESULT_CODE MiCompact::isContains(IVector const * const vec, bool & result) const
    {
        if (!vec) {
            if (logger)
                logger->log("COMPACT: need to set vector", RESULT_CODE::NAN_VALUE);
            return RESULT_CODE::NAN_VALUE;
        }

        if (this->begin_vec->getDim() != vec->getDim()) {
            result = false;
            return RESULT_CODE::WRONG_DIM;
        }

        for (size_t i = 0; i < vec->getDim(); i++) {
            if (vec->getCoord(i) < this->begin_vec->getCoord(i) ||  vec->getCoord(i) > this->end_vec->getCoord(i)) {
                result = false;
                return RESULT_CODE::SUCCESS;
            }
        }

        result = true;
        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE MiCompact::isSubSet(ICompact const * const other, bool & result) const
    {
        if (!other) {
            if (logger)
                logger->log("COMPACT: need to set compact", RESULT_CODE::NAN_VALUE);
            return RESULT_CODE::NAN_VALUE;
        }

        if (other->getDim() != this->getDim()) {
            result = false;
            return RESULT_CODE::WRONG_DIM;
        }
        
        IVector* b = other->getBegin();
        IVector* e = other->getEnd();

        for (size_t i = 0; i < this->getDim(); i++) {
            //other включен в this
            if (this->begin_vec->getCoord(i) > b->getCoord(i) || this->end_vec->getCoord(i) < e->getCoord(i)) {

                delete b;
                delete e;

                result = false;
                return RESULT_CODE::SUCCESS;
            }
        }

        delete b;
        delete e;

        result = true;
        return RESULT_CODE::SUCCESS;
    }

    RESULT_CODE MiCompact::isIntersects(ICompact const * const other, bool & result) const
    {
        if (!other) {
            if (logger)
                logger->log("COMPACT: need to set compact", RESULT_CODE::NAN_VALUE);
            return RESULT_CODE::NAN_VALUE;
        }

        if (other->getDim() != this->getDim()) {
            result = false;
            return RESULT_CODE::WRONG_DIM;
        }

        IVector* b2 = other->getBegin();
        IVector* e2 = other->getEnd();

        double MIN, MAX;

        for (size_t i = 0; i < this->getDim(); i++) {
            MIN = std::min(this->end_vec->getCoord(i), e2->getCoord(i));
            MAX = std::max(this->begin_vec->getCoord(i), b2->getCoord(i));

            if (MAX > MIN) {

                delete b2;
                delete e2;

                result = false;
                return RESULT_CODE::SUCCESS;
            }
        }

        delete b2;
        delete e2;

        result = true;
        return RESULT_CODE::SUCCESS;
    }

    size_t MiCompact::getDim() const
    {
        return this->begin_vec->getDim();
    }

    ICompact* MiCompact::clone() const
    {
        return new MiCompact(this->begin_vec->clone(), this->end_vec->clone(), this->logger);
    }

    ICompact::~ICompact() {};