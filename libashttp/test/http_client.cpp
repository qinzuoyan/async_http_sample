/*
 * Copyright © 2014-2015, Tolga HOŞGÖR.
 *
 * File created on: 28.01.2015
*/

/*
  This file is part of libashttp.

  libashttp is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  libashttp is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with libashttp.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../ashttp/header.hpp"
#include "../ashttp/client/client.hpp"
#include "../ashttp/client/request.hpp"
#include "../ashttp/type.hpp"

//#include <boost/log/trivial.hpp>
//#include <boost/log/expressions.hpp>

#include <iostream>
#include <thread>

using namespace ashttp;
using namespace ashttp::client;

ashttp::asio::io_service* pIoService;

void sigintHandler(int);

int main(int argc, char* argv[]) {
  //boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::severity_level::trace);

  ashttp::asio::io_service ioService;
  pIoService = &ioService;

  std::signal(SIGTERM, sigintHandler);

  {
    const auto client = ClientHTTPS::create("www.baidu.com", ioService);

    client->onConnect([](const ErrorCode& ec) {
      std::cerr << "\tclient onconnect " << ec << std::endl;
    });

    {
      auto request = client->get("/");

      request->onBodyChunk([](const ErrorCode& ec, std::istream& is, std::size_t chunkSize) {
        std::cerr << "\trequest body chunk; size: " << chunkSize << std::endl;
        std::istreambuf_iterator<char> eof;
        std::string str(std::istreambuf_iterator<char>(is), eof);
        std::cerr << str << std::endl;

        is.ignore(chunkSize);
      })
      .onComplete([client, request](const ErrorCode& ec) {
        std::cerr << "\trequest oncomplete" << ec << std::endl;
      });

      client->schedule(request);
    }
  }

  std::thread t{[&ioService]() { ioService.run(); }};

  std::cerr << "\tJoining io_service thread." << std::endl;

  t.join();

  std::cerr << "\tJoined io_service thread." << std::endl;

  return 0;
}

void sigintHandler(int) {
  std::cerr << "\tClose requested." << std::endl;

  pIoService->stop();
}
