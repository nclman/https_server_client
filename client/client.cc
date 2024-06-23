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

  string filename("test_svr.json");
  if (auto res = cli.Get("/" + filename)) {
    cout << "GET Response: " << res->status << endl;
    cout << "GET Content-Type: " << res->get_header_value("Content-Type") << endl;
    cout << "GET Content-Length: " << res->get_header_value("Content-Length") << endl;

    // Save the file locally
    ofstream file(filename);
    if (file.is_open()) {
      file << res->body;
      file.close();
      cout << "Downloaded file" << endl;
    } else {
      cout << "Could not create file" << endl;
    }
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
      cout << hex << c << endl << dec;
    }
  }

  // Send an image file
  cout << "POST: Send file" << endl;
  string post_fname("cat.jpeg");
  ifstream postfile("content//" + post_fname, ios::binary | ios::ate);

  if (postfile.is_open()) {
    streampos size = postfile.tellg();
    char *buf = new char[size];

    cout << "Size: " << std::to_string(size) << endl;
    postfile.seekg(0, ios::beg);
    postfile.read(buf, size);
    postfile.close();

    if (auto res = cli.Post("/" + post_fname, buf, size, "image/jpeg")) {
      cout << "POST Response: " << res->status << endl;
    } else {
      cout << "error code: " << res.error() << std::endl;
    }
  } else {
    cout << "POST: Failed to send file" << endl;
  }

  return 0;
}
