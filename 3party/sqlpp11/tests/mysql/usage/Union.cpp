/*
 * Copyright (c) 2013 - 2016, Roland Bock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "make_test_connection.h"
#include "TabSample.h"
#include <sqlpp11/mysql/connection.h>
#include <sqlpp11/sqlpp11.h>

#include <iostream>

const auto library_raii = sqlpp::mysql::scoped_library_initializer_t{0, nullptr, nullptr};

namespace sql = sqlpp::mysql;
const auto tab = TabSample{};

int Union(int, char*[])
{
  sql::global_library_init();
  try
  {
    auto db = sql::make_test_connection();
    db.execute(R"(DROP TABLE IF EXISTS tab_sample)");
    db.execute(R"(CREATE TABLE tab_sample (
		alpha bigint(20) AUTO_INCREMENT,
			beta bool DEFAULT NULL,
			gamma varchar(255) DEFAULT NULL,
			PRIMARY KEY (alpha)
			))");

    auto u = select(all_of(tab)).from(tab).unconditionally().union_all(select(all_of(tab)).from(tab).unconditionally());

    for (const auto& row : db(u))
    {
      std::cout << row.alpha << row.beta << row.gamma << std::endl;
    }

    for (const auto& row : db(u.union_distinct(select(all_of(tab)).from(tab).unconditionally())))
    {
      std::cout << row.alpha << row.beta << row.gamma << std::endl;
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
