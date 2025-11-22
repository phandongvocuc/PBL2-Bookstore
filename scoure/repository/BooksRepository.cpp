#include "repository/BooksRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

BooksRepository::BooksRepository(const core::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, core::CustomStringLiteral("books.bin"))) {}

void BooksRepository::saveAll(const core::DynamicArray<model::Book> &books) const {
	serialization::BinaryFileStore::writeBooks(books, dataPath);
}

core::DynamicArray<model::Book> BooksRepository::loadAll() const {
	return serialization::BinaryFileStore::readBooks(dataPath);
}

}  // namespace pbl2::repository
