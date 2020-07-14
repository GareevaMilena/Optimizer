#include <vector>
#include"ISet.h"
#include"ILogger.h"


namespace {
	class MiSet : public ISet {
	private:
		ILogger *logger;
		std::vector<IVector*> vectors;
	public:

		MiSet(ILogger *pLogger);
		~MiSet() {
			for (size_t i = 0; i < vectors.size(); i++)
			{
				delete vectors[i];
			}
			vectors.clear();
		};

		RESULT_CODE insert(const IVector* pVector, IVector::NORM norm, double tolerance);
		RESULT_CODE get(IVector*& pVector, size_t index)const;
		RESULT_CODE get(IVector*& pVector, IVector const* pSample, IVector::NORM norm, double tolerance)const;
		size_t getDim() const; //space dimension
		size_t getSize() const; //num elements in set
		void clear(); // delete all
		RESULT_CODE erase(size_t index);
		RESULT_CODE erase(IVector const* pSample, IVector::NORM norm, double tolerance);
		MiSet* clone()const;

	};
}

ISet *ISet::createSet(ILogger* pLogger)
{
	return new MiSet(pLogger);
}

ISet* ISet::intersect(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger)
{
	if (!pOperand1 || !pOperand2){
		if (pLogger)
			pLogger->log("SET: enter pOperand1 and pOperand2", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	if (pOperand1->getDim() != pOperand2->getDim()){
		if (pLogger)
			pLogger->log("SET: perands must be of the same dimension", RESULT_CODE::WRONG_DIM);
		return nullptr;
	}
	if (isnan(tolerance) || tolerance < 0){
		if (pLogger)
			pLogger->log("SET: NAN", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	ISet *intersection = ISet::createSet(pLogger);
	IVector *v1, *v2, *difference;
	for (size_t i = 0; i < pOperand1->getSize(); ++i)
	{
		for (size_t j = 0; j < pOperand2->getSize(); ++j)
		{
			pOperand1->get(v1, i);
			pOperand2->get(v2, j);
			difference = IVector::sub(v1, v2, pLogger);
			if (!difference)
				return nullptr;
			if (difference->norm(norm) < tolerance)
				intersection->insert(v1, norm, tolerance);
			delete v1;
			delete v2;
			delete difference;
		}
	}
	return intersection;
}

ISet* ISet::add(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger)
{
	if (!pOperand1 || !pOperand2){
		if (pLogger)
			pLogger->log("SET: enter pOperand1 and pOperand2", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	if (pOperand1->getDim() != pOperand2->getDim()){
		if (pLogger)
			pLogger->log("SET: perands must be of the same dimension", RESULT_CODE::WRONG_DIM);
		return nullptr;
	}
	if (isnan(tolerance) || tolerance < 0){
		if (pLogger)
			pLogger->log("SET: NAN", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	ISet *Union = createSet(pLogger);
	int countOfOperands = 2;
	std::vector<ISet const*> operands;
	operands.push_back(pOperand1);
	operands.push_back(pOperand2);
	IVector *v;
	for (int j = 0; j < countOfOperands; j++)
	{
		for (int i = 0; i < operands[j]->getSize(); ++i)
		{
			operands[j]->get(v, i);
			Union->insert(v, norm, tolerance);
			delete v;
		}
	}
	return Union;
}


ISet* ISet::sub(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger)
{
	if (!pOperand1 || !pOperand2)
	{
		if (pLogger)
			pLogger->log("SET: enter pOperand1 and pOperand2", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	if (pOperand1->getDim() != pOperand2->getDim()) {
		if (pLogger)
			pLogger->log("SET: perands must be of the same dimension", RESULT_CODE::WRONG_DIM);
		return nullptr;
	}
	if (isnan(tolerance) || tolerance < 0) {
		if (pLogger)
			pLogger->log("SET: NAN", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	ISet *difference = ISet::createSet(pLogger);
	IVector *v;
	for (size_t i = 0; i < pOperand1->getSize(); ++i) 
	{
		pOperand1->get(v, i);
		RESULT_CODE rc = pOperand2->get(v, v, norm, tolerance);

		if (rc == RESULT_CODE::NOT_FOUND) {
			difference->insert(v, norm, tolerance);
		}
		delete v;
	}
	return difference;
}

ISet* ISet::symSub(ISet const* pOperand1, ISet const* pOperand2, IVector::NORM norm, double tolerance, ILogger* pLogger)
{
	if (!pOperand1 || !pOperand2)
	{
		if (pLogger)
			pLogger->log("SET: enter pOperand1 and pOperand2", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	if (pOperand1->getDim() != pOperand2->getDim()) {
		if (pLogger)
			pLogger->log("SET: perands must be of the same dimension", RESULT_CODE::WRONG_DIM);
		return nullptr;
	}
	if (isnan(tolerance) || tolerance < 0) {
		if (pLogger)
			pLogger->log("SET: NAN", RESULT_CODE::NAN_VALUE);
		return nullptr;
	}
	ISet *a = ISet::add(pOperand1, pOperand2, norm, tolerance, pLogger);
	ISet *i = ISet::intersect(pOperand1, pOperand2, norm, tolerance, pLogger);
	if (!a || !i) {
		if (pLogger)
			pLogger->log("SET: impossible to make symmetric difference", RESULT_CODE::CALCULATION_ERROR);
		return nullptr;
	}
	ISet *s = ISet::sub(a, i, norm, tolerance, pLogger);
	delete a;
	delete i;
	return s;
}

MiSet::MiSet(ILogger *pLogger) :
	logger(pLogger)
{}

RESULT_CODE MiSet::get(IVector*& pVector, IVector const* pSample, IVector::NORM norm, double tolerance) const
{
	if (!pSample)
		return RESULT_CODE::NAN_VALUE;
	if (isnan(tolerance) || tolerance < 0)
		return RESULT_CODE::NAN_VALUE;
	for (int i = 0; i < vectors.size(); ++i) {
		IVector *difference = IVector::sub(pSample, vectors[i], logger);
		if (!difference)
			return RESULT_CODE::CALCULATION_ERROR;
		double n = difference->norm(norm);
		delete difference;
		if (n < tolerance)
		{
			pVector = vectors[i]->clone();
			return RESULT_CODE::SUCCESS;
		}
	}
	return RESULT_CODE::NOT_FOUND;
}

RESULT_CODE MiSet::insert(const IVector* pVector, IVector::NORM norm, double tolerance)
{
	if (!pVector)
		return RESULT_CODE::NAN_VALUE;
	if (isnan(tolerance) || tolerance < 0)
		return RESULT_CODE::NAN_VALUE;
	IVector *vec = nullptr;
	if (get(vec, pVector, norm, tolerance) == RESULT_CODE::NOT_FOUND) 
	{
		if (vectors.size() == 0 || (vectors.size() > 0 && vectors[0]->getDim() == pVector->getDim())) 
		{
			vectors.push_back(pVector->clone());
			return RESULT_CODE::SUCCESS;
		}
		else
		{
			logger->log("SET: This element is not added to the set because it has a different dimension.", RESULT_CODE::WRONG_DIM);
			return RESULT_CODE::WRONG_DIM;
		}
	}
	else
	{
		logger->log("SET: This element is not added to the set since one already exists in it", RESULT_CODE::MULTIPLE_DEFINITION);
		delete vec;
		return RESULT_CODE::MULTIPLE_DEFINITION;
	}
}

RESULT_CODE MiSet::get(IVector*& pVector, size_t index) const
{
	if (index < vectors.size()) 
	{
		pVector = vectors[index]->clone();
		return RESULT_CODE::SUCCESS;
	}
	else
	{
		logger->log("SET: It is impossible to get an element with such an id", RESULT_CODE::OUT_OF_BOUNDS);
		pVector = nullptr;
		return RESULT_CODE::OUT_OF_BOUNDS;
	}
}

size_t MiSet::getDim() const
{
	if (vectors.size() != 0) {
		return vectors[0]->getDim();
	}
	return 0;
}

size_t MiSet::getSize() const
{
	return vectors.size();
}

void MiSet::clear()
{
	for (size_t i = 0; i < this->getSize(); i++)
	{
		delete vectors[i];
	}
	vectors.clear();
}

RESULT_CODE MiSet::erase(size_t index)
{
	if (index < vectors.size()) 
	{
		delete vectors[index];
		vectors.erase(vectors.begin() + index);
		return RESULT_CODE::SUCCESS;
	}
	else
	{
		logger->log("SET: It is impossible to delete this element", RESULT_CODE::OUT_OF_BOUNDS);
		return RESULT_CODE::OUT_OF_BOUNDS;
	}
	
}

RESULT_CODE MiSet::erase(IVector const* pSample, IVector::NORM norm, double tolerance)
{
	if (!pSample)
		return RESULT_CODE::NAN_VALUE;
	if (isnan(tolerance) || tolerance < 0)
		return RESULT_CODE::NAN_VALUE;
	for (int i = 0; i < vectors.size(); ++i) 
	{
		IVector *difference = IVector::sub(pSample, vectors[i], logger);
		if (!difference)
			return RESULT_CODE::CALCULATION_ERROR;
		double n = difference->norm(norm);
		delete difference;
		if (n < tolerance)
		{
			delete vectors[i];
			vectors.erase(vectors.begin() + i);
			return RESULT_CODE::SUCCESS;
		}
	}
	logger->log("SET: This element not found in set", RESULT_CODE::NOT_FOUND);
	return RESULT_CODE::NOT_FOUND;
}

MiSet* MiSet::clone() const
{
	MiSet *set = new(std::nothrow) MiSet(this->logger);
	set->vectors.reserve(vectors.size());
	for (int i = 0; i < vectors.size(); ++i)
		set->vectors.push_back(vectors[i]->clone());
	return set;
}

ISet::~ISet() {}
