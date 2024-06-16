//
//  client.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//

//#include <httplib.h>
#include "httplib.h"
#include <iostream>

#define CA_CERT_FILE "../certificates/rootCA.crt"

using namespace std;

int main(void) {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  httplib::SSLClient cli("localhost", 8585);
  cli.set_ca_cert_path(CA_CERT_FILE);
  cli.enable_server_certificate_verification(false);
  //cli.enable_server_certificate_verification(true);
#else
  httplib::Client cli("localhost", 8080);
#endif

  if (auto res = cli.Get("/hi")) {
    cout << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  } else {
    cout << "error code: " << res.error() << std::endl;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto result = cli.get_openssl_verify_result();
    if (result) {
      cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
    }
#endif
  }

  // Send a simple POST
  string body("body");
  if (auto res = cli.Post("/post", body, "application/octet-stream")) {
    cout << res->status << endl;
    cout << res->body << endl;
  }

  return 0;
}
