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

  // Example for crafting headers
  httplib::Headers headers = {
    {"Content-Type", "application/octet-stream"}
  };

  // Send HEAD
  if (auto res = cli.Head("/head", headers)) {
    cout << "HEAD Response: " << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  }

  if (auto res = cli.Get("/get")) {
    cout << "GET Response: " << res->status << endl;
    cout << "GET Content-Type: " << res->get_header_value("Content-Type") << endl;
    cout << "GET Content-Length: " << res->get_header_value("Content-Length") << endl;
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
    cout << "POST Response: " << res->status << endl;
    cout << "POST Content-Type: " << res->get_header_value("Content-Type") << endl;
    cout << "POST Content-Length: " << res->get_header_value("Content-Length") << endl;

    // Print content
    int length = stoi(res->get_header_value("Content-Length"));

    for (int i=0; i<length; i++) {
      int c = res->body[i];
      cout << hex << c << endl;
    }
  }

  return 0;
}
