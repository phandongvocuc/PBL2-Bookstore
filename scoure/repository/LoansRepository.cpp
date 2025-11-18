#include "repository/LoansRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

LoansRepository::LoansRepository(const custom::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, custom::CustomStringLiteral("loans.bin"))) {}

custom::DynamicArray<model::Loan> LoansRepository::loadAll() const {
	return serialization::BinaryFileStore::readLoans(dataPath);
}

void LoansRepository::saveAll(const custom::DynamicArray<model::Loan> &loans) const {
	serialization::BinaryFileStore::writeLoans(loans, dataPath);
}

}  // namespace pbl2::repository
