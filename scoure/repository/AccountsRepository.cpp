#include "repository/AccountsRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

AccountsRepository::AccountsRepository(const core::CustomString &dataDir)
    : dataPath(core::path::join(dataDir, core::CustomStringLiteral("users.bin"))) {}

core::DynamicArray<model::Account> AccountsRepository::loadAll() const {
    return serialization::BinaryFileStore::readAccounts(dataPath);
}

void AccountsRepository::saveAll(const core::DynamicArray<model::Account> &accounts) const {
    serialization::BinaryFileStore::writeAccounts(accounts, dataPath);
}

core::CustomString AccountsRepository::hashPassword(const core::CustomString &plainText) {
    // Simple password storage - just return plaintext
    return plainText;
}

}  // namespace pbl2::repository
