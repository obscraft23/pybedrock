#include "leveldb/db.h"

#include <stdio.h>
#include <cassert>
#include <iostream>

using namespace std;
//using namespace leveldb

int main(){
    cout << "Hello world." << endl;

    leveldb::DB* db;
    leveldb::Options options_;

    options_.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options_, "../../TbLUY1uSXQA=/db", &db);
    assert(status.ok());

    return 0;
};