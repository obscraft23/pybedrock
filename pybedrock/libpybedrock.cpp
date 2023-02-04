#include "leveldb/db.h"

#include <stdio.h>
#include <cassert>

leveldb::DB* db;
leveldb::Options options;
options.create_if_missing = true;
leveldb::Status status = leveldb::DB::Open(options, "../../TbLUY1uSXQA=/db", &db);
assert(status.ok());