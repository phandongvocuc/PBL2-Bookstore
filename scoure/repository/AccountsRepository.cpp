#include "repository/AccountsRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

AccountsRepository::AccountsRepository(const custom::CustomString &dataDir)
    : dataPath(core::path::join(dataDir, custom::CustomStringLiteral("users.bin"))) {}

custom::DynamicArray<model::Account> AccountsRepository::loadAll() const {
    return serialization::BinaryFileStore::readAccounts(dataPath);
}

void AccountsRepository::saveAll(const custom::DynamicArray<model::Account> &accounts) const {
    serialization::BinaryFileStore::writeAccounts(accounts, dataPath);
}

custom::CustomString AccountsRepository::hashPassword(const custom::CustomString &plainText) {
    // Simple password storage - just return plaintext
    return plainText;
}

}  // namespace pbl2::repository
