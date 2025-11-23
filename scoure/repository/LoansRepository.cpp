#include "repository/LoansRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

LoansRepository::LoansRepository(const core::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, core::CustomStringLiteral("loans.bin"))) {}

core::DynamicArray<model::Loan> LoansRepository::loadAll() const {
	return serialization::BinaryFileStore::readLoans(dataPath);
}

void LoansRepository::saveAll(const core::DynamicArray<model::Loan> &loans) const {
	serialization::BinaryFileStore::writeLoans(loans, dataPath);
}

}  // namespace pbl2::repository
