#include "repository/BooksRepository.h"

#include "datafile/BinaryFileStore.h"
#include "core/PathUtils.h"

using namespace std;

namespace pbl2::repository {

BooksRepository::BooksRepository(const custom::CustomString &dataDir)
	: dataPath(core::path::join(dataDir, custom::CustomStringLiteral("books.bin"))) {}

void BooksRepository::saveAll(const custom::DynamicArray<model::Book> &books) const {
	serialization::BinaryFileStore::writeBooks(books, dataPath);
}

custom::DynamicArray<model::Book> BooksRepository::loadAll() const {
	return serialization::BinaryFileStore::readBooks(dataPath);
}

}  // namespace pbl2::repository
